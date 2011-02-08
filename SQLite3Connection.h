/*
 *  SQLite3Connection.h
 *
 *  Created by Mirek Rusin on 07/02/2011.
 *  Copyright 2011 Inteliv Ltd. All rights reserved.
 *
 */

#import "CoreSQLite3.h"

#ifndef SQLITE3_FRAMEWORK
#define SQLITE3_FRAMEWORK 1

//CFStringRef SQLite3ErrorDomain = CFSTR("com.github.mirek.sqlite3");

SQLite3ConnectionRef SQLite3ConnectionCreate(CFAllocatorRef allocator, CFStringRef path, int flags, const char *zVfs);
void SQLite3ConnectionRelease(SQLite3ConnectionRef connection);

BOOL SQLite3ConnectionHasError(SQLite3ConnectionRef connection);
CFErrorRef SQLite3ConnectionCreateError(SQLite3ConnectionRef connection);

#endif