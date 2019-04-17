/*
 *  Copyright (c) 2014-present, Facebook, Inc.
 *
 *  This source code is licensed under the MIT license found in the LICENSE
 *  file in the root directory of this source tree.
 *
 */
#include <gtest/gtest.h>
#include <bindyoga/BNDYGNode.h>
#include <bindyoga/BindYoga.h>

static BNDYGSize _measure(BNDYGNodeRef node,
                       float width,
                       BNDYGMeasureMode widthMode,
                       float height,
                       BNDYGMeasureMode heightMode) {
  int* measureCount = (int*)node->getContext();
  if (measureCount) {
    (*measureCount)++;
  }

  return BNDYGSize{
      .width = 10, .height = 10,
  };
}

static BNDYGSize _simulate_wrapping_text(BNDYGNodeRef node,
                                      float width,
                                      BNDYGMeasureMode widthMode,
                                      float height,
                                      BNDYGMeasureMode heightMode) {
  if (widthMode == BNDYGMeasureModeUndefined || width >= 68) {
    return BNDYGSize{.width = 68, .height = 16};
  }

  return BNDYGSize{
      .width = 50, .height = 32,
  };
}

static BNDYGSize _measure_assert_negative(BNDYGNodeRef node,
                                       float width,
                                       BNDYGMeasureMode widthMode,
                                       float height,
                                       BNDYGMeasureMode heightMode) {
  EXPECT_GE(width, 0);
  EXPECT_GE(height, 0);

  return BNDYGSize{
    .width = 0, .height = 0,
  };
}

TEST(YogaTest, dont_measure_single_grow_shrink_child) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  int measureCount = 0;

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  root_child0->setContext(&measureCount);
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetFlexShrink(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, measureCount);

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, measure_absolute_child_with_no_constraints) {
  const BNDYGNodeRef root = BNDYGNodeNew();

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeInsertChild(root, root_child0, 0);

  int measureCount = 0;

  const BNDYGNodeRef root_child0_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetPositionType(root_child0_child0, BNDYGPositionTypeAbsolute);
  root_child0_child0->setContext(&measureCount);
  root_child0_child0->setMeasureFunc(_measure);
  BNDYGNodeInsertChild(root_child0, root_child0_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(1, measureCount);

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, dont_measure_when_min_equals_max) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  int measureCount = 0;

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  root_child0->setContext(&measureCount);
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeStyleSetMinWidth(root_child0, 10);
  BNDYGNodeStyleSetMaxWidth(root_child0, 10);
  BNDYGNodeStyleSetMinHeight(root_child0, 10);
  BNDYGNodeStyleSetMaxHeight(root_child0, 10);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, measureCount);
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, dont_measure_when_min_equals_max_percentages) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  int measureCount = 0;

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  root_child0->setContext(&measureCount);
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeStyleSetMinWidthPercent(root_child0, 10);
  BNDYGNodeStyleSetMaxWidthPercent(root_child0, 10);
  BNDYGNodeStyleSetMinHeightPercent(root_child0, 10);
  BNDYGNodeStyleSetMaxHeightPercent(root_child0, 10);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, measureCount);
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}


TEST(YogaTest, measure_nodes_with_margin_auto_and_stretch) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 500);
  BNDYGNodeStyleSetHeight(root, 500);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeStyleSetMarginAuto(root_child0, BNDYGEdgeLeft);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  EXPECT_EQ(490, BNDYGNodeLayoutGetLeft(root_child0));
  EXPECT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  EXPECT_EQ(10, BNDYGNodeLayoutGetWidth(root_child0));
  EXPECT_EQ(10, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, dont_measure_when_min_equals_max_mixed_width_percent) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  int measureCount = 0;

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  root_child0->setContext(&measureCount);
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeStyleSetMinWidthPercent(root_child0, 10);
  BNDYGNodeStyleSetMaxWidthPercent(root_child0, 10);
  BNDYGNodeStyleSetMinHeight(root_child0, 10);
  BNDYGNodeStyleSetMaxHeight(root_child0, 10);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, measureCount);
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, dont_measure_when_min_equals_max_mixed_height_percent) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  int measureCount = 0;

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  root_child0->setContext(&measureCount);
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeStyleSetMinWidth(root_child0, 10);
  BNDYGNodeStyleSetMaxWidth(root_child0, 10);
  BNDYGNodeStyleSetMinHeightPercent(root_child0, 10);
  BNDYGNodeStyleSetMaxHeightPercent(root_child0, 10);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(0, measureCount);
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, measure_enough_size_should_be_in_single_line) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignSelf(root_child0, BNDYGAlignFlexStart);
  root_child0->setMeasureFunc(_simulate_wrapping_text);

  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(68, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(16, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, measure_not_enough_size_should_wrap) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 55);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignSelf(root_child0, BNDYGAlignFlexStart);
  //  BNDYGNodeSetMeasureFunc(root_child0, _simulate_wrapping_text);
  root_child0->setMeasureFunc(_simulate_wrapping_text);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(32, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, measure_zero_space_should_grow) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root, 200);
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionColumn);
  BNDYGNodeStyleSetFlexGrow(root, 0);

  int measureCount = 0;

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root_child0, BNDYGFlexDirectionColumn);
  BNDYGNodeStyleSetPadding(root_child0, BNDYGEdgeAll, 100);
  root_child0->setContext(&measureCount);
  root_child0->setMeasureFunc(_measure);

  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, 282, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(282, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, measure_flex_direction_row_and_padding) {
  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetPadding(root, BNDYGEdgeLeft, 25);
  BNDYGNodeStyleSetPadding(root, BNDYGEdgeTop, 25);
  BNDYGNodeStyleSetPadding(root, BNDYGEdgeRight, 25);
  BNDYGNodeStyleSetPadding(root, BNDYGEdgeBottom, 25);
  BNDYGNodeStyleSetWidth(root, 50);
  BNDYGNodeStyleSetHeight(root, 50);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  root_child0->setMeasureFunc(_simulate_wrapping_text);
  //  BNDYGNodeSetMeasureFunc(root_child0, _simulate_wrapping_text);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(root_child1, 5);
  BNDYGNodeStyleSetHeight(root_child1, 5);
  BNDYGNodeInsertChild(root, root_child1, 1);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(75, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(5, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(5, BNDYGNodeLayoutGetHeight(root_child1));

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}

TEST(YogaTest, measure_flex_direction_column_and_padding) {
  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetMargin(root, BNDYGEdgeTop, 20);
  BNDYGNodeStyleSetPadding(root, BNDYGEdgeAll, 25);
  BNDYGNodeStyleSetWidth(root, 50);
  BNDYGNodeStyleSetHeight(root, 50);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  root_child0->setMeasureFunc(_simulate_wrapping_text);
  //  BNDYGNodeSetMeasureFunc(root_child0, _simulate_wrapping_text);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(root_child1, 5);
  BNDYGNodeStyleSetHeight(root_child1, 5);
  BNDYGNodeInsertChild(root, root_child1, 1);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(32, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(57, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(5, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(5, BNDYGNodeLayoutGetHeight(root_child1));

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}

TEST(YogaTest, measure_flex_direction_row_no_padding) {
  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetMargin(root, BNDYGEdgeTop, 20);
  BNDYGNodeStyleSetWidth(root, 50);
  BNDYGNodeStyleSetHeight(root, 50);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  //  BNDYGNodeSetMeasureFunc(root_child0, _simulate_wrapping_text);
  root_child0->setMeasureFunc(_simulate_wrapping_text);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(root_child1, 5);
  BNDYGNodeStyleSetHeight(root_child1, 5);
  BNDYGNodeInsertChild(root, root_child1, 1);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(5, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(5, BNDYGNodeLayoutGetHeight(root_child1));

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}

TEST(YogaTest, measure_flex_direction_row_no_padding_align_items_flexstart) {
  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetMargin(root, BNDYGEdgeTop, 20);
  BNDYGNodeStyleSetWidth(root, 50);
  BNDYGNodeStyleSetHeight(root, 50);
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  root_child0->setMeasureFunc(_simulate_wrapping_text);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(root_child1, 5);
  BNDYGNodeStyleSetHeight(root_child1, 5);
  BNDYGNodeInsertChild(root, root_child1, 1);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(32, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(5, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(5, BNDYGNodeLayoutGetHeight(root_child1));

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}

TEST(YogaTest, measure_with_fixed_size) {
  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetMargin(root, BNDYGEdgeTop, 20);
  BNDYGNodeStyleSetPadding(root, BNDYGEdgeAll, 25);
  BNDYGNodeStyleSetWidth(root, 50);
  BNDYGNodeStyleSetHeight(root, 50);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  root_child0->setMeasureFunc(_simulate_wrapping_text);
  BNDYGNodeStyleSetWidth(root_child0, 10);
  BNDYGNodeStyleSetHeight(root_child0, 10);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(root_child1, 5);
  BNDYGNodeStyleSetHeight(root_child1, 5);
  BNDYGNodeInsertChild(root, root_child1, 1);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(35, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(5, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(5, BNDYGNodeLayoutGetHeight(root_child1));

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}

TEST(YogaTest, measure_with_flex_shrink) {
  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetMargin(root, BNDYGEdgeTop, 20);
  BNDYGNodeStyleSetPadding(root, BNDYGEdgeAll, 25);
  BNDYGNodeStyleSetWidth(root, 50);
  BNDYGNodeStyleSetHeight(root, 50);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  root_child0->setMeasureFunc(_simulate_wrapping_text);
  BNDYGNodeStyleSetFlexShrink(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(root_child1, 5);
  BNDYGNodeStyleSetHeight(root_child1, 5);
  BNDYGNodeInsertChild(root, root_child1, 1);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(25, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(5, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(5, BNDYGNodeLayoutGetHeight(root_child1));

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}

TEST(YogaTest, measure_no_padding) {
  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetMargin(root, BNDYGEdgeTop, 20);
  BNDYGNodeStyleSetWidth(root, 50);
  BNDYGNodeStyleSetHeight(root, 50);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  root_child0->setMeasureFunc(_simulate_wrapping_text);
  BNDYGNodeStyleSetFlexShrink(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(root_child1, 5);
  BNDYGNodeStyleSetHeight(root_child1, 5);
  BNDYGNodeInsertChild(root, root_child1, 1);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(32, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(32, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(5, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(5, BNDYGNodeLayoutGetHeight(root_child1));

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}

#if GTEST_HAS_DEATH_TEST
TEST(YogaDeathTest, cannot_add_child_to_node_with_measure_func) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  root->setMeasureFunc(_measure);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  ASSERT_DEATH(BNDYGNodeInsertChild(root, root_child0, 0), "Cannot add child.*");
  BNDYGNodeFree(root_child0);
  BNDYGNodeFreeRecursive(root);
}

TEST(YogaDeathTest, cannot_add_nonnull_measure_func_to_non_leaf_node) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeInsertChild(root, root_child0, 0);
  ASSERT_DEATH(root->setMeasureFunc(_measure), "Cannot set measure function.*");
  BNDYGNodeFreeRecursive(root);
}

#endif

TEST(YogaTest, can_nullify_measure_func_on_any_node) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeInsertChild(root, BNDYGNodeNew(), 0);
  root->setMeasureFunc(nullptr);
  ASSERT_TRUE(root->getMeasure() == NULL);
  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, cant_call_negative_measure) {
  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionColumn);
  BNDYGNodeStyleSetWidth(root, 50);
  BNDYGNodeStyleSetHeight(root, 10);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  root_child0->setMeasureFunc(_measure_assert_negative);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeTop, 20);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  BNDYGNodeFreeRecursive(root);
  BNDYGConfigFree(config);
}

TEST(YogaTest, cant_call_negative_measure_horizontal) {
  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetWidth(root, 10);
  BNDYGNodeStyleSetHeight(root, 20);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  root_child0->setMeasureFunc(_measure_assert_negative);
  BNDYGNodeStyleSetMargin(root_child0, BNDYGEdgeStart, 20);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  BNDYGNodeFreeRecursive(root);
  BNDYGConfigFree(config);
}

static BNDYGSize _measure_90_10(BNDYGNodeRef node,
  float width,
  BNDYGMeasureMode widthMode,
  float height,
  BNDYGMeasureMode heightMode) {

  return BNDYGSize{
    .width = 90, .height = 10,
  };
}

TEST(YogaTest, percent_with_text_node) {
  const BNDYGConfigRef config = BNDYGConfigNew();

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetJustifyContent(root, BNDYGJustifySpaceBetween);
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignCenter);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 80);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNewWithConfig(config);
  root_child1->setMeasureFunc(_measure_90_10);
  BNDYGNodeStyleSetMaxWidthPercent(root_child1, 50);
  BNDYGNodeStyleSetPaddingPercent(root_child1, BNDYGEdgeTop, 50);
  BNDYGNodeInsertChild(root, root_child1, 1);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, BNDYGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(80, BNDYGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(40, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(15, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetHeight(root_child1));

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}
