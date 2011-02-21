//
// SQLite3Connection.c
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 07/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#include "SQLite3Connection.h"

#pragma Lifecycle

inline SQLite3ConnectionRef _SQLite3ConnectionCreate(CFAllocatorRef allocator, CFStringRef path, int flags, const char *zVfs) {
  SQLite3ConnectionRef connection = CFAllocatorAllocate(allocator, sizeof(SQLite3ConnectionRef), 0);
  if (connection) {
    connection->allocator = allocator ? CFRetain(allocator) : NULL;
    connection->retainCount = 1;
    
    // Default date formatter
    connection->defaultDateFormatter = CFDateFormatterCreate(NULL, NULL, kCFDateFormatterNoStyle, kCFDateFormatterNoStyle);
    CFDateFormatterSetFormat(connection->defaultDateFormatter, CFSTR("yyyy-MM-dd HH:mm:ss"));
    
    __SQLite3UTF8String utf8Path = __SQLite3UTF8StringMake(connection->allocator, path);
    int sqlite3_open_v2_result = sqlite3_open_v2(__SQLite3UTF8StringGetBuffer(utf8Path), &connection->db, flags, zVfs);
    __SQLite3UTF8StringDestroy(utf8Path);
    
    if (sqlite3_open_v2_result == SQLITE_OK) {
      // OK
    } else {
      if (!connection->db) // If the sqlite3 connection has not been allocated, we deallocate connection and return NULL
        SQLite3ConnectionRelease(connection);
    }
  }
  return connection;
}

inline SQLite3ConnectionRef SQLite3ConnectionCreate(CFStringRef path, int flags, const char *zVfs) {
  return _SQLite3ConnectionCreate(NULL, path, flags, zVfs);
}

// TODO: Open in read-only mode
// Pass bundle = NULL to use the main bundle
void SQLite3OpenResource(CFBundleRef bundle, CFStringRef resourceName, CFStringRef resourceType, CFStringRef subDirName, sqlite3 **db) {
  if (bundle == NULL)
    bundle = CFBundleGetMainBundle();
  CFURLRef url = CFBundleCopyResourceURL(bundle, resourceName, resourceType, subDirName);
  if (url) {
    CFShow(url);
    if (sqlite3_open_v2(CFStringGetCStringPtr(CFURLGetString(url), kCFStringEncodingUTF8), db, SQLITE_OPEN_READONLY, NULL)) {
      printf("ERROR %s\n", (const unsigned char *)sqlite3_errmsg(*db));
    }
    CFRelease(url);
  }
}

inline int SQLite3ConnectionClose(SQLite3ConnectionRef connection) {
  return connection ? sqlite3_close(connection->db) : SQLITE_OK;
}

inline SQLite3ConnectionRef SQLite3ConnectionRelease(SQLite3ConnectionRef connection) {
  if (--connection->retainCount <= 0) {
    
    // Fist, let's close connection
    SQLite3ConnectionClose(connection);
    
    // Release date formatter created on init
    CFRelease(connection->defaultDateFormatter);
    
    // Deallocate connection stuct
    CFAllocatorRef allocator = connection->allocator;
    CFAllocatorDeallocate(allocator, connection);
    connection = NULL;
    
    // Release allocator if has been set
    if (allocator)
      CFRelease(allocator);
  }
  return connection;
}

inline CFIndex SQLite3ConnectionGetRetainCount(SQLite3ConnectionRef connection) {
  return connection->retainCount;
}

inline SQLite3ConnectionRef SQLite3ConnectionRetain(SQLite3ConnectionRef connection) {
  ++connection->retainCount;
  return connection;
}

// If the connection is NULL, return error. Otherwise return NULL if the sqlite3 connection
// doesn't have an error. Otherwise return the error.
CFErrorRef SQLite3ConnectionCreateError(SQLite3ConnectionRef connection) {
  CFErrorRef error = NULL;
  if (SQLite3ConnectionHasError(connection)) {
    const char *errmsg = connection ? sqlite3_errmsg(connection->db) : "Connection has not been allocated";
    int errcode = connection ? sqlite3_errcode(connection->db) : SQLITE_ERROR;
    CFStringRef keys[1] = { kCFErrorLocalizedDescriptionKey };
    CFStringRef values[1] = { CFStringCreateWithCString(NULL, errmsg, kCFStringEncodingUTF8) };
    CFDictionaryRef userInfo = CFDictionaryCreate(NULL, (void *)keys, (void *)values, 1, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    error = CFErrorCreate(NULL, CFSTR("com.github.mirek.CoreSQLite3"), errcode, userInfo);
    CFRelease(userInfo);
    CFRelease(values[1]);
  }
  return error;
}

// If the connection is NULL or sqlite3 connection has an error, return YES. Otherwise return NO.
inline bool SQLite3ConnectionHasError(SQLite3ConnectionRef connection) {
  return connection ? sqlite3_errcode(connection->db) != SQLITE_OK : 1;
}

// Return NULL if the connection is not allocated. Otherwise return sqlite3 connection.
inline sqlite3 *SQLite3ConnectionGetConnection(SQLite3ConnectionRef connection) {
  return connection ? connection->db : NULL;
}

#pragma Resultset utility functions

inline int SQLite3ConnectionExecutev(SQLite3ConnectionRef connection, CFStringRef sql, ...) {
  va_list arguments;
  va_start(arguments, sql);
  CFStringRef sqlv = CFStringCreateWithFormatAndArguments(connection->allocator, NULL, sql, arguments);
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sqlv);
  CFRelease(sqlv);
  va_end(arguments);
  int code = SQLite3StatementExecute(statement);
  SQLite3StatementRelease(statement);
  return code;
}

inline int SQLite3ConnectionExecute(SQLite3ConnectionRef connection, CFStringRef sql) {
  int code = SQLITE_ERROR;
  if (sql) {
    __SQLite3UTF8String utf8Sql = __SQLite3UTF8StringMake(connection->allocator, sql);
    code = sqlite3_exec(connection->db, __SQLite3UTF8StringGetBuffer(utf8Sql), NULL, NULL, NULL);
    __SQLite3UTF8StringDestroy(utf8Sql);
//    SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
//    code = SQLite3StatementExecute(statement);
//    SQLite3StatementRelease(statement);
  }
  return code;
}

// Executes UTF-8 sql file.
inline int SQLite3ConnectionExecuteWithContentsOfFileAtPath(SQLite3ConnectionRef connection, CFStringRef path) {
  // TODO: Remove Objective-C
  
//  __SQLite3UTF8String utf8Sql = __SQLite3UTF8StringCreate(connection->allocator, sql);
//  __SQLite3UTF8StringGetBuffer(utf8Sql)
//  __SQLite3UTFStringDestroy(utf8Sql);

  
//  CFStringRef sql = (CFStringRef)[[NSString alloc] initWithContentsOfFile: (NSString *)path encoding: NSUTF8StringEncoding error: nil];
//  int result = SQLite3ConnectionExecute(connection, sql);
//  CFRelease(sql);
//  return result;
  return -1;
}

inline int32_t SQLite3ConnectionGetInt32WithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  int32_t value = 0;
//  va_list arguments;
//  va_start(arguments, sql);
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
//  va_end(arguments);
  for (; SQLite3StatementStep(statement) == SQLITE_ROW; ) {
    value = SQLite3StatementGetInt32WithColumn(statement, 0);
    break;
  }
  SQLite3StatementRelease(statement);
  return value;
}

inline int64_t SQLite3ConnectionGetInt64WithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  int64_t value = 0;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  for (; SQLite3StatementStep(statement) == SQLITE_ROW; ) {
    value = SQLite3StatementGetInt64WithColumn(statement, 0);
    break;
  }
  SQLite3StatementRelease(statement);
  return value;
}

inline bool SQLite3ConnectionGetBOOLWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  bool value = 0;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  for (; SQLite3StatementStep(statement) == SQLITE_ROW; ) {
    value = SQLite3StatementGetBOOLWithColumn(statement, 0);
    break;
  }
  SQLite3StatementRelease(statement);
  return value;
}

inline CFStringRef SQLite3ConnectionCreateStringWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  CFStringRef value = NULL;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  for (; SQLite3StatementStep(statement) == SQLITE_ROW; ) {
    value = SQLite3StatementCreateStringWithColumn(statement, 0);
    break;
  }
  SQLite3StatementRelease(statement);
  return value;
}

inline CFDataRef SQLite3ConnectionCreateDataWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  CFDataRef value = NULL;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  for (; SQLite3StatementStep(statement) == SQLITE_ROW; ) {
    value = SQLite3StatementCreateDataWithColumn(statement, 0);
    break;
  }
  SQLite3StatementRelease(statement);
  return value;
}

//inline CGImageRef SQLite3ConnectionCreateImageWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
//  CGImageRef value = NULL;
//  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
//  for (; SQLite3StatementStep(statement) == SQLITE_ROW; ) {
//    value = SQLite3StatementCreateImageWithColumn(statement, 0);
//    break;
//  }
//  SQLite3StatementRelease(statement);
//  return value;
//}

inline CFDateRef SQLite3ConnectionCreateDateWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  CFDateRef value = NULL;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  for (; SQLite3StatementStep(statement) == SQLITE_ROW; ) {
    value = SQLite3StatementCreateDateWithColumn(statement, 0);
    break;
  }
  SQLite3StatementRelease(statement);
  return value;
}

inline CFDictionaryRef SQLite3ConnectionCreateDictionaryForAllColumnsWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  CFDictionaryRef value = NULL;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  for (; SQLite3StatementStep(statement) == SQLITE_ROW; ) {
    value = SQLite3StatementCreateDictionaryWithAllColumns(statement);
    break;
  }
  SQLite3StatementRelease(statement);
  return value;
}

bool SQLite3ConnectionDoesTableExistWithName(SQLite3ConnectionRef connection, CFStringRef name) {
//  CFStringRef sql = CFStringCreateWithFormat(connection->allocator, CFSTR(""), )
//  BOOL value = SQLite3ConnectionGetBOOLWithQuery(connection, );
//  return value;
  return 0;
}

bool SQLite3ConnectionDoesViewExistWithName(SQLite3ConnectionRef connection, CFStringRef name) {
  return 0;
}

bool SQLite3ConnectionDoesTableOrViewExistWithName(SQLite3ConnectionRef connection, CFStringRef name) {
  return 0;
}
