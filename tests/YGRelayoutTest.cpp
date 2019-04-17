/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <bindyoga/BindYoga.h>

TEST(YogaTest, dont_cache_computed_flex_basis_between_layouts) {
  const BNDYGConfigRef config = BNDYGConfigNew();
  BNDYGConfigSetExperimentalFeatureEnabled(config, BNDYGExperimentalFeatureWebFlexBasis, true);

  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetHeightPercent(root, 100);
  BNDYGNodeStyleSetWidthPercent(root, 100);

  const BNDYGNodeRef root_child0 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetFlexBasisPercent(root_child0, 100);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, 100, BNDYGUndefined, BNDYGDirectionLTR);
  BNDYGNodeCalculateLayout(root, 100, 100, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(100, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);

  BNDYGConfigFree(config);
}

TEST(YogaTest, recalculate_resolvedDimonsion_onchange) {
  const BNDYGNodeRef root = BNDYGNodeNew();

  const BNDYGNodeRef root_child0 = BNDYGNodeNew();
  BNDYGNodeStyleSetMinHeight(root_child0, 10);
  BNDYGNodeStyleSetMaxHeight(root_child0, 10);
  BNDYGNodeInsertChild(root, root_child0, 0);

  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);
  ASSERT_FLOAT_EQ(10, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeStyleSetMinHeight(root_child0, BNDYGUndefined);
  BNDYGNodeCalculateLayout(root, BNDYGUndefined, BNDYGUndefined, BNDYGDirectionLTR);

  ASSERT_FLOAT_EQ(0, BNDYGNodeLayoutGetHeight(root_child0));

  BNDYGNodeFreeRecursive(root);
}
