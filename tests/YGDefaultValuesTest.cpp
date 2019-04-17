/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BindYoga.h>

TEST(YogaTest, assert_default_values) {
  const BNDYGNodeRef root = BNDYGNodeNew();

  ASSERT_EQ(0, BNDYGNodeGetChildCount(root));
  ASSERT_EQ(NULL, BNDYGNodeGetChild(root, 1));

  ASSERT_EQ(BNDYGDirectionInherit, BNDYGNodeStyleGetDirection(root));
  ASSERT_EQ(BNDYGFlexDirectionColumn, BNDYGNodeStyleGetFlexDirection(root));
  ASSERT_EQ(BNDYGJustifyFlexStart, BNDYGNodeStyleGetJustifyContent(root));
  ASSERT_EQ(BNDYGAlignFlexStart, BNDYGNodeStyleGetAlignContent(root));
  ASSERT_EQ(BNDYGAlignStretch, BNDYGNodeStyleGetAlignItems(root));
  ASSERT_EQ(BNDYGAlignAuto, BNDYGNodeStyleGetAlignSelf(root));
  ASSERT_EQ(BNDYGPositionTypeRelative, BNDYGNodeStyleGetPositionType(root));
  ASSERT_EQ(BNDYGWrapNoWrap, BNDYGNodeStyleGetFlexWrap(root));
  ASSERT_EQ(BNDYGOverflowVisible, BNDYGNodeStyleGetOverflow(root));
  ASSERT_FLOAT_EQ(0, BNDYGNodeStyleGetFlexGrow(root));
  ASSERT_FLOAT_EQ(0, BNDYGNodeStyleGetFlexShrink(root));
  ASSERT_EQ(BNDYGNodeStyleGetFlexBasis(root).unit, BNDYGUnitAuto);

  ASSERT_EQ(BNDYGNodeStyleGetPosition(root, BNDYGEdgeLeft).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetPosition(root, BNDYGEdgeTop).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetPosition(root, BNDYGEdgeRight).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetPosition(root, BNDYGEdgeBottom).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetPosition(root, BNDYGEdgeStart).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetPosition(root, BNDYGEdgeEnd).unit, BNDYGUnitUndefined);

  ASSERT_EQ(BNDYGNodeStyleGetMargin(root, BNDYGEdgeLeft).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetMargin(root, BNDYGEdgeTop).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetMargin(root, BNDYGEdgeRight).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetMargin(root, BNDYGEdgeBottom).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetMargin(root, BNDYGEdgeStart).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetMargin(root, BNDYGEdgeEnd).unit, BNDYGUnitUndefined);

  ASSERT_EQ(BNDYGNodeStyleGetPadding(root, BNDYGEdgeLeft).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetPadding(root, BNDYGEdgeTop).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetPadding(root, BNDYGEdgeRight).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetPadding(root, BNDYGEdgeBottom).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetPadding(root, BNDYGEdgeStart).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetPadding(root, BNDYGEdgeEnd).unit, BNDYGUnitUndefined);

  ASSERT_TRUE(BNDYGFloatIsUndefined(BNDYGNodeStyleGetBorder(root, BNDYGEdgeLeft)));
  ASSERT_TRUE(BNDYGFloatIsUndefined(BNDYGNodeStyleGetBorder(root, BNDYGEdgeTop)));
  ASSERT_TRUE(BNDYGFloatIsUndefined(BNDYGNodeStyleGetBorder(root, BNDYGEdgeRight)));
  ASSERT_TRUE(BNDYGFloatIsUndefined(BNDYGNodeStyleGetBorder(root, BNDYGEdgeBottom)));
  ASSERT_TRUE(BNDYGFloatIsUndefined(BNDYGNodeStyleGetBorder(root, BNDYGEdgeStart)));
  ASSERT_TRUE(BNDYGFloatIsUndefined(BNDYGNodeStyleGetBorder(root, BNDYGEdgeEnd)));

  ASSERT_EQ(BNDYGNodeStyleGetWidth(root).unit, BNDYGUnitAuto);
  ASSERT_EQ(BNDYGNodeStyleGetHeight(root).unit, BNDYGUnitAuto);
  ASSERT_EQ(BNDYGNodeStyleGetMinWidth(root).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetMinHeight(root).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetMaxWidth(root).unit, BNDYGUnitUndefined);
  ASSERT_EQ(BNDYGNodeStyleGetMaxHeight(root).unit, BNDYGUnitUndefined);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetRight(root));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetBottom(root));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetMargin(root, BNDYGEdgeLeft));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetMargin(root, BNDYGEdgeTop));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetMargin(root, BNDYGEdgeRight));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetMargin(root, BNDYGEdgeBottom));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetPadding(root, BNDYGEdgeLeft));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetPadding(root, BNDYGEdgeTop));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetPadding(root, BNDYGEdgeRight));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetPadding(root, BNDYGEdgeBottom));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetBorder(root, BNDYGEdgeLeft));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetBorder(root, BNDYGEdgeTop));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetBorder(root, BNDYGEdgeRight));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetBorder(root, BNDYGEdgeBottom));

  ASSERT_TRUE(BNDYGFloatIsUndefined(BNDYGNodeLayoutGetWidth(root)));
  ASSERT_TRUE(BNDYGFloatIsUndefined(BNDYGNodeLayoutGetHeight(root)));
  ASSERT_EQ(BNDYGDirectionInherit, BNDYGNodeLayoutGetDirection(root));

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, assert_webdefault_values) {
  BNDYGConfig *config = BNDYGConfigNew();
  BNDYGConfigSetUseWebDefaults(config, true);
  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);

  ASSERT_EQ(BNDYGFlexDirectionRow, BNDYGNodeStyleGetFlexDirection(root));
  ASSERT_EQ(BNDYGAlignStretch, BNDYGNodeStyleGetAlignContent(root));
  ASSERT_FLOAT_EQ(1.0f, BNDYGNodeStyleGetFlexShrink(root));

  BNDYGNodeFreeRecursive(root);
  BNDYGConfigFree(config);
}

TEST(YogaTest, assert_webdefault_values_reset) {
  BNDYGConfig *config = BNDYGConfigNew();
  BNDYGConfigSetUseWebDefaults(config, true);
  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeReset(root);

  ASSERT_EQ(BNDYGFlexDirectionRow, BNDYGNodeStyleGetFlexDirection(root));
  ASSERT_EQ(BNDYGAlignStretch, BNDYGNodeStyleGetAlignContent(root));
  ASSERT_FLOAT_EQ(1.0f, BNDYGNodeStyleGetFlexShrink(root));

  BNDYGNodeFreeRecursive(root);
  BNDYGConfigFree(config);
}

TEST(YogaTest, assert_legacy_stretch_behaviour) {
  BNDYGConfig *config = BNDYGConfigNew();
  BNDYGConfigSetUseLegacyStretchBehaviour(config, true);
  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetWidth(root, 500);
  BNDYGNodeStyleSetHeight(root, 500);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetAlignItems(root_child0, BNDYGAlignFlexStart);
  BNDYGNodeInsertChild(root, root_child0, 0);

  const BNDYGNodeRef root_child0_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexGrow(root_child0_child0, 1);
  BNDYGNodeStyleSetFlexShrink(root_child0_child0, 1);
  BNDYGNodeInsertChild(root_child0, root_child0_child0, 0);

  const BNDYGNodeRef root_child0_child0_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexGrow(root_child0_child0_child0, 1);
  BNDYGNodeStyleSetFlexShrink(root_child0_child0_child0, 1);
  BNDYGNodeInsertChild(root_child0_child0, root_child0_child0_child0, 0);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(500, BNDYGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(500, BNDYGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(500, BNDYGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(500, BNDYGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetWidth(root_child0_child0));
  ASSERT_FLOAT_EQ(500, BNDYGNodeLayoutGetHeight(root_child0_child0));

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetLeft(root_child0_child0_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetTop(root_child0_child0_child0));
  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetWidth(root_child0_child0_child0));
  ASSERT_FLOAT_EQ(500, BNDYGNodeLayoutGetHeight(root_child0_child0_child0));

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}
