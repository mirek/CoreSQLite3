//
// SQLite3Connection.c
// CoreSQLite3
//
// Copyright 2011 Mirek Rusin <mirek [at] me [dot] com>
//

#include "SQLite3Connection.h"

#define kSQlite3UserCallbacksMaxCount 64

#pragma Lifecycle

inline SQLite3ConnectionRef _SQLite3ConnectionCreate(CFAllocatorRef allocator, CFStringRef path, SQLite3OpenOptions flags, const char *zVfs) {
  SQLite3ConnectionRef connection = CFAllocatorAllocate(allocator, sizeof(__SQLite3Connection), 0);
  if (connection) {
    connection->allocator = allocator ? CFRetain(allocator) : NULL;
    connection->retainCount = 1;
    
    // Please note update hook will be set automatically when adding update callbacks
    // When removing callbacks, if the number of callbacks goes back to 0, we're removing
    // default hook.
    connection->__updateCallbacksWithUserInfoCount = 0;
    
    // Default date formatter
    connection->defaultDateFormatter = CFDateFormatterCreate(allocator, NULL, kCFDateFormatterNoStyle, kCFDateFormatterNoStyle);
    CFDateFormatterSetFormat(connection->defaultDateFormatter, CFSTR("yyyy-MM-dd HH:mm:ss.SSS"));
    
    __SQLite3UTF8String utf8Path = __SQLite3UTF8StringMake(connection->allocator, path);
    if (kSQLite3StatusOK == sqlite3_open_v2(__SQLite3UTF8StringGetBuffer(utf8Path), &connection->db, flags, zVfs)) {
        // All ok
    } else {
      // Move to SQLite3ConnectionCreateWithError, otherwise we don't know what happened
      connection = SQLite3ConnectionRelease(connection);
    }
    __SQLite3UTF8StringDestroy(utf8Path);
  }
  return connection;
}

inline SQLite3ConnectionRef SQLite3ConnectionCreate(CFAllocatorRef allocator, CFStringRef path, SQLite3OpenOptions flags, const char *zVfs) {
  return _SQLite3ConnectionCreate(allocator, path, flags, zVfs);
}

// TODO: Open in read-only mode
// Pass bundle = NULL to use the main bundle
void SQLite3OpenResource(CFAllocatorRef allocator, CFBundleRef bundle, CFStringRef resourceName, CFStringRef resourceType, CFStringRef subDirName, sqlite3 **db) {
//  if (bundle == NULL)
//    bundle = CFBundleGetMainBundle();
//  CFURLRef url = CFBundleCopyResourceURL(bundle, resourceName, resourceType, subDirName);
//  if (url) {
//    CFShow(url);
//    if (sqlite3_open_v2(CFStringGetCStringPtr(CFURLGetString(url), kCFStringEncodingUTF8), db, SQLITE_OPEN_READONLY, NULL)) {
//      printf("ERROR %s\n", (const unsigned char *)sqlite3_errmsg(*db));
//    }
//    CFRelease(url);
//  }
}

inline SQLite3Status SQLite3ConnectionClose(SQLite3ConnectionRef connection) {
  return connection ? sqlite3_close(connection->db) : kSQLite3StatusOK;
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
  CFAllocatorRef allocator = NULL;
  const char *errmsg = NULL;
  int errcode = kSQLite3StatusOK;
  if (connection == NULL) {
    errmsg = "SQLite3ConnectionRef not allocated";
    errcode = kSQLite3StatusError;
  } else {
    allocator = connection->allocator;
    if (SQLite3ConnectionHasError(connection)) {
      errmsg = sqlite3_errmsg(connection->db);
      errcode = sqlite3_errcode(connection->db);
    }
  }
  if (errmsg) {
    CFStringRef keys[1] = { kCFErrorLocalizedDescriptionKey };
    CFStringRef values[1] = { CFStringCreateWithCString(allocator, errmsg, kCFStringEncodingUTF8) };
    CFDictionaryRef userInfo = CFDictionaryCreate(allocator, (void *)keys, (void *)values, 1, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    error = CFErrorCreate(allocator, CFSTR("com.github.mirek.CoreSQLite3"), errcode, userInfo);
    CFRelease(userInfo);
    CFRelease(values[0]);
  }
  return error;
}

inline SQLite3Status SQLite3ConnectionGetStatus(SQLite3ConnectionRef connection) {
  return connection ? sqlite3_errcode(connection->db) : kSQLite3StatusError;
}

// If the connection is NULL or sqlite3 connection has an error, return YES. Otherwise return NO.
inline bool SQLite3ConnectionHasError(SQLite3ConnectionRef connection) {
  return connection == NULL ? 1 : sqlite3_errcode(connection->db) != kSQLite3StatusOK;
}

// Return NULL if the connection is not allocated. Otherwise return sqlite3 connection.
inline sqlite3 *SQLite3ConnectionGetConnection(SQLite3ConnectionRef connection) {
  return connection ? connection->db : NULL;
}

#pragma mark Function and aggregate registration

SQLite3Status SQLite3ConnectionRegisterFunction(SQLite3ConnectionRef connection, CFStringRef name, CFIndex argc, void (*f)(sqlite3_context *, int, sqlite3_value **)) {
  SQLite3Status status = kSQLite3StatusError;
  if (connection) {
    if (name) {
      __SQLite3UTF8String utf8Name = __SQLite3UTF8StringMake(connection->allocator, name);
      status = sqlite3_create_function_v2(connection->db, __SQLite3UTF8StringGetBuffer(utf8Name), (int)argc, SQLITE_ANY, NULL, f, NULL, NULL, NULL);
      __SQLite3UTF8StringDestroy(utf8Name);
    }
  }
  return status;
}

SQLite3Status SQLite3ConnectionUnregisterFunction(SQLite3ConnectionRef connection, CFStringRef name, CFIndex argc) {
  return SQLite3ConnectionRegisterFunction(connection, name, argc, NULL);
}

#pragma mark Resultset utility functions

inline SQLite3Status SQLite3ConnectionExecuteWithContentsOfURL(SQLite3ConnectionRef connection, CFURLRef url) {
  SQLite3Status status = kSQLite3StatusError;
  SInt32 errorCode = 0;
  CFDictionaryRef properties = NULL;
  CFDataRef data = NULL;
  if (CFURLCreateDataAndPropertiesFromResource(connection->allocator, url, &data, &properties, NULL, &errorCode)) {
    CFStringRef sql = CFStringCreateWithBytes(connection->allocator, CFDataGetBytePtr(data), CFDataGetLength(data), kCFStringEncodingUTF8, 0);
    status = SQLite3ConnectionExecute(connection, sql);
    CFRelease(sql);
  } else {
    // TODO: Error
  }
  return status;
}

inline SQLite3Status SQLite3ConnectionExecutev(SQLite3ConnectionRef connection, CFStringRef sql, ...) {
  va_list arguments;
  va_start(arguments, sql);
  CFStringRef sqlv = CFStringCreateWithFormatAndArguments(connection->allocator, NULL, sql, arguments);
  va_end(arguments);
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sqlv);
  SQLite3Status status = SQLite3StatementStep(statement);
  SQLite3StatementReset(statement);
  SQLite3StatementRelease(statement);
  CFRelease(sqlv);
  return status;
}

inline SQLite3Status SQLite3ConnectionExecute(SQLite3ConnectionRef connection, CFStringRef sql) {
  SQLite3Status status = kSQLite3StatusError;
  if (sql) {
    __SQLite3UTF8String utf8Sql = __SQLite3UTF8StringMake(connection->allocator, sql);
    status = sqlite3_exec(connection->db, __SQLite3UTF8StringGetBuffer(utf8Sql), NULL, NULL, NULL);
    __SQLite3UTF8StringDestroy(utf8Sql);
  }
  return status;
}

inline SQLite3Status SQLite3ConnectionExecuteWithArrayBindingsAndError(SQLite3ConnectionRef connection, CFStringRef sql, CFArrayRef values, CFErrorRef *error) {
  SQLite3Status status = kSQLite3StatusError;
  SQLite3StatementRef statement = SQLite3StatementCreateWithError(connection, sql, error);
  if (statement) {
    SQLite3StatementBindArray(statement, values);
    status = SQLite3StatementStep(statement);
    SQLite3StatementReset(statement);
    SQLite3StatementClearBindings(statement);
    SQLite3StatementFinalize(statement);
    SQLite3StatementRelease(statement);
  }
  return status;
}

inline SQLite3Status SQLite3ConnectionExecuteWithArrayBindings(SQLite3ConnectionRef connection, CFStringRef sql, CFArrayRef values) {
  return SQLite3ConnectionExecuteWithArrayBindingsAndError(connection, sql, values, NULL);
}

inline SQLite3Status SQLite3ConnectionExecuteWithDictionaryBindings(SQLite3ConnectionRef connection, CFStringRef sql, CFDictionaryRef keyValuePairs) {
  SQLite3Status status = kSQLite3StatusError;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  if (statement) {
    SQLite3StatementBindDictionary(statement, keyValuePairs);
    status = SQLite3StatementStep(statement);
    SQLite3StatementReset(statement);
    SQLite3StatementClearBindings(statement);
    SQLite3StatementFinalize(statement);
    SQLite3StatementRelease(statement);
  }
  return status;
}

inline int32_t SQLite3ConnectionGetInt32WithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  int32_t value = 0;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
    value = SQLite3StatementGetInt32WithColumn(statement, 0);
  SQLite3StatementReset(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return value;
}

inline int64_t SQLite3ConnectionGetInt64WithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  int64_t value = 0;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
    value = SQLite3StatementGetInt64WithColumn(statement, 0);
  SQLite3StatementReset(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return value;
}

inline double_t SQLite3ConnectionGetDoubleWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  double_t value = 0.0;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
    value = SQLite3StatementGetDoubleWithColumn(statement, 0);
  SQLite3StatementReset(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return value;
}

inline int64_t SQLite3ConnectionGetLastInsertedRowId(SQLite3ConnectionRef connection) {
  return SQLite3ConnectionGetInt64WithQuery(connection, CFSTR("select last_insert_rowid()"));
}

inline bool SQLite3ConnectionGetBOOLWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  bool value = 0;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
    value = SQLite3StatementGetBOOLWithColumn(statement, 0);
  SQLite3StatementReset(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return value;
}

inline CFTypeRef SQLite3ConnectionCreateCFTypeWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  CFTypeRef value = NULL;
  if (connection) {
    if (sql) {
      SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
      if (statement) {
        if (kSQLite3StatusRow == SQLite3StatementStep(statement)) {
          switch (SQLite3StatementGetColumnType(statement, 0)) {
            case kSQLite3TypeNULL:
              value = kCFNull;
              break;
              
            case kSQLite3TypeInteger:
            case kSQLite3TypeFloat:
              value = SQLite3StatementCreateNumberWithColumn(statement, 0);
              break;
              
            case kSQLite3TypeString:
              value = SQLite3StatementCreateStringWithColumn(statement, 0);
              break;
              
            case kSQLite3TypeData:
              value = SQLite3StatementCreateDataWithColumn(statement, 0);
              break;
          }
        }
        SQLite3StatementReset(statement);
        SQLite3StatementFinalize(statement);
        SQLite3StatementRelease(statement);
      }
    }
  }
  return value;
}

inline CFNumberRef SQLite3ConnectionCreateNumberWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  CFNumberRef value = NULL;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
    value = SQLite3StatementCreateNumberWithColumn(statement, 0);
  SQLite3StatementReset(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return value;
}

inline CFStringRef SQLite3ConnectionCreateStringWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  CFStringRef value = NULL;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
    value = SQLite3StatementCreateStringWithColumn(statement, 0);
  SQLite3StatementReset(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return value;
}

inline CFDataRef SQLite3ConnectionCreateDataWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  CFDataRef value = NULL;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
    value = SQLite3StatementCreateDataWithColumn(statement, 0);
  SQLite3StatementReset(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return value;
}

//inline CGImageRef SQLite3ConnectionCreateImageWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
//  CGImageRef value = NULL;
//  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
//  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
//    value = SQLite3StatementCreateImageWithColumn(statement, 0);
//  SQLite3StatementReset(statement);
//  SQLite3StatementFinalize(statement);
//  SQLite3StatementRelease(statement);
//  return value;
//}

inline CFDateRef SQLite3ConnectionCreateDateWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  CFDateRef value = NULL;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
    value = SQLite3StatementCreateDateWithColumn(statement, 0);
  SQLite3StatementReset(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return value;
}

inline CFDictionaryRef SQLite3ConnectionCreateDictionaryForAllColumnsWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  CFDictionaryRef value = NULL;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
    value = SQLite3StatementCreateDictionaryForAllColumns(statement);
  SQLite3StatementReset(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return value;
}

inline CFPropertyListRef SQLite3ConnectionCreatePropertyListWithQuery(SQLite3ConnectionRef connection, CFStringRef sql, CFOptionFlags options, CFPropertyListFormat *format) {
  CFPropertyListRef value = NULL;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
  if (statement) {
    if (kSQLite3StatusRow == SQLite3StatementStep(statement))
      value = SQLite3StatementCreatePropertyListWithColumn(statement, 0, options, format);
    SQLite3StatementReset(statement);
    SQLite3StatementFinalize(statement);
    SQLite3StatementRelease(statement);
  }
  return value;
}

inline CFArrayRef SQLite3ConnectionCreateArrayForFirstRowWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  CFArrayRef array = NULL;
  if (connection) {
    if (sql) {
      SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
      if (statement) {
        if (kSQLite3StatusRow == SQLite3StatementStep(statement)) {
          array = SQLite3StatementCreateArrayForAllColumns(statement);
        }
        SQLite3StatementFinalize(statement);
        SQLite3StatementRelease(statement);
      }
    }
  }
  return array;
}

CFDictionaryRef SQLite3ConnectionCreateDictionaryForFirstRowWithQuery(SQLite3ConnectionRef connection, CFStringRef sql) {
  CFDictionaryRef dictionary = NULL;
  if (connection) {
    if (sql) {
      SQLite3StatementRef statement = SQLite3StatementCreate(connection, sql);
      if (statement) {
        if (kSQLite3StatusRow == SQLite3StatementStep(statement)) {
          dictionary = SQLite3StatementCreateDictionaryForAllColumns(statement);
        }
        SQLite3StatementFinalize(statement);
        SQLite3StatementRelease(statement);
      }
    }
  }
  return dictionary;
}

bool SQLite3ConnectionDoesTableOrViewHaveRowWithID(SQLite3ConnectionRef connection, CFStringRef name, int64_t rowID) {
  bool result = 0;
  if (connection) {
    if (name) {
      CFStringRef sql = CFStringCreateWithFormat(connection->allocator, NULL, CFSTR("select 1 from %@ where _rowid_ = %ll"), name, rowID);
      if (sql) {
        result = SQLite3ConnectionGetBOOLWithQuery(connection, sql);
        CFRelease(sql);
      }
    }
  }
  return result;
}

CFArrayRef SQLite3ConnectionCreateArrayWithTableOrViewForRowID(SQLite3ConnectionRef connection, CFStringRef name, int64_t rowID) {
  CFArrayRef array = NULL;
  if (connection) {
    if (name) {
      CFStringRef sql = CFStringCreateWithFormat(connection->allocator, NULL, CFSTR("select * from %@ where _rowid_ = %ll"), name, rowID);
      if (sql) {
        array = SQLite3ConnectionCreateArrayForFirstRowWithQuery(connection, sql);
        CFRelease(sql);
      }
    }
  }
  return array;
}

CFDictionaryRef SQLite3ConnectionCreateDictionaryWithTableOrViewForRowID(SQLite3ConnectionRef connection, CFStringRef name, int64_t rowID) {
  CFDictionaryRef dictionary = NULL;
  if (connection) {
    if (name) {
      CFStringRef sql = CFStringCreateWithFormat(connection->allocator, NULL, CFSTR("select * from %@ where _rowid_ = %ll"), name, rowID);
      if (sql) {
        dictionary = SQLite3ConnectionCreateDictionaryForFirstRowWithQuery(connection, sql);
        CFRelease(sql);
      }
    }
  }
  return dictionary;
}

#pragma Utility functions

inline SQLite3Status SQLite3ConnectionDropTable(SQLite3ConnectionRef connection, CFStringRef name) {
  return SQLite3ConnectionExecutev(connection, CFSTR("drop table %@;"), name); // TODO:
}

inline SQLite3Status SQLite3ConnectionDropView(SQLite3ConnectionRef connection, CFStringRef name) {
  return SQLite3ConnectionExecutev(connection, CFSTR("drop view %@;"), name); // TODO:
}

inline SQLite3Status SQLite3ConnectionDropIndex(SQLite3ConnectionRef connection, CFStringRef name) {
  return SQLite3ConnectionExecutev(connection, CFSTR("drop index %@;"), name); // TODO:
}

inline SQLite3Status SQLite3ConnectionDropTableIfExists(SQLite3ConnectionRef connection, CFStringRef name) {
  return SQLite3ConnectionExecutev(connection, CFSTR("drop table if exists %@;"), name); // TODO:
}

inline SQLite3Status SQLite3ConnectionDropViewIfExists(SQLite3ConnectionRef connection, CFStringRef name) {
  return SQLite3ConnectionExecutev(connection, CFSTR("drop view if exists %@;"), name); // TODO:
}

inline SQLite3Status SQLite3ConnectionDropIndexIfExists(SQLite3ConnectionRef connection, CFStringRef name) {
  return SQLite3ConnectionExecutev(connection, CFSTR("drop index if exists %@;"), name); // TODO:
}

inline bool SQLite3ConnectionDoesTableExist(SQLite3ConnectionRef connection, CFStringRef name) {
  bool exists = 0;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("select count(*) from sqlite_master where type = 'table' and name = ?"));
  SQLite3StatementBindString(statement, 1, name);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
    exists = SQLite3StatementGetBOOLWithColumn(statement, 0);
  SQLite3StatementReset(statement);
  SQLite3StatementClearBindings(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return exists;
}

inline bool SQLite3ConnectionDoesViewExist(SQLite3ConnectionRef connection, CFStringRef name) {
  bool exists = 0;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("select count(*) from sqlite_master where type = 'view' and name = ?"));
  SQLite3StatementBindString(statement, 1, name);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
    exists = SQLite3StatementGetBOOLWithColumn(statement, 0);
  SQLite3StatementReset(statement);
  SQLite3StatementClearBindings(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return exists;
}

inline bool SQLite3ConnectionDoesTableOrViewExist(SQLite3ConnectionRef connection, CFStringRef name) {
  bool exists = 0;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("select count(*) from sqlite_master where (type = 'table' or type = 'view') and name = ?"));
  SQLite3StatementBindString(statement, 1, name);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement)) {
    exists = SQLite3StatementGetBOOLWithColumn(statement, 0);
  }
  SQLite3StatementReset(statement);
  SQLite3StatementClearBindings(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return exists;
}

inline bool SQLite3ConnectionDoesIndexExist(SQLite3ConnectionRef connection, CFStringRef name) {
  bool exists = 0;
  SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("select count(*) from sqlite_master where type = 'index' and name = ?"));
  SQLite3StatementBindString(statement, 1, name);
  if (kSQLite3StatusRow == SQLite3StatementStep(statement))
    exists = SQLite3StatementGetBOOLWithColumn(statement, 0);
  SQLite3StatementReset(statement);
  SQLite3StatementClearBindings(statement);
  SQLite3StatementFinalize(statement);
  SQLite3StatementRelease(statement);
  return exists;
}

inline SQLite3Status SQLite3ConnectionSetBusyTimeout(SQLite3ConnectionRef connection, CFTimeInterval ti) {
  return sqlite3_busy_timeout(connection->db, (int)(ti * 1000));
}

#pragma Update callbacks

//inline void __SQLite3ConnectionSetUpdateHook(SQLite3ConnectionRef connection, void(*callback)(void *userInfo, int action, char const *databaseName, char const *tableName, sqlite3_int64 rowId), void *userInfo) {
//  if (connection) {
//    sqlite3_update_hook(connection->db, callback, userInfo);
//  }
//}

inline void __SQLite3ConnectionDefaultUpdateHook(void *userInfo, int anAction, char const *aDatabaseName, char const *aTableName, sqlite3_int64 aRowId) {
  SQLite3ConnectionRef connection = (SQLite3ConnectionRef)userInfo;
  if (connection) {
    if (connection->__updateCallbacksWithUserInfoCount > 0) {
      CFStringRef table = CFStringCreateWithCStringNoCopy(connection->allocator, aTableName, kCFStringEncodingUTF8, kCFAllocatorNull);
      for (CFIndex i = 0; i < connection->__updateCallbacksWithUserInfoCount; i++)
        ((SQLite3UpdateCallback)connection->__updateCallbacksWithUserInfo[i].callback)(connection, anAction, table, aRowId, connection->__updateCallbacksWithUserInfo[i].userInfo);
      CFRelease(table);
    }
  }
}

inline bool SQLite3ConnectionAppendUpdateCallback(SQLite3ConnectionRef connection, SQLite3UpdateCallback callback, void *userInfo) {
  bool success = 0;
  if (connection && callback) {
      
    if (connection->__updateCallbacksWithUserInfoCount < kSQlite3UserCallbacksMaxCount) {
      connection->__updateCallbacksWithUserInfo[connection->__updateCallbacksWithUserInfoCount].callback = callback;
      connection->__updateCallbacksWithUserInfo[connection->__updateCallbacksWithUserInfoCount].userInfo = userInfo;
      connection->__updateCallbacksWithUserInfoCount++;
      success = 1;
    }
    
    // Registered the first callback? Set the hook.
    if (connection->__updateCallbacksWithUserInfoCount == 1)
      sqlite3_update_hook(connection->db, __SQLite3ConnectionDefaultUpdateHook, connection);

  }
  return success;
}

inline bool SQLite3ConnectionRemoveUpdateCallback(SQLite3ConnectionRef connection, SQLite3UpdateCallback callback, void *userInfo) {
  bool success = 0;
  if (connection && callback) {
    
    // TODO: critial section
    if (connection->__updateCallbacksWithUserInfoCount > 0) {
      for (int i = 0; i < connection->__updateCallbacksWithUserInfoCount; i++) {
        if (connection->__updateCallbacksWithUserInfo[i].callback == callback && connection->__updateCallbacksWithUserInfo[i].userInfo == userInfo) {
          CFIndex n = --connection->__updateCallbacksWithUserInfoCount;
          connection->__updateCallbacksWithUserInfo[i].callback = connection->__updateCallbacksWithUserInfo[n].callback;
          connection->__updateCallbacksWithUserInfo[i].userInfo = connection->__updateCallbacksWithUserInfo[n].userInfo;
          success = 1;
          break;
        }
      }
    }
    
    // De-registered last callback? Nullify the hook.
    if (connection->__updateCallbacksWithUserInfoCount == 0)
      sqlite3_update_hook(connection->db, NULL, connection);
  }
  return success;
}

//CFMutableArrayRef __SQLite3ConnectionGetObserversWithTable(SQLite3ConnectionRef connection, CFStringRef tableOrNull) {
//  CFMutableArrayRef array = NULL;
//  if (connection) {
//    CFTypeRef key = tableOrNull ? (CFTypeRef)tableOrNull : kCFNull;
//    array = (CFMutableArrayRef)CFDictionaryGetValue(connection->__observers, key);
//    if (array == NULL) {
//      array = CFArrayCreateMutable(connection->allocator, 0, &kCFTypeArrayCallBacks);
//      CFDictionarySetValue(connection->__observers, key, array);
//      CFRelease(array);
//    }
//  }
//  return array;
//}
//
//bool __SQLite3ConnectionObserverIsEnabled(SQLite3ConnectionRef connection, SQLite3ObserverRef observer) {
//  CFArrayRef observers = __SQLite3ConnectionGetObserversWithTable(connection, observer->table);
//  return CFArrayGetFirstIndexOfValue(observers, CFRangeMake(0, CFArrayGetCount(observers)), observer);
//}
//
//void __SQLite3ConnectionObserverEnable(SQLite3ConnectionRef connection, SQLite3ObserverRef observer) {
//  if (connection) {
//    if (observer) {
//      CFMutableArrayRef observers = __SQLite3ConnectionGetObserversWithTable(connection, observer->table);
//    }
//  }
//}
//
//void __SQLite3ConnectionObserverDisable(SQLite3ConnectionRef connection, SQLite3ObserverRef observer);
