//
// SQLite3Connection.h
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 07/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#include "CoreSQLite3.h"

       SQLite3ConnectionRef _SQLite3ConnectionCreate (CFAllocatorRef allocator, CFStringRef path, int flags, const char *zVfs);
extern SQLite3ConnectionRef  SQLite3ConnectionCreate (CFAllocatorRef allocator, CFStringRef path, int flags, const char *zVfs);
extern void                  SQLite3OpenResource     (CFAllocatorRef allocator, CFBundleRef bundle, CFStringRef resourceName, CFStringRef resourceType, CFStringRef subDirName, sqlite3 **db);

extern SQLite3ConnectionRef SQLite3ConnectionRetain      (SQLite3ConnectionRef connection);
extern CFIndex              SQLite3ConnectionRetainCount (SQLite3ConnectionRef connection);
extern SQLite3ConnectionRef SQLite3ConnectionRelease     (SQLite3ConnectionRef connection);
extern SQLite3Status        SQLite3ConnectionClose       (SQLite3ConnectionRef connection);

extern bool       SQLite3ConnectionHasError    (SQLite3ConnectionRef connection);
extern CFErrorRef SQLite3ConnectionCreateError (SQLite3ConnectionRef connection);

extern bool          SQLite3ConnectionDoesTableExist    (SQLite3ConnectionRef connection, CFStringRef name);
extern SQLite3Status SQLite3ConnectionDropTable         (SQLite3ConnectionRef connection, CFStringRef name);
extern SQLite3Status SQLite3ConnectionDropTableIfExists (SQLite3ConnectionRef connection, CFStringRef name);

extern SQLite3Status SQLite3ConnectionSetBusyTimeout    (SQLite3ConnectionRef connection, CFTimeInterval ti);

#pragma Resultset utility functions

SQLite3Status     SQLite3ConnectionExecuteWithContentsOfURL               (SQLite3ConnectionRef connection, CFURLRef url);

SQLite3Status     SQLite3ConnectionExecute                                (SQLite3ConnectionRef connection, CFStringRef sql);
SQLite3Status     SQLite3ConnectionExecutev                               (SQLite3ConnectionRef connection, CFStringRef sql, ...) CF_FORMAT_FUNCTION(2, 3);
SQLite3Status     SQLite3ConnectionExecuteWithArrayBindings               (SQLite3ConnectionRef connection, CFStringRef sql, CFArrayRef values);
SQLite3Status     SQLite3ConnectionExecuteWithDictionaryBindings          (SQLite3ConnectionRef connection, CFStringRef sql, CFDictionaryRef keysValues);

int32_t           SQLite3ConnectionGetInt32WithQuery                      (SQLite3ConnectionRef connection, CFStringRef sql);
int64_t           SQLite3ConnectionGetInt64WithQuery                      (SQLite3ConnectionRef connection, CFStringRef sql);
bool              SQLite3ConnectionGetBOOLWithQuery                       (SQLite3ConnectionRef connection, CFStringRef sql);
CFStringRef       SQLite3ConnectionCreateStringWithQuery                  (SQLite3ConnectionRef connection, CFStringRef sql);
CFDataRef         SQLite3ConnectionCreateDataWithQuery                    (SQLite3ConnectionRef connection, CFStringRef sql);
//CGImageRef      SQLite3ConnectionCreateImageWithQuery                   (SQLite3ConnectionRef connection, CFStringRef sql);
CFDateRef         SQLite3ConnectionCreateDateWithQuery                    (SQLite3ConnectionRef connection, CFStringRef sql);
CFDictionaryRef   SQLite3ConnectionCreateDictionaryForAllColumnsWithQuery (SQLite3ConnectionRef connection, CFStringRef sql);
CFPropertyListRef SQLite3ConnectionCreatePropertyListWithQuery            (SQLite3ConnectionRef connection, CFStringRef sql, CFOptionFlags options, CFPropertyListFormat *format, CFErrorRef *error);

bool SQLite3ConnectionDoesTableExistWithName       (SQLite3ConnectionRef connection, CFStringRef name);
bool SQLite3ConnectionDoesViewExistWithName        (SQLite3ConnectionRef connection, CFStringRef name);
bool SQLite3ConnectionDoesTableOrViewExistWithName (SQLite3ConnectionRef connection, CFStringRef name);
