//
// CoreSQLite3.h
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 07/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#if !defined(__CORE_SQLITE3__)
#define __CORE_SQLITE3__ 1

#include <CoreFoundation/CoreFoundation.h>
#include "sqlite3.h"
#include "dirent.h"

#ifdef __cplusplus
extern "C" {
#endif

// Internal
#include "CoreSQLite3UTF8String.h"

#include "SQLite3Types.h"
#include "SQLite3Connection.h"
#include "SQLite3ConnectionPool.h"
#include "SQLite3Statement.h"
#include "SQLite3Migration.h"

#ifdef __cplusplus
}
#endif

#endif
