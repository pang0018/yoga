/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#import "UIView+BindYoga.h"
#import "BNDYGLayout+Private.h"
#import <objc/runtime.h>

static const void *kBNDYGYogaAssociatedKey = &kBNDYGYogaAssociatedKey;

@implementation UIView (YogaKit)

- (BNDYGLayout *)yoga
{
  BNDYGLayout *yoga = objc_getAssociatedObject(self, kBNDYGYogaAssociatedKey);
  if (!yoga) {
    yoga = [[BNDYGLayout alloc] initWithView:self];
    objc_setAssociatedObject(self, kBNDYGYogaAssociatedKey, yoga, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }

  return yoga;
}

- (BOOL)isYogaEnabled
{
  return objc_getAssociatedObject(self, kBNDYGYogaAssociatedKey) != nil;
}

- (void)configureLayoutWithBlock:(BNDYGLayoutConfigurationBlock)block
{
  if (block != nil) {
    block(self.yoga);
  }
}

@end
