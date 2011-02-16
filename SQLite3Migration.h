//
// SQLite3Migration.h
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 08/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "CoreSQLite3.h"
#import "dirent.h"

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

BOOL SQLite3MigrationExecute(SQLite3ConnectionRef connection, CFStringRef version, CFStringRef sql);
BOOL SQLite3MigrationWithDirectoryPath(SQLite3ConnectionRef connection, CFStringRef path);
