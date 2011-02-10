//
// SQLite3Migration.c
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 08/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "SQLite3Migration.h"

inline BOOL SQLite3MigrationDoesTableExist(SQLite3ConnectionRef connection) {
  return NO;
}

// A list of version strings currently in schema_changes table.
// If the migration table doesn't exist, returns NULL.
inline CFArrayRef SQLite3MigrationCreateVersionsArray(SQLite3ConnectionRef connection) {
  CFArrayRef versions = NULL;
  if (SQLite3MigrationDoesTableExist(connection)) {
//    CFStringRef sql = CFStr();
//    versions = SQLite3CreateArrayBySelectingSingleColumn(connection, CFSTR(""));
  }
  return versions;
}

inline void SQLite3MigrationCreateTable(SQLite3ConnectionRef connection) {
//  if (!SQLite3MigrationDoesTableExist(connection)) {
//    //SQLite3Execute(connection, );
//  }
}

//inline void SQLite3MigrationExecute(SQLite3ConnectionRef connection, SQLite3MigrationType type, CFStringRef version, CFStringRef sqlUp, CFStringRef sqlDown) {
//}

//inline void SQLite3MigrationWithDirectoryPath() {
//}
