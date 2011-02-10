//
// SQLite3ConnectionPool.c
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 07/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "SQLite3ConnectionPool.h"

SQLite3ConnectionPoolRef SQLite3ConnectionPoolCreateWithPath(CFStringRef path) {
  return NULL;
}

// Dequeue connection. If there are no connections or all connections are not available
// a new one is created.
// When finished with the connection invoke SQLite3ConnectionRelease(connection) so it can
// be reused again.
SQLite3ConnectionRef SQLite3ConnectionPoolCreateConnection(SQLite3ConnectionPoolRef pool) {
  return NULL;
}
