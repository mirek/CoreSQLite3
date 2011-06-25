//
//  SQLite3Statement.m
//  SQLite3
//
//  Created by Mirek Rusin on 25/06/2011.
//  Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "SQLite3Statement.h"

@implementation SQLite3Statement

@synthesize connection;
@synthesize statement;

- (id) initWithConnection: (SQLite3Connection *) anConnection query: (NSString *) sql {
  if ((self = [super init])) {
    connection = [anConnection retain];
    statement = SQLite3StatementCreate(connection.connection, (CFStringRef)sql);
  }
  return self;
}

+ (id) statementWithConnection: (SQLite3Connection *) connection query: (NSString *) sql {
  return [[[self alloc] initWithConnection: connection query: sql] autorelease];
}

- (void) dealloc {
  SQLite3StatementRelease(statement);
  [connection release];
  [super dealloc];
}

#pragma mark Binding

- (SQLite3Status) bindObject: (id) object withName: (NSString *) name {
  return SQLite3StatementBindCFTypeWithName(statement, (CFStringRef)name, (CFTypeRef)object);
}

#pragma mark Fast Enumeration

- (NSUInteger) countByEnumeratingWithState: (NSFastEnumerationState *) state objects: (id *) stackbuf count: (NSUInteger) len {
	NSUInteger count = 0;
	if (state->state == 0) {
		state->mutationsPtr = &state->extra[0];
    state->extra[0] = kSQLite3StatusRow;
	}
  if (state->extra[0] == kSQLite3StatusRow) {
    state->itemsPtr = stackbuf;
    while (kSQLite3StatusRow == (state->extra[0] = SQLite3StatementStep(statement)) && (count < len) ) {
      stackbuf[count] = [(id)SQLite3StatementCreateDictionaryWithAllColumns(statement) autorelease];
      state->state++;
      count++;
    }
  }
  if (count == 0) {
    SQLite3StatementReset(statement);
  }
	return count;
}

@end
