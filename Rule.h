#ifndef LAB2_RULE_H
#define LAB2_RULE_H

#include "Predicate.h"
#include "Relation.h"

class Rule {
public:

   Predicate headPredicate;
   vector<Predicate> predicateList;
   Relation relation;

public:
   Rule() {}

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
