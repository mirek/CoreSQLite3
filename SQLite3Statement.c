//
// SQLite3Statement.c
// CoreSQLite3
//
// Copyright 2011 Mirek Rusin <mirek [at] me [dot] com>
//

#include "SQLite3Statement.h"

#pragma mark Lifecycle

inline SQLite3StatementRef _SQLite3StatementCreate(CFAllocatorRef allocator, SQLite3ConnectionRef connection, CFStringRef sql, CFErrorRef *error) {
  SQLite3StatementRef statement = NULL;
  if (connection && sql) {
    statement = CFAllocatorAllocate(allocator, sizeof(__SQLite3Statement), 0);
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
  }
  return statement;
}

inline SQLite3StatementRef SQLite3StatementCreateWithError(SQLite3ConnectionRef connection, CFStringRef sql, CFErrorRef *error) {
  SQLite3StatementRef statement = NULL;
  if (connection) {
    if (sql) {
      statement = _SQLite3StatementCreate(connection->allocator, connection, sql, error);
    }
  }
  return statement;
}

inline SQLite3StatementRef SQLite3StatementCreate(SQLite3ConnectionRef connection, CFStringRef sql) {
  return SQLite3StatementCreateWithError(connection, sql, NULL);
}

inline SQLite3StatementRef SQLite3StatementCreateWithArrayBindingsReturningStatus(SQLite3ConnectionRef connection, CFStringRef sql, CFArrayRef array, SQLite3Status *outStatus) {
  SQLite3Status status = kSQLite3StatusError;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  if (statement) {
    status = SQLite3StatementBindArray(statement, array);
  }
  if (outStatus) {
    *outStatus = status;
  }
  return statement;
}

inline SQLite3StatementRef SQLite3StatementCreateWithArrayBindings(SQLite3ConnectionRef connection, CFStringRef sql, CFArrayRef array) {
  return SQLite3StatementCreateWithArrayBindingsReturningStatus(connection, sql, array, NULL);
}

inline SQLite3StatementRef SQLite3StatementCreateWithDictionaryBindingsReturningStatus(SQLite3ConnectionRef connection, CFStringRef sql, CFDictionaryRef dictionary, SQLite3Status *outStatus) {
  SQLite3Status status = kSQLite3StatusError;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  if (statement) {
    status = SQLite3StatementBindDictionary(statement, dictionary);
  }
  if (outStatus) {
    *outStatus = status;
  }
  return statement;
}

inline SQLite3StatementRef SQLite3StatementCreateWithDictionaryBindings(SQLite3ConnectionRef connection, CFStringRef sql, CFDictionaryRef dictionary) {
  return SQLite3StatementCreateWithDictionaryBindingsReturningStatus(connection, sql, dictionary, NULL);
}

inline SQLite3StatementRef SQLite3StatementCreateWithBundleResource(SQLite3ConnectionRef connection, CFBundleRef bundle, CFStringRef type, CFStringRef name, CFStringRef subdir) {
  SQLite3StatementRef statement = NULL;
  if (connection) {
    SInt32 errorCode = 0;
    CFDictionaryRef properties = NULL;
    CFDataRef data = NULL;
    CFURLRef url = CFBundleCopyResourceURL(bundle, name, type, subdir);
    if (url) {
      if (CFURLCreateDataAndPropertiesFromResource(connection->allocator, url, &data, &properties, NULL, &errorCode)) {
        CFStringRef sql = CFStringCreateWithBytes(connection->allocator, CFDataGetBytePtr(data), CFDataGetLength(data), kCFStringEncodingUTF8, 0);
        if (sql) {
          statement = SQLite3StatementCreate(connection, sql);
          CFRelease(sql);
        }
        CFRelease(data);
        if (properties)
          CFRelease(properties);
      } else {
        // TODO: Error
      }
      CFRelease(url);
    }
  }
  return statement;
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

inline CFStringRef SQLite3StatementCreateSQLString(SQLite3StatementRef statement) {
  CFStringRef sql = NULL;
  if (statement) {
    sql = CFStringCreateWithCString(statement->allocator, sqlite3_sql(statement->stmt), kCFStringEncodingUTF8);
  }
  return sql;
}

#pragma mark Bindings
#pragma mark -

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

inline CFStringRef SQLite3StatementCreateBindParameterNameWithIndex(SQLite3StatementRef statement, CFIndex index, bool withoutSpecialCharacter) {
  CFStringRef name = NULL;
  const char *name_ = sqlite3_bind_parameter_name(statement->stmt, (int)index);
  if (name_)
    name = CFStringCreateWithCString(statement->allocator, name_ + (int)withoutSpecialCharacter, kCFStringEncodingUTF8);
  return name;
}

#pragma mark Index based bindings

inline SQLite3Status SQLite3StatementBindNULL(SQLite3StatementRef statement, CFIndex index) {
  return sqlite3_bind_null(statement->stmt, (int)index);
}

inline SQLite3Status SQLite3StatementBindBool(SQLite3StatementRef statement, CFIndex index, bool value) {
  return sqlite3_bind_int(statement->stmt, (int)index, (int)value);
}

inline SQLite3Status SQLite3StatementBindInt32(SQLite3StatementRef statement, CFIndex index, int32_t value) {
  return sqlite3_bind_int(statement->stmt, (int)index, value);
}

inline SQLite3Status SQLite3StatementBindInt64(SQLite3StatementRef statement, CFIndex index, int64_t value) {
  return sqlite3_bind_int64(statement->stmt, (int)index, value);
}

inline SQLite3Status SQLite3StatementBindDouble(SQLite3StatementRef statement, CFIndex index, double_t value) {
  return sqlite3_bind_double(statement->stmt, (int)index, value);
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

inline SQLite3Status SQLite3StatementBindAbsoluteTime(SQLite3StatementRef statement, CFIndex index, CFAbsoluteTime value) {
  return SQLite3StatementBindDouble(statement, index, value - kCFAbsoluteTimeIntervalSince1970);
}

inline SQLite3Status SQLite3StatementBindDate(SQLite3StatementRef statement, CFIndex index, CFDateRef value) {
  if (value)
    return SQLite3StatementBindAbsoluteTime(statement, index, CFDateGetAbsoluteTime(value));
  else
    return SQLite3StatementBindNULL(statement, index);
}

inline SQLite3Status SQLite3StatementBindData(SQLite3StatementRef statement, CFIndex index, CFDataRef value) {
  int result;
  if (value)
    result = sqlite3_bind_blob(statement->stmt, (int)index, CFDataGetBytePtr(value), (int)CFDataGetLength(value), SQLITE_TRANSIENT);
  else
    result = SQLite3StatementBindNULL(statement, index);
  return result;
}

inline SQLite3Status SQLite3StatementBindCFType(SQLite3StatementRef statement, CFIndex index, CFTypeRef value) {
  SQLite3Status status = kSQLite3StatusError;
  if (value) {
    CFTypeID valueTypeID = CFGetTypeID(value);
    if (CFStringGetTypeID() == valueTypeID)
      status = SQLite3StatementBindString(statement, index, (CFStringRef)value);
    else if (CFDataGetTypeID() == valueTypeID)
      status = SQLite3StatementBindData(statement, index, (CFDataRef)value);
//  else if (CGImageGetTypeID() == valueTypeID)
//    status = SQLite3StatementBindImage(statement, index, (CGImageRef)value);
    else if (CFDateGetTypeID() == valueTypeID)
      status = SQLite3StatementBindDate(statement, index, (CFDateRef)value);
    else if (CFNumberGetTypeID() == valueTypeID)
      status = SQLite3StatementBindNumber(statement, index, (CFNumberRef)value);
    else
      status = kSQLite3StatusError;
  } else {
    status = SQLite3StatementBindNULL(statement, index);
  }
  return status;
}

#pragma mark Name based bindings

inline SQLite3Status SQLite3StatementBindNULLWithName(SQLite3StatementRef statement, CFStringRef name) {
  return SQLite3StatementBindNULL(statement, SQLite3StatementGetBindParameterIndexWithName(statement, name));
}

inline SQLite3Status SQLite3StatementBindBoolWithName(SQLite3StatementRef statement, CFStringRef name, bool value) {
  return SQLite3StatementBindBool(statement, SQLite3StatementGetBindParameterIndexWithName(statement, name), value);
}

inline SQLite3Status SQLite3StatementBindInt32WithName(SQLite3StatementRef statement, CFStringRef name, int32_t value) {
  return SQLite3StatementBindInt32(statement, SQLite3StatementGetBindParameterIndexWithName(statement, name), value);
}

inline SQLite3Status SQLite3StatementBindInt64WithName(SQLite3StatementRef statement, CFStringRef name, int64_t value) {
  return SQLite3StatementBindInt64(statement, SQLite3StatementGetBindParameterIndexWithName(statement, name), value);
}

inline SQLite3Status SQLite3StatementBindDoubleWithName(SQLite3StatementRef statement, CFStringRef name, double_t value) {
  return SQLite3StatementBindDouble(statement, SQLite3StatementGetBindParameterIndexWithName(statement, name), value);
}

inline SQLite3Status SQLite3StatementBindCFTypeWithName(SQLite3StatementRef statement, CFStringRef name, CFTypeRef value) {
  return SQLite3StatementBindCFType(statement, SQLite3StatementGetBindParameterIndexWithName(statement, name), value);
}

inline SQLite3Status SQLite3StatementBindNumberWithName(SQLite3StatementRef statement, CFStringRef name, CFNumberRef value) {
  return SQLite3StatementBindNumber(statement, SQLite3StatementGetBindParameterIndexWithName(statement, name), value);
}

inline SQLite3Status SQLite3StatementBindStringWithName(SQLite3StatementRef statement, CFStringRef name, CFStringRef value) {
  return SQLite3StatementBindString(statement, SQLite3StatementGetBindParameterIndexWithName(statement, name), value);
}

inline SQLite3Status SQLite3StatementBindDateWithName(SQLite3StatementRef statement, CFStringRef name, CFDateRef value) {
  return SQLite3StatementBindDate(statement, SQLite3StatementGetBindParameterIndexWithName(statement, name), value);
}

inline SQLite3Status SQLite3StatementBindDataWithName(SQLite3StatementRef statement, CFStringRef name, CFDataRef value) {
  return SQLite3StatementBindData(statement, SQLite3StatementGetBindParameterIndexWithName(statement, name), value);
}

inline SQLite3Status SQLite3StatementBindPropertyListWithName(SQLite3StatementRef statement, CFStringRef name, CFPropertyListRef value, CFPropertyListFormat format) {
  return SQLite3StatementBindPropertyList(statement, SQLite3StatementGetBindParameterIndexWithName(statement, name), value, format);
}

#pragma mark Array and dictionary bindings

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

#pragma mark Resultsets
#pragma mark -

inline CFIndex SQLite3StatementGetColumnCount(SQLite3StatementRef statement) {
  return sqlite3_column_count(statement->stmt);
}

inline SQLite3ColumnType SQLite3StatementGetColumnType(SQLite3StatementRef statement, CFIndex index) {
  return sqlite3_column_type(statement->stmt, (int)index);
}

inline SQLite3ColumnType SQLite3StatementGetColumnTypeWithName(SQLite3StatementRef statement, CFStringRef name) {
  return SQLite3StatementGetColumnType(statement, SQLite3StatementGetColumnIndexWithName(statement, name));
}

inline CFStringRef SQLite3StatementCreateColumnNameWithIndex(SQLite3StatementRef statement, CFIndex index) {
  CFStringRef name = NULL;
  const char *name_ = sqlite3_column_name(statement->stmt, (int)index);
  if (name_) {
    name = CFStringCreateWithCString(statement->allocator, name_, kCFStringEncodingUTF8);
  }
  return name;
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

#pragma mark Values

inline bool SQLite3StatementGetBOOLWithColumn(SQLite3StatementRef statement, CFIndex index) {
  return sqlite3_column_int(statement->stmt, (int)index) != 0;
}

inline int32_t SQLite3StatementGetInt32WithColumn(SQLite3StatementRef statement, CFIndex index) {
  return (int32_t)sqlite3_column_int(statement->stmt, (int)index);
}

inline int64_t SQLite3StatementGetInt64WithColumn(SQLite3StatementRef statement, CFIndex index) {
  return (int64_t)sqlite3_column_int64(statement->stmt, (int)index);
}

inline double_t SQLite3StatementGetDoubleWithColumn(SQLite3StatementRef statement, CFIndex index) {
  return (double_t)sqlite3_column_double(statement->stmt, (int)index);
}

#pragma mark Objects

// Returns NULL, kCFNull, CFNumberRef, CFDataRef or CFStringRef. If you need correct CFTypeRef, test the result
// for NULL.
inline CFTypeRef SQLite3StatementCreateCFTypeWithColumn(SQLite3StatementRef statement, CFIndex index) {
  CFTypeRef value = NULL;
  if (statement) {
    switch (SQLite3StatementGetColumnType(statement, index)) {
      case kSQLite3TypeNULL:
        value = kCFNull;
        break;
      case kSQLite3TypeInteger:
      case kSQLite3TypeFloat:
        value = SQLite3StatementCreateNumberWithColumn(statement, index);
        break;
      case kSQLite3TypeString:
        value = SQLite3StatementCreateStringWithColumn(statement, index);
        break;
      case kSQLite3TypeData:
        value = SQLite3StatementCreateDataWithColumn(statement, index);
        break;
    }
  }
  return value;
}

inline CFNumberRef SQLite3StatementCreateNumberWithColumn(SQLite3StatementRef statement, CFIndex index) {
  CFNumberRef value = NULL;
  if (statement) {
    switch (SQLite3StatementGetColumnType(statement, index)) {
        
      case kSQLite3TypeNULL: {
        int zero = 0;
        value = CFNumberCreate(statement->allocator, kCFNumberIntType, (const void *)&zero);
        break;
      }
        
      case kSQLite3TypeInteger: {
        long long value_ = (long long)SQLite3StatementGetInt64WithColumn(statement, index);
        value = CFNumberCreate(statement->allocator, kCFNumberLongLongType, (const void *)&value_);
        break;
      }
        
      case kSQLite3TypeFloat: {
        double_t value_ = SQLite3StatementGetDoubleWithColumn(statement, index);
        value = CFNumberCreate(statement->allocator, kCFNumberDoubleType, (const void *)&value_);
        break;
      }
        
      case kSQLite3TypeString: {
        // TODO: parse?
        break;
      }
        
      case kSQLite3TypeData:
        break;
    }
  }
  return value;
}

inline CFStringRef SQLite3StatementCreateStringWithColumn(SQLite3StatementRef statement, CFIndex index) {
  return CFStringCreateWithBytes(statement->allocator, sqlite3_column_text(statement->stmt, (int)index), sqlite3_column_bytes(statement->stmt, (int)index), kCFStringEncodingUTF8, 0);
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

inline CFDataRef SQLite3StatementCreateDataWithColumn(SQLite3StatementRef statement, CFIndex index) {
  return CFDataCreate(statement->allocator, sqlite3_column_blob(statement->stmt, (int)index), sqlite3_column_bytes(statement->stmt, (int)index));
}

#pragma mark Name based values

inline bool SQLite3StatementGetBOOLWithColumnName(SQLite3StatementRef statement, CFStringRef name) {
  return SQLite3StatementGetBOOLWithColumn(statement, SQLite3StatementGetColumnIndexWithName(statement, name));
}

inline int32_t SQLite3StatementGetInt32WithColumnName(SQLite3StatementRef statement, CFStringRef name) {
  return SQLite3StatementGetInt32WithColumn(statement, SQLite3StatementGetColumnIndexWithName(statement, name));
}

inline int64_t SQLite3StatementGetInt64WithColumnName(SQLite3StatementRef statement, CFStringRef name) {
  return SQLite3StatementGetInt64WithColumn(statement, SQLite3StatementGetColumnIndexWithName(statement, name));
}

inline double_t SQLite3StatementGetDoubleWithColumnName(SQLite3StatementRef statement, CFStringRef name) {
  return SQLite3StatementGetDoubleWithColumn(statement, SQLite3StatementGetColumnIndexWithName(statement, name));
}

#pragma mark Name based objects

inline CFTypeRef SQLite3StatementCreateCFTypeWithColumnName(SQLite3StatementRef statement, CFStringRef name) {
  return SQLite3StatementCreateCFTypeWithColumn(statement, SQLite3StatementGetColumnIndexWithName(statement, name));
}

inline CFNumberRef SQLite3StatementCreateNumberWithColumnName(SQLite3StatementRef statement, CFStringRef name) {
  return SQLite3StatementCreateNumberWithColumn(statement, SQLite3StatementGetColumnIndexWithName(statement, name));
}

inline CFStringRef SQLite3StatementCreateStringWithColumnName(SQLite3StatementRef statement, CFStringRef name) {
  return SQLite3StatementCreateStringWithColumn(statement, SQLite3StatementGetColumnIndexWithName(statement, name));
}

inline CFDateRef SQLite3StatementCreateDateWithColumnName(SQLite3StatementRef statement, CFStringRef name) {
  return SQLite3StatementCreateDateWithColumn(statement, SQLite3StatementGetColumnIndexWithName(statement, name));
}

inline CFDataRef SQLite3StatementCreateDataWithColumnName(SQLite3StatementRef statement, CFStringRef name) {
  return SQLite3StatementCreateDataWithColumn(statement, SQLite3StatementGetColumnIndexWithName(statement, name));
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

inline CFArrayRef SQLite3StatementCreateArrayForAllColumns(SQLite3StatementRef statement) {
  CFArrayRef array = NULL;
  if (statement) {
    CFIndex count = SQLite3StatementGetColumnCount(statement);
    if (count > 0) {
      const void **values = CFAllocatorAllocate(statement->allocator, sizeof(const void *) * count, 0);
      if (values) {
        for (CFIndex i = 0; i < count; i++) {
          values[i] = SQLite3StatementCreateCFTypeWithColumn(statement, i);
        }
        array = CFArrayCreate(statement->allocator, values, count, &kCFTypeArrayCallBacks);
        for (CFIndex i = 0; i < count; i++) {
          if (values[i])
            CFRelease(values[i]);
        }
        CFAllocatorDeallocate(statement->allocator, values);
      }
    }
  }
  return array;
}

inline CFDictionaryRef SQLite3StatementCreateDictionaryForAllColumns(SQLite3StatementRef statement) {
  CFDictionaryRef dictionary = NULL;
  CFIndex count = SQLite3StatementGetColumnCount(statement);
  if (count > 0) {
    CFStringRef *keys   = CFAllocatorAllocate(statement->allocator, sizeof(CFStringRef) * count, 0);
    CFStringRef *values = CFAllocatorAllocate(statement->allocator, sizeof(CFStringRef) * count, 0);
    for (CFIndex i = 0; i < count; i++) {
      keys[i] = SQLite3StatementCreateColumnNameWithIndex(statement, i);
      values[i] = SQLite3StatementCreateCFTypeWithColumn(statement, i);
    }
    dictionary = CFDictionaryCreate(statement->allocator, (const void **)keys, (const void **)values, count, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    for (CFIndex i = 0; i < count; i++) {
      if (keys[i])
        CFRelease(keys[i]);
      if (values[i])
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

#pragma mark Resultsets with column name

//inline CGImageRef SQLite3StatementCreateImageWithColumnName(SQLite3StatementRef statement, CFStringRef name) {
//  return SQLite3StatementCreateImageWithColumn(statement, SQLite3StatementGetColumnIndexWithName(statement, name));
//}

// Helper function used to create "?,?,...\0" cstring value placeholders.
// TODO: Rename to SQLite3CreateCStringValuePlaceholdersWithCount(...)
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
inline SQLite3Status SQLite3InsertWithTableNameAndDictionary(SQLite3ConnectionRef connection, CFStringRef table, CFDictionaryRef dictionary) {
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
