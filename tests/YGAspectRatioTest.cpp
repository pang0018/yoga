/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BNDYGNode.h>
#include <bindyoga/BindYoga.h>

static BNDYGSize _measure(BNDYGNodeRef node,
                       float width,
                       BNDYGMeasureMode widthMode,
                       float height,
                       BNDYGMeasureMode heightMode) {
  return BNDYGSize{
      .width = widthMode == BNDYGMeasureModeExactly ? width : 50,
      .height = heightMode == BNDYGMeasureModeExactly ? height : 50,
  };
}

TEST(YogaTest, aspect_ratio_cross_defined) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_main_defined) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_both_dimensions_defined_row) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 100);
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_both_dimensions_defined_column) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 100);
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_align_stretch) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_flex_grow) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_flex_shrink) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root_child0, 150);
  BNDYGNodeStyleSetFlexShrink(root_child0, 1);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_flex_shrink_2) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeightPercent(root_child0, 100);
  BNDYGNodeStyleSetFlexShrink(root_child0, 1);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeightPercent(root_child1, 100);
  BNDYGNodeStyleSetFlexShrink(root_child1, 1);
  BNDYGNodeStyleSetAspectRatio(root_child1, 1);
  BNDYGNodeInsertChild(root, root_child1, 1);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_EQ(50, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child1));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_basis) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexBasis(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_absolute_layout_width_defined) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetPositionType(root_child0, BNDYGPositionTypeAbsolute);
  BNDYGNodeStyleSetPosition(root_child0, BNDYGEdgeLeft, 0);
  BNDYGNodeStyleSetPosition(root_child0, BNDYGEdgeTop, 0);
  BNDYGNodeStyleSetWidth(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_absolute_layout_height_defined) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetPositionType(root_child0, BNDYGPositionTypeAbsolute);
  BNDYGNodeStyleSetPosition(root_child0, BNDYGEdgeLeft, 0);
  BNDYGNodeStyleSetPosition(root_child0, BNDYGEdgeTop, 0);
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_with_max_cross_defined) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeStyleSetMaxWidth(root_child0, 40);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(40, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_with_max_main_defined) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 50);
  BNDYGNodeStyleSetMaxHeight(root_child0, 40);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(40, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(40, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_with_min_cross_defined) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root_child0, 30);
  BNDYGNodeStyleSetMinWidth(root_child0, 40);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(40, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(30, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_with_min_main_defined) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 30);
  BNDYGNodeStyleSetMinHeight(root_child0, 40);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(40, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(40, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_double_cross) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 2);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_half_cross) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root_child0, 100);
  BNDYGNodeStyleSetAspectRatio(root_child0, 0.5);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_double_main) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 0.5);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_half_main) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 100);
  BNDYGNodeStyleSetAspectRatio(root_child0, 2);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_with_measure_func) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_width_height_flex_grow_row) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 200);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 50);
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_width_height_flex_grow_column) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 200);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 50);
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_height_as_flex_basis) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetWidth(root, 200);
  BNDYGNodeStyleSetHeight(root, 200);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root_child1, 100);
  BNDYGNodeStyleSetFlexGrow(root_child1, 1);
  BNDYGNodeStyleSetAspectRatio(root_child1, 1);
  BNDYGNodeInsertChild(root, root_child1, 1);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(75, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(75, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_EQ(75, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_EQ(125, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_EQ(125, BNDYGNodeLayoutGetHeight(root_child1));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_width_as_flex_basis) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 200);
  BNDYGNodeStyleSetHeight(root, 200);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 50);
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child1, 100);
  BNDYGNodeStyleSetFlexGrow(root_child1, 1);
  BNDYGNodeStyleSetAspectRatio(root_child1, 1);
  BNDYGNodeInsertChild(root, root_child1, 1);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(75, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(75, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_EQ(75, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_EQ(125, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_EQ(125, BNDYGNodeLayoutGetHeight(root_child1));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_overrides_flex_grow_row) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 50);
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetAspectRatio(root_child0, 0.5);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(200, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_overrides_flex_grow_column) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetAspectRatio(root_child0, 2);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(200, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_left_right_absolute) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetPositionType(root_child0, BNDYGPositionTypeAbsolute);
  BNDYGNodeStyleSetPosition(root_child0, BNDYGEdgeLeft, 10);
  BNDYGNodeStyleSetPosition(root_child0, BNDYGEdgeTop, 10);
  BNDYGNodeStyleSetPosition(root_child0, BNDYGEdgeRight, 10);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(10, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(10, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(80, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(80, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_top_bottom_absolute) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetPositionType(root_child0, BNDYGPositionTypeAbsolute);
  BNDYGNodeStyleSetPosition(root_child0, BNDYGEdgeLeft, 10);
  BNDYGNodeStyleSetPosition(root_child0, BNDYGEdgeTop, 10);
  BNDYGNodeStyleSetPosition(root_child0, BNDYGEdgeBottom, 10);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(10, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(10, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(80, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(80, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_width_overrides_align_stretch_row) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_height_overrides_align_stretch_column) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_allow_child_overflow_parent_size) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 4);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root));

  ASSERT_EQ(200, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_defined_main_with_margin) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignCenter);
  BNDYGNodeStyleSetJustifyContent(root, BNDYGJustifyCenter);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeLeft, 10);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeRight, 10);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root));

  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_defined_cross_with_margin) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignCenter);
  BNDYGNodeStyleSetJustifyContent(root, BNDYGJustifyCenter);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeLeft, 10);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeRight, 10);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root));

  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_defined_cross_with_main_margin) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignCenter);
  BNDYGNodeStyleSetJustifyContent(root, BNDYGJustifyCenter);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 50);
  BNDYGNodeStyleSetAspectRatio(root_child0, 1);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeTop, 10);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeBottom, 10);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root));

  ASSERT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_should_prefer_explicit_height) {
  const BNDYGConfigRef config = BNDYGConfigNew();
  BNDYGConfigSetUseWebDefaults(config, true);

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionColumn);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexDirection(root_child0, BNDYGFlexDirectionColumn);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child0_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexDirection(root_child0_child0, BNDYGFlexDirectionColumn);
  BNDYGNodeStyleSetHeight(root_child0_child0, 100);
  BNDYGNodeStyleSetAspectRatio(root_child0_child0, 2);
  BNDYGNodeInsertChild(root_child0, root_child0_child0, 0);

  BNDYGNodeCalculateLayout(root, 100, 200, BNDYGDirectionLTR);

  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root));
  ASSERT_EQ(200, BNDYGNodeLayoutGetHeight(root));

  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_EQ(200, BNDYGNodeLayoutGetWidth(root_child0_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root_child0_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_should_prefer_explicit_width) {
  const BNDYGConfigRef config = BNDYGConfigNew();
  BNDYGConfigSetUseWebDefaults(config, true);

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexDirection(root_child0, BNDYGFlexDirectionRow);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child0_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexDirection(root_child0_child0, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetWidth(root_child0_child0, 100);
  BNDYGNodeStyleSetAspectRatio(root_child0_child0, 0.5);
  BNDYGNodeInsertChild(root_child0, root_child0_child0, 0);

  BNDYGNodeCalculateLayout(root, 200, 100, BNDYGDirectionLTR);

  ASSERT_EQ(200, BNDYGNodeLayoutGetWidth(root));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root));

  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0_child0));
  ASSERT_EQ(200, BNDYGNodeLayoutGetHeight(root_child0_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, aspect_ratio_should_prefer_flexed_dimension) {
  const BNDYGConfigRef config = BNDYGConfigNew();
  BNDYGConfigSetUseWebDefaults(config, true);

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexDirection(root_child0, BNDYGFlexDirectionColumn);
  BNDYGNodeStyleSetAspectRatio(root_child0, 2);
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child0_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetAspectRatio(root_child0_child0, 4);
  BNDYGNodeStyleSetFlexGrow(root_child0_child0, 1);
  BNDYGNodeInsertChild(root_child0, root_child0_child0, 0);

  BNDYGNodeCalculateLayout(root, 100, 100, BNDYGDirectionLTR);

  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root));
  ASSERT_EQ(100, BNDYGNodeLayoutGetHeight(root));

  ASSERT_EQ(100, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_EQ(200, BNDYGNodeLayoutGetWidth(root_child0_child0));
  ASSERT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0_child0));

  BNDYGNodeFreeRecursive(root);
}
