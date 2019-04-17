/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once
#include <stdio.h>
#include "BNDYGConfig.h"
#include "BNDYGLayout.h"
#include "BNDYGStyle.h"
#include "BindYoga-internal.h"

struct BNDYGNode {
 private:
  void* context_;
  BNDYGPrintFunc print_;
  bool hasNewLayout_;
  BNDYGNodeType nodeType_;
  BNDYGMeasureFunc measure_;
  BNDYGBaselineFunc baseline_;
  BNDYGDirtiedFunc dirtied_;
  BNDYGStyle style_;
  BNDYGLayout layout_;
  uint32_t lineIndex_;
  BNDYGNodeRef owner_;
  BNDYGVector children_;
  BNDYGConfigRef config_;
  bool isDirty_;
  std::array<BNDYGValue, 2> resolvedDimensions_;

  BNDYGFloatOptional relativePosition(
      const BNDYGFlexDirection& axis,
      const float& axisSize) const;

 public:
  BNDYGNode();
  ~BNDYGNode();
  explicit BNDYGNode(const BNDYGConfigRef newConfig);
  BNDYGNode(const BNDYGNode& node);
  BNDYGNode& operator=(const BNDYGNode& node);
  BNDYGNode(
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
      std::array<BNDYGValue, 2> resolvedDimensions);

  // Getters
  void* getContext() const {
    return context_;
  }

  BNDYGPrintFunc getPrintFunc() const {
    return print_;
  }

  bool getHasNewLayout() const {
    return hasNewLayout_;
  }
  
  BNDYGNodeType getNodeType() const {
    return nodeType_;
  }
  
  BNDYGMeasureFunc getMeasure() const {
    return measure_;
  }
  
  BNDYGBaselineFunc getBaseline() const {
    return baseline_;
  }
  
  BNDYGDirtiedFunc getDirtied() const {
    return dirtied_;
  }
  
  // For Performance reasons passing as reference.
  BNDYGStyle& getStyle() {
    return style_;
  }

  const BNDYGStyle& getStyle() const {
    return style_;
  }

  // For Performance reasons passing as reference.
  BNDYGLayout& getLayout() {
    return layout_;
  }

  const BNDYGLayout& getLayout() const {
    return layout_;
  }
  
  uint32_t getLineIndex() const {
    return lineIndex_;
  }
  
  // returns the BNDYGNodeRef that owns this BNDYGNode. An owner is used to identify
  // the YogaTree that a BNDYGNode belongs to.
  // This method will return the parent of the BNDYGNode when a BNDYGNode only belongs
  // to one YogaTree or nullptr when the BNDYGNode is shared between two or more
  // YogaTrees.
  BNDYGNodeRef getOwner() const {
    return owner_;
  }
  
  // Deprecated, use getOwner() instead.
  BNDYGNodeRef getParent() const {
    return getOwner();
  }
  
  BNDYGVector getChildren() const {
    return children_;
  }
  
  uint32_t getChildrenCount() const {
    return static_cast<uint32_t>(children_.size());
  }
  
  BNDYGNodeRef getChild(uint32_t index) const {
    return children_.at(index);
  }

  BNDYGConfigRef getConfig() const {
    return config_;
  }
  
  bool isDirty() const {
    return isDirty_;
  }
  
  std::array<BNDYGValue, 2> getResolvedDimensions() const {
    return resolvedDimensions_;
  }
  
  BNDYGValue getResolvedDimension(int index) const {
    return resolvedDimensions_[index];
  }

  // Methods related to positions, margin, padding and border
  BNDYGFloatOptional getLeadingPosition(const BNDYGFlexDirection& axis,
      const float& axisSize) const;
  bool isLeadingPositionDefined(const BNDYGFlexDirection& axis) const;
  bool isTrailingPosDefined(const BNDYGFlexDirection& axis) const;
  BNDYGFloatOptional getTrailingPosition(
      const BNDYGFlexDirection& axis,
      const float& axisSize) const;
  BNDYGFloatOptional getLeadingMargin(
      const BNDYGFlexDirection& axis,
      const float& widthSize) const;
  BNDYGFloatOptional getTrailingMargin(
      const BNDYGFlexDirection& axis,
      const float& widthSize) const;
  float getLeadingBorder(const BNDYGFlexDirection& flexDirection) const;
  float getTrailingBorder(const BNDYGFlexDirection& flexDirection) const;
  BNDYGFloatOptional getLeadingPadding(
      const BNDYGFlexDirection& axis,
      const float& widthSize) const;
  BNDYGFloatOptional getTrailingPadding(
      const BNDYGFlexDirection& axis,
      const float& widthSize) const;
  BNDYGFloatOptional getLeadingPaddingAndBorder(
      const BNDYGFlexDirection& axis,
      const float& widthSize) const;
  BNDYGFloatOptional getTrailingPaddingAndBorder(
      const BNDYGFlexDirection& axis,
      const float& widthSize) const;
  BNDYGFloatOptional getMarginForAxis(
      const BNDYGFlexDirection& axis,
      const float& widthSize) const;
  // Setters

  void setContext(void* context) {
    context_ = context;
  }

  void setPrintFunc(BNDYGPrintFunc printFunc) {
    print_ = printFunc;
  }

  void setHasNewLayout(bool hasNewLayout) {
    hasNewLayout_ = hasNewLayout;
  }
  
  void setNodeType(BNDYGNodeType nodeType) {
    nodeType_ = nodeType;
  }

  void setStyleFlexDirection(BNDYGFlexDirection direction) {
    style_.flexDirection = direction;
  }

  void setStyleAlignContent(BNDYGAlign alignContent) {
    style_.alignContent = alignContent;
  }

  void setMeasureFunc(BNDYGMeasureFunc measureFunc);

  void setBaseLineFunc(BNDYGBaselineFunc baseLineFunc) {
    baseline_ = baseLineFunc;
  }

  void setDirtiedFunc(BNDYGDirtiedFunc dirtiedFunc) {
    dirtied_ = dirtiedFunc;
  }

  void setStyle(const BNDYGStyle& style) {
    style_ = style;
  }
  
  void setLayout(const BNDYGLayout& layout) {
    layout_ = layout;
  }
  
  void setLineIndex(uint32_t lineIndex) {
    lineIndex_ = lineIndex;
  }
  
  void setOwner(BNDYGNodeRef owner) {
    owner_ = owner;
  }

  void setChildren(const BNDYGVector& children) {
    children_ = children;
  }

  // TODO: rvalue override for setChildren
  
  void setConfig(BNDYGConfigRef config) {
    config_ = config;
  }

  void setDirty(bool isDirty);
  void setLayoutLastOwnerDirection(BNDYGDirection direction);
  void setLayoutComputedFlexBasis(const BNDYGFloatOptional& computedFlexBasis);
  void setLayoutComputedFlexBasisGeneration(
      uint32_t computedFlexBasisGeneration);
  void setLayoutMeasuredDimension(float measuredDimension, int index);
  void setLayoutHadOverflow(bool hadOverflow);
  void setLayoutDimension(float dimension, int index);
  void setLayoutDirection(BNDYGDirection direction);
  void setLayoutMargin(float margin, int index);
  void setLayoutBorder(float border, int index);
  void setLayoutPadding(float padding, int index);
  void setLayoutPosition(float position, int index);
  void setPosition(
      const BNDYGDirection direction,
      const float mainSize,
      const float crossSize,
      const float ownerWidth);
  void setAndPropogateUseLegacyFlag(bool useLegacyFlag);
  void setLayoutDoesLegacyFlagAffectsLayout(bool doesLegacyFlagAffectsLayout);
  void setLayoutDidUseLegacyFlag(bool didUseLegacyFlag);
  void markDirtyAndPropogateDownwards();

  // Other methods
  BNDYGValue marginLeadingValue(const BNDYGFlexDirection axis) const;
  BNDYGValue marginTrailingValue(const BNDYGFlexDirection axis) const;
  BNDYGValue resolveFlexBasisPtr() const;
  void resolveDimension();
  BNDYGDirection resolveDirection(const BNDYGDirection ownerDirection);
  void clearChildren();
  /// Replaces the occurrences of oldChild with newChild
  void replaceChild(BNDYGNodeRef oldChild, BNDYGNodeRef newChild);
  void replaceChild(BNDYGNodeRef child, uint32_t index);
  void insertChild(BNDYGNodeRef child, uint32_t index);
  /// Removes the first occurrence of child
  bool removeChild(BNDYGNodeRef child);
  void removeChild(uint32_t index);

  void cloneChildrenIfNeeded();
  void markDirtyAndPropogate();
  float resolveFlexGrow();
  float resolveFlexShrink();
  bool isNodeFlexible();
  bool didUseLegacyFlag();
  bool isLayoutTreeEqualToNode(const BNDYGNode& node) const;
};
