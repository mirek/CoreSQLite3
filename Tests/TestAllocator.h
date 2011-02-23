//
//  TestAllocator.h
//  SQLite3
//
//  Created by Mirek Rusin on 21/02/2011.
//  Copyright 2011 Inteliv Ltd. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>

typedef struct TestAllocatorInfo {
  CFAllocatorRef allocator;
  CFIndex retainsCount;
  CFIndex releasesCount;
  CFIndex allocationsCount;
  CFIndex reallocationsCount;
  CFIndex deallocationsCount;
} TestAllocatorInfo;

typedef TestAllocatorInfo *TestAllocatorInfoRef;

CFAllocatorRef TestAllocatorCreate                ();
void           TestAllocatorRelease               (CFAllocatorRef allocator);

const void    *TestAllocatorRetainCall            (const void *info);
void           TestAllocatorReleaseCall           (const void *info);
CFStringRef    TestAllocatorCreateDescriptionCall (const void *info);
void          *TestAllocatorAllocateCall          (CFIndex allocSize, CFOptionFlags hint, void *info);
void          *TestAllocatorReallocateCall        (void *ptr, CFIndex newsize, CFOptionFlags hint, void *info);
void           TestAllocatorDeallocateCall        (void *ptr, void *info);
CFIndex        TestAllocatorPreferedSizeCall      (CFIndex size, CFOptionFlags hint, void *info);
