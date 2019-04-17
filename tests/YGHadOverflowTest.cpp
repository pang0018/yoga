// Copyright 2004-present Facebook. All Rights Reserved.

#include <gtest/gtest.h>
#include <bindyoga/BindYoga.h>

using namespace ::testing;

class YogaTest_HadOverflowTests : public Test {
protected:
  YogaTest_HadOverflowTests() {
    config = BNDYGConfigNew();
    root = BNDYGNodeNewWithConfig(config);
    BNDYGNodeStyleSetWidth(root, 200);
    BNDYGNodeStyleSetHeight(root, 100);
    BNDYGNodeStyleSetFlexDirection(root, BNDYGFlexDirectionColumn);
    BNDYGNodeStyleSetFlexWrap(root, BNDYGWrapNoWrap);
  }

  ~YogaTest_HadOverflowTests() {
    BNDYGNodeFreeRecursive(root);
    BNDYGConfigFree(config);
  }

  BNDYGNodeRef root;
  BNDYGConfigRef config;
};

TEST_F(YogaTest_HadOverflowTests, children_overflow_no_wrap_and_no_flex_children) {
  const BNDYGNodeRef child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(child0, 80);
  BNDYGNodeStyleSetHeight(child0, 40);
  BNDYGNodeStyleSetMargin(child0, BNDYGEdgeTop, 10);
  BNDYGNodeStyleSetMargin(child0, BNDYGEdgeBottom, 15);
  BNDYGNodeInsertChild(root, child0, 0);
  const BNDYGNodeRef child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(child1, 80);
  BNDYGNodeStyleSetHeight(child1, 40);
  BNDYGNodeStyleSetMargin(child1, BNDYGEdgeBottom, 5);
  BNDYGNodeInsertChild(root, child1, 1);

  BNDYGNodeCalculateLayout(root, 200, 100, BNDYGDirectionLTR);

  ASSERT_TRUE(BNDYGNodeLayoutGetHadOverflow(root));
}

TEST_F(YogaTest_HadOverflowTests, spacing_overflow_no_wrap_and_no_flex_children) {
  const BNDYGNodeRef child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(child0, 80);
  BNDYGNodeStyleSetHeight(child0, 40);
  BNDYGNodeStyleSetMargin(child0, BNDYGEdgeTop, 10);
  BNDYGNodeStyleSetMargin(child0, BNDYGEdgeBottom, 10);
  BNDYGNodeInsertChild(root, child0, 0);
  const BNDYGNodeRef child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(child1, 80);
  BNDYGNodeStyleSetHeight(child1, 40);
  BNDYGNodeStyleSetMargin(child1, BNDYGEdgeBottom, 5);
  BNDYGNodeInsertChild(root, child1, 1);

  BNDYGNodeCalculateLayout(root, 200, 100, BNDYGDirectionLTR);

  ASSERT_TRUE(BNDYGNodeLayoutGetHadOverflow(root));
}

TEST_F(YogaTest_HadOverflowTests, no_overflow_no_wrap_and_flex_children) {
  const BNDYGNodeRef child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(child0, 80);
  BNDYGNodeStyleSetHeight(child0, 40);
  BNDYGNodeStyleSetMargin(child0, BNDYGEdgeTop, 10);
  BNDYGNodeStyleSetMargin(child0, BNDYGEdgeBottom, 10);
  BNDYGNodeInsertChild(root, child0, 0);
  const BNDYGNodeRef child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(child1, 80);
  BNDYGNodeStyleSetHeight(child1, 40);
  BNDYGNodeStyleSetMargin(child1, BNDYGEdgeBottom, 5);
  BNDYGNodeStyleSetFlexShrink(child1, 1);
  BNDYGNodeInsertChild(root, child1, 1);

  BNDYGNodeCalculateLayout(root, 200, 100, BNDYGDirectionLTR);

  ASSERT_FALSE(BNDYGNodeLayoutGetHadOverflow(root));
}

TEST_F(YogaTest_HadOverflowTests, hadOverflow_gets_reset_if_not_logger_valid) {
  const BNDYGNodeRef child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(child0, 80);
  BNDYGNodeStyleSetHeight(child0, 40);
  BNDYGNodeStyleSetMargin(child0, BNDYGEdgeTop, 10);
  BNDYGNodeStyleSetMargin(child0, BNDYGEdgeBottom, 10);
  BNDYGNodeInsertChild(root, child0, 0);
  const BNDYGNodeRef child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(child1, 80);
  BNDYGNodeStyleSetHeight(child1, 40);
  BNDYGNodeStyleSetMargin(child1, BNDYGEdgeBottom, 5);
  BNDYGNodeInsertChild(root, child1, 1);

  BNDYGNodeCalculateLayout(root, 200, 100, BNDYGDirectionLTR);

  ASSERT_TRUE(BNDYGNodeLayoutGetHadOverflow(root));

  BNDYGNodeStyleSetFlexShrink(child1, 1);

  BNDYGNodeCalculateLayout(root, 200, 100, BNDYGDirectionLTR);

  ASSERT_FALSE(BNDYGNodeLayoutGetHadOverflow(root));
}

TEST_F(YogaTest_HadOverflowTests, spacing_overflow_in_nested_nodes) {
  const BNDYGNodeRef child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(child0, 80);
  BNDYGNodeStyleSetHeight(child0, 40);
  BNDYGNodeStyleSetMargin(child0, BNDYGEdgeTop, 10);
  BNDYGNodeStyleSetMargin(child0, BNDYGEdgeBottom, 10);
  BNDYGNodeInsertChild(root, child0, 0);
  const BNDYGNodeRef child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(child1, 80);
  BNDYGNodeStyleSetHeight(child1, 40);
  BNDYGNodeInsertChild(root, child1, 1);
  const BNDYGNodeRef child1_1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(child1_1, 80);
  BNDYGNodeStyleSetHeight(child1_1, 40);
  BNDYGNodeStyleSetMargin(child1_1, BNDYGEdgeBottom, 5);
  BNDYGNodeInsertChild(child1, child1_1, 0);

  BNDYGNodeCalculateLayout(root, 200, 100, BNDYGDirectionLTR);

  ASSERT_TRUE(BNDYGNodeLayoutGetHadOverflow(root));
}
