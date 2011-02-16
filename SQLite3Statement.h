//
// SQLite3Statement.h
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 07/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "CoreSQLite3.h"

#pragma Utility functions

const char *_SQLite3CreateValuesPlaceholderCString(CFAllocatorRef allocator, NSUInteger n);

#pragma Lifecycle

SQLite3StatementRef _SQLite3StatementCreate        (CFAllocatorRef allocator, SQLite3ConnectionRef connection, CFStringRef sql);
SQLite3StatementRef  SQLite3StatementCreate        (SQLite3ConnectionRef connection, CFStringRef sql);
SQLite3StatementRef  SQLite3StatementRetain        (SQLite3StatementRef  statement);
SQLite3StatementRef  SQLite3StatementRelease       (SQLite3StatementRef  statement);
int                  SQLite3StatementFinalize      (SQLite3StatementRef  statement);
int                  SQLite3StatementClearBindings (SQLite3StatementRef  statement);

SQLite3ConnectionRef SQLite3StatementGetConnection(SQLite3StatementRef statement);

int SQLite3StatementStep     (SQLite3StatementRef  statement);
int SQLite3StatementExecute  (SQLite3StatementRef  statement);

CFStringRef SQLite3CreateColumnNameStringWithIndex (SQLite3StatementRef statement, NSInteger   index);
NSUInteger  SQLite3GetColumnIndexWithName          (SQLite3StatementRef statement, CFStringRef name);
NSInteger   SQLite3StatementGetColumnIndexWithName (SQLite3StatementRef statement, CFStringRef name);

#pragma Binding

int SQLite3StatementBindInt32  (SQLite3StatementRef statement, NSInteger index, int32_t     value);
int SQLite3StatementBindInt64  (SQLite3StatementRef statement, NSInteger index, int64_t     value);
int SQLite3StatementBindDouble (SQLite3StatementRef statement, NSInteger index, double_t    value);
int SQLite3StatementBindNULL   (SQLite3StatementRef statement, NSInteger index);
int SQLite3StatementBindString (SQLite3StatementRef statement, NSInteger index, CFStringRef value);
int SQLite3StatementBindData   (SQLite3StatementRef statement, NSInteger index, CFDataRef   value);

#pragma Extra binding functions

int SQLite3StatementBindCFType     (SQLite3StatementRef statement, NSInteger index, CFTypeRef       value);
int SQLite3StatementBindArray      (SQLite3StatementRef statement,                  CFArrayRef      values);
int SQLite3StatementBindDictionary (SQLite3StatementRef statement,                  CFDictionaryRef keyValuePairs);

#pragma Resultsets

SQLite3Type SQLite3StatementGetColumnType          (SQLite3StatementRef statement, NSInteger index);

NSInteger   SQLite3StatementGetIntegerWithColumn   (SQLite3StatementRef statement, NSInteger index);
int32_t     SQLite3StatementGetInt32WithColumn     (SQLite3StatementRef statement, NSInteger index);
int64_t     SQLite3StatementGetInt64WithColumn     (SQLite3StatementRef statement, NSInteger index);
BOOL        SQLite3StatementGetBOOLWithColumn      (SQLite3StatementRef statement, NSInteger index);
CFStringRef SQLite3StatementCreateStringWithColumn (SQLite3StatementRef statement, NSInteger index);
CFDataRef   SQLite3StatementCreateDataWithColumn   (SQLite3StatementRef statement, NSInteger index);
CGImageRef  SQLite3StatementCreateImageWithColumn  (SQLite3StatementRef statement, NSInteger index);

#pragma Name based resultsets

CGImageRef   SQLite3StatementCreateImageWithColumnName(SQLite3StatementRef statement, CFStringRef name);
void         SQLite3InsertWithDictionary(SQLite3ConnectionRef connection, CFStringRef table, CFDictionaryRef dictionary);

//NSInteger   SQLite3StatementColumnGetIntegerWithName(SQLite3StatementRef statement, CFStringRef name);
//CFStringRef SQLite3StatementColumnGetStringWithName(SQLite3StatementRef statement, CFStringRef name);
//BOOL        SQLite3StatementColumnGetBOOLWithName(SQLite3StatementRef statement, CFStringRef name);
//CFDataRef   SQLite3StatementColumnGetDataWithName(SQLite3StatementRef statement, CFStringRef name);

BOOL        SQLite3StatementColumnGetBOOL(SQLite3StatementRef statement, NSUInteger index);
CFDataRef   SQLite3StatementColumnCreateData(SQLite3StatementRef statement, NSUInteger index);
