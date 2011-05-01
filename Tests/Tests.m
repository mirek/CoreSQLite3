//
//  Tests.m
//  Tests
//
//  Created by Mirek Rusin on 10/02/2011.
//  Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "Tests.h"

void TestUpdateCallbackCallback1(SQLite3ConnectionRef connection, SQLite3Action action, CFStringRef table, sqlite3_int64 rowId, void *userInfo) {
  *(bool *)userInfo = 1;
}

@implementation Tests

- (void) setUp {
  [super setUp];
  allocator = TestAllocatorCreate();
  connection = SQLite3ConnectionCreate(allocator, CFSTR("/Users/Mirek/my.db"), kSQLite3OpenCreate | kSQLite3OpenReadWrite, NULL);
  SQLite3ConnectionSetBusyTimeout(connection, 3.0);
  SQLite3ExtRegisterAllFunctions(connection);
  STAssertTrue(connection != NULL, @"Connection should be allocated");
  STAssertFalse(SQLite3ConnectionHasError(connection), @"Connection should't have errors, but got '%s'", sqlite3_errmsg(connection->db));
}

- (void) tearDown {
  
  SQLite3ConnectionRelease(connection);

  STAssertTrue(TestAllocatorGetAllocationsCount(allocator) >= 0, @"Allocations count should be more than 0");
  STAssertTrue(TestAllocatorGetDeallocationsCount(allocator) >= 0, @"Deallocations count should be more than 0");
  STAssertEquals(TestAllocatorGetAllocationsCount(allocator), TestAllocatorGetDeallocationsCount(allocator), @"Allocations/deallocations mismatch");
  
  CFRelease(allocator);
  
  [super tearDown];
}

- (void) testMigrationVersionStrings {
  {
    CFStringRef version = SQLite3MigrationCreateVersionStringWithPath(NULL, CFSTR("/Users/Mirek/Project/migrations/2010-02-21_11-56-00.test.sql"));
    STAssertEqualObjects(@"2010-02-21_11-56-00", (id)version, @"Version should match");
    CFRelease(version);
  }
  {
    CFStringRef version = SQLite3MigrationCreateVersionStringWithPath(NULL, CFSTR("2010-02-21_11-56-00.test.sql"));
    STAssertEqualObjects(@"2010-02-21_11-56-00", (id)version, @"Version should match");
    CFRelease(version);
  }
  {
    CFStringRef version = SQLite3MigrationCreateVersionStringWithPath(NULL, CFSTR("2010-02-21_11-56-00.sql"));
    STAssertEqualObjects(@"2010-02-21_11-56-00", (id)version, @"Version should match");
    CFRelease(version);
  }
  {
    CFStringRef version = SQLite3MigrationCreateVersionStringWithPath(NULL, CFSTR("2010-02-21_11-56-00"));
    STAssertEqualObjects(@"2010-02-21_11-56-00", (id)version, @"Version should match");
    CFRelease(version);
  }
}

- (void) testMigrations {
  NSBundle *bundle = [NSBundle bundleForClass: [Tests class]];
  NSString *resourcePath = [bundle resourcePath];
  NSString *migrationsPath = [resourcePath stringByAppendingPathComponent: @"Migrations"];
  NSURL *migrationsURL = [NSURL fileURLWithPath: migrationsPath];
  SQLite3MigrationMigrateWithDirectoryURL(connection, (CFURLRef)migrationsURL);
}

- (void)  testExtensions {
  STAssertEqualsWithAccuracy(1.0, SQLite3ConnectionGetDoubleWithQuery(connection, CFSTR("select sin(rad2deg(90))")), 0.000001, @"Sine")
  {
    CFStringRef r = SQLite3ConnectionCreateStringWithQuery(connection, CFSTR("select if(1 > 2, 'error', 'ok')"));
    NSLog(@"if: %@", r);
    CFRelease(r);
  }
  SQLite3ExtensionsMathUnregisterSin(connection);
}

- (void) testUpdateCallback {
  
  
  SQLite3ConnectionExecute(connection, CFSTR("create table test_update_callback(id int primary key, name string)"));
  
  bool didInvoke = 0;
  
//  SQLite3ObserverRef observer = SQLite3ConnectionCreateObserver(connection, kSQLite3ActionInsert | kSQLite3ActionUpdate | kSQLite3ActionDelete);
//  
//  SQLite3ObserverSetBlock(observer, ^(SQLite3Action action, CFStringRef table, sqlite3_int64 rowId) {
//    if (CFStringCompare(CFSTR("test_update_callback"), table, 0)) {
//      NSLog(@"Change of %lld", rowId);
//    }
//  });
//  
//  SQLite3ObserverRelease(observer);
  
  SQLite3ConnectionAppendUpdateCallback(connection, TestUpdateCallbackCallback1, &didInvoke);
  
  STAssertFalse(didInvoke, @"Should't yet invoke callback");
  SQLite3ConnectionExecute(connection, CFSTR("insert into test_update_callback(id, name) values(1, 'mirek')"));
  STAssertTrue(didInvoke, @"Should invoke callback");

  //SQLite3ConnectionRemoveUpdateCallback(connection, TestUpdateCallbackCallback1);

  SQLite3ConnectionDropTable(connection, CFSTR("test_update_callback"));
}

- (void) testCreateUsersTable {
  NSError *error = nil;
  
  SQLite3ConnectionDropTableIfExists(connection, CFSTR("users"));
  
  SQLite3ConnectionExecute(connection, (CFStringRef)@"create table users(id int primary key, username varchar, name varchar, surname varchar, data text)");
  STAssertNil(error = (NSError *)SQLite3ConnectionCreateError(connection), @"Connection should't have error %@", error);
  
  NSDictionary *user = [[NSDictionary alloc] initWithObjectsAndKeys:
                        [NSNumber numberWithInt: 1], @"id",
                        @"mirek", @"username",
                        @"Mirek", @"name",
                        @"Rusin", @"surname",
                        nil];
  
  NSArray *userArray = [[NSArray alloc] initWithObjects: @"mirek2", @"Mirek", @"Rusin", nil];

  SQLite3ConnectionExecuteWithDictionaryBindings(connection, (CFStringRef)@"insert into users(name, username, surname) values(:name, :username, :surname)", (CFDictionaryRef)user);
  
  SQLite3ConnectionExecuteWithArrayBindings(connection, (CFStringRef)@"insert into users(username, name, surname) values(?, ?, ?)", (CFArrayRef)userArray);

//  NSDictionary *user2 = (NSDictionary *)SQLite3ConnectionCreateDictionaryForAllColumnsWithQuery(connection, CFSTR("select * from users limit 1"));
//  NSLog(@"user %@", user2);

  STAssertEquals(2, SQLite3ConnectionGetInt32WithQuery(connection, CFSTR("select count(*) from users")), @"Should be 2 users");

  {
    NSDictionary *preferences = [NSDictionary dictionaryWithObjectsAndKeys:
                                 [NSArray arrayWithObjects: @"English", @"German", nil], @"languages",
                                 @"English", @"preferred",
                                 nil];
    SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("insert into users(data) values(?)"));
    SQLite3StatementBindPropertyList(statement, 1, preferences, kCFPropertyListXMLFormat_v1_0);
    if (kSQLite3StatusDone != SQLite3StatementStep(statement)) {
      STAssertTrue(NO, @"Insert should return done");
    }
    SQLite3StatementReset(statement);
    SQLite3StatementClearBindings(statement);
    SQLite3StatementFinalize(statement);
    SQLite3StatementRelease(statement);
    
    STAssertEquals(1, SQLite3ConnectionGetInt32WithQuery(connection, CFSTR("select count(*) from users where data is not null")), @"Should be one user with data");
    CFPropertyListRef propertyList = SQLite3ConnectionCreatePropertyListWithQuery(connection, CFSTR("select data from users where data is not null"), kCFPropertyListImmutable, NULL);
    STAssertNotNil(propertyList, @"Property list shouldn't be nil");
    CFRelease(propertyList);
  }
  
  SQLite3ConnectionDropTableIfExists(connection, CFSTR("users"));
  
  [user release];
  
  [error release];
}

@end
