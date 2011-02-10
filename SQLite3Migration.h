//
// SQLite3Migration.h
// CoreSQLite3 Framework
//
// Created by Mirek Rusin on 08/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#import "CoreSQLite3.h"

#define SQLite3MigrationTableName      "schema_changes"
#define SQLite3MigrationCreateTableSQL "create table schema_changes(version varchar)"
