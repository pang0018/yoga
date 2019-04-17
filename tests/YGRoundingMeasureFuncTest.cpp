/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BNDYGNode.h>
#include <bindyoga/BindYoga.h>

static BNDYGSize _measureFloor(BNDYGNodeRef node,
                            float width,
                            BNDYGMeasureMode widthMode,
                            float height,
                            BNDYGMeasureMode heightMode) {
  return BNDYGSize{
      width = 10.2f, height = 10.2f,
  };
}

static BNDYGSize _measureCeil(BNDYGNodeRef node,
                           float width,
                           BNDYGMeasureMode widthMode,
                           float height,
                           BNDYGMeasureMode heightMode) {
  return BNDYGSize{
      width = 10.5f, height = 10.5f,
  };
}

static BNDYGSize _measureFractial(BNDYGNodeRef node,
  float width,
  BNDYGMeasureMode widthMode,
  float height,
  BNDYGMeasureMode heightMode) {
  return BNDYGSize{
    width = 0.5f, height = 0.5f,
  };
}

TEST(YogaTest, rounding_feature_with_custom_measure_func_floor) {
  const BNDYGConfigRef config = BNDYGConfigNew();
  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  root_child0->setMeasureFunc(_measureFloor);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGConfigSetPointScaleFactor(config, 0.0f);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionRTL);

  ASSERT_FLOAT_EQ(10.2, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(10.2, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGConfigSetPointScaleFactor(config, 1.0f);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(11, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(11, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGConfigSetPointScaleFactor(config, 2.0f);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionRTL);

  ASSERT_FLOAT_EQ(10.5, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(10.5, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGConfigSetPointScaleFactor(config, 4.0f);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(10.25, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(10.25, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGConfigSetPointScaleFactor(config, 1.0f / 3.0f);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionRTL);

  ASSERT_FLOAT_EQ(12.0, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(12.0, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}

TEST(YogaTest, rounding_feature_with_custom_measure_func_ceil) {
  const BNDYGConfigRef config = BNDYGConfigNew();
  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  root_child0->setMeasureFunc(_measureCeil);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGConfigSetPointScaleFactor(config, 1.0f);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(11, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(11, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}

TEST(YogaTest, rounding_feature_with_custom_measure_and_fractial_matching_scale) {
  const BNDYGConfigRef config = BNDYGConfigNew();
  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetPosition(root_child0, BNDYGEdgeLeft, 73.625);
  root_child0->setMeasureFunc(_measureFractial);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGConfigSetPointScaleFactor(config, 2.0f);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0.5, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(0.5, BNDYGNodeLayoutGetHeight(root_child0));
  ASSERT_FLOAT_EQ(73.5, BNDYGNodeLayoutGetLeft(root_child0));

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}
