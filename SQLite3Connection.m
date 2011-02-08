/*
 *  SQLite3Connection.c
 *
 *  Created by Mirek Rusin on 07/02/2011.
 *  Copyright 2011 Inteliv Ltd. All rights reserved.
 *
 */

#import "SQLite3Connection.h"


SQLite3ConnectionRef SQLite3ConnectionCreate(CFAllocatorRef allocator, CFStringRef path, int flags, const char *zVfs) {
  // TODO: set the allocator
  SQLite3ConnectionRef connection = CFAllocatorAllocate(allocator, sizeof(SQLite3ConnectionRef), 0);
  if (sqlite3_open_v2([(NSString *)path UTF8String], &connection->db, flags, zVfs)) {
    // TODO: Failure
    NSLog(@"FAIL: %s", sqlite3_errmsg(connection->db));
    //CFAllocatorDeallocate(allocator, connection);
  } else {
    NSLog(@"OK %@", path);
    // Success
  }
  return connection;
}

CFErrorRef SQLite3ConnectionCreateError(SQLite3ConnectionRef connection) {
  CFErrorRef error = NULL;
  if (SQLite3ConnectionHasError(connection)) {
    CFStringRef keys[1] = { kCFErrorLocalizedDescriptionKey };
    CFStringRef values[1] = { CFStringCreateWithCString(NULL, sqlite3_errmsg(connection->db), kCFStringEncodingUTF8) };
    CFDictionaryRef userInfo = CFDictionaryCreate(NULL, (void *)keys, (void *)values, 1, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    error = CFErrorCreate(NULL, CFSTR("com.github.mirek.sqlite3"), sqlite3_errcode(connection->db), userInfo);
    CFRelease(userInfo);
    CFRelease(values[1]);
  }
  return error;
}

inline BOOL SQLite3ConnectionHasError(SQLite3ConnectionRef connection) {
  return sqlite3_errcode(connection->db) != SQLITE_OK;
}

inline void SQLite3ConnectionRelease(SQLite3ConnectionRef connection) {
  // TODO: Release
}

inline sqlite3 *SQLite3ConnectionGetConnection(SQLite3ConnectionRef connection) {
  return connection->db;
}

inline NSUInteger SQLite3ConnectionGetRetainCount(SQLite3ConnectionRef connection) {
  return connection->retainCount;
}

inline NSUInteger SQLite3ConnectionRetain(SQLite3ConnectionRef connection) {
  return ++connection->retainCount;
}


