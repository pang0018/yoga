/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>

#include <bindyoga/BNDBNDYGNode.h>
#include <bindyoga/BindYoga.h>

TEST(YogaTest, zero_out_layout) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetWidth(root, 200);
  BNDYGNodeStyleSetHeight(root, 200);

  const BNDYGNodeRef child = BNDYGNodeNew();
  BNDYGNodeInsertChild(root, child, 0);
  BNDYGNodeStyleSetWidth(child, 100);
  BNDYGNodeStyleSetHeight(child, 100);
  BNDYGNodeStyleSetMargin(child, BNDYGEdgeTop, 10);
  BNDYGNodeStyleSetPadding(child, BNDYGEdgeTop, 10);

  BNDYGNodeCalculateLayout(root, 100, 100, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetMargin(child, BNDYGEdgeTop));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetPadding(child, BNDYGEdgeTop));

  BNDYGNodeStyleSetDisplay(child, BNDYGDisplayNone);

  BNDYGNodeCalculateLayout(root, 100, 100, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetMargin(child, BNDYGEdgeTop));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetPadding(child, BNDYGEdgeTop));

  BNDYGNodeFreeRecursive(root);
}
