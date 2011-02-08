/*
 *  SQLite3.h
 *
 *  Created by Mirek Rusin on 07/02/2011.
 *  Copyright 2011 Inteliv Ltd. All rights reserved.
 *
 */

#import <sqlite3.h>

#import "SQLite3Types.h"
#import "sqlite3_tiles.h"
#import "SQLite3Connection.h"
#import "SQLite3ConnectionPool.h"
#import "SQLite3Statement.h"
#import "SQLite3Migration.h"


//typedef enum {
//  kSQLite3OpenOptionReadOnly  = SQLITE_OPEN_READONLY,
//  kSQLite3OpenOptionReadWrite = SQLITE_OPEN_READWRITE,
//  kSQLite3OpenOptionAutoProxy = SQLITE_OPEN_AUTOPROXY,
//} SQLite3OpenOptions;

//@interface SQLite3 : NSObject {
//
//@private
//  sqlite3 *db;
//
//}
//
//- (id) initWithPath: (NSString *) path;
////- (id) initWithPath: (NSString *) path options: (SQLite3Options) options;
//
////- (void) migrateWithDirectoryPath: (NSString *) path;
////- (void) migrateWithQuery: (NSString *) query;
////
////- (void) select: (NSString *) block: (^)(NSMutableSet result);
//
//@end
//
//@implementation SQLite3
//
//- (id) initWithPath:(NSString *)path {
//  if ((self = [super init])) {
//    if (SQLITE_OK != sqlite3_open_v2([path fileSystemRepresentation], &db, 0, NULL)) {
//      // TODO: buu
//    }
//  }
//  return self;
//}
//
//- (NSArray *) tableNames {
//  return nil;
//}
//
//- (BOOL) doesTableExistWithName: (NSString *) name {
//  return NO;
//}
//
//- (BOOL) doesTableOrViewExistWithName: (NSString *) tableOrViewName {
//  return NO;
//}
//
//- (BOOL) doesRecordExistWithTableName: (NSString *) table column: (NSString *) name value: (NSString *) value {
//  return NO;
//}
//
//- (BOOL) doesRecordExistWithTableName: (NSString *) table id: (NSString *) id {
//  return NO;
//}
//
//- (void) insertIntoTable: (NSString *) table keysAndObjects: (id *) keysAndObjects {
//}
//
//- (void) insertIntoTable: (NSString *) table keys: (id *) keys objects: (id *) objects {
//}
//
//@end
