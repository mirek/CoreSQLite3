//
// SQLite3Statement.c
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 07/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#include "SQLite3Statement.h"

#pragma Lifecycle

inline SQLite3StatementRef _SQLite3StatementCreate(CFAllocatorRef allocator, SQLite3ConnectionRef connection, CFStringRef sql, CFErrorRef *error) {
  SQLite3StatementRef statement = CFAllocatorAllocate(allocator, sizeof(SQLite3Statement), 0);
  if (statement) {
    statement->connection = SQLite3ConnectionRetain(connection);   // Let's retain connection
    statement->allocator = allocator ? CFRetain(allocator) : NULL; // ...and allocator if not null
    statement->retainCount = 1;
    statement->stmt = NULL;
    __SQLite3UTF8String utf8Sql = __SQLite3UTF8StringMake(statement->allocator, sql);
    SQLite3Status status = sqlite3_prepare_v2(connection->db, __SQLite3UTF8StringGetBuffer(utf8Sql), -1, &statement->stmt, NULL);
    if (kSQLite3StatusOK != status) {
      if (error) {
        *error = CFErrorCreate(allocator, CFSTR("com.github.mirek.SQLite3"), status, NULL);
      }
      printf("ERROR: %s\n", sqlite3_errmsg(connection->db));
      statement = SQLite3StatementRelease(statement); // ...will be set to NULL
    }
    __SQLite3UTF8StringDestroy(utf8Sql);
  }
  return statement;
}

inline SQLite3StatementRef SQLite3StatementCreateWithError(SQLite3ConnectionRef connection, CFStringRef sql, CFErrorRef *error) {
  return _SQLite3StatementCreate(connection->allocator, connection, sql, error);
}

inline SQLite3StatementRef SQLite3StatementCreate(SQLite3ConnectionRef connection, CFStringRef sql) {
  return SQLite3StatementCreateWithError(connection, sql, NULL);
}

inline SQLite3StatementRef SQLite3StatementRetain(SQLite3StatementRef statement) {
  statement->retainCount++;
  return statement;
}

// Before releasing SQLite3StatementRef you should invoke:
// SQLite3StatementReset(statement);
// SQLite3StatementClearBindings(statement); // if bindings were used
// SQLite3StatementFinalize(statement);
inline SQLite3StatementRef SQLite3StatementRelease(SQLite3StatementRef statement) {
  if (--statement->retainCount <= 0) {
    SQLite3ConnectionRelease(statement->connection);
    CFAllocatorRef allocator = statement->allocator;
    CFAllocatorDeallocate(allocator, statement);
    statement = NULL;
    if (allocator)
      CFRelease(allocator);
  }
  return statement;
}

inline SQLite3Status SQLite3StatementReset(SQLite3StatementRef statement) {
  return sqlite3_reset(statement->stmt);
}

inline SQLite3Status SQLite3StatementClearBindings(SQLite3StatementRef statement) {
  return sqlite3_clear_bindings(statement->stmt);
}

inline SQLite3Status SQLite3StatementFinalize(SQLite3StatementRef statement) {
  return sqlite3_finalize(statement->stmt);
}

inline SQLite3ConnectionRef SQLite3StatementGetConnection(SQLite3StatementRef statement) {
  return statement->connection;
}

inline SQLite3Status SQLite3StatementStep(SQLite3StatementRef statement) {
  return sqlite3_step(statement->stmt);
}

//inline SQLite3Status SQLite3StatementExecute(SQLite3StatementRef statement) {
//  SQLite3Status status = SQLite3StatementStep(statement);
//  SQLite3StatementReset(statement);
//  SQLite3StatementClearBindings(statement);
//  SQLite3StatementFinalize(statement);
//  return status;
//}

// Column name by index
inline CFStringRef SQLite3CreateColumnNameStringWithIndex(SQLite3StatementRef statement, CFIndex index) {
  CFStringRef name = NULL;
  const char *name_ = sqlite3_column_name(statement->stmt, (int)index);
  if (name_) {
    name = CFStringCreateWithCString(statement->allocator, name_, kCFStringEncodingUTF8);
    // TODO: Do we need to release name_?
  }
  return name;
}

inline CFIndex SQLite3StatementGetColumnCount(SQLite3StatementRef statement) {
  return sqlite3_column_count(statement->stmt);
}

inline CFIndex SQLite3StatementGetColumnIndexWithName(SQLite3StatementRef statement, CFStringRef name) {
  CFIndex index = -1;
  if (name) {
    __SQLite3UTF8String utf8Name = __SQLite3UTF8StringMake(statement->allocator, name);
    if (__SQLite3UTF8StringGetBuffer(utf8Name)) {
      CFIndex n = sqlite3_column_count(statement->stmt);
      for (CFIndex i = 0; i < n; i++) {
        if (0 == strcmp(__SQLite3UTF8StringGetBuffer(utf8Name), sqlite3_column_name(statement->stmt, (int)i))) {
          index = i;
          break;
        }
      }
    }
    __SQLite3UTF8StringDestroy(utf8Name);
  }
  return index;
}

#pragma Binding

inline CFIndex SQLite3StatementGetBindParameterIndexWithName(SQLite3StatementRef statement, CFStringRef name) {
  int index = 0;
  if (name) {
    __SQLite3UTF8String utf8Name = __SQLite3UTF8StringMake(statement->allocator, name);
    index = sqlite3_bind_parameter_index(statement->stmt, __SQLite3UTF8StringGetBuffer(utf8Name));
    __SQLite3UTF8StringDestroy(utf8Name);
  }
  return index;
}

inline CFIndex SQLite3StatementGetBindParameterCount(SQLite3StatementRef statement) {
  return sqlite3_bind_parameter_count(statement->stmt);
}

inline CFStringRef SQLite3StatementCreateBindParameterNameWithIndex(SQLite3StatementRef statement, CFIndex index, bool withoutLeadingCharacter) {
  CFStringRef name = NULL;
  const char *name_ = sqlite3_bind_parameter_name(statement->stmt, (int)index);
  if (name_)
    name = CFStringCreateWithCString(statement->allocator, name_ + withoutLeadingCharacter, kCFStringEncodingUTF8);
  return name;
}

inline SQLite3Status SQLite3StatementBindData(SQLite3StatementRef statement, CFIndex index, CFDataRef data) {
  int result;
  if (data)
    result = sqlite3_bind_blob(statement->stmt, (int)index, CFDataGetBytePtr(data), (int)CFDataGetLength(data), SQLITE_TRANSIENT);
  else
    result = SQLite3StatementBindNULL(statement, index);
  return result;
}

inline SQLite3Status SQLite3StatementBindDouble(SQLite3StatementRef statement, CFIndex index, double_t value) {
  return sqlite3_bind_double(statement->stmt, (int)index, value);
}

inline SQLite3Status SQLite3StatementBindInt32(SQLite3StatementRef statement, CFIndex index, int32_t value) {
  return sqlite3_bind_int(statement->stmt, (int)index, value);
}

inline SQLite3Status SQLite3StatementBindInt32WithName(SQLite3StatementRef statement, CFStringRef name, int32_t value) {
  return SQLite3StatementBindInt32(statement, SQLite3StatementGetBindParameterIndexWithName(statement, name), value);
}

inline SQLite3Status SQLite3StatementBindInt64(SQLite3StatementRef statement, CFIndex index, int64_t value) {
  return sqlite3_bind_int64(statement->stmt, (int)index, value);
}

inline SQLite3Status SQLite3StatementBindNULL(SQLite3StatementRef statement, CFIndex index) {
  return sqlite3_bind_null(statement->stmt, (int)index);
}

inline SQLite3Status SQLite3StatementBindString(SQLite3StatementRef statement, CFIndex index, CFStringRef value) {
  SQLite3Status status = kSQLite3StatusError;
  if (value) {
    __SQLite3UTF8String utf8Value = __SQLite3UTF8StringMake(statement->allocator, value);
    status = sqlite3_bind_text(statement->stmt, (int)index, __SQLite3UTF8StringGetBuffer(utf8Value), -1, SQLITE_TRANSIENT);
    __SQLite3UTF8StringDestroy(utf8Value);
  } else {
    status = SQLite3StatementBindNULL(statement, index);
  }
  return status;
}

inline SQLite3Status SQLite3StatementBindPropertyList(SQLite3StatementRef statement, CFIndex index, CFPropertyListRef value, CFPropertyListFormat format) {
  SQLite3Status status = kSQLite3StatusError;
  CFDataRef data = CFPropertyListCreateData(statement->allocator, value, format, 0, NULL);
  if (data) {
    status = SQLite3StatementBindData(statement, index, data);
    CFRelease(data);
  }
  return status;
}

inline SQLite3Status SQLite3StatementBindPropertyListWithName(SQLite3StatementRef statement, CFStringRef name, CFPropertyListRef value, CFPropertyListFormat format) {
  return SQLite3StatementBindPropertyList(statement, SQLite3StatementGetBindParameterIndexWithName(statement, name), value, format);
}

#pragma Extra binding functions

inline SQLite3Status SQLite3StatementBindNumber(SQLite3StatementRef statement, CFIndex index, CFNumberRef value) {
  SQLite3Status status = kSQLite3StatusError;
  if (value) {
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
        status = SQLite3StatementBindInt64(statement, index, value_);
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
        status = SQLite3StatementBindDouble(statement, index, value_);
        break;
      }
    }
  } else {
    status = SQLite3StatementBindNULL(statement, index);
  }
  return status;
}

inline SQLite3Status SQLite3StatementBindDateTimeWithAbsoluteTime(SQLite3StatementRef statement, CFIndex index, CFAbsoluteTime value) {
  CFDateRef date = CFDateCreate(statement->allocator, value);
  CFStringRef string = CFDateFormatterCreateStringWithDate(statement->allocator, statement->connection->defaultDateFormatter, date);
  int result = SQLite3StatementBindString(statement, index, string);
  CFRelease(string);
  CFRelease(date);
  return result;
}

inline SQLite3Status SQLite3StatementBindDateTimeWithDate(SQLite3StatementRef statement, CFIndex index, CFDateRef value) {
  if (value)
    return SQLite3StatementBindDateTimeWithAbsoluteTime(statement, index, CFDateGetAbsoluteTime(value));
  else
    return SQLite3StatementBindNULL(statement, index);
}

inline SQLite3Status SQLite3StatementBindCFType(SQLite3StatementRef statement, CFIndex index, CFTypeRef value) {
  SQLite3Status status = kSQLite3StatusError;
  //CFShow(value);
  if (value) {
    CFTypeID valueTypeID = CFGetTypeID(value);
    if (CFStringGetTypeID() == valueTypeID)
      status = SQLite3StatementBindString(statement, index, (CFStringRef)value);
    else if (CFDataGetTypeID() == valueTypeID)
      status = SQLite3StatementBindData(statement, index, (CFDataRef)value);
  //  else if (CGImageGetTypeID() == valueTypeID)
  //    status = SQLite3StatementBindImage(statement, index, (CGImageRef)value);
    else if (CFDateGetTypeID() == valueTypeID)
      status = SQLite3StatementBindDateTimeWithDate(statement, index, (CFDateRef)value);
    else if (CFNumberGetTypeID() == valueTypeID)
      status = SQLite3StatementBindNumber(statement, index, (CFNumberRef)value);
    else
      status = kSQLite3StatusError;
  } else {
    status = SQLite3StatementBindNULL(statement, index);
  }
  return status;
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
inline SQLite3Status SQLite3StatementBindArray(SQLite3StatementRef statement, CFArrayRef values) {
  SQLite3Status status = kSQLite3StatusOK;
  CFIndex n = CFArrayGetCount(values);
  for (CFIndex i = 0; i < n; i++)
    if (kSQLite3StatusOK != (status = SQLite3StatementBindCFType(statement, i + 1, CFArrayGetValueAtIndex(values, i))))
      break;
  return status;
}

// Note that the same name placeholders have the index of the first occurance in sqlite3.
// We're binding only the keys which are present in the query. The dictionary can have
// other keys, which will be simply ignored.
inline SQLite3Status SQLite3StatementBindDictionary(SQLite3StatementRef statement, CFDictionaryRef keyValuePairs) {
  SQLite3Status status = kSQLite3StatusOK;
  CFIndex n = SQLite3StatementGetBindParameterCount(statement);
  for (CFIndex i = 0; i < n; i++) {
    CFStringRef name = SQLite3StatementCreateBindParameterNameWithIndex(statement, i + 1, 1);
    if (name) {
      status = SQLite3StatementBindCFType(statement, i + 1, CFDictionaryGetValue(keyValuePairs, name));
      CFRelease(name);
    } else {
      
      // i-th parameter is not in :name, @name or $name format. We don't know how to bind it
      // properly, so we're setting it to NULL.
      status = SQLite3StatementBindNULL(statement, i + 1);
    }
//    if (status != kSQLite3StatusOK)
//      break;
  }
  return status;
}

#pragma Resultsets

inline CFIndex SQLite3StatementGetColumnsCount(SQLite3StatementRef statement) {
  return sqlite3_column_count(statement->stmt);
}

inline SQLite3Type SQLite3StatementGetColumnType(SQLite3StatementRef statement, CFIndex index) {
  return sqlite3_column_type(statement->stmt, (int)index);
}

inline CFIndex SQLite3StatementGetIntegerWithColumn(SQLite3StatementRef statement, CFIndex index) {
  return (CFIndex)sqlite3_column_int(statement->stmt, (int)index);
}

inline int32_t SQLite3StatementGetInt32WithColumn(SQLite3StatementRef statement, CFIndex index) {
  return (int32_t)sqlite3_column_int(statement->stmt, (int)index);
}

inline int64_t SQLite3StatementGetInt64WithColumn(SQLite3StatementRef statement, CFIndex index) {
  return (int64_t)sqlite3_column_int64(statement->stmt, (int)index);
}

inline bool SQLite3StatementGetBOOLWithColumn(SQLite3StatementRef statement, CFIndex index) {
  return sqlite3_column_int(statement->stmt, (int)index) != 0;
}

inline CFStringRef SQLite3StatementCreateStringWithColumn(SQLite3StatementRef statement, CFIndex index) {
  return CFStringCreateWithBytes(statement->allocator, sqlite3_column_text(statement->stmt, (int)index), sqlite3_column_bytes(statement->stmt, (int)index), kCFStringEncodingUTF8, 0);
}

inline CFDataRef SQLite3StatementCreateDataWithColumn(SQLite3StatementRef statement, CFIndex index) {
  return CFDataCreate(statement->allocator, sqlite3_column_blob(statement->stmt, (int)index), sqlite3_column_bytes(statement->stmt, (int)index));
}

inline CFDateRef SQLite3StatementCreateDateWithColumn(SQLite3StatementRef statement, CFIndex index) {
  CFDateRef date = NULL;
  CFStringRef string = NULL;
  switch (SQLite3StatementGetColumnType(statement, index)) {
      
    // For integer and float, we're interpreting as unix timestamp
    case kSQLite3TypeInteger:
    case kSQLite3TypeFloat:
      date = CFDateCreate(statement->allocator, SQLite3StatementGetInt64WithColumn(statement, index) - kCFAbsoluteTimeIntervalSince1970);
      break;
      
    // For string we're using default format
    case kSQLite3TypeString:
      string = SQLite3StatementCreateStringWithColumn(statement, index);
      date = CFDateFormatterCreateDateFromString(statement->allocator, statement->connection->defaultDateFormatter, string, NULL);
      CFRelease(string);
      break;
    
    // For blobs, nulls (or unknown column type?) we're leaving it as NULL
    case kSQLite3TypeData:
    case kSQLite3TypeNULL:
    default:
      break;
  }
  return date;
}

//inline CGImageRef SQLite3StatementCreateImageWithColumn(SQLite3StatementRef statement, NSInteger index) {
//  CGImageRef image = NULL;
//  CFDataRef data = SQLite3StatementCreateDataWithColumn(statement, index);
//  if (data) {
//    CGImageSourceRef source = CGImageSourceCreateWithData(data, NULL);
//    image = CGImageSourceCreateImageAtIndex(source, 0, NULL);
//    CFRelease(source);
//    CFRelease(data);
//  }
//  return image;
//}

inline CFDictionaryRef SQLite3StatementCreateDictionaryWithAllColumns(SQLite3StatementRef statement) {
  CFDictionaryRef dictionary = NULL;
  CFIndex count = SQLite3StatementGetColumnCount(statement);
  if (count > 0) {
    CFStringRef *keys   = CFAllocatorAllocate(statement->allocator, sizeof(CFStringRef) * count, 0);
    CFStringRef *values = CFAllocatorAllocate(statement->allocator, sizeof(CFStringRef) * count, 0);
    
    for (CFIndex i = 0; i < count; i++) {
      keys[i] = SQLite3CreateColumnNameStringWithIndex(statement, i);
      values[i] = SQLite3StatementCreateStringWithColumn(statement, i);
    }
    
    dictionary = CFDictionaryCreate(statement->allocator, (const void **)keys, (const void **)values, count, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

    for (CFIndex i = 0; i < count; i++) {
      CFRelease(keys[i]);
      CFRelease(values[i]);
    }

    CFAllocatorDeallocate(statement->allocator, keys);
    CFAllocatorDeallocate(statement->allocator, values);
  }
  return dictionary;
}

inline CFPropertyListRef SQLite3StatementCreatePropertyListWithColumn(SQLite3StatementRef statement, CFIndex index, CFOptionFlags options, CFPropertyListFormat *format) {
  CFPropertyListRef result = NULL;
  CFDataRef data = SQLite3StatementCreateDataWithColumn(statement, index);
  if (data) {
    result = CFPropertyListCreateWithData(statement->allocator, data, options, format, NULL);
    CFRelease(data);
  }
  return result;
}

#pragma Resultsets with column name

//inline CGImageRef SQLite3StatementCreateImageWithColumnName(SQLite3StatementRef statement, CFStringRef name) {
//  return SQLite3StatementCreateImageWithColumn(statement, SQLite3StatementGetColumnIndexWithName(statement, name));
//}

// Helper function used to create "?,?,...\0" value placeholders.
inline const char *_SQLite3CreateValuesPlaceholderCString(CFAllocatorRef allocator, CFIndex n) {
  char *s = CFAllocatorAllocate(allocator, 2 * n, 0);
  for (CFIndex i = 0; i < n * 2; i += 2) {
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
inline SQLite3Status SQLite3InsertWithDictionary(SQLite3ConnectionRef connection, CFStringRef table, CFDictionaryRef dictionary) {
  SQLite3Status status = kSQLite3StatusError;
  CFIndex n = CFDictionaryGetCount(dictionary);
  const void **keys = CFAllocatorAllocate(connection->allocator, sizeof(void *) * n, 0);
  const void **values = CFAllocatorAllocate(connection->allocator, sizeof(void *) * n, 0);
  CFDictionaryGetKeysAndValues(dictionary, keys, values);
  CFArrayRef keysArray = CFArrayCreate(connection->allocator, keys, n, &kCFTypeArrayCallBacks);
  CFArrayRef valuesArray = CFArrayCreate(connection->allocator, values, n, &kCFTypeArrayCallBacks);
  CFStringRef keysString = CFStringCreateByCombiningStrings(connection->allocator, keysArray, CFSTR(", "));
  const char *valuesCString = _SQLite3CreateValuesPlaceholderCString(connection->allocator, n);
  CFStringRef sql = CFStringCreateWithFormat(connection->allocator, NULL, CFSTR("INSERT INTO %@(%@) VALUES(%s)"), table, keysString, valuesCString);
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  if (statement) {
    SQLite3StatementBindArray(statement, valuesArray);
    status = SQLite3StatementStep(statement);
    SQLite3StatementReset(statement);
    SQLite3StatementClearBindings(statement);
    SQLite3StatementFinalize(statement);
    SQLite3StatementRelease(statement);
  }
  CFRelease(sql);
  CFAllocatorDeallocate(connection->allocator, (void *)valuesCString);
  CFRelease(keysString);
  CFRelease(valuesArray);
  CFRelease(keysArray);
  CFAllocatorDeallocate(connection->allocator, keys);
  CFAllocatorDeallocate(connection->allocator, values);
  return status;
}
