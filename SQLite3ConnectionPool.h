//
// SQLite3ConnectionPool.h
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 07/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#if !defined(__CORE_SQLITE3_CONNECTION_POOL__)
#define __CORE_SQLITE3_CONNECTION_POOL__ 1

#include "CoreSQLite3.h"

typedef struct {
  CFTypeRef isa;
  CFMutableSetRef used;
  CFMutableSetRef unused;
} SQLite3ConnectionPool;

typedef SQLite3ConnectionPool* SQLite3ConnectionPoolRef;

#endif