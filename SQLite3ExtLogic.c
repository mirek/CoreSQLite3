//
// SQLite3ExtLogic.c
// CoreSQLite3
//
// Copyright 2011 Mirek Rusin <mirek [at] me [dot] com>
//

#include "SQLite3ExtLogic.h"

SQLite3Status SQLite3ExtLogicRegisterAllFunctions(SQLite3ConnectionRef connection) {
  SQLite3Status status = kSQLite3StatusOK;
  if (kSQLite3StatusOK != (status = SQLite3ExtLogicRegisterFunctionIf(connection))) return status;
  return status;
}

void __SQLite3ExtLogicFunctionIf(sqlite3_context *context, int argc, sqlite3_value **argv) {
  if (argc == 3) {
    sqlite3_result_value(context, sqlite3_value_int(argv[0]) ? argv[1] : argv[2]);
  } else {
    sqlite3_result_null(context);
  }
}

SQLite3Status SQLite3ExtLogicRegisterFunctionIf(SQLite3ConnectionRef connection) {
  return SQLite3ConnectionRegisterFunction(connection, CFSTR("if"), 3, __SQLite3ExtLogicFunctionIf);
}

SQLite3Status SQLite3ExtLogicUnregisterFunctionIf(SQLite3ConnectionRef connection) {
  return SQLite3ConnectionUnregisterFunction(connection, CFSTR("if"), 3);
}
