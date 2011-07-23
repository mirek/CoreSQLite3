//
// CoreSQLite3UTF8String.h
// CoreSQLite3
//
// Created by Mirek Rusin on 21/02/2011.
// Copyright 2011 Inteliv Ltd. All rights reserved.
//

#if !defined(__CORE_SQLITE3_UTF8STRING__)
#define      __CORE_SQLITE3_UTF8STRING__ 1

#include <CoreFoundation/CoreFoundation.h>

#pragma Internal string helper for fast UTF8 buffer access

// Internal helper structure supporting fast and easy access to CFStringRef's
// string buffer. Whenever possible getting the buffer is O(1) without copying
// CFStringRef's buffer. If not possible buffer is being copied.
//
// Provided CFStringRef has to be valid for the lifetime of the struct.
// 
// When finished invoke __SQLite3UTF8StringDestroy(...) to deallocate internal
// data properly. Internal members of this struct should not be accessed directly.
// Use provided functions instead.
//
// This structure is not opaque and is not intended to be passed as function
// argument. CFStringRef should be passed instead. The structure should be
// used inside the function to get access to UTF8 buffer of CFStringRef.
typedef struct {
  CFAllocatorRef allocator;
  CFStringRef string;
  const char *pointer;
  const char *buffer;
  CFIndex maximumSize;
} __SQLite3UTF8String;

__SQLite3UTF8String  __SQLite3UTF8StringMake           (CFAllocatorRef allocator, CFStringRef string);
const char          *__SQLite3UTF8StringGetBuffer      (__SQLite3UTF8String utf8String);
CFIndex              __SQLite3UTF8StringGetMaximumSize (__SQLite3UTF8String utf8String);
void                 __SQLite3UTF8StringDestroy        (__SQLite3UTF8String utf8String);

#endif
