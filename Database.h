#ifndef DATALOGPARSER_DATABASE_H
#define DATALOGPARSER_DATABASE_H

#include "Relation.h"
#include "StringUtil.h"
#include "Utils.h"
#include "Rule.h"
#include "Query.h"

class Database {

public:
   //vector<Relation> relations;
   vector<Relation> schemes;
   vector<Query> queries;
   vector<Rule> rules;

public:

   // ***********************************************************************************
   Database() {}

   // ***********************************************************************************
   void addScheme(string relationString) {
      // Create Relation from parsing string
      string relationName = StringUtil::getSubstringToChar(StringUtil::leftTrim(relationString), '(');
      Scheme scheme(Utils::getParameters(relationString));
      Relation relation(relationName, scheme);
      schemes.push_back(relation);
   }

   // ***********************************************************************************
   void addFact(string relationString) {
      // Iterate through relation until relationName == relation.name, then push newTuple
      string relationName = StringUtil::getSubstringToChar(StringUtil::leftTrim(relationString), '(');
      Tuple myTuple(Utils::getParameters(relationString));
      for (long unsigned int i = 0; i < schemes.size(); ++i) {
         if (schemes.at(i).getName() == relationName) {
            schemes.at(i).addTuple(myTuple);
            break;
         }
      }
   }

   // ***********************************************************************************
   void addRule(string line) {
      string temp = line;
      StringUtil::leftTrim(temp);
      Predicate headPredicate(StringUtil::getSubstringToChar(temp, ':'));
      Rule newRule(headPredicate);
      newRule.ruleString = temp;
      vector<Predicate> predicateList;
      for(auto relation : schemes) {
         if(relation.name == newRule.headPredicate.predicateId) {
            newRule.originalScheme = relation.scheme;
         }
      }
      // Parse until comma, and populate vector. Loop until Period.
      temp = StringUtil::getSubstringAfterChar(line, '-');
      while (true) {
         string predString = getNextPredicateString(temp);
         Predicate newPred(predString);
         newRule.addPredicate(newPred);
         if (temp[0] == '.') {
            break;
         }
         temp = StringUtil::getSubstringAfterChar(temp, ',');
      }
      rules.push_back(newRule);
   }

   // ***********************************************************************************
   void addQuery(string line) {
      Query query(line);
      queries.push_back(query);
   }

   // ***********************************************************************************
   set<Tuple> evaluateRule(Rule &rule) {
      set<Tuple> tuplesAdded;
      vector<Relation> ruleRelations;
      for (long unsigned int i = 0; i < rule.predicateList.size(); i++) {
         Relation ruleRelation = evaluateQuery(rule.predicateList[i].predicate);
         ruleRelations.push_back(ruleRelation);
      }
      Relation joinedRelation;
      if (ruleRelations.size() > 1) {
         // Do merge
         for (int i = 1; i < ruleRelations.size(); i++) {
            Relation lRelation = ruleRelations[i - 1];
            Relation rRelation = ruleRelations[i];
            joinedRelation = lRelation.join(rRelation);
         }
      } else {
         // No merge required because we only have 1 relation
         joinedRelation = ruleRelations[0];
      }

      // We now have a joined relation, so we need to create a new relation based on the rule head
      Relation projectedRelation = joinedRelation.projectRelation(rule.headPredicate);
      rule.relation = projectedRelation;

      // Add tuples to matching scheme
      for (int i = 0; i < schemes.size(); i++) {
         if (schemes[i].name == rule.headPredicate.predicateId) {
            for (auto tuple: rule.relation.tuples) {
               bool added = schemes[i].addTuple(tuple);
               if(added) {
                  tuplesAdded.insert(tuple);
               }
            }
         }
      }
      return tuplesAdded;
   }

   // ***********************************************************************************
   string getNextPredicateString(string &line) {
      StringUtil::leftTrim(line);
      string temp = line;
      string::size_type pos = temp.find(')');
      if (pos != string::npos) {
         string predString = temp.substr(0, pos + 1);
         line = temp.substr(pos + 1);
         return predString;
      } else {
         return "";
      }
   }

   // ***********************************************************************************
   Relation evaluateQuery(string queryString) {
      // Get the relation of the query.
      int relationIndex = -1;
      string relationName = StringUtil::getSubstringToChar(StringUtil::leftTrim(queryString), '(');
      relationName = StringUtil::rightTrim(relationName);
      Relation relation("", vector<string>());
      for (long unsigned int i = 0; i < schemes.size(); ++i) {
         if (schemes[i].getName() == relationName) {
            relation = schemes[i];
            relationIndex = i;
            break;
         }
      }

      // Break down query string and get renamed scheme
      // set subRelation scheme to the new scheme
      Scheme newScheme(Utils::getParameters(queryString));

      // Gets subset relation based on constants and variables
      Relation subRelation = relation;
      subRelation.scheme = newScheme;
      vector<string> queryParameters = Utils::getParameters(queryString);
      for (long unsigned int i = 0; i < relation.scheme.size(); i++) {
         const string param = queryParameters[i];
         if (paramIsConstant(param)) {
            subRelation = subRelation.select(i, param);
         }
      }

      for (long unsigned int i = 0; i < subRelation.scheme.size(); i++) {
         const string param = queryParameters[i];
         if (!paramIsConstant(param)) {
            // store variable name
            for (long unsigned int j = i + 1; j < subRelation.scheme.size(); j++) {
               const string nextParam = queryParameters[j];
               if (!paramIsConstant(nextParam) && param == nextParam) {
                  set<Tuple> dupTuples;
                  Relation duplicateRelation("", vector<string>());
                  // loop through the facts calling select on both columns with the value of the first fact
                  for (auto fact: relation.tuples) {
                     duplicateRelation = subRelation;
                     string firstValue = fact[i];
                     duplicateRelation = duplicateRelation.select(i, firstValue);
                     duplicateRelation = duplicateRelation.select(j, firstValue);
                     dupTuples.insert(duplicateRelation.tuples.begin(), duplicateRelation.tuples.end());
                  }
                  subRelation.tuples = dupTuples;
                  dupTuples.clear();
               }
            }
         }
      }
      return subRelation;
   }

   // ***********************************************************************************
   bool paramIsConstant(string param) {
      return param[0] == '\'';
   }

   // ***********************************************************************************
   string printRules() {
      ostringstream oss;
      bool tuplesAdded = true;
      int passes = 0;
      oss << "Rule Evaluation" << endl;
      while(tuplesAdded) {
         passes++;
         oss << "PASS: " << passes << endl;
         int tuplesAddedThisPass = 0;
         for (unsigned int x = 0; x < rules.size(); x++) {
            set<Tuple> tuplesAdded = evaluateRule(rules[x]);
            tuplesAddedThisPass += tuplesAdded.size();
            oss << rules[x].ruleString << endl;
            if(tuplesAdded.size() > 0) {
               //for (auto tuple: rules[x].relation.tuples) {
               for (auto tuple: tuplesAdded) {
                  set<string> printedParams;
                  oss << "  ";
                  for (long unsigned int i = 0; i < rules[x].relation.scheme.size(); ++i) {
                     if (i != 0) {
                        oss << ", ";
                     }
                     oss << rules[x].originalScheme[i] << "=" << tuple[i];
                  }
                  oss << endl;
               }
            }
            rules[x].relation.tuples.clear();
         }
         tuplesAdded = tuplesAddedThisPass > 0;
      }

      oss << endl << "Schemes populated after " << passes << " passes through the Rules." << endl;

      return oss.str();
   }

   // evaluate rule and print out new tuples
   //

   // ***********************************************************************************
   string printQueries() {
      ostringstream oss;
      oss << "Query Evaluation" << endl;
      for(auto query : queries) {
         Relation subRelation = evaluateQuery(query.queryString);
         Scheme scheme = subRelation.scheme;
         oss << query.queryString;
         if (subRelation.tuples.size()) {
            oss << " Yes(" << subRelation.tuples.size() << ")" << endl;
         } else {
            oss << " No" << endl;
         }
         for (auto tuple: subRelation.tuples) {
            set<string> printedParams;
            bool firstPrintedParam = true;
            for (long unsigned int j = 0; j < subRelation.scheme.size(); ++j) {
               if (!Utils::paramIsConstant(scheme[j])) {
                  if (printedParams.find(scheme[j]) != printedParams.end()) {
                  } else {
                     printedParams.insert(scheme[j]);
                     if (!firstPrintedParam) {
                        oss << ", ";
                     } else {
                        oss << "  ";
                        firstPrintedParam = false;
                     }
                     oss << scheme[j] << "=" << tuple[j];
                  }
               }
            }
            if (!firstPrintedParam) {
               oss << endl;
            }
         }
      }
      return oss.str();
   }
};

#endif //DATALOGPARSER_DATABASE_H
