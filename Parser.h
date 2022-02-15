//
// Created by Logan Lawson on 2/2/22.
//

#ifndef LAB2_PARSER_H
#define LAB2_PARSER_H

#include <vector>
#include "Token.h"
#include "DatalogProgram.h"

class Parser {
private:
    vector<Token*> tokens;
    vector<Predicate> currentPredicates;
    vector<Rule> currentRules;

public:
    string errorString = "";
    Parser(const vector<Token*>& tokens) : tokens(tokens) {}
    TokenType tokenType() {
        return tokens.at(0)->getTypeName();
    }
    void advanceToken() {
        tokens.erase(tokens.begin());
    }
    void throwError(Token t) {
        errorString = "Failure!\n  " + t.toString();
    }
    void match(TokenType t) {
        if (tokenType() == t) {
            advanceToken();
        } else {
            throwError(*tokens.at(0));
        }
    }

    /* **********************************
    *************************************
               DataLog Program
    *************************************
    ********************************** */
    DatalogProgram datalogProgram() {
        DatalogProgram myDLProgram;
        match(SCHEMES);
        match(COLON);
        scheme();
        schemeList();
        // Add Schemes Predicates to DLP
        myDLProgram.setSchemes(currentPredicates);
        currentPredicates.clear();
        match(FACTS);
        match(COLON);
        factList();
        // Add Facts Predicates to DLP
        myDLProgram.setFacts(currentPredicates);
        currentPredicates.clear();
        match(RULES);
        match(COLON);
        currentPredicates.clear();
        ruleList();
        // Add Rules to DLP
        myDLProgram.setRules(currentRules);
        currentRules.clear();
        currentPredicates.clear();
        match(QUERIES);
        match(COLON);
        query();
        queryList();
        // Add Queries Predicates to DLP
        myDLProgram.setQueries(currentPredicates);
        currentPredicates.clear();
        match(FILE_END);
        return myDLProgram;
    }
    void addNewPred() {
        Predicate currentPred(tokens.at(0)->getData());
        currentPredicates.push_back(currentPred);
    }
    void addNewRule() {
        Rule currentRule(currentPredicates.front());
        for (int i = 1; i < currentPredicates.size(); ++i) {
            currentRule.addPredicate(currentPredicates.at(i));
        }
        currentRules.push_back(currentRule);
        currentPredicates.clear();
    }
    void addNewParam() {
        Parameter newParam = tokens[0]->getData();
        currentPredicates.back().addParameter(newParam);
    }

    /* **********************************
    *************************************
          Scheme, Fact, Rule, Query
    *************************************
    ********************************** */
    void scheme() {
        if (tokenType() == ID) {
            addNewPred();
            match(ID);
            match(LEFT_PAREN);
            addNewParam();
            match(ID);
            idList();
            match(RIGHT_PAREN);
        } else {
            throwError(*tokens.at(0));
        }
    }
    void fact() {
        if (tokenType() == ID) {
            addNewPred();
            match(ID);
            match(LEFT_PAREN);
            addNewParam();
            match(STRING);
            stringList();
            match(RIGHT_PAREN);
            match(PERIOD);
        } else {
            throwError(*tokens.at(0));
        }
    }
    void rule() {
        if (tokenType() == ID) {
            headPredicate();
            match(COLON_DASH);
            predicate();
            predicateList();
            match(PERIOD);
        } else {
            throwError(*tokens.at(0));
        }
    }
    void query() {
        if (tokenType() == ID) {
            predicate();
            match(Q_MARK);
        } else {
            throwError(*tokens.at(0));
        }
    }

    /* **********************************
    *************************************
       Lists: Scheme, Fact, Rule, Query
    *************************************
    ********************************** */
    void schemeList() {
        if (tokenType() == ID) {
            scheme();
            schemeList();
        } else {
            // lambda
        }
    }
    void factList() {
        if (tokenType() == ID) {
            fact();
            factList();
        } else {
            // lambda
        }
    }
    void ruleList() {
        if (tokenType() == ID) {
            rule();
            addNewRule();
            ruleList();
        } else {
            // lambda
        }
    }
    void queryList() {
        if (tokenType() == ID) {
            query();
            queryList();
        } else {
            // lambda
        }
    }

    /* **********************************
    *************************************
            Predicates & Parameters
    *************************************
    ********************************** */
    void parameter() {
        if (tokenType() == STRING) {
            addNewParam();
            match(STRING);
        } else {
            addNewParam();
            match(ID);
        }
    }
    void idList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            addNewParam();
            match(ID);
            idList();
        } else {
            // lambda
        }
    }
    void stringList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            addNewParam();
            match(STRING);
            stringList();
        } else {
            // lambda
        }
    }
    void parameterList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            parameter();
            parameterList();
        } else {
            // lambda
        }
    }
    void predicateList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            predicate();
            predicateList();
        } else {
            // lambda
        }
    }
    void predicate() {
        if (tokenType() == ID) {
            addNewPred();
            match(ID);
            match(LEFT_PAREN);
            parameter();
            parameterList();
            match(RIGHT_PAREN);
        }
    }
    void headPredicate() {
        if (tokenType() == ID) {
            addNewPred();
            match(ID);
            match(LEFT_PAREN);
            addNewParam();
            match(ID);
            idList();
            match(RIGHT_PAREN);
        }
    }
};

#endif //LAB2_PARSER_H
