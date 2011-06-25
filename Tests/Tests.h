//
// Tests.h
// CoreSQLite3
//
// Copyright 2011 Mirek Rusin <mirek [at] me [dot] com>
//

#import <SenTestingKit/SenTestingKit.h>
#import "CoreSQLite3.h"
#import "TestAllocator.h"

@interface Tests : SenTestCase {
@private
  CFAllocatorRef allocator;
  SQLite3ConnectionRef coreConnection;
  SQLite3Connection *connection;
}

@end
