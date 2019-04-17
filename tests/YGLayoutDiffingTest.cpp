/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BNDYGNode.h>
#include <bindyoga/BindYoga.h>

TEST(YogaTest, assert_layout_trees_are_same) {
  BNDYGConfig* config = BNDYGConfigNew();
  BNDYGConfigSetUseLegacyStretchBehaviour(config, true);
  const BNDYGNodeRef root1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(root1, 500);
  BNDYGNodeStyleSetHeight(root1, 500);

  const BNDYGNodeRef root1_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetAlignItems(root1_child0, BNDYGAlignFlexStart);
  BNDYGNodeInsertChild(root1, root1_child0, 0);

  const BNDYGNodeRef root1_child0_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexGrow(root1_child0_child0, 1);
  BNDYGNodeStyleSetFlexShrink(root1_child0_child0, 1);
  BNDYGNodeInsertChild(root1_child0, root1_child0_child0, 0);

  const BNDYGNodeRef root1_child0_child0_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexGrow(root1_child0_child0_child0, 1);
  BNDYGNodeStyleSetFlexShrink(root1_child0_child0_child0, 1);
  BNDYGNodeInsertChild(root1_child0_child0, root1_child0_child0_child0, 0);

  const int32_t cal1_configInstanceCount = BNDYGConfigGetInstanceCount();
  const int32_t cal1_nodeInstanceCount = BNDYGNodeGetInstanceCount();

  BNDYGNodeCalculateLayout(root1, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(BNDYGConfigGetInstanceCount(), cal1_configInstanceCount);
  ASSERT_EQ(BNDYGNodeGetInstanceCount(), cal1_nodeInstanceCount);

  const BNDYGNodeRef root2 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(root2, 500);
  BNDYGNodeStyleSetHeight(root2, 500);

  const BNDYGNodeRef root2_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetAlignItems(root2_child0, BNDYGAlignFlexStart);
  BNDYGNodeInsertChild(root2, root2_child0, 0);

  const BNDYGNodeRef root2_child0_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexGrow(root2_child0_child0, 1);
  BNDYGNodeStyleSetFlexShrink(root2_child0_child0, 1);
  BNDYGNodeInsertChild(root2_child0, root2_child0_child0, 0);

  const BNDYGNodeRef root2_child0_child0_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexGrow(root2_child0_child0_child0, 1);
  BNDYGNodeStyleSetFlexShrink(root2_child0_child0_child0, 1);
  BNDYGNodeInsertChild(root2_child0_child0, root2_child0_child0_child0, 0);

  const int32_t cal2_configInstanceCount = BNDYGConfigGetInstanceCount();
  const int32_t cal2_nodeInstanceCount = BNDYGNodeGetInstanceCount();

  BNDYGNodeCalculateLayout(root2, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(BNDYGConfigGetInstanceCount(), cal2_configInstanceCount);
  ASSERT_EQ(BNDYGNodeGetInstanceCount(), cal2_nodeInstanceCount);

  ASSERT_TRUE(BNDYGNodeLayoutGetDidUseLegacyFlag(root1));
  ASSERT_TRUE(BNDYGNodeLayoutGetDidUseLegacyFlag(root2));
  ASSERT_TRUE(root1->isLayoutTreeEqualToNode(*root2));

  BNDYGNodeStyleSetAlignItems(root2, BNDYGAlignFlexEnd);

  const int32_t cal3_configInstanceCount = BNDYGConfigGetInstanceCount();
  const int32_t cal3_nodeInstanceCount = BNDYGNodeGetInstanceCount();

  BNDYGNodeCalculateLayout(root2, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(BNDYGConfigGetInstanceCount(), cal3_configInstanceCount);
  ASSERT_EQ(BNDYGNodeGetInstanceCount(), cal3_nodeInstanceCount);

  ASSERT_FALSE(root1->isLayoutTreeEqualToNode(*root2));

  BNDYGNodeFreeRecursive(root1);
  BNDYGNodeFreeRecursive(root2);

  BNDYGConfigFree(config);
}
