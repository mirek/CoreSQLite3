//
// SQLite3Connection.h
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 07/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "CoreSQLite3.h"

       SQLite3ConnectionRef _SQLite3ConnectionCreate (CFAllocatorRef allocator, CFStringRef path, int flags, const char *zVfs);
extern SQLite3ConnectionRef  SQLite3ConnectionCreate (CFStringRef path, int flags, const char *zVfs);
extern void                  SQLite3OpenResource     (CFBundleRef bundle, CFStringRef resourceName, CFStringRef resourceType, CFStringRef subDirName, sqlite3 **db);

extern SQLite3ConnectionRef SQLite3ConnectionRetain      (SQLite3ConnectionRef connection);
extern NSInteger            SQLite3ConnectionRetainCount (SQLite3ConnectionRef connection);
extern SQLite3ConnectionRef SQLite3ConnectionRelease     (SQLite3ConnectionRef connection);
extern int                  SQLite3ConnectionClose       (SQLite3ConnectionRef connection);

extern BOOL       SQLite3ConnectionHasError    (SQLite3ConnectionRef connection);
extern CFErrorRef SQLite3ConnectionCreateError (SQLite3ConnectionRef connection);

#pragma Resultset utility functions

int             SQLite3ConnectionExecute                                (SQLite3ConnectionRef connection, CFStringRef sql);
int             SQLite3ConnectionExecutev                               (SQLite3ConnectionRef connection, CFStringRef sql, ...) CF_FORMAT_FUNCTION(2, 3);
int             SQLite3ConnectionExecuteWithContentsOfFileAtPath        (SQLite3ConnectionRef connection, CFStringRef path);

int32_t         SQLite3ConnectionGetInt32WithQuery                      (SQLite3ConnectionRef connection, CFStringRef sql);
int64_t         SQLite3ConnectionGetInt64WithQuery                      (SQLite3ConnectionRef connection, CFStringRef sql);
BOOL            SQLite3ConnectionGetBOOLWithQuery                       (SQLite3ConnectionRef connection, CFStringRef sql);
CFStringRef     SQLite3ConnectionCreateStringWithQuery                  (SQLite3ConnectionRef connection, CFStringRef sql);
CFDataRef       SQLite3ConnectionCreateDataWithQuery                    (SQLite3ConnectionRef connection, CFStringRef sql);
CGImageRef      SQLite3ConnectionCreateImageWithQuery                   (SQLite3ConnectionRef connection, CFStringRef sql);
CFDateRef       SQLite3ConnectionCreateDateWithQuery                    (SQLite3ConnectionRef connection, CFStringRef sql);
CFDictionaryRef SQLite3ConnectionCreateDictionaryForAllColumnsWithQuery (SQLite3ConnectionRef connection, CFStringRef sql);

BOOL SQLite3ConnectionDoesTableExistWithName       (SQLite3ConnectionRef connection, CFStringRef name);
BOOL SQLite3ConnectionDoesViewExistWithName        (SQLite3ConnectionRef connection, CFStringRef name);
BOOL SQLite3ConnectionDoesTableOrViewExistWithName (SQLite3ConnectionRef connection, CFStringRef name);
