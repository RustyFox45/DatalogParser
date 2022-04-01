#ifndef DATALOGPARSER_UTILS_H
#define DATALOGPARSER_UTILS_H

class Utils {

public:

   static vector<string> getParameters(string line) {
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

   static bool paramIsConstant(string param) {
      return param[0] == '\'';
   }

};

#endif //DATALOGPARSER_UTILS_H
