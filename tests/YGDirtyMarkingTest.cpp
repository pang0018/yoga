/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BindYoga.h>

TEST(YogaTest, dirty_propagation) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 50);
  BNDYGNodeStyleSetHeight(root_child0, 20);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child1, 50);
  BNDYGNodeStyleSetHeight(root_child1, 20);
  BNDYGNodeInsertChild(root, root_child1, 1);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  BNDYGNodeStyleSetWidth(root_child0, 20);

  EXPECT_TRUE(root_child0->isDirty());
  EXPECT_FALSE(root_child1->isDirty());
  EXPECT_TRUE(root->isDirty());

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  EXPECT_FALSE(root_child0->isDirty());
  EXPECT_FALSE(root_child1->isDirty());
  EXPECT_FALSE(root->isDirty());

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, dirty_propagation_only_if_prop_changed) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 50);
  BNDYGNodeStyleSetHeight(root_child0, 20);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child1, 50);
  BNDYGNodeStyleSetHeight(root_child1, 20);
  BNDYGNodeInsertChild(root, root_child1, 1);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  BNDYGNodeStyleSetWidth(root_child0, 50);

  EXPECT_FALSE(root_child0->isDirty());
  EXPECT_FALSE(root_child1->isDirty());
  EXPECT_FALSE(root->isDirty());

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, dirty_mark_all_children_as_dirty_when_display_changes) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexGrow(child0, 1);
  const BNDYGNodeRef child1 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexGrow(child1, 1);

  const BNDYGNodeRef child1_child0 = BNDYGNodeNew();
  const BNDYGNodeRef child1_child0_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(child1_child0_child0, 8);
  BNDYGNodeStyleSetHeight(child1_child0_child0, 16);

  BNDYGNodeInsertChild(child1_child0, child1_child0_child0, 0);

  BNDYGNodeInsertChild(child1, child1_child0, 0);
  BNDYGNodeInsertChild(root, child0, 0);
  BNDYGNodeInsertChild(root, child1, 0);

  BNDYGNodeStyleSetDisplay(child0, BNDYGDisplayFlex);
  BNDYGNodeStyleSetDisplay(child1, BNDYGDisplayNone);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetWidth(child1_child0_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetHeight(child1_child0_child0));

  BNDYGNodeStyleSetDisplay(child0, BNDYGDisplayNone);
  BNDYGNodeStyleSetDisplay(child1, BNDYGDisplayFlex);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);
  ASSERT_FLOAT_EQ(8, BNDYGNodeLayoutGetWidth(child1_child0_child0));
  ASSERT_FLOAT_EQ(16, BNDYGNodeLayoutGetHeight(child1_child0_child0));

  BNDYGNodeStyleSetDisplay(child0, BNDYGDisplayFlex);
  BNDYGNodeStyleSetDisplay(child1, BNDYGDisplayNone);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetWidth(child1_child0_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetHeight(child1_child0_child0));

  BNDYGNodeStyleSetDisplay(child0, BNDYGDisplayNone);
  BNDYGNodeStyleSetDisplay(child1, BNDYGDisplayFlex);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);
  ASSERT_FLOAT_EQ(8, BNDYGNodeLayoutGetWidth(child1_child0_child0));
  ASSERT_FLOAT_EQ(16, BNDYGNodeLayoutGetHeight(child1_child0_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, dirty_node_only_if_children_are_actually_removed) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 50);
  BNDYGNodeStyleSetHeight(root, 50);

  const BNDYGNodeRef child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(child0, 50);
  BNDYGNodeStyleSetHeight(child0, 25);
  BNDYGNodeInsertChild(root, child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  const BNDYGNodeRef child1 = BNDYGNodeNew();
  BNDYGNodeRemoveChild(root, child1);
  EXPECT_FALSE(root->isDirty());
  BNDYGNodeFree(child1);

  BNDYGNodeRemoveChild(root, child0);
  EXPECT_TRUE(root->isDirty());
  BNDYGNodeFree(child0);

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, dirty_node_only_if_undefined_values_gets_set_to_undefined) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 50);
  BNDYGNodeStyleSetHeight(root, 50);
  BNDYGNodeStyleSetMinWidth(root, BNDYGUndefined);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  EXPECT_FALSE(root->isDirty());

  BNDYGNodeStyleSetMinWidth(root, BNDYGUndefined);

  EXPECT_FALSE(root->isDirty());

  BNDYGNodeFreeRecursive(root);
}
