/*
 *  Copyright (c) 2014-present, Facebook, Inc.
 *
 *  This source code is licensed under the MIT license found in the LICENSE
 *  file in the root directory of this source tree.
 *
 */

#include "BindYoga.h"
#include <float.h>
#include <string.h>
#include <algorithm>
#include "Utils.h"
#include "BNDYGNode.h"
#include "BNDYGNodePrint.h"
#include "BindYoga-internal.h"
#ifdef _MSC_VER
#include <float.h>

/* define fmaxf if < VC12 */
#if _MSC_VER < 1800
__forceinline const float fmaxf(const float a, const float b) {
  if (!BNDYGFloatIsUndefined(a) && !BNDYGFloatIsUndefined(b)) {
    return (a > b) ? a : b;
  }
  return BNDYGFloatIsUndefined(a) ? b : a;
}
#endif
#endif

#ifdef ANDROID
static int BNDYGAndroidLog(const BNDYGConfigRef config,
                        const BNDYGNodeRef node,
                        BNDYGLogLevel level,
                        const char *format,
                        va_list args);
#else
static int BNDYGDefaultLog(const BNDYGConfigRef config,
                        const BNDYGNodeRef node,
                        BNDYGLogLevel level,
                        const char *format,
                        va_list args);
#endif

const BNDYGValue BNDYGValueZero = {0, BNDYGUnitPoint};
const BNDYGValue BNDYGValueUndefined = {BNDYGUndefined, BNDYGUnitUndefined};
const BNDYGValue BNDYGValueAuto = {BNDYGUndefined, BNDYGUnitAuto};

#ifdef ANDROID
#include <android/log.h>
static int BNDYGAndroidLog(const BNDYGConfigRef config,
                        const BNDYGNodeRef node,
                        BNDYGLogLevel level,
                        const char *format,
                        va_list args) {
  int androidLevel = BNDYGLogLevelDebug;
  switch (level) {
    case BNDYGLogLevelFatal:
      androidLevel = ANDROID_LOG_FATAL;
      break;
    case BNDYGLogLevelError:
      androidLevel = ANDROID_LOG_ERROR;
      break;
    case BNDYGLogLevelWarn:
      androidLevel = ANDROID_LOG_WARN;
      break;
    case BNDYGLogLevelInfo:
      androidLevel = ANDROID_LOG_INFO;
      break;
    case BNDYGLogLevelDebug:
      androidLevel = ANDROID_LOG_DEBUG;
      break;
    case BNDYGLogLevelVerbose:
      androidLevel = ANDROID_LOG_VERBOSE;
      break;
  }
  const int result = __android_log_vprint(androidLevel, "yoga", format, args);
  return result;
}
#else
#define BNDYG_UNUSED(x) (void)(x);

static int BNDYGDefaultLog(const BNDYGConfigRef config,
                        const BNDYGNodeRef node,
                        BNDYGLogLevel level,
                        const char *format,
                        va_list args) {
  BNDYG_UNUSED(config);
  BNDYG_UNUSED(node);
  switch (level) {
    case BNDYGLogLevelError:
    case BNDYGLogLevelFatal:
      return vfprintf(stderr, format, args);
    case BNDYGLogLevelWarn:
    case BNDYGLogLevelInfo:
    case BNDYGLogLevelDebug:
    case BNDYGLogLevelVerbose:
    default:
      return vprintf(format, args);
  }
}

#undef BNDYG_UNUSED
#endif

bool BNDYGFloatIsUndefined(const float value) {
  // Value of a float in the case of it being not defined is 10.1E20. Earlier
  // it used to be NAN, the benefit of which was that if NAN is involved in any
  // mathematical expression the result was NAN. But since we want to have
  // `-ffast-math` flag being used by compiler which assumes that the floating
  // point values are not NAN and Inf, we represent BNDYGUndefined as 10.1E20. But
  // now if BNDYGUndefined is involved in any mathematical operations this
  // value(10.1E20) would change. So the following check makes sure that if the
  // value is outside a range (-10E8, 10E8) then it is undefined.
  return value >= 10E8 || value <= -10E8;
}

const BNDYGValue* BNDYGComputedEdgeValue(
    const std::array<BNDYGValue, BNDYGEdgeCount>& edges,
    const BNDYGEdge edge,
    const BNDYGValue* const defaultValue) {
  if (edges[edge].unit != BNDYGUnitUndefined) {
    return &edges[edge];
  }

  if ((edge == BNDYGEdgeTop || edge == BNDYGEdgeBottom) &&
      edges[BNDYGEdgeVertical].unit != BNDYGUnitUndefined) {
    return &edges[BNDYGEdgeVertical];
  }

  if ((edge == BNDYGEdgeLeft || edge == BNDYGEdgeRight || edge == BNDYGEdgeStart || edge == BNDYGEdgeEnd) &&
      edges[BNDYGEdgeHorizontal].unit != BNDYGUnitUndefined) {
    return &edges[BNDYGEdgeHorizontal];
  }

  if (edges[BNDYGEdgeAll].unit != BNDYGUnitUndefined) {
    return &edges[BNDYGEdgeAll];
  }

  if (edge == BNDYGEdgeStart || edge == BNDYGEdgeEnd) {
    return &BNDYGValueUndefined;
  }

  return defaultValue;
}

void* BNDYGNodeGetContext(BNDYGNodeRef node) {
  return node->getContext();
}

void BNDYGNodeSetContext(BNDYGNodeRef node, void* context) {
  return node->setContext(context);
}

BNDYGMeasureFunc BNDYGNodeGetMeasureFunc(BNDYGNodeRef node) {
  return node->getMeasure();
}

void BNDYGNodeSetMeasureFunc(BNDYGNodeRef node, BNDYGMeasureFunc measureFunc) {
  node->setMeasureFunc(measureFunc);
}

BNDYGBaselineFunc BNDYGNodeGetBaselineFunc(BNDYGNodeRef node) {
  return node->getBaseline();
}

void BNDYGNodeSetBaselineFunc(BNDYGNodeRef node, BNDYGBaselineFunc baselineFunc) {
  node->setBaseLineFunc(baselineFunc);
}

BNDYGDirtiedFunc BNDYGNodeGetDirtiedFunc(BNDYGNodeRef node) {
  return node->getDirtied();
}

void BNDYGNodeSetDirtiedFunc(BNDYGNodeRef node, BNDYGDirtiedFunc dirtiedFunc) {
  node->setDirtiedFunc(dirtiedFunc);
}

BNDYGPrintFunc BNDYGNodeGetPrintFunc(BNDYGNodeRef node) {
  return node->getPrintFunc();
}

void BNDYGNodeSetPrintFunc(BNDYGNodeRef node, BNDYGPrintFunc printFunc) {
  node->setPrintFunc(printFunc);
}

bool BNDYGNodeGetHasNewLayout(BNDYGNodeRef node) {
  return node->getHasNewLayout();
}

void BNDYGNodeSetHasNewLayout(BNDYGNodeRef node, bool hasNewLayout) {
  node->setHasNewLayout(hasNewLayout);
}

BNDYGNodeType BNDYGNodeGetNodeType(BNDYGNodeRef node) {
  return node->getNodeType();
}

void BNDYGNodeSetNodeType(BNDYGNodeRef node, BNDYGNodeType nodeType) {
  return node->setNodeType(nodeType);
}

bool BNDYGNodeIsDirty(BNDYGNodeRef node) {
  return node->isDirty();
}

bool BNDYGNodeLayoutGetDidUseLegacyFlag(const BNDYGNodeRef node) {
  return node->didUseLegacyFlag();
}

void BNDYGNodeMarkDirtyAndPropogateToDescendants(const BNDYGNodeRef node) {
  return node->markDirtyAndPropogateDownwards();
}

int32_t gNodeInstanceCount = 0;
int32_t gBNDConfigInstanceCount = 0;

WIN_EXPORT BNDYGNodeRef BNDYGNodeNewWithConfig(const BNDYGConfigRef config) {
  const BNDYGNodeRef node = new BNDYGNode();
  BNDYGAssertWithConfig(
      config, node != nullptr, "Could not allocate memory for node");
  gNodeInstanceCount++;

  if (config->useWebDefaults) {
    node->setStyleFlexDirection(BNDYGFlexDirectionRow);
    node->setStyleAlignContent(BNDYGAlignStretch);
  }
  node->setConfig(config);
  return node;
}

BNDYGConfigRef BNDYGConfigGetDefault() {
  static BNDYGConfigRef defaultConfig = BNDYGConfigNew();
  return defaultConfig;
}

BNDYGNodeRef BNDYGNodeNew(void) {
  return BNDYGNodeNewWithConfig(BNDYGConfigGetDefault());
}

BNDYGNodeRef BNDYGNodeClone(BNDYGNodeRef oldNode) {
  BNDYGNodeRef node = new BNDYGNode(*oldNode);
  BNDYGAssertWithConfig(
      oldNode->getConfig(),
      node != nullptr,
      "Could not allocate memory for node");
  gNodeInstanceCount++;
  node->setOwner(nullptr);
  return node;
}

static BNDYGConfigRef BNDYGConfigClone(const BNDYGConfig& oldConfig) {
  const BNDYGConfigRef config = new BNDYGConfig(oldConfig);
  BNDYGAssert(config != nullptr, "Could not allocate memory for config");
  if (config == nullptr) {
    abort();
  }
  gBNDConfigInstanceCount++;
  return config;
}

static BNDYGNodeRef BNDYGNodeDeepClone(BNDYGNodeRef oldNode) {
  BNDYGNodeRef node = BNDYGNodeClone(oldNode);
  BNDYGVector vec = BNDYGVector();
  vec.reserve(oldNode->getChildren().size());
  BNDYGNodeRef childNode = nullptr;
  for (auto& item : oldNode->getChildren()) {
    childNode = BNDYGNodeDeepClone(item);
    childNode->setOwner(node);
    vec.push_back(childNode);
  }
  node->setChildren(vec);

  if (oldNode->getConfig() != nullptr) {
    node->setConfig(BNDYGConfigClone(*(oldNode->getConfig())));
  }

  return node;
}

void BNDYGNodeFree(const BNDYGNodeRef node) {
  if (BNDYGNodeRef owner = node->getOwner()) {
    owner->removeChild(node);
    node->setOwner(nullptr);
  }

  const uint32_t childCount = BNDYGNodeGetChildCount(node);
  for (uint32_t i = 0; i < childCount; i++) {
    const BNDYGNodeRef child = BNDYGNodeGetChild(node, i);
    child->setOwner(nullptr);
  }

  node->clearChildren();
  delete node;
  gNodeInstanceCount--;
}

static void BNDYGConfigFreeRecursive(const BNDYGNodeRef root) {
  if (root->getConfig() != nullptr) {
    gBNDConfigInstanceCount--;
    delete root->getConfig();
  }
  // Delete configs recursively for childrens
  for (uint32_t i = 0; i < root->getChildrenCount(); ++i) {
    BNDYGConfigFreeRecursive(root->getChild(i));
  }
}

void BNDYGNodeFreeRecursive(const BNDYGNodeRef root) {
  while (BNDYGNodeGetChildCount(root) > 0) {
    const BNDYGNodeRef child = BNDYGNodeGetChild(root, 0);
    if (child->getOwner() != root) {
      // Don't free shared nodes that we don't own.
      break;
    }
    BNDYGNodeRemoveChild(root, child);
    BNDYGNodeFreeRecursive(child);
  }
  BNDYGNodeFree(root);
}

void BNDYGNodeReset(const BNDYGNodeRef node) {
  BNDYGAssertWithNode(node,
                   BNDYGNodeGetChildCount(node) == 0,
                   "Cannot reset a node which still has children attached");
  BNDYGAssertWithNode(
      node,
      node->getOwner() == nullptr,
      "Cannot reset a node still attached to a owner");

  node->clearChildren();

  const BNDYGConfigRef config = node->getConfig();
  *node = BNDYGNode();
  if (config->useWebDefaults) {
    node->setStyleFlexDirection(BNDYGFlexDirectionRow);
    node->setStyleAlignContent(BNDYGAlignStretch);
  }
  node->setConfig(config);
}

int32_t BNDYGNodeGetInstanceCount(void) {
  return gNodeInstanceCount;
}

int32_t BNDYGConfigGetInstanceCount(void) {
  return gBNDConfigInstanceCount;
}

BNDYGConfigRef BNDYGConfigNew(void) {
  #ifdef ANDROID
  const BNDYGConfigRef config = new BNDYGConfig(BNDYGAndroidLog);
  #else
  const BNDYGConfigRef config = new BNDYGConfig(BNDYGDefaultLog);
  #endif
  gBNDConfigInstanceCount++;
  return config;
}

void BNDYGConfigFree(const BNDYGConfigRef config) {
  free(config);
  gBNDConfigInstanceCount--;
}

void BNDYGConfigCopy(const BNDYGConfigRef dest, const BNDYGConfigRef src) {
  memcpy(dest, src, sizeof(BNDYGConfig));
}

void BNDYGNodeInsertChild(const BNDYGNodeRef node, const BNDYGNodeRef child, const uint32_t index) {
  BNDYGAssertWithNode(
      node,
      child->getOwner() == nullptr,
      "Child already has a owner, it must be removed first.");

  BNDYGAssertWithNode(
      node,
      node->getMeasure() == nullptr,
      "Cannot add child: Nodes with measure functions cannot have children.");

  node->cloneChildrenIfNeeded();
  node->insertChild(child, index);
  BNDYGNodeRef owner = child->getOwner() ? nullptr : node;
  child->setOwner(owner);
  node->markDirtyAndPropogate();
}

void BNDYGNodeInsertSharedChild(
    const BNDYGNodeRef node,
    const BNDYGNodeRef child,
    const uint32_t index) {
  BNDYGAssertWithNode(
      node,
      node->getMeasure() == nullptr,
      "Cannot add child: Nodes with measure functions cannot have children.");

  node->insertChild(child, index);
  child->setOwner(nullptr);
  node->markDirtyAndPropogate();
}

void BNDYGNodeRemoveChild(const BNDYGNodeRef owner, const BNDYGNodeRef excludedChild) {
  // This algorithm is a forked variant from cloneChildrenIfNeeded in BNDYGNode
  // that excludes a child.
  const uint32_t childCount = BNDYGNodeGetChildCount(owner);

  if (childCount == 0) {
    // This is an empty set. Nothing to remove.
    return;
  }
  const BNDYGNodeRef firstChild = BNDYGNodeGetChild(owner, 0);
  if (firstChild->getOwner() == owner) {
    // If the first child has this node as its owner, we assume that it is already unique.
    // We can now try to delete a child in this list.
    if (owner->removeChild(excludedChild)) {
      excludedChild->setLayout(
          BNDYGNode().getLayout()); // layout is no longer valid
      excludedChild->setOwner(nullptr);
      owner->markDirtyAndPropogate();
    }
    return;
  }
  // Otherwise we have to clone the node list except for the child we're trying to delete.
  // We don't want to simply clone all children, because then the host will need to free
  // the clone of the child that was just deleted.
  const BNDYGCloneNodeFunc cloneNodeCallback =
      owner->getConfig()->cloneNodeCallback;
  uint32_t nextInsertIndex = 0;
  for (uint32_t i = 0; i < childCount; i++) {
    const BNDYGNodeRef oldChild = owner->getChild(i);
    if (excludedChild == oldChild) {
      // Ignore the deleted child. Don't reset its layout or owner since it is still valid
      // in the other owner. However, since this owner has now changed, we need to mark it
      // as dirty.
      owner->markDirtyAndPropogate();
      continue;
    }
    BNDYGNodeRef newChild = nullptr;
    if (cloneNodeCallback) {
      newChild = cloneNodeCallback(oldChild, owner, nextInsertIndex);
    }
    if (newChild == nullptr) {
      newChild = BNDYGNodeClone(oldChild);
    }
    owner->replaceChild(newChild, nextInsertIndex);
    newChild->setOwner(owner);

    nextInsertIndex++;
  }
  while (nextInsertIndex < childCount) {
    owner->removeChild(nextInsertIndex);
    nextInsertIndex++;
  }
}

void BNDYGNodeRemoveAllChildren(const BNDYGNodeRef owner) {
  const uint32_t childCount = BNDYGNodeGetChildCount(owner);
  if (childCount == 0) {
    // This is an empty set already. Nothing to do.
    return;
  }
  const BNDYGNodeRef firstChild = BNDYGNodeGetChild(owner, 0);
  if (firstChild->getOwner() == owner) {
    // If the first child has this node as its owner, we assume that this child set is unique.
    for (uint32_t i = 0; i < childCount; i++) {
      const BNDYGNodeRef oldChild = BNDYGNodeGetChild(owner, i);
      oldChild->setLayout(BNDYGNode().getLayout()); // layout is no longer valid
      oldChild->setOwner(nullptr);
    }
    owner->clearChildren();
    owner->markDirtyAndPropogate();
    return;
  }
  // Otherwise, we are not the owner of the child set. We don't have to do anything to clear it.
  owner->setChildren(BNDYGVector());
  owner->markDirtyAndPropogate();
}

static void BNDYGNodeSetChildrenInternal(BNDYGNodeRef const owner, const std::vector<BNDYGNodeRef> &children)
{
  if (!owner) {
    return;
  }
  if (children.size() == 0) {
    if (BNDYGNodeGetChildCount(owner) > 0) {
      for (BNDYGNodeRef const child : owner->getChildren()) {
        child->setLayout(BNDYGLayout());
        child->setOwner(nullptr);
      }
      owner->setChildren(BNDYGVector());
      owner->markDirtyAndPropogate();
    }
  } else {
    if (BNDYGNodeGetChildCount(owner) > 0) {
      for (BNDYGNodeRef const oldChild : owner->getChildren()) {
        // Our new children may have nodes in common with the old children. We don't reset these common nodes.
        if (std::find(children.begin(), children.end(), oldChild) == children.end()) {
          oldChild->setLayout(BNDYGLayout());
          oldChild->setOwner(nullptr);
        }
      }
    }
    owner->setChildren(children);
    for (BNDYGNodeRef child : children) {
      child->setOwner(owner);
    }
    owner->markDirtyAndPropogate();
  }
}

void BNDYGNodeSetChildren(BNDYGNodeRef const owner, const BNDYGNodeRef c[], const uint32_t count) {
  const BNDYGVector children = {c, c + count};
  BNDYGNodeSetChildrenInternal(owner, children);
}

void BNDYGNodeSetChildren(BNDYGNodeRef const owner, const std::vector<BNDYGNodeRef> &children)
{
  BNDYGNodeSetChildrenInternal(owner, children);
}

BNDYGNodeRef BNDYGNodeGetChild(const BNDYGNodeRef node, const uint32_t index) {
  if (index < node->getChildren().size()) {
    return node->getChild(index);
  }
  return nullptr;
}

uint32_t BNDYGNodeGetChildCount(const BNDYGNodeRef node) {
  return static_cast<uint32_t>(node->getChildren().size());
}

BNDYGNodeRef BNDYGNodeGetOwner(const BNDYGNodeRef node) {
  return node->getOwner();
}

BNDYGNodeRef BNDYGNodeGetParent(const BNDYGNodeRef node) {
  return node->getOwner();
}

void BNDYGNodeMarkDirty(const BNDYGNodeRef node) {
  BNDYGAssertWithNode(
      node,
      node->getMeasure() != nullptr,
      "Only leaf nodes with custom measure functions"
      "should manually mark themselves as dirty");

  node->markDirtyAndPropogate();
}

void BNDYGNodeCopyStyle(const BNDYGNodeRef dstNode, const BNDYGNodeRef srcNode) {
  if (!(dstNode->getStyle() == srcNode->getStyle())) {
    dstNode->setStyle(srcNode->getStyle());
    dstNode->markDirtyAndPropogate();
  }
}

float BNDYGNodeStyleGetFlexGrow(const BNDYGNodeRef node) {
  return node->getStyle().flexGrow.isUndefined()
      ? kDefaultFlexGrow
      : node->getStyle().flexGrow.getValue();
}

float BNDYGNodeStyleGetFlexShrink(const BNDYGNodeRef node) {
  return node->getStyle().flexShrink.isUndefined()
      ? (node->getConfig()->useWebDefaults ? kWebDefaultFlexShrink
                                           : kDefaultFlexShrink)
      : node->getStyle().flexShrink.getValue();
}

#define BNDYG_NODE_STYLE_PROPERTY_SETTER_IMPL(                               \
    type, name, paramName, instanceName)                                  \
  void BNDYGNodeStyleSet##name(const BNDYGNodeRef node, const type paramName) { \
    if (node->getStyle().instanceName != paramName) {                     \
      BNDYGStyle style = node->getStyle();                                   \
      style.instanceName = paramName;                                     \
      node->setStyle(style);                                              \
      node->markDirtyAndPropogate();                                      \
    }                                                                     \
  }

#define BNDYG_NODE_STYLE_PROPERTY_SETTER_UNIT_IMPL(                          \
    type, name, paramName, instanceName)                                  \
  void BNDYGNodeStyleSet##name(const BNDYGNodeRef node, const type paramName) { \
    BNDYGValue value = {                                                     \
        BNDYGFloatSanitize(paramName),                                       \
        BNDYGFloatIsUndefined(paramName) ? BNDYGUnitUndefined : BNDYGUnitPoint,    \
    };                                                                    \
    if ((node->getStyle().instanceName.value != value.value &&            \
         value.unit != BNDYGUnitUndefined) ||                                \
        node->getStyle().instanceName.unit != value.unit) {               \
      BNDYGStyle style = node->getStyle();                                   \
      style.instanceName = value;                                         \
      node->setStyle(style);                                              \
      node->markDirtyAndPropogate();                                      \
    }                                                                     \
  }                                                                       \
                                                                          \
  void BNDYGNodeStyleSet##name##Percent(                                     \
      const BNDYGNodeRef node, const type paramName) {                       \
    BNDYGValue value = {                                                     \
        BNDYGFloatSanitize(paramName),                                       \
        BNDYGFloatIsUndefined(paramName) ? BNDYGUnitUndefined : BNDYGUnitPercent,  \
    };                                                                    \
    if ((node->getStyle().instanceName.value != value.value &&            \
         value.unit != BNDYGUnitUndefined) ||                                \
        node->getStyle().instanceName.unit != value.unit) {               \
      BNDYGStyle style = node->getStyle();                                   \
                                                                          \
      style.instanceName = value;                                         \
      node->setStyle(style);                                              \
      node->markDirtyAndPropogate();                                      \
    }                                                                     \
  }

#define BNDYG_NODE_STYLE_PROPERTY_SETTER_UNIT_AUTO_IMPL(                        \
    type, name, paramName, instanceName)                                     \
  void BNDYGNodeStyleSet##name(const BNDYGNodeRef node, const type paramName) {    \
    BNDYGValue value = {                                                        \
        BNDYGFloatSanitize(paramName),                                          \
        BNDYGFloatIsUndefined(paramName) ? BNDYGUnitUndefined : BNDYGUnitPoint,       \
    };                                                                       \
    if ((node->getStyle().instanceName.value != value.value &&               \
         value.unit != BNDYGUnitUndefined) ||                                   \
        node->getStyle().instanceName.unit != value.unit) {                  \
      BNDYGStyle style = node->getStyle();                                      \
      style.instanceName = value;                                            \
      node->setStyle(style);                                                 \
      node->markDirtyAndPropogate();                                         \
    }                                                                        \
  }                                                                          \
                                                                             \
  void BNDYGNodeStyleSet##name##Percent(                                        \
      const BNDYGNodeRef node, const type paramName) {                          \
    if (node->getStyle().instanceName.value != BNDYGFloatSanitize(paramName) || \
        node->getStyle().instanceName.unit != BNDYGUnitPercent) {               \
      BNDYGStyle style = node->getStyle();                                      \
      style.instanceName.value = BNDYGFloatSanitize(paramName);                 \
      style.instanceName.unit =                                              \
          BNDYGFloatIsUndefined(paramName) ? BNDYGUnitAuto : BNDYGUnitPercent;        \
      node->setStyle(style);                                                 \
      node->markDirtyAndPropogate();                                         \
    }                                                                        \
  }                                                                          \
                                                                             \
  void BNDYGNodeStyleSet##name##Auto(const BNDYGNodeRef node) {                    \
    if (node->getStyle().instanceName.unit != BNDYGUnitAuto) {                  \
      BNDYGStyle style = node->getStyle();                                      \
      style.instanceName.value = 0;                                          \
      style.instanceName.unit = BNDYGUnitAuto;                                  \
      node->setStyle(style);                                                 \
      node->markDirtyAndPropogate();                                         \
    }                                                                        \
  }

#define BNDYG_NODE_STYLE_PROPERTY_IMPL(type, name, paramName, instanceName)  \
  BNDYG_NODE_STYLE_PROPERTY_SETTER_IMPL(type, name, paramName, instanceName) \
                                                                          \
  type BNDYGNodeStyleGet##name(const BNDYGNodeRef node) {                       \
    return node->getStyle().instanceName;                                 \
  }

#define BNDYG_NODE_STYLE_PROPERTY_UNIT_IMPL(type, name, paramName, instanceName) \
  BNDYG_NODE_STYLE_PROPERTY_SETTER_UNIT_IMPL(                                    \
      float, name, paramName, instanceName)                                   \
                                                                              \
  type BNDYGNodeStyleGet##name(const BNDYGNodeRef node) {                           \
    BNDYGValue value = node->getStyle().instanceName;                            \
    if (value.unit == BNDYGUnitUndefined || value.unit == BNDYGUnitAuto) {          \
      value.value = BNDYGUndefined;                                              \
    }                                                                         \
    return value;                                                             \
  }

#define BNDYG_NODE_STYLE_PROPERTY_UNIT_AUTO_IMPL(                       \
    type, name, paramName, instanceName)                             \
  BNDYG_NODE_STYLE_PROPERTY_SETTER_UNIT_AUTO_IMPL(                      \
      float, name, paramName, instanceName)                          \
                                                                     \
  type BNDYGNodeStyleGet##name(const BNDYGNodeRef node) {                  \
    BNDYGValue value = node->getStyle().instanceName;                   \
    if (value.unit == BNDYGUnitUndefined || value.unit == BNDYGUnitAuto) { \
      value.value = BNDYGUndefined;                                     \
    }                                                                \
    return value;                                                    \
  }

#define BNDYG_NODE_STYLE_EDGE_PROPERTY_UNIT_AUTO_IMPL(type, name, instanceName) \
  void BNDYGNodeStyleSet##name##Auto(const BNDYGNodeRef node, const BNDYGEdge edge) { \
    if (node->getStyle().instanceName[edge].unit != BNDYGUnitAuto) {            \
      BNDYGStyle style = node->getStyle();                                      \
      style.instanceName[edge].value = 0;                                    \
      style.instanceName[edge].unit = BNDYGUnitAuto;                            \
      node->setStyle(style);                                                 \
      node->markDirtyAndPropogate();                                         \
    }                                                                        \
  }

#define BNDYG_NODE_STYLE_EDGE_PROPERTY_UNIT_IMPL(                           \
    type, name, paramName, instanceName)                                 \
  void BNDYGNodeStyleSet##name(                                             \
      const BNDYGNodeRef node, const BNDYGEdge edge, const float paramName) {  \
    BNDYGValue value = {                                                    \
        BNDYGFloatSanitize(paramName),                                      \
        BNDYGFloatIsUndefined(paramName) ? BNDYGUnitUndefined : BNDYGUnitPoint,   \
    };                                                                   \
    if ((node->getStyle().instanceName[edge].value != value.value &&     \
         value.unit != BNDYGUnitUndefined) ||                               \
        node->getStyle().instanceName[edge].unit != value.unit) {        \
      BNDYGStyle style = node->getStyle();                                  \
      style.instanceName[edge] = value;                                  \
      node->setStyle(style);                                             \
      node->markDirtyAndPropogate();                                     \
    }                                                                    \
  }                                                                      \
                                                                         \
  void BNDYGNodeStyleSet##name##Percent(                                    \
      const BNDYGNodeRef node, const BNDYGEdge edge, const float paramName) {  \
    BNDYGValue value = {                                                    \
        BNDYGFloatSanitize(paramName),                                      \
        BNDYGFloatIsUndefined(paramName) ? BNDYGUnitUndefined : BNDYGUnitPercent, \
    };                                                                   \
    if ((node->getStyle().instanceName[edge].value != value.value &&     \
         value.unit != BNDYGUnitUndefined) ||                               \
        node->getStyle().instanceName[edge].unit != value.unit) {        \
      BNDYGStyle style = node->getStyle();                                  \
      style.instanceName[edge] = value;                                  \
      node->setStyle(style);                                             \
      node->markDirtyAndPropogate();                                     \
    }                                                                    \
  }                                                                      \
                                                                         \
  WIN_STRUCT(type)                                                       \
  BNDYGNodeStyleGet##name(const BNDYGNodeRef node, const BNDYGEdge edge) {        \
    BNDYGValue value = node->getStyle().instanceName[edge];                 \
    if (value.unit == BNDYGUnitUndefined || value.unit == BNDYGUnitAuto) {     \
      value.value = BNDYGUndefined;                                         \
    }                                                                    \
    return WIN_STRUCT_REF(value);                                        \
  }

#define BNDYG_NODE_LAYOUT_PROPERTY_IMPL(type, name, instanceName) \
  type BNDYGNodeLayoutGet##name(const BNDYGNodeRef node) {           \
    return node->getLayout().instanceName;                     \
  }

#define BNDYG_NODE_LAYOUT_RESOLVED_PROPERTY_IMPL(type, name, instanceName) \
  type BNDYGNodeLayoutGet##name(const BNDYGNodeRef node, const BNDYGEdge edge) { \
    BNDYGAssertWithNode(                                                   \
        node,                                                           \
        edge <= BNDYGEdgeEnd,                                              \
        "Cannot get layout properties of multi-edge shorthands");       \
                                                                        \
    if (edge == BNDYGEdgeLeft) {                                           \
      if (node->getLayout().direction == BNDYGDirectionRTL) {              \
        return node->getLayout().instanceName[BNDYGEdgeEnd];               \
      } else {                                                          \
        return node->getLayout().instanceName[BNDYGEdgeStart];             \
      }                                                                 \
    }                                                                   \
                                                                        \
    if (edge == BNDYGEdgeRight) {                                          \
      if (node->getLayout().direction == BNDYGDirectionRTL) {              \
        return node->getLayout().instanceName[BNDYGEdgeStart];             \
      } else {                                                          \
        return node->getLayout().instanceName[BNDYGEdgeEnd];               \
      }                                                                 \
    }                                                                   \
                                                                        \
    return node->getLayout().instanceName[edge];                        \
  }

// BNDYG_NODE_PROPERTY_IMPL(void *, Context, context, context);
// BNDYG_NODE_PROPERTY_IMPL(BNDYGPrintFunc, PrintFunc, printFunc, print);
// BNDYG_NODE_PROPERTY_IMPL(bool, HasNewLayout, hasNewLayout, hasNewLayout);
// BNDYG_NODE_PROPERTY_IMPL(BNDYGNodeType, NodeType, nodeType, nodeType);

BNDYG_NODE_STYLE_PROPERTY_IMPL(BNDYGDirection, Direction, direction, direction);
BNDYG_NODE_STYLE_PROPERTY_IMPL(BNDYGFlexDirection, FlexDirection, flexDirection, flexDirection);
BNDYG_NODE_STYLE_PROPERTY_IMPL(BNDYGJustify, JustifyContent, justifyContent, justifyContent);
BNDYG_NODE_STYLE_PROPERTY_IMPL(BNDYGAlign, AlignContent, alignContent, alignContent);
BNDYG_NODE_STYLE_PROPERTY_IMPL(BNDYGAlign, AlignItems, alignItems, alignItems);
BNDYG_NODE_STYLE_PROPERTY_IMPL(BNDYGAlign, AlignSelf, alignSelf, alignSelf);
BNDYG_NODE_STYLE_PROPERTY_IMPL(BNDYGPositionType, PositionType, positionType, positionType);
BNDYG_NODE_STYLE_PROPERTY_IMPL(BNDYGWrap, FlexWrap, flexWrap, flexWrap);
BNDYG_NODE_STYLE_PROPERTY_IMPL(BNDYGOverflow, Overflow, overflow, overflow);
BNDYG_NODE_STYLE_PROPERTY_IMPL(BNDYGDisplay, Display, display, display);

// TODO(T26792433): Change the API to accept BNDYGFloatOptional.
void BNDYGNodeStyleSetFlex(const BNDYGNodeRef node, const float flex) {
  if (node->getStyle().flex != flex) {
    BNDYGStyle style = node->getStyle();
    if (BNDYGFloatIsUndefined(flex)) {
      style.flex = BNDYGFloatOptional();
    } else {
      style.flex = BNDYGFloatOptional(flex);
    }
    node->setStyle(style);
    node->markDirtyAndPropogate();
  }
}

// TODO(T26792433): Change the API to accept BNDYGFloatOptional.
float BNDYGNodeStyleGetFlex(const BNDYGNodeRef node) {
  return node->getStyle().flex.isUndefined() ? BNDYGUndefined
                                             : node->getStyle().flex.getValue();
}

// TODO(T26792433): Change the API to accept BNDYGFloatOptional.
void BNDYGNodeStyleSetFlexGrow(const BNDYGNodeRef node, const float flexGrow) {
  if (node->getStyle().flexGrow != flexGrow) {
    BNDYGStyle style = node->getStyle();
    if (BNDYGFloatIsUndefined(flexGrow)) {
      style.flexGrow = BNDYGFloatOptional();
    } else {
      style.flexGrow = BNDYGFloatOptional(flexGrow);
    }
    node->setStyle(style);
    node->markDirtyAndPropogate();
  }
}

// TODO(T26792433): Change the API to accept BNDYGFloatOptional.
void BNDYGNodeStyleSetFlexShrink(const BNDYGNodeRef node, const float flexShrink) {
  if (node->getStyle().flexShrink != flexShrink) {
    BNDYGStyle style = node->getStyle();
    if (BNDYGFloatIsUndefined(flexShrink)) {
      style.flexShrink = BNDYGFloatOptional();
    } else {
      style.flexShrink = BNDYGFloatOptional(flexShrink);
    }
    node->setStyle(style);
    node->markDirtyAndPropogate();
  }
}

BNDYGValue BNDYGNodeStyleGetFlexBasis(const BNDYGNodeRef node) {
  BNDYGValue flexBasis = node->getStyle().flexBasis;
  if (flexBasis.unit == BNDYGUnitUndefined || flexBasis.unit == BNDYGUnitAuto) {
    // TODO(T26792433): Get rid off the use of BNDYGUndefined at client side
    flexBasis.value = BNDYGUndefined;
  }
  return flexBasis;
}

void BNDYGNodeStyleSetFlexBasis(const BNDYGNodeRef node, const float flexBasis) {
  BNDYGValue value = {
      BNDYGFloatSanitize(flexBasis),
      BNDYGFloatIsUndefined(flexBasis) ? BNDYGUnitUndefined : BNDYGUnitPoint,
  };
  if ((node->getStyle().flexBasis.value != value.value &&
       value.unit != BNDYGUnitUndefined) ||
      node->getStyle().flexBasis.unit != value.unit) {
    BNDYGStyle style = node->getStyle();
    style.flexBasis = value;
    node->setStyle(style);
    node->markDirtyAndPropogate();
  }
}

void BNDYGNodeStyleSetFlexBasisPercent(
    const BNDYGNodeRef node,
    const float flexBasisPercent) {
  if (node->getStyle().flexBasis.value != flexBasisPercent ||
      node->getStyle().flexBasis.unit != BNDYGUnitPercent) {
    BNDYGStyle style = node->getStyle();
    style.flexBasis.value = BNDYGFloatSanitize(flexBasisPercent);
    style.flexBasis.unit =
        BNDYGFloatIsUndefined(flexBasisPercent) ? BNDYGUnitAuto : BNDYGUnitPercent;
    node->setStyle(style);
    node->markDirtyAndPropogate();
  }
}

void BNDYGNodeStyleSetFlexBasisAuto(const BNDYGNodeRef node) {
  if (node->getStyle().flexBasis.unit != BNDYGUnitAuto) {
    BNDYGStyle style = node->getStyle();
    style.flexBasis.value = 0;
    style.flexBasis.unit = BNDYGUnitAuto;
    node->setStyle(style);
    node->markDirtyAndPropogate();
  }
}

BNDYG_NODE_STYLE_EDGE_PROPERTY_UNIT_IMPL(BNDYGValue, Position, position, position);
BNDYG_NODE_STYLE_EDGE_PROPERTY_UNIT_IMPL(BNDYGValue, Margin, margin, margin);
BNDYG_NODE_STYLE_EDGE_PROPERTY_UNIT_AUTO_IMPL(BNDYGValue, Margin, margin);
BNDYG_NODE_STYLE_EDGE_PROPERTY_UNIT_IMPL(BNDYGValue, Padding, padding, padding);

// TODO(T26792433): Change the API to accept BNDYGFloatOptional.
void BNDYGNodeStyleSetBorder(
    const BNDYGNodeRef node,
    const BNDYGEdge edge,
    const float border) {
  BNDYGValue value = {
      BNDYGFloatSanitize(border),
      BNDYGFloatIsUndefined(border) ? BNDYGUnitUndefined : BNDYGUnitPoint,
  };
  if ((node->getStyle().border[edge].value != value.value &&
       value.unit != BNDYGUnitUndefined) ||
      node->getStyle().border[edge].unit != value.unit) {
    BNDYGStyle style = node->getStyle();
    style.border[edge] = value;
    node->setStyle(style);
    node->markDirtyAndPropogate();
  }
}

float BNDYGNodeStyleGetBorder(const BNDYGNodeRef node, const BNDYGEdge edge) {
  if (node->getStyle().border[edge].unit == BNDYGUnitUndefined ||
      node->getStyle().border[edge].unit == BNDYGUnitAuto) {
    // TODO(T26792433): Rather than returning BNDYGUndefined, change the api to
    // return BNDYGFloatOptional.
    return BNDYGUndefined;
  }

  return node->getStyle().border[edge].value;
}

// BindYoga specific properties, not compatible with flexbox specification

// TODO(T26792433): Change the API to accept BNDYGFloatOptional.
float BNDYGNodeStyleGetAspectRatio(const BNDYGNodeRef node) {
  const BNDYGFloatOptional op = node->getStyle().aspectRatio;
  return op.isUndefined() ? BNDYGUndefined : op.getValue();
}

// TODO(T26792433): Change the API to accept BNDYGFloatOptional.
void BNDYGNodeStyleSetAspectRatio(const BNDYGNodeRef node, const float aspectRatio) {
  if (node->getStyle().aspectRatio != aspectRatio) {
    BNDYGStyle style = node->getStyle();
    style.aspectRatio = BNDYGFloatOptional(aspectRatio);
    node->setStyle(style);
    node->markDirtyAndPropogate();
  }
}

BNDYG_NODE_STYLE_PROPERTY_UNIT_AUTO_IMPL(BNDYGValue, Width, width, dimensions[BNDYGDimensionWidth]);
BNDYG_NODE_STYLE_PROPERTY_UNIT_AUTO_IMPL(BNDYGValue, Height, height, dimensions[BNDYGDimensionHeight]);
BNDYG_NODE_STYLE_PROPERTY_UNIT_IMPL(BNDYGValue, MinWidth, minWidth, minDimensions[BNDYGDimensionWidth]);
BNDYG_NODE_STYLE_PROPERTY_UNIT_IMPL(BNDYGValue, MinHeight, minHeight, minDimensions[BNDYGDimensionHeight]);
BNDYG_NODE_STYLE_PROPERTY_UNIT_IMPL(BNDYGValue, MaxWidth, maxWidth, maxDimensions[BNDYGDimensionWidth]);
BNDYG_NODE_STYLE_PROPERTY_UNIT_IMPL(BNDYGValue, MaxHeight, maxHeight, maxDimensions[BNDYGDimensionHeight]);
BNDYG_NODE_LAYOUT_PROPERTY_IMPL(float, Left, position[BNDYGEdgeLeft]);
BNDYG_NODE_LAYOUT_PROPERTY_IMPL(float, Top, position[BNDYGEdgeTop]);
BNDYG_NODE_LAYOUT_PROPERTY_IMPL(float, Right, position[BNDYGEdgeRight]);
BNDYG_NODE_LAYOUT_PROPERTY_IMPL(float, Bottom, position[BNDYGEdgeBottom]);
BNDYG_NODE_LAYOUT_PROPERTY_IMPL(float, Width, dimensions[BNDYGDimensionWidth]);
BNDYG_NODE_LAYOUT_PROPERTY_IMPL(float, Height, dimensions[BNDYGDimensionHeight]);
BNDYG_NODE_LAYOUT_PROPERTY_IMPL(BNDYGDirection, Direction, direction);
BNDYG_NODE_LAYOUT_PROPERTY_IMPL(bool, HadOverflow, hadOverflow);

BNDYG_NODE_LAYOUT_RESOLVED_PROPERTY_IMPL(float, Margin, margin);
BNDYG_NODE_LAYOUT_RESOLVED_PROPERTY_IMPL(float, Border, border);
BNDYG_NODE_LAYOUT_RESOLVED_PROPERTY_IMPL(float, Padding, padding);

bool BNDYGNodeLayoutGetDidLegacyStretchFlagAffectLayout(const BNDYGNodeRef node) {
  return node->getLayout().doesLegacyStretchFlagAffectsLayout;
}

uint32_t gBNDCurrentGenerationCount = 0;

bool BNDYGLayoutNodeInternal(const BNDYGNodeRef node,
                          const float availableWidth,
                          const float availableHeight,
                          const BNDYGDirection ownerDirection,
                          const BNDYGMeasureMode widthMeasureMode,
                          const BNDYGMeasureMode heightMeasureMode,
                          const float ownerWidth,
                          const float ownerHeight,
                          const bool performLayout,
                          const char *reason,
                          const BNDYGConfigRef config);

static void BNDYGNodePrintInternal(const BNDYGNodeRef node,
                                const BNDYGPrintOptions options) {
  std::string str;
  facebook::bindyoga::BNDYGNodeToString(&str, node, options, 0);
  BNDYGLog(node, BNDYGLogLevelDebug, str.c_str());
}

void BNDYGNodePrint(const BNDYGNodeRef node, const BNDYGPrintOptions options) {
  BNDYGNodePrintInternal(node, options);
}

const std::array<BNDYGEdge, 4> leadingBND = {
    {BNDYGEdgeTop, BNDYGEdgeBottom, BNDYGEdgeLeft, BNDYGEdgeRight}};

const std::array<BNDYGEdge, 4> trailingBND = {
    {BNDYGEdgeBottom, BNDYGEdgeTop, BNDYGEdgeRight, BNDYGEdgeLeft}};
static const std::array<BNDYGEdge, 4> pos = {{
    BNDYGEdgeTop,
    BNDYGEdgeBottom,
    BNDYGEdgeLeft,
    BNDYGEdgeRight,
}};

static const std::array<BNDYGDimension, 4> dim = {
    {BNDYGDimensionHeight, BNDYGDimensionHeight, BNDYGDimensionWidth, BNDYGDimensionWidth}};

static inline float BNDYGNodePaddingAndBorderForAxis(const BNDYGNodeRef node,
                                                  const BNDYGFlexDirection axis,
                                                  const float widthSize) {
  return BNDYGUnwrapFloatOptional(
      node->getLeadingPaddingAndBorder(axis, widthSize) +
      node->getTrailingPaddingAndBorder(axis, widthSize));
}

static inline BNDYGAlign BNDYGNodeAlignItem(const BNDYGNodeRef node, const BNDYGNodeRef child) {
  const BNDYGAlign align = child->getStyle().alignSelf == BNDYGAlignAuto
      ? node->getStyle().alignItems
      : child->getStyle().alignSelf;
  if (align == BNDYGAlignBaseline &&
      BNDYGFlexDirectionIsColumn(node->getStyle().flexDirection)) {
    return BNDYGAlignFlexStart;
  }
  return align;
}

static float BNDYGBaseline(const BNDYGNodeRef node) {
  if (node->getBaseline() != nullptr) {
    const float baseline = node->getBaseline()(
        node,
        node->getLayout().measuredDimensions[BNDYGDimensionWidth],
        node->getLayout().measuredDimensions[BNDYGDimensionHeight]);
    BNDYGAssertWithNode(node,
                     !BNDYGFloatIsUndefined(baseline),
                     "Expect custom baseline function to not return NaN");
    return baseline;
  }

  BNDYGNodeRef baselineChild = nullptr;
  const uint32_t childCount = BNDYGNodeGetChildCount(node);
  for (uint32_t i = 0; i < childCount; i++) {
    const BNDYGNodeRef child = BNDYGNodeGetChild(node, i);
    if (child->getLineIndex() > 0) {
      break;
    }
    if (child->getStyle().positionType == BNDYGPositionTypeAbsolute) {
      continue;
    }
    if (BNDYGNodeAlignItem(node, child) == BNDYGAlignBaseline) {
      baselineChild = child;
      break;
    }

    if (baselineChild == nullptr) {
      baselineChild = child;
    }
  }

  if (baselineChild == nullptr) {
    return node->getLayout().measuredDimensions[BNDYGDimensionHeight];
  }

  const float baseline = BNDYGBaseline(baselineChild);
  return baseline + baselineChild->getLayout().position[BNDYGEdgeTop];
}

static bool BNDYGIsBaselineLayout(const BNDYGNodeRef node) {
  if (BNDYGFlexDirectionIsColumn(node->getStyle().flexDirection)) {
    return false;
  }
  if (node->getStyle().alignItems == BNDYGAlignBaseline) {
    return true;
  }
  const uint32_t childCount = BNDYGNodeGetChildCount(node);
  for (uint32_t i = 0; i < childCount; i++) {
    const BNDYGNodeRef child = BNDYGNodeGetChild(node, i);
    if (child->getStyle().positionType == BNDYGPositionTypeRelative &&
        child->getStyle().alignSelf == BNDYGAlignBaseline) {
      return true;
    }
  }

  return false;
}

static inline float BNDYGNodeDimWithMargin(const BNDYGNodeRef node,
                                        const BNDYGFlexDirection axis,
                                        const float widthSize) {
  return node->getLayout().measuredDimensions[dim[axis]] +
      BNDYGUnwrapFloatOptional(
             node->getLeadingMargin(axis, widthSize) +
             node->getTrailingMargin(axis, widthSize));
}

static inline bool BNDYGNodeIsStyleDimDefined(const BNDYGNodeRef node,
                                           const BNDYGFlexDirection axis,
                                           const float ownerSize) {
  bool isUndefined =
      BNDYGFloatIsUndefined(node->getResolvedDimension(dim[axis]).value);
  return !(
      node->getResolvedDimension(dim[axis]).unit == BNDYGUnitAuto ||
      node->getResolvedDimension(dim[axis]).unit == BNDYGUnitUndefined ||
      (node->getResolvedDimension(dim[axis]).unit == BNDYGUnitPoint &&
       !isUndefined && node->getResolvedDimension(dim[axis]).value < 0.0f) ||
      (node->getResolvedDimension(dim[axis]).unit == BNDYGUnitPercent &&
       !isUndefined &&
       (node->getResolvedDimension(dim[axis]).value < 0.0f ||
        BNDYGFloatIsUndefined(ownerSize))));
}

static inline bool BNDYGNodeIsLayoutDimDefined(const BNDYGNodeRef node, const BNDYGFlexDirection axis) {
  const float value = node->getLayout().measuredDimensions[dim[axis]];
  return !BNDYGFloatIsUndefined(value) && value >= 0.0f;
}

static BNDYGFloatOptional BNDYGNodeBoundAxisWithinMinAndMax(
    const BNDYGNodeRef node,
    const BNDYGFlexDirection& axis,
    const float& value,
    const float& axisSize) {
  BNDYGFloatOptional min;
  BNDYGFloatOptional max;

  if (BNDYGFlexDirectionIsColumn(axis)) {
    min = BNDYGResolveValue(
        node->getStyle().minDimensions[BNDYGDimensionHeight], axisSize);
    max = BNDYGResolveValue(
        node->getStyle().maxDimensions[BNDYGDimensionHeight], axisSize);
  } else if (BNDYGFlexDirectionIsRow(axis)) {
    min = BNDYGResolveValue(
        node->getStyle().minDimensions[BNDYGDimensionWidth], axisSize);
    max = BNDYGResolveValue(
        node->getStyle().maxDimensions[BNDYGDimensionWidth], axisSize);
  }

  if (!max.isUndefined() && max.getValue() >= 0 && value > max.getValue()) {
    return max;
  }

  if (!min.isUndefined() && min.getValue() >= 0 && value < min.getValue()) {
    return min;
  }

  return BNDYGFloatOptional(value);
}

// Like BNDYGNodeBoundAxisWithinMinAndMax but also ensures that the value doesn't go
// below the
// padding and border amount.
static inline float BNDYGNodeBoundAxis(const BNDYGNodeRef node,
                                    const BNDYGFlexDirection axis,
                                    const float value,
                                    const float axisSize,
                                    const float widthSize) {
  return BNDYGFloatMax(
      BNDYGUnwrapFloatOptional(
          BNDYGNodeBoundAxisWithinMinAndMax(node, axis, value, axisSize)),
      BNDYGNodePaddingAndBorderForAxis(node, axis, widthSize));
}

static void BNDYGNodeSetChildTrailingPosition(const BNDYGNodeRef node,
                                           const BNDYGNodeRef child,
                                           const BNDYGFlexDirection axis) {
  const float size = child->getLayout().measuredDimensions[dim[axis]];
  child->setLayoutPosition(
      node->getLayout().measuredDimensions[dim[axis]] - size -
          child->getLayout().position[pos[axis]],
      trailingBND[axis]);
}

static void BNDYGConstrainMaxSizeForMode(const BNDYGNodeRef node,
                                      const enum BNDYGFlexDirection axis,
                                      const float ownerAxisSize,
                                      const float ownerWidth,
                                      BNDYGMeasureMode *mode,
                                      float *size) {
  const BNDYGFloatOptional maxSize =
      BNDYGResolveValue(
          node->getStyle().maxDimensions[dim[axis]], ownerAxisSize) +
      BNDYGFloatOptional(node->getMarginForAxis(axis, ownerWidth));
  switch (*mode) {
    case BNDYGMeasureModeExactly:
    case BNDYGMeasureModeAtMost:
      *size = (maxSize.isUndefined() || *size < maxSize.getValue())
          ? *size
          : maxSize.getValue();
      break;
    case BNDYGMeasureModeUndefined:
      if (!maxSize.isUndefined()) {
        *mode = BNDYGMeasureModeAtMost;
        *size = maxSize.getValue();
      }
      break;
  }
}

static void BNDYGNodeComputeFlexBasisForChild(const BNDYGNodeRef node,
                                           const BNDYGNodeRef child,
                                           const float width,
                                           const BNDYGMeasureMode widthMode,
                                           const float height,
                                           const float ownerWidth,
                                           const float ownerHeight,
                                           const BNDYGMeasureMode heightMode,
                                           const BNDYGDirection direction,
                                           const BNDYGConfigRef config) {
  const BNDYGFlexDirection mainAxis =
      BNDYGResolveFlexDirection(node->getStyle().flexDirection, direction);
  const bool isMainAxisRow = BNDYGFlexDirectionIsRow(mainAxis);
  const float mainAxisSize = isMainAxisRow ? width : height;
  const float mainAxisownerSize = isMainAxisRow ? ownerWidth : ownerHeight;

  float childWidth;
  float childHeight;
  BNDYGMeasureMode childWidthMeasureMode;
  BNDYGMeasureMode childHeightMeasureMode;

  const BNDYGFloatOptional resolvedFlexBasis =
      BNDYGResolveValue(child->resolveFlexBasisPtr(), mainAxisownerSize);
  const bool isRowStyleDimDefined = BNDYGNodeIsStyleDimDefined(child, BNDYGFlexDirectionRow, ownerWidth);
  const bool isColumnStyleDimDefined =
      BNDYGNodeIsStyleDimDefined(child, BNDYGFlexDirectionColumn, ownerHeight);

  if (!resolvedFlexBasis.isUndefined() && !BNDYGFloatIsUndefined(mainAxisSize)) {
    if (child->getLayout().computedFlexBasis.isUndefined() ||
        (BNDYGConfigIsExperimentalFeatureEnabled(
             child->getConfig(), BNDYGExperimentalFeatureWebFlexBasis) &&
         child->getLayout().computedFlexBasisGeneration !=
             gBNDCurrentGenerationCount)) {
      const BNDYGFloatOptional& paddingAndBorder = BNDYGFloatOptional(
          BNDYGNodePaddingAndBorderForAxis(child, mainAxis, ownerWidth));
      child->setLayoutComputedFlexBasis(
          BNDYGFloatOptionalMax(resolvedFlexBasis, paddingAndBorder));
    }
  } else if (isMainAxisRow && isRowStyleDimDefined) {
    // The width is definite, so use that as the flex basis.
    const BNDYGFloatOptional& paddingAndBorder = BNDYGFloatOptional(
        BNDYGNodePaddingAndBorderForAxis(child, BNDYGFlexDirectionRow, ownerWidth));

    child->setLayoutComputedFlexBasis(BNDYGFloatOptionalMax(
        BNDYGResolveValue(
            child->getResolvedDimension(BNDYGDimensionWidth), ownerWidth),
        paddingAndBorder));
  } else if (!isMainAxisRow && isColumnStyleDimDefined) {
    // The height is definite, so use that as the flex basis.
    const BNDYGFloatOptional& paddingAndBorder =
        BNDYGFloatOptional(BNDYGNodePaddingAndBorderForAxis(
            child, BNDYGFlexDirectionColumn, ownerWidth));
    child->setLayoutComputedFlexBasis(BNDYGFloatOptionalMax(
        BNDYGResolveValue(
            child->getResolvedDimension(BNDYGDimensionHeight), ownerHeight),
        paddingAndBorder));
  } else {
    // Compute the flex basis and hypothetical main size (i.e. the clamped
    // flex basis).
    childWidth = BNDYGUndefined;
    childHeight = BNDYGUndefined;
    childWidthMeasureMode = BNDYGMeasureModeUndefined;
    childHeightMeasureMode = BNDYGMeasureModeUndefined;

    const float& marginRow = BNDYGUnwrapFloatOptional(
        child->getMarginForAxis(BNDYGFlexDirectionRow, ownerWidth));
    const float& marginColumn = BNDYGUnwrapFloatOptional(
        child->getMarginForAxis(BNDYGFlexDirectionColumn, ownerWidth));

    if (isRowStyleDimDefined) {
      childWidth =
          BNDYGUnwrapFloatOptional(BNDYGResolveValue(
              child->getResolvedDimension(BNDYGDimensionWidth), ownerWidth)) +
          marginRow;
      childWidthMeasureMode = BNDYGMeasureModeExactly;
    }
    if (isColumnStyleDimDefined) {
      childHeight =
          BNDYGUnwrapFloatOptional(BNDYGResolveValue(
              child->getResolvedDimension(BNDYGDimensionHeight), ownerHeight)) +
          marginColumn;
      childHeightMeasureMode = BNDYGMeasureModeExactly;
    }

    // The W3C spec doesn't say anything about the 'overflow' property,
    // but all major browsers appear to implement the following logic.
    if ((!isMainAxisRow && node->getStyle().overflow == BNDYGOverflowScroll) ||
        node->getStyle().overflow != BNDYGOverflowScroll) {
      if (BNDYGFloatIsUndefined(childWidth) && !BNDYGFloatIsUndefined(width)) {
        childWidth = width;
        childWidthMeasureMode = BNDYGMeasureModeAtMost;
      }
    }

    if ((isMainAxisRow && node->getStyle().overflow == BNDYGOverflowScroll) ||
        node->getStyle().overflow != BNDYGOverflowScroll) {
      if (BNDYGFloatIsUndefined(childHeight) && !BNDYGFloatIsUndefined(height)) {
        childHeight = height;
        childHeightMeasureMode = BNDYGMeasureModeAtMost;
      }
    }

    if (!child->getStyle().aspectRatio.isUndefined()) {
      if (!isMainAxisRow && childWidthMeasureMode == BNDYGMeasureModeExactly) {
        childHeight = marginColumn +
            (childWidth - marginRow) / child->getStyle().aspectRatio.getValue();
        childHeightMeasureMode = BNDYGMeasureModeExactly;
      } else if (isMainAxisRow && childHeightMeasureMode == BNDYGMeasureModeExactly) {
        childWidth = marginRow +
            (childHeight - marginColumn) *
                child->getStyle().aspectRatio.getValue();
        childWidthMeasureMode = BNDYGMeasureModeExactly;
      }
    }

    // If child has no defined size in the cross axis and is set to stretch,
    // set the cross
    // axis to be measured exactly with the available inner width

    const bool hasExactWidth = !BNDYGFloatIsUndefined(width) && widthMode == BNDYGMeasureModeExactly;
    const bool childWidthStretch = BNDYGNodeAlignItem(node, child) == BNDYGAlignStretch &&
                                   childWidthMeasureMode != BNDYGMeasureModeExactly;
    if (!isMainAxisRow && !isRowStyleDimDefined && hasExactWidth && childWidthStretch) {
      childWidth = width;
      childWidthMeasureMode = BNDYGMeasureModeExactly;
      if (!child->getStyle().aspectRatio.isUndefined()) {
        childHeight =
            (childWidth - marginRow) / child->getStyle().aspectRatio.getValue();
        childHeightMeasureMode = BNDYGMeasureModeExactly;
      }
    }

    const bool hasExactHeight = !BNDYGFloatIsUndefined(height) && heightMode == BNDYGMeasureModeExactly;
    const bool childHeightStretch = BNDYGNodeAlignItem(node, child) == BNDYGAlignStretch &&
                                    childHeightMeasureMode != BNDYGMeasureModeExactly;
    if (isMainAxisRow && !isColumnStyleDimDefined && hasExactHeight && childHeightStretch) {
      childHeight = height;
      childHeightMeasureMode = BNDYGMeasureModeExactly;

      if (!child->getStyle().aspectRatio.isUndefined()) {
        childWidth = (childHeight - marginColumn) *
            child->getStyle().aspectRatio.getValue();
        childWidthMeasureMode = BNDYGMeasureModeExactly;
      }
    }

    BNDYGConstrainMaxSizeForMode(
        child, BNDYGFlexDirectionRow, ownerWidth, ownerWidth, &childWidthMeasureMode, &childWidth);
    BNDYGConstrainMaxSizeForMode(child,
                              BNDYGFlexDirectionColumn,
                              ownerHeight,
                              ownerWidth,
                              &childHeightMeasureMode,
                              &childHeight);

    // Measure the child
    BNDYGLayoutNodeInternal(child,
                         childWidth,
                         childHeight,
                         direction,
                         childWidthMeasureMode,
                         childHeightMeasureMode,
                         ownerWidth,
                         ownerHeight,
                         false,
                         "measure",
                         config);

    child->setLayoutComputedFlexBasis(BNDYGFloatOptional(BNDYGFloatMax(
        child->getLayout().measuredDimensions[dim[mainAxis]],
        BNDYGNodePaddingAndBorderForAxis(child, mainAxis, ownerWidth))));
  }
  child->setLayoutComputedFlexBasisGeneration(gBNDCurrentGenerationCount);
}

static void BNDYGNodeAbsoluteLayoutChild(const BNDYGNodeRef node,
                                      const BNDYGNodeRef child,
                                      const float width,
                                      const BNDYGMeasureMode widthMode,
                                      const float height,
                                      const BNDYGDirection direction,
                                      const BNDYGConfigRef config) {
  const BNDYGFlexDirection mainAxis =
      BNDYGResolveFlexDirection(node->getStyle().flexDirection, direction);
  const BNDYGFlexDirection crossAxis = BNDYGFlexDirectionCross(mainAxis, direction);
  const bool isMainAxisRow = BNDYGFlexDirectionIsRow(mainAxis);

  float childWidth = BNDYGUndefined;
  float childHeight = BNDYGUndefined;
  BNDYGMeasureMode childWidthMeasureMode = BNDYGMeasureModeUndefined;
  BNDYGMeasureMode childHeightMeasureMode = BNDYGMeasureModeUndefined;

  const float& marginRow =
      BNDYGUnwrapFloatOptional(child->getMarginForAxis(BNDYGFlexDirectionRow, width));
  const float& marginColumn = BNDYGUnwrapFloatOptional(
      child->getMarginForAxis(BNDYGFlexDirectionColumn, width));

  if (BNDYGNodeIsStyleDimDefined(child, BNDYGFlexDirectionRow, width)) {
    childWidth =
        BNDYGUnwrapFloatOptional(BNDYGResolveValue(child->getResolvedDimension(BNDYGDimensionWidth), width)) +
        marginRow;
  } else {
    // If the child doesn't have a specified width, compute the width based
    // on the left/right
    // offsets if they're defined.
    if (child->isLeadingPositionDefined(BNDYGFlexDirectionRow) &&
        child->isTrailingPosDefined(BNDYGFlexDirectionRow)) {
      childWidth = node->getLayout().measuredDimensions[BNDYGDimensionWidth] -
          (node->getLeadingBorder(BNDYGFlexDirectionRow) +
           node->getTrailingBorder(BNDYGFlexDirectionRow)) -
          BNDYGUnwrapFloatOptional(
                       child->getLeadingPosition(BNDYGFlexDirectionRow, width) +
                       child->getTrailingPosition(BNDYGFlexDirectionRow, width));
      childWidth = BNDYGNodeBoundAxis(child, BNDYGFlexDirectionRow, childWidth, width, width);
    }
  }

  if (BNDYGNodeIsStyleDimDefined(child, BNDYGFlexDirectionColumn, height)) {
    childHeight =
        BNDYGUnwrapFloatOptional(BNDYGResolveValue(child->getResolvedDimension(BNDYGDimensionHeight), height)) +
        marginColumn;
  } else {
    // If the child doesn't have a specified height, compute the height
    // based on the top/bottom
    // offsets if they're defined.
    if (child->isLeadingPositionDefined(BNDYGFlexDirectionColumn) &&
        child->isTrailingPosDefined(BNDYGFlexDirectionColumn)) {
      childHeight =
          node->getLayout().measuredDimensions[BNDYGDimensionHeight] -
          (node->getLeadingBorder(BNDYGFlexDirectionColumn) +
           node->getTrailingBorder(BNDYGFlexDirectionColumn)) -
          BNDYGUnwrapFloatOptional(
              child->getLeadingPosition(BNDYGFlexDirectionColumn, height) +
              child->getTrailingPosition(BNDYGFlexDirectionColumn, height));
      childHeight = BNDYGNodeBoundAxis(child, BNDYGFlexDirectionColumn, childHeight, height, width);
    }
  }

  // Exactly one dimension needs to be defined for us to be able to do aspect ratio
  // calculation. One dimension being the anchor and the other being flexible.
  if (BNDYGFloatIsUndefined(childWidth) ^ BNDYGFloatIsUndefined(childHeight)) {
    if (!child->getStyle().aspectRatio.isUndefined()) {
      if (BNDYGFloatIsUndefined(childWidth)) {
        childWidth = marginRow +
            (childHeight - marginColumn) *
                child->getStyle().aspectRatio.getValue();
      } else if (BNDYGFloatIsUndefined(childHeight)) {
        childHeight = marginColumn +
            (childWidth - marginRow) / child->getStyle().aspectRatio.getValue();
      }
    }
  }

  // If we're still missing one or the other dimension, measure the content.
  if (BNDYGFloatIsUndefined(childWidth) || BNDYGFloatIsUndefined(childHeight)) {
    childWidthMeasureMode =
        BNDYGFloatIsUndefined(childWidth) ? BNDYGMeasureModeUndefined : BNDYGMeasureModeExactly;
    childHeightMeasureMode =
        BNDYGFloatIsUndefined(childHeight) ? BNDYGMeasureModeUndefined : BNDYGMeasureModeExactly;

    // If the size of the owner is defined then try to constrain the absolute child to that size
    // as well. This allows text within the absolute child to wrap to the size of its owner.
    // This is the same behavior as many browsers implement.
    if (!isMainAxisRow && BNDYGFloatIsUndefined(childWidth) &&
        widthMode != BNDYGMeasureModeUndefined && !BNDYGFloatIsUndefined(width) &&
        width > 0) {
      childWidth = width;
      childWidthMeasureMode = BNDYGMeasureModeAtMost;
    }

    BNDYGLayoutNodeInternal(child,
                         childWidth,
                         childHeight,
                         direction,
                         childWidthMeasureMode,
                         childHeightMeasureMode,
                         childWidth,
                         childHeight,
                         false,
                         "abs-measure",
                         config);
    childWidth = child->getLayout().measuredDimensions[BNDYGDimensionWidth] +
        BNDYGUnwrapFloatOptional(
                     child->getMarginForAxis(BNDYGFlexDirectionRow, width));
    childHeight = child->getLayout().measuredDimensions[BNDYGDimensionHeight] +
        BNDYGUnwrapFloatOptional(
                      child->getMarginForAxis(BNDYGFlexDirectionColumn, width));
  }

  BNDYGLayoutNodeInternal(child,
                       childWidth,
                       childHeight,
                       direction,
                       BNDYGMeasureModeExactly,
                       BNDYGMeasureModeExactly,
                       childWidth,
                       childHeight,
                       true,
                       "abs-layout",
                       config);

  if (child->isTrailingPosDefined(mainAxis) &&
      !child->isLeadingPositionDefined(mainAxis)) {
    child->setLayoutPosition(
        node->getLayout().measuredDimensions[dim[mainAxis]] -
            child->getLayout().measuredDimensions[dim[mainAxis]] -
            node->getTrailingBorder(mainAxis) -
            BNDYGUnwrapFloatOptional(child->getTrailingMargin(mainAxis, width)) -
            BNDYGUnwrapFloatOptional(child->getTrailingPosition(
                mainAxis, isMainAxisRow ? width : height)),
        leadingBND[mainAxis]);
  } else if (
      !child->isLeadingPositionDefined(mainAxis) &&
      node->getStyle().justifyContent == BNDYGJustifyCenter) {
    child->setLayoutPosition(
        (node->getLayout().measuredDimensions[dim[mainAxis]] -
         child->getLayout().measuredDimensions[dim[mainAxis]]) /
            2.0f,
        leadingBND[mainAxis]);
  } else if (
      !child->isLeadingPositionDefined(mainAxis) &&
      node->getStyle().justifyContent == BNDYGJustifyFlexEnd) {
    child->setLayoutPosition(
        (node->getLayout().measuredDimensions[dim[mainAxis]] -
         child->getLayout().measuredDimensions[dim[mainAxis]]),
        leadingBND[mainAxis]);
  }

  if (child->isTrailingPosDefined(crossAxis) &&
      !child->isLeadingPositionDefined(crossAxis)) {
    child->setLayoutPosition(
        node->getLayout().measuredDimensions[dim[crossAxis]] -
            child->getLayout().measuredDimensions[dim[crossAxis]] -
            node->getTrailingBorder(crossAxis) -
            BNDYGUnwrapFloatOptional(child->getTrailingMargin(crossAxis, width)) -
            BNDYGUnwrapFloatOptional(child->getTrailingPosition(
                crossAxis, isMainAxisRow ? height : width)),
        leadingBND[crossAxis]);

  } else if (
      !child->isLeadingPositionDefined(crossAxis) &&
      BNDYGNodeAlignItem(node, child) == BNDYGAlignCenter) {
    child->setLayoutPosition(
        (node->getLayout().measuredDimensions[dim[crossAxis]] -
         child->getLayout().measuredDimensions[dim[crossAxis]]) /
            2.0f,
        leadingBND[crossAxis]);
  } else if (
      !child->isLeadingPositionDefined(crossAxis) &&
      ((BNDYGNodeAlignItem(node, child) == BNDYGAlignFlexEnd) ^
       (node->getStyle().flexWrap == BNDYGWrapWrapReverse))) {
    child->setLayoutPosition(
        (node->getLayout().measuredDimensions[dim[crossAxis]] -
         child->getLayout().measuredDimensions[dim[crossAxis]]),
        leadingBND[crossAxis]);
  }
}

static void BNDYGNodeWithMeasureFuncSetMeasuredDimensions(const BNDYGNodeRef node,
                                                       const float availableWidth,
                                                       const float availableHeight,
                                                       const BNDYGMeasureMode widthMeasureMode,
                                                       const BNDYGMeasureMode heightMeasureMode,
                                                       const float ownerWidth,
                                                       const float ownerHeight) {
  BNDYGAssertWithNode(
      node,
      node->getMeasure() != nullptr,
      "Expected node to have custom measure function");

  const float paddingAndBorderAxisRow =
      BNDYGNodePaddingAndBorderForAxis(node, BNDYGFlexDirectionRow, availableWidth);
  const float paddingAndBorderAxisColumn =
      BNDYGNodePaddingAndBorderForAxis(node, BNDYGFlexDirectionColumn, availableWidth);
  const float marginAxisRow = BNDYGUnwrapFloatOptional(
      node->getMarginForAxis(BNDYGFlexDirectionRow, availableWidth));
  const float marginAxisColumn = BNDYGUnwrapFloatOptional(
      node->getMarginForAxis(BNDYGFlexDirectionColumn, availableWidth));

  // We want to make sure we don't call measure with negative size
  const float innerWidth = BNDYGFloatIsUndefined(availableWidth)
      ? availableWidth
      : BNDYGFloatMax(0, availableWidth - marginAxisRow - paddingAndBorderAxisRow);
  const float innerHeight = BNDYGFloatIsUndefined(availableHeight)
      ? availableHeight
      : BNDYGFloatMax(
            0, availableHeight - marginAxisColumn - paddingAndBorderAxisColumn);

  if (widthMeasureMode == BNDYGMeasureModeExactly &&
      heightMeasureMode == BNDYGMeasureModeExactly) {
    // Don't bother sizing the text if both dimensions are already defined.
    node->setLayoutMeasuredDimension(
        BNDYGNodeBoundAxis(
            node,
            BNDYGFlexDirectionRow,
            availableWidth - marginAxisRow,
            ownerWidth,
            ownerWidth),
        BNDYGDimensionWidth);
    node->setLayoutMeasuredDimension(
        BNDYGNodeBoundAxis(
            node,
            BNDYGFlexDirectionColumn,
            availableHeight - marginAxisColumn,
            ownerHeight,
            ownerWidth),
        BNDYGDimensionHeight);
  } else {
    // Measure the text under the current constraints.
    const BNDYGSize measuredSize = node->getMeasure()(
        node, innerWidth, widthMeasureMode, innerHeight, heightMeasureMode);

    node->setLayoutMeasuredDimension(
        BNDYGNodeBoundAxis(
            node,
            BNDYGFlexDirectionRow,
            (widthMeasureMode == BNDYGMeasureModeUndefined ||
             widthMeasureMode == BNDYGMeasureModeAtMost)
                ? measuredSize.width + paddingAndBorderAxisRow
                : availableWidth - marginAxisRow,
            ownerWidth,
            ownerWidth),
        BNDYGDimensionWidth);

    node->setLayoutMeasuredDimension(
        BNDYGNodeBoundAxis(
            node,
            BNDYGFlexDirectionColumn,
            (heightMeasureMode == BNDYGMeasureModeUndefined ||
             heightMeasureMode == BNDYGMeasureModeAtMost)
                ? measuredSize.height + paddingAndBorderAxisColumn
                : availableHeight - marginAxisColumn,
            ownerHeight,
            ownerWidth),
        BNDYGDimensionHeight);
  }
}

// For nodes with no children, use the available values if they were provided,
// or the minimum size as indicated by the padding and border sizes.
static void BNDYGNodeEmptyContainerSetMeasuredDimensions(const BNDYGNodeRef node,
                                                      const float availableWidth,
                                                      const float availableHeight,
                                                      const BNDYGMeasureMode widthMeasureMode,
                                                      const BNDYGMeasureMode heightMeasureMode,
                                                      const float ownerWidth,
                                                      const float ownerHeight) {
  const float paddingAndBorderAxisRow =
      BNDYGNodePaddingAndBorderForAxis(node, BNDYGFlexDirectionRow, ownerWidth);
  const float paddingAndBorderAxisColumn =
      BNDYGNodePaddingAndBorderForAxis(node, BNDYGFlexDirectionColumn, ownerWidth);
  const float marginAxisRow = BNDYGUnwrapFloatOptional(
      node->getMarginForAxis(BNDYGFlexDirectionRow, ownerWidth));
  const float marginAxisColumn = BNDYGUnwrapFloatOptional(
      node->getMarginForAxis(BNDYGFlexDirectionColumn, ownerWidth));

  node->setLayoutMeasuredDimension(
      BNDYGNodeBoundAxis(
          node,
          BNDYGFlexDirectionRow,
          (widthMeasureMode == BNDYGMeasureModeUndefined ||
           widthMeasureMode == BNDYGMeasureModeAtMost)
              ? paddingAndBorderAxisRow
              : availableWidth - marginAxisRow,
          ownerWidth,
          ownerWidth),
      BNDYGDimensionWidth);

  node->setLayoutMeasuredDimension(
      BNDYGNodeBoundAxis(
          node,
          BNDYGFlexDirectionColumn,
          (heightMeasureMode == BNDYGMeasureModeUndefined ||
           heightMeasureMode == BNDYGMeasureModeAtMost)
              ? paddingAndBorderAxisColumn
              : availableHeight - marginAxisColumn,
          ownerHeight,
          ownerWidth),
      BNDYGDimensionHeight);
}

static bool BNDYGNodeFixedSizeSetMeasuredDimensions(const BNDYGNodeRef node,
                                                 const float availableWidth,
                                                 const float availableHeight,
                                                 const BNDYGMeasureMode widthMeasureMode,
                                                 const BNDYGMeasureMode heightMeasureMode,
                                                 const float ownerWidth,
                                                 const float ownerHeight) {
  if ((!BNDYGFloatIsUndefined(availableWidth) &&
       widthMeasureMode == BNDYGMeasureModeAtMost && availableWidth <= 0.0f) ||
      (!BNDYGFloatIsUndefined(availableHeight) &&
       heightMeasureMode == BNDYGMeasureModeAtMost && availableHeight <= 0.0f) ||
      (widthMeasureMode == BNDYGMeasureModeExactly &&
       heightMeasureMode == BNDYGMeasureModeExactly)) {
    const float& marginAxisColumn = BNDYGUnwrapFloatOptional(
        node->getMarginForAxis(BNDYGFlexDirectionColumn, ownerWidth));
    const float& marginAxisRow = BNDYGUnwrapFloatOptional(
        node->getMarginForAxis(BNDYGFlexDirectionRow, ownerWidth));

    node->setLayoutMeasuredDimension(
        BNDYGNodeBoundAxis(
            node,
            BNDYGFlexDirectionRow,
            BNDYGFloatIsUndefined(availableWidth) ||
                    (widthMeasureMode == BNDYGMeasureModeAtMost &&
                     availableWidth < 0.0f)
                ? 0.0f
                : availableWidth - marginAxisRow,
            ownerWidth,
            ownerWidth),
        BNDYGDimensionWidth);

    node->setLayoutMeasuredDimension(
        BNDYGNodeBoundAxis(
            node,
            BNDYGFlexDirectionColumn,
            BNDYGFloatIsUndefined(availableHeight) ||
                    (heightMeasureMode == BNDYGMeasureModeAtMost &&
                     availableHeight < 0.0f)
                ? 0.0f
                : availableHeight - marginAxisColumn,
            ownerHeight,
            ownerWidth),
        BNDYGDimensionHeight);
    return true;
  }

  return false;
}

static void BNDYGZeroOutLayoutRecursivly(const BNDYGNodeRef node) {
  memset(&(node->getLayout()), 0, sizeof(BNDYGLayout));
  node->setHasNewLayout(true);
  node->cloneChildrenIfNeeded();
  const uint32_t childCount = BNDYGNodeGetChildCount(node);
  for (uint32_t i = 0; i < childCount; i++) {
    const BNDYGNodeRef child = node->getChild(i);
    BNDYGZeroOutLayoutRecursivly(child);
  }
}

static float BNDYGNodeCalculateAvailableInnerDim(
    const BNDYGNodeRef node,
    BNDYGFlexDirection axis,
    float availableDim,
    float ownerDim) {
  BNDYGFlexDirection direction =
      BNDYGFlexDirectionIsRow(axis) ? BNDYGFlexDirectionRow : BNDYGFlexDirectionColumn;
  BNDYGDimension dimension =
      BNDYGFlexDirectionIsRow(axis) ? BNDYGDimensionWidth : BNDYGDimensionHeight;

  const float margin =
      BNDYGUnwrapFloatOptional(node->getMarginForAxis(direction, ownerDim));
  const float paddingAndBorder =
      BNDYGNodePaddingAndBorderForAxis(node, direction, ownerDim);

  float availableInnerDim = availableDim - margin - paddingAndBorder;
  // Max dimension overrides predefined dimension value; Min dimension in turn
  // overrides both of the above
  if (!BNDYGFloatIsUndefined(availableInnerDim)) {
    // We want to make sure our available height does not violate min and max
    // constraints
    const BNDYGFloatOptional minDimensionOptional = BNDYGResolveValue(node->getStyle().minDimensions[dimension], ownerDim);
    const float minInnerDim = minDimensionOptional.isUndefined()
        ? 0.0f
        : minDimensionOptional.getValue() - paddingAndBorder;

    const BNDYGFloatOptional maxDimensionOptional = BNDYGResolveValue(node->getStyle().maxDimensions[dimension], ownerDim) ;

    const float maxInnerDim = maxDimensionOptional.isUndefined()
        ? FLT_MAX
        : maxDimensionOptional.getValue() - paddingAndBorder;
    availableInnerDim =
        BNDYGFloatMax(BNDYGFloatMin(availableInnerDim, maxInnerDim), minInnerDim);
  }

  return availableInnerDim;
}

static void BNDYGNodeComputeFlexBasisForChildren(
    const BNDYGNodeRef node,
    const float availableInnerWidth,
    const float availableInnerHeight,
    BNDYGMeasureMode widthMeasureMode,
    BNDYGMeasureMode heightMeasureMode,
    BNDYGDirection direction,
    BNDYGFlexDirection mainAxis,
    const BNDYGConfigRef config,
    bool performLayout,
    float& totalOuterFlexBasis) {
  BNDYGNodeRef singleFlexChild = nullptr;
  BNDYGVector children = node->getChildren();
  BNDYGMeasureMode measureModeMainDim =
      BNDYGFlexDirectionIsRow(mainAxis) ? widthMeasureMode : heightMeasureMode;
  // If there is only one child with flexGrow + flexShrink it means we can set
  // the computedFlexBasis to 0 instead of measuring and shrinking / flexing the
  // child to exactly match the remaining space
  if (measureModeMainDim == BNDYGMeasureModeExactly) {
    for (auto child : children) {
      if (child->isNodeFlexible()) {
        if (singleFlexChild != nullptr ||
            BNDYGFloatsEqual(child->resolveFlexGrow(), 0.0f) ||
            BNDYGFloatsEqual(child->resolveFlexShrink(), 0.0f)) {
          // There is already a flexible child, or this flexible child doesn't
          // have flexGrow and flexShrink, abort
          singleFlexChild = nullptr;
          break;
        } else {
          singleFlexChild = child;
        }
      }
    }
  }

  for (auto child : children) {
    child->resolveDimension();
    if (child->getStyle().display == BNDYGDisplayNone) {
      BNDYGZeroOutLayoutRecursivly(child);
      child->setHasNewLayout(true);
      child->setDirty(false);
      continue;
    }
    if (performLayout) {
      // Set the initial position (relative to the owner).
      const BNDYGDirection childDirection = child->resolveDirection(direction);
      const float mainDim = BNDYGFlexDirectionIsRow(mainAxis)
          ? availableInnerWidth
          : availableInnerHeight;
      const float crossDim = BNDYGFlexDirectionIsRow(mainAxis)
          ? availableInnerHeight
          : availableInnerWidth;
      child->setPosition(
          childDirection, mainDim, crossDim, availableInnerWidth);
    }

    if (child->getStyle().positionType == BNDYGPositionTypeAbsolute) {
      continue;
    }
    if (child == singleFlexChild) {
      child->setLayoutComputedFlexBasisGeneration(gBNDCurrentGenerationCount);
      child->setLayoutComputedFlexBasis(BNDYGFloatOptional(0));
    } else {
      BNDYGNodeComputeFlexBasisForChild(
          node,
          child,
          availableInnerWidth,
          widthMeasureMode,
          availableInnerHeight,
          availableInnerWidth,
          availableInnerHeight,
          heightMeasureMode,
          direction,
          config);
    }

    totalOuterFlexBasis += BNDYGUnwrapFloatOptional(
        child->getLayout().computedFlexBasis +
        child->getMarginForAxis(mainAxis, availableInnerWidth));
  }
}

// This function assumes that all the children of node have their
// computedFlexBasis properly computed(To do this use
// BNDYGNodeComputeFlexBasisForChildren function).
// This function calculates BNDYGCollectFlexItemsRowMeasurement
static BNDYGCollectFlexItemsRowValues BNDYGCalculateCollectFlexItemsRowValues(
    const BNDYGNodeRef& node,
    const BNDYGDirection ownerDirection,
    const float mainAxisownerSize,
    const float availableInnerWidth,
    const float availableInnerMainDim,
    const uint32_t startOfLineIndex,
    const uint32_t lineCount) {
  BNDYGCollectFlexItemsRowValues flexAlgoRowMeasurement = {};
  flexAlgoRowMeasurement.relativeChildren.reserve(node->getChildren().size());

  float sizeConsumedOnCurrentLineIncludingMinConstraint = 0;
  const BNDYGFlexDirection mainAxis = BNDYGResolveFlexDirection(
      node->getStyle().flexDirection, node->resolveDirection(ownerDirection));
  const bool isNodeFlexWrap = node->getStyle().flexWrap != BNDYGWrapNoWrap;

  // Add items to the current line until it's full or we run out of items.
  uint32_t endOfLineIndex = startOfLineIndex;
  for (; endOfLineIndex < node->getChildrenCount(); endOfLineIndex++) {
    const BNDYGNodeRef child = node->getChild(endOfLineIndex);
    if (child->getStyle().display == BNDYGDisplayNone ||
        child->getStyle().positionType == BNDYGPositionTypeAbsolute) {
      continue;
    }
    child->setLineIndex(lineCount);
    const float childMarginMainAxis = BNDYGUnwrapFloatOptional(
        child->getMarginForAxis(mainAxis, availableInnerWidth));
    const float flexBasisWithMinAndMaxConstraints =
        BNDYGUnwrapFloatOptional(BNDYGNodeBoundAxisWithinMinAndMax(
            child,
            mainAxis,
            BNDYGUnwrapFloatOptional(child->getLayout().computedFlexBasis),
            mainAxisownerSize));

    // If this is a multi-line flow and this item pushes us over the
    // available size, we've
    // hit the end of the current line. Break out of the loop and lay out
    // the current line.
    if (sizeConsumedOnCurrentLineIncludingMinConstraint +
                flexBasisWithMinAndMaxConstraints + childMarginMainAxis >
            availableInnerMainDim &&
        isNodeFlexWrap && flexAlgoRowMeasurement.itemsOnLine > 0) {
      break;
    }

    sizeConsumedOnCurrentLineIncludingMinConstraint +=
        flexBasisWithMinAndMaxConstraints + childMarginMainAxis;
    flexAlgoRowMeasurement.sizeConsumedOnCurrentLine +=
        flexBasisWithMinAndMaxConstraints + childMarginMainAxis;
    flexAlgoRowMeasurement.itemsOnLine++;

    if (child->isNodeFlexible()) {
      flexAlgoRowMeasurement.totalFlexGrowFactors += child->resolveFlexGrow();

      // Unlike the grow factor, the shrink factor is scaled relative to the
      // child dimension.
      flexAlgoRowMeasurement.totalFlexShrinkScaledFactors +=
          -child->resolveFlexShrink() *
          BNDYGUnwrapFloatOptional(child->getLayout().computedFlexBasis);
    }

    flexAlgoRowMeasurement.relativeChildren.push_back(child);
  }

  // The total flex factor needs to be floored to 1.
  if (flexAlgoRowMeasurement.totalFlexGrowFactors > 0 &&
      flexAlgoRowMeasurement.totalFlexGrowFactors < 1) {
    flexAlgoRowMeasurement.totalFlexGrowFactors = 1;
  }

  // The total flex shrink factor needs to be floored to 1.
  if (flexAlgoRowMeasurement.totalFlexShrinkScaledFactors > 0 &&
      flexAlgoRowMeasurement.totalFlexShrinkScaledFactors < 1) {
    flexAlgoRowMeasurement.totalFlexShrinkScaledFactors = 1;
  }
  flexAlgoRowMeasurement.endOfLineIndex = endOfLineIndex;
  return flexAlgoRowMeasurement;
}

// It distributes the free space to the flexible items and ensures that the size
// of the flex items abide the min and max constraints. At the end of this
// function the child nodes would have proper size. Prior using this function
// please ensure that BNDYGDistributeFreeSpaceFirstPass is called.
static float BNDYGDistributeFreeSpaceSecondPass(
    BNDYGCollectFlexItemsRowValues& collectedFlexItemsValues,
    const BNDYGNodeRef node,
    const BNDYGFlexDirection mainAxis,
    const BNDYGFlexDirection crossAxis,
    const float mainAxisownerSize,
    const float availableInnerMainDim,
    const float availableInnerCrossDim,
    const float availableInnerWidth,
    const float availableInnerHeight,
    const bool flexBasisOverflows,
    const BNDYGMeasureMode measureModeCrossDim,
    const bool performLayout,
    const BNDYGConfigRef config) {
  float childFlexBasis = 0;
  float flexShrinkScaledFactor = 0;
  float flexGrowFactor = 0;
  float deltaFreeSpace = 0;
  const bool isMainAxisRow = BNDYGFlexDirectionIsRow(mainAxis);
  const bool isNodeFlexWrap = node->getStyle().flexWrap != BNDYGWrapNoWrap;

  for (auto currentRelativeChild : collectedFlexItemsValues.relativeChildren) {
    childFlexBasis = BNDYGUnwrapFloatOptional(BNDYGNodeBoundAxisWithinMinAndMax(
        currentRelativeChild,
        mainAxis,
        BNDYGUnwrapFloatOptional(
            currentRelativeChild->getLayout().computedFlexBasis),
        mainAxisownerSize));
    float updatedMainSize = childFlexBasis;

    if (!BNDYGFloatIsUndefined(collectedFlexItemsValues.remainingFreeSpace) &&
        collectedFlexItemsValues.remainingFreeSpace < 0) {
      flexShrinkScaledFactor =
          -currentRelativeChild->resolveFlexShrink() * childFlexBasis;
      // Is this child able to shrink?
      if (flexShrinkScaledFactor != 0) {
        float childSize;

        if (!BNDYGFloatIsUndefined(
                collectedFlexItemsValues.totalFlexShrinkScaledFactors) &&
            collectedFlexItemsValues.totalFlexShrinkScaledFactors == 0) {
          childSize = childFlexBasis + flexShrinkScaledFactor;
        } else {
          childSize = childFlexBasis +
              (collectedFlexItemsValues.remainingFreeSpace /
               collectedFlexItemsValues.totalFlexShrinkScaledFactors) *
                  flexShrinkScaledFactor;
        }

        updatedMainSize = BNDYGNodeBoundAxis(
            currentRelativeChild,
            mainAxis,
            childSize,
            availableInnerMainDim,
            availableInnerWidth);
      }
    } else if (
        !BNDYGFloatIsUndefined(collectedFlexItemsValues.remainingFreeSpace) &&
        collectedFlexItemsValues.remainingFreeSpace > 0) {
      flexGrowFactor = currentRelativeChild->resolveFlexGrow();

      // Is this child able to grow?
      if (!BNDYGFloatIsUndefined(flexGrowFactor) && flexGrowFactor != 0) {
        updatedMainSize = BNDYGNodeBoundAxis(
            currentRelativeChild,
            mainAxis,
            childFlexBasis +
                collectedFlexItemsValues.remainingFreeSpace /
                    collectedFlexItemsValues.totalFlexGrowFactors *
                    flexGrowFactor,
            availableInnerMainDim,
            availableInnerWidth);
      }
    }

    deltaFreeSpace += updatedMainSize - childFlexBasis;

    const float marginMain = BNDYGUnwrapFloatOptional(
        currentRelativeChild->getMarginForAxis(mainAxis, availableInnerWidth));
    const float marginCross = BNDYGUnwrapFloatOptional(
        currentRelativeChild->getMarginForAxis(crossAxis, availableInnerWidth));

    float childCrossSize;
    float childMainSize = updatedMainSize + marginMain;
    BNDYGMeasureMode childCrossMeasureMode;
    BNDYGMeasureMode childMainMeasureMode = BNDYGMeasureModeExactly;

    if (!currentRelativeChild->getStyle().aspectRatio.isUndefined()) {
      childCrossSize = isMainAxisRow ? (childMainSize - marginMain) /
              currentRelativeChild->getStyle().aspectRatio.getValue()
                                     : (childMainSize - marginMain) *
              currentRelativeChild->getStyle().aspectRatio.getValue();
      childCrossMeasureMode = BNDYGMeasureModeExactly;

      childCrossSize += marginCross;
    } else if (
        !BNDYGFloatIsUndefined(availableInnerCrossDim) &&
        !BNDYGNodeIsStyleDimDefined(
            currentRelativeChild, crossAxis, availableInnerCrossDim) &&
        measureModeCrossDim == BNDYGMeasureModeExactly &&
        !(isNodeFlexWrap && flexBasisOverflows) &&
        BNDYGNodeAlignItem(node, currentRelativeChild) == BNDYGAlignStretch &&
        currentRelativeChild->marginLeadingValue(crossAxis).unit !=
            BNDYGUnitAuto &&
        currentRelativeChild->marginTrailingValue(crossAxis).unit !=
            BNDYGUnitAuto) {
      childCrossSize = availableInnerCrossDim;
      childCrossMeasureMode = BNDYGMeasureModeExactly;
    } else if (!BNDYGNodeIsStyleDimDefined(
                   currentRelativeChild, crossAxis, availableInnerCrossDim)) {
      childCrossSize = availableInnerCrossDim;
      childCrossMeasureMode = BNDYGFloatIsUndefined(childCrossSize)
          ? BNDYGMeasureModeUndefined
          : BNDYGMeasureModeAtMost;
    } else {
      childCrossSize =
          BNDYGUnwrapFloatOptional(BNDYGResolveValue(
              currentRelativeChild->getResolvedDimension(dim[crossAxis]),
              availableInnerCrossDim)) +
          marginCross;
      const bool isLoosePercentageMeasurement =
          currentRelativeChild->getResolvedDimension(dim[crossAxis]).unit ==
              BNDYGUnitPercent &&
          measureModeCrossDim != BNDYGMeasureModeExactly;
      childCrossMeasureMode =
          BNDYGFloatIsUndefined(childCrossSize) || isLoosePercentageMeasurement
          ? BNDYGMeasureModeUndefined
          : BNDYGMeasureModeExactly;
    }

    BNDYGConstrainMaxSizeForMode(
        currentRelativeChild,
        mainAxis,
        availableInnerMainDim,
        availableInnerWidth,
        &childMainMeasureMode,
        &childMainSize);
    BNDYGConstrainMaxSizeForMode(
        currentRelativeChild,
        crossAxis,
        availableInnerCrossDim,
        availableInnerWidth,
        &childCrossMeasureMode,
        &childCrossSize);

    const bool requiresStretchLayout =
        !BNDYGNodeIsStyleDimDefined(
            currentRelativeChild, crossAxis, availableInnerCrossDim) &&
        BNDYGNodeAlignItem(node, currentRelativeChild) == BNDYGAlignStretch &&
        currentRelativeChild->marginLeadingValue(crossAxis).unit !=
            BNDYGUnitAuto &&
        currentRelativeChild->marginTrailingValue(crossAxis).unit != BNDYGUnitAuto;

    const float childWidth = isMainAxisRow ? childMainSize : childCrossSize;
    const float childHeight = !isMainAxisRow ? childMainSize : childCrossSize;

    const BNDYGMeasureMode childWidthMeasureMode =
        isMainAxisRow ? childMainMeasureMode : childCrossMeasureMode;
    const BNDYGMeasureMode childHeightMeasureMode =
        !isMainAxisRow ? childMainMeasureMode : childCrossMeasureMode;

    // Recursively call the layout algorithm for this child with the updated
    // main size.
    BNDYGLayoutNodeInternal(
        currentRelativeChild,
        childWidth,
        childHeight,
        node->getLayout().direction,
        childWidthMeasureMode,
        childHeightMeasureMode,
        availableInnerWidth,
        availableInnerHeight,
        performLayout && !requiresStretchLayout,
        "flex",
        config);
    node->setLayoutHadOverflow(
        node->getLayout().hadOverflow |
        currentRelativeChild->getLayout().hadOverflow);
  }
  return deltaFreeSpace;
}

// It distributes the free space to the flexible items.For those flexible items
// whose min and max constraints are triggered, those flex item's clamped size
// is removed from the remaingfreespace.
static void BNDYGDistributeFreeSpaceFirstPass(
    BNDYGCollectFlexItemsRowValues& collectedFlexItemsValues,
    const BNDYGFlexDirection mainAxis,
    const float mainAxisownerSize,
    const float availableInnerMainDim,
    const float availableInnerWidth) {
  float flexShrinkScaledFactor = 0;
  float flexGrowFactor = 0;
  float baseMainSize = 0;
  float boundMainSize = 0;
  float deltaFreeSpace = 0;

  for (auto currentRelativeChild : collectedFlexItemsValues.relativeChildren) {
    float childFlexBasis = BNDYGUnwrapFloatOptional(BNDYGNodeBoundAxisWithinMinAndMax(
        currentRelativeChild,
        mainAxis,
        BNDYGUnwrapFloatOptional(
            currentRelativeChild->getLayout().computedFlexBasis),
        mainAxisownerSize));

    if (collectedFlexItemsValues.remainingFreeSpace < 0) {
      flexShrinkScaledFactor =
          -currentRelativeChild->resolveFlexShrink() * childFlexBasis;

      // Is this child able to shrink?
      if (!BNDYGFloatIsUndefined(flexShrinkScaledFactor) &&
          flexShrinkScaledFactor != 0) {
        baseMainSize = childFlexBasis +
            collectedFlexItemsValues.remainingFreeSpace /
                collectedFlexItemsValues.totalFlexShrinkScaledFactors *
                flexShrinkScaledFactor;
        boundMainSize = BNDYGNodeBoundAxis(
            currentRelativeChild,
            mainAxis,
            baseMainSize,
            availableInnerMainDim,
            availableInnerWidth);
        if (!BNDYGFloatIsUndefined(baseMainSize) &&
            !BNDYGFloatIsUndefined(boundMainSize) &&
            baseMainSize != boundMainSize) {
          // By excluding this item's size and flex factor from remaining,
          // this item's
          // min/max constraints should also trigger in the second pass
          // resulting in the
          // item's size calculation being identical in the first and second
          // passes.
          deltaFreeSpace += boundMainSize - childFlexBasis;
          collectedFlexItemsValues.totalFlexShrinkScaledFactors -=
              flexShrinkScaledFactor;
        }
      }
    } else if (
        !BNDYGFloatIsUndefined(collectedFlexItemsValues.remainingFreeSpace) &&
        collectedFlexItemsValues.remainingFreeSpace > 0) {
      flexGrowFactor = currentRelativeChild->resolveFlexGrow();

      // Is this child able to grow?
      if (!BNDYGFloatIsUndefined(flexGrowFactor) && flexGrowFactor != 0) {
        baseMainSize = childFlexBasis +
            collectedFlexItemsValues.remainingFreeSpace /
                collectedFlexItemsValues.totalFlexGrowFactors * flexGrowFactor;
        boundMainSize = BNDYGNodeBoundAxis(
            currentRelativeChild,
            mainAxis,
            baseMainSize,
            availableInnerMainDim,
            availableInnerWidth);

        if (!BNDYGFloatIsUndefined(baseMainSize) &&
            !BNDYGFloatIsUndefined(boundMainSize) &&
            baseMainSize != boundMainSize) {
          // By excluding this item's size and flex factor from remaining,
          // this item's
          // min/max constraints should also trigger in the second pass
          // resulting in the
          // item's size calculation being identical in the first and second
          // passes.
          deltaFreeSpace += boundMainSize - childFlexBasis;
          collectedFlexItemsValues.totalFlexGrowFactors -= flexGrowFactor;
        }
      }
    }
  }
  collectedFlexItemsValues.remainingFreeSpace -= deltaFreeSpace;
}

// Do two passes over the flex items to figure out how to distribute the
// remaining space.
// The first pass finds the items whose min/max constraints trigger,
// freezes them at those
// sizes, and excludes those sizes from the remaining space. The second
// pass sets the size
// of each flexible item. It distributes the remaining space amongst the
// items whose min/max
// constraints didn't trigger in pass 1. For the other items, it sets
// their sizes by forcing
// their min/max constraints to trigger again.
//
// This two pass approach for resolving min/max constraints deviates from
// the spec. The
// spec (https://www.w3.org/TR/BNDYG-flexbox-1/#resolve-flexible-lengths)
// describes a process
// that needs to be repeated a variable number of times. The algorithm
// implemented here
// won't handle all cases but it was simpler to implement and it mitigates
// performance
// concerns because we know exactly how many passes it'll do.
//
// At the end of this function the child nodes would have the proper size
// assigned to them.
//
static void BNDYGResolveFlexibleLength(
    const BNDYGNodeRef node,
    BNDYGCollectFlexItemsRowValues& collectedFlexItemsValues,
    const BNDYGFlexDirection mainAxis,
    const BNDYGFlexDirection crossAxis,
    const float mainAxisownerSize,
    const float availableInnerMainDim,
    const float availableInnerCrossDim,
    const float availableInnerWidth,
    const float availableInnerHeight,
    const bool flexBasisOverflows,
    const BNDYGMeasureMode measureModeCrossDim,
    const bool performLayout,
    const BNDYGConfigRef config) {
  const float originalFreeSpace = collectedFlexItemsValues.remainingFreeSpace;
  // First pass: detect the flex items whose min/max constraints trigger
  BNDYGDistributeFreeSpaceFirstPass(
      collectedFlexItemsValues,
      mainAxis,
      mainAxisownerSize,
      availableInnerMainDim,
      availableInnerWidth);

  // Second pass: resolve the sizes of the flexible items
  const float distributedFreeSpace = BNDYGDistributeFreeSpaceSecondPass(
      collectedFlexItemsValues,
      node,
      mainAxis,
      crossAxis,
      mainAxisownerSize,
      availableInnerMainDim,
      availableInnerCrossDim,
      availableInnerWidth,
      availableInnerHeight,
      flexBasisOverflows,
      measureModeCrossDim,
      performLayout,
      config);

  collectedFlexItemsValues.remainingFreeSpace =
      originalFreeSpace - distributedFreeSpace;
}

static void BNDYGJustifyMainAxis(
    const BNDYGNodeRef node,
    BNDYGCollectFlexItemsRowValues& collectedFlexItemsValues,
    const uint32_t& startOfLineIndex,
    const BNDYGFlexDirection& mainAxis,
    const BNDYGFlexDirection& crossAxis,
    const BNDYGMeasureMode& measureModeMainDim,
    const BNDYGMeasureMode& measureModeCrossDim,
    const float& mainAxisownerSize,
    const float& ownerWidth,
    const float& availableInnerMainDim,
    const float& availableInnerCrossDim,
    const float& availableInnerWidth,
    const bool& performLayout) {
  const BNDYGStyle style = node->getStyle();

  // If we are using "at most" rules in the main axis. Calculate the remaining
  // space when constraint by the min size defined for the main axis.
  if (measureModeMainDim == BNDYGMeasureModeAtMost &&
      collectedFlexItemsValues.remainingFreeSpace > 0) {
    if (style.minDimensions[dim[mainAxis]].unit != BNDYGUnitUndefined &&
        !BNDYGResolveValue(style.minDimensions[dim[mainAxis]], mainAxisownerSize)
             .isUndefined()) {
      collectedFlexItemsValues.remainingFreeSpace = BNDYGFloatMax(
          0,
          BNDYGUnwrapFloatOptional(BNDYGResolveValue(
              style.minDimensions[dim[mainAxis]], mainAxisownerSize)) -
              (availableInnerMainDim -
               collectedFlexItemsValues.remainingFreeSpace));
    } else {
      collectedFlexItemsValues.remainingFreeSpace = 0;
    }
  }

  int numberOfAutoMarginsOnCurrentLine = 0;
  for (uint32_t i = startOfLineIndex;
       i < collectedFlexItemsValues.endOfLineIndex;
       i++) {
    const BNDYGNodeRef child = node->getChild(i);
    if (child->getStyle().positionType == BNDYGPositionTypeRelative) {
      if (child->marginLeadingValue(mainAxis).unit == BNDYGUnitAuto) {
        numberOfAutoMarginsOnCurrentLine++;
      }
      if (child->marginTrailingValue(mainAxis).unit == BNDYGUnitAuto) {
        numberOfAutoMarginsOnCurrentLine++;
      }
    }
  }

  // In order to position the elements in the main axis, we have two
  // controls. The space between the beginning and the first element
  // and the space between each two elements.
  float leadingMainDim = 0;
  float betweenMainDim = 0;
  const BNDYGJustify justifyContent = node->getStyle().justifyContent;

  if (numberOfAutoMarginsOnCurrentLine == 0) {
    switch (justifyContent) {
      case BNDYGJustifyCenter:
        leadingMainDim = collectedFlexItemsValues.remainingFreeSpace / 2;
        break;
      case BNDYGJustifyFlexEnd:
        leadingMainDim = collectedFlexItemsValues.remainingFreeSpace;
        break;
      case BNDYGJustifySpaceBetween:
        if (collectedFlexItemsValues.itemsOnLine > 1) {
          betweenMainDim =
              BNDYGFloatMax(collectedFlexItemsValues.remainingFreeSpace, 0) /
              (collectedFlexItemsValues.itemsOnLine - 1);
        } else {
          betweenMainDim = 0;
        }
        break;
      case BNDYGJustifySpaceEvenly:
        // Space is distributed evenly across all elements
        betweenMainDim = collectedFlexItemsValues.remainingFreeSpace /
            (collectedFlexItemsValues.itemsOnLine + 1);
        leadingMainDim = betweenMainDim;
        break;
      case BNDYGJustifySpaceAround:
        // Space on the edges is half of the space between elements
        betweenMainDim = collectedFlexItemsValues.remainingFreeSpace /
            collectedFlexItemsValues.itemsOnLine;
        leadingMainDim = betweenMainDim / 2;
        break;
      case BNDYGJustifyFlexStart:
        break;
    }
  }

  const float leadingPaddingAndBorderMain = BNDYGUnwrapFloatOptional(
      node->getLeadingPaddingAndBorder(mainAxis, ownerWidth));
  collectedFlexItemsValues.mainDim =
      leadingPaddingAndBorderMain + leadingMainDim;
  collectedFlexItemsValues.crossDim = 0;

  for (uint32_t i = startOfLineIndex;
       i < collectedFlexItemsValues.endOfLineIndex;
       i++) {
    const BNDYGNodeRef child = node->getChild(i);
    const BNDYGStyle childStyle = child->getStyle();
    const BNDYGLayout childLayout = child->getLayout();
    if (childStyle.display == BNDYGDisplayNone) {
      continue;
    }
    if (childStyle.positionType == BNDYGPositionTypeAbsolute &&
        child->isLeadingPositionDefined(mainAxis)) {
      if (performLayout) {
        // In case the child is position absolute and has left/top being
        // defined, we override the position to whatever the user said
        // (and margin/border).
        child->setLayoutPosition(
            BNDYGUnwrapFloatOptional(
                child->getLeadingPosition(mainAxis, availableInnerMainDim)) +
                node->getLeadingBorder(mainAxis) +
                BNDYGUnwrapFloatOptional(
                    child->getLeadingMargin(mainAxis, availableInnerWidth)),
            pos[mainAxis]);
      }
    } else {
      // Now that we placed the element, we need to update the variables.
      // We need to do that only for relative elements. Absolute elements
      // do not take part in that phase.
      if (childStyle.positionType == BNDYGPositionTypeRelative) {
        if (child->marginLeadingValue(mainAxis).unit == BNDYGUnitAuto) {
          collectedFlexItemsValues.mainDim +=
              collectedFlexItemsValues.remainingFreeSpace /
              numberOfAutoMarginsOnCurrentLine;
        }

        if (performLayout) {
          child->setLayoutPosition(
              childLayout.position[pos[mainAxis]] +
                  collectedFlexItemsValues.mainDim,
              pos[mainAxis]);
        }

        if (child->marginTrailingValue(mainAxis).unit == BNDYGUnitAuto) {
          collectedFlexItemsValues.mainDim +=
              collectedFlexItemsValues.remainingFreeSpace /
              numberOfAutoMarginsOnCurrentLine;
        }
        bool canSkipFlex =
            !performLayout && measureModeCrossDim == BNDYGMeasureModeExactly;
        if (canSkipFlex) {
          // If we skipped the flex step, then we can't rely on the
          // measuredDims because
          // they weren't computed. This means we can't call
          // BNDYGNodeDimWithMargin.
          collectedFlexItemsValues.mainDim += betweenMainDim +
              BNDYGUnwrapFloatOptional(child->getMarginForAxis(
                  mainAxis, availableInnerWidth)) +
              BNDYGUnwrapFloatOptional(childLayout.computedFlexBasis);
          collectedFlexItemsValues.crossDim = availableInnerCrossDim;
        } else {
          // The main dimension is the sum of all the elements dimension plus
          // the spacing.
          collectedFlexItemsValues.mainDim += betweenMainDim +
              BNDYGNodeDimWithMargin(child, mainAxis, availableInnerWidth);

          // The cross dimension is the max of the elements dimension since
          // there can only be one element in that cross dimension.
          collectedFlexItemsValues.crossDim = BNDYGFloatMax(
              collectedFlexItemsValues.crossDim,
              BNDYGNodeDimWithMargin(child, crossAxis, availableInnerWidth));
        }
      } else if (performLayout) {
        child->setLayoutPosition(
            childLayout.position[pos[mainAxis]] +
                node->getLeadingBorder(mainAxis) + leadingMainDim,
            pos[mainAxis]);
      }
    }
  }
  collectedFlexItemsValues.mainDim += BNDYGUnwrapFloatOptional(
      node->getTrailingPaddingAndBorder(mainAxis, ownerWidth));
}

//
// This is the main routine that implements a subset of the flexbox layout
// algorithm
// described in the W3C BNDYG documentation: https://www.w3.org/TR/BNDYG3-flexbox/.
//
// Limitations of this algorithm, compared to the full standard:
//  * Display property is always assumed to be 'flex' except for Text nodes,
//  which
//    are assumed to be 'inline-flex'.
//  * The 'zIndex' property (or any form of z ordering) is not supported. Nodes
//  are
//    stacked in document order.
//  * The 'order' property is not supported. The order of flex items is always
//  defined
//    by document order.
//  * The 'visibility' property is always assumed to be 'visible'. Values of
//  'collapse'
//    and 'hidden' are not supported.
//  * There is no support for forced breaks.
//  * It does not support vertical inline directions (top-to-bottom or
//  bottom-to-top text).
//
// Deviations from standard:
//  * Section 4.5 of the spec indicates that all flex items have a default
//  minimum
//    main size. For text blocks, for example, this is the width of the widest
//    word.
//    Calculating the minimum width is expensive, so we forego it and assume a
//    default
//    minimum main size of 0.
//  * Min/Max sizes in the main axis are not honored when resolving flexible
//  lengths.
//  * The spec indicates that the default value for 'flexDirection' is 'row',
//  but
//    the algorithm below assumes a default of 'column'.
//
// Input parameters:
//    - node: current node to be sized and layed out
//    - availableWidth & availableHeight: available size to be used for sizing
//    the node
//      or BNDYGUndefined if the size is not available; interpretation depends on
//      layout
//      flags
//    - ownerDirection: the inline (text) direction within the owner
//    (left-to-right or
//      right-to-left)
//    - widthMeasureMode: indicates the sizing rules for the width (see below
//    for explanation)
//    - heightMeasureMode: indicates the sizing rules for the height (see below
//    for explanation)
//    - performLayout: specifies whether the caller is interested in just the
//    dimensions
//      of the node or it requires the entire node and its subtree to be layed
//      out
//      (with final positions)
//
// Details:
//    This routine is called recursively to lay out subtrees of flexbox
//    elements. It uses the
//    information in node.style, which is treated as a read-only input. It is
//    responsible for
//    setting the layout.direction and layout.measuredDimensions fields for the
//    input node as well
//    as the layout.position and layout.lineIndex fields for its child nodes.
//    The
//    layout.measuredDimensions field includes any border or padding for the
//    node but does
//    not include margins.
//
//    The spec describes four different layout modes: "fill available", "max
//    content", "min
//    content",
//    and "fit content". Of these, we don't use "min content" because we don't
//    support default
//    minimum main sizes (see above for details). Each of our measure modes maps
//    to a layout mode
//    from the spec (https://www.w3.org/TR/BNDYG3-sizing/#terms):
//      - BNDYGMeasureModeUndefined: max content
//      - BNDYGMeasureModeExactly: fill available
//      - BNDYGMeasureModeAtMost: fit content
//
//    When calling BNDYGNodelayoutImpl and BNDYGLayoutNodeInternal, if the caller passes
//    an available size of
//    undefined then it must also pass a measure mode of BNDYGMeasureModeUndefined
//    in that dimension.
//
static void BNDYGNodelayoutImpl(const BNDYGNodeRef node,
                             const float availableWidth,
                             const float availableHeight,
                             const BNDYGDirection ownerDirection,
                             const BNDYGMeasureMode widthMeasureMode,
                             const BNDYGMeasureMode heightMeasureMode,
                             const float ownerWidth,
                             const float ownerHeight,
                             const bool performLayout,
                             const BNDYGConfigRef config) {
  BNDYGAssertWithNode(node,
                   BNDYGFloatIsUndefined(availableWidth) ? widthMeasureMode == BNDYGMeasureModeUndefined
                                                      : true,
                   "availableWidth is indefinite so widthMeasureMode must be "
                   "BNDYGMeasureModeUndefined");
  BNDYGAssertWithNode(node,
                   BNDYGFloatIsUndefined(availableHeight) ? heightMeasureMode == BNDYGMeasureModeUndefined
                                                       : true,
                   "availableHeight is indefinite so heightMeasureMode must be "
                   "BNDYGMeasureModeUndefined");

  // Set the resolved resolution in the node's layout.
  const BNDYGDirection direction = node->resolveDirection(ownerDirection);
  node->setLayoutDirection(direction);

  const BNDYGFlexDirection flexRowDirection = BNDYGResolveFlexDirection(BNDYGFlexDirectionRow, direction);
  const BNDYGFlexDirection flexColumnDirection =
      BNDYGResolveFlexDirection(BNDYGFlexDirectionColumn, direction);

  node->setLayoutMargin(
      BNDYGUnwrapFloatOptional(
          node->getLeadingMargin(flexRowDirection, ownerWidth)),
      BNDYGEdgeStart);
  node->setLayoutMargin(
      BNDYGUnwrapFloatOptional(
          node->getTrailingMargin(flexRowDirection, ownerWidth)),
      BNDYGEdgeEnd);
  node->setLayoutMargin(
      BNDYGUnwrapFloatOptional(
          node->getLeadingMargin(flexColumnDirection, ownerWidth)),
      BNDYGEdgeTop);
  node->setLayoutMargin(
      BNDYGUnwrapFloatOptional(
          node->getTrailingMargin(flexColumnDirection, ownerWidth)),
      BNDYGEdgeBottom);

  node->setLayoutBorder(node->getLeadingBorder(flexRowDirection), BNDYGEdgeStart);
  node->setLayoutBorder(node->getTrailingBorder(flexRowDirection), BNDYGEdgeEnd);
  node->setLayoutBorder(node->getLeadingBorder(flexColumnDirection), BNDYGEdgeTop);
  node->setLayoutBorder(
      node->getTrailingBorder(flexColumnDirection), BNDYGEdgeBottom);

  node->setLayoutPadding(
      BNDYGUnwrapFloatOptional(
          node->getLeadingPadding(flexRowDirection, ownerWidth)),
      BNDYGEdgeStart);
  node->setLayoutPadding(
      BNDYGUnwrapFloatOptional(
          node->getTrailingPadding(flexRowDirection, ownerWidth)),
      BNDYGEdgeEnd);
  node->setLayoutPadding(
      BNDYGUnwrapFloatOptional(
          node->getLeadingPadding(flexColumnDirection, ownerWidth)),
      BNDYGEdgeTop);
  node->setLayoutPadding(
      BNDYGUnwrapFloatOptional(
          node->getTrailingPadding(flexColumnDirection, ownerWidth)),
      BNDYGEdgeBottom);

  if (node->getMeasure() != nullptr) {
    BNDYGNodeWithMeasureFuncSetMeasuredDimensions(node,
                                               availableWidth,
                                               availableHeight,
                                               widthMeasureMode,
                                               heightMeasureMode,
                                               ownerWidth,
                                               ownerHeight);
    return;
  }

  const uint32_t childCount = BNDYGNodeGetChildCount(node);
  if (childCount == 0) {
    BNDYGNodeEmptyContainerSetMeasuredDimensions(node,
                                              availableWidth,
                                              availableHeight,
                                              widthMeasureMode,
                                              heightMeasureMode,
                                              ownerWidth,
                                              ownerHeight);
    return;
  }

  // If we're not being asked to perform a full layout we can skip the algorithm if we already know
  // the size
  if (!performLayout && BNDYGNodeFixedSizeSetMeasuredDimensions(node,
                                                             availableWidth,
                                                             availableHeight,
                                                             widthMeasureMode,
                                                             heightMeasureMode,
                                                             ownerWidth,
                                                             ownerHeight)) {
    return;
  }

  // At this point we know we're going to perform work. Ensure that each child has a mutable copy.
  node->cloneChildrenIfNeeded();
  // Reset layout flags, as they could have changed.
  node->setLayoutHadOverflow(false);

  // STEP 1: CALCULATE VALUES FOR REMAINDER OF ALGORITHM
  const BNDYGFlexDirection mainAxis =
      BNDYGResolveFlexDirection(node->getStyle().flexDirection, direction);
  const BNDYGFlexDirection crossAxis = BNDYGFlexDirectionCross(mainAxis, direction);
  const bool isMainAxisRow = BNDYGFlexDirectionIsRow(mainAxis);
  const bool isNodeFlexWrap = node->getStyle().flexWrap != BNDYGWrapNoWrap;

  const float mainAxisownerSize = isMainAxisRow ? ownerWidth : ownerHeight;
  const float crossAxisownerSize = isMainAxisRow ? ownerHeight : ownerWidth;

  const float leadingPaddingAndBorderCross = BNDYGUnwrapFloatOptional(
      node->getLeadingPaddingAndBorder(crossAxis, ownerWidth));
  const float paddingAndBorderAxisMain = BNDYGNodePaddingAndBorderForAxis(node, mainAxis, ownerWidth);
  const float paddingAndBorderAxisCross =
      BNDYGNodePaddingAndBorderForAxis(node, crossAxis, ownerWidth);

  BNDYGMeasureMode measureModeMainDim = isMainAxisRow ? widthMeasureMode : heightMeasureMode;
  BNDYGMeasureMode measureModeCrossDim = isMainAxisRow ? heightMeasureMode : widthMeasureMode;

  const float paddingAndBorderAxisRow =
      isMainAxisRow ? paddingAndBorderAxisMain : paddingAndBorderAxisCross;
  const float paddingAndBorderAxisColumn =
      isMainAxisRow ? paddingAndBorderAxisCross : paddingAndBorderAxisMain;

  const float marginAxisRow = BNDYGUnwrapFloatOptional(
      node->getMarginForAxis(BNDYGFlexDirectionRow, ownerWidth));
  const float marginAxisColumn = BNDYGUnwrapFloatOptional(
      node->getMarginForAxis(BNDYGFlexDirectionColumn, ownerWidth));

  const float minInnerWidth =
      BNDYGUnwrapFloatOptional(BNDYGResolveValue(node->getStyle().minDimensions[BNDYGDimensionWidth], ownerWidth)) -
      paddingAndBorderAxisRow;
  const float maxInnerWidth =
      BNDYGUnwrapFloatOptional(BNDYGResolveValue(node->getStyle().maxDimensions[BNDYGDimensionWidth], ownerWidth)) -
      paddingAndBorderAxisRow;
  const float minInnerHeight =
      BNDYGUnwrapFloatOptional(BNDYGResolveValue(node->getStyle().minDimensions[BNDYGDimensionHeight], ownerHeight)) -
      paddingAndBorderAxisColumn;
  const float maxInnerHeight =
      BNDYGUnwrapFloatOptional(BNDYGResolveValue(
          node->getStyle().maxDimensions[BNDYGDimensionHeight], ownerHeight)) -
      paddingAndBorderAxisColumn;

  const float minInnerMainDim = isMainAxisRow ? minInnerWidth : minInnerHeight;
  const float maxInnerMainDim = isMainAxisRow ? maxInnerWidth : maxInnerHeight;

  // STEP 2: DETERMINE AVAILABLE SIZE IN MAIN AND CROSS DIRECTIONS

  float availableInnerWidth = BNDYGNodeCalculateAvailableInnerDim(
      node, BNDYGFlexDirectionRow, availableWidth, ownerWidth);
  float availableInnerHeight = BNDYGNodeCalculateAvailableInnerDim(
      node, BNDYGFlexDirectionColumn, availableHeight, ownerHeight);

  float availableInnerMainDim =
      isMainAxisRow ? availableInnerWidth : availableInnerHeight;
  const float availableInnerCrossDim =
      isMainAxisRow ? availableInnerHeight : availableInnerWidth;

  float totalOuterFlexBasis = 0;

  // STEP 3: DETERMINE FLEX BASIS FOR EACH ITEM

  BNDYGNodeComputeFlexBasisForChildren(
      node,
      availableInnerWidth,
      availableInnerHeight,
      widthMeasureMode,
      heightMeasureMode,
      direction,
      mainAxis,
      config,
      performLayout,
      totalOuterFlexBasis);

  const bool flexBasisOverflows = measureModeMainDim == BNDYGMeasureModeUndefined
      ? false
      : totalOuterFlexBasis > availableInnerMainDim;
  if (isNodeFlexWrap && flexBasisOverflows &&
      measureModeMainDim == BNDYGMeasureModeAtMost) {
    measureModeMainDim = BNDYGMeasureModeExactly;
  }
  // STEP 4: COLLECT FLEX ITEMS INTO FLEX LINES

  // Indexes of children that represent the first and last items in the line.
  uint32_t startOfLineIndex = 0;
  uint32_t endOfLineIndex = 0;

  // Number of lines.
  uint32_t lineCount = 0;

  // Accumulated cross dimensions of all lines so far.
  float totalLineCrossDim = 0;

  // Max main dimension of all the lines.
  float maxLineMainDim = 0;
  BNDYGCollectFlexItemsRowValues collectedFlexItemsValues;
  for (; endOfLineIndex < childCount;
       lineCount++, startOfLineIndex = endOfLineIndex) {
    collectedFlexItemsValues = BNDYGCalculateCollectFlexItemsRowValues(
        node,
        ownerDirection,
        mainAxisownerSize,
        availableInnerWidth,
        availableInnerMainDim,
        startOfLineIndex,
        lineCount);
    endOfLineIndex = collectedFlexItemsValues.endOfLineIndex;

    // If we don't need to measure the cross axis, we can skip the entire flex
    // step.
    const bool canSkipFlex =
        !performLayout && measureModeCrossDim == BNDYGMeasureModeExactly;

    // STEP 5: RESOLVING FLEXIBLE LENGTHS ON MAIN AXIS
    // Calculate the remaining available space that needs to be allocated.
    // If the main dimension size isn't known, it is computed based on
    // the line length, so there's no more space left to distribute.

    bool sizeBasedOnContent = false;
    // If we don't measure with exact main dimension we want to ensure we don't violate min and max
    if (measureModeMainDim != BNDYGMeasureModeExactly) {
      if (!BNDYGFloatIsUndefined(minInnerMainDim) &&
          collectedFlexItemsValues.sizeConsumedOnCurrentLine <
              minInnerMainDim) {
        availableInnerMainDim = minInnerMainDim;
      } else if (
          !BNDYGFloatIsUndefined(maxInnerMainDim) &&
          collectedFlexItemsValues.sizeConsumedOnCurrentLine >
              maxInnerMainDim) {
        availableInnerMainDim = maxInnerMainDim;
      } else {
        if (!node->getConfig()->useLegacyStretchBehaviour &&
            ((BNDYGFloatIsUndefined(
                  collectedFlexItemsValues.totalFlexGrowFactors) &&
              collectedFlexItemsValues.totalFlexGrowFactors == 0) ||
             (BNDYGFloatIsUndefined(node->resolveFlexGrow()) &&
              node->resolveFlexGrow() == 0))) {
          // If we don't have any children to flex or we can't flex the node
          // itself, space we've used is all space we need. Root node also
          // should be shrunk to minimum
          availableInnerMainDim =
              collectedFlexItemsValues.sizeConsumedOnCurrentLine;
        }

        if (node->getConfig()->useLegacyStretchBehaviour) {
          node->setLayoutDidUseLegacyFlag(true);
        }
        sizeBasedOnContent = !node->getConfig()->useLegacyStretchBehaviour;
      }
    }

    if (!sizeBasedOnContent && !BNDYGFloatIsUndefined(availableInnerMainDim)) {
      collectedFlexItemsValues.remainingFreeSpace = availableInnerMainDim -
          collectedFlexItemsValues.sizeConsumedOnCurrentLine;
    } else if (collectedFlexItemsValues.sizeConsumedOnCurrentLine < 0) {
      // availableInnerMainDim is indefinite which means the node is being sized based on its
      // content.
      // sizeConsumedOnCurrentLine is negative which means the node will allocate 0 points for
      // its content. Consequently, remainingFreeSpace is 0 - sizeConsumedOnCurrentLine.
      collectedFlexItemsValues.remainingFreeSpace =
          -collectedFlexItemsValues.sizeConsumedOnCurrentLine;
    }

    if (!canSkipFlex) {
      BNDYGResolveFlexibleLength(
          node,
          collectedFlexItemsValues,
          mainAxis,
          crossAxis,
          mainAxisownerSize,
          availableInnerMainDim,
          availableInnerCrossDim,
          availableInnerWidth,
          availableInnerHeight,
          flexBasisOverflows,
          measureModeCrossDim,
          performLayout,
          config);
    }

    node->setLayoutHadOverflow(
        node->getLayout().hadOverflow |
        (collectedFlexItemsValues.remainingFreeSpace < 0));

    // STEP 6: MAIN-AXIS JUSTIFICATION & CROSS-AXIS SIZE DETERMINATION

    // At this point, all the children have their dimensions set in the main
    // axis.
    // Their dimensions are also set in the cross axis with the exception of
    // items
    // that are aligned "stretch". We need to compute these stretch values and
    // set the final positions.

    BNDYGJustifyMainAxis(
        node,
        collectedFlexItemsValues,
        startOfLineIndex,
        mainAxis,
        crossAxis,
        measureModeMainDim,
        measureModeCrossDim,
        mainAxisownerSize,
        ownerWidth,
        availableInnerMainDim,
        availableInnerCrossDim,
        availableInnerWidth,
        performLayout);

    float containerCrossAxis = availableInnerCrossDim;
    if (measureModeCrossDim == BNDYGMeasureModeUndefined ||
        measureModeCrossDim == BNDYGMeasureModeAtMost) {
      // Compute the cross axis from the max cross dimension of the children.
      containerCrossAxis =
          BNDYGNodeBoundAxis(
              node,
              crossAxis,
              collectedFlexItemsValues.crossDim + paddingAndBorderAxisCross,
              crossAxisownerSize,
              ownerWidth) -
          paddingAndBorderAxisCross;
    }

    // If there's no flex wrap, the cross dimension is defined by the container.
    if (!isNodeFlexWrap && measureModeCrossDim == BNDYGMeasureModeExactly) {
      collectedFlexItemsValues.crossDim = availableInnerCrossDim;
    }

    // Clamp to the min/max size specified on the container.
    collectedFlexItemsValues.crossDim =
        BNDYGNodeBoundAxis(
            node,
            crossAxis,
            collectedFlexItemsValues.crossDim + paddingAndBorderAxisCross,
            crossAxisownerSize,
            ownerWidth) -
        paddingAndBorderAxisCross;

    // STEP 7: CROSS-AXIS ALIGNMENT
    // We can skip child alignment if we're just measuring the container.
    if (performLayout) {
      for (uint32_t i = startOfLineIndex; i < endOfLineIndex; i++) {
        const BNDYGNodeRef child = node->getChild(i);
        if (child->getStyle().display == BNDYGDisplayNone) {
          continue;
        }
        if (child->getStyle().positionType == BNDYGPositionTypeAbsolute) {
          // If the child is absolutely positioned and has a
          // top/left/bottom/right set, override
          // all the previously computed positions to set it correctly.
          const bool isChildLeadingPosDefined =
              child->isLeadingPositionDefined(crossAxis);
          if (isChildLeadingPosDefined) {
            child->setLayoutPosition(
                BNDYGUnwrapFloatOptional(child->getLeadingPosition(
                    crossAxis, availableInnerCrossDim)) +
                    node->getLeadingBorder(crossAxis) +
                    BNDYGUnwrapFloatOptional(child->getLeadingMargin(
                        crossAxis, availableInnerWidth)),
                pos[crossAxis]);
          }
          // If leading position is not defined or calculations result in Nan, default to border + margin
          if (!isChildLeadingPosDefined ||
              BNDYGFloatIsUndefined(child->getLayout().position[pos[crossAxis]])) {
            child->setLayoutPosition(
                node->getLeadingBorder(crossAxis) +
                    BNDYGUnwrapFloatOptional(child->getLeadingMargin(
                        crossAxis, availableInnerWidth)),
                pos[crossAxis]);
          }
        } else {
          float leadingCrossDim = leadingPaddingAndBorderCross;

          // For a relative children, we're either using alignItems (owner) or
          // alignSelf (child) in order to determine the position in the cross
          // axis
          const BNDYGAlign alignItem = BNDYGNodeAlignItem(node, child);

          // If the child uses align stretch, we need to lay it out one more
          // time, this time
          // forcing the cross-axis size to be the computed cross size for the
          // current line.
          if (alignItem == BNDYGAlignStretch &&
              child->marginLeadingValue(crossAxis).unit != BNDYGUnitAuto &&
              child->marginTrailingValue(crossAxis).unit != BNDYGUnitAuto) {
            // If the child defines a definite size for its cross axis, there's
            // no need to stretch.
            if (!BNDYGNodeIsStyleDimDefined(child, crossAxis, availableInnerCrossDim)) {
              float childMainSize =
                  child->getLayout().measuredDimensions[dim[mainAxis]];
              float childCrossSize =
                  !child->getStyle().aspectRatio.isUndefined()
                  ? ((BNDYGUnwrapFloatOptional(child->getMarginForAxis(
                          crossAxis, availableInnerWidth)) +
                      (isMainAxisRow ? childMainSize /
                               child->getStyle().aspectRatio.getValue()
                                     : childMainSize *
                               child->getStyle().aspectRatio.getValue())))
                  : collectedFlexItemsValues.crossDim;

              childMainSize += BNDYGUnwrapFloatOptional(
                  child->getMarginForAxis(mainAxis, availableInnerWidth));

              BNDYGMeasureMode childMainMeasureMode = BNDYGMeasureModeExactly;
              BNDYGMeasureMode childCrossMeasureMode = BNDYGMeasureModeExactly;
              BNDYGConstrainMaxSizeForMode(child,
                                        mainAxis,
                                        availableInnerMainDim,
                                        availableInnerWidth,
                                        &childMainMeasureMode,
                                        &childMainSize);
              BNDYGConstrainMaxSizeForMode(child,
                                        crossAxis,
                                        availableInnerCrossDim,
                                        availableInnerWidth,
                                        &childCrossMeasureMode,
                                        &childCrossSize);

              const float childWidth = isMainAxisRow ? childMainSize : childCrossSize;
              const float childHeight = !isMainAxisRow ? childMainSize : childCrossSize;

              const BNDYGMeasureMode childWidthMeasureMode =
                  BNDYGFloatIsUndefined(childWidth) ? BNDYGMeasureModeUndefined
                                                 : BNDYGMeasureModeExactly;
              const BNDYGMeasureMode childHeightMeasureMode =
                  BNDYGFloatIsUndefined(childHeight) ? BNDYGMeasureModeUndefined
                                                  : BNDYGMeasureModeExactly;

              BNDYGLayoutNodeInternal(
                  child,
                  childWidth,
                  childHeight,
                  direction,
                  childWidthMeasureMode,
                  childHeightMeasureMode,
                  availableInnerWidth,
                  availableInnerHeight,
                  true,
                  "stretch",
                  config);
            }
          } else {
            const float remainingCrossDim = containerCrossAxis -
                BNDYGNodeDimWithMargin(child, crossAxis, availableInnerWidth);

            if (child->marginLeadingValue(crossAxis).unit == BNDYGUnitAuto &&
                child->marginTrailingValue(crossAxis).unit == BNDYGUnitAuto) {
              leadingCrossDim += BNDYGFloatMax(0.0f, remainingCrossDim / 2);
            } else if (
                child->marginTrailingValue(crossAxis).unit == BNDYGUnitAuto) {
              // No-Op
            } else if (
                child->marginLeadingValue(crossAxis).unit == BNDYGUnitAuto) {
              leadingCrossDim += BNDYGFloatMax(0.0f, remainingCrossDim);
            } else if (alignItem == BNDYGAlignFlexStart) {
              // No-Op
            } else if (alignItem == BNDYGAlignCenter) {
              leadingCrossDim += remainingCrossDim / 2;
            } else {
              leadingCrossDim += remainingCrossDim;
            }
          }
          // And we apply the position
          child->setLayoutPosition(
              child->getLayout().position[pos[crossAxis]] + totalLineCrossDim +
                  leadingCrossDim,
              pos[crossAxis]);
        }
      }
    }

    totalLineCrossDim += collectedFlexItemsValues.crossDim;
    maxLineMainDim =
        BNDYGFloatMax(maxLineMainDim, collectedFlexItemsValues.mainDim);
  }

  // STEP 8: MULTI-LINE CONTENT ALIGNMENT
  if (performLayout && (lineCount > 1 || BNDYGIsBaselineLayout(node)) &&
      !BNDYGFloatIsUndefined(availableInnerCrossDim)) {
    const float remainingAlignContentDim = availableInnerCrossDim - totalLineCrossDim;

    float crossDimLead = 0;
    float currentLead = leadingPaddingAndBorderCross;

    switch (node->getStyle().alignContent) {
      case BNDYGAlignFlexEnd:
        currentLead += remainingAlignContentDim;
        break;
      case BNDYGAlignCenter:
        currentLead += remainingAlignContentDim / 2;
        break;
      case BNDYGAlignStretch:
        if (availableInnerCrossDim > totalLineCrossDim) {
          crossDimLead = remainingAlignContentDim / lineCount;
        }
        break;
      case BNDYGAlignSpaceAround:
        if (availableInnerCrossDim > totalLineCrossDim) {
          currentLead += remainingAlignContentDim / (2 * lineCount);
          if (lineCount > 1) {
            crossDimLead = remainingAlignContentDim / lineCount;
          }
        } else {
          currentLead += remainingAlignContentDim / 2;
        }
        break;
      case BNDYGAlignSpaceBetween:
        if (availableInnerCrossDim > totalLineCrossDim && lineCount > 1) {
          crossDimLead = remainingAlignContentDim / (lineCount - 1);
        }
        break;
      case BNDYGAlignAuto:
      case BNDYGAlignFlexStart:
      case BNDYGAlignBaseline:
        break;
    }

    uint32_t endIndex = 0;
    for (uint32_t i = 0; i < lineCount; i++) {
      const uint32_t startIndex = endIndex;
      uint32_t ii;

      // compute the line's height and find the endIndex
      float lineHeight = 0;
      float maxAscentForCurrentLine = 0;
      float maxDescentForCurrentLine = 0;
      for (ii = startIndex; ii < childCount; ii++) {
        const BNDYGNodeRef child = node->getChild(ii);
        if (child->getStyle().display == BNDYGDisplayNone) {
          continue;
        }
        if (child->getStyle().positionType == BNDYGPositionTypeRelative) {
          if (child->getLineIndex() != i) {
            break;
          }
          if (BNDYGNodeIsLayoutDimDefined(child, crossAxis)) {
            lineHeight = BNDYGFloatMax(
                lineHeight,
                child->getLayout().measuredDimensions[dim[crossAxis]] +
                    BNDYGUnwrapFloatOptional(child->getMarginForAxis(
                        crossAxis, availableInnerWidth)));
          }
          if (BNDYGNodeAlignItem(node, child) == BNDYGAlignBaseline) {
            const float ascent = BNDYGBaseline(child) +
                BNDYGUnwrapFloatOptional(child->getLeadingMargin(
                    BNDYGFlexDirectionColumn, availableInnerWidth));
            const float descent =
                child->getLayout().measuredDimensions[BNDYGDimensionHeight] +
                BNDYGUnwrapFloatOptional(child->getMarginForAxis(
                    BNDYGFlexDirectionColumn, availableInnerWidth)) -
                ascent;
            maxAscentForCurrentLine =
                BNDYGFloatMax(maxAscentForCurrentLine, ascent);
            maxDescentForCurrentLine =
                BNDYGFloatMax(maxDescentForCurrentLine, descent);
            lineHeight = BNDYGFloatMax(
                lineHeight, maxAscentForCurrentLine + maxDescentForCurrentLine);
          }
        }
      }
      endIndex = ii;
      lineHeight += crossDimLead;

      if (performLayout) {
        for (ii = startIndex; ii < endIndex; ii++) {
          const BNDYGNodeRef child = node->getChild(ii);
          if (child->getStyle().display == BNDYGDisplayNone) {
            continue;
          }
          if (child->getStyle().positionType == BNDYGPositionTypeRelative) {
            switch (BNDYGNodeAlignItem(node, child)) {
              case BNDYGAlignFlexStart: {
                child->setLayoutPosition(
                    currentLead +
                        BNDYGUnwrapFloatOptional(child->getLeadingMargin(
                            crossAxis, availableInnerWidth)),
                    pos[crossAxis]);
                break;
              }
              case BNDYGAlignFlexEnd: {
                child->setLayoutPosition(
                    currentLead + lineHeight -
                        BNDYGUnwrapFloatOptional(child->getTrailingMargin(
                            crossAxis, availableInnerWidth)) -
                        child->getLayout().measuredDimensions[dim[crossAxis]],
                    pos[crossAxis]);
                break;
              }
              case BNDYGAlignCenter: {
                float childHeight =
                    child->getLayout().measuredDimensions[dim[crossAxis]];

                child->setLayoutPosition(
                    currentLead + (lineHeight - childHeight) / 2,
                    pos[crossAxis]);
                break;
              }
              case BNDYGAlignStretch: {
                child->setLayoutPosition(
                    currentLead +
                        BNDYGUnwrapFloatOptional(child->getLeadingMargin(
                            crossAxis, availableInnerWidth)),
                    pos[crossAxis]);

                // Remeasure child with the line height as it as been only measured with the
                // owners height yet.
                if (!BNDYGNodeIsStyleDimDefined(child, crossAxis, availableInnerCrossDim)) {
                  const float childWidth = isMainAxisRow
                      ? (child->getLayout()
                             .measuredDimensions[BNDYGDimensionWidth] +
                         BNDYGUnwrapFloatOptional(child->getMarginForAxis(
                             mainAxis, availableInnerWidth)))
                      : lineHeight;

                  const float childHeight = !isMainAxisRow
                      ? (child->getLayout()
                             .measuredDimensions[BNDYGDimensionHeight] +
                         BNDYGUnwrapFloatOptional(child->getMarginForAxis(
                             crossAxis, availableInnerWidth)))
                      : lineHeight;

                  if (!(BNDYGFloatsEqual(
                            childWidth,
                            child->getLayout()
                                .measuredDimensions[BNDYGDimensionWidth]) &&
                        BNDYGFloatsEqual(
                            childHeight,
                            child->getLayout()
                                .measuredDimensions[BNDYGDimensionHeight]))) {
                    BNDYGLayoutNodeInternal(child,
                                         childWidth,
                                         childHeight,
                                         direction,
                                         BNDYGMeasureModeExactly,
                                         BNDYGMeasureModeExactly,
                                         availableInnerWidth,
                                         availableInnerHeight,
                                         true,
                                         "multiline-stretch",
                                         config);
                  }
                }
                break;
              }
              case BNDYGAlignBaseline: {
                child->setLayoutPosition(
                    currentLead + maxAscentForCurrentLine - BNDYGBaseline(child) +
                        BNDYGUnwrapFloatOptional(child->getLeadingPosition(
                            BNDYGFlexDirectionColumn, availableInnerCrossDim)),
                    BNDYGEdgeTop);

                break;
              }
              case BNDYGAlignAuto:
              case BNDYGAlignSpaceBetween:
              case BNDYGAlignSpaceAround:
                break;
            }
          }
        }
      }

      currentLead += lineHeight;
    }
  }

  // STEP 9: COMPUTING FINAL DIMENSIONS

  node->setLayoutMeasuredDimension(
      BNDYGNodeBoundAxis(
          node,
          BNDYGFlexDirectionRow,
          availableWidth - marginAxisRow,
          ownerWidth,
          ownerWidth),
      BNDYGDimensionWidth);

  node->setLayoutMeasuredDimension(
      BNDYGNodeBoundAxis(
          node,
          BNDYGFlexDirectionColumn,
          availableHeight - marginAxisColumn,
          ownerHeight,
          ownerWidth),
      BNDYGDimensionHeight);

  // If the user didn't specify a width or height for the node, set the
  // dimensions based on the children.
  if (measureModeMainDim == BNDYGMeasureModeUndefined ||
      (node->getStyle().overflow != BNDYGOverflowScroll &&
       measureModeMainDim == BNDYGMeasureModeAtMost)) {
    // Clamp the size to the min/max size, if specified, and make sure it
    // doesn't go below the padding and border amount.
    node->setLayoutMeasuredDimension(
        BNDYGNodeBoundAxis(
            node, mainAxis, maxLineMainDim, mainAxisownerSize, ownerWidth),
        dim[mainAxis]);

  } else if (
      measureModeMainDim == BNDYGMeasureModeAtMost &&
      node->getStyle().overflow == BNDYGOverflowScroll) {
    node->setLayoutMeasuredDimension(
        BNDYGFloatMax(
            BNDYGFloatMin(
                availableInnerMainDim + paddingAndBorderAxisMain,
                BNDYGUnwrapFloatOptional(BNDYGNodeBoundAxisWithinMinAndMax(
                    node, mainAxis, maxLineMainDim, mainAxisownerSize))),
            paddingAndBorderAxisMain),
        dim[mainAxis]);
  }

  if (measureModeCrossDim == BNDYGMeasureModeUndefined ||
      (node->getStyle().overflow != BNDYGOverflowScroll &&
       measureModeCrossDim == BNDYGMeasureModeAtMost)) {
    // Clamp the size to the min/max size, if specified, and make sure it
    // doesn't go below the padding and border amount.

    node->setLayoutMeasuredDimension(
        BNDYGNodeBoundAxis(
            node,
            crossAxis,
            totalLineCrossDim + paddingAndBorderAxisCross,
            crossAxisownerSize,
            ownerWidth),
        dim[crossAxis]);

  } else if (
      measureModeCrossDim == BNDYGMeasureModeAtMost &&
      node->getStyle().overflow == BNDYGOverflowScroll) {
    node->setLayoutMeasuredDimension(
        BNDYGFloatMax(
            BNDYGFloatMin(
                availableInnerCrossDim + paddingAndBorderAxisCross,
                BNDYGUnwrapFloatOptional(BNDYGNodeBoundAxisWithinMinAndMax(
                    node,
                    crossAxis,
                    totalLineCrossDim + paddingAndBorderAxisCross,
                    crossAxisownerSize))),
            paddingAndBorderAxisCross),
        dim[crossAxis]);
  }

  // As we only wrapped in normal direction yet, we need to reverse the positions on wrap-reverse.
  if (performLayout && node->getStyle().flexWrap == BNDYGWrapWrapReverse) {
    for (uint32_t i = 0; i < childCount; i++) {
      const BNDYGNodeRef child = BNDYGNodeGetChild(node, i);
      if (child->getStyle().positionType == BNDYGPositionTypeRelative) {
        child->setLayoutPosition(
            node->getLayout().measuredDimensions[dim[crossAxis]] -
                child->getLayout().position[pos[crossAxis]] -
                child->getLayout().measuredDimensions[dim[crossAxis]],
            pos[crossAxis]);
      }
    }
  }

  if (performLayout) {
    // STEP 10: SIZING AND POSITIONING ABSOLUTE CHILDREN
    for (auto child : node->getChildren()) {
      if (child->getStyle().positionType != BNDYGPositionTypeAbsolute) {
        continue;
      }
      BNDYGNodeAbsoluteLayoutChild(
          node,
          child,
          availableInnerWidth,
          isMainAxisRow ? measureModeMainDim : measureModeCrossDim,
          availableInnerHeight,
          direction,
          config);
    }

    // STEP 11: SETTING TRAILING POSITIONS FOR CHILDREN
    const bool needsMainTrailingPos =
        mainAxis == BNDYGFlexDirectionRowReverse || mainAxis == BNDYGFlexDirectionColumnReverse;
    const bool needsCrossTrailingPos =
        crossAxis == BNDYGFlexDirectionRowReverse || crossAxis == BNDYGFlexDirectionColumnReverse;

    // Set trailing position if necessary.
    if (needsMainTrailingPos || needsCrossTrailingPos) {
      for (uint32_t i = 0; i < childCount; i++) {
        const BNDYGNodeRef child = node->getChild(i);
        if (child->getStyle().display == BNDYGDisplayNone) {
          continue;
        }
        if (needsMainTrailingPos) {
          BNDYGNodeSetChildTrailingPosition(node, child, mainAxis);
        }

        if (needsCrossTrailingPos) {
          BNDYGNodeSetChildTrailingPosition(node, child, crossAxis);
        }
      }
    }
  }
}

uint32_t gBNDDepth = 0;
bool gPrintTree = false;
bool gBNDPrintChanges = false;
bool gBNDPrintSkips = false;

static const char *spacer = "                                                            ";

static const char *BNDYGSpacer(const unsigned long level) {
  const size_t spacerLen = strlen(spacer);
  if (level > spacerLen) {
    return &spacer[0];
  } else {
    return &spacer[spacerLen - level];
  }
}

static const char *BNDYGMeasureModeName(const BNDYGMeasureMode mode, const bool performLayout) {
  const char *kMeasureModeNames[BNDYGMeasureModeCount] = {"UNDEFINED", "EXACTLY", "AT_MOST"};
  const char *kLayoutModeNames[BNDYGMeasureModeCount] = {"LAY_UNDEFINED",
                                                      "LAY_EXACTLY",
                                                      "LAY_AT_"
                                                      "MOST"};

  if (mode >= BNDYGMeasureModeCount) {
    return "";
  }

  return performLayout ? kLayoutModeNames[mode] : kMeasureModeNames[mode];
}

static inline bool BNDYGMeasureModeSizeIsExactAndMatchesOldMeasuredSize(BNDYGMeasureMode sizeMode,
                                                                     float size,
                                                                     float lastComputedSize) {
  return sizeMode == BNDYGMeasureModeExactly && BNDYGFloatsEqual(size, lastComputedSize);
}

static inline bool BNDYGMeasureModeOldSizeIsUnspecifiedAndStillFits(BNDYGMeasureMode sizeMode,
                                                                 float size,
                                                                 BNDYGMeasureMode lastSizeMode,
                                                                 float lastComputedSize) {
  return sizeMode == BNDYGMeasureModeAtMost && lastSizeMode == BNDYGMeasureModeUndefined &&
         (size >= lastComputedSize || BNDYGFloatsEqual(size, lastComputedSize));
}

static inline bool BNDYGMeasureModeNewMeasureSizeIsStricterAndStillValid(BNDYGMeasureMode sizeMode,
                                                                      float size,
                                                                      BNDYGMeasureMode lastSizeMode,
                                                                      float lastSize,
                                                                      float lastComputedSize) {
  return lastSizeMode == BNDYGMeasureModeAtMost &&
      sizeMode == BNDYGMeasureModeAtMost && !BNDYGFloatIsUndefined(lastSize) &&
      !BNDYGFloatIsUndefined(size) && !BNDYGFloatIsUndefined(lastComputedSize) &&
      lastSize > size &&
      (lastComputedSize <= size || BNDYGFloatsEqual(size, lastComputedSize));
}

float BNDYGRoundValueToPixelGrid(const float value,
                              const float pointScaleFactor,
                              const bool forceCeil,
                              const bool forceFloor) {
  float scaledValue = value * pointScaleFactor;
  float fractial = fmodf(scaledValue, 1.0f);
  if (BNDYGFloatsEqual(fractial, 0)) {
    // First we check if the value is already rounded
    scaledValue = scaledValue - fractial;
  } else if (BNDYGFloatsEqual(fractial, 1.0f)) {
    scaledValue = scaledValue - fractial + 1.0f;
  } else if (forceCeil) {
    // Next we check if we need to use forced rounding
    scaledValue = scaledValue - fractial + 1.0f;
  } else if (forceFloor) {
    scaledValue = scaledValue - fractial;
  } else {
    // Finally we just round the value
    scaledValue = scaledValue - fractial +
        (!BNDYGFloatIsUndefined(fractial) &&
                 (fractial > 0.5f || BNDYGFloatsEqual(fractial, 0.5f))
             ? 1.0f
             : 0.0f);
  }
  return (BNDYGFloatIsUndefined(scaledValue) ||
          BNDYGFloatIsUndefined(pointScaleFactor))
      ? BNDYGUndefined
      : scaledValue / pointScaleFactor;
}

bool BNDYGNodeCanUseCachedMeasurement(const BNDYGMeasureMode widthMode,
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
                                   const BNDYGConfigRef config) {
  if ((!BNDYGFloatIsUndefined(lastComputedHeight) && lastComputedHeight < 0) ||
      (!BNDYGFloatIsUndefined(lastComputedWidth) && lastComputedWidth < 0)) {
    return false;
  }
  bool useRoundedComparison =
      config != nullptr && config->pointScaleFactor != 0;
  const float effectiveWidth =
      useRoundedComparison ? BNDYGRoundValueToPixelGrid(width, config->pointScaleFactor, false, false)
                           : width;
  const float effectiveHeight =
      useRoundedComparison ? BNDYGRoundValueToPixelGrid(height, config->pointScaleFactor, false, false)
                           : height;
  const float effectiveLastWidth =
      useRoundedComparison
          ? BNDYGRoundValueToPixelGrid(lastWidth, config->pointScaleFactor, false, false)
          : lastWidth;
  const float effectiveLastHeight =
      useRoundedComparison
          ? BNDYGRoundValueToPixelGrid(lastHeight, config->pointScaleFactor, false, false)
          : lastHeight;

  const bool hasSameWidthSpec =
      lastWidthMode == widthMode && BNDYGFloatsEqual(effectiveLastWidth, effectiveWidth);
  const bool hasSameHeightSpec =
      lastHeightMode == heightMode && BNDYGFloatsEqual(effectiveLastHeight, effectiveHeight);

  const bool widthIsCompatible =
      hasSameWidthSpec || BNDYGMeasureModeSizeIsExactAndMatchesOldMeasuredSize(widthMode,
                                                                            width - marginRow,
                                                                            lastComputedWidth) ||
      BNDYGMeasureModeOldSizeIsUnspecifiedAndStillFits(widthMode,
                                                    width - marginRow,
                                                    lastWidthMode,
                                                    lastComputedWidth) ||
      BNDYGMeasureModeNewMeasureSizeIsStricterAndStillValid(
          widthMode, width - marginRow, lastWidthMode, lastWidth, lastComputedWidth);

  const bool heightIsCompatible =
      hasSameHeightSpec || BNDYGMeasureModeSizeIsExactAndMatchesOldMeasuredSize(heightMode,
                                                                             height - marginColumn,
                                                                             lastComputedHeight) ||
      BNDYGMeasureModeOldSizeIsUnspecifiedAndStillFits(heightMode,
                                                    height - marginColumn,
                                                    lastHeightMode,
                                                    lastComputedHeight) ||
      BNDYGMeasureModeNewMeasureSizeIsStricterAndStillValid(
          heightMode, height - marginColumn, lastHeightMode, lastHeight, lastComputedHeight);

  return widthIsCompatible && heightIsCompatible;
}

//
// This is a wrapper around the BNDYGNodelayoutImpl function. It determines
// whether the layout request is redundant and can be skipped.
//
// Parameters:
//  Input parameters are the same as BNDYGNodelayoutImpl (see above)
//  Return parameter is true if layout was performed, false if skipped
//
bool BNDYGLayoutNodeInternal(const BNDYGNodeRef node,
                          const float availableWidth,
                          const float availableHeight,
                          const BNDYGDirection ownerDirection,
                          const BNDYGMeasureMode widthMeasureMode,
                          const BNDYGMeasureMode heightMeasureMode,
                          const float ownerWidth,
                          const float ownerHeight,
                          const bool performLayout,
                          const char *reason,
                          const BNDYGConfigRef config) {
  BNDYGLayout* layout = &node->getLayout();

  gBNDDepth++;

  const bool needToVisitNode =
      (node->isDirty() && layout->generationCount != gBNDCurrentGenerationCount) ||
      layout->lastOwnerDirection != ownerDirection;

  if (needToVisitNode) {
    // Invalidate the cached results.
    layout->nextCachedMeasurementsIndex = 0;
    layout->cachedLayout.widthMeasureMode = (BNDYGMeasureMode) -1;
    layout->cachedLayout.heightMeasureMode = (BNDYGMeasureMode) -1;
    layout->cachedLayout.computedWidth = -1;
    layout->cachedLayout.computedHeight = -1;
  }

  BNDYGCachedMeasurement* cachedResults = nullptr;

  // Determine whether the results are already cached. We maintain a separate
  // cache for layouts and measurements. A layout operation modifies the
  // positions
  // and dimensions for nodes in the subtree. The algorithm assumes that each
  // node
  // gets layed out a maximum of one time per tree layout, but multiple
  // measurements
  // may be required to resolve all of the flex dimensions.
  // We handle nodes with measure functions specially here because they are the
  // most
  // expensive to measure, so it's worth avoiding redundant measurements if at
  // all possible.
  if (node->getMeasure() != nullptr) {
    const float marginAxisRow = BNDYGUnwrapFloatOptional(
        node->getMarginForAxis(BNDYGFlexDirectionRow, ownerWidth));
    const float marginAxisColumn = BNDYGUnwrapFloatOptional(
        node->getMarginForAxis(BNDYGFlexDirectionColumn, ownerWidth));

    // First, try to use the layout cache.
    if (BNDYGNodeCanUseCachedMeasurement(widthMeasureMode,
                                      availableWidth,
                                      heightMeasureMode,
                                      availableHeight,
                                      layout->cachedLayout.widthMeasureMode,
                                      layout->cachedLayout.availableWidth,
                                      layout->cachedLayout.heightMeasureMode,
                                      layout->cachedLayout.availableHeight,
                                      layout->cachedLayout.computedWidth,
                                      layout->cachedLayout.computedHeight,
                                      marginAxisRow,
                                      marginAxisColumn,
                                      config)) {
      cachedResults = &layout->cachedLayout;
    } else {
      // Try to use the measurement cache.
      for (uint32_t i = 0; i < layout->nextCachedMeasurementsIndex; i++) {
        if (BNDYGNodeCanUseCachedMeasurement(widthMeasureMode,
                                          availableWidth,
                                          heightMeasureMode,
                                          availableHeight,
                                          layout->cachedMeasurements[i].widthMeasureMode,
                                          layout->cachedMeasurements[i].availableWidth,
                                          layout->cachedMeasurements[i].heightMeasureMode,
                                          layout->cachedMeasurements[i].availableHeight,
                                          layout->cachedMeasurements[i].computedWidth,
                                          layout->cachedMeasurements[i].computedHeight,
                                          marginAxisRow,
                                          marginAxisColumn,
                                          config)) {
          cachedResults = &layout->cachedMeasurements[i];
          break;
        }
      }
    }
  } else if (performLayout) {
    if (BNDYGFloatsEqual(layout->cachedLayout.availableWidth, availableWidth) &&
        BNDYGFloatsEqual(layout->cachedLayout.availableHeight, availableHeight) &&
        layout->cachedLayout.widthMeasureMode == widthMeasureMode &&
        layout->cachedLayout.heightMeasureMode == heightMeasureMode) {
      cachedResults = &layout->cachedLayout;
    }
  } else {
    for (uint32_t i = 0; i < layout->nextCachedMeasurementsIndex; i++) {
      if (BNDYGFloatsEqual(layout->cachedMeasurements[i].availableWidth, availableWidth) &&
          BNDYGFloatsEqual(layout->cachedMeasurements[i].availableHeight, availableHeight) &&
          layout->cachedMeasurements[i].widthMeasureMode == widthMeasureMode &&
          layout->cachedMeasurements[i].heightMeasureMode == heightMeasureMode) {
        cachedResults = &layout->cachedMeasurements[i];
        break;
      }
    }
  }

  if (!needToVisitNode && cachedResults != nullptr) {
    layout->measuredDimensions[BNDYGDimensionWidth] = cachedResults->computedWidth;
    layout->measuredDimensions[BNDYGDimensionHeight] = cachedResults->computedHeight;

    if (gBNDPrintChanges && gBNDPrintSkips) {
      BNDYGLog(node, BNDYGLogLevelVerbose, "%s%d.{[skipped] ", BNDYGSpacer(gBNDDepth), gBNDDepth);
      if (node->getPrintFunc() != nullptr) {
        node->getPrintFunc()(node);
      }
      BNDYGLog(
          node,
          BNDYGLogLevelVerbose,
          "wm: %s, hm: %s, aw: %f ah: %f => d: (%f, %f) %s\n",
          BNDYGMeasureModeName(widthMeasureMode, performLayout),
          BNDYGMeasureModeName(heightMeasureMode, performLayout),
          availableWidth,
          availableHeight,
          cachedResults->computedWidth,
          cachedResults->computedHeight,
          reason);
    }
  } else {
    if (gBNDPrintChanges) {
      BNDYGLog(
          node,
          BNDYGLogLevelVerbose,
          "%s%d.{%s",
          BNDYGSpacer(gBNDDepth),
          gBNDDepth,
          needToVisitNode ? "*" : "");
      if (node->getPrintFunc() != nullptr) {
        node->getPrintFunc()(node);
      }
      BNDYGLog(
          node,
          BNDYGLogLevelVerbose,
          "wm: %s, hm: %s, aw: %f ah: %f %s\n",
          BNDYGMeasureModeName(widthMeasureMode, performLayout),
          BNDYGMeasureModeName(heightMeasureMode, performLayout),
          availableWidth,
          availableHeight,
          reason);
    }

    BNDYGNodelayoutImpl(node,
                     availableWidth,
                     availableHeight,
                     ownerDirection,
                     widthMeasureMode,
                     heightMeasureMode,
                     ownerWidth,
                     ownerHeight,
                     performLayout,
                     config);

    if (gBNDPrintChanges) {
      BNDYGLog(
          node,
          BNDYGLogLevelVerbose,
          "%s%d.}%s",
          BNDYGSpacer(gBNDDepth),
          gBNDDepth,
          needToVisitNode ? "*" : "");
      if (node->getPrintFunc() != nullptr) {
        node->getPrintFunc()(node);
      }
      BNDYGLog(
          node,
          BNDYGLogLevelVerbose,
          "wm: %s, hm: %s, d: (%f, %f) %s\n",
          BNDYGMeasureModeName(widthMeasureMode, performLayout),
          BNDYGMeasureModeName(heightMeasureMode, performLayout),
          layout->measuredDimensions[BNDYGDimensionWidth],
          layout->measuredDimensions[BNDYGDimensionHeight],
          reason);
    }

    layout->lastOwnerDirection = ownerDirection;

    if (cachedResults == nullptr) {
      if (layout->nextCachedMeasurementsIndex == BNDYG_MAX_CACHED_RESULT_COUNT) {
        if (gBNDPrintChanges) {
          BNDYGLog(node, BNDYGLogLevelVerbose, "Out of cache entries!\n");
        }
        layout->nextCachedMeasurementsIndex = 0;
      }

      BNDYGCachedMeasurement *newCacheEntry;
      if (performLayout) {
        // Use the single layout cache entry.
        newCacheEntry = &layout->cachedLayout;
      } else {
        // Allocate a new measurement cache entry.
        newCacheEntry = &layout->cachedMeasurements[layout->nextCachedMeasurementsIndex];
        layout->nextCachedMeasurementsIndex++;
      }

      newCacheEntry->availableWidth = availableWidth;
      newCacheEntry->availableHeight = availableHeight;
      newCacheEntry->widthMeasureMode = widthMeasureMode;
      newCacheEntry->heightMeasureMode = heightMeasureMode;
      newCacheEntry->computedWidth = layout->measuredDimensions[BNDYGDimensionWidth];
      newCacheEntry->computedHeight = layout->measuredDimensions[BNDYGDimensionHeight];
    }
  }

  if (performLayout) {
    node->setLayoutDimension(
        node->getLayout().measuredDimensions[BNDYGDimensionWidth],
        BNDYGDimensionWidth);
    node->setLayoutDimension(
        node->getLayout().measuredDimensions[BNDYGDimensionHeight],
        BNDYGDimensionHeight);

    node->setHasNewLayout(true);
    node->setDirty(false);
  }

  gBNDDepth--;
  layout->generationCount = gBNDCurrentGenerationCount;
  return (needToVisitNode || cachedResults == nullptr);
}

void BNDYGConfigSetPointScaleFactor(const BNDYGConfigRef config, const float pixelsInPoint) {
  BNDYGAssertWithConfig(config, pixelsInPoint >= 0.0f, "Scale factor should not be less than zero");

  // We store points for Pixel as we will use it for rounding
  if (pixelsInPoint == 0.0f) {
    // Zero is used to skip rounding
    config->pointScaleFactor = 0.0f;
  } else {
    config->pointScaleFactor = pixelsInPoint;
  }
}

static void BNDYGRoundToPixelGrid(const BNDYGNodeRef node,
                               const float pointScaleFactor,
                               const float absoluteLeft,
                               const float absoluteTop) {
  if (pointScaleFactor == 0.0f) {
    return;
  }

  const float nodeLeft = node->getLayout().position[BNDYGEdgeLeft];
  const float nodeTop = node->getLayout().position[BNDYGEdgeTop];

  const float nodeWidth = node->getLayout().dimensions[BNDYGDimensionWidth];
  const float nodeHeight = node->getLayout().dimensions[BNDYGDimensionHeight];

  const float absoluteNodeLeft = absoluteLeft + nodeLeft;
  const float absoluteNodeTop = absoluteTop + nodeTop;

  const float absoluteNodeRight = absoluteNodeLeft + nodeWidth;
  const float absoluteNodeBottom = absoluteNodeTop + nodeHeight;

  // If a node has a custom measure function we never want to round down its size as this could
  // lead to unwanted text truncation.
  const bool textRounding = node->getNodeType() == BNDYGNodeTypeText;

  node->setLayoutPosition(
      BNDYGRoundValueToPixelGrid(nodeLeft, pointScaleFactor, false, textRounding),
      BNDYGEdgeLeft);

  node->setLayoutPosition(
      BNDYGRoundValueToPixelGrid(nodeTop, pointScaleFactor, false, textRounding),
      BNDYGEdgeTop);

  // We multiply dimension by scale factor and if the result is close to the whole number, we don't
  // have any fraction
  // To verify if the result is close to whole number we want to check both floor and ceil numbers
  const bool hasFractionalWidth = !BNDYGFloatsEqual(fmodf(nodeWidth * pointScaleFactor, 1.0), 0) &&
                                  !BNDYGFloatsEqual(fmodf(nodeWidth * pointScaleFactor, 1.0), 1.0);
  const bool hasFractionalHeight = !BNDYGFloatsEqual(fmodf(nodeHeight * pointScaleFactor, 1.0), 0) &&
                                   !BNDYGFloatsEqual(fmodf(nodeHeight * pointScaleFactor, 1.0), 1.0);

  node->setLayoutDimension(
      BNDYGRoundValueToPixelGrid(
          absoluteNodeRight,
          pointScaleFactor,
          (textRounding && hasFractionalWidth),
          (textRounding && !hasFractionalWidth)) -
          BNDYGRoundValueToPixelGrid(
              absoluteNodeLeft, pointScaleFactor, false, textRounding),
      BNDYGDimensionWidth);

  node->setLayoutDimension(
      BNDYGRoundValueToPixelGrid(
          absoluteNodeBottom,
          pointScaleFactor,
          (textRounding && hasFractionalHeight),
          (textRounding && !hasFractionalHeight)) -
          BNDYGRoundValueToPixelGrid(
              absoluteNodeTop, pointScaleFactor, false, textRounding),
      BNDYGDimensionHeight);

  const uint32_t childCount = BNDYGNodeGetChildCount(node);
  for (uint32_t i = 0; i < childCount; i++) {
    BNDYGRoundToPixelGrid(
        BNDYGNodeGetChild(node, i),
        pointScaleFactor,
        absoluteNodeLeft,
        absoluteNodeTop);
  }
}

void BNDYGNodeCalculateLayout(
    const BNDYGNodeRef node,
    const float ownerWidth,
    const float ownerHeight,
    const BNDYGDirection ownerDirection) {
  // Increment the generation count. This will force the recursive routine to
  // visit
  // all dirty nodes at least once. Subsequent visits will be skipped if the
  // input
  // parameters don't change.
  gBNDCurrentGenerationCount++;
  node->resolveDimension();
  float width = BNDYGUndefined;
  BNDYGMeasureMode widthMeasureMode = BNDYGMeasureModeUndefined;
  if (BNDYGNodeIsStyleDimDefined(node, BNDYGFlexDirectionRow, ownerWidth)) {
    width = BNDYGUnwrapFloatOptional(
        BNDYGResolveValue(
            node->getResolvedDimension(dim[BNDYGFlexDirectionRow]), ownerWidth) +
        node->getMarginForAxis(BNDYGFlexDirectionRow, ownerWidth));
    widthMeasureMode = BNDYGMeasureModeExactly;
  } else if (!BNDYGResolveValue(
                  node->getStyle().maxDimensions[BNDYGDimensionWidth], ownerWidth)
                  .isUndefined()) {
    width = BNDYGUnwrapFloatOptional(BNDYGResolveValue(
        node->getStyle().maxDimensions[BNDYGDimensionWidth], ownerWidth));
    widthMeasureMode = BNDYGMeasureModeAtMost;
  } else {
    width = ownerWidth;
    widthMeasureMode = BNDYGFloatIsUndefined(width) ? BNDYGMeasureModeUndefined
                                                 : BNDYGMeasureModeExactly;
  }

  float height = BNDYGUndefined;
  BNDYGMeasureMode heightMeasureMode = BNDYGMeasureModeUndefined;
  if (BNDYGNodeIsStyleDimDefined(node, BNDYGFlexDirectionColumn, ownerHeight)) {
    height = BNDYGUnwrapFloatOptional(
        BNDYGResolveValue(
            node->getResolvedDimension(dim[BNDYGFlexDirectionColumn]),
            ownerHeight) +
        node->getMarginForAxis(BNDYGFlexDirectionColumn, ownerWidth));
    heightMeasureMode = BNDYGMeasureModeExactly;
  } else if (!BNDYGResolveValue(
                  node->getStyle().maxDimensions[BNDYGDimensionHeight],
                  ownerHeight)
                  .isUndefined()) {
    height = BNDYGUnwrapFloatOptional(BNDYGResolveValue(node->getStyle().maxDimensions[BNDYGDimensionHeight], ownerHeight));
    heightMeasureMode = BNDYGMeasureModeAtMost;
  } else {
    height = ownerHeight;
    heightMeasureMode = BNDYGFloatIsUndefined(height) ? BNDYGMeasureModeUndefined
                                                   : BNDYGMeasureModeExactly;
  }
  if (BNDYGLayoutNodeInternal(
          node,
          width,
          height,
          ownerDirection,
          widthMeasureMode,
          heightMeasureMode,
          ownerWidth,
          ownerHeight,
          true,
          "initial",
          node->getConfig())) {
    node->setPosition(
        node->getLayout().direction, ownerWidth, ownerHeight, ownerWidth);
    BNDYGRoundToPixelGrid(node, node->getConfig()->pointScaleFactor, 0.0f, 0.0f);

    if (gPrintTree) {
      BNDYGNodePrint(
          node,
          (BNDYGPrintOptions)(
              BNDYGPrintOptionsLayout | BNDYGPrintOptionsChildren |
              BNDYGPrintOptionsStyle));
    }
  }

  // We want to get rid off `useLegacyStretchBehaviour` from BNDYGConfig. But we
  // aren't sure whether client's of yoga have gotten rid off this flag or not.
  // So logging this in BNDYGLayout would help to find out the call sites depending
  // on this flag. This check would be removed once we are sure no one is
  // dependent on this flag anymore. The flag
  // `shouldDiffLayoutWithoutLegacyStretchBehaviour` in BNDYGConfig will help to
  // run experiments.
  if (node->getConfig()->shouldDiffLayoutWithoutLegacyStretchBehaviour &&
      node->didUseLegacyFlag()) {
    const BNDYGNodeRef originalNode = BNDYGNodeDeepClone(node);
    originalNode->resolveDimension();
    // Recursively mark nodes as dirty
    originalNode->markDirtyAndPropogateDownwards();
    gBNDCurrentGenerationCount++;
    // Rerun the layout, and calculate the diff
    originalNode->setAndPropogateUseLegacyFlag(false);
    if (BNDYGLayoutNodeInternal(
            originalNode,
            width,
            height,
            ownerDirection,
            widthMeasureMode,
            heightMeasureMode,
            ownerWidth,
            ownerHeight,
            true,
            "initial",
            originalNode->getConfig())) {
      originalNode->setPosition(
          originalNode->getLayout().direction,
          ownerWidth,
          ownerHeight,
          ownerWidth);
      BNDYGRoundToPixelGrid(
          originalNode,
          originalNode->getConfig()->pointScaleFactor,
          0.0f,
          0.0f);

      // Set whether the two layouts are different or not.
      node->setLayoutDoesLegacyFlagAffectsLayout(
          !originalNode->isLayoutTreeEqualToNode(*node));

      if (gPrintTree) {
        BNDYGNodePrint(
            originalNode,
            (BNDYGPrintOptions)(
                BNDYGPrintOptionsLayout | BNDYGPrintOptionsChildren |
                BNDYGPrintOptionsStyle));
      }
    }
    BNDYGConfigFreeRecursive(originalNode);
    BNDYGNodeFreeRecursive(originalNode);
  }
}

void BNDYGConfigSetLogger(const BNDYGConfigRef config, BNDYGLogger logger) {
  if (logger != nullptr) {
    config->logger = logger;
  } else {
#ifdef ANDROID
    config->logger = &BNDYGAndroidLog;
#else
    config->logger = &BNDYGDefaultLog;
#endif
  }
}

void BNDYGConfigSetShouldDiffLayoutWithoutLegacyStretchBehaviour(
    const BNDYGConfigRef config,
    const bool shouldDiffLayout) {
  config->shouldDiffLayoutWithoutLegacyStretchBehaviour = shouldDiffLayout;
}

static void BNDYGVLog(const BNDYGConfigRef config,
                   const BNDYGNodeRef node,
                   BNDYGLogLevel level,
                   const char *format,
                   va_list args) {
  const BNDYGConfigRef logConfig = config != nullptr ? config : BNDYGConfigGetDefault();
  logConfig->logger(logConfig, node, level, format, args);

  if (level == BNDYGLogLevelFatal) {
    abort();
  }
}

void BNDYGLogWithConfig(const BNDYGConfigRef config, BNDYGLogLevel level, const char *format, ...) {
  va_list args;
  va_start(args, format);
  BNDYGVLog(config, nullptr, level, format, args);
  va_end(args);
}

void BNDYGLog(const BNDYGNodeRef node, BNDYGLogLevel level, const char *format, ...) {
  va_list args;
  va_start(args, format);
  BNDYGVLog(
      node == nullptr ? nullptr : node->getConfig(), node, level, format, args);
  va_end(args);
}

void BNDYGAssert(const bool condition, const char *message) {
  if (!condition) {
    BNDYGLog(nullptr, BNDYGLogLevelFatal, "%s\n", message);
  }
}

void BNDYGAssertWithNode(const BNDYGNodeRef node, const bool condition, const char *message) {
  if (!condition) {
    BNDYGLog(node, BNDYGLogLevelFatal, "%s\n", message);
  }
}

void BNDYGAssertWithConfig(const BNDYGConfigRef config, const bool condition, const char *message) {
  if (!condition) {
    BNDYGLogWithConfig(config, BNDYGLogLevelFatal, "%s\n", message);
  }
}

void BNDYGConfigSetExperimentalFeatureEnabled(const BNDYGConfigRef config,
                                           const BNDYGExperimentalFeature feature,
                                           const bool enabled) {
  config->experimentalFeatures[feature] = enabled;
}

inline bool BNDYGConfigIsExperimentalFeatureEnabled(const BNDYGConfigRef config,
                                                 const BNDYGExperimentalFeature feature) {
  return config->experimentalFeatures[feature];
}

void BNDYGConfigSetUseWebDefaults(const BNDYGConfigRef config, const bool enabled) {
  config->useWebDefaults = enabled;
}

void BNDYGConfigSetUseLegacyStretchBehaviour(const BNDYGConfigRef config,
                                          const bool useLegacyStretchBehaviour) {
  config->useLegacyStretchBehaviour = useLegacyStretchBehaviour;
}

bool BNDYGConfigGetUseWebDefaults(const BNDYGConfigRef config) {
  return config->useWebDefaults;
}

void BNDYGConfigSetContext(const BNDYGConfigRef config, void *context) {
  config->context = context;
}

void *BNDYGConfigGetContext(const BNDYGConfigRef config) {
  return config->context;
}

void BNDYGConfigSetCloneNodeFunc(const BNDYGConfigRef config, const BNDYGCloneNodeFunc callback) {
  config->cloneNodeCallback = callback;
}

static void BNDYGTraverseChildrenPreOrder(const BNDYGVector& children, const std::function<void(BNDYGNodeRef node)>& f) {
  for (BNDYGNodeRef node : children) {
    f(node);
    BNDYGTraverseChildrenPreOrder(node->getChildren(), f);
  }
}

void BNDYGTraversePreOrder(BNDYGNodeRef const node, std::function<void(BNDYGNodeRef node)>&& f) {
  if (!node) {
    return;
  }
  f(node);
  BNDYGTraverseChildrenPreOrder(node->getChildren(), f);
}
