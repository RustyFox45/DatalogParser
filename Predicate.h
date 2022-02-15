//
// Created by Logan Lawson on 2/10/22.
//

#ifndef LAB2_PREDICATE_H
#define LAB2_PREDICATE_H
#include "Parameter.h"


class Predicate {
private:
    string predicateId;
    vector<Parameter> myParameters;

public:

    Predicate(string predicateId) : predicateId(predicateId) {}

    void addParameter(Parameter newParam) {
        myParameters.push_back(newParam);
    }
    vector<Parameter> getParameters() const {
        return myParameters;
    }

    string toString() const {
        stringstream out;
        stringstream parametersList;
        for (long unsigned int i = 0; i < myParameters.size(); ++i) {
            if (i > 0) {
                parametersList << "," << myParameters[i].toString();
            } else {
                parametersList << myParameters[i].toString();
            }
        }
        out << predicateId << "(" << parametersList.str() << ")";
        return out.str();
    }

   friend ostream &operator<<(ostream &os, const Predicate &item) {
      os << item.toString();
      return os;
   }
};

#endif //LAB2_PREDICATE_H
