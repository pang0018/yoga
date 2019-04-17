/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BNDYGNode.h>
#include <bindyoga/BindYoga.h>

static BNDYGSize _measureMax(BNDYGNodeRef node,
                          float width,
                          BNDYGMeasureMode widthMode,
                          float height,
                          BNDYGMeasureMode heightMode) {
  int* measureCount = (int*)node->getContext();
  (*measureCount)++;

  return BNDYGSize{
      .width = widthMode == BNDYGMeasureModeUndefined ? 10 : width,
      .height = heightMode == BNDYGMeasureModeUndefined ? 10 : height,
  };
}

static BNDYGSize _measureMin(BNDYGNodeRef node,
                          float width,
                          BNDYGMeasureMode widthMode,
                          float height,
                          BNDYGMeasureMode heightMode) {
  int* measureCount = (int*)node->getContext();
  *measureCount = *measureCount + 1;
  return BNDYGSize{
      .width =
          widthMode == BNDYGMeasureModeUndefined || (widthMode == BNDYGMeasureModeAtMost && width > 10)
              ? 10
              : width,
      .height =
          heightMode == BNDYGMeasureModeUndefined || (heightMode == BNDYGMeasureModeAtMost && height > 10)
              ? 10
              : height,
  };
}

static BNDYGSize _measure_84_49(BNDYGNodeRef node,
                             float width,
                             BNDYGMeasureMode widthMode,
                             float height,
                             BNDYGMeasureMode heightMode) {
  int* measureCount = (int*)node->getContext();
  if (measureCount) {
    (*measureCount)++;
  }

  return BNDYGSize{
      .width = 84.f, .height = 49.f,
  };
}

TEST(YogaTest, measure_once_single_flexible_child) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  int measureCount = 0;
  root_child0->setContext(&measureCount);
  root_child0->setMeasureFunc(_measureMax);
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(1, measureCount);

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, remeasure_with_same_exact_width_larger_than_needed_height) {
  const BNDYGNodeRef root = BNDYGNodeNew();

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  int measureCount = 0;
  root_child0->setContext(&measureCount);
  root_child0->setMeasureFunc(_measureMin);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, 100, 100, BNDYGDirectionLTR);
  BNDYGNodeCalculateLayout(root, 100, 50, BNDYGDirectionLTR);

  ASSERT_EQ(1, measureCount);

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, remeasure_with_same_atmost_width_larger_than_needed_height) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  int measureCount = 0;
  root_child0->setContext(&measureCount);
  root_child0->setMeasureFunc(_measureMin);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, 100, 100, BNDYGDirectionLTR);
  BNDYGNodeCalculateLayout(root, 100, 50, BNDYGDirectionLTR);

  ASSERT_EQ(1, measureCount);

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, remeasure_with_computed_width_larger_than_needed_height) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  int measureCount = 0;
  root_child0->setContext(&measureCount);
  root_child0->setMeasureFunc(_measureMin);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, 100, 100, BNDYGDirectionLTR);
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignStretch);
  BNDYGNodeCalculateLayout(root, 10, 50, BNDYGDirectionLTR);

  ASSERT_EQ(1, measureCount);

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, remeasure_with_atmost_computed_width_undefined_height) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  int measureCount = 0;
  root_child0->setContext(&measureCount);
  root_child0->setMeasureFunc(_measureMin);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, 100, BNDYGUndefined, BNDYGDirectionLTR);
  BNDYGNodeCalculateLayout(root, 10, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(1, measureCount);

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, remeasure_with_already_measured_value_smaller_but_still_float_equal) {
  int measureCount = 0;

  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 288.f);
  BNDYGNodeStyleSetHeight(root, 288.f);
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetPadding(root_child0, BNDYGEdgeAll, 2.88f);
  BNDYGNodeStyleSetFlexDirection(root_child0, BNDYGFlexDirectionRow);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child0_child0 = BNDYGNodeNew();
  root_child0_child0->setContext(&measureCount);
  root_child0_child0->setMeasureFunc(_measure_84_49);
  BNDYGNodeInsertChild(root_child0, root_child0_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  BNDYGNodeFreeRecursive(root);

  ASSERT_EQ(1, measureCount);
}
