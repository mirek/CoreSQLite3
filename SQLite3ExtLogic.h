//
// SQLite3ExtLogic.h
// CoreSQLite3
//
// Copyright 2011 Mirek Rusin <mirek [at] me [dot] com>
//

#include "CoreSQLite3.h"

void          __SQLite3ExtLogicFunctionIf           (sqlite3_context *context, int argc, sqlite3_value **argv);
SQLite3Status   SQLite3ExtLogicRegisterFunctionIf   (SQLite3ConnectionRef connection);
SQLite3Status   SQLite3ExtLogicUnregisterFunctionIf (SQLite3ConnectionRef connection);

SQLite3Status   SQLite3ExtLogicRegisterAllFunctions (SQLite3ConnectionRef connection);
