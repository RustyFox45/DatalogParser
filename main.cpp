#include <iostream>
#include <fstream>
#include <vector>
#include "Token.h"
#include "Scanner.h"
#include "Parser.h"

using namespace std;

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
    return returnString;
}

int main(int argc, char* argv[]) {

    // Get file names
    string inputFileName = argv[1];
    string outputFileName = argv[2];

    //
    ifstream in(inputFileName);
    ofstream out(outputFileName);

    string inputString = myInput(in);

    Scanner s = Scanner(inputString);
    Token* t;

    vector<Token*> tokenVector;

    int numTokens = 0;
    do {
        t = s.scanToken();
        if (t!= nullptr) {
            tokenVector.push_back(t);
            if (t->getTypeName() == FILE_END) {
                numTokens++;
                break;
            }
            numTokens++;
        } else {
            break;
        }
    } while (true);

    Parser parser(tokenVector);
    DatalogProgram datalogProgram = parser.datalogProgram();

    out << datalogProgram << endl;

    // Clean up memory
    for(Token* t : tokenVector) {
        delete t;
    }
    in.close();
    out.close();

    cout << "Hi" << endl;

    out << "Total Tokens = " << numTokens;

    return 0;
}