/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Utils.h"

BNDYGFlexDirection BNDYGFlexDirectionCross(
    const BNDYGFlexDirection flexDirection,
    const BNDYGDirection direction) {
  return BNDYGFlexDirectionIsColumn(flexDirection)
      ? BNDYGResolveFlexDirection(BNDYGFlexDirectionRow, direction)
      : BNDYGFlexDirectionColumn;
}

float BNDYGFloatMax(const float a, const float b) {
  if (!BNDYGFloatIsUndefined(a) && !BNDYGFloatIsUndefined(b)) {
    return fmaxf(a, b);
  }
  return BNDYGFloatIsUndefined(a) ? b : a;
}

float BNDYGFloatMin(const float a, const float b) {
  if (!BNDYGFloatIsUndefined(a) && !BNDYGFloatIsUndefined(b)) {
    return fminf(a, b);
  }

  return BNDYGFloatIsUndefined(a) ? b : a;
}

bool BNDYGValueEqual(const BNDYGValue a, const BNDYGValue b) {
  if (a.unit != b.unit) {
    return false;
  }

  if (a.unit == BNDYGUnitUndefined ||
      (BNDYGFloatIsUndefined(a.value) && BNDYGFloatIsUndefined(b.value))) {
    return true;
  }

  return fabs(a.value - b.value) < 0.0001f;
}

bool BNDYGFloatsEqual(const float a, const float b) {
  if (!BNDYGFloatIsUndefined(a) && !BNDYGFloatIsUndefined(b)) {
    return fabs(a - b) < 0.0001f;
  }
  return BNDYGFloatIsUndefined(a) && BNDYGFloatIsUndefined(b);
}

float BNDYGFloatSanitize(const float& val) {
  return BNDYGFloatIsUndefined(val) ? 0 : val;
}

float BNDYGUnwrapFloatOptional(const BNDYGFloatOptional& op) {
  return op.isUndefined() ? BNDYGUndefined : op.getValue();
}

BNDYGFloatOptional BNDYGFloatOptionalMax(
    const BNDYGFloatOptional& op1,
    const BNDYGFloatOptional& op2) {
  if (!op1.isUndefined() && !op2.isUndefined()) {
    return op1.getValue() > op2.getValue() ? op1 : op2;
  }
  return op1.isUndefined() ? op2 : op1;
}
