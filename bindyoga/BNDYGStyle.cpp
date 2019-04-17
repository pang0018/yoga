/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "BNDYGStyle.h"

const BNDYGValue kBNDYGValueUndefined = {0, BNDYGUnitUndefined};

const BNDYGValue kBNDYGValueAuto = {0, BNDYGUnitAuto};

const std::array<BNDYGValue, BNDYGEdgeCount> kBNDYGDefaultEdgeValuesUnit = {
    {kBNDYGValueUndefined,
     kBNDYGValueUndefined,
     kBNDYGValueUndefined,
     kBNDYGValueUndefined,
     kBNDYGValueUndefined,
     kBNDYGValueUndefined,
     kBNDYGValueUndefined,
     kBNDYGValueUndefined,
     kBNDYGValueUndefined}};

const std::array<BNDYGValue, 2> kBNDYGDefaultDimensionValuesAutoUnit = {
    {kBNDYGValueAuto, kBNDYGValueAuto}};

const std::array<BNDYGValue, 2> kBNDYGDefaultDimensionValuesUnit = {
    {kBNDYGValueUndefined, kBNDYGValueUndefined}};

BNDYGStyle::BNDYGStyle()
    : direction(BNDYGDirectionInherit),
      flexDirection(BNDYGFlexDirectionColumn),
      justifyContent(BNDYGJustifyFlexStart),
      alignContent(BNDYGAlignFlexStart),
      alignItems(BNDYGAlignStretch),
      alignSelf(BNDYGAlignAuto),
      positionType(BNDYGPositionTypeRelative),
      flexWrap(BNDYGWrapNoWrap),
      overflow(BNDYGOverflowVisible),
      display(BNDYGDisplayFlex),
      flex(BNDYGFloatOptional()),
      flexGrow(BNDYGFloatOptional()),
      flexShrink(BNDYGFloatOptional()),
      flexBasis(kBNDYGValueAuto),
      margin(kBNDYGDefaultEdgeValuesUnit),
      position(kBNDYGDefaultEdgeValuesUnit),
      padding(kBNDYGDefaultEdgeValuesUnit),
      border(kBNDYGDefaultEdgeValuesUnit),
      dimensions(kBNDYGDefaultDimensionValuesAutoUnit),
      minDimensions(kBNDYGDefaultDimensionValuesUnit),
      maxDimensions(kBNDYGDefaultDimensionValuesUnit),
      aspectRatio(BNDYGFloatOptional()) {}

// Yoga specific properties, not compatible with flexbox specification
bool BNDYGStyle::operator==(const BNDYGStyle& style) {
  bool areNonFloatValuesEqual = direction == style.direction &&
      flexDirection == style.flexDirection &&
      justifyContent == style.justifyContent &&
      alignContent == style.alignContent && alignItems == style.alignItems &&
      alignSelf == style.alignSelf && positionType == style.positionType &&
      flexWrap == style.flexWrap && overflow == style.overflow &&
      display == style.display && BNDYGValueEqual(flexBasis, style.flexBasis) &&
      BNDYGValueArrayEqual(margin, style.margin) &&
      BNDYGValueArrayEqual(position, style.position) &&
      BNDYGValueArrayEqual(padding, style.padding) &&
      BNDYGValueArrayEqual(border, style.border) &&
      BNDYGValueArrayEqual(dimensions, style.dimensions) &&
      BNDYGValueArrayEqual(minDimensions, style.minDimensions) &&
      BNDYGValueArrayEqual(maxDimensions, style.maxDimensions);

  areNonFloatValuesEqual =
      areNonFloatValuesEqual && flex.isUndefined() == style.flex.isUndefined();
  if (areNonFloatValuesEqual && !flex.isUndefined() &&
      !style.flex.isUndefined()) {
    areNonFloatValuesEqual =
        areNonFloatValuesEqual && flex.getValue() == style.flex.getValue();
  }

  areNonFloatValuesEqual = areNonFloatValuesEqual &&
      flexGrow.isUndefined() == style.flexGrow.isUndefined();
  if (areNonFloatValuesEqual && !flexGrow.isUndefined()) {
    areNonFloatValuesEqual = areNonFloatValuesEqual &&
        flexGrow.getValue() == style.flexGrow.getValue();
  }

  areNonFloatValuesEqual = areNonFloatValuesEqual &&
      flexShrink.isUndefined() == style.flexShrink.isUndefined();
  if (areNonFloatValuesEqual && !style.flexShrink.isUndefined()) {
    areNonFloatValuesEqual = areNonFloatValuesEqual &&
        flexShrink.getValue() == style.flexShrink.getValue();
  }

  if (!(aspectRatio.isUndefined() && style.aspectRatio.isUndefined())) {
    areNonFloatValuesEqual = areNonFloatValuesEqual &&
        aspectRatio.getValue() == style.aspectRatio.getValue();
  }

  return areNonFloatValuesEqual;
}

bool BNDYGStyle::operator!=(BNDYGStyle style) {
  return !(*this == style);
}

BNDYGStyle::~BNDYGStyle() {}
