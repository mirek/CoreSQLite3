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

- (BOOL) boolWithQuery: (NSString *) sql {
  return SQLite3ConnectionGetBOOLWithQuery(connection, (CFStringRef)sql);
}

- (NSString *) createStringWithQuery: (NSString *) sql {
  return (NSString *)SQLite3ConnectionCreateStringWithQuery(connection, (CFStringRef)sql);
}

- (NSString *) stringWithQuery: (NSString *) sql {
  return [[self createStringWithQuery: sql] autorelease];
}

- (SQLite3Status) executeWithQuery: (NSString *) sql {
  return SQLite3ConnectionExecute(connection, (CFStringRef) sql);
}

- (SQLite3Status) executeWithQuery: (NSString *) sql dictionary: (NSDictionary *) dictionary {
  return SQLite3ConnectionExecuteWithDictionaryBindings(connection, (CFStringRef)sql, (CFDictionaryRef)dictionary);
}

- (SQLite3Status) executeWithQuery: (NSString *) sql array: (NSArray *) array {
  return SQLite3ConnectionExecuteWithArrayBindings(connection, (CFStringRef)sql, (CFArrayRef)array);
}

- (int32_t) int32WithQuery: (NSString *) sql {
  return SQLite3ConnectionGetInt32WithQuery(connection, (CFStringRef)sql);
}

- (int64_t) int64WithQuery: (NSString *) sql {
  return SQLite3ConnectionGetInt64WithQuery(connection, (CFStringRef)sql);
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

- (BOOL) doesTableExistWithName: (NSString *) name {
  return SQLite3ConnectionDoesTableExist(connection, (CFStringRef)name);
}

- (BOOL) doesViewExistWithName: (NSString *) name {
  return SQLite3ConnectionDoesViewExist(connection, (CFStringRef)name);
}

- (BOOL) doesTableOrViewExistWithName: (NSString *) name {
  return SQLite3ConnectionDoesTableOrViewExist(connection, (CFStringRef)name);
}

- (BOOL) doesIndexExistWithName: (NSString *) name {
  return SQLite3ConnectionDoesIndexExist(connection, (CFStringRef)name);
}

//- (BOOL) doesTableOrView: (NSString *) name haveRowWithID: (int64_t) rowID {
//  return SQLite3ConnectionDoesTableOrViewHaveRowWithID(connection, (CFStringRef)name, rowID);
//}

//- (NSDictionary *) createDictionaryWithTableOrView: (NSString *) name forRowID: (int64_t) rowID {
//  return (NSDictionary *)SQLite3ConnectionCreateDictionaryWithTableOrViewForRowID(connection, (CFStringRef)name, rowID);
//}

//- (NSDictionary *) dictionaryWithTableOrView: (NSString *) name forRowID: (int64_t) rowID {
//  return [[self createDictionaryWithTableOrView: name forRowID: rowID] autorelease];
//}

//- (NSArray *) createArrayWithTableOrView: (NSString *) name forRowID: (int64_t) rowID {
//  return (NSArray *)SQLite3ConnectionCreateArrayWithTableOrViewForRowID(connection, (CFStringRef)name, rowID);
//}

//- (NSArray *) arrayWithTableOrView: (NSString *) name forRowID: (int64_t) rowID {
//  return [[self createArrayWithTableOrView: name forRowID: rowID] autorelease];
//}

@end
