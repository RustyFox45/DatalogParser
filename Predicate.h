#ifndef LAB2_PREDICATE_H
#define LAB2_PREDICATE_H
#include "Parameter.h"
#include "StringUtil.h"
#include "Utils.h"


class Predicate {
private:
    string predicateId;
    vector<Parameter> myParameters;

public:

    Predicate(string predicateString) {
       parsePredicate(predicateString);
    }

    void addParameter(Parameter newParam) {
        myParameters.push_back(newParam);
    }

    vector<Parameter> getParameters() const {
        return myParameters;
    }

    void parsePredicate(string predicateString) {
       string temp = predicateString;
       StringUtil::leftTrim(predicateString);
       StringUtil::rightTrim(predicateString);

       predicateId = StringUtil::getSubstringToChar(StringUtil::leftTrim(predicateString), '(');
       vector<string> pList = Utils::getParameters(predicateString);
       for (auto pName: pList) {
          Parameter p(pName);
          myParameters.push_back(p);
       }
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
