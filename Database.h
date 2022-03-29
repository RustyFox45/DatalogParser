#ifndef DATALOGPARSER_DATABASE_H
#define DATALOGPARSER_DATABASE_H

#include "Relation.h"

class Database {

private:

   vector<Relation> relations;

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
      for (int i = 0; i < relations.size(); ++i) {
         if (relations.at(i).getName() == relationName) {
            relations.at(i).addTuple(myTuple);
            break;
         }
      }
   }

   void checkAndEvaluateRule(string ruleString) {
      string ruleRelationName;
      Tuple ruleTuple();
      // call func that checks if rules evaluate to true and stores tuple values, tossing them back
      // push tuple values into ruleTuple()
      // push tuple into database relations.
   }


   void evaluateQuery(string queryString) {
      // Get the relation of the query.
      string relationName = getSubstringToChar(leftTrim(queryString), '(');
      Relation relation("", vector<string>());
      for (int i = 0; i < relations.size(); ++i) {
         if (relations.at(i).getName() == relationName) {
            relation = relations.at(i);
            break;
         }
      }

      // Gets subset relation based on constants and variables
      Relation subRelation = relation;
      vector<string> queryVariables = getParameters(queryString);
      for (int i = 0; i < relation.scheme.size(); i++) {
         const string param = queryVariables[i];
         if (paramIsConstant(param)) {
            subRelation = subRelation.select(i, param);
         }
      }

      set<Tuple> dupTuples;
      for (int i = 0; i < subRelation.scheme.size(); i++) {
         const string param = queryVariables[i];
         if (!paramIsConstant(param)) {
            // store variable name
            for(int j = i+1; j < subRelation.scheme.size(); j++) {
               const string nextParam = queryVariables[j];
               if (!paramIsConstant(nextParam) && param == nextParam) {
                  Relation duplicateRelation("", vector<string>());
                  // loop through the facts calling select on both columns with the value of the first fact
                  for(auto fact : relation.tuples) {
                     cout << fact << endl;
                     duplicateRelation = subRelation;
                     string firstValue = fact[i];
                     duplicateRelation = duplicateRelation.select(i, firstValue);
                     duplicateRelation = duplicateRelation.select(j, firstValue);
                     dupTuples.insert(duplicateRelation.tuples.begin(), duplicateRelation.tuples.end());
                  }
                  subRelation.tuples = dupTuples;
                  cout << duplicateRelation.toString() << endl;
               }
            }
         }
      }
      cout << subRelation.toString() << endl;
   }

   void parseVariables(string queryString, vector<string> &queryVariables) {

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
