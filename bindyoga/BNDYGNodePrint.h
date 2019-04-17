/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#pragma once
#include <string>

#include "BindYoga.h"

namespace facebook {
namespace bindyoga {

void BNDYGNodeToString(
    std::string* str,
    BNDYGNodeRef node,
    BNDYGPrintOptions options,
    uint32_t level);

} // namespace bindyoga
} // namespace facebook
