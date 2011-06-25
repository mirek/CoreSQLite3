//
// SQLite3ExtMath.h
// CoreSQLite3
//
// Copyright 2011 Mirek Rusin <mirek [at] me [dot] com>
//

#include "CoreSQLite3.h"

SQLite3Status   SQLite3ExtMathRegisterAllFunctions (SQLite3ConnectionRef connection);

void          __SQLite3ExtMathSin                  (sqlite3_context *context, int argc, sqlite3_value **argv);
SQLite3Status   SQLite3ExtMathRegisterSin          (SQLite3ConnectionRef connection);
SQLite3Status   SQLite3ExtMathUnregisterSin        (SQLite3ConnectionRef connection);

void          __SQLite3ExtMathCos                  (sqlite3_context *context, int argc, sqlite3_value **argv);
SQLite3Status   SQLite3ExtMathRegisterCos          (SQLite3ConnectionRef connection);
SQLite3Status   SQLite3ExtMathUnregisterCos        (SQLite3ConnectionRef connection);
