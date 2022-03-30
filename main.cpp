#include <iostream>
#include <fstream>
#include <vector>
#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include "Database.h"

using namespace std;

enum SectionName {
    Schemes,
    Facts,
    Rules,
    Queries
};

string& leftTrim(string& str, string chars);

string& rightTrim(string& str, string chars);

string fixQueryString(string stringToFix);

string getSubstringToChar(string const &s, char c);

string getSubstringAfterChar(string const &s, char c);

string myInput(ifstream &in) {
    string returnString = "";
    string line = "";
    int i = 0;
    while (getline(in, line)) {
        if (i > 0) {
            returnString.push_back('\n');
        }
        returnString.append(line);
        i++;
    }
    returnString.push_back('\n');
    return returnString;
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

int main(int argc, char* argv[]) {

    // Get file names
//    string inputFileName = argv[1];
//
//    //
//    ifstream in(inputFileName);
//
//    string inputString = myInput(in);
//
//    Scanner s = Scanner(inputString);
//    Token* t;
//
//    vector<Token*> tokenVector;
//
//    int numTokens = 0;
//    do {
//        t = s.scanToken();
//        if (t!= nullptr) {
//            if (t->getTypeName() != COMMENT) {
//                tokenVector.push_back(t);
//            }
//            if (t->getTypeName() == FILE_END) {
//                numTokens++;
//                break;
//            }
//            numTokens++;
//        } else {
//            break;
//        }
//    } while (true);
//
//    Parser parser(tokenVector);
//    DatalogProgram datalogProgram;
//    try {
//        datalogProgram = parser.datalogProgram();
//        cout << "Success!\n" << datalogProgram;
//    }
//    catch (const std::invalid_argument& e) {
//        cout  << parser.errorString << endl;
//    }
//
//    // Clean up memory
//    for(Token* t : tokenVector) {
//        delete t;
//    }
//    in.close();

//    Database database;
//
//    vector<string> names = { "ID", "Name", "Major" };
//
//    Scheme scheme(names);
//
//    Relation relation("student", scheme);
//
//    vector<string> values[] = {
//            {"'42'", "'Ann'", "'CS'"},
//            {"'32'", "'Bob'", "'CS'"},
//            {"'64'", "'Ned'", "'EE'"},
//            {"'16'", "'Jim'", "'EE'"},
//    };
//
//    for (auto& value : values) {
//        Tuple tuple(value);
//        cout << tuple.toString(scheme) << endl;
//        relation.addTuple(tuple);
//    }
//
//    database.addRelation(relation);
//
//    cout << "relation:" << endl;
//    cout << relation.toString();
//
//    Relation result = relation.select(2, "'CS'");
//
//    cout << "select Major='CS' result:" << endl;
//    cout << result.toString();


    // Get file names
    string inputFileName = argv[1];

    //
    ifstream in(inputFileName);

    //string inputString = myInput(in);

    Database database;

    SectionName sectionName;

    string line, section;
    while (getline(in, line)) {
        try {

            // Step through input file till I hit "Schemes:"
            if (line.size() == 0 || line.substr(0,1) == "#") continue;
            if (getKeywords(line) == "Schemes") {
                sectionName = Schemes;
            } else if (getKeywords(line) == "Facts") {
                sectionName = Facts;
            } else if (getKeywords(line) == "Rules") {
                sectionName = Rules;
            } else if (getKeywords(line) == "Queries") {
                sectionName = Queries;
            } else {
                switch (sectionName) {
                    case Schemes:
                        database.addRelation(line);
                        break;
                    case Facts:
                        database.addTupleToRelation(line);
                        break;
                    case Rules:
                        // database.checkAndEvaluateRule(line);
                        break;
                    case Queries:
                        Relation subRelation = database.evaluateQuery(line);
                        vector<string> parameters = database.getParameters(line);
                        line = fixQueryString(line);
                        cout << line;
                        if (subRelation.tuples.size()) {
                           cout << " Yes(" << subRelation.tuples.size() << ")" << endl;
                        } else {
                           cout << " No" << endl;
                        }
                        //for(int i = 0; i < subRelation.tuples.size(); i++) {
                        for(auto tuple : subRelation.tuples) {
                           set<string> printedParams;
                           bool firstPrintedParam = true;
                           for (long unsigned int j = 0; j < subRelation.scheme.size(); ++j) {
                              if (!database.paramIsConstant(parameters[j])) {
                                 if (printedParams.find(parameters[j]) != printedParams.end()) {
                                 }
                                 else {
                                    printedParams.insert(parameters[j]);
                                    if (!firstPrintedParam) {
                                       cout << ", ";
                                    } else {
                                       cout << "  ";
                                       firstPrintedParam = false;
                                    }
                                    cout << parameters[j] << "=" << tuple[j];
                                 }
                              }
                           }
                           if (!firstPrintedParam) {
                              cout << endl;
                           }
                        }
                        // database.projectQueries(tuples);
                        break;
                }
            }
        }
        catch (const std::invalid_argument& e) {

        }
        catch (...) {

        }
    }

    // Print out all the stuff

    // Clean up memory

    in.close();

    return 0;
}

string& leftTrim(string& str, string chars)
{
   str.erase(0, str.find_first_not_of(chars));
   return str;
}

string& rightTrim(string& str, string chars)
{
   str.erase(str.find_last_not_of(chars) + 1);
   return str;
}

string fixQueryString(string stringToFix) {
   string newString = stringToFix;
   newString = leftTrim(newString, " ");
   newString = rightTrim(newString, " ");
   string name = getSubstringToChar(newString, '(');
   leftTrim(name, " ");
   rightTrim(name, " ");
   string balanceOfQueryString = getSubstringAfterChar(newString, '(');
   return name + balanceOfQueryString;
}

string getSubstringToChar(string const &s, char c) {
   string::size_type pos = s.find(c);
   if (pos != string::npos) {
      return s.substr(0, pos);
   } else {
      return s;
   }
}

string getSubstringAfterChar(string const &s, char c) {
   string::size_type pos = s.find(c);
   if (pos != string::npos) {
      return s.substr(pos);
   } else {
      return s;
   }
}