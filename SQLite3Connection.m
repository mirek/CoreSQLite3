//
//  SQLite3Connection.m
//  SQLite3
//
//  Created by Mirek Rusin on 25/06/2011.
//  Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "SQLite3Connection.h"

@implementation SQLite3Connection

@synthesize connection;
@synthesize delegate;

- (id) initWithConnection: (SQLite3ConnectionRef) anConnection {
  if ((self = [super init])) {
    connection = SQLite3ConnectionRetain(anConnection);
    delegate = nil;
  }
  return self;
}

- (id) initWithPath: (NSString *) path flags: (SQLite3OpenOptions) flags zVfs: (const char *) zVfs {
  if ((self = [super init])) {
    connection = SQLite3ConnectionCreate(kCFAllocatorDefault, (CFStringRef)path, flags, zVfs);
    delegate = nil;
  }
  return self;
}

- (id) initWithPath: (NSString *) path flags: (SQLite3OpenOptions) flags {
  if ((self = [self initWithPath: path flags: flags zVfs: NULL])) {
    delegate = nil;
  }
  return self;
}

//- (id) initWithPath: (NSString *) path {
//  if ((self = [self initWithPath: path flags:]
//}

//+ (id) connection: (NSString *) name;

- (void) dealloc {
  SQLite3ConnectionRelease(connection);
  [super dealloc];
}

- (void) setDelegate: (id<SQLite3ConnectionDelegate>) value {
  if (delegate != value) {
    delegate = value;
  }
}

//- (void) replaceBusyHandlerWithDefaultBusyTimeout: (int) bt {
////- (void) setBusyTimeout {
////  SQLite3ConnectionGetBu
////  SQLite3ConnectionSetBusyTimeout(<#SQLite3ConnectionRef connection#>, <#CFTimeInterval ti#>)
//}

- (SQLite3Status) status {
  return SQLite3ConnectionGetStatus(connection);
}

- (SQLite3Statement *) createStatementWithQuery: (NSString *) sql {
  return [[SQLite3Statement alloc] initWithConnection: self query: sql];
}

- (SQLite3Statement *) statementWithQuery: (NSString *) sql {
  return [[self createStatementWithQuery: sql] autorelease];
}

- (SQLite3Status) enumerateWithQuery: (NSString *) sql usingBlock: (void (^)(NSDictionary *row, BOOL *terminate)) block {
  SQLite3Status status = kSQLite3StatusOK;
  SQLite3Statement *statement = [self createStatementWithQuery: sql];
  BOOL terminate = NO;
  for (NSDictionary *row in statement) {
    block(row, &terminate);
    if (terminate) {
      break;
    }
  }
//  status = [statement status];
  [statement release];
  return status;
}

- (SQLite3Status) execute: (NSString *) sql {
  return SQLite3ConnectionExecute(connection, (CFStringRef) sql);
}

- (SQLite3Status) execute: (NSString *) sql withDictionaryBindings: (NSDictionary *) dictionary {
  return SQLite3ConnectionExecuteWithDictionaryBindings(connection, (CFStringRef)sql, (CFDictionaryRef)dictionary);
}

- (SQLite3Status) execute: (NSString *) sql withArrayBindings: (NSArray *) array {
  return SQLite3ConnectionExecuteWithArrayBindings(connection, (CFStringRef)sql, (CFArrayRef)array);
}

#pragma mark Value results

- (boolean_t) boolWithQuery: (NSString *) sql {
  return SQLite3ConnectionGetBOOLWithQuery(connection, (CFStringRef)sql);
}

- (int32_t) intWithQuery: (NSString *) sql {
  return SQLite3ConnectionGetInt32WithQuery(connection, (CFStringRef)sql);
}

- (int64_t) int64WithQuery: (NSString *) sql {
  return SQLite3ConnectionGetInt64WithQuery(connection, (CFStringRef)sql);
}

- (double_t) doubleWithQuery: (NSString *) sql {
  return SQLite3ConnectionGetDoubleWithQuery(connection, (CFStringRef)sql);
}

#pragma mark Reference counted object results

- (id) createObjectWithQuery: (NSString *) sql {
  return (id)SQLite3ConnectionCreateCFTypeWithQuery(connection, (CFStringRef)sql);
}

- (NSString *) createStringWithQuery: (NSString *) sql {
  return (NSString *)SQLite3ConnectionCreateStringWithQuery(connection, (CFStringRef)sql);
}

- (NSNumber *) createNumberWithQuery: (NSString *) sql {
  return (NSNumber *)SQLite3ConnectionCreateNumberWithQuery(connection, (CFStringRef)sql);
}

- (NSDate *) createDateWithQuery: (NSString *) sql {
  return (NSDate *)SQLite3ConnectionCreateDateWithQuery(connection, (CFStringRef)sql);
}

- (NSData *) createDataWithQuery: (NSString *) sql {
  return (NSData *)SQLite3ConnectionCreateDataWithQuery(connection, (CFStringRef)sql);
}

#pragma mark Autoreleased object results

- (id) objectWithQuery: (NSString *) sql {
  return [[self createObjectWithQuery: sql] autorelease];
}

- (NSString *) stringWithQuery: (NSString *) sql {
  return [[self createStringWithQuery: sql] autorelease];
}

- (NSNumber *) numberWithQuery: (NSString *) sql {
  return [[self createNumberWithQuery: sql] autorelease];
}

- (NSDate *) dateWithQuery: (NSString *) sql {
  return [[self createDateWithQuery: sql] autorelease];
}

- (NSData *) dataWithQuery: (NSString *) sql {
  return [[self createDataWithQuery: sql] autorelease];
}

#pragma mark Utility functions

- (BOOL) doesTableExist: (NSString *) name {
  return SQLite3ConnectionDoesTableExist(connection, (CFStringRef)name);
}

- (BOOL) doesViewExist: (NSString *) name {
  return SQLite3ConnectionDoesViewExist(connection, (CFStringRef)name);
}

- (BOOL) doesTableOrViewExist: (NSString *) name {
  return SQLite3ConnectionDoesTableOrViewExist(connection, (CFStringRef)name);
}

- (BOOL) doesIndexExist: (NSString *) name {
  return SQLite3ConnectionDoesIndexExist(connection, (CFStringRef)name);
}

- (SQLite3Status) dropTable: (NSString *) name {
  return SQLite3ConnectionDropTable(connection, (CFStringRef)name);
}

- (SQLite3Status) dropView: (NSString *) name {
  return SQLite3ConnectionDropView(connection, (CFStringRef)name);
}

- (SQLite3Status) dropIndex: (NSString *) name {
  return SQLite3ConnectionDropIndex(connection, (CFStringRef)name);
}

- (SQLite3Status) dropTableIfExists: (NSString *) name {
  return SQLite3ConnectionDropTableIfExists(connection, (CFStringRef)name);
}

- (SQLite3Status) dropViewIfExists: (NSString *) name {
  return SQLite3ConnectionDropViewIfExists(connection, (CFStringRef)name);
}

- (SQLite3Status) dropIndexIfExists: (NSString *) name {
  return SQLite3ConnectionDropIndexIfExists(connection, (CFStringRef)name);
}

- (BOOL) doesTableOrView: (NSString *) name haveRowWithID: (int64_t) rowID {
  return SQLite3ConnectionDoesTableOrViewHaveRowWithID(connection, (CFStringRef)name, rowID);
}

- (NSArray *) createArrayWithTableOrView: (NSString *) name forRowID: (int64_t) rowID {
  return (NSArray *)SQLite3ConnectionCreateArrayWithTableOrViewForRowID(connection, (CFStringRef)name, rowID);
}

- (NSDictionary *) createDictionaryWithTableOrView: (NSString *) name forRowID: (int64_t) rowID {
  return (NSDictionary *)SQLite3ConnectionCreateDictionaryWithTableOrViewForRowID(connection, (CFStringRef)name, rowID);
}

- (NSDictionary *) dictionaryWithTableOrView: (NSString *) name forRowID: (int64_t) rowID {
  return [[self createDictionaryWithTableOrView: name forRowID: rowID] autorelease];
}

- (NSArray *) arrayWithTableOrView: (NSString *) name forRowID: (int64_t) rowID {
  return [[self createArrayWithTableOrView: name forRowID: rowID] autorelease];
}

@end
