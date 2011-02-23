//
// SQLite3Migration.h
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 08/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#if !defined(__CORE_SQLITE3_MIGRATION__)
#define __CORE_SQLITE3_MIGRATION__ 1

#include "CoreSQLite3.h"
#include "dirent.h"

#define SQLite3MigrationTableName      "schema_changes"
#define SQLite3MigrationCreateTableSQL "create table schema_changes(version varchar)"

typedef enum {
  kSQLite3MigrationTypeUnknown,
  kSQLite3MigrationTypeMigration,
  kSQLite3MigrationTypeUndoMigration
} SQLite3MigrationType;

#pragma Migration utility functions

SQLite3MigrationType SQLite3MigrationGetTypeWithPath                 (                          CFStringRef path);
SQLite3MigrationType SQLite3MigrationGetTypeWithURL                  (                          CFURLRef    url);
CFStringRef          SQLite3MigrationCreateVersionStringWithPath     (CFAllocatorRef allocator, CFStringRef path);
CFStringRef          SQLite3MigrationCreateVersionStringWithURL      (CFAllocatorRef allocator, CFURLRef url);
CFMutableArrayRef    SQlite3MigrationCreateURLsArrayWithDirectoryURL (CFAllocatorRef allocator, CFURLRef directoryURL);

SQLite3Status SQLite3MigrationCreateTableIfDoesntExist (SQLite3ConnectionRef connection);

SQLite3Status SQLite3MigrationInsertVersion (SQLite3ConnectionRef connection, CFStringRef version);
SQLite3Status SQLite3MigrationDeleteVersion (SQLite3ConnectionRef connection, CFStringRef version);

#pragma Migration functionality

SQLite3Status SQLite3MigrationExecuteWithContentsOfURL (SQLite3ConnectionRef connection, CFURLRef url);
SQLite3Status SQLite3MigrationExecute                  (SQLite3ConnectionRef connection, CFStringRef version, CFStringRef sql);
SQLite3Status SQLite3MigrationExecuteUndo              (SQLite3ConnectionRef connection, CFStringRef version, CFStringRef sql);

SQLite3Status SQLite3MigrationMigrateWithDirectoryURL (SQLite3ConnectionRef connection, CFURLRef directoryURL);

#endif
