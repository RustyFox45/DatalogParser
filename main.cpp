#include <iostream>
#include <fstream>
#include <vector>
#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include "Database.h"
#include "Rule.h"

using namespace std;

enum SectionName {
   Schemes,
   Facts,
   Rules,
   Queries
};

string fixQueryString(string stringToFix);

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

int main(int argc, char *argv[]) {

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
   if (argc == 0) {
      cout << "Please add input file." << endl;
      exit(0);
   }

   string inputFileName = argv[1];
   ifstream in(inputFileName);
   if (in) {

   } else {
      cerr << "File could not be opened: " << inputFileName << endl;
      cerr << "Error Code: " << strerror(errno) << endl;
      return 0;
   }


   Database database;

   SectionName sectionName;

   string line, section;
   while (getline(in, line)) {
      try {

         // Step through input file till I hit "Schemes:"
         if (line.size() == 0 || line.substr(0, 1) == "#") continue;
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
                  database.addScheme(line);
                  break;
               case Facts:
                  database.addFact(line);
                  break;
               case Rules:
                  database.addRule(line);
                  break;
               case Queries:
                  database.addQuery(line);
                  break;
            }
         }
      }
      catch (const std::invalid_argument &e) {

      }
      catch (...) {

      }
   }

   // Print out all the stuff
   cout << database.printRules() << endl;
   cout << database.printQueries() << endl;
   // Clean up memory

   in.close();


//   // Get file names
//   string inputFileName = argv[1];
//
//   ifstream in(inputFileName);
//
//   Relation studentRelation("students", Scheme( {"ID", "Name", "Major"} ));
//
//   vector<string> studentValues[] = {
//         {"'42'", "'Ann'", "'CS'"},
//         {"'64'", "'Ned'", "'EE'"},
//   };
//
//   for (auto& value : studentValues)
//      studentRelation.addTuple(Tuple(value));
//
//   //studentRelation.join(studentRelation);
//
//   Relation courseRelation("courses", Scheme( {"ID", "Course"} ));
//
//   vector<string> courseValues[] = {
//         {"'42'", "'CS 100'"},
//         {"'32'", "'CS 232'"},
//   };
//
//   for (auto& value : courseValues)
//      courseRelation.addTuple(Tuple(value));
//
//   studentRelation.join(courseRelation);
//
//   in.close();

   return 0;
}

