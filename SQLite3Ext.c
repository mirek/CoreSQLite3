//
//  SQLite3ExtMath.c
//  SQLite3
//
//  Created by Mirek Rusin on 20/04/2011.
//  Copyright 2011 Inteliv Ltd. All rights reserved.
//

#include "SQLite3ExtMath.h"

void __SQLite3ExtensionsMathSin(sqlite3_context *context, int argc, sqlite3_value **argv) {
  if (argc == 1 && sqlite3_value_type(argv[0]) != SQLITE_NULL) {
    sqlite3_result_double(context, sin(sqlite3_value_double(argv[0])));
  } else {
    sqlite3_result_null(context);
  }
}

SQLite3Status SQLite3ExtensionsMathRegisterSin(SQLite3ConnectionRef connection) {
  return SQLite3ConnectionRegisterFunction(connection, CFSTR("sin"), 1, __SQLite3ExtensionsMathSin);
}

SQLite3Status SQLite3ExtensionsMathUnregisterSin(SQLite3ConnectionRef connection) {
  return SQLite3ConnectionUnregisterFunction(connection, CFSTR("sin"), 1);
}
