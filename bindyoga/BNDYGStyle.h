/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once
#include "BNDYGFloatOptional.h"
#include "BindYoga-internal.h"
#include "BindYoga.h"

struct BNDYGStyle {
  BNDYGDirection direction;
  BNDYGFlexDirection flexDirection;
  BNDYGJustify justifyContent;
  BNDYGAlign alignContent;
  BNDYGAlign alignItems;
  BNDYGAlign alignSelf;
  BNDYGPositionType positionType;
  BNDYGWrap flexWrap;
  BNDYGOverflow overflow;
  BNDYGDisplay display;
  BNDYGFloatOptional flex;
  BNDYGFloatOptional flexGrow;
  BNDYGFloatOptional flexShrink;
  BNDYGValue flexBasis;
  std::array<BNDYGValue, BNDYGEdgeCount> margin;
  std::array<BNDYGValue, BNDYGEdgeCount> position;
  std::array<BNDYGValue, BNDYGEdgeCount> padding;
  std::array<BNDYGValue, BNDYGEdgeCount> border;
  std::array<BNDYGValue, 2> dimensions;
  std::array<BNDYGValue, 2> minDimensions;
  std::array<BNDYGValue, 2> maxDimensions;
  BNDYGFloatOptional aspectRatio;

  BNDYGStyle();
  // BindYoga specific properties, not compatible with flexbox specification
  bool operator==(const BNDYGStyle& style);

  bool operator!=(BNDYGStyle style);
  ~BNDYGStyle();
};
