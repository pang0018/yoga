/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "BNDYGMacros.h"

BNDYG_EXTERN_C_BEGIN

#define BNDYGAlignCount 8
typedef BNDYG_ENUM_BEGIN(BNDYGAlign) {
  BNDYGAlignAuto,
  BNDYGAlignFlexStart,
  BNDYGAlignCenter,
  BNDYGAlignFlexEnd,
  BNDYGAlignStretch,
  BNDYGAlignBaseline,
  BNDYGAlignSpaceBetween,
  BNDYGAlignSpaceAround,
} BNDYG_ENUM_END(BNDYGAlign);
WIN_EXPORT const char *BNDYGAlignToString(const BNDYGAlign value);

#define BNDYGDimensionCount 2
typedef BNDYG_ENUM_BEGIN(BNDYGDimension) {
  BNDYGDimensionWidth,
  BNDYGDimensionHeight,
} BNDYG_ENUM_END(BNDYGDimension);
WIN_EXPORT const char *BNDYGDimensionToString(const BNDYGDimension value);

#define BNDYGDirectionCount 3
typedef BNDYG_ENUM_BEGIN(BNDYGDirection) {
  BNDYGDirectionInherit,
  BNDYGDirectionLTR,
  BNDYGDirectionRTL,
} BNDYG_ENUM_END(BNDYGDirection);
WIN_EXPORT const char *BNDYGDirectionToString(const BNDYGDirection value);

#define BNDYGDisplayCount 2
typedef BNDYG_ENUM_BEGIN(BNDYGDisplay) {
  BNDYGDisplayFlex,
  BNDYGDisplayNone,
} BNDYG_ENUM_END(BNDYGDisplay);
WIN_EXPORT const char *BNDYGDisplayToString(const BNDYGDisplay value);

#define BNDYGEdgeCount 9
typedef BNDYG_ENUM_BEGIN(BNDYGEdge) {
  BNDYGEdgeLeft,
  BNDYGEdgeTop,
  BNDYGEdgeRight,
  BNDYGEdgeBottom,
  BNDYGEdgeStart,
  BNDYGEdgeEnd,
  BNDYGEdgeHorizontal,
  BNDYGEdgeVertical,
  BNDYGEdgeAll,
} BNDYG_ENUM_END(BNDYGEdge);
WIN_EXPORT const char *BNDYGEdgeToString(const BNDYGEdge value);

#define BNDYGExperimentalFeatureCount 1
typedef BNDYG_ENUM_BEGIN(BNDYGExperimentalFeature) {
  BNDYGExperimentalFeatureWebFlexBasis,
} BNDYG_ENUM_END(BNDYGExperimentalFeature);
WIN_EXPORT const char *BNDYGExperimentalFeatureToString(const BNDYGExperimentalFeature value);

#define BNDYGFlexDirectionCount 4
typedef BNDYG_ENUM_BEGIN(BNDYGFlexDirection) {
  BNDYGFlexDirectionColumn,
  BNDYGFlexDirectionColumnReverse,
  BNDYGFlexDirectionRow,
  BNDYGFlexDirectionRowReverse,
} BNDYG_ENUM_END(BNDYGFlexDirection);
WIN_EXPORT const char *BNDYGFlexDirectionToString(const BNDYGFlexDirection value);

#define BNDYGJustifyCount 6
typedef BNDYG_ENUM_BEGIN(BNDYGJustify){
    BNDYGJustifyFlexStart,
    BNDYGJustifyCenter,
    BNDYGJustifyFlexEnd,
    BNDYGJustifySpaceBetween,
    BNDYGJustifySpaceAround,
    BNDYGJustifySpaceEvenly,
} BNDYG_ENUM_END(BNDYGJustify);
WIN_EXPORT const char *BNDYGJustifyToString(const BNDYGJustify value);

#define BNDYGLogLevelCount 6
typedef BNDYG_ENUM_BEGIN(BNDYGLogLevel) {
  BNDYGLogLevelError,
  BNDYGLogLevelWarn,
  BNDYGLogLevelInfo,
  BNDYGLogLevelDebug,
  BNDYGLogLevelVerbose,
  BNDYGLogLevelFatal,
} BNDYG_ENUM_END(BNDYGLogLevel);
WIN_EXPORT const char *BNDYGLogLevelToString(const BNDYGLogLevel value);

#define BNDYGMeasureModeCount 3
typedef BNDYG_ENUM_BEGIN(BNDYGMeasureMode) {
  BNDYGMeasureModeUndefined,
  BNDYGMeasureModeExactly,
  BNDYGMeasureModeAtMost,
} BNDYG_ENUM_END(BNDYGMeasureMode);
WIN_EXPORT const char *BNDYGMeasureModeToString(const BNDYGMeasureMode value);

#define BNDYGNodeTypeCount 2
typedef BNDYG_ENUM_BEGIN(BNDYGNodeType) {
  BNDYGNodeTypeDefault,
  BNDYGNodeTypeText,
} BNDYG_ENUM_END(BNDYGNodeType);
WIN_EXPORT const char *BNDYGNodeTypeToString(const BNDYGNodeType value);

#define BNDYGOverflowCount 3
typedef BNDYG_ENUM_BEGIN(BNDYGOverflow) {
  BNDYGOverflowVisible,
  BNDYGOverflowHidden,
  BNDYGOverflowScroll,
} BNDYG_ENUM_END(BNDYGOverflow);
WIN_EXPORT const char *BNDYGOverflowToString(const BNDYGOverflow value);

#define BNDYGPositionTypeCount 2
typedef BNDYG_ENUM_BEGIN(BNDYGPositionType) {
  BNDYGPositionTypeRelative,
  BNDYGPositionTypeAbsolute,
} BNDYG_ENUM_END(BNDYGPositionType);
WIN_EXPORT const char *BNDYGPositionTypeToString(const BNDYGPositionType value);

#define BNDYGPrintOptionsCount 3
typedef BNDYG_ENUM_BEGIN(BNDYGPrintOptions) {
  BNDYGPrintOptionsLayout = 1,
  BNDYGPrintOptionsStyle = 2,
  BNDYGPrintOptionsChildren = 4,
} BNDYG_ENUM_END(BNDYGPrintOptions);
WIN_EXPORT const char *BNDYGPrintOptionsToString(const BNDYGPrintOptions value);

#define BNDYGUnitCount 4
typedef BNDYG_ENUM_BEGIN(BNDYGUnit) {
  BNDYGUnitUndefined,
  BNDYGUnitPoint,
  BNDYGUnitPercent,
  BNDYGUnitAuto,
} BNDYG_ENUM_END(BNDYGUnit);
WIN_EXPORT const char *BNDYGUnitToString(const BNDYGUnit value);

#define BNDYGWrapCount 3
typedef BNDYG_ENUM_BEGIN(BNDYGWrap) {
  BNDYGWrapNoWrap,
  BNDYGWrapWrap,
  BNDYGWrapWrapReverse,
} BNDYG_ENUM_END(BNDYGWrap);
WIN_EXPORT const char *BNDYGWrapToString(const BNDYGWrap value);

BNDYG_EXTERN_C_END
