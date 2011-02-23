# CoreSQLite3 Framework Reference

## Overview

`CoreSQLite3` Framework is fast, `Core Foundation` oriented, `sqlite3` based framework written in C.

`CoreSQLite3` works on iOS and OSX platforms.

Toll-free bridging between `Core Foundation` and Objective-C allows `CoreSQLite3` framework to be used directly from Objective-C code.

## Functions by Task

### Creating Connection

`SQLite3ConnectionCreate`

### Closing Connection

`SQLite3ConnectionClose`
`SQLite3ConnectionRelease`

### Creating Statement

`SQLite3StatementCreate`

### Binding

Additional binding functions:

#### Binding arrays and dictionaries

SQLite3 supports the following format for parameter bindings:

* `?` or `?1` - index based bindings
* `:name`, `@name`, `$name` - name based bindings

Example queries:

* `select * from users where username = ? and password = ?`
* `select * from users where username = ?1 and password = ?2 and (?1 <> ?2)`
* `select * from users where username = :username and password = :password`

#### Binding arrays of arrays and arrays of dictionaries

#### Binding images

#### Serializing values

SQLite3 supports automatic property list serialisation:

    NSDictionary *preferences = [NSDictionary dictionaryWithObjectsAndKeys:
                                 [NSArray arrayWithObjects: @"English", @"German"], @"languages",
                                 @"English", @"preferred",
                                 nil];

    SQLite3StatementRef statement = SQLite3StatementCreate(connection, @"update users set data = :preferences where id = :id");
    SQLite3StatementBindIntegerWithName(statement, @"id", 1);
    SQLite3StatementBindPropertyList(statement, preferences);
    SQlite3StatementExecute(statement);

To retrieve serialised object graph back you can use `SQLite3StatementCreatePropertyListWithColumn` or connection's function:

    id preferences = SQLite3ConnectionCreatePropertyListWithQuery(connection, @"select data from users where id = 1");

### Backups

### Schema Migrations

Schema migration pattern has been included as an extra functionality built on top of the core functions.

Pattern of schema migrations is similar to Ruby on Rails one.

    /migrations/20100209-111000-init.sql
    /migrations/20100209-111000.undo.sql (optional)
    /migrations/20100209-111000-create-users.sql
    /migrations/20100209-111000.undo.sql (optional)

## Examples

Typical usage:

    CFErrorRef error = NULL;

    // As an example, let's create in-memory database
    SQLite3ConnectionRef connection = SQLite3ConnectionCreate(CFSTR(":memory:"), 0);

    // SQL queries can be executed directly on the connection
    SQLite3ConnectionExecute(connection, CFSTR("create table users(id integer primary key, name varchar, surname varchar)"));
    if ((error = SQLite3ConnectionCreateError(connection))) {
      CFShow(error);
      CFRelease(error);
      exit(-1);
    }

    // Simple binding
    {
      SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("insert into users(name, surname) values(?, ?)"));
      SQLite3StatementBindString(statement, CFSTR("Mirek"));
      SQLite3StatementBindString(statement, CFSTR("Rusin"));
      SQLite3StatementExecute(statement);
      if ((error = SQLite3ConnectionCreateError(connection))) {
        CFShow(error);
        CFRelease(error);
        exit(-1);
      }
      SQLite3StatementRelease(statement);
    }

    // We can use toll-free bridging for Core Foundation types, so it's easy to use the library from Objective-C
    {
      NSArray values = [NSArray arrayWithObjects: @"Mirek", @"Rusin", nil];
      SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("insert into users(name, surname) values(?, ?)"));
      SQLite3StatementBindArray(statement, (CFArrayRef)values);
      SQLite3StatementExecute(statement);
      if ((error = SQLite3ConnectionCreateError(connection))) {
        CFShow(error);
        CFRelease(error);
        exit(-1);
      }
      SQLite3StatementRelease(statement);
    }

    // Selecting is easy
    {
      SQLite3StatementRef statement = SQLite3StatementCreate(connection, CFSTR("select name, surname from users"));
      while (SQLite3StatementStep(statement) == SQLITE_ROW) {
        CFStringRef name = SQLite3StatementGetString(statement, 0);
        CFStringRef surname = SQLite3StatementGetString(statement, 1);
        NSLog(@"name: %@, surname: %@", name, surname);
        CFRelease(name);
        CFRelease(surname);
      }
      if ((error = SQLite3ConnectionCreateError(connection))) {
        CFShow(error);
        CFRelease(error);
        exit(-1);
      }
      SQLite3StatementRelease(statement);
    }

    // Images are supported out-of-the-box
    {

    }

    SQLite3ConnectionRelease(connection);

## Sqlite3 to CoreSQLite3 Quick Function Lookup Table

| Sqlite3 API                      | CoreSQLite3 API                                                              
|:---------------------------------|:---------------
| `sqlite3_aggregate_context`      | `-`
| `sqlite3_auto_extension`         | `-`
| `sqlite3_backup_finish`          | `-`
| `sqlite3_backup_init`            | `-`
| `sqlite3_backup_pagecount`       | `-`
| `sqlite3_backup_remaining`       | `-`
| `sqlite3_backup_step`            | `-`
| `sqlite3_bind_blob`              | `SQLite3StatementBindData`
| `-`                              | `SQLite3StatementBindImage`
| `sqlite3_bind_double`            | `SQLite3StatementBindDouble`    
| `sqlite3_bind_int`               | `SQLite3StatementBindInt32`   
| `sqlite3_bind_int64`             | `SQLite3StatementBindInt64`  
| `sqlite3_bind_null`              | `SQLite3StatementBindNULL`       
| `sqlite3_bind_parameter_count`   | `SQLite3StatementGetBindParameterCount`                                                                            
| `sqlite3_bind_parameter_index`   | `SQLite3StatementGetBindParameterIndex`                                                                            
| `sqlite3_bind_parameter_name`    | `SQLite3StatementCreateBindNameStringWithIndex`                                                                            
| `sqlite3_bind_text`              | `SQLite3StatementBindString`
| `sqlite3_bind_text16`            | `-`                                                                            
| `sqlite3_bind_value`             | `-`                                                                            
| `sqlite3_bind_zeroblob`          | `-`
| `-`                              | `SQLite3StatementBindWithDictionary`
| `-`                              | `SQLite3StatementBindWithArray`
| `sqlite3_blob_bytes`             | `-`                                                                            
| `sqlite3_blob_close`             | `-`                                                                            
| `sqlite3_blob_open`              | `-`                                                                            
| `sqlite3_blob_read`              | `-`                                                                            
| `sqlite3_blob_reopen`            | `-`                                                                            
| `sqlite3_blob_write`             | `-`                                                                            
| `sqlite3_busy_handler`           | `-`                                                                            
| `sqlite3_busy_timeout`           | `-`                                                                            
| `sqlite3_changes`                | `-`                                                                            
| `sqlite3_clear_bindings`         | `SQLite3StatementClearBindings`                 
| `sqlite3_close`                  | `SQLite3ConnectionClose`                                                                            
| `sqlite3_collation_needed`       | `-`                                                                            
| `sqlite3_collation_needed16`     | `-`                                                                            
| `sqlite3_column_blob`            | `-`
| `sqlite3_column_bytes`           | `SQLite3StatementCreateDataWithColumn`
| `-`                              | `SQLite3StatementCreateDataWithColumnName`
| `sqlite3_column_bytes16`         | `-`                                                                           
| `sqlite3_column_count`           | `SQLite3StatementGetColumnCount`                                                                            
| `sqlite3_column_database_name`   | `-`                                                                            
| `sqlite3_column_database_name16` | `-`                                                                            
| `sqlite3_column_decltype`        | `-`                                                                            
| `sqlite3_column_decltype16`      | `-`                                                                            
| `sqlite3_column_double`          | `SQLite3StatementGetDoubleWithColumn`
| `-`                              | `SQLite3StatementGetDoubleWithColumnName`
| `sqlite3_column_int`             | `SQLite3StatementGetInt32WithColumn`                                                                            
| `-`                              | `SQLite3StatementGetInt32WithColumnName`
| `sqlite3_column_int64`           | `SQLite3StatementGetInt64WithColumn`                                                                            
| `-`                              | `SQLite3StatementGetInt64WithColumnName`
| `sqlite3_column_name`            | `SQLite3StatementCreateColumnNameString`
| `-`                              | `SQLite3StatementGetColumnIndexWithName`
| `sqlite3_column_name16`          | `-`                                                                            
| `sqlite3_column_origin_name`     | `-`                                                                            
| `sqlite3_column_origin_name16`   | `-`                                                                            
| `sqlite3_column_table_name`      | `-`                                                                            
| `sqlite3_column_table_name16`    | `-`                                                                            
| `sqlite3_column_text`            | `SQLite3StatementCreateStringWithColumn`                                                                            
| `-`                              | `SQLite3StatementCreateStringWithColumnName`                                                                            
| `sqlite3_column_text16`          | `-`                                                                            
| `sqlite3_column_type`            | `SQLite3StatementGetColumnType`                                                                            
| `sqlite3_column_value`           | `-`                                                                            
| `sqlite3_commit_hook`            | `-`                                                                            
| `sqlite3_compileoption_get`      | `-`                                                                            
| `sqlite3_compileoption_used`     | `-`                                                                            
| `sqlite3_complete`               | `-`                                                                            
| `sqlite3_complete16`             | `-`                                                                            
| `sqlite3_config`                 | `-`                                                                            
| `sqlite3_context_db_handle`      | `-`                                                                            
| `sqlite3_create_collation`       | `-`                                                                            
| `sqlite3_create_collation16`     | `-`                                                                            
| `sqlite3_create_collation_v2`    | `-`                                                                            
| `sqlite3_create_function`        | `-`                                                                            
| `sqlite3_create_function16`      | `-`                                                                            
| `sqlite3_create_function_v2`     | `-`                                                                            
| `sqlite3_create_module`          | `-`                                                                            
| `sqlite3_create_module_v2`       | `-`                                                                            
| `sqlite3_data_count`             | `-`                                                                            
| `sqlite3_db_config`              | `-`                                                                            
| `sqlite3_db_handle`              | `-`                                                                            
| `sqlite3_db_mutex`               | `-`                                                                            
| `sqlite3_db_status`              | `-`                                                                            
| `sqlite3_declare_vtab`           | `-`                                                                            
| `sqlite3_enable_load_extension`  | `-`                                                                            
| `sqlite3_enable_shared_cache`    | `-`                                                                            
| `sqlite3_errcode`                | `SQLite3ConnectionErrorCode`                                                                            
| `-`                              | `SQLite3ConnectionHasError`                                                                            
| `sqlite3_errmsg`                 | `SQLite3ConnectionCreateError`                                                                            
| `sqlite3_errmsg16`               | `-`                                                                            
| `sqlite3_exec`                   | `SQLite3ConnectionExecute`                                                                            
| `sqlite3_extended_errcode`       | `-`                                                                            
| `sqlite3_extended_result_codes`  | `-`                                                                            
| `sqlite3_file_control`           | `-`                                                                            
| `sqlite3_finalize`               | `SQLite3ConnectionFinalize`                                                                            
| `sqlite3_free`                   | `-`                                                                            
| `sqlite3_free_table`             | `-`                                                                            
| `sqlite3_get_autocommit`         | `-`                                                                            
| `sqlite3_get_auxdata`            | `-`                                                                            
| `sqlite3_get_table`              | `-`                                                                            
| `sqlite3_initialize`             | `SQLite3ConnectionCreate`                                                                            
| `sqlite3_interrupt`              | `-`                                                                            
| `sqlite3_last_insert_rowid`      | `-`                                                                            
| `sqlite3_libversion`             | `-`                                                                            
| `sqlite3_libversion_number`      | `-`                                                                            
| `sqlite3_limit`                  | `-`                                                                            
| `sqlite3_load_extension`         | `-`                                                                            
| `sqlite3_log`                    | `-`                                                                            
| `sqlite3_malloc`                 | `-`                                                                            
| `sqlite3_memory_highwater`       | `-`                                                                            
| `sqlite3_memory_used`            | `-`                                                                            
| `sqlite3_mprintf`                | `-`                                                                            
| `sqlite3_mutex_alloc`            | `-`                                                                            
| `sqlite3_mutex_enter`            | `-`                                                                            
| `sqlite3_mutex_free`             | `-`                                                                            
| `sqlite3_mutex_held`             | `-`                                                                            
| `sqlite3_mutex_leave`            | `-`                                                                            
| `sqlite3_mutex_notheld`          | `-`                                                                            
| `sqlite3_mutex_try`              | `-`                                                                            
| `sqlite3_next_stmt`              | `-`                                                                            
| `sqlite3_open`                   | `-`                                                                            
| `sqlite3_open16`                 | `-`                                                                            
| `sqlite3_open_v2`                | `SQLite3ConnectionCreate`                                                                            
| `sqlite3_os_end`                 | `-`                                                                            
| `sqlite3_os_init`                | `-`                                                                            
| `sqlite3_overload_function`      | `-`                                                                            
| `sqlite3_prepare`                | `-`                                                                            
| `sqlite3_prepare16`              | `-`                                                                            
| `sqlite3_prepare16_v2`           | `-`                                                                            
| `sqlite3_prepare_v2`             | `SQLite3StatementCreate`                                                                            
| `sqlite3_profile`                | `-`                                                                            
| `sqlite3_progress_handler`       | `-`                                                                            
| `sqlite3_randomness`             | `-`                                                                            
| `sqlite3_realloc`                | `-`                                                                            
| `sqlite3_release_memory`         | `-`                                                                            
| `sqlite3_reset`                  | `SQLite3StatementReset`                                                                            
| `sqlite3_reset_auto_extension`   | `-`                                                                            
| `sqlite3_result_blob`            | `-`                                                                            
| `sqlite3_result_double`          | `-`                                                                            
| `sqlite3_result_error`           | `-`                                                                            
| `sqlite3_result_error16`         | `-`                                                                            
| `sqlite3_result_error_code`      | `-`                                                                            
| `sqlite3_result_error_nomem`     | `-`                                                                            
| `sqlite3_result_error_toobig`    | `-`                                                                            
| `sqlite3_result_int`             | `-`                                                                            
| `sqlite3_result_int64`           | `-`                                                                            
| `sqlite3_result_null`            | `-`                                                                            
| `sqlite3_result_text`            | `-`                                                                            
| `sqlite3_result_text16`          | `-`                                                                            
| `sqlite3_result_text16be`        | `-`                                                                            
| `sqlite3_result_text16le`        | `-`                                                                            
| `sqlite3_result_value`           | `-`                                                                            
| `sqlite3_result_zeroblob`        | `-`                                                                            
| `sqlite3_rollback_hook`          | `-`                                                                            
| `sqlite3_set_authorizer`         | `-`                                                                            
| `sqlite3_set_auxdata`            | `-`                                                                            
| `sqlite3_shutdown`               | `-`                                                                            
| `sqlite3_sleep`                  | `-`                                                                            
| `sqlite3_snprintf`               | `-`                                                                            
| `sqlite3_soft_heap_limit64`      | `-`                                                                            
| `sqlite3_sourceid`               | `-`                                                                            
| `sqlite3_sql`                    | `-`                                                                            
| `sqlite3_status`                 | `-`                                                                            
| `sqlite3_step`                   | `SQLite3StatementStep`                                                                            
| `sqlite3_stmt_readonly`          | `-`                                                                            
| `sqlite3_stmt_status`            | `-`                                                                            
| `sqlite3_strnicmp`               | `-`                                                                            
| `sqlite3_table_column_metadata`  | `-`                                                                            
| `sqlite3_test_control`           | `-`                                                                            
| `sqlite3_threadsafe`             | `-`                                                                            
| `sqlite3_total_changes`          | `-`                                                                            
| `sqlite3_trace`                  | `-`                                                                            
| `sqlite3_unlock_notify`          | `-`                                                                            
| `sqlite3_update_hook`            | `-`                                                                            
| `sqlite3_user_data`              | `-`                                                                            
| `sqlite3_value_blob`             | `-`                                                                            
| `sqlite3_value_bytes`            | `-`                                                                            
| `sqlite3_value_bytes16`          | `-`                                                                            
| `sqlite3_value_double`           | `-`                                                                            
| `sqlite3_value_int`              | `-`                                                                            
| `sqlite3_value_int64`            | `-`                                                                            
| `sqlite3_value_numeric_type`     | `-`                                                                            
| `sqlite3_value_text`             | `-`                                                                            
| `sqlite3_value_text16`           | `-`                                                                            
| `sqlite3_value_text16be`         | `-`                                                                            
| `sqlite3_value_text16le`         | `-`                                                                            
| `sqlite3_value_type`             | `-`                                                                            
| `sqlite3_version`                | `-`                                                                            
| `sqlite3_vfs_find`               | `-`                                                                            
| `sqlite3_vfs_register`           | `-`                                                                            
| `sqlite3_vfs_unregister`         | `-`                                                                            
| `sqlite3_vmprintf`               | `-`                                                                            
| `sqlite3_vsnprintf`              | `-`                                                                            
| `sqlite3_wal_autocheckpoint`     | `-`                                                                            
| `sqlite3_wal_checkpoint`         | `-`                                                                            
| `sqlite3_wal_hook`               | `-`                                                                            
