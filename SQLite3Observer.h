//
// SQLite3Observer.h
// CoreSQLite3
//
// Copyright 2011 Mirek Rusin <mirek [at] me [dot] com>
//

#import "CoreSQLite3.h"

SQLite3ObserverRef SQLite3ObserverCreate                    (SQLite3ConnectionRef connection);
SQLite3ObserverRef SQLite3ObserverCreateForActions          (SQLite3ConnectionRef connection, SQLite3Action actions);
SQLite3ObserverRef SQLite3ObserverCreateWithTable           (SQLite3ConnectionRef connection, CFStringRef table);
SQLite3ObserverRef SQLite3ObserverCreateWithTableForActions (SQLite3ConnectionRef connection, CFStringRef table, SQLite3Action actions);
