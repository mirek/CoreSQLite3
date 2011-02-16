//
// SQLite3Statement.c
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 07/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "SQLite3Statement.h"

#pragma Lifecycle

// SQLite3ConnectionRef connection is retained by the statement
inline SQLite3StatementRef _SQLite3StatementCreate(CFAllocatorRef allocator, SQLite3ConnectionRef connection, CFStringRef sql) {
  SQLite3StatementRef statement = CFAllocatorAllocate(allocator, sizeof(SQLite3Statement), 0);
  if (statement) {
    statement->connection = SQLite3ConnectionRetain(connection);
    statement->allocator = allocator ? CFRetain(allocator) : NULL;
    statement->retainCount = 1;
    sqlite3_prepare(connection->db, [(NSString *)sql UTF8String], -1, &statement->stmt, NULL);
//    va_list arguments;
//    va_start(arguments, sql);
//    CFTypeRef argument = NULL;
//    NSInteger i = 0;
//    while ((argument = va_arg(arguments, CFTypeRef))) {
//      NSLog(@"will bind i = %ld, %p %p", i, (void *)argument, (void *)sql);
//      CFShow(sql);
//      CFShow(argument);
//      //SQLite3StatementBindCFType(statement, i++, argument);
//    }
//    va_end(arguments);
  }
  return statement;
}

inline SQLite3StatementRef SQLite3StatementCreate(SQLite3ConnectionRef connection, CFStringRef sql) {
//	va_list arguments;
//	va_start(arguments, sql);
  SQLite3StatementRef value = _SQLite3StatementCreate(NULL, connection, sql);
//	va_end(arguments);
  return value;
}

inline SQLite3StatementRef SQLite3StatementRetain(SQLite3StatementRef statement) {
  statement->retainCount++;
  return statement;
}

inline SQLite3StatementRef SQLite3StatementRelease(SQLite3StatementRef statement) {
  if (--statement->retainCount <= 0) {
    //SQLite3StatementClearBindings(statement);
    //SQLite3StatementFinalize(statement);
    SQLite3ConnectionRelease(statement->connection);
    CFAllocatorRef allocator = statement->allocator;
    CFAllocatorDeallocate(allocator, statement);
    statement = NULL;
    if (allocator)
      CFRelease(allocator);
  }
  return statement;
}

inline int SQLite3StatementFinalize(SQLite3StatementRef statement) {
  return sqlite3_finalize(statement->stmt);
}

inline int SQLite3StatementClearBindings(SQLite3StatementRef statement) {
  return sqlite3_clear_bindings(statement->stmt);
}

inline SQLite3ConnectionRef SQLite3StatementGetConnection(SQLite3StatementRef statement) {
  return statement->connection;
}

inline int SQLite3StatementStep(SQLite3StatementRef statement) {
  return sqlite3_step(statement->stmt);
}

inline int SQLite3StatementExecute(SQLite3StatementRef statement) {
  for (; sqlite3_step(statement->stmt) == SQLITE_ROW; ) {}
  sqlite3_finalize(statement->stmt);
  return SQLITE_OK; // TODO: change this one
}

// Column name by index
inline CFStringRef SQLite3CreateColumnNameStringWithIndex(SQLite3StatementRef statement, NSInteger index) {
  return CFStringCreateWithCString(NULL, sqlite3_column_name(statement->stmt, (int)index), kCFStringEncodingUTF8);
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
      NSInteger n = sqlite3_column_count(statement->stmt);
      for (NSInteger i = 0; i < n; i++) {
        if (0 == strcmp(nameUTF8, sqlite3_column_name(statement->stmt, (int)i))) {
          index = i;
          break;
        }
      }
    }
  }
  return index;
}

#pragma Binding

inline int SQLite3StatementBindData(SQLite3StatementRef statement, NSInteger index, CFDataRef data) {
  return sqlite3_bind_blob(statement->stmt, (int)index, CFDataGetBytePtr(data), (int)CFDataGetLength(data), NULL);
}

inline int SQLite3StatementBindDouble(SQLite3StatementRef statement, NSInteger index, double_t value) {
  return sqlite3_bind_double(statement->stmt, (int)index, value);
}


inline int SQLite3StatementBindInt32(SQLite3StatementRef statement, NSInteger index, int32_t value) {
  return sqlite3_bind_int(statement->stmt, (int)index, value);
}

inline int SQLite3StatementBindInt64(SQLite3StatementRef statement, NSInteger index, int64_t value) {
  return sqlite3_bind_int64(statement->stmt, (int)index, value);
}

inline int SQLite3StatementBindNULL(SQLite3StatementRef statement, NSInteger index) {
  return sqlite3_bind_null(statement->stmt, (int)index);
}

inline int SQLite3StatementBindString(SQLite3StatementRef statement, NSInteger index, CFStringRef value) {
  const char *valueCString = [(NSString *)value UTF8String];
  return sqlite3_bind_text(statement->stmt, (int)index, valueCString, -1, NULL);
}

#pragma Extra binding functions

inline int SQLite3StatementBindCFType(SQLite3StatementRef statement, NSInteger index, CFTypeRef value) {
  int result = SQLITE_ERROR;
  CFTypeID valueTypeID = CFGetTypeID(value);
  if (CFStringGetTypeID() == valueTypeID)
    result = SQLite3StatementBindString(statement, index, (CFStringRef)value);
  else if (CFDataGetTypeID() == valueTypeID)
    result = SQLite3StatementBindData(statement, index, (CFDataRef)value);
//  else if (CGImageGetTypeID() == valueTypeID)
//    result = SQLite3StatementBindImage(statement, index, (CGImageRef)value);
  else if (CFNumberGetTypeID() == valueTypeID)
    switch (CFNumberGetType(value)) {
      case kCFNumberSInt8Type:
      case kCFNumberSInt16Type:
      case kCFNumberSInt32Type:
      case kCFNumberSInt64Type:
      case kCFNumberCharType:
      case kCFNumberShortType:
      case kCFNumberIntType:
      case kCFNumberLongType:
      case kCFNumberLongLongType:
      case kCFNumberCFIndexType:
      case kCFNumberNSIntegerType:
      {
        int64_t value_ = 0;
        CFNumberGetValue(value, kCFNumberSInt64Type, (void *)&value_);
        SQLite3StatementBindInt64(statement, index, value_);
        break;
      } 
      case kCFNumberFloat32Type:
      case kCFNumberFloat64Type:
      case kCFNumberFloatType:
      case kCFNumberDoubleType:
      case kCFNumberCGFloatType: // == kCFNumberMaxType
      {
        double_t value_ = 0.0;
        CFNumberGetValue(value, kCFNumberDoubleType, (void *)&value_);
        SQLite3StatementBindDouble(statement, index, value_);
        break;
      }
    }
  else
    result = SQLITE_ERROR;
  return result;
}

// Bind an array of values to the statement.
//
// If any of the binding fail, return immediatelly without binding following values.
//
// Return SQLITE_OK if all values have been bound or the result code of the first failed binding.
//
// Example:
// 
//   NSArray *values = [NSArray arrayWithObjects: @"Mirek", @"Rusin", nil];
//   SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("insert into users(name, surname) values(?, ?)"));
//   SQLite3StatementBindArray(statement, values);
//   SQLite3StatementExecute(statement);
//
inline int SQLite3StatementBindArray(SQLite3StatementRef statement, CFArrayRef values) {
  int result = SQLITE_OK;
  NSUInteger n = CFArrayGetCount(values);
  for (NSUInteger i = 0; i < n; i++) {
    int ith_result = SQLite3StatementBindCFType(statement, i + 1, CFArrayGetValueAtIndex(values, i));
    if (ith_result != SQLITE_OK) {
      result = ith_result;
      break;
    }
  }
  return result;
}

inline int SQLite3StatementBindDictionary(SQLite3StatementRef statement, CFDictionaryRef keyValuePairs) {
  int result = SQLITE_OK;
  assert(false); // TODO: implement bind dict
  return result;
}

#pragma Resultsets

inline SQLite3Type SQLite3StatementGetColumnType(SQLite3StatementRef statement, NSInteger index) {
  return sqlite3_column_type(statement->stmt, (int)index);
}

inline NSInteger SQLite3StatementGetIntegerWithColumn(SQLite3StatementRef statement, NSInteger index) {
  return (NSInteger)sqlite3_column_int(statement->stmt, (int)index);
}

inline int32_t SQLite3StatementGetInt32WithColumn(SQLite3StatementRef statement, NSInteger index) {
  return (int32_t)sqlite3_column_int(statement->stmt, (int)index);
}

inline int64_t SQLite3StatementGetInt64WithColumn(SQLite3StatementRef statement, NSInteger index) {
  return (int64_t)sqlite3_column_int64(statement->stmt, (int)index);
}

inline BOOL SQLite3StatementGetBOOLWithColumn(SQLite3StatementRef statement, NSInteger index) {
  return sqlite3_column_int(statement->stmt, (int)index) != 0;
}

inline CFStringRef SQLite3StatementCreateStringWithColumn(SQLite3StatementRef statement, NSInteger index) {
  return CFStringCreateWithBytes(NULL, sqlite3_column_text(statement->stmt, (int)index), sqlite3_column_bytes(statement->stmt, (int)index), kCFStringEncodingUTF8, NO);
}

inline CFDataRef SQLite3StatementCreateDataWithColumn(SQLite3StatementRef statement, NSInteger index) {
  return CFDataCreate(NULL, sqlite3_column_blob(statement->stmt, (int)index), sqlite3_column_bytes(statement->stmt, (int)index));
}

inline CGImageRef SQLite3StatementCreateImageWithColumn(SQLite3StatementRef statement, NSInteger index) {
  CGImageRef image = NULL;
  CFDataRef data = SQLite3StatementCreateDataWithColumn(statement, index);
  if (data) {
    CGImageSourceRef source = CGImageSourceCreateWithData(data, NULL);
    image = CGImageSourceCreateImageAtIndex(source, 0, NULL);
    CFRelease(source);
    CFRelease(data);
  }
  return image;
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

//inline int64_t     SQLite3ConnectionGetInt64     (SQLite3ConnectionRef connection, CFStringRef sql);
//inline BOOL        SQLite3ConnectionGetBOOL      (SQLite3ConnectionRef connection, CFStringRef sql);
//inline CFStringRef SQLite3ConnectionCreateString (SQLite3ConnectionRef connection, CFStringRef sql);
//inline CFDataRef   SQLite3ConnectionCreateData   (SQLite3ConnectionRef connection, CFStringRef sql);
//inline CGImageRef  SQLite3ConnectionCreateImage  (SQLite3ConnectionRef connection, CFStringRef sql);


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
