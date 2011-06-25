//
// SQLite3Observer.c
// CoreSQLite3
//
// Copyright 2011 Mirek Rusin <mirek [at] me [dot] com>
//

#include "SQLite3Observer.h"

#pragma Lifecycle

inline SQLite3ObserverRef SQLite3ObserverCreateWithTableForActions(SQLite3ConnectionRef connection, CFStringRef table, SQLite3Action actions) {
  SQLite3ObserverRef observer = NULL;
  if (connection) {
    if ((observer = CFAllocatorAllocate(connection->allocator, sizeof(SQLite3Observer), 0))) {
      observer->allocator = connection->allocator ? CFRetain(connection->allocator) : NULL;
      observer->retainCount = 1;
      observer->connection = SQLite3ConnectionRetain(connection);
      observer->enabled = 0;
      observer->table = CFStringCreateCopy(observer->allocator, table);
      observer->actions = actions;
      observer->callback = NULL;
    }
  }
  return observer;
}

inline SQLite3ObserverRef SQLite3ObserverCreateWithTable(SQLite3ConnectionRef connection, CFStringRef table) {
  return SQLite3ObserverCreateWithTableForActions(connection, table, kSQLite3ActionInsert | kSQLite3ActionUpdate | kSQLite3ActionDelete);
}

inline SQLite3ObserverRef SQLite3ObserverCreateForActions(SQLite3ConnectionRef connection, SQLite3Action actions) {
  return SQLite3ObserverCreateWithTableForActions(connection, NULL, actions);
}

inline SQLite3ObserverRef SQLite3ObserverCreate(SQLite3ConnectionRef connection) {
  return SQLite3ObserverCreateWithTableForActions(connection, NULL, kSQLite3ActionInsert | kSQLite3ActionUpdate | kSQLite3ActionDelete);
}

inline SQLite3ObserverRef SQLite3ObserverRelease(SQLite3ObserverRef observer) {
  
  return NULL; // TODO:
}

#pragma Callbacks

//inline void SQLite3ObserverEnable(SQLite3ObserverRef observer) {
//  observer->enabled = 1;
//  __SQLite3ObserverSetUp(observer);
//}
//
//inline void SQLite3ObserverDisable(SQLite3ObserverRef observer) {
//  observer->enabled = 0;
//  __SQLite3ObserverSetUp(observer);
//}
//
//inline void SQLite3ObserverSetCallback(SQLite3ObserverRef observer, SQLite3ObserverCallback callback) {
//  observer->callback = callback;
//  __SQLite3ObserverEnableOrDisable(observer);
//}
//
//inline void __SQLite3ObserverEnable(SQLite3ObserverRef observer) {
//  if (observer)
//    if (observer->connection)
//      __SQLite3ConnectionEnableObserver(observer->connection, observer);
//}
//
//inline void __SQLite3ObserverDisable(SQLite3ObserverRef observer) {
//  if (observer)
//    if (observer->connection)
//      __SQLite3ConnectionDisableObserver(observer->connection, observer);
//}
//
//inline void __SQLite3ObserverSetUp(SQLite3ObserverRef observer) {
//  if (observer->enabled) {
//    if (observer->callback) {
//      __SQlite3ObserverEnable(observer);
//    } else {
//      __SQlite3ObserverDisable(observer);
//    }
//  } else {
//    __SQLite3ObserverDisable(observer);
//  }
//}

