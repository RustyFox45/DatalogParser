//
// Created by Logan Lawson on 3/7/22.
//

#ifndef DATALOGPARSER_SCHEME_H
#define DATALOGPARSER_SCHEME_H

#include <vector>

class Scheme : public vector<string> {

public:
   Scheme() {}

   Scheme(vector<string> names) : vector<string>(names) {}

};

#endif //DATALOGPARSER_SCHEME_H
