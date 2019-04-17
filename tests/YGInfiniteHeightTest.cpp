/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BindYoga.h>

// This test isn't correct from the Flexbox standard standpoint,
// because percentages are calculated with parent constraints.
// However, we need to make sure we fail gracefully in this case, not returning NaN
TEST(YogaTest, percent_absolute_position_infinite_height) {
  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetWidth(root, 300);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(root_child0, 300);
  BNDYGNodeStyleSetHeight(root_child0, 300);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetPositionType(root_child1, BNDYGPositionTypeAbsolute);
  BNDYGNodeStyleSetPositionPercent(root_child1, BNDYGEdgeLeft, 20);
  BNDYGNodeStyleSetPositionPercent(root_child1, BNDYGEdgeTop, 20);
  BNDYGNodeStyleSetWidthPercent(root_child1, 20);
  BNDYGNodeStyleSetHeightPercent(root_child1, 20);
  BNDYGNodeInsertChild(root, root_child1, 1);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(300, BNDYGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(300, BNDYGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(300, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(300, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(60, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(60, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetHeight(root_child1));

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}
