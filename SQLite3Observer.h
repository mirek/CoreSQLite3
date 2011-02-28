//
//  SQLite3Observer.h
//  SQLite3
//
//  Created by Mirek Rusin on 28/02/2011.
//  Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "CoreSQLite3.h"

SQLite3ObserverRef SQLite3ObserverCreate                    (SQLite3ConnectionRef connection);
SQLite3ObserverRef SQLite3ObserverCreateForActions          (SQLite3ConnectionRef connection, SQLite3Action actions);
SQLite3ObserverRef SQLite3ObserverCreateWithTable           (SQLite3ConnectionRef connection, CFStringRef table);
SQLite3ObserverRef SQLite3ObserverCreateWithTableForActions (SQLite3ConnectionRef connection, CFStringRef table, SQLite3Action actions);
