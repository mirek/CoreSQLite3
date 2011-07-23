//
// CoreSQLite3.h
// CoreSQLite3
//
// Copyright 2011 Mirek Rusin <mirek [at] me [dot] com>
//

#if !defined(__CORE_SQLITE3__)
#define __CORE_SQLITE3__ 1

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

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

#include "SQLite3Statement.h"
#include "SQLite3Migration.h"

#include "SQLite3Ext.h"

#ifdef __cplusplus
}
#endif

#endif
