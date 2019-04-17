/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BindYoga.h>

TEST(YogaTest, cloning_shared_root) {
  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetFlexBasis(root_child0, 50);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexGrow(root_child1, 1);
  BNDYGNodeInsertChild(root, root_child1, 1);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, BNDYGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, BNDYGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(75, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(75, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(100, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetHeight(root_child1));

  const BNDYGNodeRef root2 = BNDYGNodeClone(root);
  BNDYGNodeStyleSetWidth(root2, 100);

  ASSERT_EQ(2, BNDYGNodeGetChildCount(root2));
  // The children should have referential equality at this point.
  ASSERT_EQ(root_child0, BNDYGNodeGetChild(root2, 0));
  ASSERT_EQ(root_child1, BNDYGNodeGetChild(root2, 1));

  BNDYGNodeCalculateLayout(root2, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(2, BNDYGNodeGetChildCount(root2));
  // Relayout with no changed input should result in referential equality.
  ASSERT_EQ(root_child0, BNDYGNodeGetChild(root2, 0));
  ASSERT_EQ(root_child1, BNDYGNodeGetChild(root2, 1));

  BNDYGNodeStyleSetWidth(root2, 150);
  BNDYGNodeStyleSetHeight(root2, 200);
  BNDYGNodeCalculateLayout(root2, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(2, BNDYGNodeGetChildCount(root2));
  // Relayout with changed input should result in cloned children.
  const BNDYGNodeRef root2_child0 = BNDYGNodeGetChild(root2, 0);
  const BNDYGNodeRef root2_child1 = BNDYGNodeGetChild(root2, 1);
  ASSERT_NE(root_child0, root2_child0);
  ASSERT_NE(root_child1, root2_child1);

  // Everything in the root should remain unchanged.
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, BNDYGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, BNDYGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(75, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(75, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(100, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetHeight(root_child1));

  // The new root now has new layout.
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root2));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root2));
  ASSERT_FLOAT_EQ(150, BNDYGNodeLayoutGetWidth(root2));
  ASSERT_FLOAT_EQ(200, BNDYGNodeLayoutGetHeight(root2));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root2_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root2_child0));
  ASSERT_FLOAT_EQ(150, BNDYGNodeLayoutGetWidth(root2_child0));
  ASSERT_FLOAT_EQ(125, BNDYGNodeLayoutGetHeight(root2_child0));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root2_child1));
  ASSERT_FLOAT_EQ(125, BNDYGNodeLayoutGetTop(root2_child1));
  ASSERT_FLOAT_EQ(150, BNDYGNodeLayoutGetWidth(root2_child1));
  ASSERT_FLOAT_EQ(75, BNDYGNodeLayoutGetHeight(root2_child1));

  BNDYGNodeFreeRecursive(root2);

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}

TEST(YogaTest, mutating_children_of_a_clone_clones) {
  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  ASSERT_EQ(0, BNDYGNodeGetChildCount(root));

  const BNDYGNodeRef root2 = BNDYGNodeClone(root);
  ASSERT_EQ(0, BNDYGNodeGetChildCount(root2));

  const BNDYGNodeRef root2_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeInsertChild(root2, root2_child0, 0);

  ASSERT_EQ(0, BNDYGNodeGetChildCount(root));
  ASSERT_EQ(1, BNDYGNodeGetChildCount(root2));

  const BNDYGNodeRef root3 = BNDYGNodeClone(root2);
  ASSERT_EQ(1, BNDYGNodeGetChildCount(root2));
  ASSERT_EQ(1, BNDYGNodeGetChildCount(root3));
  ASSERT_EQ(BNDYGNodeGetChild(root2, 0), BNDYGNodeGetChild(root3, 0));

  const BNDYGNodeRef root3_child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeInsertChild(root3, root3_child1, 1);
  ASSERT_EQ(1, BNDYGNodeGetChildCount(root2));
  ASSERT_EQ(2, BNDYGNodeGetChildCount(root3));
  ASSERT_EQ(root3_child1, BNDYGNodeGetChild(root3, 1));
  ASSERT_NE(BNDYGNodeGetChild(root2, 0), BNDYGNodeGetChild(root3, 0));

  const BNDYGNodeRef root4 = BNDYGNodeClone(root3);
  ASSERT_EQ(root3_child1, BNDYGNodeGetChild(root4, 1));

  BNDYGNodeRemoveChild(root4, root3_child1);
  ASSERT_EQ(2, BNDYGNodeGetChildCount(root3));
  ASSERT_EQ(1, BNDYGNodeGetChildCount(root4));
  ASSERT_NE(BNDYGNodeGetChild(root3, 0), BNDYGNodeGetChild(root4, 0));

  BNDYGNodeFreeRecursive(root4);
  BNDYGNodeFreeRecursive(root3);
  BNDYGNodeFreeRecursive(root2);
  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}

TEST(YogaTest, cloning_two_levels) {
  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetFlexBasis(root_child0, 15);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexGrow(root_child1, 1);
  BNDYGNodeInsertChild(root, root_child1, 1);

  const BNDYGNodeRef root_child1_0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexBasis(root_child1_0, 10);
  BNDYGNodeStyleSetFlexGrow(root_child1_0, 1);
  BNDYGNodeInsertChild(root_child1, root_child1_0, 0);

  const BNDYGNodeRef root_child1_1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexBasis(root_child1_1, 25);
  BNDYGNodeInsertChild(root_child1, root_child1_1, 1);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(40, BNDYGNodeLayoutGetHeight(root_child0));
  ASSERT_FLOAT_EQ(60, BNDYGNodeLayoutGetHeight(root_child1));
  ASSERT_FLOAT_EQ(35, BNDYGNodeLayoutGetHeight(root_child1_0));
  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetHeight(root_child1_1));

  const BNDYGNodeRef root2_child0 = BNDYGNodeClone(root_child0);
  const BNDYGNodeRef root2_child1 = BNDYGNodeClone(root_child1);
  const BNDYGNodeRef root2 = BNDYGNodeClone(root);

  BNDYGNodeStyleSetFlexGrow(root2_child0, 0);
  BNDYGNodeStyleSetFlexBasis(root2_child0, 40);

  BNDYGNodeRemoveAllChildren(root2);
  BNDYGNodeInsertChild(root2, root2_child0, 0);
  BNDYGNodeInsertChild(root2, root2_child1, 1);
  ASSERT_EQ(2, BNDYGNodeGetChildCount(root2));

  BNDYGNodeCalculateLayout(root2, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  // Original root is unchanged
  ASSERT_FLOAT_EQ(40, BNDYGNodeLayoutGetHeight(root_child0));
  ASSERT_FLOAT_EQ(60, BNDYGNodeLayoutGetHeight(root_child1));
  ASSERT_FLOAT_EQ(35, BNDYGNodeLayoutGetHeight(root_child1_0));
  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetHeight(root_child1_1));

  // New root has new layout at the top
  ASSERT_FLOAT_EQ(40, BNDYGNodeLayoutGetHeight(root2_child0));
  ASSERT_FLOAT_EQ(60, BNDYGNodeLayoutGetHeight(root2_child1));

  // The deeper children are untouched.
  ASSERT_EQ(BNDYGNodeGetChild(root2_child1, 0), root_child1_0);
  ASSERT_EQ(BNDYGNodeGetChild(root2_child1, 1), root_child1_1);

  BNDYGNodeFreeRecursive(root2);
  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}

TEST(YogaTest, cloning_and_freeing) {
  const int32_t initialInstanceCount = BNDYGNodeGetInstanceCount();

  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);
  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeInsertChild(root, root_child0, 0);
  const BNDYGNodeRef root_child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeInsertChild(root, root_child1, 1);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  const BNDYGNodeRef root2 = BNDYGNodeClone(root);

  // Freeing the original root should be safe as long as we don't free its
  // children.
  BNDYGNodeFree(root);

  BNDYGNodeCalculateLayout(root2, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  BNDYGNodeFreeRecursive(root2);

  BNDYGNodeFree(root_child0);
  BNDYGNodeFree(root_child1);

  BNDYGConfigFree(config);

  ASSERT_EQ(initialInstanceCount, BNDYGNodeGetInstanceCount());
}
