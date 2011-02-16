//
// CoreSQLite3.h
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 07/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#if !defined(__CORE_SQLITE3__)
#define __CORE_SQLITE3__ 1

#import <CoreFoundation/CoreFoundation.h>
#import "sqlite3.h"
#import "dirent.h"

#ifdef __cplusplus
extern "C" {
#endif
  
#import "SQLite3Types.h"
#import "SQLite3Connection.h"
#import "SQLite3ConnectionPool.h"
#import "SQLite3Statement.h"
#import "SQLite3Migration.h"

#ifdef __cplusplus
}
#endif

#endif
