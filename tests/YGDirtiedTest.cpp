/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BindYoga.h>

static void _dirtied(BNDYGNodeRef node) {
  int* dirtiedCount = (int*)node->getContext();
  (*dirtiedCount)++;
}

TEST(YogaTest, dirtied) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  int dirtiedCount = 0;
  root->setContext(&dirtiedCount);
  root->setDirtiedFunc(_dirtied);

  ASSERT_EQ(0, dirtiedCount);

  // `_dirtied` MUST be called in case of explicit dirtying.
  root->setDirty(true);
  ASSERT_EQ(1, dirtiedCount);

  // `_dirtied` MUST be called ONCE.
  root->setDirty(true);
  ASSERT_EQ(1, dirtiedCount);
}

TEST(YogaTest, dirtied_propagation) {
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

  int dirtiedCount = 0;
  root->setContext(&dirtiedCount);
  root->setDirtiedFunc(_dirtied);

  ASSERT_EQ(0, dirtiedCount);

  // `_dirtied` MUST be called for the first time.
  root_child0->markDirtyAndPropogate();
  ASSERT_EQ(1, dirtiedCount);

  // `_dirtied` must NOT be called for the second time.
  root_child0->markDirtyAndPropogate();
  ASSERT_EQ(1, dirtiedCount);
}

TEST(YogaTest, dirtied_hierarchy) {
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

  int dirtiedCount = 0;
  root_child0->setContext(&dirtiedCount);
  root_child0->setDirtiedFunc(_dirtied);

  ASSERT_EQ(0, dirtiedCount);

  // `_dirtied` must NOT be called for descendants.
  root->markDirtyAndPropogate();
  ASSERT_EQ(0, dirtiedCount);

  // `_dirtied` must NOT be called for the sibling node.
  root_child1->markDirtyAndPropogate();
  ASSERT_EQ(0, dirtiedCount);

  // `_dirtied` MUST be called in case of explicit dirtying.
  root_child0->markDirtyAndPropogate();
  ASSERT_EQ(1, dirtiedCount);
}
