//
// SQLite3Migration.c
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 08/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#include "SQLite3Migration.h"

#define SQLite3MigrationMaxCount 4096

#pragma Global stuff

SQLite3MigrationType SQLite3MigrationGetTypeWithPath(CFStringRef path) {
  return CFStringHasSuffix(path, CFSTR(".sql")) ? (CFStringHasSuffix(path, CFSTR(".undo.sql")) ?
                                                   kSQLite3MigrationTypeUndoMigration :
                                                   kSQLite3MigrationTypeMigration) :
                                                   kSQLite3MigrationTypeUnknown;
}

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

CFMutableArrayRef SQlite3MigrationCreateFilenameArrayWithDirectory(CFAllocatorRef allocator, CFStringRef directory) {
  CFMutableArrayRef array = NULL;
  if (directory) {
    __SQLite3UTF8String utf8Directory = __SQLite3UTF8StringMake(allocator, directory);
    DIR *dir = opendir(__SQLite3UTF8StringGetBuffer(utf8Directory));
    struct dirent *ent = NULL;
    if (dir != NULL) {
      array = CFArrayCreateMutable(allocator, 0, &kCFTypeArrayCallBacks);
      while ((ent = readdir(dir)) != NULL) {
        CFStringRef filename = CFStringCreateWithCString(NULL, ent->d_name, kCFStringEncodingUTF8);
        CFArrayAppendValue(array, filename);
        CFRelease(filename);
      }
    }
    __SQLite3UTF8StringDestroy(utf8Directory);
  }
  return array;
  
  //  struct dirent *ent;
  //  size_t bufferLength = CFStringGetMaximumSizeOfFileSystemRepresentation(path);
  //  const char *buffer = CFAllocatorAllocate(connection->allocator, bufferLength, 0);
  //  CFStringGetFileSystemRepresentation(path, (char *)buffer, bufferLength);
  //  dir = opendir(buffer);
  //  
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
}

#pragma Connection related stuff

bool SQLite3MigrationDoesTableExist(SQLite3ConnectionRef connection) {
  return SQLite3ConnectionDoesTableExist(connection, CFSTR("schema_migrations"));
}

SQLite3Status SQLite3MigrationCreateTable(SQLite3ConnectionRef connection) {
  SQLite3Status status = kSQLite3StatusOK;
  if (!SQLite3MigrationDoesTableExist(connection))
    status = SQLite3ConnectionExecute(connection, CFSTR("create table schema_migrations(version string)"));
  return status;
}

// A list of version strings currently in schema_changes table.
// If the migration table doesn't exist, returns NULL.
CFArrayRef SQLite3MigrationCreateVersionsArray(SQLite3ConnectionRef connection) {
  CFMutableArrayRef versions = NULL;
  if (SQLite3MigrationDoesTableExist(connection)) {
    versions = CFArrayCreateMutable(connection->allocator, 0, &kCFTypeArrayCallBacks);
    SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("select version from schema_migrations"));
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
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("select exists(select * from schema_migrations where version = ?)"));
  SQLite3StatementBindString(statement, 1, version);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
    didMigrated = SQLite3StatementGetBOOLWithColumn(statement, 0);
  SQLite3StatementClearBindings(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return didMigrated;
}

SQLite3Status SQLite3MigrationInsertVersion(SQLite3ConnectionRef connection, CFStringRef version) {
  SQLite3Status status = kSQLite3StatusError;
  if (!SQLite3MigrationDidMigratedVersion(connection, version)) {
    SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("insert into schema_migrations(version) values(?)"));
    SQLite3StatementBindString(statement, 1, version);
    status = SQLite3StatementStep(statement);
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
    SQLite3StatementClearBindings(statement);
    SQLite3StatementFinalize(statement);
    SQLite3StatementRelease(statement);
  }
  return status;
}

SQLite3Status SQLite3MigrationExecute(SQLite3ConnectionRef connection, CFStringRef version, CFStringRef sql) {
  SQLite3Status status = kSQLite3StatusOK;
  if (!SQLite3MigrationDidMigratedVersion(connection, version)) {
    // TODO: transaction
    status = SQLite3ConnectionExecute(connection, sql);
    SQLite3MigrationInsertVersion(connection, version); // TODO: check status
  }
  return status;
}

SQLite3Status SQLite3MigrationExecuteUndo(SQLite3ConnectionRef connection, CFStringRef version, CFStringRef sql) {
  SQLite3Status status = kSQLite3StatusOK;
  if (SQLite3MigrationDidMigratedVersion(connection, version)) {
    // TODO: transaction
    status = SQLite3ConnectionExecute(connection, sql);
    SQLite3MigrationDeleteVersion(connection, version); // TODO: check status
  }
  return status;
}

bool SQLite3MigrationWithDirectoryPath(SQLite3ConnectionRef connection, CFStringRef path) {  
  bool result = 1;
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
