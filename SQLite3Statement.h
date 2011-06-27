//
// SQLite3Statement.h
// CoreSQLite3
//
// Copyright 2011 Mirek Rusin <mirek [at] me [dot] com>
//

#import "CoreSQLite3.h"

#pragma mark Utility functions

const char *_SQLite3CreateValuesPlaceholderCString(CFAllocatorRef allocator, CFIndex n);

#pragma mark Lifecycle

SQLite3StatementRef _SQLite3StatementCreate (CFAllocatorRef allocator, SQLite3ConnectionRef connection, CFStringRef sql, CFErrorRef *error);

SQLite3StatementRef  SQLite3StatementCreateWithError                             (SQLite3ConnectionRef connection, CFStringRef sql, CFErrorRef *error);
SQLite3StatementRef  SQLite3StatementCreate                                      (SQLite3ConnectionRef connection, CFStringRef sql);
SQLite3StatementRef  SQLite3StatementCreateWithArrayBindingsReturningStatus      (SQLite3ConnectionRef connection, CFStringRef sql, CFArrayRef array, SQLite3Status *status);
SQLite3StatementRef  SQLite3StatementCreateWithArrayBindings                     (SQLite3ConnectionRef connection, CFStringRef sql, CFArrayRef array);
SQLite3StatementRef  SQLite3StatementCreateWithDictionaryBindings                (SQLite3ConnectionRef connection, CFStringRef sql, CFDictionaryRef dictionary);
SQLite3StatementRef  SQLite3StatementCreateWithDictionaryBindingsReturningStatus (SQLite3ConnectionRef connection, CFStringRef sql, CFDictionaryRef dictionary, SQLite3Status *status);
SQLite3StatementRef  SQLite3StatementCreateWithBundleResource                    (SQLite3ConnectionRef connection, CFBundleRef bundle, CFStringRef type, CFStringRef name, CFStringRef subdir);

SQLite3StatementRef  SQLite3StatementRetain          (SQLite3StatementRef statement);
SQLite3StatementRef  SQLite3StatementRelease         (SQLite3StatementRef statement);
SQLite3Status        SQLite3StatementFinalize        (SQLite3StatementRef statement);
SQLite3Status        SQLite3StatementReset           (SQLite3StatementRef statement);
SQLite3Status        SQLite3StatementClearBindings   (SQLite3StatementRef statement);

SQLite3ConnectionRef SQLite3StatementGetConnection (SQLite3StatementRef statement);

SQLite3Status        SQLite3StatementStep          (SQLite3StatementRef statement);
//SQLite3Status        SQLite3StatementExecute       (SQLite3StatementRef statement);

CFStringRef SQLite3StatementCreateSQLString        (SQLite3StatementRef statement) CF_RETURNS_RETAINED;

#pragma mark Binding

CFIndex       SQLite3StatementGetBindParameterIndexWithName    (SQLite3StatementRef statement, CFStringRef  nameWithSpecialCharacter);
CFIndex       SQLite3StatementGetBindParameterCount            (SQLite3StatementRef statement);
CFStringRef   SQLite3StatementCreateBindParameterNameWithIndex (SQLite3StatementRef statement, CFIndex     _1BasedIndex, bool withoutSpecialCharacter);

SQLite3Status SQLite3StatementBindNULL                 (SQLite3StatementRef statement, CFIndex     _1BasedIndex);
SQLite3Status SQLite3StatementBindBool                 (SQLite3StatementRef statement, CFIndex     _1BasedIndex,              bool              value);
SQLite3Status SQLite3StatementBindInt32                (SQLite3StatementRef statement, CFIndex     _1BasedIndex,              int32_t           value);
SQLite3Status SQLite3StatementBindInt64                (SQLite3StatementRef statement, CFIndex     _1BasedIndex,              int64_t           value);
SQLite3Status SQLite3StatementBindDouble               (SQLite3StatementRef statement, CFIndex     _1BasedIndex,              double_t          value);
SQLite3Status SQLite3StatementBindAbsoluteTime         (SQLite3StatementRef statement, CFIndex     _1BasedIndex,              CFAbsoluteTime    value);

SQLite3Status SQLite3StatementBindCFType               (SQLite3StatementRef statement, CFIndex     _1BasedIndex,              CFTypeRef         value);
SQLite3Status SQLite3StatementBindString               (SQLite3StatementRef statement, CFIndex     _1BasedIndex,              CFStringRef       value);
SQLite3Status SQLite3StatementBindNumber               (SQLite3StatementRef statement, CFIndex     _1BasedIndex,              CFNumberRef       value);
SQLite3Status SQLite3StatementBindDate                 (SQLite3StatementRef statement, CFIndex     _1BasedIndex,              CFDateRef         value);
SQLite3Status SQLite3StatementBindData                 (SQLite3StatementRef statement, CFIndex     _1BasedIndex,              CFDataRef         value);

SQLite3Status SQLite3StatementBindArray                (SQLite3StatementRef statement,                                        CFArrayRef        values);
SQLite3Status SQLite3StatementBindDictionary           (SQLite3StatementRef statement,                                        CFDictionaryRef   keyValuePairs);
SQLite3Status SQLite3StatementBindPropertyList         (SQLite3StatementRef statement, CFIndex     _1BasedIndex,              CFPropertyListRef value, CFPropertyListFormat format);

SQLite3Status SQLite3StatementBindNULLWithName         (SQLite3StatementRef statement, CFStringRef  nameWithSpecialCharacter);
SQLite3Status SQLite3StatementBindBoolWithName         (SQLite3StatementRef statement, CFStringRef  nameWithSpecialCharacter, bool        value);
SQLite3Status SQLite3StatementBindInt32WithName        (SQLite3StatementRef statement, CFStringRef  nameWithSpecialCharacter, int32_t     value);
SQLite3Status SQLite3StatementBindInt64WithName        (SQLite3StatementRef statement, CFStringRef  nameWithSpecialCharacter, int64_t     value);
SQLite3Status SQLite3StatementBindDoubleWithName       (SQLite3StatementRef statement, CFStringRef  nameWithSpecialCharacter, double_t    value);
SQLite3Status SQLite3StatementBindAbsoluteTimeWithName (SQLite3StatementRef statement, CFIndex     _1BasedIndex,              CFAbsoluteTime    value);

SQLite3Status SQLite3StatementBindCFTypeWithName       (SQLite3StatementRef statement, CFStringRef  nameWithSpecialCharacter, CFTypeRef         value);
SQLite3Status SQLite3StatementBindStringWithName       (SQLite3StatementRef statement, CFStringRef  nameWithSpecialCharacter, CFStringRef value);
SQLite3Status SQLite3StatementBindNumberWithName       (SQLite3StatementRef statement, CFStringRef  nameWithSpecialCharacter, CFNumberRef value);
SQLite3Status SQLite3StatementBindDateWithName         (SQLite3StatementRef statement, CFStringRef  nameWithSpecialCharacter, CFDateRef   value);
SQLite3Status SQLite3StatementBindDataWithName         (SQLite3StatementRef statement, CFStringRef  nameWithSpecialCharacter, CFDataRef   value);
SQLite3Status SQLite3StatementBindPropertyListWithName (SQLite3StatementRef statement, CFStringRef  nameWithSpecialCharacter, CFPropertyListRef value, CFPropertyListFormat format);

#pragma mark Insert helpers

SQLite3Status  SQLite3InsertWithTableNameAndArray      (SQLite3ConnectionRef connection, CFStringRef table, CFArrayRef array);
SQLite3Status  SQLite3InsertWithTableNameAndDictionary (SQLite3ConnectionRef connection, CFStringRef table, CFDictionaryRef dictionary);

#pragma mark Resultsets

CFIndex           SQLite3StatementGetColumnCount               (SQLite3StatementRef statement);
SQLite3ColumnType SQLite3StatementGetColumnType                (SQLite3StatementRef statement, CFIndex     index);
SQLite3ColumnType SQLite3StatementGetColumnTypeWithName        (SQLite3StatementRef statement, CFStringRef name);
CFIndex           SQLite3StatementGetColumnIndexWithName       (SQLite3StatementRef statement, CFStringRef name);
CFStringRef       SQLite3StatementCreateColumnNameWithIndex    (SQLite3StatementRef statement, CFIndex     index) CF_RETURNS_RETAINED;

bool              SQLite3StatementGetBOOLWithColumn            (SQLite3StatementRef statement, CFIndex index);
int32_t           SQLite3StatementGetInt32WithColumn           (SQLite3StatementRef statement, CFIndex index);
int64_t           SQLite3StatementGetInt64WithColumn           (SQLite3StatementRef statement, CFIndex index);
double_t          SQLite3StatementGetDoubleWithColumn          (SQLite3StatementRef statement, CFIndex index);

CFTypeRef         SQLite3StatementCreateCFTypeWithColumn       (SQLite3StatementRef statement, CFIndex index) CF_RETURNS_RETAINED;
CFStringRef       SQLite3StatementCreateStringWithColumn       (SQLite3StatementRef statement, CFIndex index) CF_RETURNS_RETAINED;
CFNumberRef       SQLite3StatementCreateNumberWithColumn       (SQLite3StatementRef statement, CFIndex index) CF_RETURNS_RETAINED;
CFDataRef         SQLite3StatementCreateDataWithColumn         (SQLite3StatementRef statement, CFIndex index) CF_RETURNS_RETAINED;
CFDateRef         SQLite3StatementCreateDateWithColumn         (SQLite3StatementRef statement, CFIndex index) CF_RETURNS_RETAINED;
CFPropertyListRef SQLite3StatementCreatePropertyListWithColumn (SQLite3StatementRef statement, CFIndex index, CFOptionFlags options, CFPropertyListFormat *format) CF_RETURNS_RETAINED;
//CGImageRef SQLite3StatementCreateImageWithColumn     (SQLite3StatementRef statement, CFIndex index) CF_RETURNS_RETAINED;

bool              SQLite3StatementGetBOOLWithColumnName            (SQLite3StatementRef statement, CFStringRef name);
int32_t           SQLite3StatementGetInt32WithColumnName           (SQLite3StatementRef statement, CFStringRef name);
int64_t           SQLite3StatementGetInt64WithColumnName           (SQLite3StatementRef statement, CFStringRef name);
double_t          SQLite3StatementGetDoubleWithColumnName          (SQLite3StatementRef statement, CFStringRef name);

CFTypeRef         SQLite3StatementCreateCFTypeWithColumnName       (SQLite3StatementRef statement, CFStringRef name) CF_RETURNS_RETAINED;
CFStringRef       SQLite3StatementCreateStringWithColumnName       (SQLite3StatementRef statement, CFStringRef name) CF_RETURNS_RETAINED;
CFNumberRef       SQLite3StatementCreateNumberWithColumnName       (SQLite3StatementRef statement, CFStringRef name) CF_RETURNS_RETAINED;
CFDateRef         SQLite3StatementCreateDateWithColumnName         (SQLite3StatementRef statement, CFStringRef name) CF_RETURNS_RETAINED;
CFDataRef         SQLite3StatementCreateDataWithColumnName         (SQLite3StatementRef statement, CFStringRef name) CF_RETURNS_RETAINED;
CFPropertyListRef SQLite3StatementCreatePropertyListWithColumnName (SQLite3StatementRef statement, CFStringRef name, CFOptionFlags options, CFPropertyListFormat *format) CF_RETURNS_RETAINED;
//CGImageRef SQLite3StatementCreateImageWithColumnName (SQLite3StatementRef statement, CFStringRef name) CF_RETURNS_RETAINED;

CFArrayRef        SQLite3StatementCreateArrayForAllColumns      (SQLite3StatementRef statement) CF_RETURNS_RETAINED;
CFDictionaryRef   SQLite3StatementCreateDictionaryForAllColumns (SQLite3StatementRef statement) CF_RETURNS_RETAINED;

#pragma mark Objective-C

#ifdef __OBJC__

@class SQLite3Connection;

@interface SQLite3Statement : NSObject <NSFastEnumeration> {
  SQLite3Connection *connection;
  SQLite3StatementRef statement;
}

@property (nonatomic, readonly) SQLite3Connection *connection;
@property (nonatomic, readonly) SQLite3StatementRef statement;

#pragma mark Lifecycle

- (id) initWithConnection: (SQLite3Connection *) connection query: (NSString *) sql;
- (id) initWithConnection: (SQLite3Connection *) connection query: (NSString *) sql arrayBindings: (NSArray *) arrayBindings;
- (id) initWithConnection: (SQLite3Connection *) connection query: (NSString *) sql dictionaryBindings: (NSDictionary *) dictionaryBindings;

+ (id) statementWithConnection: (SQLite3Connection *) connection query: (NSString *) sql;
+ (id) statementWithConnection: (SQLite3Connection *) connection query: (NSString *) sql arrayBindings: (NSArray *) arrayBindings;
+ (id) statementWithConnection: (SQLite3Connection *) connection query: (NSString *) sql dictionaryBindings: (NSDictionary *) dictionaryBindings;

- (void) dealloc;

- (SQLite3Status) step;
- (SQLite3Status) reset;

#pragma mark Column info

- (NSInteger) columnCount;
- (SQLite3ColumnType) columnTypeWithIndex: (NSInteger) index;
- (SQLite3ColumnType) columnTypeWithName:  (NSString *) name;

#pragma mark Bindings

- (SQLite3Status) clearBindings;

- (NSInteger) bindParameterIndexWithName: (NSString *) nameWithSpecialCharacter;
- (NSInteger) bindParametersCount;
- (NSString *) createBindParameterNameWithIndex: (NSInteger) _1BasedIndex withoutSpecialCharacter: (BOOL) withoutSpecialCharacter NS_RETURNS_RETAINED;
- (NSString *) bindParameterNameWithIndex: (NSInteger) _1BasedIndex withoutSpecialCharacter: (BOOL) withoutSpecialCharacter;

- (SQLite3Status) bindWithArray: (NSArray *) array;
- (SQLite3Status) bindWithDictionary: (NSDictionary *) dictionary;

- (SQLite3Status) bindObject:       (id)         value withIndex: (NSInteger) _1BasedIndex;
- (SQLite3Status) bindString:       (NSString *) value withIndex: (NSInteger) _1BasedIndex;
- (SQLite3Status) bindNumber:       (NSNumber *) value withIndex: (NSInteger) _1BasedIndex;
- (SQLite3Status) bindDate:         (NSDate *)   value withIndex: (NSInteger) _1BasedIndex;
- (SQLite3Status) bindData:         (NSData *)   value withIndex: (NSInteger) _1BasedIndex;
- (SQLite3Status) bindPropertyList: (id)         value withIndex: (NSInteger) _1BasedIndex format: (NSPropertyListFormat) format;

- (SQLite3Status) bindBOOL:   (BOOL)       value withIndex: (NSInteger) _1BasedIndex;
- (SQLite3Status) bindInt32:  (int32_t)    value withIndex: (NSInteger) _1BasedIndex;
- (SQLite3Status) bindInt64:  (int64_t)    value withIndex: (NSInteger) _1BasedIndex;
- (SQLite3Status) bindDouble: (double_t)   value withIndex: (NSInteger) _1BasedIndex;

- (SQLite3Status) bindObject:       (id)         value withName: (NSString *) nameWithSpecialCharacter;
- (SQLite3Status) bindString:       (NSString *) value withName: (NSString *) nameWithSpecialCharacter;
- (SQLite3Status) bindNumber:       (NSNumber *) value withName: (NSString *) nameWithSpecialCharacter;
- (SQLite3Status) bindDate:         (NSDate *)   value withName: (NSString *) nameWithSpecialCharacter;
- (SQLite3Status) bindData:         (NSData *)   value withName: (NSString *) nameWithSpecialCharacter;
- (SQLite3Status) bindPropertyList: (id)         value withName: (NSString *) nameWithSpecialCharacter format: (NSPropertyListFormat) format;

- (SQLite3Status) bindBOOL:   (BOOL)       value withName: (NSString *) nameWithSpecialCharacter;
- (SQLite3Status) bindInt32:  (int32_t)    value withName: (NSString *) nameWithSpecialCharacter;
- (SQLite3Status) bindInt64:  (int64_t)    value withName: (NSString *) nameWithSpecialCharacter;
- (SQLite3Status) bindDouble: (double_t)   value withName: (NSString *) nameWithSpecialCharacter;

#pragma mark Resultsets
#pragma mark -

#pragma mark Index based values

- (boolean_t) boolWithColumn:   (NSInteger) index;
- (int32_t)   intWithColumn:    (NSInteger) index;
- (int64_t)   int64WithColumn:  (NSInteger) index;
- (double_t)  doubleWithColumn: (NSInteger) index;

#pragma mark Name based values

- (boolean_t) boolWithColumnName:   (NSString *) name;
- (int32_t)   intWithColumnName:    (NSString *) name;
- (int64_t)   int64WithColumnName:  (NSString *) name;
- (double_t)  doubleWithColumnName: (NSString *) name;

#pragma mark Index based, reference counted

- (id)         createObjectWithColumn: (NSInteger) index NS_RETURNS_RETAINED;
- (NSString *) createStringWithColumn: (NSInteger) index NS_RETURNS_RETAINED;
- (NSNumber *) createNumberWithColumn: (NSInteger) index NS_RETURNS_RETAINED;
- (NSDate *)   createDateWithColumn:   (NSInteger) index NS_RETURNS_RETAINED;
- (NSData *)   createDataWithColumn:   (NSInteger) index NS_RETURNS_RETAINED;

#pragma mark Index based, autoreleased

- (id)         objectWithColumn: (NSInteger) index;
- (NSString *) stringWithColumn: (NSInteger) index;
- (NSNumber *) numberWithColumn: (NSInteger) index;
- (NSDate *)   dateWithColumn:   (NSInteger) index;
- (NSData *)   dataWithColumn:   (NSInteger) index;

#pragma mark Name based, reference counted

- (id)         createObjectWithColumnName: (NSString *) name NS_RETURNS_RETAINED;
- (NSString *) createStringWithColumnName: (NSString *) name NS_RETURNS_RETAINED;
- (NSNumber *) createNumberWithColumnName: (NSString *) name NS_RETURNS_RETAINED;
- (NSDate *)   createDateWithColumnName:   (NSString *) name NS_RETURNS_RETAINED;
- (NSData *)   createDataWithColumnName:   (NSString *) name NS_RETURNS_RETAINED;

#pragma mark Name based, autoreleased

- (id)         objectWithColumnName: (NSString *) name;
- (NSString *) stringWithColumnName: (NSString *) name;
- (NSNumber *) numberWithColumnName: (NSString *) name;
- (NSDate *)   dateWithColumnName:   (NSString *) name;
- (NSData *)   dataWithColumnName:   (NSString *) name;

@end

#endif
