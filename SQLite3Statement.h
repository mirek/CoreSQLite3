//
// SQLite3Statement.h
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 07/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "CoreSQLite3.h"

#pragma Utility functions

const char *_SQLite3CreateValuesPlaceholderCString(CFAllocatorRef allocator, CFIndex n);

#pragma Lifecycle

SQLite3StatementRef _SQLite3StatementCreate        (CFAllocatorRef allocator, SQLite3ConnectionRef connection, CFStringRef sql);
SQLite3StatementRef  SQLite3StatementCreate        (SQLite3ConnectionRef connection, CFStringRef sql);
SQLite3StatementRef  SQLite3StatementRetain        (SQLite3StatementRef statement);
SQLite3StatementRef  SQLite3StatementRelease       (SQLite3StatementRef statement);
SQLite3Status        SQLite3StatementFinalize      (SQLite3StatementRef statement);
SQLite3Status        SQLite3StatementReset         (SQLite3StatementRef statement);
SQLite3Status        SQLite3StatementClearBindings (SQLite3StatementRef statement);

SQLite3ConnectionRef SQLite3StatementGetConnection (SQLite3StatementRef statement);

SQLite3Status        SQLite3StatementStep          (SQLite3StatementRef statement);
//SQLite3Status        SQLite3StatementExecute       (SQLite3StatementRef statement);

CFStringRef SQLite3CreateColumnNameStringWithIndex (SQLite3StatementRef statement, CFIndex     index);
CFIndex     SQLite3GetColumnIndexWithName          (SQLite3StatementRef statement, CFStringRef name);
CFIndex     SQLite3StatementGetColumnIndexWithName (SQLite3StatementRef statement, CFStringRef name);

#pragma Binding

SQLite3Status SQLite3StatementBindInt32                        (SQLite3StatementRef statement, CFIndex     index, int32_t     value);
SQLite3Status SQLite3StatementBindInt32WithName                (SQLite3StatementRef statement, CFStringRef name,  int32_t     value);
SQLite3Status SQLite3StatementBindInt64                        (SQLite3StatementRef statement, CFIndex     index, int64_t     value);
SQLite3Status SQLite3StatementBindNumber                       (SQLite3StatementRef statement, CFIndex     index, CFNumberRef value);
SQLite3Status SQLite3StatementBindDouble                       (SQLite3StatementRef statement, CFIndex     index, double_t    value);
SQLite3Status SQLite3StatementBindNULL                         (SQLite3StatementRef statement, CFIndex     index);
SQLite3Status SQLite3StatementBindString                       (SQLite3StatementRef statement, CFIndex     index, CFStringRef value);
SQLite3Status SQLite3StatementBindData                         (SQLite3StatementRef statement, CFIndex     index, CFDataRef   value);
CFIndex       SQLite3StatementGetBindParameterIndexWithName    (SQLite3StatementRef statement, CFStringRef name);
CFIndex       SQLite3StatementGetBindParameterCount            (SQLite3StatementRef statement);
CFStringRef   SQLite3StatementCreateBindParameterNameWithIndex (SQLite3StatementRef statement, CFIndex index);

#pragma Extra binding functions

SQLite3Status SQLite3StatementBindDateTimeWithAbsoluteTime (SQLite3StatementRef statement, CFIndex index, CFAbsoluteTime    value);
SQLite3Status SQLite3StatementBindDateTimeWithDate         (SQLite3StatementRef statement, CFIndex index, CFDateRef         value);
SQLite3Status SQLite3StatementBindCFType                   (SQLite3StatementRef statement, CFIndex index, CFTypeRef         value);
SQLite3Status SQLite3StatementBindArray                    (SQLite3StatementRef statement,                CFArrayRef        values);
SQLite3Status SQLite3StatementBindDictionary               (SQLite3StatementRef statement,                CFDictionaryRef   keyValuePairs);
SQLite3Status SQLite3StatementBindPropertyList             (SQLite3StatementRef statement, CFIndex index, CFPropertyListRef value, CFPropertyListFormat format);
SQLite3Status SQLite3StatementBindPropertyListWithName     (SQLite3StatementRef statement, CFStringRef name, CFPropertyListRef value, CFPropertyListFormat format);


#pragma Resultsets

CFIndex           SQLite3StatementGetColumnCount               (SQLite3StatementRef statement);
SQLite3Type       SQLite3StatementGetColumnType                (SQLite3StatementRef statement, CFIndex index);

CFIndex           SQLite3StatementGetIntegerWithColumn         (SQLite3StatementRef statement, CFIndex index);
int32_t           SQLite3StatementGetInt32WithColumn           (SQLite3StatementRef statement, CFIndex index);
int64_t           SQLite3StatementGetInt64WithColumn           (SQLite3StatementRef statement, CFIndex index);
bool              SQLite3StatementGetBOOLWithColumn            (SQLite3StatementRef statement, CFIndex index);
CFStringRef       SQLite3StatementCreateStringWithColumn       (SQLite3StatementRef statement, CFIndex index);
CFDataRef         SQLite3StatementCreateDataWithColumn         (SQLite3StatementRef statement, CFIndex index);
CFDateRef         SQLite3StatementCreateDateWithColumn         (SQLite3StatementRef statement, CFIndex index);
CFPropertyListRef SQLite3StatementCreatePropertyListWithColumn (SQLite3StatementRef statement, CFIndex index, CFOptionFlags options, CFPropertyListFormat *format, CFErrorRef *error);

//CGImageRef  SQLite3StatementCreateImageWithColumn  (SQLite3StatementRef statement, CFIndex index);

CFDictionaryRef SQLite3StatementCreateDictionaryWithAllColumns(SQLite3StatementRef statement);

#pragma Name based resultsets

//CGImageRef   SQLite3StatementCreateImageWithColumnName(SQLite3StatementRef statement, CFStringRef name);
SQLite3Status  SQLite3InsertWithDictionary(SQLite3ConnectionRef connection, CFStringRef table, CFDictionaryRef dictionary);

//NSInteger   SQLite3StatementColumnGetIntegerWithName(SQLite3StatementRef statement, CFStringRef name);
//CFStringRef SQLite3StatementColumnGetStringWithName(SQLite3StatementRef statement, CFStringRef name);
//BOOL        SQLite3StatementColumnGetBOOLWithName(SQLite3StatementRef statement, CFStringRef name);
//CFDataRef   SQLite3StatementColumnGetDataWithName(SQLite3StatementRef statement, CFStringRef name);

bool        SQLite3StatementColumnGetBOOL(SQLite3StatementRef statement, CFIndex index);
CFDataRef   SQLite3StatementColumnCreateData(SQLite3StatementRef statement, CFIndex index);
