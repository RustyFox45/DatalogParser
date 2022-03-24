#ifndef DATALOGPARSER_DATABASE_H
#define DATALOGPARSER_DATABASE_H

#include "Relation.h"

class Database {

private:

    vector<Relation> relations;

public:

    Database() {}

    void addRelation(string relationString) {
        // Create Relation from parsing string

        string relationName = getSubstringToChar(leftTrim(relationString), '(');
        Scheme scheme(getSchemeNames(relationString));
        Relation relation(relationName, scheme);
        relations.push_back(relation);
    }

    void addTupleToRelation(string relationName, Tuple newTuple) {
        // Iterate through relation until relationName == relation.name, then push newTuple
    }

    string getSubstringToChar(string const& s, char c) {
        string::size_type pos = s.find(c);
        if (pos != string::npos)
        {
            return s.substr(0, pos);
        }
        else
        {
            return s;
        }
    }

    string& leftTrim(string& str) {
        str.erase(0, str.find_first_not_of(' '));
        return str;
    }

    vector<string> getSchemeNames(string line) {
        vector<string> returnVector;
        string tempString = line;
        string param;
        string::size_type pos = line.find('(');
        if (pos != string::npos) {
            tempString = tempString.substr(pos + 1);
            while (tempString.substr(0,1) != ")") {
                param = "";
                while (tempString.substr(0,1) != "," && tempString.substr(0,1) != ")") {
                    param = param.append(tempString.substr(0,1));
                    tempString = tempString.substr(1);
                }
                returnVector.push_back(param);
                if (tempString.substr(0,1) == ",") {
                    tempString = tempString.substr(1);
                }
            }
        } else {
            throw "error";
        }
        return returnVector;
    }

};

#endif //DATALOGPARSER_DATABASE_H
