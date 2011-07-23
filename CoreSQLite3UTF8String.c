//
//  CoreSQLite3UTF8String.c
//  SQLite3
//
//  Created by Mirek Rusin on 21/02/2011.
//  Copyright 2011 Inteliv Ltd. All rights reserved.
//

#include "CoreSQLite3UTF8String.h"

#pragma Internal string helper for fast UTF8 buffer access

inline __SQLite3UTF8String __SQLite3UTF8StringMake(CFAllocatorRef allocator, CFStringRef string) {
  __SQLite3UTF8String utf8String;
  utf8String.allocator = allocator;
  utf8String.string = string;
  utf8String.maximumSize = CFStringGetMaximumSizeForEncoding(CFStringGetLength(string), kCFStringEncodingUTF8) + 1;
  if ((utf8String.pointer = (const char *)CFStringGetCStringPtr(string, kCFStringEncodingUTF8))) {
    utf8String.buffer = NULL;
  } else {
    utf8String.buffer = CFAllocatorAllocate(allocator, utf8String.maximumSize, 0);
    if (utf8String.buffer) {
      CFStringGetCString(string, (char *)utf8String.buffer, utf8String.maximumSize, kCFStringEncodingUTF8);
    }
  }
  return utf8String;
}

inline const char *__SQLite3UTF8StringGetBuffer(__SQLite3UTF8String utf8String) {
  return utf8String.pointer ? utf8String.pointer : utf8String.buffer;
}

inline CFIndex __SQLite3UTF8StringGetMaximumSize(__SQLite3UTF8String utf8String) {
  return utf8String.maximumSize;
}

inline void __SQLite3UTF8StringDestroy(__SQLite3UTF8String utf8String) {
  if (utf8String.buffer)
    CFAllocatorDeallocate(utf8String.allocator, (void *)utf8String.buffer);
}
