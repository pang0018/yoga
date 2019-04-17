/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BindYoga.h>

TEST(YogaTest, computed_layout_margin) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);
  BNDYGNodeStyleSetMarginPercent(root, BNDYGEdgeStart, 10);

  BNDYGNodeCalculateLayout(root, 100, 100, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetMargin(root, BNDYGEdgeLeft));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetMargin(root, BNDYGEdgeRight));

  BNDYGNodeCalculateLayout(root, 100, 100, BNDYGDirectionRTL);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetMargin(root, BNDYGEdgeLeft));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetMargin(root, BNDYGEdgeRight));

  BNDYGNodeFreeRecursive(root);
}
