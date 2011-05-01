//
//  SQLite3ExtMath.c
//  SQLite3
//
//  Created by Mirek Rusin on 20/04/2011.
//  Copyright 2011 Inteliv Ltd. All rights reserved.
//

#include "SQLite3ExtMath.h"

void __SQLite3ExtensionsMathMD5(sqlite3_context *context, int argc, sqlite3_value **argv) {
  if (argc == 1 && sqlite3_value_type(argv[0]) != SQLITE_NULL) {
    sqlite3_result_double(context, sin(sqlite3_value_double(argv[0])));
    
//#if (TARGET_OS_IPHONE)
//      CC_MD5_CTX mdctx;
//      CC_MD5_Init(&mdctx);
//      unsigned char buffer[CC_MD5_DIGEST_LENGTH];
//      CC_MD5_Update(&mdctx, CFDataGetBytePtr(data), CFDataGetLength(data));
//      CC_MD5_Final(buffer, &mdctx);
//      //return CFDataCreate(allocator, buffer, CC_MD5_DIGEST_LENGTH);
//#else
//      EVP_MD_CTX mdctx;
//      unsigned char buffer[EVP_MAX_MD_SIZE];
//      unsigned int length = 0;
//      CFMutableDataRef data = CFDataCreateMutable(allocator, 0);
//      EVP_DigestInit(&mdctx, EVP_md5());
//      __WebSocketDataAppendKey(data, key1);
//      __WebSocketDataAppendKey(data, key2);
//      CFDataAppendBytes(data, CFDataGetBytePtr(key3), CFDataGetLength(key3));
//      EVP_DigestUpdate(&mdctx, CFDataGetBytePtr(data), CFDataGetLength(data));
//      EVP_DigestFinal(&mdctx, buffer, &length);
//      CFRelease(data);
//      return CFDataCreate(allocator, buffer, length);
//#endif
  } else {
    sqlite3_result_null(context);
  }
}

SQLite3Status SQLite3ExtensionsMathRegisterMD5(SQLite3ConnectionRef connection) {
  return SQLite3ConnectionRegisterFunction(connection, CFSTR("md5"), 1, __SQLite3ExtensionsMathSin);
}

SQLite3Status SQLite3ExtensionsMathUnregisterMD5(SQLite3ConnectionRef connection) {
  return SQLite3ConnectionUnregisterFunction(connection, CFSTR("md5"), 1);
}

//void __SQLite3ExtensionsMathSHA1(sqlite3_context *context, int argc, sqlite3_value **argv) {
//  if (argc == 1 && sqlite3_value_type(argv[0]) != SQLITE_NULL) {
//    sqlite3_result_double(context, sin(sqlite3_value_double(argv[0])));
//  } else {
//    sqlite3_result_null(context);
//  }
//}
//
//SQLite3Status SQLite3ExtensionsMathRegisterSHA1(SQLite3ConnectionRef connection) {
//  return SQLite3ConnectionRegisterFunction(connection, CFSTR("sha1"), 1, __SQLite3ExtensionsMathSin);
//}
//
//SQLite3Status SQLite3ExtensionsMathUnregisterSHA1(SQLite3ConnectionRef connection) {
//  return SQLite3ConnectionUnregisterFunction(connection, CFSTR("sha1"), 1);
//}
