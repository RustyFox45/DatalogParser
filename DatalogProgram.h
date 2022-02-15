//
// Created by Logan Lawson on 2/10/22.
//

#ifndef LAB2_DATALOGPROGRAM_H
#define LAB2_DATALOGPROGRAM_H

#include "Rule.h"
#include "Predicate.h"
#include <set>

class DatalogProgram {
private:

    vector<Predicate> mySchemes;
    vector<Predicate> myFacts;
    vector<Rule> myRules;
    vector<Predicate> myQueries;

public:

    DatalogProgram() {}

    void setSchemes(vector<Predicate> newPred) {
        for (long unsigned int i = 0; i < newPred.size(); ++i) {
            mySchemes.push_back(newPred[i]);
        }
    }
    void setFacts(vector<Predicate> newPred) {
        for (long unsigned int i = 0; i < newPred.size(); ++i) {
            myFacts.push_back(newPred[i]);
        }
    }
    void setRules(vector<Rule> newRule) {
        for (long unsigned int i = 0; i < newRule.size(); ++i) {
            myRules.push_back(newRule[i]);
        }
    }
    void setQueries(vector<Predicate> newPred) {
        for (long unsigned int i = 0; i < newPred.size(); ++i) {
            myQueries.push_back(newPred[i]);
        }
    }

    string toString() const {
        stringstream out;
        stringstream mySchemesStream;
        stringstream myFactsStream;
        stringstream myRulesStream;
        stringstream myQueriesStream;
        stringstream myDomainStream;
        for (long unsigned int i = 0; i < mySchemes.size(); ++i) {
            mySchemesStream << "\n  " << mySchemes[i].toString();
        }
        for (long unsigned int i = 0; i < myFacts.size(); ++i) {
            myFactsStream << "\n  " << myFacts[i].toString();
        }
        for (long unsigned int i = 0; i < myRules.size(); ++i) {
            myRulesStream << "\n  " << myRules[i].toString();
        }
        for (long unsigned int i = 0; i < myQueries.size(); ++i) {
            myQueriesStream << "\n  " << myQueries[i].toString();
        }
        set<string> domainSet;
        for (long unsigned int i = 0; i < myFacts.size(); ++i) {
            vector<Parameter> params = myFacts[i].getParameters();
            for (Parameter p : params) {
                domainSet.insert("  " + p.toString());
            }
        }
        for (string s : domainSet) {
            myDomainStream << s << endl;
        }
        out << "Schemes(" << mySchemes.size() << "):"
            << mySchemesStream.str()
            << "\nFacts(" << myFacts.size() << "):"
            << myFactsStream.str()
            << "\nRules(" << myRules.size() << "):"
            << myRulesStream.str()
            << "\nQueries(" << myQueries.size() << "):"
            << myQueriesStream.str()
            << "\nDomain(" << domainSet.size() << "):\n"
            << myDomainStream.str();
        return out.str();
    }

    friend ostream &operator<<(ostream &os, const DatalogProgram &item) {
        os << item.toString();
        return os;
    }

};

#endif //LAB2_DATALOGPROGRAM_H
