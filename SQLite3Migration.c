//
// SQLite3Migration.c
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 08/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#include "SQLite3Migration.h"

#pragma Global stuff

// Migration files are using names with the following convention:
// * file names ending with ".sql" are considered migration files
// * file names ending with ".undo.sql" are considered undo-migration files (reverting ".sql" migrations)
// ...read more at SQLite3MigrationCreateVersionStringWithPath
SQLite3MigrationType SQLite3MigrationGetTypeWithPath(CFStringRef path) {
  return CFStringHasSuffix(path, CFSTR(".sql")) ? (CFStringHasSuffix(path, CFSTR(".undo.sql")) ?
                                                   kSQLite3MigrationTypeUndoMigration :
                                                   kSQLite3MigrationTypeMigration) :
                                                   kSQLite3MigrationTypeUnknown;
}

CFStringRef SQLite3MigrationCreateVersionStringWithURL(CFAllocatorRef allocator, CFURLRef url) {
  CFStringRef path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
  CFStringRef version = SQLite3MigrationCreateVersionStringWithPath(allocator, path);
  CFRelease(path);
  return version;
}

// ...additionally, the `version` of the migration starts at the begining of the filename and ends
// before the first occurance of "." character or end of the filename.
//
// Migration versions are invoked in the lexicological order.
//
// Examples:
// * ../migrations/2010-02-23_19-25-00.init.sql      -- ok, migration file
// * ../migrations/2010-02-23_19-25-00.init.undo.sql -- ok, undo-migration file (reverting previous file)
// * ../migrations/1.sql                             -- not recommended, use 001.sql instead or date based filenames
CFStringRef SQLite3MigrationCreateVersionStringWithPath(CFAllocatorRef allocator, CFStringRef path) {
  CFStringRef version = NULL;
  CFIndex startIndex = CFStringFind(path, CFSTR("/"), kCFCompareCaseInsensitive | kCFCompareBackwards).location;
  CFIndex endIndex = CFStringFind(path, CFSTR("."), kCFCompareCaseInsensitive).location;
  startIndex = startIndex + 1;
  endIndex = endIndex < 0 ? CFStringGetLength(path) : endIndex;
  if (endIndex > startIndex)
    version = CFStringCreateWithSubstring(allocator, path, CFRangeMake(startIndex, endIndex - startIndex));
  return version;
}

// Returns an CFArrayRef of CFURLRefs for the migration files (migration and undo-migration files).
CFMutableArrayRef SQlite3MigrationCreateURLsArrayWithDirectoryURL(CFAllocatorRef allocator, CFURLRef directoryURL) {
  CFMutableArrayRef array = NULL;
  if (directoryURL) {
    CFStringRef directory = CFURLCopyFileSystemPath(directoryURL, kCFURLPOSIXPathStyle);
    __SQLite3UTF8String utf8Directory = __SQLite3UTF8StringMake(allocator, directory);
    DIR *dir = opendir(__SQLite3UTF8StringGetBuffer(utf8Directory));
    struct dirent *ent = NULL;
    if (dir != NULL) {
      array = CFArrayCreateMutable(allocator, 0, &kCFTypeArrayCallBacks);
      while ((ent = readdir(dir)) != NULL) {
        CFStringRef filename = CFStringCreateWithCString(allocator, ent->d_name, kCFStringEncodingUTF8);
        switch (SQLite3MigrationGetTypeWithPath(filename)) {
          case kSQLite3MigrationTypeMigration:
          case kSQLite3MigrationTypeUndoMigration:
          {
            CFURLRef url = CFURLCreateWithFileSystemPathRelativeToBase(allocator, filename, kCFURLPOSIXPathStyle, 0, directoryURL);
            CFArrayAppendValue(array, url);
            CFRelease(url);
            break;
          }
            
          default:
            break;
        }
        CFRelease(filename);
      }
    }
    __SQLite3UTF8StringDestroy(utf8Directory);
    CFRelease(directory);
  }
  return array;
}

#pragma Connection related stuff

// Return true if migration table (schema_migrations) exists, false otherwise.
bool SQLite3MigrationDoesTableExist(SQLite3ConnectionRef connection) {
  return SQLite3ConnectionDoesTableExist(connection, CFSTR("schema_migrations"));
}

// Create migrations table if it doesn't exist yet.
// Return kSQLite3StatusOK if the table already exists or execute status after invoking create table query.
SQLite3Status SQLite3MigrationCreateTableIfDoesntExist(SQLite3ConnectionRef connection) {
  SQLite3Status status = kSQLite3StatusOK;
  if (!SQLite3MigrationDoesTableExist(connection))
    status = SQLite3ConnectionExecute(connection, CFSTR("create table schema_migrations(version string);"));
  return status;
}

// A list of version strings currently in schema_changes table.
// If the migration table doesn't exist, returns NULL.
CFArrayRef SQLite3MigrationCreateVersionsArray(SQLite3ConnectionRef connection) {
  CFMutableArrayRef versions = NULL;
  if (SQLite3MigrationDoesTableExist(connection)) {
    versions = CFArrayCreateMutable(connection->allocator, 0, &kCFTypeArrayCallBacks);
    SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("select version from schema_migrations;"));
    while (kSQLite3StatusRow == SQLite3StatementStep(statement)) {
      CFStringRef version = SQLite3StatementCreateStringWithColumn(statement, 0);
      CFArrayAppendValue(versions, version);
      CFRelease(version);
    }
    SQLite3StatementFinalize(statement);
    SQLite3StatementRelease(statement);
  }
  return versions;
}

// Check if the migration with provided version has been already performed.
bool SQLite3MigrationDidMigratedVersion(SQLite3ConnectionRef connection, CFStringRef version) {
  bool didMigrated = 0;
  SQLite3Status status = SQLite3MigrationCreateTableIfDoesntExist(connection);
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("select exists(select * from schema_migrations where version = ?);"));
  SQLite3StatementBindString(statement, 1, version);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
    didMigrated = SQLite3StatementGetBOOLWithColumn(statement, 0);
  SQLite3StatementReset(statement);
  SQLite3StatementClearBindings(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return didMigrated;
}

SQLite3Status SQLite3MigrationInsertVersion(SQLite3ConnectionRef connection, CFStringRef version) {
  SQLite3Status status = kSQLite3StatusError;
  if (!SQLite3MigrationDidMigratedVersion(connection, version)) {
    SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("insert into schema_migrations(version) values(?);"));
    SQLite3StatementBindString(statement, 1, version);
    status = SQLite3StatementStep(statement);
    SQLite3StatementReset(statement);
    SQLite3StatementClearBindings(statement);
    SQLite3StatementFinalize(statement);
    SQLite3StatementRelease(statement);
  }
  return status;
}

SQLite3Status SQLite3MigrationDeleteVersion(SQLite3ConnectionRef connection, CFStringRef version) {
  SQLite3Status status = kSQLite3StatusError;
  if (SQLite3MigrationDidMigratedVersion(connection, version)) {
    SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("delete from schema_migrations where version = ?"));
    SQLite3StatementBindString(statement, 1, version);
    status = SQLite3StatementStep(statement);
    SQLite3StatementReset(statement);
    SQLite3StatementClearBindings(statement);
    SQLite3StatementFinalize(statement);
    SQLite3StatementRelease(statement);
  }
  return status;
}

SQLite3Status SQLite3MigrationExecuteWithContentsOfURL(SQLite3ConnectionRef connection, CFURLRef url) {
  SQLite3Status status = SQLite3MigrationCreateTableIfDoesntExist(connection);
  CFStringRef version = SQLite3MigrationCreateVersionStringWithURL(connection->allocator, url);
  if (version) {
    if (!SQLite3MigrationDidMigratedVersion(connection, version)) {
      // TODO: transaction
      status = SQLite3ConnectionExecuteWithContentsOfURL(connection, url);
      SQLite3MigrationInsertVersion(connection, version); // TODO: check status
    }
    CFRelease(version);
  }
  return status;
}

SQLite3Status SQLite3MigrationExecute(SQLite3ConnectionRef connection, CFStringRef version, CFStringRef sql) {
  SQLite3Status status = SQLite3MigrationCreateTableIfDoesntExist(connection);
  if (!SQLite3MigrationDidMigratedVersion(connection, version)) {
    // TODO: transaction
    status = SQLite3ConnectionExecute(connection, sql);
    SQLite3MigrationInsertVersion(connection, version); // TODO: check status
  }
  return status;
}

SQLite3Status SQLite3MigrationExecuteUndo(SQLite3ConnectionRef connection, CFStringRef version, CFStringRef sql) {
  SQLite3Status status = SQLite3MigrationCreateTableIfDoesntExist(connection);
  if (SQLite3MigrationDidMigratedVersion(connection, version)) {
    // TODO: transaction
    status = SQLite3ConnectionExecute(connection, sql);
    SQLite3MigrationDeleteVersion(connection, version); // TODO: check status
  }
  return status;
}

inline SQLite3Status SQLite3MigrationMigrateWithDirectoryURL(SQLite3ConnectionRef connection, CFURLRef directoryURL) {
  SQLite3Status status = kSQLite3StatusError;
  CFArrayRef array = SQlite3MigrationCreateURLsArrayWithDirectoryURL(connection->allocator, directoryURL);
  if (array) {
    for (CFIndex i = 0; i < CFArrayGetCount(array); i++)
      if (kSQLite3StatusOK != (status = SQLite3MigrationExecuteWithContentsOfURL(connection, CFArrayGetValueAtIndex(array, i))))
        break;
    CFRelease(array);
  }
  return status;
}
