//
// SQLite3Types.h
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 08/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "CoreSQLite3.h"

#pragma Public types

typedef enum SQLite3Status {
  kSQLite3StatusOK                    = SQLITE_OK,         //   0 Successful result
  kSQLite3StatusRow                   = SQLITE_ROW,        // 100 sqlite3_step() has another row ready
  kSQLite3StatusDone                  = SQLITE_DONE,       // 101 sqlite3_step() has finished executing
  
  kSQLite3StatusError                 = SQLITE_ERROR,      //   1 SQL error or missing database
  kSQLite3StatusInternalError         = SQLITE_INTERNAL,   //   2 Internal logic error in SQLite
  kSQLite3StatusPermissionDeniedError = SQLITE_PERM,       //   3 Access permission denied
  kSQLite3StatusAbort                 = SQLITE_ABORT,      //   4 Callback routine requested an abort
  kSQLite3StatusBusy                  = SQLITE_BUSY,       //   5 The database file is locked
  kSQLite3StatusLocked                = SQLITE_LOCKED,     //   6 A table in the database is locked
  kSQLite3StatusNoMemError            = SQLITE_NOMEM,      //   7 A malloc() failed
  kSQLite3StatusReadOnly              = SQLITE_READONLY,   //   8 Attempt to write a readonly database
  kSQLite3StatusInterrupt             = SQLITE_INTERRUPT,  //   9 Operation terminated by sqlite3_interrupt()
  kSQLite3StatusIOError               = SQLITE_IOERR,      //  10 Some kind of disk I/O error occurred
  kSQLite3StatusCorruptError          = SQLITE_CORRUPT,    //  11 The database disk image is malformed
  kSQLite3StatusNotFoundError         = SQLITE_NOTFOUND,   //  12 Unknown opcode in sqlite3_file_control()
  kSQLite3StatusFullError             = SQLITE_FULL,       //  13 Insertion failed because database is full
  kSQLite3StatusCantOpenError         = SQLITE_CANTOPEN,   //  14 Unable to open the database file
  kSQLite3StatusProtocolError         = SQLITE_PROTOCOL,   //  15 Database lock protocol error
  kSQLite3StatusEmpty                 = SQLITE_EMPTY,      //  16 Database is empty
  kSQLite3StatusSchema                = SQLITE_SCHEMA,     //  17 The database schema changed
  kSQLite3StatusTooBigError           = SQLITE_TOOBIG,     //  18 String or BLOB exceeds size limit
  kSQLite3StatusConstraintError       = SQLITE_CONSTRAINT, //  19 Abort due to constraint violation
  kSQLite3StatusMismatchError         = SQLITE_MISMATCH,   //  20 Data type mismatch
  kSQLite3StatusMisuseError           = SQLITE_MISUSE,     //  21 Library used incorrectly
  kSQLite3StatusOSError               = SQLITE_NOLFS,      //  22 Uses OS features not supported on host
  kSQLite3StatusAuthorizationError    = SQLITE_AUTH,       //  23 Authorization denied
  kSQLite3StatusFormatError           = SQLITE_FORMAT,     //  24 Auxiliary database format error
  kSQLite3StatusOutOfRangeError       = SQLITE_RANGE,      //  25 2nd parameter to sqlite3_bind out of range
  kSQLite3StatusNotDatabaseError      = SQLITE_NOTADB      //  26 File opened that is not a database file
} SQLite3Status;

typedef struct SQLite3Connection {
  CFAllocatorRef     allocator;
  CFIndex            retainCount;
  sqlite3           *db;
  CFDateFormatterRef defaultDateFormatter;
} SQLite3Connection;

typedef SQLite3Connection* SQLite3ConnectionRef;

typedef struct SQLite3Statement {
  CFAllocatorRef       allocator;
  CFIndex              retainCount;
  sqlite3_stmt        *stmt;
  SQLite3ConnectionRef connection;
} SQLite3Statement;

typedef SQLite3Statement *SQLite3StatementRef;

typedef enum SQLite3Type {
  kSQLite3TypeInteger = SQLITE_INTEGER,
  kSQLite3TypeFloat   = SQLITE_FLOAT,
  kSQLite3TypeData    = SQLITE_BLOB,
  kSQLite3TypeNULL    = SQLITE_NULL,
  kSQLite3TypeString  = SQLITE_TEXT,
} SQLite3Type;

//typedef enum {
//  kSQLite3OpenOptionReadOnly  = SQLITE_OPEN_READONLY,
//  kSQLite3OpenOptionReadWrite = SQLITE_OPEN_READWRITE,
//  kSQLite3OpenOptionAutoProxy = SQLITE_OPEN_AUTOPROXY,
//} SQLite3OpenOptions;
