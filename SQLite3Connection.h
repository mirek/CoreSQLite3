//
// SQLite3Connection.h
// CoreSQLite3
//
// Copyright 2011 Mirek Rusin <mirek [at] me [dot] com>
//

#include "CoreSQLite3.h"

       SQLite3ConnectionRef _SQLite3ConnectionCreate (CFAllocatorRef allocator, CFStringRef path, SQLite3OpenOptions flags, const char *zVfs);
extern SQLite3ConnectionRef  SQLite3ConnectionCreate (CFAllocatorRef allocator, CFStringRef path, SQLite3OpenOptions flags, const char *zVfs);
extern void                  SQLite3OpenResource     (CFAllocatorRef allocator, CFBundleRef bundle, CFStringRef resourceName, CFStringRef resourceType, CFStringRef subDirName, sqlite3 **db);

extern SQLite3ConnectionRef SQLite3ConnectionRetain      (SQLite3ConnectionRef connection);
extern CFIndex              SQLite3ConnectionRetainCount (SQLite3ConnectionRef connection);
extern SQLite3ConnectionRef SQLite3ConnectionRelease     (SQLite3ConnectionRef connection);
extern SQLite3Status        SQLite3ConnectionClose       (SQLite3ConnectionRef connection);

extern SQLite3Status SQLite3ConnectionGetStatus   (SQLite3ConnectionRef connection);
extern bool          SQLite3ConnectionHasError    (SQLite3ConnectionRef connection);
extern CFErrorRef    SQLite3ConnectionCreateError (SQLite3ConnectionRef connection);

extern bool          SQLite3ConnectionDoesTableExist    (SQLite3ConnectionRef connection, CFStringRef name);
extern SQLite3Status SQLite3ConnectionDropTable         (SQLite3ConnectionRef connection, CFStringRef name);
extern SQLite3Status SQLite3ConnectionDropTableIfExists (SQLite3ConnectionRef connection, CFStringRef name);

extern SQLite3Status SQLite3ConnectionSetBusyTimeout    (SQLite3ConnectionRef connection, CFTimeInterval ti);

#pragma Observers

//extern void __SQLite3ConnectionSetUpdateHook              (SQLite3ConnectionRef connection, void(*callback)(void *userInfo, int action, char const *databaseName, char const *tableName, sqlite3_int64 rowId), void *userInfo);

extern void __SQLite3ConnectionDefaultUpdateHook          (void *userInfo, int anAction, char const *aDatabaseName, char const *aTableName, sqlite3_int64 aRowId);

extern bool SQLite3ConnectionAppendUpdateCallback       (SQLite3ConnectionRef connection, SQLite3UpdateCallback callback, void *userInfo);
extern bool SQLite3ConnectionRemoveUpdateCallback       (SQLite3ConnectionRef connection, SQLite3UpdateCallback callback, void *userInfo);

//void __SQLite3ConnectionEnableObserver  (SQLite3ConnectionRef connection, SQLite3ObserverRef observer);
//void __SQLite3ConnectionDisableObserver (SQLite3ConnectionRef connection, SQLite3ObserverRef observer);

//typedef SQLite3ConnectionDefaultUpdateHook SQLite3ConnectionDefaultUpdateHook;
//kSQLite3ConnectionDefaultUpdateHook;

#pragma mark Function and aggregate registration

SQLite3Status SQLite3ConnectionRegisterFunction   (SQLite3ConnectionRef connection, CFStringRef name, CFIndex argc, void (*f)(sqlite3_context *, int, sqlite3_value **));
SQLite3Status SQLite3ConnectionUnregisterFunction (SQLite3ConnectionRef connection, CFStringRef name, CFIndex argc);

#pragma mark Resultset utility functions

SQLite3Status     SQLite3ConnectionExecuteWithContentsOfURL               (SQLite3ConnectionRef connection, CFURLRef url);

SQLite3Status     SQLite3ConnectionExecute                                (SQLite3ConnectionRef connection, CFStringRef sql);
SQLite3Status     SQLite3ConnectionExecutev                               (SQLite3ConnectionRef connection, CFStringRef sql, ...) CF_FORMAT_FUNCTION(2, 3);
SQLite3Status     SQLite3ConnectionExecuteWithArrayBindings               (SQLite3ConnectionRef connection, CFStringRef sql, CFArrayRef values);
SQLite3Status     SQLite3ConnectionExecuteWithArrayBindingsAndError       (SQLite3ConnectionRef connection, CFStringRef sql, CFArrayRef values, CFErrorRef *error);
SQLite3Status     SQLite3ConnectionExecuteWithDictionaryBindings          (SQLite3ConnectionRef connection, CFStringRef sql, CFDictionaryRef keysValues);

int32_t           SQLite3ConnectionGetInt32WithQuery                      (SQLite3ConnectionRef connection, CFStringRef sql);
int64_t           SQLite3ConnectionGetInt64WithQuery                      (SQLite3ConnectionRef connection, CFStringRef sql);
int64_t           SQLite3ConnectionGetLastInsertedRowId                   (SQLite3ConnectionRef connection);
double_t          SQLite3ConnectionGetDoubleWithQuery                     (SQLite3ConnectionRef connection, CFStringRef sql);
bool              SQLite3ConnectionGetBOOLWithQuery                       (SQLite3ConnectionRef connection, CFStringRef sql);
CFStringRef       SQLite3ConnectionCreateStringWithQuery                  (SQLite3ConnectionRef connection, CFStringRef sql);
CFDataRef         SQLite3ConnectionCreateDataWithQuery                    (SQLite3ConnectionRef connection, CFStringRef sql);
//CGImageRef      SQLite3ConnectionCreateImageWithQuery                   (SQLite3ConnectionRef connection, CFStringRef sql);
CFDateRef         SQLite3ConnectionCreateDateWithQuery                    (SQLite3ConnectionRef connection, CFStringRef sql);
CFDictionaryRef   SQLite3ConnectionCreateDictionaryForAllColumnsWithQuery (SQLite3ConnectionRef connection, CFStringRef sql);
CFPropertyListRef SQLite3ConnectionCreatePropertyListWithQuery            (SQLite3ConnectionRef connection, CFStringRef sql, CFOptionFlags options, CFPropertyListFormat *format);

bool SQLite3ConnectionDoesTableExistWithName       (SQLite3ConnectionRef connection, CFStringRef name);
bool SQLite3ConnectionDoesViewExistWithName        (SQLite3ConnectionRef connection, CFStringRef name);
bool SQLite3ConnectionDoesTableOrViewExistWithName (SQLite3ConnectionRef connection, CFStringRef name);

#ifdef __OBJC__

@class SQLite3Statement;

@interface SQLite3Connection : NSObject {
  SQLite3ConnectionRef connection;
}

@property (nonatomic, readonly) SQLite3ConnectionRef connection;

- (id) initWithConnection: (SQLite3ConnectionRef) connection;
- (id) initWithPath: (NSString *) path flags: (SQLite3OpenOptions) flags zVfs: (const char *) zVfs;
- (id) initWithPath: (NSString *) path flags: (SQLite3OpenOptions) flags;
- (void) dealloc;

- (SQLite3Statement *) createStatementWithQuery: (NSString *) sql NS_RETURNS_RETAINED;
- (SQLite3Statement *) statementWithQuery: (NSString *) sql;

- (SQLite3Status) enumerateWithQuery: (NSString *) sql usingBlock: (BOOL (^)(NSDictionary *row)) block;

- (BOOL) boolWithQuery: (NSString *) sql;
- (NSString *) createStringWithQuery: (NSString *) sql NS_RETURNS_RETAINED;
- (NSString *) stringWithQuery: (NSString *) sql;

- (SQLite3Status) executeWithQuery: (NSString *) sql;
- (SQLite3Status) executeWithQuery: (NSString *) sql dictionary: (NSDictionary *) dictionary;
- (SQLite3Status) executeWithQuery: (NSString *) sql array: (NSArray *) array;

- (int32_t) int32WithQuery: (NSString *) sql;
- (int64_t) int64WithQuery: (NSString *) sql;

- (SQLite3Status) dropTableWithName: (NSString *) name;

@end

#endif
