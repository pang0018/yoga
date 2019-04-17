/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BNDYGNode.h>
#include <bindyoga/BindYoga.h>

struct _MeasureConstraint {
  float width;
  BNDYGMeasureMode widthMode;
  float height;
  BNDYGMeasureMode heightMode;
};

struct _MeasureConstraintList {
  uint32_t length;
  struct _MeasureConstraint *constraints;
};

static BNDYGSize _measure(BNDYGNodeRef node,
                       float width,
                       BNDYGMeasureMode widthMode,
                       float height,
                       BNDYGMeasureMode heightMode) {
  struct _MeasureConstraintList* constraintList =
      (struct _MeasureConstraintList*)node->getContext();
  struct _MeasureConstraint *constraints = constraintList->constraints;
  uint32_t currentIndex = constraintList->length;
  (&constraints[currentIndex])->width = width;
  (&constraints[currentIndex])->widthMode = widthMode;
  (&constraints[currentIndex])->height = height;
  (&constraints[currentIndex])->heightMode = heightMode;
  constraintList->length = currentIndex + 1;

  return BNDYGSize{
      .width = widthMode == BNDYGMeasureModeUndefined ? 10 : width,
      .height = heightMode == BNDYGMeasureModeUndefined ? 10 : width,
  };
}

TEST(YogaTest, exactly_measure_stretched_child_column) {
  struct _MeasureConstraintList constraintList = _MeasureConstraintList{
      .length = 0,
      .constraints = (struct _MeasureConstraint *) malloc(10 * sizeof(struct _MeasureConstraint)),
  };

  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  //  root_child0->setContext(&constraintList);
  root_child0->setContext(&constraintList);
  root_child0->setMeasureFunc(_measure);
  //  root_child0->setMeasureFunc(_measure);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(1, constraintList.length);

  ASSERT_FLOAT_EQ(100, constraintList.constraints[0].width);
  ASSERT_EQ(BNDYGMeasureModeExactly, constraintList.constraints[0].widthMode);

  free(constraintList.constraints);
  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, exactly_measure_stretched_child_row) {
  struct _MeasureConstraintList constraintList = _MeasureConstraintList{
      .length = 0,
      .constraints = (struct _MeasureConstraint *) malloc(10 * sizeof(struct _MeasureConstraint)),
  };

  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  //  root_child0->setContext(&constraintList);
  root_child0->setContext(&constraintList);
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(1, constraintList.length);

  ASSERT_FLOAT_EQ(100, constraintList.constraints[0].height);
  ASSERT_EQ(BNDYGMeasureModeExactly, constraintList.constraints[0].heightMode);

  free(constraintList.constraints);
  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, at_most_main_axis_column) {
  struct _MeasureConstraintList constraintList = _MeasureConstraintList{
      .length = 0,
      .constraints = (struct _MeasureConstraint *) malloc(10 * sizeof(struct _MeasureConstraint)),
  };

  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  root_child0->setContext(&constraintList);
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(1, constraintList.length);

  ASSERT_FLOAT_EQ(100, constraintList.constraints[0].height);
  ASSERT_EQ(BNDYGMeasureModeAtMost, constraintList.constraints[0].heightMode);

  free(constraintList.constraints);
  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, at_most_cross_axis_column) {
  struct _MeasureConstraintList constraintList = _MeasureConstraintList{
      .length = 0,
      .constraints = (struct _MeasureConstraint *) malloc(10 * sizeof(struct _MeasureConstraint)),
  };

  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  root_child0->setContext(&constraintList);
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(1, constraintList.length);

  ASSERT_FLOAT_EQ(100, constraintList.constraints[0].width);
  ASSERT_EQ(BNDYGMeasureModeAtMost, constraintList.constraints[0].widthMode);

  free(constraintList.constraints);
  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, at_most_main_axis_row) {
  struct _MeasureConstraintList constraintList = _MeasureConstraintList{
      .length = 0,
      .constraints = (struct _MeasureConstraint *) malloc(10 * sizeof(struct _MeasureConstraint)),
  };

  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  root_child0->setContext(&constraintList);
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(1, constraintList.length);

  ASSERT_FLOAT_EQ(100, constraintList.constraints[0].width);
  ASSERT_EQ(BNDYGMeasureModeAtMost, constraintList.constraints[0].widthMode);

  free(constraintList.constraints);
  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, at_most_cross_axis_row) {
  struct _MeasureConstraintList constraintList = _MeasureConstraintList{
      .length = 0,
      .constraints = (struct _MeasureConstraint *) malloc(10 * sizeof(struct _MeasureConstraint)),
  };

  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  root_child0->setContext(&constraintList);
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(1, constraintList.length);

  ASSERT_FLOAT_EQ(100, constraintList.constraints[0].height);
  ASSERT_EQ(BNDYGMeasureModeAtMost, constraintList.constraints[0].heightMode);

  free(constraintList.constraints);
  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, flex_child) {
  struct _MeasureConstraintList constraintList = _MeasureConstraintList{
      .length = 0,
      .constraints = (struct _MeasureConstraint *) malloc(10 * sizeof(struct _MeasureConstraint)),
  };

  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  root_child0->setContext(&constraintList);
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(2, constraintList.length);

  ASSERT_FLOAT_EQ(100, constraintList.constraints[0].height);
  ASSERT_EQ(BNDYGMeasureModeAtMost, constraintList.constraints[0].heightMode);

  ASSERT_FLOAT_EQ(100, constraintList.constraints[1].height);
  ASSERT_EQ(BNDYGMeasureModeExactly, constraintList.constraints[1].heightMode);

  free(constraintList.constraints);
  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, flex_child_with_flex_basis) {
  struct _MeasureConstraintList constraintList = _MeasureConstraintList{
      .length = 0,
      .constraints = (struct _MeasureConstraint *) malloc(10 * sizeof(struct _MeasureConstraint)),
  };

  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexGrow(root_child0, 1);
  BNDYGNodeStyleSetFlexBasis(root_child0, 0);
  root_child0->setContext(&constraintList);
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(1, constraintList.length);

  ASSERT_FLOAT_EQ(100, constraintList.constraints[0].height);
  ASSERT_EQ(BNDYGMeasureModeExactly, constraintList.constraints[0].heightMode);

  free(constraintList.constraints);
  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, overflow_scroll_column) {
  struct _MeasureConstraintList constraintList = _MeasureConstraintList{
      .length = 0,
      .constraints = (struct _MeasureConstraint *) malloc(10 * sizeof(struct _MeasureConstraint)),
  };

  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetOverflow(root, BNDYGOverflowScroll);
  BNDYGNodeStyleSetHeight(root, 100);
  BNDYGNodeStyleSetWidth(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  root_child0->setContext(&constraintList);
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(1, constraintList.length);

  ASSERT_FLOAT_EQ(100, constraintList.constraints[0].width);
  ASSERT_EQ(BNDYGMeasureModeAtMost, constraintList.constraints[0].widthMode);

  ASSERT_TRUE(BNDYGFloatIsUndefined(constraintList.constraints[0].height));
  ASSERT_EQ(BNDYGMeasureModeUndefined, constraintList.constraints[0].heightMode);

  free(constraintList.constraints);
  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, overflow_scroll_row) {
  struct _MeasureConstraintList constraintList = _MeasureConstraintList{
      .length = 0,
      .constraints = (struct _MeasureConstraint *) malloc(10 * sizeof(struct _MeasureConstraint)),
  };

  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignFlexStart);
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetOverflow(root, BNDYGOverflowScroll);
  BNDYGNodeStyleSetHeight(root, 100);
  BNDYGNodeStyleSetWidth(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  root_child0->setContext(&constraintList);
  root_child0->setMeasureFunc(_measure);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_EQ(1, constraintList.length);

  ASSERT_TRUE(BNDYGFloatIsUndefined(constraintList.constraints[0].width));
  ASSERT_EQ(BNDYGMeasureModeUndefined, constraintList.constraints[0].widthMode);

  ASSERT_FLOAT_EQ(100, constraintList.constraints[0].height);
  ASSERT_EQ(BNDYGMeasureModeAtMost, constraintList.constraints[0].heightMode);

  free(constraintList.constraints);
  BNDYGNodeFreeRecursive(root);
}
