/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "BNDYGLayout.h"
#include "Utils.h"

const std::array<float, 2> kBNDYGDefaultDimensionValues = {
    {BNDYGUndefined, BNDYGUndefined}};

BNDYGLayout::BNDYGLayout()
    : position(),
      dimensions(kBNDYGDefaultDimensionValues),
      margin(),
      border(),
      padding(),
      direction(BNDYGDirectionInherit),
      computedFlexBasisGeneration(0),
      computedFlexBasis(BNDYGFloatOptional()),
      hadOverflow(false),
      generationCount(0),
      lastOwnerDirection((BNDYGDirection)-1),
      nextCachedMeasurementsIndex(0),
      cachedMeasurements(),
      measuredDimensions(kBNDYGDefaultDimensionValues),
      cachedLayout(BNDYGCachedMeasurement()),
      didUseLegacyFlag(false),
      doesLegacyStretchFlagAffectsLayout(false) {}

bool BNDYGLayout::operator==(BNDYGLayout layout) const {
  bool isEqual = BNDYGFloatArrayEqual(position, layout.position) &&
      BNDYGFloatArrayEqual(dimensions, layout.dimensions) &&
      BNDYGFloatArrayEqual(margin, layout.margin) &&
      BNDYGFloatArrayEqual(border, layout.border) &&
      BNDYGFloatArrayEqual(padding, layout.padding) &&
      direction == layout.direction && hadOverflow == layout.hadOverflow &&
      lastOwnerDirection == layout.lastOwnerDirection &&
      nextCachedMeasurementsIndex == layout.nextCachedMeasurementsIndex &&
      cachedLayout == layout.cachedLayout &&
      computedFlexBasis == layout.computedFlexBasis;

  for (uint32_t i = 0; i < BNDYG_MAX_CACHED_RESULT_COUNT && isEqual; ++i) {
    isEqual = isEqual && cachedMeasurements[i] == layout.cachedMeasurements[i];
  }

  if (!BNDYGFloatIsUndefined(measuredDimensions[0]) ||
      !BNDYGFloatIsUndefined(layout.measuredDimensions[0])) {
    isEqual =
        isEqual && (measuredDimensions[0] == layout.measuredDimensions[0]);
  }
  if (!BNDYGFloatIsUndefined(measuredDimensions[1]) ||
      !BNDYGFloatIsUndefined(layout.measuredDimensions[1])) {
    isEqual =
        isEqual && (measuredDimensions[1] == layout.measuredDimensions[1]);
  }

  return isEqual;
}

bool BNDYGLayout::operator!=(BNDYGLayout layout) const {
  return !(*this == layout);
}
