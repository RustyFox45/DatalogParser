#ifndef DATALOGPARSER_RELATION_H
#define DATALOGPARSER_RELATION_H

#include <set>
#include "Tuple.h"
#include "Scheme.h"

class Relation {

public:
   string name;
   Scheme scheme;
   set<Tuple> tuples;

public:
   Relation() {}

   Relation(const string &name, const Scheme &scheme)
         : name(name), scheme(scheme) {}

   void addTuple(const Tuple &tuple) {
      tuples.insert(tuple);
   }

   string getName() {
      return name;
   }

   Relation select(int index, const string &value) const {
      Relation result(name, scheme);
      for (auto &tuple: tuples)
         if (tuple.at(index) == value)
            result.addTuple(tuple);
      return result;
   }

   static bool joinable(const Scheme &leftScheme, const Scheme &rightScheme,
                        const Tuple &leftTuple, const Tuple &rightTuple) {
      bool joinableBool = false;

      for (unsigned leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++) {
         const string &leftName = leftScheme[leftIndex];
         const string &leftValue = leftTuple[leftIndex];
         cout << "left name: " << leftName << " value: " << leftValue << endl;
         for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {
            const string &rightName = rightScheme[rightIndex];
            const string &rightValue = rightTuple[rightIndex];
            cout << "right name: " << rightName << " value: " << rightValue << endl;
            if (leftScheme[leftIndex] == rightScheme[rightIndex]) {
               //check for equal values
               if (leftTuple[leftIndex] == rightTuple[rightIndex]) {
                  joinableBool = true;
               }
            }
         }
      }

      return joinableBool;
   }

   Relation join(const Relation &r) {
      Relation joinedRelation;
      joinedRelation.scheme = joinSchemes(r.scheme);
      cout << "Joining " << name << " with " << r.name << endl;
      for (auto lTuple: tuples) {
         for (auto rTuple: r.tuples) {
            cout << "Left Tuple: " << lTuple.toString(scheme) << "    Right Tuple: " << rTuple.toString(r.scheme) << endl;
            for (int lIndex = 0; lIndex < scheme.size(); lIndex++) {
               for (int rIndex = 0; rIndex < r.scheme.size(); rIndex++) {
                  if (scheme[lIndex] == r.scheme[rIndex]) {
                     // matching columns
                     if (lTuple[lIndex] == rTuple[rIndex]) {
                        // Matching value in tuple, so join the tuples
                        cout << "     Joinable Tuples" << endl;
                        // Tuple joinedTuple;
                        Tuple combinedTuple = joinTuples(scheme, lTuple, r.scheme, rTuple, joinedRelation.scheme);
                        joinedRelation.addTuple(combinedTuple);
                     }
                  }
               }
            }
         }
      }
      return joinedRelation;
   }

   Scheme joinSchemes(const Scheme &rScheme) {
      Scheme newScheme;
      set<string> newSchemeSet(scheme.begin(), scheme.end());
      std::copy(rScheme.begin(), rScheme.end(), std::inserter(newSchemeSet, newSchemeSet.end()));
      newScheme.assign(newSchemeSet.begin(), newSchemeSet.end());
      return newScheme;
   }

   Tuple joinTuples(const Scheme &lScheme, const Tuple &lTuple, const Scheme &rScheme, const Tuple &rTuple, const Scheme &joinedScheme) {
      Tuple newTuple;
      // loop through the joined scheme and get the name

         // look for value in lTuple where column matches joined scheme column
         // look for value in rTuple where column matches joined scheme column

      for(int i = 0; i < joinedScheme.size(); i++) {
         string columnName = joinedScheme[i];
         bool valueFound = false;
         for (int lIndex = 0; lIndex < lScheme.size(); lIndex++) {
            if(lScheme[lIndex] == columnName) {
               newTuple.push_back(lTuple[lIndex]);
               valueFound = true;
            }
         }
         if(!valueFound) {
            for (int rIndex = 0; rIndex < rScheme.size(); rIndex++) {
               if(rScheme[rIndex] == columnName) {
                  newTuple.push_back(rTuple[rIndex]);
               }
            }
         }
      }
      return newTuple;
   }

   Relation projectRelation(const Predicate &predicate) {
      Relation projectedRelation;
      projectedRelation.name = predicate.predicateId;
      for(auto param : predicate.myParameters) {
         projectedRelation.scheme.push_back(param.value);
      }

      // Add column values from current tuples
      for(auto joinedTuple : tuples) {
         Tuple projectedTuple;
         for (unsigned leftIndex = 0; leftIndex < projectedRelation.scheme.size(); leftIndex++) {
            const string &leftName = projectedRelation.scheme[leftIndex];
            for (unsigned rightIndex = 0; rightIndex < scheme.size(); rightIndex++) {
               const string &rightName = scheme[rightIndex];
               if (leftName == rightName) {
                  projectedTuple.push_back(joinedTuple[leftIndex]);
               }
            }
         }
         projectedRelation.tuples.insert(projectedTuple);
      }
      return projectedRelation;
   }

   string toString() const {
      stringstream out;
      for (auto &tuple: tuples)
         out << tuple.toString(scheme) << endl;
      return out.str();
   }

};

#endif //DATALOGPARSER_RELATION_H
