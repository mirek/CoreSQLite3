//
//  SQLite3ExtMath.h
//  SQLite3
//
//  Created by Mirek Rusin on 20/04/2011.
//  Copyright 2011 Inteliv Ltd. All rights reserved.
//

#include "CoreSQLite3.h"

void          __SQLite3ExtensionsMathSin           (sqlite3_context *context, int argc, sqlite3_value **argv);
SQLite3Status   SQLite3ExtensionsMathRegisterSin   (SQLite3ConnectionRef connection);
SQLite3Status   SQLite3ExtensionsMathUnregisterSin (SQLite3ConnectionRef connection);

void          __SQLite3ExtensionsMathCos           (sqlite3_context *context, int argc, sqlite3_value **argv);
SQLite3Status   SQLite3ExtensionsMathRegisterCos   (SQLite3ConnectionRef connection);
SQLite3Status   SQLite3ExtensionsMathUnregisterCos (SQLite3ConnectionRef connection);

//void          __SQLite3ExtensionsMathCos           (sqlite3_context *context, int argc, sqlite3_value **argv);
//SQLite3Status   SQLite3ExtensionsMathRegisterCos   (SQLite3ConnectionRef connection);
//SQLite3Status   SQLite3ExtensionsMathUnregisterCos (SQLite3ConnectionRef connection);
