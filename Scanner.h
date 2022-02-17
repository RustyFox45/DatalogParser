//
// Created by Logan Lawson on 2/10/22.
//
#include <string>
#include <cctype>
#include "Token.h"
using namespace std;

#ifndef LAB1_SCANNER_H
#define LAB1_SCANNER_H

string trim(string str);

class Scanner {
private:
    string input;

public:
    int lineNumber = 1;
    Scanner(const string& input) : input(input) {}

    Token* scanToken() {
        TokenType type;
        int mySize = 0;
        char myChar = 'a';
        string value = "";

        while (!input.empty() && isspace(input.at(0))) {
            if (input.at(0) == '\n') {
                incLine();
            }
            input = input.substr(1);
        }

        if (input.size() == 0) {
            type = FILE_END;
            mySize = 0;
        } else {
            myChar = input.at(0);

            switch (myChar) {
                case ',': {
                    type = COMMA;
                    mySize = 1;
                    break;
                }
                case '.': {
                    type = PERIOD;
                    mySize = 1;
                    break;
                }
                case '?': {
                    type = Q_MARK;
                    mySize = 1;
                    break;
                }
                case '(': {
                    type = LEFT_PAREN;
                    mySize = 1;
                    break;
                }
                case ')': {
                    type = RIGHT_PAREN;
                    mySize = 1;
                    break;
                }
                case ':': {

                    if (input.at(1) == '-') {
                        type = COLON_DASH;
                        mySize = 2;
                        break;
                    } else {
                        type = COLON;
                        mySize = 1;
                        break;
                    }



//                    if (!isspace(input.at(1))) {
//                        if (input[1] == '-') {
//                            type = COLON_DASH;
//                            mySize = 2;
//                            break;
//                        } else {
//                            type = UNDEFINED;
//                            int i = 1;
//                            while (!isspace(input[i])) {
//                                i++;
//                            }
//                            mySize = i;
//                            break;
//                        }
//                    }
//                    type = COLON;
//                    mySize = 1;
//                    break;
                }
                case '*': {
                    type = MULTIPLY;
                    mySize = 1;
                    break;
                }
                case '+': {
                    type = ADD;
                    mySize = 1;
                    break;
                }
                case 'S': {
                    string temp = getKeywords(input);
                    if (!temp.empty()) {
                        type = SCHEMES;
                        mySize = temp.length();
                    } else {
                        // Is probably ID
                        temp = getVariableName(input);
                        if (!temp.empty()) {
                            type = ID;
                            mySize = temp.length();
                        } else {
                            type = UNDEFINED;
                            mySize = 1;
                        }
                    }
                    break;
                }
                case 'F': {
                    string temp = getKeywords(input);
                    if (!temp.empty()) {
                        type = FACTS;
                        mySize = temp.length();
                    } else {
                        // Is probably ID
                        temp = getVariableName(input);
                        if (!temp.empty()) {
                            type = ID;
                            mySize = temp.length();
                        } else {
                            type = UNDEFINED;
                            mySize = 1;
                        }
                    }
                    break;
                }
                case 'R': {
                    string temp = getKeywords(input);
                    if (!temp.empty()) {
                        type = RULES;
                        mySize = temp.length();
                    } else {
                        // Is probably ID
                        temp = getVariableName(input);
                        if (!temp.empty()) {
                            type = ID;
                            mySize = temp.length();
                        } else {
                            type = UNDEFINED;
                            mySize = 1;
                        }
                    }
                    break;
                }
                case 'Q': {
                    string temp = getKeywords(input);
                    if (!temp.empty()) {
                        type = QUERIES;
                        mySize = temp.length();
                    } else {
                        // Is probably ID
                        temp = getVariableName(input);
                        if (!temp.empty()) {
                            type = ID;
                            mySize = temp.length();
                        } else {
                            type = UNDEFINED;
                            mySize = 1;
                        }
                    }
                    break;
                }
                case '\'': {
                    type = STRING;
                    // Call String Function to grab value
                    string myStringToken;
                    bool setUndefined = false;
                    myStringToken = stringToken(input, setUndefined);
                    mySize = myStringToken.length();
                    if (setUndefined) {
                        type = UNDEFINED;
                    }
                    break;
                }
                case '#': {
                    type = COMMENT;
                    // Call Comment Function to grab value
                    string myCommentToken;
                    bool setUndefined = false;
                    myCommentToken = commentToken(input, setUndefined);
                    mySize = myCommentToken.size();
                    if (setUndefined) {
                        type = UNDEFINED;
                    }
                    break;
                }
                default: {
                    string temp = getVariableName(input);
                    if (!temp.empty()) {
                        type = ID;
                        mySize = temp.length();
                    } else {
                        type = UNDEFINED;
                        mySize = 1;
                    }
                    // Set when string/comment isn't terminated or operator is invalid
                    break;
                }
            }
            value = input.substr(0, mySize);
            input = input.substr(mySize);
            //input = trim(input);
        }
        return new Token(type, value, lineNumber);
    }

    string getKeywords(string keywordInput) {
        // QUERIES, SCHEMES, FACTS, RULES
        string result = "";
        if (keywordInput.length() >= 7 && keywordInput.substr(0, 7) == "Queries") {
            result = "Queries";
        } else if (keywordInput.length() >= 7 && keywordInput.substr(0, 7) == "Schemes") {
            result = "Schemes";
        } else if (keywordInput.length() >= 5 && keywordInput.substr(0, 5) == "Facts") {
            result = "Facts";
        } else if (keywordInput.length() >= 5 && keywordInput.substr(0, 5) == "Rules") {
            result = "Rules";
        }
        return result;
    }

    string commentToken(string myString, bool& setUndefined) {
        string returnString = "";
        char currChar = myString.at(0);
        for (int i = 1; currChar != '\n'; i++) {
            returnString.push_back(currChar);
            if (myString.empty()) {
                setUndefined = true;
                break;
            }
            currChar = myString.at(i);
        }
        return returnString;
    }

    string stringToken(string myString, bool& setUndefined) {
        string returnString = "\'";
        char currChar = 'a';
        for (int i = 1; currChar != '\''; i++) {
            currChar = myString.at(i);
            returnString.push_back(currChar);
            if (myString.empty()) {
                setUndefined = true;
                break;
            }
        }
        return returnString;
    }

    void incLine() {
        lineNumber++;
    }

    string getVariableName(string variableInput) {
        // QUERIES, SCHEMES, FACTS, RULES
        string results;
        if (isalpha(variableInput.at(0))) {
            int i = 0;
            while (isalnum(variableInput[i])) i++;
            results = variableInput.substr(0, i);
        }
        return results;
    }

};

#endif //LAB1_SCANNER_H
