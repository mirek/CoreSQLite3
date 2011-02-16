//
// SQLite3Migration.c
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 08/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "SQLite3Migration.h"

#define SQLite3MigrationMaxCount 4096

inline BOOL SQLite3MigrationDoesTableExist(SQLite3ConnectionRef connection) {
  return SQLite3ConnectionDoesTableExistWithName(connection, CFSTR("schema_migrations"));
}

inline void SQLite3MigrationCreateTable(SQLite3ConnectionRef connection) {
  if (!SQLite3MigrationDoesTableExist(connection)) {
    SQLite3ConnectionExecute(connection, CFSTR("create table schema_migrations(version varchar)"));
  }
}

// A list of version strings currently in schema_changes table.
// If the migration table doesn't exist, returns NULL.
inline CFArrayRef SQLite3MigrationCreateVersionsArray(SQLite3ConnectionRef connection) {
  CFMutableArrayRef versions = NULL;
  if (SQLite3MigrationDoesTableExist(connection)) {
    versions = CFArrayCreateMutable(connection->allocator, SQLite3MigrationMaxCount, &kCFTypeArrayCallBacks);
    SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("select version from schema_migrations"));
    while (SQLite3StatementStep(statement) == SQLITE_ROW) {
      CFStringRef version = SQLite3StatementCreateStringWithColumn(statement, 0);
      CFArrayAppendValue(versions, version);
      CFRelease(version);
    }
  }
  return versions;
}

// Check if the migration with provided version has been already performed.
inline BOOL SQLite3MigrationDidMigratedVersion(SQLite3ConnectionRef connection, CFStringRef version) {
  BOOL didMigrated = NO;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("select exists(select * from schema_migrations where version = ?)"));
  SQLite3StatementBindString(statement, 1, version);
  while (SQLite3StatementStep(statement) == SQLITE_ROW) {
    didMigrated = SQLite3StatementGetBOOLWithColumn(statement, 0);
    break;
  }
  return didMigrated;
}

// Return true on success, false otherwise.
inline BOOL SQLite3MigrationExecute(SQLite3ConnectionRef connection, CFStringRef version, CFStringRef sql) {
  SQLite3ConnectionExecute(connection, sql);
  if (!SQLite3ConnectionHasError(connection)) {
    SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("insert into schema_migrations(version) values(?)"));
    SQLite3StatementBindString(statement, 1, version);
    SQLite3StatementExecute(statement);
  }
  return !SQLite3ConnectionHasError(connection);
}

inline SQLite3MigrationType SQLite3MigrationGetTypeWithPath(CFStringRef path) {
  return CFStringHasSuffix(path, CFSTR(".sql")) ?
    (CFStringHasSuffix(path, CFSTR(".undo.sql")) ?
      kSQLite3MigrationTypeUndoMigration :
      kSQLite3MigrationTypeMigration) :
    kSQLite3MigrationTypeUnknown;
}

inline CFStringRef SQLite3MigrationCreateVersionStringWithPath(CFAllocatorRef allocator, CFStringRef path) {
  CFStringRef version = NULL;
  CFRange range = CFStringFind(path, CFSTR("."), kCFCompareCaseInsensitive);
  if (range.location > 1)
    version = CFStringCreateWithSubstring(allocator, path, CFRangeMake(0, range.location));
  return version;
}

inline BOOL SQLite3MigrationWithDirectoryPath(SQLite3ConnectionRef connection, CFStringRef path) {  
  BOOL result = YES;
//  DIR *dir;
//  struct dirent *ent;
//  size_t bufferLength = CFStringGetMaximumSizeOfFileSystemRepresentation(path);
//  const char *buffer = CFAllocatorAllocate(connection->allocator, bufferLength, 0);
//  CFStringGetFileSystemRepresentation(path, (char *)buffer, bufferLength);
//  dir = opendir(buffer);
//  if (dir != NULL) {
//    while ((ent = readdir(dir)) != NULL) {
//      CFStringRef filename = CFStringCreateWithCString(NULL, ent->d_name, kCFStringEncodingUTF8);
//      switch (SQLite3MigrationGetTypeWithPath(filename)) {
//        case kSQLite3MigrationTypeMigration:
//        {
//          CFStringRef version = SQLite3MigrationCreateVersionStringWithPath(connection->allocator, filename);
//          //NSLog(@"will load %@", path_);
//          
//          CFURLRef fullPathURL = CFURLCreateCopyAppendingPathComponent(connection->allocator, directory, path_, NO);
//          CFStringRef fullPath = CFURLCopyPath(url);
//          
//          CFStringRef sql = (CFStringRef)[[NSString alloc] initWithContentsOfFile: (NSString *)path_ encoding: NSUTF8StringEncoding error: NULL];
//          result = SQLite3MigrationExecute(connection, version, sql) ? result : NO;
//          CFRelease(sql);
//          CFRelease(version);
//        }
//        default:
//        {
//        }
//      }
//      CFRelease(path_);
//    }
//    closedir (dir);
//  } else {
//    // Could not open directory
//  }
  return result;
}
