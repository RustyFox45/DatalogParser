#ifndef DATALOGPARSER_RELATION_H
#define DATALOGPARSER_RELATION_H

#include "Tuple.h"

class Relation {

public:
    string name;
    Scheme scheme;
    set<Tuple> tuples;

public:

    Relation(const string& name, const Scheme& scheme)
            : name(name), scheme(scheme) { }

    void addTuple(const Tuple& tuple) {
        tuples.insert(tuple);
    }

    string getName() {
        return name;
    }

    Relation select(int index, const string& value) const {
        Relation result(name, scheme);
        for (auto& tuple : tuples)
            if (tuple.at(index) == value)
                result.addTuple(tuple);
        return result;
    }

   static bool joinable(const Scheme& leftScheme, const Scheme& rightScheme,
                        const Tuple& leftTuple, const Tuple& rightTuple) {
       bool joinableBool = false;

      for (unsigned leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++) {
         const string& leftName = leftScheme.at(leftIndex);
         const string& leftValue = leftTuple.at(leftIndex);
         cout << "left name: " << leftName << " value: " << leftValue << endl;
         for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {
            const string& rightName = rightScheme.at(rightIndex);
            const string& rightValue = rightTuple.at(rightIndex);
            cout << "right name: " << rightName << " value: " << rightValue << endl;
            if (leftScheme.at(leftIndex) == rightScheme.at(rightIndex)) {
               //check for equal values
               if (leftTuple.at(leftIndex) == rightTuple.at(rightIndex)) {
                  joinableBool = true;
               }
            }
         }
      }

      return joinableBool;
   }

   Relation join(const Relation& r) {
      Relation newRelation(name, scheme);
      const Scheme& leftScheme = scheme;
      const Scheme& rightScheme = r.scheme;
      for (const Tuple& leftTuple : tuples) {
         cout << "left tuple: " << leftTuple.toString(leftScheme) << endl;
         for (const Tuple& rightTuple : r.tuples) {
            cout << "right tuple: " << rightTuple.toString(rightScheme) << endl;
         }
      }




      return newRelation;
   }

    string toString() const {
        stringstream out;
        for (auto& tuple : tuples)
            out << tuple.toString(scheme) << endl;
        return out.str();
    }

};

#endif //DATALOGPARSER_RELATION_H
