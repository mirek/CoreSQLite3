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

- (id) initWithConnection: (SQLite3ConnectionRef) anConnection {
  if ((self = [super init])) {
    connection = SQLite3ConnectionRetain(anConnection);
  }
  return self;
}

- (id) initWithPath: (NSString *) path flags: (SQLite3OpenOptions) flags zVfs: (const char *) zVfs {
  if ((self = [super init])) {
    connection = SQLite3ConnectionCreate(kCFAllocatorDefault, (CFStringRef)path, flags, zVfs);
  }
  return self;
}

- (id) initWithPath: (NSString *) path flags: (SQLite3OpenOptions) flags {
  if ((self = [self initWithPath: path flags: flags zVfs: NULL])) {
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

- (SQLite3Statement *) createStatementWithQuery: (NSString *) sql {
  return [[SQLite3Statement alloc] initWithConnection: self query: sql];
}

- (SQLite3Statement *) statementWithQuery: (NSString *) sql {
  return [[self createStatementWithQuery: sql] autorelease];
}

- (SQLite3Status) enumerateWithQuery: (NSString *) sql usingBlock: (BOOL (^)(NSDictionary *row)) block {
  SQLite3Status status = kSQLite3StatusOK;
  SQLite3Statement *statement = [self createStatementWithQuery: sql];
  for (NSDictionary *row in statement) {
    if (block(row)) {
      break;
    }
  }
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

- (SQLite3Status) dropTableWithName: (NSString *) name {
  return SQLite3ConnectionDropTable(connection, (CFStringRef)name);
}

@end
