//
//  SQLite3Migration.c
//  SQLite3
//
//  Created by Mirek Rusin on 08/02/2011.
//  Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "SQLite3Migration.h"

#define SQLite3MigrationTableName      "schema_changes"
#define SQLite3MigrationCreateTableSQL "create table schema_changes(version varchar)"

inline BOOL SQLite3MigrationDoesTableExist(SQLite3ConnectionRef connection) {
  return NO;
}

inline void SQLite3MigrationCreateTable(SQLite3ConnectionRef connection) {
  if (!SQLite3MigrationDoesTableExist(connection)) {
    //SQLite3Execute(connection, );
  }
}

inline void SQLite3MigrationWithDirectoryPath() {
}

// Migrate database schema with dictionary.
// Keys should be timestamps and values sql strings.
//
// Return keys (timestamp versions) of executed queries.
//
inline void SQLite3MigrationWithDictionary() {
}
