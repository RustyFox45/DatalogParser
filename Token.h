//
// Created by Logan Lawson on 1/12/22.
//
#include <string>
#include <sstream>
using namespace std;

#ifndef LAB1_TOKEN_H
#define LAB1_TOKEN_H

enum TokenType {
    COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY,
    ADD, SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COMMENT, UNDEFINED, FILE_END,
};

class Token {

private:
    TokenType type;
    string value;
    int line;

public:
    Token(TokenType type, string value, int line)
    : type(type), value(value), line(line) {}

    TokenType getTypeName() {
        return type;
    }
    string getData() {
        return value;
    }

    string typeToString(TokenType type) const {
        switch (type) {
            case COMMA: {
                return "COMMA";
            }
            case PERIOD: {
                return "PERIOD";
            }
            case Q_MARK: {
                return "Q_MARK";
            }
            case LEFT_PAREN: {
                return "LEFT_PAREN";
            }
            case RIGHT_PAREN: {
                return "RIGHT_PAREN";
            }
            case COLON: {
                return "COLON";
            }
            case COLON_DASH: {
                return "COLON_DASH";
            }
            case MULTIPLY: {
                return "MULTIPLY";
            }
            case ADD: {
                return "ADD";
            }
            case SCHEMES: {
                return "SCHEMES";
            }
            case FACTS: {
                return "FACTS";
            }
            case RULES: {
                return "RULES";
            }
            case QUERIES: {
                return "QUERIES";
            }
            case ID: {
                return "ID";
            }
            case STRING: {
                return "STRING";
            }
            case COMMENT: {
                return "COMMENT";
            }
            case UNDEFINED: {
                return "UNDEFINED";
            }
            case FILE_END: {
                return "FILE_END";
            }
            default: {
                return "UNDEFINED";
            }
        }
    }

    string toString() const {
        stringstream out;
        out << "(" << typeToString(type) << ",\"" << value << "\"," << line << ")";
        return out.str();
    }

   friend ostream &operator<<(ostream &os, const Token &item) {
      os << item.toString();
      return os;
   }


};

#endif //LAB1_TOKEN_H
