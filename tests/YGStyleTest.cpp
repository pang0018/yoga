/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>

#include <bindyoga/BNDYGNode.h>
#include <bindyoga/BindYoga.h>
#include <iostream>

TEST(YogaTest, copy_style_same) {
  const BNDYGNodeRef node0 = BNDYGNodeNew();
  const BNDYGNodeRef node1 = BNDYGNodeNew();
  ASSERT_FALSE(node0->isDirty());

  BNDYGNodeCopyStyle(node0, node1);
  ASSERT_FALSE(node0->isDirty());

  BNDYGNodeFree(node0);
  BNDYGNodeFree(node1);
}

TEST(YogaTest, copy_style_modified) {
  const BNDYGNodeRef node0 = BNDYGNodeNew();
  ASSERT_FALSE(node0->isDirty());
  ASSERT_EQ(BNDYGFlexDirectionColumn, BNDYGNodeStyleGetFlexDirection(node0));
  ASSERT_FALSE(BNDYGNodeStyleGetMaxHeight(node0).unit != BNDYGUnitUndefined);

  const BNDYGNodeRef node1 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(node1, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetMaxHeight(node1, 10);

  BNDYGNodeCopyStyle(node0, node1);
  ASSERT_TRUE(node0->isDirty());
  ASSERT_EQ(BNDYGFlexDirectionRow, BNDYGNodeStyleGetFlexDirection(node0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeStyleGetMaxHeight(node0).value);

  BNDYGNodeFree(node0);
  BNDYGNodeFree(node1);
}

TEST(YogaTest, copy_style_modified_same) {
  const BNDYGNodeRef node0 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(node0, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetMaxHeight(node0, 10);
  BNDYGNodeCalculateLayout(node0, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);
  ASSERT_FALSE(node0->isDirty());

  const BNDYGNodeRef node1 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(node1, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetMaxHeight(node1, 10);

  BNDYGNodeCopyStyle(node0, node1);
  ASSERT_FALSE(node0->isDirty());

  BNDYGNodeFree(node0);
  BNDYGNodeFree(node1);
}

TEST(YogaTest, initialise_flexShrink_flexGrow) {
  const BNDYGNodeRef node0 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexShrink(node0, 1);
  ASSERT_EQ(1, BNDYGNodeStyleGetFlexShrink(node0));

  BNDYGNodeStyleSetFlexShrink(node0, BNDYGUndefined);
  BNDYGNodeStyleSetFlexGrow(node0, 3);
  ASSERT_EQ(
      0,
      BNDYGNodeStyleGetFlexShrink(
          node0)); // Default value is Zero, if flex shrink is not defined
  ASSERT_EQ(3, BNDYGNodeStyleGetFlexGrow(node0));

  BNDYGNodeStyleSetFlexGrow(node0, BNDYGUndefined);
  BNDYGNodeStyleSetFlexShrink(node0, 3);
  ASSERT_EQ(
      0,
      BNDYGNodeStyleGetFlexGrow(
          node0)); // Default value is Zero, if flex grow is not defined
  ASSERT_EQ(3, BNDYGNodeStyleGetFlexShrink(node0));
  BNDYGNodeFree(node0);
}
