/*
 *  SQLite3ConnectionPool.h
 *
 *  Created by Mirek Rusin on 07/02/2011.
 *  Copyright 2011 Inteliv Ltd. All rights reserved.
 *
 */

#import "CoreSQLite3.h"

typedef struct {
  CFTypeRef isa;
  CFMutableSetRef used;
  CFMutableSetRef unused;
} SQLite3ConnectionPool;

typedef SQLite3ConnectionPool* SQLite3ConnectionPoolRef;
