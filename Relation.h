#ifndef DATALOGPARSER_RELATION_H
#define DATALOGPARSER_RELATION_H

#include "Tuple.h"

class Relation {

private:

    string name;
    Scheme scheme;
    set<Tuple> tuples;

public:

    Relation(const string& name, const Scheme& scheme)
            : name(name), scheme(scheme) { }

    void addTuple(const Tuple& tuple) {
        tuples.insert(tuple);
    }

    Relation select(int index, const string& value) const {
        Relation result(name, scheme);
        for (auto& tuple : tuples)
            if (tuple.at(index) == value)
                result.addTuple(tuple);
        return result;
    }

    Relation project(Scheme newScheme) const {
        Relation result(name, newScheme);
        // set result to relation of schema from maybe a vector of strings?
        return result;
    }

    Relation rename(int index, const string& value) const {
        Relation result(name, scheme);
        // set Schemes 'names' array at index to new value
        return result;
    }

    string toString() const {
        stringstream out;
        for (auto& tuple : tuples)
            out << tuple.toString(scheme) << endl;
        return out.str();
    }

};

#endif //DATALOGPARSER_RELATION_H
