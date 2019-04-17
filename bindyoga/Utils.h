/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once
#include "BNDYGNode.h"
#include "BindYoga-internal.h"

// This struct is an helper model to hold the data for step 4 of flexbox
// algo, which is collecting the flex items in a line.
//
// - itemsOnLine: Number of items which can fit in a line considering the
// available Inner dimension, the flex items computed flexbasis and their
// margin. It may be different than the difference between start and end
// indicates because we skip over absolute-positioned items.
//
// - sizeConsumedOnCurrentLine: It is accumulation of the dimensions and margin
// of all the children on the current line. This will be used in order to either
// set the dimensions of the node if none already exist or to compute the
// remaining space left for the flexible children.
//
// - totalFlexGrowFactors: total flex grow factors of flex items which are to be
// layed in the current line
//
// - totalFlexShrinkFactors: total flex shrink factors of flex items which are
// to be layed in the current line
//
// - endOfLineIndex: Its the end index of the last flex item which was examined
// and it may or may not be part of the current line(as it may be absolutely
// positioned or inculding it may have caused to overshoot availableInnerDim)
//
// - relativeChildren: Maintain a vector of the child nodes that can shrink
// and/or grow.

struct BNDYGCollectFlexItemsRowValues {
  uint32_t itemsOnLine;
  float sizeConsumedOnCurrentLine;
  float totalFlexGrowFactors;
  float totalFlexShrinkScaledFactors;
  uint32_t endOfLineIndex;
  std::vector<BNDYGNodeRef> relativeChildren;
  float remainingFreeSpace;
  // The size of the mainDim for the row after considering size, padding, margin
  // and border of flex items. This is used to calculate maxLineDim after going
  // through all the rows to decide on the main axis size of owner.
  float mainDim;
  // The size of the crossDim for the row after considering size, padding,
  // margin and border of flex items. Used for calculating containers crossSize.
  float crossDim;
};

bool BNDYGValueEqual(const BNDYGValue a, const BNDYGValue b);

// This custom float equality function returns true if either absolute
// difference between two floats is less than 0.0001f or both are undefined.
bool BNDYGFloatsEqual(const float a, const float b);

// We need custom max function, since we want that, if one argument is
// BNDYGUndefined then the max funtion should return the other argument as the max
// value. We wouldn't have needed a custom max function if BNDYGUndefined was NAN
// as fmax has the same behaviour, but with NAN we cannot use `-ffast-math`
// compiler flag.
float BNDYGFloatMax(const float a, const float b);

BNDYGFloatOptional BNDYGFloatOptionalMax(
    const BNDYGFloatOptional& op1,
    const BNDYGFloatOptional& op2);

// We need custom min function, since we want that, if one argument is
// BNDYGUndefined then the min funtion should return the other argument as the min
// value. We wouldn't have needed a custom min function if BNDYGUndefined was NAN
// as fmin has the same behaviour, but with NAN we cannot use `-ffast-math`
// compiler flag.
float BNDYGFloatMin(const float a, const float b);

// This custom float comparision function compares the array of float with
// BNDYGFloatsEqual, as the default float comparision operator will not work(Look
// at the comments of BNDYGFloatsEqual function).
template <std::size_t size>
bool BNDYGFloatArrayEqual(
    const std::array<float, size>& val1,
    const std::array<float, size>& val2) {
  bool areEqual = true;
  for (std::size_t i = 0; i < size && areEqual; ++i) {
    areEqual = BNDYGFloatsEqual(val1[i], val2[i]);
  }
  return areEqual;
}

// This function returns 0 if BNDYGFloatIsUndefined(val) is true and val otherwise
float BNDYGFloatSanitize(const float& val);

// This function unwraps optional and returns BNDYGUndefined if not defined or
// op.value otherwise
// TODO: Get rid off this function
float BNDYGUnwrapFloatOptional(const BNDYGFloatOptional& op);

BNDYGFlexDirection BNDYGFlexDirectionCross(
    const BNDYGFlexDirection flexDirection,
    const BNDYGDirection direction);

inline bool BNDYGFlexDirectionIsRow(const BNDYGFlexDirection flexDirection) {
  return flexDirection == BNDYGFlexDirectionRow ||
      flexDirection == BNDYGFlexDirectionRowReverse;
}

inline BNDYGFloatOptional BNDYGResolveValue(const BNDYGValue value, const float ownerSize) {
  switch (value.unit) {
    case BNDYGUnitUndefined:
    case BNDYGUnitAuto:
      return BNDYGFloatOptional();
    case BNDYGUnitPoint:
      return BNDYGFloatOptional(value.value);
    case BNDYGUnitPercent:
      return BNDYGFloatOptional(
          static_cast<float>(value.value * ownerSize * 0.01));
  }
  return BNDYGFloatOptional();
}

inline bool BNDYGFlexDirectionIsColumn(const BNDYGFlexDirection flexDirection) {
  return flexDirection == BNDYGFlexDirectionColumn ||
      flexDirection == BNDYGFlexDirectionColumnReverse;
}

inline BNDYGFlexDirection BNDYGResolveFlexDirection(
    const BNDYGFlexDirection flexDirection,
    const BNDYGDirection direction) {
  if (direction == BNDYGDirectionRTL) {
    if (flexDirection == BNDYGFlexDirectionRow) {
      return BNDYGFlexDirectionRowReverse;
    } else if (flexDirection == BNDYGFlexDirectionRowReverse) {
      return BNDYGFlexDirectionRow;
    }
  }

  return flexDirection;
}

static inline BNDYGFloatOptional BNDYGResolveValueMargin(
    const BNDYGValue value,
    const float ownerSize) {
  return value.unit == BNDYGUnitAuto ? BNDYGFloatOptional(0)
                                  : BNDYGResolveValue(value, ownerSize);
}
