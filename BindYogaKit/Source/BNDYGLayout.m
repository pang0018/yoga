/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#import "BNDYGLayout+Private.h"
#import "UIView+BindYoga.h"

#define BNDYG_PROPERTY(type, lowercased_name, capitalized_name)    \
- (type)lowercased_name                                         \
{                                                               \
  return BNDYGNodeStyleGet##capitalized_name(self.node);           \
}                                                               \
                                                                \
- (void)set##capitalized_name:(type)lowercased_name             \
{                                                               \
  BNDYGNodeStyleSet##capitalized_name(self.node, lowercased_name); \
}

#define BNDYG_VALUE_PROPERTY(lowercased_name, capitalized_name)                       \
- (BNDYGValue)lowercased_name                                                         \
{                                                                                  \
  return BNDYGNodeStyleGet##capitalized_name(self.node);                              \
}                                                                                  \
                                                                                   \
- (void)set##capitalized_name:(BNDYGValue)lowercased_name                             \
{                                                                                  \
  switch (lowercased_name.unit) {                                                  \
     case BNDYGUnitUndefined:                                                         \
      BNDYGNodeStyleSet##capitalized_name(self.node, lowercased_name.value);          \
      break;                                                                       \
    case BNDYGUnitPoint:                                                              \
      BNDYGNodeStyleSet##capitalized_name(self.node, lowercased_name.value);          \
      break;                                                                       \
    case BNDYGUnitPercent:                                                            \
      BNDYGNodeStyleSet##capitalized_name##Percent(self.node, lowercased_name.value); \
      break;                                                                       \
    default:                                                                       \
      NSAssert(NO, @"Not implemented");                                            \
  }                                                                                \
}

#define BNDYG_AUTO_VALUE_PROPERTY(lowercased_name, capitalized_name)                  \
- (BNDYGValue)lowercased_name                                                         \
{                                                                                  \
  return BNDYGNodeStyleGet##capitalized_name(self.node);                              \
}                                                                                  \
                                                                                   \
- (void)set##capitalized_name:(BNDYGValue)lowercased_name                             \
{                                                                                  \
  switch (lowercased_name.unit) {                                                  \
    case BNDYGUnitPoint:                                                              \
      BNDYGNodeStyleSet##capitalized_name(self.node, lowercased_name.value);          \
      break;                                                                       \
    case BNDYGUnitPercent:                                                            \
      BNDYGNodeStyleSet##capitalized_name##Percent(self.node, lowercased_name.value); \
      break;                                                                       \
    case BNDYGUnitAuto:                                                               \
      BNDYGNodeStyleSet##capitalized_name##Auto(self.node);                           \
      break;                                                                       \
    default:                                                                       \
      NSAssert(NO, @"Not implemented");                                            \
  }                                                                                \
}

#define BNDYG_EDGE_PROPERTY_GETTER(type, lowercased_name, capitalized_name, property, edge) \
- (type)lowercased_name                                                                  \
{                                                                                        \
  return BNDYGNodeStyleGet##property(self.node, edge);                                      \
}

#define BNDYG_EDGE_PROPERTY_SETTER(lowercased_name, capitalized_name, property, edge) \
- (void)set##capitalized_name:(CGFloat)lowercased_name                             \
{                                                                                  \
  BNDYGNodeStyleSet##property(self.node, edge, lowercased_name);                      \
}

#define BNDYG_EDGE_PROPERTY(lowercased_name, capitalized_name, property, edge)         \
BNDYG_EDGE_PROPERTY_GETTER(CGFloat, lowercased_name, capitalized_name, property, edge) \
BNDYG_EDGE_PROPERTY_SETTER(lowercased_name, capitalized_name, property, edge)

#define BNDYG_VALUE_EDGE_PROPERTY_SETTER(objc_lowercased_name, objc_capitalized_name, c_name, edge) \
- (void)set##objc_capitalized_name:(BNDYGValue)objc_lowercased_name                                 \
{                                                                                                \
  switch (objc_lowercased_name.unit) {                                                           \
    case BNDYGUnitUndefined:                                                                        \
      BNDYGNodeStyleSet##c_name(self.node, edge, objc_lowercased_name.value);                       \
      break;                                                                                     \
    case BNDYGUnitPoint:                                                                            \
      BNDYGNodeStyleSet##c_name(self.node, edge, objc_lowercased_name.value);                       \
      break;                                                                                     \
    case BNDYGUnitPercent:                                                                          \
      BNDYGNodeStyleSet##c_name##Percent(self.node, edge, objc_lowercased_name.value);              \
      break;                                                                                     \
    default:                                                                                     \
      NSAssert(NO, @"Not implemented");                                                          \
  }                                                                                              \
}

#define BNDYG_VALUE_EDGE_PROPERTY(lowercased_name, capitalized_name, property, edge)   \
BNDYG_EDGE_PROPERTY_GETTER(BNDYGValue, lowercased_name, capitalized_name, property, edge) \
BNDYG_VALUE_EDGE_PROPERTY_SETTER(lowercased_name, capitalized_name, property, edge)

#define BNDYG_VALUE_EDGES_PROPERTIES(lowercased_name, capitalized_name)                                                  \
BNDYG_VALUE_EDGE_PROPERTY(lowercased_name##Left, capitalized_name##Left, capitalized_name, BNDYGEdgeLeft)                   \
BNDYG_VALUE_EDGE_PROPERTY(lowercased_name##Top, capitalized_name##Top, capitalized_name, BNDYGEdgeTop)                      \
BNDYG_VALUE_EDGE_PROPERTY(lowercased_name##Right, capitalized_name##Right, capitalized_name, BNDYGEdgeRight)                \
BNDYG_VALUE_EDGE_PROPERTY(lowercased_name##Bottom, capitalized_name##Bottom, capitalized_name, BNDYGEdgeBottom)             \
BNDYG_VALUE_EDGE_PROPERTY(lowercased_name##Start, capitalized_name##Start, capitalized_name, BNDYGEdgeStart)                \
BNDYG_VALUE_EDGE_PROPERTY(lowercased_name##End, capitalized_name##End, capitalized_name, BNDYGEdgeEnd)                      \
BNDYG_VALUE_EDGE_PROPERTY(lowercased_name##Horizontal, capitalized_name##Horizontal, capitalized_name, BNDYGEdgeHorizontal) \
BNDYG_VALUE_EDGE_PROPERTY(lowercased_name##Vertical, capitalized_name##Vertical, capitalized_name, BNDYGEdgeVertical)       \
BNDYG_VALUE_EDGE_PROPERTY(lowercased_name, capitalized_name, capitalized_name, BNDYGEdgeAll)

BNDYGValue BNDYGPointValue(CGFloat value)
{
  return (BNDYGValue) { .value = value, .unit = BNDYGUnitPoint };
}

BNDYGValue BNDYGPercentValue(CGFloat value)
{
  return (BNDYGValue) { .value = value, .unit = BNDYGUnitPercent };
}

static BNDYGConfigRef globalConfig;

@interface BNDYGLayout ()

@property (nonatomic, weak, readonly) UIView *view;

@end

@implementation BNDYGLayout

@synthesize isEnabled=_isEnabled;
@synthesize isIncludedInLayout=_isIncludedInLayout;
@synthesize node=_node;

+ (void)initialize
{
  globalConfig = BNDYGConfigNew();
  BNDYGConfigSetExperimentalFeatureEnabled(globalConfig, BNDYGExperimentalFeatureWebFlexBasis, true);
  BNDYGConfigSetPointScaleFactor(globalConfig, [UIScreen mainScreen].scale);
}

- (instancetype)initWithView:(UIView*)view
{
  if (self = [super init]) {
    _view = view;
    _node = BNDYGNodeNewWithConfig(globalConfig);
    BNDYGNodeSetContext(_node, (__bridge void *) view);
    _isEnabled = NO;
    _isIncludedInLayout = YES;
  }

  return self;
}

- (void)dealloc
{
  BNDYGNodeFree(self.node);
}

- (BOOL)isDirty
{
  return BNDYGNodeIsDirty(self.node);
}

- (void)markDirty
{
  if (self.isDirty || !self.isLeaf) {
    return;
  }

  // Yoga is not happy if we try to mark a node as "dirty" before we have set
  // the measure function. Since we already know that this is a leaf,
  // this *should* be fine. Forgive me Hack Gods.
  const BNDYGNodeRef node = self.node;
  if (BNDYGNodeGetMeasureFunc(node) == NULL) {
    BNDYGNodeSetMeasureFunc(node, BNDYGMeasureView);
  }

  BNDYGNodeMarkDirty(node);
}

- (NSUInteger)numberOfChildren
{
  return BNDYGNodeGetChildCount(self.node);
}

- (BOOL)isLeaf
{
  NSAssert([NSThread isMainThread], @"This method must be called on the main thread.");
  if (self.isEnabled) {
    for (UIView *subview in self.view.subviews) {
      BNDYGLayout *const yoga = subview.yoga;
      if (yoga.isEnabled && yoga.isIncludedInLayout) {
        return NO;
      }
    }
  }

  return YES;
}

#pragma mark - Style

- (BNDYGPositionType)position
{
  return BNDYGNodeStyleGetPositionType(self.node);
}

- (void)setPosition:(BNDYGPositionType)position
{
  BNDYGNodeStyleSetPositionType(self.node, position);
}

BNDYG_PROPERTY(BNDYGDirection, direction, Direction)
BNDYG_PROPERTY(BNDYGFlexDirection, flexDirection, FlexDirection)
BNDYG_PROPERTY(BNDYGJustify, justifyContent, JustifyContent)
BNDYG_PROPERTY(BNDYGAlign, alignContent, AlignContent)
BNDYG_PROPERTY(BNDYGAlign, alignItems, AlignItems)
BNDYG_PROPERTY(BNDYGAlign, alignSelf, AlignSelf)
BNDYG_PROPERTY(BNDYGWrap, flexWrap, FlexWrap)
BNDYG_PROPERTY(BNDYGOverflow, overflow, Overflow)
BNDYG_PROPERTY(BNDYGDisplay, display, Display)

BNDYG_PROPERTY(CGFloat, flexGrow, FlexGrow)
BNDYG_PROPERTY(CGFloat, flexShrink, FlexShrink)
BNDYG_AUTO_VALUE_PROPERTY(flexBasis, FlexBasis)

BNDYG_VALUE_EDGE_PROPERTY(left, Left, Position, BNDYGEdgeLeft)
BNDYG_VALUE_EDGE_PROPERTY(top, Top, Position, BNDYGEdgeTop)
BNDYG_VALUE_EDGE_PROPERTY(right, Right, Position, BNDYGEdgeRight)
BNDYG_VALUE_EDGE_PROPERTY(bottom, Bottom, Position, BNDYGEdgeBottom)
BNDYG_VALUE_EDGE_PROPERTY(start, Start, Position, BNDYGEdgeStart)
BNDYG_VALUE_EDGE_PROPERTY(end, End, Position, BNDYGEdgeEnd)
BNDYG_VALUE_EDGES_PROPERTIES(margin, Margin)
BNDYG_VALUE_EDGES_PROPERTIES(padding, Padding)

BNDYG_EDGE_PROPERTY(borderLeftWidth, BorderLeftWidth, Border, BNDYGEdgeLeft)
BNDYG_EDGE_PROPERTY(borderTopWidth, BorderTopWidth, Border, BNDYGEdgeTop)
BNDYG_EDGE_PROPERTY(borderRightWidth, BorderRightWidth, Border, BNDYGEdgeRight)
BNDYG_EDGE_PROPERTY(borderBottomWidth, BorderBottomWidth, Border, BNDYGEdgeBottom)
BNDYG_EDGE_PROPERTY(borderStartWidth, BorderStartWidth, Border, BNDYGEdgeStart)
BNDYG_EDGE_PROPERTY(borderEndWidth, BorderEndWidth, Border, BNDYGEdgeEnd)
BNDYG_EDGE_PROPERTY(borderWidth, BorderWidth, Border, BNDYGEdgeAll)

BNDYG_AUTO_VALUE_PROPERTY(width, Width)
BNDYG_AUTO_VALUE_PROPERTY(height, Height)
BNDYG_VALUE_PROPERTY(minWidth, MinWidth)
BNDYG_VALUE_PROPERTY(minHeight, MinHeight)
BNDYG_VALUE_PROPERTY(maxWidth, MaxWidth)
BNDYG_VALUE_PROPERTY(maxHeight, MaxHeight)
BNDYG_PROPERTY(CGFloat, aspectRatio, AspectRatio)

#pragma mark - Layout and Sizing

- (BNDYGDirection)resolvedDirection
{
  return BNDYGNodeLayoutGetDirection(self.node);
}

- (void)applyLayout
{
  [self calculateLayoutWithSize:self.view.bounds.size];
  BNDYGApplyLayoutToViewHierarchy(self.view, NO);
}

- (void)applyLayoutPreservingOrigin:(BOOL)preserveOrigin
{
  [self calculateLayoutWithSize:self.view.bounds.size];
  BNDYGApplyLayoutToViewHierarchy(self.view, preserveOrigin);
}

- (void)applyLayoutPreservingOrigin:(BOOL)preserveOrigin dimensionFlexibility:(BNDYGDimensionFlexibility)dimensionFlexibility
{
  CGSize size = self.view.bounds.size;
  if (dimensionFlexibility & BNDYGDimensionFlexibilityFlexibleWidth) {
    size.width = BNDYGUndefined;
  }
  if (dimensionFlexibility & BNDYGDimensionFlexibilityFlexibleHeight) {
    size.height = BNDYGUndefined;
  }
  [self calculateLayoutWithSize:size];
  BNDYGApplyLayoutToViewHierarchy(self.view, preserveOrigin);
}


- (CGSize)intrinsicSize
{
  const CGSize constrainedSize = {
    .width = BNDYGUndefined,
    .height = BNDYGUndefined,
  };
  return [self calculateLayoutWithSize:constrainedSize];
}

- (CGSize)calculateLayoutWithSize:(CGSize)size
{
  NSAssert([NSThread isMainThread], @"Yoga calculation must be done on main.");
  NSAssert(self.isEnabled, @"Yoga is not enabled for this view.");

  BNDYGAttachNodesFromViewHierachy(self.view);

  const BNDYGNodeRef node = self.node;
  BNDYGNodeCalculateLayout(
    node,
    size.width,
    size.height,
    BNDYGNodeStyleGetDirection(node));

  return (CGSize) {
    .width = BNDYGNodeLayoutGetWidth(node),
    .height = BNDYGNodeLayoutGetHeight(node),
  };
}

#pragma mark - Private

static BNDYGSize BNDYGMeasureView(
  BNDYGNodeRef node,
  float width,
  BNDYGMeasureMode widthMode,
  float height,
  BNDYGMeasureMode heightMode)
{
  const CGFloat constrainedWidth = (widthMode == BNDYGMeasureModeUndefined) ? CGFLOAT_MAX : width;
  const CGFloat constrainedHeight = (heightMode == BNDYGMeasureModeUndefined) ? CGFLOAT_MAX: height;

  UIView *view = (__bridge UIView*) BNDYGNodeGetContext(node);
  const CGSize sizeThatFits = [view sizeThatFits:(CGSize) {
    .width = constrainedWidth,
    .height = constrainedHeight,
  }];

  return (BNDYGSize) {
    .width = BNDYGSanitizeMeasurement(constrainedWidth, sizeThatFits.width, widthMode),
    .height = BNDYGSanitizeMeasurement(constrainedHeight, sizeThatFits.height, heightMode),
  };
}

static CGFloat BNDYGSanitizeMeasurement(
  CGFloat constrainedSize,
  CGFloat measuredSize,
  BNDYGMeasureMode measureMode)
{
  CGFloat result;
  if (measureMode == BNDYGMeasureModeExactly) {
    result = constrainedSize;
  } else if (measureMode == BNDYGMeasureModeAtMost) {
    result = MIN(constrainedSize, measuredSize);
  } else {
    result = measuredSize;
  }

  return result;
}

static BOOL BNDYGNodeHasExactSameChildren(const BNDYGNodeRef node, NSArray<UIView *> *subviews)
{
  if (BNDYGNodeGetChildCount(node) != subviews.count) {
    return NO;
  }

  for (int i=0; i<subviews.count; i++) {
    if (BNDYGNodeGetChild(node, i) != subviews[i].yoga.node) {
      return NO;
    }
  }

  return YES;
}

static void BNDYGAttachNodesFromViewHierachy(UIView *const view)
{
  BNDYGLayout *const yoga = view.yoga;
  const BNDYGNodeRef node = yoga.node;

  // Only leaf nodes should have a measure function
  if (yoga.isLeaf) {
    BNDYGRemoveAllChildren(node);
    BNDYGNodeSetMeasureFunc(node, BNDYGMeasureView);
  } else {
    BNDYGNodeSetMeasureFunc(node, NULL);

    NSMutableArray<UIView *> *subviewsToInclude = [[NSMutableArray alloc] initWithCapacity:view.subviews.count];
    for (UIView *subview in view.subviews) {
      if (subview.yoga.isEnabled && subview.yoga.isIncludedInLayout) {
        [subviewsToInclude addObject:subview];
      }
    }

    if (!BNDYGNodeHasExactSameChildren(node, subviewsToInclude)) {
      BNDYGRemoveAllChildren(node);
      for (int i=0; i<subviewsToInclude.count; i++) {
        BNDYGNodeInsertChild(node, subviewsToInclude[i].yoga.node, i);
      }
    }

    for (UIView *const subview in subviewsToInclude) {
      BNDYGAttachNodesFromViewHierachy(subview);
    }
  }
}

static void BNDYGRemoveAllChildren(const BNDYGNodeRef node)
{
  if (node == NULL) {
    return;
  }

  while (BNDYGNodeGetChildCount(node) > 0) {
    BNDYGNodeRemoveChild(node, BNDYGNodeGetChild(node, BNDYGNodeGetChildCount(node) - 1));
  }
}

static CGFloat BNDYGRoundPixelValue(CGFloat value)
{
  static CGFloat scale;
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^(){
    scale = [UIScreen mainScreen].scale;
  });

  return roundf(value * scale) / scale;
}

static void BNDYGApplyLayoutToViewHierarchy(UIView *view, BOOL preserveOrigin)
{
  NSCAssert([NSThread isMainThread], @"Framesetting should only be done on the main thread.");

  const BNDYGLayout *yoga = view.yoga;

  if (!yoga.isIncludedInLayout) {
     return;
  }

  BNDYGNodeRef node = yoga.node;
  const CGPoint topLeft = {
    BNDYGNodeLayoutGetLeft(node),
    BNDYGNodeLayoutGetTop(node),
  };

  const CGPoint bottomRight = {
    topLeft.x + BNDYGNodeLayoutGetWidth(node),
    topLeft.y + BNDYGNodeLayoutGetHeight(node),
  };

  const CGPoint origin = preserveOrigin ? view.frame.origin : CGPointZero;
  view.frame = (CGRect) {
    .origin = {
      .x = BNDYGRoundPixelValue(topLeft.x + origin.x),
      .y = BNDYGRoundPixelValue(topLeft.y + origin.y),
    },
    .size = {
      .width = BNDYGRoundPixelValue(bottomRight.x) - BNDYGRoundPixelValue(topLeft.x),
      .height = BNDYGRoundPixelValue(bottomRight.y) - BNDYGRoundPixelValue(topLeft.y),
    },
  };

  if (!yoga.isLeaf) {
    for (NSUInteger i=0; i<view.subviews.count; i++) {
      BNDYGApplyLayoutToViewHierarchy(view.subviews[i], NO);
    }
  }
}

@end
