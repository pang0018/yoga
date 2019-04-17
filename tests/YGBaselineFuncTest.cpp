/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BNDYGNode.h>
#include <bindyoga/BindYoga.h>

static float _baseline(BNDYGNodeRef node, const float width, const float height) {
  float* baseline = (float*)node->getContext();
  return *baseline;
}

TEST(YogaTest, align_baseline_customer_func) {
  const BNDYGNodeRef root = BNDYGNodeNew();
  BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionRow);
  BNDYGNodeStyleSetAlignItems(root, BNDYGAlignBaseline);
  BNDYGNodeStyleSetWidth(root, 100);
  BNDYGNodeStyleSetHeight(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child0, 50);
  BNDYGNodeStyleSetHeight(root_child0, 50);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child1 = BNDYGNodeNew();
  BNDYGNodeStyleSetWidth(root_child1, 50);
  BNDYGNodeStyleSetHeight(root_child1, 20);
  BNDYGNodeInsertChild(root, root_child1, 1);

  float baselineValue = 10;
  const BNDYGNodeRef root_child1_child0 = BNDYGNodeNew();
  root_child1_child0->setContext(&baselineValue);
  BNDYGNodeStyleSetWidth(root_child1_child0, 50);
  root_child1_child0->setBaseLineFunc(_baseline);
  BNDYGNodeStyleSetHeight(root_child1_child0, 20);
  BNDYGNodeInsertChild(root_child1, root_child1_child0, 0);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, BNDYGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, BNDYGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(40, BNDYGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetHeight(root_child1));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child1_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child1_child0));
  ASSERT_FLOAT_EQ(50, BNDYGNodeLayoutGetWidth(root_child1_child0));
  ASSERT_FLOAT_EQ(20, BNDYGNodeLayoutGetHeight(root_child1_child0));
}
