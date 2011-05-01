//
// SQLite3ExtLogic.h
// http://github.com/mirek/CoreSQLite3
// Created by Mirek Rusin on 20/04/2011
// Copyright 2011 Mirek Rusin
//
// CoreSQLite3 Logic Extension adds the following functions:
//
// * if(:test, :result_if_non_zero, :result_if_zero)
//

#include "CoreSQLite3.h"

void          __SQLite3ExtLogicFunctionIf           (sqlite3_context *context, int argc, sqlite3_value **argv);
SQLite3Status   SQLite3ExtLogicRegisterFunctionIf   (SQLite3ConnectionRef connection);
SQLite3Status   SQLite3ExtLogicUnregisterFunctionIf (SQLite3ConnectionRef connection);

SQLite3Status   SQLite3ExtLogicRegisterAllFunctions (SQLite3ConnectionRef connection);
