/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>

#include <bindyoga/BNDYGNode.h>
#include <bindyoga/BindYoga.h>
#include <bindyoga/BindYoga-internal.h>

TEST(YogaTest, rounding_value) {
  // Test that whole numbers are rounded to whole despite ceil/floor flags
  ASSERT_FLOAT_EQ(6.0, BNDYGRoundValueToPixelGrid(6.000001, 2.0, false, false));
  ASSERT_FLOAT_EQ(6.0, BNDYGRoundValueToPixelGrid(6.000001, 2.0, true, false));
  ASSERT_FLOAT_EQ(6.0, BNDYGRoundValueToPixelGrid(6.000001, 2.0, false, true));
  ASSERT_FLOAT_EQ(6.0, BNDYGRoundValueToPixelGrid(5.999999, 2.0, false, false));
  ASSERT_FLOAT_EQ(6.0, BNDYGRoundValueToPixelGrid(5.999999, 2.0, true, false));
  ASSERT_FLOAT_EQ(6.0, BNDYGRoundValueToPixelGrid(5.999999, 2.0, false, true));

  // Test that numbers with fraction are rounded correctly accounting for ceil/floor flags
  ASSERT_FLOAT_EQ(6.0, BNDYGRoundValueToPixelGrid(6.01, 2.0, false, false));
  ASSERT_FLOAT_EQ(6.5, BNDYGRoundValueToPixelGrid(6.01, 2.0, true, false));
  ASSERT_FLOAT_EQ(6.0, BNDYGRoundValueToPixelGrid(6.01, 2.0, false, true));
  ASSERT_FLOAT_EQ(6.0, BNDYGRoundValueToPixelGrid(5.99, 2.0, false, false));
  ASSERT_FLOAT_EQ(6.0, BNDYGRoundValueToPixelGrid(5.99, 2.0, true, false));
  ASSERT_FLOAT_EQ(5.5, BNDYGRoundValueToPixelGrid(5.99, 2.0, false, true));
}
