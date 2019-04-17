/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "BNDYGConfig.h"

const std::array<bool, BNDYGExperimentalFeatureCount>
    kBNDYGDefaultExperimentalFeatures = {{false}};

BNDYGConfig::BNDYGConfig(BNDYGLogger logger)
    : experimentalFeatures(kBNDYGDefaultExperimentalFeatures),
      useWebDefaults(false),
      useLegacyStretchBehaviour(false),
      shouldDiffLayoutWithoutLegacyStretchBehaviour(false),
      pointScaleFactor(1.0f), logger(logger), cloneNodeCallback(nullptr),
      context(nullptr) {}
