/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once
#include "BindYoga-internal.h"
#include "BindYoga.h"

struct BNDYGConfig {
  std::array<bool, BNDYGExperimentalFeatureCount> experimentalFeatures;
  bool useWebDefaults;
  bool useLegacyStretchBehaviour;
  bool shouldDiffLayoutWithoutLegacyStretchBehaviour;
  float pointScaleFactor;
  BNDYGLogger logger;
  BNDYGCloneNodeFunc cloneNodeCallback;
  void* context;

  BNDYGConfig(BNDYGLogger logger);
};
