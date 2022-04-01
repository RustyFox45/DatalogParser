#ifndef DATALOGPARSER_STRINGUTIL_H
#define DATALOGPARSER_STRINGUTIL_H

class StringUtil {

public:

   static string &leftTrim(string &str) {
      str.erase(0, str.find_first_not_of(' '));
      return str;
   }

   static string &rightTrim(string &str) {
      str.erase(str.find_last_not_of(' ') + 1);
      return str;
   }

   static string getSubstringToChar(string const &s, char c) {
      string::size_type pos = s.find(c);
      if (pos != string::npos) {
         return s.substr(0, pos);
      } else {
         return s;
      }
   }

   static string getSubstringAfterChar(string const &s, char c) {
      string::size_type pos = s.find(c);
      if (pos != string::npos) {
         return s.substr(pos + 1);
      } else {
         return s;
      }
   }


};

#endif //DATALOGPARSER_STRINGUTIL_H