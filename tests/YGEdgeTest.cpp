/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BindYoga.h>

TEST(YogaTest, start_overrides) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeStart, 10);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeLeft, 20);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeRight, 20);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetRight(root_child0));

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionRTL);
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetRight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, end_overrides) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeEnd, 10);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeLeft, 20);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeRight, 20);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetRight(root_child0));

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionRTL);
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetRight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, horizontal_overridden) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeHorizontal, 10);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeLeft, 20);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetRight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, vertical_overridden) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionColumn);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeVertical, 10);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeTop, 20);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetBottom(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, horizontal_overrides_all) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionColumn);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeHorizontal, 10);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeAll, 20);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetRight(root_child0));
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetBottom(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, vertical_overrides_all) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionColumn);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeVertical, 10);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeAll, 20);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetRight(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetBottom(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, all_overridden) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionColumn);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeLeft, 10);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeTop, 10);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeRight, 10);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeBottom, 10);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeAll, 20);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetRight(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetBottom(root_child0));

  BNDYGNodeFreeRecursive(root);
}
