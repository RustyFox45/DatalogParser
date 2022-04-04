//
// Created by Logan Lawson on 2/10/22.
//

#ifndef LAB2_PREDICATE_H
#define LAB2_PREDICATE_H

#include "Parameter.h"
#include "StringUtil.h"
#include "Utils.h"

class Predicate {
public:
   string predicate;
   string predicateId;
   vector<Parameter> parameters;

public:
   Predicate() {}

   Predicate(string predicateString) {
      parsePredicate(predicateString);
   }

   void addParameter(Parameter newParam) {
      parameters.push_back(newParam);
   }

   vector<Parameter> getParameters() const {
      return parameters;
   }

   void parsePredicate(string predicateString) {
      string temp = predicateString;
      StringUtil::leftTrim(predicateString);
      StringUtil::rightTrim(predicateString);
      predicate = predicateString;
      predicateId = StringUtil::getSubstringToChar(StringUtil::leftTrim(predicateString), '(');
      vector<string> pList = Utils::getParameters(predicateString);
      for (auto pName: pList) {
         Parameter p(pName);
         parameters.push_back(p);
      }
   }

   string toString() const {
      stringstream out;
      out << predicate;
//      stringstream parametersList;
//      for (long unsigned int i = 0; i < parameters.size(); ++i) {
//         if (i > 0) {
//            parametersList << "," << parameters[i].toString();
//         } else {
//            parametersList << parameters[i].toString();
//         }
//      }
//      out << predicateId << "(" << parametersList.str() << ")";
      return out.str();
   }

   friend ostream &operator<<(ostream &os, const Predicate &item) {
      os << item.toString();
      return os;
   }
};

#endif //LAB2_PREDICATE_H
