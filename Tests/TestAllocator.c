//
//  TestAllocator.c
//  SQLite3
//
//  Created by Mirek Rusin on 21/02/2011.
//  Copyright 2011 Inteliv Ltd. All rights reserved.
//

#include "TestAllocator.h"

CFAllocatorRef TestAllocatorCreate() {
  TestAllocatorInfoRef testAllocatorInfo = CFAllocatorAllocate(NULL, sizeof(TestAllocatorInfo), 0);
  testAllocatorInfo->retainCount = 1;
  testAllocatorInfo->allocationsCount = 0;
  testAllocatorInfo->reallocationsCount = 0;
  testAllocatorInfo->deallocationsCount = 0;
  
  CFAllocatorContext *context = (CFAllocatorContext *)CFAllocatorAllocate(NULL, sizeof(CFAllocatorContext), 0);
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

const void *TestAllocatorRetainCall(const void *info) {
	((TestAllocatorInfoRef)info)->retainCount++;
	return info;
}

void TestAllocatorReleaseCall(const void *info) {
	if (0 == --((TestAllocatorInfoRef)info)->retainCount) {
    CFAllocatorDeallocate(NULL, (void *)info);
  }
}

CFStringRef TestAllocatorCreateDescriptionCall(const void *info) {
  return CFStringCreateWithFormat(NULL, NULL, CFSTR("<TestAllocator %i retain count, %i allocations, %i deallocations, %i reallocations, >"),
                                  ((TestAllocatorInfoRef)info)->retainCount,
                                  ((TestAllocatorInfoRef)info)->allocationsCount,
                                  ((TestAllocatorInfoRef)info)->deallocationsCount,
                                  ((TestAllocatorInfoRef)info)->reallocationsCount);
}

void *TestAllocatorAllocateCall(CFIndex allocSize, CFOptionFlags hint, void *info) {
  return CFAllocatorAllocate(NULL, allocSize, hint);
}

void *TestAllocatorReallocateCall(void *ptr, CFIndex newsize, CFOptionFlags hint, void *info) {
  return CFAllocatorReallocate(NULL, ptr, newsize, hint);
}

void TestAllocatorDeallocateCall(void *ptr, void *info) {
  CFAllocatorDeallocate(NULL, ptr);
}

CFIndex TestAllocatorPreferedSizeCall (CFIndex size, CFOptionFlags hint, void *info) {
  return CFAllocatorGetPreferredSizeForSize(NULL, size, hint);
}

CFIndex TestAllocatorGetAllocationsCount(CFAllocatorRef allocator) {
  CFIndex allocationsCount = -1;
  CFAllocatorContext context;
  CFAllocatorGetContext(allocator, &context);
  allocationsCount = ((TestAllocatorInfoRef)context.info)->allocationsCount;
  return allocationsCount;
}

CFIndex TestAllocatorGetDeallocationsCount(CFAllocatorRef allocator) {
  CFIndex deallocationsCount = -1;
  CFAllocatorContext context;
  CFAllocatorGetContext(allocator, &context);
  deallocationsCount = ((TestAllocatorInfoRef)context.info)->deallocationsCount;
  return deallocationsCount;
}
