/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <vector>
#include "BindYoga.h"

using BNDYGVector = std::vector<BNDYGNodeRef>;

BNDYG_EXTERN_C_BEGIN

WIN_EXPORT float BNDYGRoundValueToPixelGrid(const float value,
                                         const float pointScaleFactor,
                                         const bool forceCeil,
                                         const bool forceFloor);

BNDYG_EXTERN_C_END

extern const std::array<BNDYGEdge, 4> trailingBND;
extern const std::array<BNDYGEdge, 4> leadingBND;
extern bool BNDYGValueEqual(const BNDYGValue a, const BNDYGValue b);
extern const BNDYGValue BNDYGValueUndefined;
extern const BNDYGValue BNDYGValueAuto;
extern const BNDYGValue BNDYGValueZero;

template <std::size_t size>
bool BNDYGValueArrayEqual(
    const std::array<BNDYGValue, size> val1,
    const std::array<BNDYGValue, size> val2) {
  bool areEqual = true;
  for (uint32_t i = 0; i < size && areEqual; ++i) {
    areEqual = BNDYGValueEqual(val1[i], val2[i]);
  }
  return areEqual;
}

struct BNDYGCachedMeasurement {
  float availableWidth;
  float availableHeight;
  BNDYGMeasureMode widthMeasureMode;
  BNDYGMeasureMode heightMeasureMode;

  float computedWidth;
  float computedHeight;

  BNDYGCachedMeasurement()
      : availableWidth(0),
        availableHeight(0),
        widthMeasureMode((BNDYGMeasureMode)-1),
        heightMeasureMode((BNDYGMeasureMode)-1),
        computedWidth(-1),
        computedHeight(-1) {}

  bool operator==(BNDYGCachedMeasurement measurement) const {
    bool isEqual = widthMeasureMode == measurement.widthMeasureMode &&
        heightMeasureMode == measurement.heightMeasureMode;

    if (!BNDYGFloatIsUndefined(availableWidth) ||
        !BNDYGFloatIsUndefined(measurement.availableWidth)) {
      isEqual = isEqual && availableWidth == measurement.availableWidth;
    }
    if (!BNDYGFloatIsUndefined(availableHeight) ||
        !BNDYGFloatIsUndefined(measurement.availableHeight)) {
      isEqual = isEqual && availableHeight == measurement.availableHeight;
    }
    if (!BNDYGFloatIsUndefined(computedWidth) ||
        !BNDYGFloatIsUndefined(measurement.computedWidth)) {
      isEqual = isEqual && computedWidth == measurement.computedWidth;
    }
    if (!BNDYGFloatIsUndefined(computedHeight) ||
        !BNDYGFloatIsUndefined(measurement.computedHeight)) {
      isEqual = isEqual && computedHeight == measurement.computedHeight;
    }

    return isEqual;
  }
};

// This value was chosen based on empiracle data. Even the most complicated
// layouts should not require more than 16 entries to fit within the cache.
#define BNDYG_MAX_CACHED_RESULT_COUNT 16


static const float kDefaultFlexGrow = 0.0f;
static const float kDefaultFlexShrink = 0.0f;
static const float kWebDefaultFlexShrink = 1.0f;

extern bool BNDYGFloatsEqual(const float a, const float b);
extern bool BNDYGValueEqual(const BNDYGValue a, const BNDYGValue b);
extern const BNDYGValue* BNDYGComputedEdgeValue(
    const std::array<BNDYGValue, BNDYGEdgeCount>& edges,
    const BNDYGEdge edge,
    const BNDYGValue* const defaultValue);
