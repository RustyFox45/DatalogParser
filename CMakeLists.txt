cmake_minimum_required(VERSION 3.17)
project(DatalogParser)

set(CMAKE_CXX_STANDARD 17)

add_executable(DatalogParser main.cpp Parameter.h Parser.h Predicate.h Rule.h Scanner.h Token.h Scheme.h Tuple.h Relation.h Database.h StringUtil.h Utils.h Query.h)
