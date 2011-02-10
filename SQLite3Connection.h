//
// SQLite3Connection.h
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 07/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "CoreSQLite3.h"


//CFStringRef SQLite3ErrorDomain = CFSTR("com.github.mirek.sqlite3");

SQLite3ConnectionRef _SQLite3ConnectionCreate(CFAllocatorRef allocator, CFStringRef path, int flags, const char *zVfs);
SQLite3ConnectionRef SQLite3ConnectionCreate(CFStringRef path, int flags, const char *zVfs);
void SQLite3OpenResource(CFBundleRef bundle, CFStringRef resourceName, CFStringRef resourceType, CFStringRef subDirName, sqlite3 **db);

SQLite3ConnectionRef SQLite3ConnectionRetain(SQLite3ConnectionRef connection);
NSInteger SQLite3ConnectionRetainCount(SQLite3ConnectionRef connection);
SQLite3ConnectionRef SQLite3ConnectionRelease(SQLite3ConnectionRef connection);
int SQLite3ConnectionClose(SQLite3ConnectionRef connection);

BOOL SQLite3ConnectionHasError(SQLite3ConnectionRef connection);
CFErrorRef SQLite3ConnectionCreateError(SQLite3ConnectionRef connection);

//#endif