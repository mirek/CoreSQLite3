/*
 *  SQLite3Statement.c
 *
 *  Created by Mirek Rusin on 07/02/2011.
 *  Copyright 2011 Inteliv Ltd. All rights reserved.
 *
 */

#import "SQLite3Statement.h"

SQLite3StatementRef SQLite3StatementCreate(SQLite3ConnectionRef connection, CFStringRef sql) {
  return _SQLite3StatementCreate(NULL, connection, sql);
}

SQLite3StatementRef _SQLite3StatementCreate(CFAllocatorRef allocator, SQLite3ConnectionRef connection, CFStringRef sql) {
  SQLite3StatementRef statement = CFAllocatorAllocate(allocator, sizeof(SQLite3Statement), 0);
  if (statement) {
    char *sql_ = NULL; //CFStringGetCStringPtr(sql, kCFStringEncodingUTF8);
    if (sql_ == NULL) {
      sql_ = CFAllocatorAllocate(allocator, CFStringGetLength(sql) * 2 + 1, 0);
      CFStringGetCString(sql, sql_, CFStringGetLength(sql) * 2 + 1, kCFStringEncodingUTF8);
      sqlite3_prepare(connection->db, sql_, -1, &(statement->stmt), NULL);
      CFAllocatorDeallocate(allocator, (void *)sql_);
    }
  }
  return statement;
}

inline int SQLite3StatementStep(SQLite3StatementRef statement) {
  return sqlite3_step(statement->stmt);
}

inline void SQLite3StatementExecute(SQLite3StatementRef statement) {
  for (; sqlite3_step(statement->stmt) == SQLITE_ROW; ) {}
}

inline void SQLite3Execute(SQLite3ConnectionRef connection, CFStringRef sql) {
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  SQLite3StatementExecute(statement);
  //SQLite3StatementRelease(statement);
}

// Column name by index
inline CFStringRef SQLite3CreateColumnNameStringWithIndex(SQLite3StatementRef statement, NSInteger index) {
  return CFStringCreateWithCString(NULL, sqlite3_column_name(statement->stmt, index), kCFStringEncodingUTF8);
}

inline NSUInteger SQLite3StatementGetColumnCount(SQLite3StatementRef statement) {
  return sqlite3_column_count(statement->stmt);
}

// Return NSNotFound if not found
inline NSInteger SQLite3StatementGetColumnIndexWithName(SQLite3StatementRef statement, CFStringRef name) {
  NSInteger index = NSNotFound;
  if (name) {
    const char *nameUTF8 = [(NSString *)name UTF8String];
    if (nameUTF8) {
      NSUInteger n = sqlite3_column_count(statement->stmt);
      for (NSUInteger i = 0; i < n; i++) {
        if (0 == strcmp(nameUTF8, sqlite3_column_name(statement->stmt, i))) {
          index = i;
          break;
        }
      }
    }
  }
  return index;
}

#pragma Binding

inline int SQLite3StatementBindInt32(SQLite3StatementRef statement, NSInteger index, int32_t value) {
  return sqlite3_bind_int(statement->stmt, index, value);
}

inline int SQLite3StatementBindInt64(SQLite3StatementRef statement, NSInteger index, int64_t value) {
  return sqlite3_bind_int64(statement->stmt, index, value);
}

inline int SQLite3StatementBindDouble(SQLite3StatementRef statement, NSInteger index, double_t value) {
  return sqlite3_bind_double(statement->stmt, index, value);
}

inline int SQLite3StatementBindNULL(SQLite3StatementRef statement, NSInteger index) {
  return sqlite3_bind_null(statement->stmt, index);
}

inline int SQLite3StatementBindString(SQLite3StatementRef statement, NSInteger index, CFStringRef value) {
  const char *valueCString = [(NSString *)value UTF8String];
  return sqlite3_bind_text(statement->stmt, index, valueCString, -1, NULL);
}

inline int SQLite3StatementBindData(SQLite3StatementRef statement, NSInteger index, CFDataRef data) {
  return sqlite3_bind_blob(statement->stmt, index, CFDataGetBytePtr(data), CFDataGetLength(data), NULL);
}

// Extra stuff

inline void SQLite3StatementBindCFType(SQLite3StatementRef statement, NSInteger index, CFTypeRef value) {
  CFTypeID valueTypeID = CFGetTypeID(value);
  if (CFStringGetTypeID() == valueTypeID)
    SQLite3StatementBindString(statement, index, (CFStringRef)value);
}

// Bind an array of values to the statement.
//
// Example:
// 
//   NSArray *values = [NSArray arrayWithObjects: @"Mirek", @"Rusin", nil];
//   SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("insert into users(name, surname) values(?, ?)"));
//   SQLite3StatementBindArray(statement, values);
//   SQLite3StatementExecute(statement);
//
inline void SQLite3StatementBindArray(SQLite3StatementRef statement, CFArrayRef values) {
  NSUInteger n = CFArrayGetCount(values);
  for (NSUInteger i = 0; i < n; i++)
    SQLite3StatementBindCFType(statement, i + 1, CFArrayGetValueAtIndex(values, i));
}

#pragma Resultsets

inline NSInteger SQLite3StatementIntegerWithColumn(SQLite3StatementRef statement, NSInteger index) {
  return (NSInteger)sqlite3_column_int(statement->stmt, index);
}

inline CFStringRef SQLite3StatementCreateStringWithColumn(SQLite3StatementRef statement, NSInteger index) {
  return CFStringCreateWithBytes(NULL, sqlite3_column_text(statement->stmt, index), sqlite3_column_bytes(statement->stmt, index), kCFStringEncodingUTF8, NO);
}

inline CFDataRef SQLite3StatementCreateDataWithColumn(SQLite3StatementRef statement, NSInteger index) {
  return CFDataCreate(NULL, sqlite3_column_blob(statement->stmt, index), sqlite3_column_bytes(statement->stmt, index));
}

inline CGImageRef SQLite3StatementCreateImageWithColumn(SQLite3StatementRef statement, NSInteger index) {
  CGImageRef image = NULL;
  CFDataRef data = SQLite3StatementCreateDataWithColumn(statement, index);
  if (data) {
    CGImageSourceRef source = CGImageSourceCreateWithData(data, NULL);
    image = CGImageSourceCreateImageAtIndex(source, 0, NULL);
    // TODO: release source?
    CFRelease(data);
  }
  return image;
}

inline CGImageRef SQLite3CreateImageTile(SQLite3ConnectionRef connection, CFStringRef table, NSUInteger z, NSUInteger x, NSUInteger y) {
  CGImageRef image = NULL;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("select image from tiles where z = ? and x = ? and y = ? limit 1"));
  SQLite3StatementBindInt32(statement, 1, z);
  SQLite3StatementBindInt32(statement, 2, x);
  SQLite3StatementBindInt32(statement, 3, y);
  while (SQLITE_ROW == SQLite3StatementStep(statement)) {
    image = SQLite3StatementCreateImageWithColumn(statement, 0);
    break;
  }
  // TODO: SQLite3StatementRelease(statement);
  return image;
}

inline SQLite3Type SQLite3StatementGetColumnType(SQLite3StatementRef statement, NSInteger index) {
  return sqlite3_column_type(statement->stmt, index);
}

#pragma Resultsets with column name

inline CGImageRef SQLite3StatementCreateImageWithColumnName(SQLite3StatementRef statement, CFStringRef name) {
  return SQLite3StatementCreateImageWithColumn(statement, SQLite3StatementGetColumnIndexWithName(statement, name));
}

// Helper function used to create "?,?,...\0" value placeholders.
inline const char *_SQLite3CreateValuesPlaceholderCString(CFAllocatorRef allocator, NSUInteger n) {
  char *s = CFAllocatorAllocate(allocator, 2 * n, 0);
  for (NSUInteger i = 0; i < n * 2; i += 2) {
    s[i]     = '?';
    s[i + 1] = ',';
  }
  s[n * 2 - 1] = 0;
  return s;
}

// Insert values into the table from a CFDictionary.
//
// If the number of key-values pairs in the dictionary is zero, insert is ignored.
//
// Example:
// 
//   NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys: @"Mirek", @"name",
//                                                                          @"Rusin", @"surname"];
//
//   SQLite3InsertWithDictionary(connection, CFSTR("users"), (CFDictionaryRef)attributes);
//
// TODO:
// - sanitize table name and keys
// - return values for binding
inline void SQLite3InsertWithDictionary(SQLite3ConnectionRef connection, CFStringRef table, CFDictionaryRef dictionary) {
  NSUInteger n = CFDictionaryGetCount(dictionary);
  
  // Ignore insert if the number of arguments is less than zero
  if (n > 0) {
    const void **keys = CFAllocatorAllocate(NULL, sizeof(void *) * n, 0);
    const void **values = CFAllocatorAllocate(NULL, sizeof(void *) * n, 0);
    CFDictionaryGetKeysAndValues(dictionary, keys, values);
    CFArrayRef keysArray = CFArrayCreate(NULL, keys, n, &kCFTypeArrayCallBacks);
    CFArrayRef valuesArray = CFArrayCreate(NULL, values, n, &kCFTypeArrayCallBacks);
    CFStringRef keysString = CFStringCreateByCombiningStrings(NULL, keysArray, CFSTR(", "));
    const char *valuesCString = _SQLite3CreateValuesPlaceholderCString(NULL, n);
    CFStringRef sql = CFStringCreateWithFormat(NULL, NULL, CFSTR("INSERT INTO %@(%@) VALUES(%s)"), table, keysString, valuesCString);
    SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
    SQLite3StatementBindArray(statement, valuesArray);
    SQLite3StatementExecute(statement);
    CFRelease(sql);
    CFAllocatorDeallocate(NULL, (void *)valuesCString);
    CFRelease(keysString);
    CFRelease(keysArray);
    CFAllocatorDeallocate(NULL, keys);
    CFAllocatorDeallocate(NULL, values);
  }
}

#pragma Lifecycle

inline int SQLite3StatementFinalize(SQLite3StatementRef statement) {
  return sqlite3_finalize(statement->stmt);
}

inline int SQLite3StatementClearBindings(SQLite3StatementRef statement) {
  return sqlite3_clear_bindings(statement->stmt);
}

inline void SQLite3StatementRelease(SQLite3StatementRef statement) {
  SQLite3StatementClearBindings(statement);
  SQLite3StatementFinalize(statement);
  // TODO: proper release with retainCount check
}
