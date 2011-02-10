//
//  Tests.m
//  Tests
//
//  Created by Mirek Rusin on 10/02/2011.
//  Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "Tests.h"

@implementation Tests

- (void) setUp {
  [super setUp];
  connection = SQLite3ConnectionCreate(CFSTR(":memory:"), SQLITE_OPEN_READWRITE, NULL);
  STAssertTrue(connection != NULL, @"Connection should be allocated");
  STAssertFalse(SQLite3ConnectionHasError(connection), @"Connection should't have errors, but got '%s'", sqlite3_errmsg(connection->db));
}

- (void) tearDown {
  [super tearDown];
  STAssertTrue(NULL == SQLite3ConnectionRelease(connection), @"Connection should be released");
}

- (void) testExample {
  //STFail(@"Unit tests are not implemented yet in Tests");
}

@end
