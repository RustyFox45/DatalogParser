//
// Created by Logan Lawson on 2/10/22.
//

#ifndef LAB2_RULE_H
#define LAB2_RULE_H

#include "Predicate.h"

class Rule {
public:

    Predicate headPredicate;
    vector<Predicate> predicateList;

public:

    Rule(Predicate headPredicate) : headPredicate(headPredicate) {}

    void addPredicate(Predicate newPred) {
        predicateList.push_back(newPred);
    }

    string toString() const {
        stringstream out;
        stringstream myPredList;
        for (long unsigned int i = 0; i < predicateList.size(); ++i) {
            if (i > 0) {
                myPredList << "," << predicateList[i].toString();
            } else {
                myPredList << predicateList[i].toString();
            }
        }
        out << headPredicate.toString() << " :- " << myPredList.str();
        return out.str();
    }

   friend ostream &operator<<(ostream &os, const Rule &item) {
      os << item.toString();
      return os;
   }

};

#endif //LAB2_RULE_H
