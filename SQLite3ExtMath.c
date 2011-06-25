//
// SQLite3ExtMath.c
// CoreSQLite3
//
// Copyright 2011 Mirek Rusin <mirek [at] me [dot] com>
//

#include "SQLite3ExtMath.h"

void __SQLite3ExtMathSin(sqlite3_context *context, int argc, sqlite3_value **argv) {
  if (argc == 1 && sqlite3_value_type(argv[0]) != SQLITE_NULL) {
    sqlite3_result_double(context, sin(sqlite3_value_double(argv[0])));
  } else {
    sqlite3_result_null(context);
  }
}

SQLite3Status SQLite3ExtMathRegisterSin(SQLite3ConnectionRef connection) {
  return SQLite3ConnectionRegisterFunction(connection, CFSTR("sin"), 1, __SQLite3ExtMathSin);
}

SQLite3Status SQLite3ExtMathUnregisterSin(SQLite3ConnectionRef connection) {
  return SQLite3ConnectionUnregisterFunction(connection, CFSTR("sin"), 1);
}

void __SQLite3ExtMathCos(sqlite3_context *context, int argc, sqlite3_value **argv) {
  if (argc == 1 && sqlite3_value_type(argv[0]) != SQLITE_NULL) {
    sqlite3_result_double(context, cos(sqlite3_value_double(argv[0])));
  } else {
    sqlite3_result_null(context);
  }
}

SQLite3Status SQLite3ExtMathRegisterCos(SQLite3ConnectionRef connection) {
  return SQLite3ConnectionRegisterFunction(connection, CFSTR("cos"), 1, __SQLite3ExtMathSin);
}

SQLite3Status SQLite3ExtMathUnregisterCos(SQLite3ConnectionRef connection) {
  return SQLite3ConnectionUnregisterFunction(connection, CFSTR("cos"), 1);
}
