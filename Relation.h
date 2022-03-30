#ifndef DATALOGPARSER_RELATION_H
#define DATALOGPARSER_RELATION_H

#include "Tuple.h"
#include "Query.h"

class Relation {

public:
    string name;
    Scheme scheme;
    vector<Query> queries;
    set<Tuple> tuples;

public:

    Relation(const string& name, const Scheme& scheme)
            : name(name), scheme(scheme) { }

    void addTuple(const Tuple& tuple) {
        tuples.insert(tuple);
    }

    string getName() {
        return name;
    }

    int getTuples() {
        return tuples.size();
    }

    Relation select(int index, const string& value) const {
        Relation result(name, scheme);
        for (auto& tuple : tuples)
            if (tuple.at(index) == value)
                result.addTuple(tuple);
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
