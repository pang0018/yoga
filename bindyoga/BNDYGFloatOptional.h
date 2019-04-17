/*
 *  Copyright (c) 2014-present, Facebook, Inc.
 *
 *  This source code is licensed under the MIT license found in the LICENSE
 *  file in the root directory of this source tree.
 *
 */
#pragma once

struct BNDYGFloatOptional {
 private:
  float value_;
  bool isUndefined_;

 public:
  explicit BNDYGFloatOptional(float value);
  explicit BNDYGFloatOptional();

  // Program will terminate if the value of an undefined is accessed. Please
  // make sure to check if the optional is defined before calling this function.
  // To check if float optional is defined, use `isUndefined()`.
  const float& getValue() const;

  // Sets the value of float optional, and thus isUndefined is assigned false.
  void setValue(float val) {
    value_ = val;
    isUndefined_ = false;
  }

  bool isUndefined() const {
    return isUndefined_;
  }

  BNDYGFloatOptional operator+(const BNDYGFloatOptional& op);
  bool operator>(const BNDYGFloatOptional& op) const;
  bool operator<(const BNDYGFloatOptional& op) const;
  bool operator>=(const BNDYGFloatOptional& op) const;
  bool operator<=(const BNDYGFloatOptional& op) const;
  bool operator==(const BNDYGFloatOptional& op) const;
  bool operator!=(const BNDYGFloatOptional& op) const;

  bool operator==(float val) const;
  bool operator!=(float val) const;
};
