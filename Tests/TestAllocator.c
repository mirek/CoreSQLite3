//
//  TestAllocator.c
//  SQLite3
//
//  Created by Mirek Rusin on 21/02/2011.
//  Copyright 2011 Inteliv Ltd. All rights reserved.
//

#include "TestAllocator.h"

CFAllocatorRef TestAllocatorCreate() {
  TestAllocatorInfoRef testAllocatorInfo = malloc(sizeof(TestAllocatorInfo));
  testAllocatorInfo->retainsCount = 0;
  testAllocatorInfo->releasesCount = 0;
  testAllocatorInfo->allocationsCount = 0;
  testAllocatorInfo->reallocationsCount = 0;
  testAllocatorInfo->deallocationsCount = 0;
  
  CFAllocatorContext *context = (CFAllocatorContext *)malloc(sizeof(CFAllocatorContext));
	context->info = testAllocatorInfo;
	context->version = 0;
	context->retain = TestAllocatorRetainCall;
	context->release = TestAllocatorReleaseCall;
	context->copyDescription = TestAllocatorCreateDescriptionCall;
	context->allocate = TestAllocatorAllocateCall;
	context->reallocate = TestAllocatorReallocateCall;
	context->deallocate = TestAllocatorDeallocateCall;
	context->preferredSize = TestAllocatorPreferedSizeCall;
	CFAllocatorRef allocator = CFAllocatorCreate(kCFAllocatorDefault, context);
  
  testAllocatorInfo->allocator = allocator;
  
	return allocator;
}

void TestAllocatorRelease(CFAllocatorRef allocator) {
  CFAllocatorContext *context = NULL;
  CFAllocatorGetContext(allocator, context);
  free(context->info);
  free(context);
  CFRelease(allocator);
}

const void *TestAllocatorRetainCall(const void *info) {
  printf("* [retain]\n");
	((TestAllocatorInfoRef)info)->retainsCount++;
	return info;
}

void TestAllocatorReleaseCall(const void *info) {
  printf("* [release]\n");
	((TestAllocatorInfoRef)info)->releasesCount++;
}

CFStringRef TestAllocatorCreateDescriptionCall(const void *info) {
  return CFStringCreateWithFormat(NULL, NULL, CFSTR("<TestAllocator %i retains, %i releases, %i allocations, %i reallocations, %i deallocations>"),
                                  ((TestAllocatorInfoRef)info)->retainsCount,
                                  ((TestAllocatorInfoRef)info)->releasesCount,
                                  ((TestAllocatorInfoRef)info)->allocationsCount,
                                  ((TestAllocatorInfoRef)info)->reallocationsCount,
                                  ((TestAllocatorInfoRef)info)->deallocationsCount);
}

void *TestAllocatorAllocateCall(CFIndex allocSize, CFOptionFlags hint, void *info) {
  printf("* [%ld/%ld alloc, %ld bytes]\n", ++((TestAllocatorInfoRef)info)->allocationsCount, ((TestAllocatorInfoRef)info)->deallocationsCount, allocSize);
  return CFAllocatorAllocate(NULL, allocSize, hint);
}

void *TestAllocatorReallocateCall(void *ptr, CFIndex newsize, CFOptionFlags hint, void *info) {
  printf("* [%ld realloc, %ld bytes]\n", ++((TestAllocatorInfoRef)info)->reallocationsCount, newsize);
  return CFAllocatorReallocate(NULL, ptr, newsize, hint);
}

void TestAllocatorDeallocateCall(void *ptr, void *info) {
  printf("* [%ld/%ld dealloc]\n", ((TestAllocatorInfoRef)info)->allocationsCount, ++((TestAllocatorInfoRef)info)->deallocationsCount);
  CFAllocatorDeallocate(NULL, ptr);
}

CFIndex TestAllocatorPreferedSizeCall (CFIndex size, CFOptionFlags hint, void *info) {
  return CFAllocatorGetPreferredSizeForSize(NULL, size, hint);
}
