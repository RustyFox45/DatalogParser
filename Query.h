#ifndef DATALOGPARSER_QUERY_H
#define DATALOGPARSER_QUERY_H

#include "Tuple.h"

class Query {

public:
   string queryString;
   set<Tuple> queriedTuples;

   Query(string queryString) : queryString(queryString) {}

};

#endif //DATALOGPARSER_QUERY_H
