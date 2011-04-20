//
// SQLite3Types.h
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 08/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "CoreSQLite3.h"

#define kSQLite3UpdateCallbacksMaxCount 64

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

typedef enum SQLite3OpenOptions {
  kSQLite3OpenReadOnly          = SQLITE_OPEN_READONLY,       // 0x00000001 Ok for sqlite3_open_v2()
  kSQLite3OpenReadWrite         = SQLITE_OPEN_READWRITE,      // 0x00000002 Ok for sqlite3_open_v2()
  kSQLite3OpenCreate            = SQLITE_OPEN_CREATE,         // 0x00000004 Ok for sqlite3_open_v2()
  kSQLite3OpenDeleteOnClose     = SQLITE_OPEN_DELETEONCLOSE,  // 0x00000008 VFS only
  kSQLite3OpenExclusive         = SQLITE_OPEN_EXCLUSIVE,      // 0x00000010 VFS only
  kSQLite3OpenAutoProxy         = SQLITE_OPEN_AUTOPROXY,      // 0x00000020 VFS only
  kSQLite3OpenMainDatabase      = SQLITE_OPEN_MAIN_DB,        // 0x00000100 VFS only
  kSQLite3OpenTemporaryDatabase = SQLITE_OPEN_TEMP_DB,        // 0x00000200 VFS only
  kSQLite3OpenTransientDatabase = SQLITE_OPEN_TRANSIENT_DB,   // 0x00000400 VFS only
  kSQLite3OpenMainJournal       = SQLITE_OPEN_MAIN_JOURNAL,   // 0x00000800 VFS only
  kSQLite3OpenTempJournal       = SQLITE_OPEN_TEMP_JOURNAL,   // 0x00001000 VFS only
  kSQLite3OpenSubJournal        = SQLITE_OPEN_SUBJOURNAL,     // 0x00002000 VFS only
  kSQLite3OpenMasterJournal     = SQLITE_OPEN_MASTER_JOURNAL, // 0x00004000 VFS only
  kSQLite3OpenNoMutex           = SQLITE_OPEN_NOMUTEX,        // 0x00008000 Ok for sqlite3_open_v2()
  kSQLite3OpenFullMutex         = SQLITE_OPEN_FULLMUTEX,      // 0x00010000 Ok for sqlite3_open_v2()
  kSQLite3OpenSharedCache       = SQLITE_OPEN_SHAREDCACHE,    // 0x00020000 Ok for sqlite3_open_v2()
  kSQLite3OpenPrivateCache      = SQLITE_OPEN_PRIVATECACHE    // 0x00040000 Ok for sqlite3_open_v2()
//  kSQLite3OpenWAL               = SQLITE_OPEN_WAL             // 0x00080000 VFS only
} SQLite3OpenOptions;

typedef enum SQLite3Action {
  kSQLite3ActionCreateIndex            = SQLITE_CREATE_INDEX,        //   1 Index Name      Table Name      
  kSQLite3ActionCreateTable            = SQLITE_CREATE_TABLE,        //   2 Table Name      NULL            
  kSQLite3ActionCreateTemporaryIndex   = SQLITE_CREATE_TEMP_INDEX,   //   3 Index Name      Table Name      
  kSQLite3ActionCreateTemporaryTable   = SQLITE_CREATE_TEMP_TABLE,   //   4 Table Name      NULL            
  kSQLite3ActionCreateTemporaryTrigger = SQLITE_CREATE_TEMP_TRIGGER, //   5 Trigger Name    Table Name      
  kSQLite3ActionCreateTemporaryView    = SQLITE_CREATE_TEMP_VIEW,    //   6 View Name       NULL            
  kSQLite3ActionCreateTrigger          = SQLITE_CREATE_TRIGGER,      //   7 Trigger Name    Table Name      
  kSQLite3ActionCreateView             = SQLITE_CREATE_VIEW,         //   8 View Name       NULL            
  kSQLite3ActionDelete                 = SQLITE_DELETE,              //   9 Table Name      NULL            
  kSQLite3ActionDropIndex              = SQLITE_DROP_INDEX,          //  10 Index Name      Table Name      
  kSQLite3ActionDropTable              = SQLITE_DROP_TABLE,          //  11 Table Name      NULL            
  kSQLite3ActionDropTemporaryIndex     = SQLITE_DROP_TEMP_INDEX,     //  12 Index Name      Table Name      
  kSQLite3ActionDropTemporaryTable     = SQLITE_DROP_TEMP_TABLE,     //  13 Table Name      NULL            
  kSQLite3ActionDropTemporaryTrigger   = SQLITE_DROP_TEMP_TRIGGER,   //  14 Trigger Name    Table Name      
  kSQLite3ActionDropTemporaryView      = SQLITE_DROP_TEMP_VIEW,      //  15 View Name       NULL            
  kSQLite3ActionDropTrigger            = SQLITE_DROP_TRIGGER,        //  16 Trigger Name    Table Name      
  kSQLite3ActionDropView               = SQLITE_DROP_VIEW,           //  17 View Name       NULL            
  kSQLite3ActionInsert                 = SQLITE_INSERT,              //  18 Table Name      NULL            
  kSQLite3ActionPragma                 = SQLITE_PRAGMA,              //  19 Pragma Name     1st arg or NULL 
  kSQLite3ActionRead                   = SQLITE_READ,                //  20 Table Name      Column Name     
  kSQLite3ActionSelect                 = SQLITE_SELECT,              //  21 NULL            NULL            
  kSQLite3ActionTransaction            = SQLITE_TRANSACTION,         //  22 Operation       NULL            
  kSQLite3ActionUpdate                 = SQLITE_UPDATE,              //  23 Table Name      Column Name     
  kSQLite3ActionAttach                 = SQLITE_ATTACH,              //  24 Filename        NULL            
  kSQLite3ActionDetach                 = SQLITE_DETACH,              //  25 Database Name   NULL            
  kSQLite3ActionAlterTable             = SQLITE_ALTER_TABLE,         //  26 Database Name   Table Name      
  kSQLite3ActionReindex                = SQLITE_REINDEX,             //  27 Index Name      NULL            
  kSQLite3ActionAnalyze                = SQLITE_ANALYZE,             //  28 Table Name      NULL            
  kSQLite3ActionCreateVirtualTable     = SQLITE_CREATE_VTABLE,       //  29 Table Name      Module Name     
  kSQLite3ActionDropVirtualTable       = SQLITE_DROP_VTABLE,         //  30 Table Name      Module Name     
  kSQLite3ActionFunction               = SQLITE_FUNCTION,            //  31 NULL            Function Name   
  kSQLite3ActionSavePoint              = SQLITE_SAVEPOINT,           //  32 Operation       Savepoint Name  
} SQLite3Action;

typedef enum SQLite3ColumnType {
  kSQLite3TypeInteger = SQLITE_INTEGER,
  kSQLite3TypeFloat   = SQLITE_FLOAT,
  kSQLite3TypeData    = SQLITE_BLOB,
  kSQLite3TypeNULL    = SQLITE_NULL,
  kSQLite3TypeString  = SQLITE_TEXT,
} SQLite3ColumnType;

//extern typedef struct SQLite3ConnectionRef SQLite3ConnectionRef;

#pragma Callbacks

typedef struct __SQLite3UpdateCallbackWithUserInfo {
  void *callback;
  void *userInfo;
} __SQLite3CallbackWithUserInfo;

typedef struct SQLite3Connection {
  CFAllocatorRef     allocator;
  CFIndex            retainCount;
  sqlite3           *db;
  CFDateFormatterRef defaultDateFormatter;
  
  CFIndex                                    __updateCallbacksWithUserInfoCount;
  struct __SQLite3UpdateCallbackWithUserInfo __updateCallbacksWithUserInfo[kSQLite3UpdateCallbacksMaxCount];
} SQLite3Connection;

typedef SQLite3Connection* SQLite3ConnectionRef;

typedef void (*SQLite3UpdateCallback)(SQLite3ConnectionRef connection, SQLite3Action action, CFStringRef table, sqlite3_int64 rowId, void *userInfo);

typedef struct SQLite3Statement {
  CFAllocatorRef       allocator;
  CFIndex              retainCount;
  sqlite3_stmt        *stmt;
  SQLite3ConnectionRef connection;
} SQLite3Statement;

typedef SQLite3Statement *SQLite3StatementRef;

typedef struct SQLite3Observer {
  CFAllocatorRef       allocator;
  CFIndex              retainCount;
  SQLite3ConnectionRef connection;
  bool                 enabled;
  SQLite3Action        actions;
  CFStringRef          table;
  void                *callback;
} SQLite3Observer;

typedef SQLite3Observer *SQLite3ObserverRef;

typedef void (*SQLite3ObserverCallback)(SQLite3ObserverRef observer, SQLite3Action action, CFStringRef table, sqlite3_int64 rowId);
