/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#import <UIKit/UIKit.h>
#import <bindyoga/BNDYGEnums.h>
#import <bindyoga/BindYoga.h>
#import <bindyoga/BNDYGMacros.h>

BNDYG_EXTERN_C_BEGIN

extern BNDYGValue BNDYGPointValue(CGFloat value)
    NS_SWIFT_UNAVAILABLE("Use the swift Int and FloatingPoint extensions instead");
extern BNDYGValue BNDYGPercentValue(CGFloat value)
    NS_SWIFT_UNAVAILABLE("Use the swift Int and FloatingPoint extensions instead");

BNDYG_EXTERN_C_END

typedef NS_OPTIONS(NSInteger, BNDYGDimensionFlexibility) {
  BNDYGDimensionFlexibilityFlexibleWidth = 1 << 0,
  BNDYGDimensionFlexibilityFlexibleHeight = 1 << 1,
};

@interface BNDYGLayout : NSObject

/**
 Make default init unavailable, as it will not initialise BNDYGNode which is
 required for the setters and getters of BNDYGLayout's properties to work properly.
*/
- (instancetype)init
    __attribute__((unavailable("you are not meant to initialise BNDYGLayout")));

/**
  The property that decides if we should include this view when calculating
  layout. Defaults to YES.
 */
@property (nonatomic, readwrite, assign, setter=setIncludedInLayout:) BOOL isIncludedInLayout;

/**
 The property that decides during layout/sizing whether or not styling properties should be applied.
 Defaults to NO.
 */
@property (nonatomic, readwrite, assign, setter=setEnabled:) BOOL isEnabled;

@property (nonatomic, readwrite, assign) BNDYGDirection direction;
@property (nonatomic, readwrite, assign) BNDYGFlexDirection flexDirection;
@property (nonatomic, readwrite, assign) BNDYGJustify justifyContent;
@property (nonatomic, readwrite, assign) BNDYGAlign alignContent;
@property (nonatomic, readwrite, assign) BNDYGAlign alignItems;
@property (nonatomic, readwrite, assign) BNDYGAlign alignSelf;
@property (nonatomic, readwrite, assign) BNDYGPositionType position;
@property (nonatomic, readwrite, assign) BNDYGWrap flexWrap;
@property (nonatomic, readwrite, assign) BNDYGOverflow overflow;
@property (nonatomic, readwrite, assign) BNDYGDisplay display;

@property (nonatomic, readwrite, assign) CGFloat flexGrow;
@property (nonatomic, readwrite, assign) CGFloat flexShrink;
@property (nonatomic, readwrite, assign) BNDYGValue flexBasis;

@property (nonatomic, readwrite, assign) BNDYGValue left;
@property (nonatomic, readwrite, assign) BNDYGValue top;
@property (nonatomic, readwrite, assign) BNDYGValue right;
@property (nonatomic, readwrite, assign) BNDYGValue bottom;
@property (nonatomic, readwrite, assign) BNDYGValue start;
@property (nonatomic, readwrite, assign) BNDYGValue end;

@property (nonatomic, readwrite, assign) BNDYGValue marginLeft;
@property (nonatomic, readwrite, assign) BNDYGValue marginTop;
@property (nonatomic, readwrite, assign) BNDYGValue marginRight;
@property (nonatomic, readwrite, assign) BNDYGValue marginBottom;
@property (nonatomic, readwrite, assign) BNDYGValue marginStart;
@property (nonatomic, readwrite, assign) BNDYGValue marginEnd;
@property (nonatomic, readwrite, assign) BNDYGValue marginHorizontal;
@property (nonatomic, readwrite, assign) BNDYGValue marginVertical;
@property (nonatomic, readwrite, assign) BNDYGValue margin;

@property (nonatomic, readwrite, assign) BNDYGValue paddingLeft;
@property (nonatomic, readwrite, assign) BNDYGValue paddingTop;
@property (nonatomic, readwrite, assign) BNDYGValue paddingRight;
@property (nonatomic, readwrite, assign) BNDYGValue paddingBottom;
@property (nonatomic, readwrite, assign) BNDYGValue paddingStart;
@property (nonatomic, readwrite, assign) BNDYGValue paddingEnd;
@property (nonatomic, readwrite, assign) BNDYGValue paddingHorizontal;
@property (nonatomic, readwrite, assign) BNDYGValue paddingVertical;
@property (nonatomic, readwrite, assign) BNDYGValue padding;

@property (nonatomic, readwrite, assign) CGFloat borderLeftWidth;
@property (nonatomic, readwrite, assign) CGFloat borderTopWidth;
@property (nonatomic, readwrite, assign) CGFloat borderRightWidth;
@property (nonatomic, readwrite, assign) CGFloat borderBottomWidth;
@property (nonatomic, readwrite, assign) CGFloat borderStartWidth;
@property (nonatomic, readwrite, assign) CGFloat borderEndWidth;
@property (nonatomic, readwrite, assign) CGFloat borderWidth;

@property (nonatomic, readwrite, assign) BNDYGValue width;
@property (nonatomic, readwrite, assign) BNDYGValue height;
@property (nonatomic, readwrite, assign) BNDYGValue minWidth;
@property (nonatomic, readwrite, assign) BNDYGValue minHeight;
@property (nonatomic, readwrite, assign) BNDYGValue maxWidth;
@property (nonatomic, readwrite, assign) BNDYGValue maxHeight;

// Yoga specific properties, not compatible with flexbox specification
@property (nonatomic, readwrite, assign) CGFloat aspectRatio;

/**
 Get the resolved direction of this node. This won't be BNDYGDirectionInherit
 */
@property (nonatomic, readonly, assign) BNDYGDirection resolvedDirection;

/**
 Perform a layout calculation and update the frames of the views in the hierarchy with the results.
 If the origin is not preserved, the root view's layout results will applied from {0,0}.
 */
- (void)applyLayoutPreservingOrigin:(BOOL)preserveOrigin
    NS_SWIFT_NAME(applyLayout(preservingOrigin:));

/**
 Perform a layout calculation and update the frames of the views in the hierarchy with the results.
 If the origin is not preserved, the root view's layout results will applied from {0,0}.
 */
- (void)applyLayoutPreservingOrigin:(BOOL)preserveOrigin
               dimensionFlexibility:(BNDYGDimensionFlexibility)dimensionFlexibility
    NS_SWIFT_NAME(applyLayout(preservingOrigin:dimensionFlexibility:));

/**
 Returns the size of the view if no constraints were given. This could equivalent to calling [self
 sizeThatFits:CGSizeMake(CGFLOAT_MAX, CGFLOAT_MAX)];
 */
@property (nonatomic, readonly, assign) CGSize intrinsicSize;

/**
  Returns the size of the view based on provided constraints. Pass NaN for an unconstrained dimension.
 */
- (CGSize)calculateLayoutWithSize:(CGSize)size
    NS_SWIFT_NAME(calculateLayout(with:));

/**
 Returns the number of children that are using Flexbox.
 */
@property (nonatomic, readonly, assign) NSUInteger numberOfChildren;

/**
 Return a BOOL indiciating whether or not we this node contains any subviews that are included in
 Yoga's layout.
 */
@property (nonatomic, readonly, assign) BOOL isLeaf;

/**
 Return's a BOOL indicating if a view is dirty. When a node is dirty
 it usually indicates that it will be remeasured on the next layout pass.
 */
@property (nonatomic, readonly, assign) BOOL isDirty;

/**
 Mark that a view's layout needs to be recalculated. Only works for leaf views.
 */
- (void)markDirty;

@end
