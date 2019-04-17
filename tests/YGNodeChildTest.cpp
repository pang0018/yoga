/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BindYoga.h>

TEST(YogaTest, reset_layout_when_child_removed) {
  const BNDYGNodeRef root = BNDYGNodeNew();

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 100);
  BNDYGNodeStyleSetHeight(root_child0, 100);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeRemoveChild(root, root_child0);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_TRUE(BNDYGFloatIsUndefined(BNDYGNodeLayoutGetWidth(root_child0)));
  ASSERT_TRUE(BNDYGFloatIsUndefined(BNDYGNodeLayoutGetHeight(root_child0)));

  BNDYGNodeFreeRecursive(root);
}
