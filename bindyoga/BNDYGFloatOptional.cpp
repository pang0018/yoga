/*
 *  Copyright (c) 2014-present, Facebook, Inc.
 *
 *  This source code is licensed under the MIT license found in the LICENSE
 *  file in the root directory of this source tree.
 *
 */
#include "BNDYGFloatOptional.h"
#include <cstdlib>
#include <iostream>
#include "BindYoga.h"

BNDYGFloatOptional::BNDYGFloatOptional(float value) {
  if (BNDYGFloatIsUndefined(value)) {
    isUndefined_ = true;
    value_ = 0;
  } else {
    value_ = value;
    isUndefined_ = false;
  }
}

BNDYGFloatOptional::BNDYGFloatOptional() : value_(0), isUndefined_(true) {}

const float& BNDYGFloatOptional::getValue() const {
  if (isUndefined_) {
    // Abort, accessing a value of an undefined float optional
    std::cerr << "Tried to get value of an undefined BNDYGFloatOptional\n";
    std::exit(EXIT_FAILURE);
  }
  return value_;
}

bool BNDYGFloatOptional::operator==(const BNDYGFloatOptional& op) const {
  if (isUndefined_ == op.isUndefined()) {
    return isUndefined_ || value_ == op.getValue();
  }
  return false;
}

bool BNDYGFloatOptional::operator!=(const BNDYGFloatOptional& op) const {
  return !(*this == op);
}

bool BNDYGFloatOptional::operator==(float val) const {
  if (BNDYGFloatIsUndefined(val) == isUndefined_) {
    return isUndefined_ || val == value_;
  }
  return false;
}

bool BNDYGFloatOptional::operator!=(float val) const {
  return !(*this == val);
}

BNDYGFloatOptional BNDYGFloatOptional::operator+(const BNDYGFloatOptional& op) {
  if (!isUndefined_ && !op.isUndefined_) {
    return BNDYGFloatOptional(value_ + op.value_);
  }
  return BNDYGFloatOptional();
}

bool BNDYGFloatOptional::operator>(const BNDYGFloatOptional& op) const {
  if (isUndefined_ || op.isUndefined_) {
    return false;
  }
  return value_ > op.value_;
}

bool BNDYGFloatOptional::operator<(const BNDYGFloatOptional& op) const {
  if (isUndefined_ || op.isUndefined_) {
    return false;
  }
  return value_ < op.value_;
}

bool BNDYGFloatOptional::operator>=(const BNDYGFloatOptional& op) const {
  return *this == op || *this > op;
}

bool BNDYGFloatOptional::operator<=(const BNDYGFloatOptional& op) const {
  return *this == op || *this < op;
}
