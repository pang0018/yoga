/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

/** Large positive number signifies that the property(float) is undefined.
 *Earlier we used to have BNDYGundefined as NAN, but the downside of this is that
 *we can't use -ffast-math compiler flag as it assumes all floating-point
 *calculation involve and result into finite numbers. For more information
 *regarding -ffast-math compiler flag in clang, have a look at
 *https://clang.llvm.org/docs/UsersManual.html#cmdoption-ffast-math
 **/
#define BNDYGUndefined 10E20F

#include "BNDYGEnums.h"
#include "BNDYGMacros.h"

BNDYG_EXTERN_C_BEGIN

typedef struct BNDYGSize {
  float width;
  float height;
} BNDYGSize;

typedef struct BNDYGValue {
  float value;
  BNDYGUnit unit;
} BNDYGValue;

extern const BNDYGValue BNDYGValueUndefined;
extern const BNDYGValue BNDYGValueAuto;

typedef struct BNDYGConfig *BNDYGConfigRef;

typedef struct BNDYGNode* BNDYGNodeRef;

typedef BNDYGSize (*BNDYGMeasureFunc)(BNDYGNodeRef node,
                                float width,
                                BNDYGMeasureMode widthMode,
                                float height,
                                BNDYGMeasureMode heightMode);
typedef float (*BNDYGBaselineFunc)(BNDYGNodeRef node, const float width, const float height);
typedef void (*BNDYGDirtiedFunc)(BNDYGNodeRef node);
typedef void (*BNDYGPrintFunc)(BNDYGNodeRef node);
typedef int (*BNDYGLogger)(const BNDYGConfigRef config,
                        const BNDYGNodeRef node,
                        BNDYGLogLevel level,
                        const char *format,
                        va_list args);
typedef BNDYGNodeRef (
    *BNDYGCloneNodeFunc)(BNDYGNodeRef oldNode, BNDYGNodeRef owner, int childIndex);

// BNDYGNode
WIN_EXPORT BNDYGNodeRef BNDYGNodeNew(void);
WIN_EXPORT BNDYGNodeRef BNDYGNodeNewWithConfig(const BNDYGConfigRef config);
WIN_EXPORT BNDYGNodeRef BNDYGNodeClone(const BNDYGNodeRef node);
WIN_EXPORT void BNDYGNodeFree(const BNDYGNodeRef node);
WIN_EXPORT void BNDYGNodeFreeRecursive(const BNDYGNodeRef node);
WIN_EXPORT void BNDYGNodeReset(const BNDYGNodeRef node);
WIN_EXPORT int32_t BNDYGNodeGetInstanceCount(void);

WIN_EXPORT void BNDYGNodeInsertChild(const BNDYGNodeRef node,
                                  const BNDYGNodeRef child,
                                  const uint32_t index);

// This function inserts the child BNDYGNodeRef as a children of the node received
// by parameter and set the Owner of the child object to null. This function is
// expected to be called when using BindYoga in persistent mode in order to share a
// BNDYGNodeRef object as a child of two different BindYoga trees. The child BNDYGNodeRef
// is expected to be referenced from its original owner and from a clone of its
// original owner.
WIN_EXPORT void BNDYGNodeInsertSharedChild(
    const BNDYGNodeRef node,
    const BNDYGNodeRef child,
    const uint32_t index);
WIN_EXPORT void BNDYGNodeRemoveChild(const BNDYGNodeRef node, const BNDYGNodeRef child);
WIN_EXPORT void BNDYGNodeRemoveAllChildren(const BNDYGNodeRef node);
WIN_EXPORT BNDYGNodeRef BNDYGNodeGetChild(const BNDYGNodeRef node, const uint32_t index);
WIN_EXPORT BNDYGNodeRef BNDYGNodeGetOwner(const BNDYGNodeRef node);
WIN_EXPORT BNDYGNodeRef BNDYGNodeGetParent(const BNDYGNodeRef node);
WIN_EXPORT uint32_t BNDYGNodeGetChildCount(const BNDYGNodeRef node);
WIN_EXPORT void BNDYGNodeSetChildren(
    BNDYGNodeRef const owner,
    const BNDYGNodeRef children[],
    const uint32_t count);

WIN_EXPORT void BNDYGNodeCalculateLayout(const BNDYGNodeRef node,
                                      const float availableWidth,
                                      const float availableHeight,
                                      const BNDYGDirection ownerDirection);

// Mark a node as dirty. Only valid for nodes with a custom measure function
// set.
// BNDYG knows when to mark all other nodes as dirty but because nodes with
// measure functions
// depends on information not known to BNDYG they must perform this dirty
// marking manually.
WIN_EXPORT void BNDYGNodeMarkDirty(const BNDYGNodeRef node);

// This function marks the current node and all its descendants as dirty. This function is added to test yoga benchmarks.
// This function is not expected to be used in production as calling `BNDYGCalculateLayout` will cause the recalculation of each and every node.
WIN_EXPORT void BNDYGNodeMarkDirtyAndPropogateToDescendants(const BNDYGNodeRef node);

WIN_EXPORT void BNDYGNodePrint(const BNDYGNodeRef node, const BNDYGPrintOptions options);

WIN_EXPORT bool BNDYGFloatIsUndefined(const float value);

WIN_EXPORT bool BNDYGNodeCanUseCachedMeasurement(const BNDYGMeasureMode widthMode,
                                              const float width,
                                              const BNDYGMeasureMode heightMode,
                                              const float height,
                                              const BNDYGMeasureMode lastWidthMode,
                                              const float lastWidth,
                                              const BNDYGMeasureMode lastHeightMode,
                                              const float lastHeight,
                                              const float lastComputedWidth,
                                              const float lastComputedHeight,
                                              const float marginRow,
                                              const float marginColumn,
                                              const BNDYGConfigRef config);

WIN_EXPORT void BNDYGNodeCopyStyle(const BNDYGNodeRef dstNode, const BNDYGNodeRef srcNode);

#define BNDYG_NODE_PROPERTY(type, name, paramName)                          \
  WIN_EXPORT void BNDYGNodeSet##name(const BNDYGNodeRef node, type paramName); \
  WIN_EXPORT type BNDYGNodeGet##name(const BNDYGNodeRef node);

#define BNDYG_NODE_STYLE_PROPERTY(type, name, paramName)                               \
  WIN_EXPORT void BNDYGNodeStyleSet##name(const BNDYGNodeRef node, const type paramName); \
  WIN_EXPORT type BNDYGNodeStyleGet##name(const BNDYGNodeRef node);

#define BNDYG_NODE_STYLE_PROPERTY_UNIT(type, name, paramName)                                    \
  WIN_EXPORT void BNDYGNodeStyleSet##name(const BNDYGNodeRef node, const float paramName);          \
  WIN_EXPORT void BNDYGNodeStyleSet##name##Percent(const BNDYGNodeRef node, const float paramName); \
  WIN_EXPORT type BNDYGNodeStyleGet##name(const BNDYGNodeRef node);

#define BNDYG_NODE_STYLE_PROPERTY_UNIT_AUTO(type, name, paramName) \
  BNDYG_NODE_STYLE_PROPERTY_UNIT(type, name, paramName)            \
  WIN_EXPORT void BNDYGNodeStyleSet##name##Auto(const BNDYGNodeRef node);

#define BNDYG_NODE_STYLE_EDGE_PROPERTY(type, name, paramName)    \
  WIN_EXPORT void BNDYGNodeStyleSet##name(const BNDYGNodeRef node,  \
                                       const BNDYGEdge edge,     \
                                       const type paramName); \
  WIN_EXPORT type BNDYGNodeStyleGet##name(const BNDYGNodeRef node, const BNDYGEdge edge);

#define BNDYG_NODE_STYLE_EDGE_PROPERTY_UNIT(type, name, paramName)         \
  WIN_EXPORT void BNDYGNodeStyleSet##name(const BNDYGNodeRef node,            \
                                       const BNDYGEdge edge,               \
                                       const float paramName);          \
  WIN_EXPORT void BNDYGNodeStyleSet##name##Percent(const BNDYGNodeRef node,   \
                                                const BNDYGEdge edge,      \
                                                const float paramName); \
  WIN_EXPORT WIN_STRUCT(type) BNDYGNodeStyleGet##name(const BNDYGNodeRef node, const BNDYGEdge edge);

#define BNDYG_NODE_STYLE_EDGE_PROPERTY_UNIT_AUTO(type, name) \
  WIN_EXPORT void BNDYGNodeStyleSet##name##Auto(const BNDYGNodeRef node, const BNDYGEdge edge);

#define BNDYG_NODE_LAYOUT_PROPERTY(type, name) \
  WIN_EXPORT type BNDYGNodeLayoutGet##name(const BNDYGNodeRef node);

#define BNDYG_NODE_LAYOUT_EDGE_PROPERTY(type, name) \
  WIN_EXPORT type BNDYGNodeLayoutGet##name(const BNDYGNodeRef node, const BNDYGEdge edge);

void* BNDYGNodeGetContext(BNDYGNodeRef node);
void BNDYGNodeSetContext(BNDYGNodeRef node, void* context);
BNDYGMeasureFunc BNDYGNodeGetMeasureFunc(BNDYGNodeRef node);
void BNDYGNodeSetMeasureFunc(BNDYGNodeRef node, BNDYGMeasureFunc measureFunc);
BNDYGBaselineFunc BNDYGNodeGetBaselineFunc(BNDYGNodeRef node);
void BNDYGNodeSetBaselineFunc(BNDYGNodeRef node, BNDYGBaselineFunc baselineFunc);
BNDYGDirtiedFunc BNDYGNodeGetDirtiedFunc(BNDYGNodeRef node);
void BNDYGNodeSetDirtiedFunc(BNDYGNodeRef node, BNDYGDirtiedFunc dirtiedFunc);
BNDYGPrintFunc BNDYGNodeGetPrintFunc(BNDYGNodeRef node);
void BNDYGNodeSetPrintFunc(BNDYGNodeRef node, BNDYGPrintFunc printFunc);
bool BNDYGNodeGetHasNewLayout(BNDYGNodeRef node);
void BNDYGNodeSetHasNewLayout(BNDYGNodeRef node, bool hasNewLayout);
BNDYGNodeType BNDYGNodeGetNodeType(BNDYGNodeRef node);
void BNDYGNodeSetNodeType(BNDYGNodeRef node, BNDYGNodeType nodeType);
bool BNDYGNodeIsDirty(BNDYGNodeRef node);
bool BNDYGNodeLayoutGetDidUseLegacyFlag(const BNDYGNodeRef node);

BNDYG_NODE_STYLE_PROPERTY(BNDYGDirection, Direction, direction);
BNDYG_NODE_STYLE_PROPERTY(BNDYGFlexDirection, FlexDirection, flexDirection);
BNDYG_NODE_STYLE_PROPERTY(BNDYGJustify, JustifyContent, justifyContent);
BNDYG_NODE_STYLE_PROPERTY(BNDYGAlign, AlignContent, alignContent);
BNDYG_NODE_STYLE_PROPERTY(BNDYGAlign, AlignItems, alignItems);
BNDYG_NODE_STYLE_PROPERTY(BNDYGAlign, AlignSelf, alignSelf);
BNDYG_NODE_STYLE_PROPERTY(BNDYGPositionType, PositionType, positionType);
BNDYG_NODE_STYLE_PROPERTY(BNDYGWrap, FlexWrap, flexWrap);
BNDYG_NODE_STYLE_PROPERTY(BNDYGOverflow, Overflow, overflow);
BNDYG_NODE_STYLE_PROPERTY(BNDYGDisplay, Display, display);
BNDYG_NODE_STYLE_PROPERTY(float, Flex, flex);
BNDYG_NODE_STYLE_PROPERTY(float, FlexGrow, flexGrow);
BNDYG_NODE_STYLE_PROPERTY(float, FlexShrink, flexShrink);
BNDYG_NODE_STYLE_PROPERTY_UNIT_AUTO(BNDYGValue, FlexBasis, flexBasis);

BNDYG_NODE_STYLE_EDGE_PROPERTY_UNIT(BNDYGValue, Position, position);
BNDYG_NODE_STYLE_EDGE_PROPERTY_UNIT(BNDYGValue, Margin, margin);
BNDYG_NODE_STYLE_EDGE_PROPERTY_UNIT_AUTO(BNDYGValue, Margin);
BNDYG_NODE_STYLE_EDGE_PROPERTY_UNIT(BNDYGValue, Padding, padding);
BNDYG_NODE_STYLE_EDGE_PROPERTY(float, Border, border);

BNDYG_NODE_STYLE_PROPERTY_UNIT_AUTO(BNDYGValue, Width, width);
BNDYG_NODE_STYLE_PROPERTY_UNIT_AUTO(BNDYGValue, Height, height);
BNDYG_NODE_STYLE_PROPERTY_UNIT(BNDYGValue, MinWidth, minWidth);
BNDYG_NODE_STYLE_PROPERTY_UNIT(BNDYGValue, MinHeight, minHeight);
BNDYG_NODE_STYLE_PROPERTY_UNIT(BNDYGValue, MaxWidth, maxWidth);
BNDYG_NODE_STYLE_PROPERTY_UNIT(BNDYGValue, MaxHeight, maxHeight);

// BindYoga specific properties, not compatible with flexbox specification
// Aspect ratio control the size of the undefined dimension of a node.
// Aspect ratio is encoded as a floating point value width/height. e.g. A value of 2 leads to a node
// with a width twice the size of its height while a value of 0.5 gives the opposite effect.
//
// - On a node with a set width/height aspect ratio control the size of the unset dimension
// - On a node with a set flex basis aspect ratio controls the size of the node in the cross axis if
// unset
// - On a node with a measure function aspect ratio works as though the measure function measures
// the flex basis
// - On a node with flex grow/shrink aspect ratio controls the size of the node in the cross axis if
// unset
// - Aspect ratio takes min/max dimensions into account
BNDYG_NODE_STYLE_PROPERTY(float, AspectRatio, aspectRatio);

BNDYG_NODE_LAYOUT_PROPERTY(float, Left);
BNDYG_NODE_LAYOUT_PROPERTY(float, Top);
BNDYG_NODE_LAYOUT_PROPERTY(float, Right);
BNDYG_NODE_LAYOUT_PROPERTY(float, Bottom);
BNDYG_NODE_LAYOUT_PROPERTY(float, Width);
BNDYG_NODE_LAYOUT_PROPERTY(float, Height);
BNDYG_NODE_LAYOUT_PROPERTY(BNDYGDirection, Direction);
BNDYG_NODE_LAYOUT_PROPERTY(bool, HadOverflow);
bool BNDYGNodeLayoutGetDidLegacyStretchFlagAffectLayout(const BNDYGNodeRef node);

// Get the computed values for these nodes after performing layout. If they were set using
// point values then the returned value will be the same as BNDYGNodeStyleGetXXX. However if
// they were set using a percentage value then the returned value is the computed value used
// during layout.
BNDYG_NODE_LAYOUT_EDGE_PROPERTY(float, Margin);
BNDYG_NODE_LAYOUT_EDGE_PROPERTY(float, Border);
BNDYG_NODE_LAYOUT_EDGE_PROPERTY(float, Padding);

WIN_EXPORT void BNDYGConfigSetLogger(const BNDYGConfigRef config, BNDYGLogger logger);
WIN_EXPORT void BNDYGLog(const BNDYGNodeRef node, BNDYGLogLevel level, const char *message, ...);
WIN_EXPORT void BNDYGLogWithConfig(const BNDYGConfigRef config, BNDYGLogLevel level, const char *format, ...);
WIN_EXPORT void BNDYGAssert(const bool condition, const char *message);
WIN_EXPORT void BNDYGAssertWithNode(const BNDYGNodeRef node, const bool condition, const char *message);
WIN_EXPORT void BNDYGAssertWithConfig(const BNDYGConfigRef config,
                                   const bool condition,
                                   const char *message);
// Set this to number of pixels in 1 point to round calculation results
// If you want to avoid rounding - set PointScaleFactor to 0
WIN_EXPORT void BNDYGConfigSetPointScaleFactor(const BNDYGConfigRef config, const float pixelsInPoint);
void BNDYGConfigSetShouldDiffLayoutWithoutLegacyStretchBehaviour(
    const BNDYGConfigRef config,
    const bool shouldDiffLayout);

// BindYoga previously had an error where containers would take the maximum space possible instead of
// the minimum
// like they are supposed to. In practice this resulted in implicit behaviour similar to align-self:
// stretch;
// Because this was such a long-standing bug we must allow legacy users to switch back to this
// behaviour.
WIN_EXPORT void BNDYGConfigSetUseLegacyStretchBehaviour(const BNDYGConfigRef config,
                                                     const bool useLegacyStretchBehaviour);

// BNDYGConfig
WIN_EXPORT BNDYGConfigRef BNDYGConfigNew(void);
WIN_EXPORT void BNDYGConfigFree(const BNDYGConfigRef config);
WIN_EXPORT void BNDYGConfigCopy(const BNDYGConfigRef dest, const BNDYGConfigRef src);
WIN_EXPORT int32_t BNDYGConfigGetInstanceCount(void);

WIN_EXPORT void BNDYGConfigSetExperimentalFeatureEnabled(const BNDYGConfigRef config,
                                                      const BNDYGExperimentalFeature feature,
                                                      const bool enabled);
WIN_EXPORT bool BNDYGConfigIsExperimentalFeatureEnabled(const BNDYGConfigRef config,
                                                     const BNDYGExperimentalFeature feature);

// Using the web defaults is the prefered configuration for new projects.
// Usage of non web defaults should be considered as legacy.
WIN_EXPORT void BNDYGConfigSetUseWebDefaults(const BNDYGConfigRef config, const bool enabled);
WIN_EXPORT bool BNDYGConfigGetUseWebDefaults(const BNDYGConfigRef config);

WIN_EXPORT void BNDYGConfigSetCloneNodeFunc(const BNDYGConfigRef config,
                                          const BNDYGCloneNodeFunc callback);

// Export only for C#
WIN_EXPORT BNDYGConfigRef BNDYGConfigGetDefault(void);

WIN_EXPORT void BNDYGConfigSetContext(const BNDYGConfigRef config, void *context);
WIN_EXPORT void *BNDYGConfigGetContext(const BNDYGConfigRef config);

WIN_EXPORT float BNDYGRoundValueToPixelGrid(
    const float value,
    const float pointScaleFactor,
    const bool forceCeil,
    const bool forceFloor);

BNDYG_EXTERN_C_END

#ifdef __cplusplus

#include <functional>
#include <vector>

// Calls f on each node in the tree including the given node argument.
extern void BNDYGTraversePreOrder(BNDYGNodeRef const node, std::function<void(BNDYGNodeRef node)>&& f);

extern void BNDYGNodeSetChildren(
    BNDYGNodeRef const owner,
    const std::vector<BNDYGNodeRef>& children);

#endif
