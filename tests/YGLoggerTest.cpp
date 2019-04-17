/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>
#include <stdarg.h>
#include <bindyoga/BindYoga.h>

namespace {
char writeBuffer[4096];
int _unmanagedLogger(const BNDYGConfigRef config,
                     const BNDYGNodeRef node,
                     BNDYGLogLevel level,
                     const char *format,
                     va_list args) {
  return vsnprintf(writeBuffer + strlen(writeBuffer),
                   sizeof(writeBuffer) - strlen(writeBuffer),
                   format,
                   args);
}
}

TEST(YogaTest, logger_default_node_should_print_no_style_info) {
  writeBuffer[0] = '\0';
  const BNDYGConfigRef config = BNDYGConfigNew();
  BNDYGConfigSetLogger(config, _unmanagedLogger);
  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeCalculateLayout(root, BNDYGUnitUndefined, BNDYGUnitUndefined, BNDYGDirectionLTR);
  BNDYGNodePrint(root,
              (BNDYGPrintOptions)(BNDYGPrintOptionsLayout | BNDYGPrintOptionsChildren |
                               BNDYGPrintOptionsStyle));
  BNDYGConfigSetLogger(config, NULL);
  BNDYGNodeFree(root);

  const char *expected = "<div layout=\"width: 0; height: 0; top: 0; left: 0;\" style=\"\" ></div>";
  ASSERT_STREQ(expected, writeBuffer);
}

TEST(YogaTest, logger_node_with_percentage_absolute_position_and_margin) {
  writeBuffer[0] = '\0';
  const BNDYGConfigRef config = BNDYGConfigNew();
  BNDYGConfigSetLogger(config, _unmanagedLogger);
  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  BNDYGNodeStyleSetPositionType(root, BNDYGPositionTypeAbsolute);
  BNDYGNodeStyleSetWidthPercent(root, 50);
  BNDYGNodeStyleSetHeightPercent(root, 75);
  BNDYGNodeStyleSetFlex(root, 1);
  BNDYGNodeStyleSetMargin(root, BNDYGEdgeRight, 10);
  BNDYGNodeStyleSetMarginAuto(root, BNDYGEdgeLeft);
  BNDYGNodeCalculateLayout(root, BNDYGUnitUndefined, BNDYGUnitUndefined, BNDYGDirectionLTR);
  BNDYGNodePrint(root,
              (BNDYGPrintOptions)(BNDYGPrintOptionsLayout | BNDYGPrintOptionsChildren |
                               BNDYGPrintOptionsStyle));
  BNDYGConfigSetLogger(config, NULL);
  BNDYGNodeFree(root);

  const char *expected = "<div layout=\"width: 0; height: 0; top: 0; left: 0;\" style=\"flex: 1; "
                         "margin-left: auto; margin-right: 10px; width: 50%; height: 75%; "
                         "position: absolute; \" ></div>";
  ASSERT_STREQ(expected, writeBuffer);
}

TEST(YogaTest, logger_node_with_children_should_print_indented) {
  writeBuffer[0] = '\0';
  const BNDYGConfigRef config = BNDYGConfigNew();
  BNDYGConfigSetLogger(config, _unmanagedLogger);
  const BNDYGNodeRef root = BNDYGNodeNewWithConfig(config);
  const BNDYGNodeRef child0 = BNDYGNodeNewWithConfig(config);
  const BNDYGNodeRef child1 = BNDYGNodeNewWithConfig(config);
  BNDYGNodeInsertChild(root, child0, 0);
  BNDYGNodeInsertChild(root, child1, 1);
  BNDYGNodeCalculateLayout(root, BNDYGUnitUndefined, BNDYGUnitUndefined, BNDYGDirectionLTR);
  BNDYGNodePrint(root,
              (BNDYGPrintOptions)(BNDYGPrintOptionsLayout | BNDYGPrintOptionsChildren |
                               BNDYGPrintOptionsStyle));
  BNDYGConfigSetLogger(config, NULL);
  BNDYGNodeFreeRecursive(root);

  const char *expected = "<div layout=\"width: 0; height: 0; top: 0; left: 0;\" style=\"\" >\n  "
                         "<div layout=\"width: 0; height: 0; top: 0; left: 0;\" style=\"\" "
                         "></div>\n  <div layout=\"width: 0; height: 0; top: 0; left: 0;\" "
                         "style=\"\" ></div>\n</div>";
  ASSERT_STREQ(expected, writeBuffer);
}
