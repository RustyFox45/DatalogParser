#ifndef DATALOGPARSER_DATABASE_H
#define DATALOGPARSER_DATABASE_H

#include "Relation.h"
#include "StringUtil.h"
#include "Utils.h"
#include "Rule.h"

class Database {

private:

    vector<Relation> relations;
    vector<Rule> rules;

public:

    Database() {}

    void addRelation(string relationString) {
        // Create Relation from parsing string
        string relationName = getSubstringToChar(leftTrim(relationString), '(');
        Scheme scheme(getParameters(relationString));
        Relation relation(relationName, scheme);
        relations.push_back(relation);
    }

    void addTupleToRelation(string relationString) {
        // Iterate through relation until relationName == relation.name, then push newTuple
        string relationName = getSubstringToChar(leftTrim(relationString), '(');
        Tuple myTuple(getParameters(relationString));
        for (long unsigned int i = 0; i < relations.size(); ++i) {
            if (relations.at(i).getName() == relationName) {
                relations.at(i).addTuple(myTuple);
                break;
            }
        }
    }

   void parseRules(string rulesLine) {
      Predicate headPredicate(StringUtil::getSubstringToChar(rulesLine, ':'));
      Rule newRule(headPredicate);
      vector<Predicate> predicateList;
      // Parse until comma, and populate vector. Loop until Period.
      string temp = StringUtil::getSubstringAfterChar(rulesLine, '-');
      while (true) {
         string predString = getNextPredicateString(temp);
         Predicate newPred(predString);
         newRule.addPredicate(newPred);
         if (temp[0] == '.') {
            break;
         }
         temp =  StringUtil::getSubstringAfterChar(temp, ',');

//          temp = StringUtil::getSubstringAfterChar(temp, ',');
//          if (temp2 == temp) {
//             break;
//          }
//          temp2 = temp;
//          predicateList.push_back(newPred);
      }
      evaluatePredicates(newRule);
      rules.push_back(newRule);
      cout << newRule << endl;
   }

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

   void evaluatePredicates(Rule rule) {
      // Toss Predicates into evaluateQuery function
      vector<Relation> predRelations;
      for (long unsigned int i = 0; i < rule.predicateList.size(); i++) {
         Relation subRelation = evaluateQuery(rule.predicateList[i].toString());
         predRelations.push_back(subRelation);
      }
   }

    Relation evaluateQuery(string queryString) {
        // Get the relation of the query.
        int relationIndex = -1;
        string relationName = getSubstringToChar(leftTrim(queryString), '(');
        relationName = rightTrim(relationName);
        Relation relation("", vector<string>());
        for (long unsigned int i = 0; i < relations.size(); ++i) {
            if (relations.at(i).getName() == relationName) {
                relation = relations.at(i);
                relationIndex = i;
                break;
            }
        }
        // Gets subset relation based on constants and variables
        Relation subRelation = relation;
        vector<string> queryParameters = getParameters(queryString);
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
                        for (auto fact : relation.tuples) {
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

    bool paramIsConstant(string param) {
        return param[0] == '\'';
    }

    string getSubstringToChar(string const &s, char c) {
        string::size_type pos = s.find(c);
        if (pos != string::npos) {
            return s.substr(0, pos);
        } else {
            return s;
        }
    }

    string &leftTrim(string &str) {
        str.erase(0, str.find_first_not_of(' '));
        return str;
    }

    string& rightTrim(string& str) {
        str.erase(str.find_last_not_of(' ') + 1);
        return str;
    }

    vector<string> getParameters(string line) {
        vector<string> returnVector;
        string tempString = line;
        string param;
        string::size_type pos = line.find('(');
        if (pos != string::npos) {
            tempString = tempString.substr(pos + 1);
            while (tempString.substr(0, 1) != ")") {
                param = "";
                while (tempString.substr(0, 1) != "," && tempString.substr(0, 1) != ")") {
                    param = param.append(tempString.substr(0, 1));
                    tempString = tempString.substr(1);
                }
                returnVector.push_back(param);
                if (tempString.substr(0, 1) == ",") {
                    tempString = tempString.substr(1);
                }
            }
        } else {
            throw "error";
        }
        return returnVector;
    }

};

#endif //DATALOGPARSER_DATABASE_H
