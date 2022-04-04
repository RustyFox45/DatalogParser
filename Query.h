#ifndef DATALOGPARSER_QUERY_H
#define DATALOGPARSER_QUERY_H

#include <string>
#include "Relation.h"
#include "Scheme.h"
#include "StringUtil.h"
#include "Utils.h"

using namespace std;

class Query {
public:
   string queryString;
   Scheme scheme;
   Relation relation;
   Relation subRelation;

   Query(string line) {
      queryString = fixQueryString(line);
      vector<string> params = Utils::getParameters(line);
      Scheme queryScheme(params);
      scheme = queryScheme;
   }

   string fixQueryString(string stringToFix) {
      string newString = stringToFix;
      newString = StringUtil::leftTrim(newString);
      newString = StringUtil::rightTrim(newString);
      string name = StringUtil::getSubstringToChar(newString, '(');
      StringUtil::leftTrim(name);
      StringUtil::rightTrim(name);
      string balanceOfQueryString;

      string::size_type pos = newString.find('(');
      if (pos != string::npos) {
         balanceOfQueryString = newString.substr(pos);
      } else {
         balanceOfQueryString = newString;
      }

      return name + balanceOfQueryString;
   }
};

#endif //DATALOGPARSER_QUERY_H
