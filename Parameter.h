//
// Created by Logan Lawson on 2/10/22.
//

#ifndef LAB2_PARAMETER_H
#define LAB2_PARAMETER_H

#include <sstream>
#include <string>

class Parameter {
private:
    string value;

public:

    Parameter(string value) : value(value) {}

    string toString() const {
        stringstream out;
        out << value;
        return out.str();
    }

   friend ostream &operator<<(ostream &os, const Parameter &item) {
      os << item.toString();
      return os;
   }

};

#endif //LAB2_PARAMETER_H
