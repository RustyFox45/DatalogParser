#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Token.h"
#include "Parser.h"
#include "Predicate.h"

using namespace std;

int main(int argc, char *argv[]) {

   // Make sure we have the necessary input arguments
   if (argc != 3) {
      cout << "Please list input and output files in command line." << endl;
      cout << "For example: lab lab03_in_01.text lab03_out_01.text" << endl;
      return 0;
   }

   // Get file names
   string inputFileName = argv[1];
   string outputFileName = argv[2];

   // Open streams with file names
   ifstream in(inputFileName);
   ofstream out(outputFileName);

   // If streams fail, then explain and exit program
   if (!in) {
      cout << "Unable to open input file: " << inputFileName << endl;
      return 0;
   } else if (!out) {
      in.close();
      cout << "Unable to open output file: " << outputFileName << endl;
      return 0;
   }

   // Open input and output files
   // process input strings
   for (string line; getline(in, line);) {
      // line equal to 1st line of text in file

      string item1, item2, item3;
      string errorString = "Not Found";
      // if the line size is 0, then move to next iteration of loop (next line)
      if (line.size() == 0) continue;
      // Write the 1st line to the output file
      out << line;
      // break line into pieces and iterate
      istringstream iss(line);
      // get 1st item in line (command code)
      iss >> item1;
      // decide what to do with based on the command code
      try {
         if (item1 == "Insert") {
            while (iss >> item2) {
               // Do something with item
            }
            out << endl;
         } else if (item1 == "PrintList") {
            // out << someList << endl;
         } else {
            out << endl;
         }
      } catch (const char *e) {
         out << " " << e << endl;
      } catch (const string *e) {
         out << " " << e << endl;
      } catch (...) {
         out << " " << "Unknown Error!" << endl;
      }
   }

   // CLEAN UP EVERYTHING
   in.close();
   out.close();
   return 0;
}
