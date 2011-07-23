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

CFTypeRef         SQLite3ConnectionCreateCFTypeWithQuery                  (SQLite3ConnectionRef connection, CFStringRef sql) CF_RETURNS_RETAINED;
CFNumberRef       SQLite3ConnectionCreateNumberWithQuery                  (SQLite3ConnectionRef connection, CFStringRef sql) CF_RETURNS_RETAINED;
CFStringRef       SQLite3ConnectionCreateStringWithQuery                  (SQLite3ConnectionRef connection, CFStringRef sql) CF_RETURNS_RETAINED;
CFDateRef         SQLite3ConnectionCreateDateWithQuery                    (SQLite3ConnectionRef connection, CFStringRef sql) CF_RETURNS_RETAINED;
CFDataRef         SQLite3ConnectionCreateDataWithQuery                    (SQLite3ConnectionRef connection, CFStringRef sql) CF_RETURNS_RETAINED;
//CGImageRef      SQLite3ConnectionCreateImageWithQuery                   (SQLite3ConnectionRef connection, CFStringRef sql) CF_RETURNS_RETAINED;
CFPropertyListRef SQLite3ConnectionCreatePropertyListWithQuery            (SQLite3ConnectionRef connection, CFStringRef sql, CFOptionFlags options, CFPropertyListFormat *format) CF_RETURNS_RETAINED;

CFArrayRef        SQLite3ConnectionCreateArrayForFirstRowWithQuery      (SQLite3ConnectionRef connection, CFStringRef sql) CF_RETURNS_RETAINED;
CFDictionaryRef   SQLite3ConnectionCreateDictionaryForFirstRowWithQuery (SQLite3ConnectionRef connection, CFStringRef sql) CF_RETURNS_RETAINED;

bool              SQLite3ConnectionDoesTableOrViewHaveRowWithID            (SQLite3ConnectionRef connection, CFStringRef name, int64_t rowID);

CFArrayRef        SQLite3ConnectionCreateArrayWithTableOrViewForRowID      (SQLite3ConnectionRef connection, CFStringRef name, int64_t rowID) CF_RETURNS_RETAINED;
CFDictionaryRef   SQLite3ConnectionCreateDictionaryWithTableOrViewForRowID (SQLite3ConnectionRef connection, CFStringRef name, int64_t rowID) CF_RETURNS_RETAINED;

#pragma mark Utility functions

bool SQLite3ConnectionDoesTableExist       (SQLite3ConnectionRef connection, CFStringRef name);
bool SQLite3ConnectionDoesViewExist        (SQLite3ConnectionRef connection, CFStringRef name);
bool SQLite3ConnectionDoesTableOrViewExist (SQLite3ConnectionRef connection, CFStringRef name);
bool SQLite3ConnectionDoesIndexExist       (SQLite3ConnectionRef connection, CFStringRef name);

SQLite3Status SQLite3ConnectionDropTable (SQLite3ConnectionRef connection, CFStringRef name);
SQLite3Status SQLite3ConnectionDropView  (SQLite3ConnectionRef connection, CFStringRef name);
SQLite3Status SQLite3ConnectionDropIndex (SQLite3ConnectionRef connection, CFStringRef name);

SQLite3Status SQLite3ConnectionDropTableIfExists (SQLite3ConnectionRef connection, CFStringRef name);
SQLite3Status SQLite3ConnectionDropViewIfExists  (SQLite3ConnectionRef connection, CFStringRef name);
SQLite3Status SQLite3ConnectionDropIndexIfExists (SQLite3ConnectionRef connection, CFStringRef name);

#pragma mark Backup

//SQLite3BackupRef SQLite3BackupCreate(CFAllocatorRef allocator, SQLite3ConnectionRef destinationConnection, CFStringRef destinationName, SQLite3ConnectionRef sourceConnection, CFStringRef sourceName);
//SQLite3BackupRef SQLite3BackupRetain(SQLite3BackupRef backup);
//SQLite3BackupRef SQLite3BackupRelease(SQLite3BackupRef backup);
//
//SQLite3Status    SQLite3BackupStep(SQLite3BackupRef backup, CFIndex page);
//SQLite3Status    SQLite3BackupFinish(SQLite3BackupRef backup);
//CFIndex          SQLite3BackupRemaining(SQLite3BackupRef backup);
//CFIndex          SQLite3BackupPageCount(SQLite3BackupRef backup);

#ifdef __OBJC__

#pragma mark Delegate protocol

@class SQLite3Statement;
@class SQLite3Connection;

@protocol SQLite3ConnectionDelegate
- (void) SQLite3Connection: (SQLite3Connection *) connection didIndicateProgressAndShouldCancel: (BOOL *) shouldCancel;
- (void) SQLite3Connection: (SQLite3Connection *) connection didUpdateTable: (NSString *) table rowID: (int64_t) rowID operation: (SQLite3Operation) operation;
- (void) SQLite3Connection: (SQLite3Connection *) connection didCommitAndShouldRollback: (BOOL *) shouldRollback;
- (void) SQLite3ConnectionDidRollback: (SQLite3Connection *) connection;
- (void) SQLite3Connection: (SQLite3Connection *) connection didCommitWriteAheadLogOnDatabase: (NSString *) name numberOfPages: (int) numberOfPages returnStatus: (SQLite3Status *) returnStatus;
@end

@interface SQLite3Connection : NSObject {
  SQLite3ConnectionRef connection;
  id<SQLite3ConnectionDelegate> delegate;
}

@property (nonatomic, readonly) SQLite3ConnectionRef connection;
@property (nonatomic, assign) id<SQLite3ConnectionDelegate> delegate;

- (id) initWithConnection: (SQLite3ConnectionRef) connection;
- (id) initWithPath: (NSString *) path flags: (SQLite3OpenOptions) flags zVfs: (const char *) zVfs;
- (id) initWithPath: (NSString *) path flags: (SQLite3OpenOptions) flags;
- (void) dealloc;

- (SQLite3Status) status;

- (SQLite3Statement *) createStatementWithQuery: (NSString *) sql NS_RETURNS_RETAINED;
- (SQLite3Statement *) statementWithQuery:       (NSString *) sql;

- (SQLite3Status) enumerateWithQuery: (NSString *) sql usingBlock: (void (^)(NSDictionary *row, BOOL *stop)) block;

- (SQLite3Status) execute: (NSString *) sql;
//- (SQLite3Status) executeWithFormat: (NSString *) format, ...;
- (SQLite3Status) execute: (NSString *) sql withArrayBindings: (NSArray *) array;
- (SQLite3Status) execute: (NSString *) sql withDictionaryBindings: (NSDictionary *) dictionary;

#pragma mark Value results

- (boolean_t) boolWithQuery:   (NSString *) sql;
- (int32_t)   intWithQuery:    (NSString *) sql;
- (int64_t)   int64WithQuery:  (NSString *) sql;
- (double_t)  doubleWithQuery: (NSString *) sql;

#pragma mark Reference counted object results

- (id)         createObjectWithQuery: (NSString *) sql NS_RETURNS_RETAINED;
- (NSString *) createStringWithQuery: (NSString *) sql NS_RETURNS_RETAINED;
- (NSNumber *) createNumberWithQuery: (NSString *) sql NS_RETURNS_RETAINED;
- (NSData *)   createDataWithQuery:   (NSString *) sql NS_RETURNS_RETAINED;
- (NSDate *)   createDateWithQuery:   (NSString *) sql NS_RETURNS_RETAINED;

#pragma mark Autoreleased object results

- (id)         objectWithQuery: (NSString *) sql;
- (NSString *) stringWithQuery: (NSString *) sql;
- (NSNumber *) numberWithQuery: (NSString *) sql;
- (NSDate *)   dateWithQuery:   (NSString *) sql;
- (NSData *)   dataWithQuery:   (NSString *) sql;

#pragma mark Utility methods

- (BOOL) doesTableExist:       (NSString *) name;
- (BOOL) doesViewExist:        (NSString *) name;
- (BOOL) doesTableOrViewExist: (NSString *) name;
- (BOOL) doesIndexExist:       (NSString *) name;

- (SQLite3Status) dropTable: (NSString *) name;
- (SQLite3Status) dropView:  (NSString *) name;
- (SQLite3Status) dropIndex: (NSString *) name;

- (SQLite3Status) dropTableIfExists: (NSString *) name;
- (SQLite3Status) dropViewIfExists:  (NSString *) name;
- (SQLite3Status) dropIndexIfExists: (NSString *) name;

- (BOOL) doesTableOrView: (NSString *) name haveRowWithID: (int64_t) rowID;

- (NSArray *)      createArrayWithTableOrView:      (NSString *) name forRowID: (int64_t) rowID NS_RETURNS_RETAINED;
- (NSDictionary *) createDictionaryWithTableOrView: (NSString *) name forRowID: (int64_t) rowID NS_RETURNS_RETAINED;

- (NSArray *)      arrayWithTableOrView:      (NSString *) name forRowID: (int64_t) rowID;
- (NSDictionary *) dictionaryWithTableOrView: (NSString *) name forRowID: (int64_t) rowID;

@end

#endif
