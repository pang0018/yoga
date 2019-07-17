/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "BNDYGNode.h"
#include <iostream>
#include "Utils.h"

BNDYGFloatOptional BNDYGNode::getLeadingPosition(
    const BNDYGFlexDirection& axis,
    const float& axisSize) const {
  if (BNDYGFlexDirectionIsRow(axis)) {
    const BNDYGValue* leadingPosition =
        BNDYGComputedEdgeValue(style_.position, BNDYGEdgeStart, &BNDYGValueUndefined);
    if (leadingPosition->unit != BNDYGUnitUndefined) {
      return BNDYGResolveValue(*leadingPosition, axisSize);
    }
  }

  const BNDYGValue* leadingPosition =
      BNDYGComputedEdgeValue(style_.position, leadingBND[axis], &BNDYGValueUndefined);

  return leadingPosition->unit == BNDYGUnitUndefined
      ? BNDYGFloatOptional(0)
      : BNDYGResolveValue(*leadingPosition, axisSize);
}

BNDYGFloatOptional BNDYGNode::getTrailingPosition(
    const BNDYGFlexDirection& axis,
    const float& axisSize) const {
  if (BNDYGFlexDirectionIsRow(axis)) {
    const BNDYGValue* trailingPosition =
        BNDYGComputedEdgeValue(style_.position, BNDYGEdgeEnd, &BNDYGValueUndefined);
    if (trailingPosition->unit != BNDYGUnitUndefined) {
      return BNDYGResolveValue(*trailingPosition, axisSize);
    }
  }

  const BNDYGValue* trailingPosition =
      BNDYGComputedEdgeValue(style_.position, trailingBND[axis], &BNDYGValueUndefined);

  return trailingPosition->unit == BNDYGUnitUndefined
      ? BNDYGFloatOptional(0)
      : BNDYGResolveValue(*trailingPosition, axisSize);
}

bool BNDYGNode::isLeadingPositionDefined(const BNDYGFlexDirection& axis) const {
  return (BNDYGFlexDirectionIsRow(axis) &&
          BNDYGComputedEdgeValue(style_.position, BNDYGEdgeStart, &BNDYGValueUndefined)
                  ->unit != BNDYGUnitUndefined) ||
      BNDYGComputedEdgeValue(style_.position, leadingBND[axis], &BNDYGValueUndefined)
          ->unit != BNDYGUnitUndefined;
}

bool BNDYGNode::isTrailingPosDefined(const BNDYGFlexDirection& axis) const {
  return (BNDYGFlexDirectionIsRow(axis) &&
          BNDYGComputedEdgeValue(style_.position, BNDYGEdgeEnd, &BNDYGValueUndefined)
                  ->unit != BNDYGUnitUndefined) ||
      BNDYGComputedEdgeValue(style_.position, trailingBND[axis], &BNDYGValueUndefined)
          ->unit != BNDYGUnitUndefined;
}

BNDYGFloatOptional BNDYGNode::getLeadingMargin(
    const BNDYGFlexDirection& axis,
    const float& widthSize) const {
  if (BNDYGFlexDirectionIsRow(axis) &&
      style_.margin[BNDYGEdgeStart].unit != BNDYGUnitUndefined) {
    return BNDYGResolveValueMargin(style_.margin[BNDYGEdgeStart], widthSize);
  }

  return BNDYGResolveValueMargin(
      *BNDYGComputedEdgeValue(style_.margin, leadingBND[axis], &BNDYGValueZero),
      widthSize);
}

BNDYGFloatOptional BNDYGNode::getTrailingMargin(
    const BNDYGFlexDirection& axis,
    const float& widthSize) const {
  if (BNDYGFlexDirectionIsRow(axis) &&
      style_.margin[BNDYGEdgeEnd].unit != BNDYGUnitUndefined) {
    return BNDYGResolveValueMargin(style_.margin[BNDYGEdgeEnd], widthSize);
  }

  return BNDYGResolveValueMargin(
      *BNDYGComputedEdgeValue(style_.margin, trailingBND[axis], &BNDYGValueZero),
      widthSize);
}

BNDYGFloatOptional BNDYGNode::getMarginForAxis(
    const BNDYGFlexDirection& axis,
    const float& widthSize) const {
  return getLeadingMargin(axis, widthSize) + getTrailingMargin(axis, widthSize);
}

// Setters



void BNDYGNode::setMeasureFunc(BNDYGMeasureFunc measureFunc) {
  if (measureFunc == nullptr) {
    measure_ = nullptr;
    // TODO: t18095186 Move nodeType to opt-in function and mark appropriate
    // places in Litho
    nodeType_ = BNDYGNodeTypeDefault;
  } else {
    BNDYGAssertWithNode(
        this,
        children_.size() == 0,
        "Cannot set measure function: Nodes with measure functions cannot have children.");
    measure_ = measureFunc;
    // TODO: t18095186 Move nodeType to opt-in function and mark appropriate
    // places in Litho
    setNodeType(BNDYGNodeTypeText);
  }

  measure_ = measureFunc;
}

void BNDYGNode::replaceChild(BNDYGNodeRef child, uint32_t index) {
  children_[index] = child;
}

void BNDYGNode::replaceChild(BNDYGNodeRef oldChild, BNDYGNodeRef newChild) {
  std::replace(children_.begin(), children_.end(), oldChild, newChild);
}

void BNDYGNode::insertChild(BNDYGNodeRef child, uint32_t index) {
  children_.insert(children_.begin() + index, child);
}

void BNDYGNode::setDirty(bool isDirty) {
  if (isDirty == isDirty_) {
    return;
  }
  isDirty_ = isDirty;
  if (isDirty && dirtied_) {
    dirtied_(this);
  }
}

bool BNDYGNode::removeChild(BNDYGNodeRef child) {
  std::vector<BNDYGNodeRef>::iterator p =
      std::find(children_.begin(), children_.end(), child);
  if (p != children_.end()) {
    children_.erase(p);
    return true;
  }
  return false;
}

void BNDYGNode::removeChild(uint32_t index) {
  children_.erase(children_.begin() + index);
}

void BNDYGNode::setLayoutDirection(BNDYGDirection direction) {
  layout_.direction = direction;
}

void BNDYGNode::setLayoutMargin(float margin, int index) {
  layout_.margin[index] = margin;
}

void BNDYGNode::setLayoutBorder(float border, int index) {
  layout_.border[index] = border;
}

void BNDYGNode::setLayoutPadding(float padding, int index) {
  layout_.padding[index] = padding;
}

void BNDYGNode::setLayoutLastOwnerDirection(BNDYGDirection direction) {
  layout_.lastOwnerDirection = direction;
}

void BNDYGNode::setLayoutComputedFlexBasis(
    const BNDYGFloatOptional& computedFlexBasis) {
  layout_.computedFlexBasis = computedFlexBasis;
}

void BNDYGNode::setLayoutPosition(float position, int index) {
  layout_.position[index] = position;
}

void BNDYGNode::setLayoutComputedFlexBasisGeneration(
    uint32_t computedFlexBasisGeneration) {
  layout_.computedFlexBasisGeneration = computedFlexBasisGeneration;
}

void BNDYGNode::setLayoutMeasuredDimension(float measuredDimension, int index) {
  layout_.measuredDimensions[index] = measuredDimension;
}

void BNDYGNode::setLayoutHadOverflow(bool hadOverflow) {
  layout_.hadOverflow = hadOverflow;
}

void BNDYGNode::setLayoutDimension(float dimension, int index) {
  layout_.dimensions[index] = dimension;
}

// If both left and right are defined, then use left. Otherwise return
// +left or -right depending on which is defined.
BNDYGFloatOptional BNDYGNode::relativePosition(
    const BNDYGFlexDirection& axis,
    const float& axisSize) const {
  if (isLeadingPositionDefined(axis)) {
    return getLeadingPosition(axis, axisSize);
  }

  BNDYGFloatOptional trailingPosition = getTrailingPosition(axis, axisSize);
  if (!trailingPosition.isUndefined()) {
    trailingPosition.setValue(-1 * trailingPosition.getValue());
  }
  return trailingPosition;
}

void BNDYGNode::setPosition(
    const BNDYGDirection direction,
    const float mainSize,
    const float crossSize,
    const float ownerWidth) {
  /* Root nodes should be always layouted as LTR, so we don't return negative
   * values. */
  const BNDYGDirection directionRespectingRoot =
      owner_ != nullptr ? direction : BNDYGDirectionLTR;
  const BNDYGFlexDirection mainAxis =
      BNDYGResolveFlexDirection(style_.flexDirection, directionRespectingRoot);
  const BNDYGFlexDirection crossAxis =
      BNDYGFlexDirectionCross(mainAxis, directionRespectingRoot);

  const BNDYGFloatOptional relativePositionMain =
      relativePosition(mainAxis, mainSize);
  const BNDYGFloatOptional relativePositionCross =
      relativePosition(crossAxis, crossSize);

  setLayoutPosition(
      BNDYGUnwrapFloatOptional(
          getLeadingMargin(mainAxis, ownerWidth) + relativePositionMain),
      leadingBND[mainAxis]);
  setLayoutPosition(
      BNDYGUnwrapFloatOptional(
          getTrailingMargin(mainAxis, ownerWidth) + relativePositionMain),
      trailingBND[mainAxis]);
  setLayoutPosition(
      BNDYGUnwrapFloatOptional(
          getLeadingMargin(crossAxis, ownerWidth) + relativePositionCross),
      leadingBND[crossAxis]);
  setLayoutPosition(
      BNDYGUnwrapFloatOptional(
          getTrailingMargin(crossAxis, ownerWidth) + relativePositionCross),
      trailingBND[crossAxis]);
}

BNDYGNode::BNDYGNode()
    : context_(nullptr),
      print_(nullptr),
      hasNewLayout_(true),
      nodeType_(BNDYGNodeTypeDefault),
      measure_(nullptr),
      baseline_(nullptr),
      dirtied_(nullptr),
      style_(BNDYGStyle()),
      layout_(BNDYGLayout()),
      lineIndex_(0),
      owner_(nullptr),
      children_(BNDYGVector()),
      config_(nullptr),
      isDirty_(false),
      resolvedDimensions_({{BNDYGValueUndefined, BNDYGValueUndefined}}) {}

BNDYGNode::BNDYGNode(const BNDYGNode& node)
    : context_(node.context_),
      print_(node.print_),
      hasNewLayout_(node.hasNewLayout_),
      nodeType_(node.nodeType_),
      measure_(node.measure_),
      baseline_(node.baseline_),
      dirtied_(node.dirtied_),
      style_(node.style_),
      layout_(node.layout_),
      lineIndex_(node.lineIndex_),
      owner_(node.owner_),
      children_(node.children_),
      config_(node.config_),
      isDirty_(node.isDirty_),
      resolvedDimensions_(node.resolvedDimensions_) {}

BNDYGNode::BNDYGNode(const BNDYGConfigRef newConfig) : BNDYGNode() {
  config_ = newConfig;
}

BNDYGNode::BNDYGNode(
    void* context,
    BNDYGPrintFunc print,
    bool hasNewLayout,
    BNDYGNodeType nodeType,
    BNDYGMeasureFunc measure,
    BNDYGBaselineFunc baseline,
    BNDYGDirtiedFunc dirtied,
    BNDYGStyle style,
    const BNDYGLayout& layout,
    uint32_t lineIndex,
    BNDYGNodeRef owner,
    const BNDYGVector& children,
    BNDYGConfigRef config,
    bool isDirty,
    std::array<BNDYGValue, 2> resolvedDimensions)
    : context_(context),
      print_(print),
      hasNewLayout_(hasNewLayout),
      nodeType_(nodeType),
      measure_(measure),
      baseline_(baseline),
      dirtied_(dirtied),
      style_(style),
      layout_(layout),
      lineIndex_(lineIndex),
      owner_(owner),
      children_(children),
      config_(config),
      isDirty_(isDirty),
      resolvedDimensions_(resolvedDimensions) {}

BNDYGNode& BNDYGNode::operator=(const BNDYGNode& node) {
  if (&node == this) {
    return *this;
  }

  for (auto child : children_) {
    delete child;
  }

  context_ = node.getContext();
  print_ = node.getPrintFunc();
  hasNewLayout_ = node.getHasNewLayout();
  nodeType_ = node.getNodeType();
  measure_ = node.getMeasure();
  baseline_ = node.getBaseline();
  dirtied_ = node.getDirtied();
  style_ = node.style_;
  layout_ = node.layout_;
  lineIndex_ = node.getLineIndex();
  owner_ = node.getOwner();
  children_ = node.getChildren();
  config_ = node.getConfig();
  isDirty_ = node.isDirty();
  resolvedDimensions_ = node.getResolvedDimensions();

  return *this;
}

BNDYGValue BNDYGNode::marginLeadingValue(const BNDYGFlexDirection axis) const {
  if (BNDYGFlexDirectionIsRow(axis) &&
      style_.margin[BNDYGEdgeStart].unit != BNDYGUnitUndefined) {
    return style_.margin[BNDYGEdgeStart];
  } else {
    return style_.margin[leadingBND[axis]];
  }
}

BNDYGValue BNDYGNode::marginTrailingValue(const BNDYGFlexDirection axis) const {
  if (BNDYGFlexDirectionIsRow(axis) &&
      style_.margin[BNDYGEdgeEnd].unit != BNDYGUnitUndefined) {
    return style_.margin[BNDYGEdgeEnd];
  } else {
    return style_.margin[trailingBND[axis]];
  }
}

BNDYGValue BNDYGNode::resolveFlexBasisPtr() const {
  BNDYGValue flexBasis = style_.flexBasis;
  if (flexBasis.unit != BNDYGUnitAuto && flexBasis.unit != BNDYGUnitUndefined) {
    return flexBasis;
  }
  if (!style_.flex.isUndefined() && style_.flex.getValue() > 0.0f) {
    return config_->useWebDefaults ? BNDYGValueAuto : BNDYGValueZero;
  }
  return BNDYGValueAuto;
}

void BNDYGNode::resolveDimension() {
  for (uint32_t dim = BNDYGDimensionWidth; dim < BNDYGDimensionCount; dim++) {
    if (getStyle().maxDimensions[dim].unit != BNDYGUnitUndefined &&
        BNDYGValueEqual(
            getStyle().maxDimensions[dim], style_.minDimensions[dim])) {
      resolvedDimensions_[dim] = style_.maxDimensions[dim];
    } else {
      resolvedDimensions_[dim] = style_.dimensions[dim];
    }
  }
}

BNDYGDirection BNDYGNode::resolveDirection(const BNDYGDirection ownerDirection) {
  if (style_.direction == BNDYGDirectionInherit) {
    return ownerDirection > BNDYGDirectionInherit ? ownerDirection
                                                : BNDYGDirectionLTR;
  } else {
    return style_.direction;
  }
}

void BNDYGNode::clearChildren() {
  children_.clear();
  children_.shrink_to_fit();
}

BNDYGNode::~BNDYGNode() {
  // All the member variables are deallocated externally, so no need to
  // deallocate here
}

// Other Methods

void BNDYGNode::cloneChildrenIfNeeded() {
  // BNDYGNodeRemoveChild in yoga.cpp has a forked variant of this algorithm
  // optimized for deletions.

  const uint32_t childCount = static_cast<uint32_t>(children_.size());
  if (childCount == 0) {
    // This is an empty set. Nothing to clone.
    return;
  }

  const BNDYGNodeRef firstChild = children_.front();
  if (firstChild->getOwner() == this) {
    // If the first child has this node as its owner, we assume that it is
    // already unique. We can do this because if we have it has a child, that
    // means that its owner was at some point cloned which made that subtree
    // immutable. We also assume that all its sibling are cloned as well.
    return;
  }

  const BNDYGCloneNodeFunc cloneNodeCallback = config_->cloneNodeCallback;
  for (uint32_t i = 0; i < childCount; ++i) {
    const BNDYGNodeRef oldChild = children_[i];
    BNDYGNodeRef newChild = nullptr;
    if (cloneNodeCallback) {
      newChild = cloneNodeCallback(oldChild, this, i);
    }
    if (newChild == nullptr) {
      newChild = BNDYGNodeClone(oldChild);
    }
    replaceChild(newChild, i);
    newChild->setOwner(this);
  }
}

void BNDYGNode::markDirtyAndPropogate() {
  if (!isDirty_) {
    setDirty(true);
    setLayoutComputedFlexBasis(BNDYGFloatOptional());
    if (owner_) {
      owner_->markDirtyAndPropogate();
    }
  }
}

void BNDYGNode::markDirtyAndPropogateDownwards() {
  isDirty_ = true;
  for_each(children_.begin(), children_.end(), [](BNDYGNodeRef childNode) {
    childNode->markDirtyAndPropogateDownwards();
  });
}

float BNDYGNode::resolveFlexGrow() {
  // Root nodes flexGrow should always be 0
  if (owner_ == nullptr) {
    return 0.0;
  }
  if (!style_.flexGrow.isUndefined()) {
    return style_.flexGrow.getValue();
  }
  if (!style_.flex.isUndefined() && style_.flex.getValue() > 0.0f) {
    return style_.flex.getValue();
  }
  return kDefaultFlexGrow;
}

float BNDYGNode::resolveFlexShrink() {
  if (owner_ == nullptr) {
    return 0.0;
  }
  if (!style_.flexShrink.isUndefined()) {
    return style_.flexShrink.getValue();
  }
  if (!config_->useWebDefaults && !style_.flex.isUndefined() &&
      style_.flex.getValue() < 0.0f) {
    return -style_.flex.getValue();
  }
  return config_->useWebDefaults ? kWebDefaultFlexShrink : kDefaultFlexShrink;
}

bool BNDYGNode::isNodeFlexible() {
  return (
      (style_.positionType == BNDYGPositionTypeRelative) &&
      (resolveFlexGrow() != 0 || resolveFlexShrink() != 0));
}

float BNDYGNode::getLeadingBorder(const BNDYGFlexDirection& axis) const {
  if (BNDYGFlexDirectionIsRow(axis) &&
      style_.border[BNDYGEdgeStart].unit != BNDYGUnitUndefined &&
      !BNDYGFloatIsUndefined(style_.border[BNDYGEdgeStart].value) &&
      style_.border[BNDYGEdgeStart].value >= 0.0f) {
    return style_.border[BNDYGEdgeStart].value;
  }

  float computedEdgeValue =
      BNDYGComputedEdgeValue(style_.border, leadingBND[axis], &BNDYGValueZero)->value;
  return BNDYGFloatMax(computedEdgeValue, 0.0f);
}

float BNDYGNode::getTrailingBorder(const BNDYGFlexDirection& flexDirection) const {
  if (BNDYGFlexDirectionIsRow(flexDirection) &&
      style_.border[BNDYGEdgeEnd].unit != BNDYGUnitUndefined &&
      !BNDYGFloatIsUndefined(style_.border[BNDYGEdgeEnd].value) &&
      style_.border[BNDYGEdgeEnd].value >= 0.0f) {
    return style_.border[BNDYGEdgeEnd].value;
  }

  float computedEdgeValue =
      BNDYGComputedEdgeValue(style_.border, trailingBND[flexDirection], &BNDYGValueZero)
          ->value;
  return BNDYGFloatMax(computedEdgeValue, 0.0f);
}

BNDYGFloatOptional BNDYGNode::getLeadingPadding(
    const BNDYGFlexDirection& axis,
    const float& widthSize) const {
  const BNDYGFloatOptional& paddingEdgeStart =
      BNDYGResolveValue(style_.padding[BNDYGEdgeStart], widthSize);
  if (BNDYGFlexDirectionIsRow(axis) &&
      style_.padding[BNDYGEdgeStart].unit != BNDYGUnitUndefined &&
      !paddingEdgeStart.isUndefined() && paddingEdgeStart.getValue() > 0.0f) {
    return paddingEdgeStart;
  }

  BNDYGFloatOptional resolvedValue = BNDYGResolveValue(
      *BNDYGComputedEdgeValue(style_.padding, leadingBND[axis], &BNDYGValueZero),
      widthSize);
  return BNDYGFloatOptionalMax(resolvedValue, BNDYGFloatOptional(0.0f));
}

BNDYGFloatOptional BNDYGNode::getTrailingPadding(
    const BNDYGFlexDirection& axis,
    const float& widthSize) const {
  if (BNDYGFlexDirectionIsRow(axis) &&
      style_.padding[BNDYGEdgeEnd].unit != BNDYGUnitUndefined &&
      !BNDYGResolveValue(style_.padding[BNDYGEdgeEnd], widthSize).isUndefined() &&
      BNDYGResolveValue(style_.padding[BNDYGEdgeEnd], widthSize).getValue() >= 0.0f) {
    return BNDYGResolveValue(style_.padding[BNDYGEdgeEnd], widthSize);
  }

  BNDYGFloatOptional resolvedValue = BNDYGResolveValue(
      *BNDYGComputedEdgeValue(style_.padding, trailingBND[axis], &BNDYGValueZero),
      widthSize);

  return BNDYGFloatOptionalMax(resolvedValue, BNDYGFloatOptional(0.0f));
}

BNDYGFloatOptional BNDYGNode::getLeadingPaddingAndBorder(
    const BNDYGFlexDirection& axis,
    const float& widthSize) const {
  return getLeadingPadding(axis, widthSize) +
      BNDYGFloatOptional(getLeadingBorder(axis));
}

BNDYGFloatOptional BNDYGNode::getTrailingPaddingAndBorder(
    const BNDYGFlexDirection& axis,
    const float& widthSize) const {
  return getTrailingPadding(axis, widthSize) +
      BNDYGFloatOptional(getTrailingBorder(axis));
}

bool BNDYGNode::didUseLegacyFlag() {
  bool didUseLegacyFlag = layout_.didUseLegacyFlag;
  if (didUseLegacyFlag) {
    return true;
  }
  for (const auto& child : children_) {
    if (child->layout_.didUseLegacyFlag) {
      didUseLegacyFlag = true;
      break;
    }
  }
  return didUseLegacyFlag;
}

void BNDYGNode::setAndPropogateUseLegacyFlag(bool useLegacyFlag) {
  config_->useLegacyStretchBehaviour = useLegacyFlag;
  for_each(children_.begin(), children_.end(), [=](BNDYGNodeRef childNode) {
    childNode->getConfig()->useLegacyStretchBehaviour = useLegacyFlag;
  });
}

void BNDYGNode::setLayoutDoesLegacyFlagAffectsLayout(
    bool doesLegacyFlagAffectsLayout) {
  layout_.doesLegacyStretchFlagAffectsLayout = doesLegacyFlagAffectsLayout;
}

void BNDYGNode::setLayoutDidUseLegacyFlag(bool didUseLegacyFlag) {
  layout_.didUseLegacyFlag = didUseLegacyFlag;
}

bool BNDYGNode::isLayoutTreeEqualToNode(const BNDYGNode& node) const {
  if (children_.size() != node.children_.size()) {
    return false;
  }
  if (layout_ != node.layout_) {
    return false;
  }
  if (children_.size() == 0) {
    return true;
  }

  bool isLayoutTreeEqual = true;
  BNDYGNodeRef otherNodeChildren = nullptr;
  for (std::vector<BNDYGNodeRef>::size_type i = 0; i < children_.size(); ++i) {
    otherNodeChildren = node.children_[i];
    isLayoutTreeEqual =
        children_[i]->isLayoutTreeEqualToNode(*otherNodeChildren);
    if (!isLayoutTreeEqual) {
      return false;
    }
  }
  return isLayoutTreeEqual;
}
