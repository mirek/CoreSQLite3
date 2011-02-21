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

SQLite3MigrationType SQLite3MigrationGetTypeWithPath(CFStringRef path);
CFStringRef SQLite3MigrationCreateVersionStringWithPath(CFAllocatorRef allocator, CFStringRef path);

#pragma Migration functionality

bool SQLite3MigrationExecute(SQLite3ConnectionRef connection, CFStringRef version, CFStringRef sql);
bool SQLite3MigrationWithDirectoryPath(SQLite3ConnectionRef connection, CFStringRef path);

#endif