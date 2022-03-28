#ifndef DATALOGPARSER_TUPLE_H
#define DATALOGPARSER_TUPLE_H

#include "Scheme.h"

class Tuple : public vector<string> {

public:

    Tuple(vector<string> values) : vector<string>(values) { }

    friend ostream &operator<<(ostream &os, const Tuple &item) {

        return os;
    }

    string toString(const Scheme& scheme) const {
        stringstream out;
        for (unsigned i = 0; i < size(); i++) {
            if (i > 0)
                out << ", ";
            const string& name = scheme.at(i);
            const string& value = at(i);
            out << name << "=" << value;
        }
        return out.str();
    }

};

#endif //DATALOGPARSER_TUPLE_H
