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

    string myInputString = myInput(in);

    Scanner s = Scanner(myInputString);
    Token* t;

    int numTokens = 0;
    do {
        t = s.scanToken();
        if (t!= nullptr) {
            out << t->toString() << endl;
            if (t->getTypeName() == FILE_END) {
                numTokens++;
                break;
            }
            numTokens++;
        } else {
            break;
        }
    } while (true);

    out << "Total Tokens = " << numTokens;

    return 0;
}