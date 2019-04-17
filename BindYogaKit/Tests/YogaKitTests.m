/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#import <XCTest/XCTest.h>

#import <BindYogaKit/UIView+BindYoga.h>
#import <BindYogaKit/BNDYGLayout+Private.h>
#import <bindyoga/BindYoga.h>

@interface BindYogaKitTests : XCTestCase
@end

@implementation BindYogaKitTests

- (void)testConfigureLayoutIsNoOpWithNilBlock
{
  UIView *view = [[UIView alloc] initWithFrame:CGRectZero];
  id block = nil;
  XCTAssertNoThrow([view configureLayoutWithBlock:block]);
}

- (void)testConfigureLayoutBlockWorksWithValidBlock
{
  UIView *view = [[UIView alloc] initWithFrame:CGRectZero];
  [view configureLayoutWithBlock:^(BNDYGLayout *layout){
    XCTAssertNotNil(layout);
    layout.isEnabled = YES;
    layout.width = BNDYGPointValue(25);
  }];

  XCTAssertTrue(view.yoga.isEnabled);
  XCTAssertEqual(view.yoga.width.value, 25);
}

- (void)testNodesAreDeallocedWithSingleView
{
  __weak BNDYGLayout *layoutRef = nil;

  @autoreleasepool {
    UIView *view = [[UIView alloc] initWithFrame:CGRectZero];
    view.yoga.flexBasis = BNDYGPointValue(1);

    layoutRef = view.yoga;
    XCTAssertNotNil(layoutRef);

    view = nil;
  }

  XCTAssertNil(layoutRef);
}

- (void)testNodesAreDeallocedCascade
{
  __weak BNDYGLayout *topLayout = nil;
  __weak BNDYGLayout *subviewLayout = nil;

  @autoreleasepool {
    UIView *view = [[UIView alloc] initWithFrame:CGRectZero];
    topLayout = view.yoga;
    topLayout.flexBasis = BNDYGPointValue(1);

    UIView *subview = [[UIView alloc] initWithFrame:CGRectZero];
    subviewLayout = subview.yoga;
    subviewLayout.flexBasis = BNDYGPointValue(1);

    view = nil;
  }

  XCTAssertNil(topLayout);
  XCTAssertNil(subviewLayout);
}

- (void)testIsEnabled
{
  UIView *view = [[UIView alloc] initWithFrame:CGRectZero];
  XCTAssertFalse(view.yoga.isEnabled);

  view.yoga.isEnabled = YES;
  XCTAssertTrue(view.yoga.isEnabled);

  view.yoga.isEnabled = NO;
  XCTAssertFalse(view.yoga.isEnabled);
}

- (void)testSizeThatFitsAsserts
{
  UIView *view = [[UIView alloc] initWithFrame:CGRectZero];
  dispatch_sync(dispatch_queue_create("com.facebook.Yoga.testing", DISPATCH_QUEUE_SERIAL), ^(void){
    XCTAssertThrows(view.yoga.intrinsicSize);
  });
}

- (void)testSizeThatFitsSmoke
{
  UIView *container = [[UIView alloc] initWithFrame:CGRectZero];
  container.yoga.isEnabled = YES;
  container.yoga.flexDirection = BNDYGFlexDirectionRow;
  container.yoga.alignItems = BNDYGAlignFlexStart;

  UILabel *longTextLabel = [[UILabel alloc] initWithFrame:CGRectZero];
  longTextLabel.text = @"This is a very very very very very very very very long piece of text.";
  longTextLabel.lineBreakMode = NSLineBreakByTruncatingTail;
  longTextLabel.numberOfLines = 1;
  longTextLabel.yoga.isEnabled = YES;
  longTextLabel.yoga.flexShrink = 1;
  [container addSubview:longTextLabel];

  UIView *textBadgeView = [[UIView alloc] initWithFrame:CGRectZero];
  textBadgeView.yoga.isEnabled = YES;
  textBadgeView.yoga.margin = BNDYGPointValue(0);
  textBadgeView.yoga.width = BNDYGPointValue(10);
  textBadgeView.yoga.height = BNDYGPointValue(10);
  [container addSubview:textBadgeView];

  const CGSize textBadgeViewSize = textBadgeView.yoga.intrinsicSize;
  XCTAssertEqual(textBadgeViewSize.height, 10);
  XCTAssertEqual(textBadgeViewSize.width, 10);

  const CGSize containerSize = container.yoga.intrinsicSize;
  const CGSize longTextLabelSize = longTextLabel.yoga.intrinsicSize;

  XCTAssertEqual(longTextLabelSize.height, containerSize.height);
  XCTAssertEqual(longTextLabelSize.width + textBadgeView.yoga.intrinsicSize.width, containerSize.width);
}

- (void)testPreservingOrigin
{
  UIView *container = [[UIView alloc] initWithFrame:CGRectMake(0,0,50,75)];
  container.yoga.isEnabled = YES;

  UIView *view = [[UIView alloc] initWithFrame:CGRectZero];
  view.yoga.isEnabled = YES;
  view.yoga.flexBasis = BNDYGPointValue(0);
  view.yoga.flexGrow = 1;
  [container addSubview:view];

  UIView *view2 = [[UIView alloc] initWithFrame:CGRectZero];
  view2.yoga.isEnabled = YES;
  view2.yoga.marginTop = BNDYGPointValue(25);
  view2.yoga.flexBasis = BNDYGPointValue(0);
  view2.yoga.flexGrow = 1;
  [container addSubview:view2];

  [container.yoga applyLayoutPreservingOrigin:YES];
  XCTAssertEqual(50, view2.frame.origin.y);

  [view2.yoga applyLayoutPreservingOrigin:NO];
  XCTAssertEqual(25, view2.frame.origin.y);
}

- (void)testContainerWithFlexibleWidthGetsCorrectlySized
{
  UIView *container = [[UIView alloc] initWithFrame:CGRectMake(0,0,200,200)];
  container.yoga.isEnabled = YES;

  UIView *view = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 100, 100)];
  view.yoga.isEnabled = YES;
  view.yoga.width = BNDYGPointValue(100);
  view.yoga.height = BNDYGPointValue(100);
  [container addSubview:view];

  [container.yoga applyLayoutPreservingOrigin:YES dimensionFlexibility:BNDYGDimensionFlexibilityFlexibleWidth];
  XCTAssertEqual(100, container.frame.size.width);
  XCTAssertEqual(200, container.frame.size.height);
}

- (void)testContainerWithFlexibleHeightGetsCorrectlySized
{
  UIView *container = [[UIView alloc] initWithFrame:CGRectMake(0,0,200,200)];
  container.yoga.isEnabled = YES;

  UIView *view = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 100, 100)];
  view.yoga.isEnabled = YES;
  view.yoga.width = BNDYGPointValue(100);
  view.yoga.height = BNDYGPointValue(100);
  [container addSubview:view];

  [container.yoga
      applyLayoutPreservingOrigin:YES
             dimensionFlexibility:BNDYGDimensionFlexibilityFlexibleHeight];
  XCTAssertEqual(200, container.frame.size.width);
  XCTAssertEqual(100, container.frame.size.height);
}

- (void)testContainerWithFlexibleWidthAndHeightGetsCorrectlySized
{
  UIView *container = [[UIView alloc] initWithFrame:CGRectMake(0,0,200,200)];
  container.yoga.isEnabled = YES;

  UIView *view = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 100, 100)];
  view.yoga.isEnabled = YES;
  view.yoga.width = BNDYGPointValue(100);
  view.yoga.height = BNDYGPointValue(100);
  [container addSubview:view];

  [container.yoga
      applyLayoutPreservingOrigin:YES
             dimensionFlexibility:BNDYGDimensionFlexibilityFlexibleWidth |
             BNDYGDimensionFlexibilityFlexibleHeight];
  XCTAssertEqual(100, container.frame.size.width);
  XCTAssertEqual(100, container.frame.size.height);
}

- (void)testMarkingDirtyOnlyWorksOnLeafNodes
{
  UIView *container = [[UIView alloc] initWithFrame:CGRectZero];
  container.yoga.isEnabled = YES;

  UIView *subview = [[UIView alloc] initWithFrame:CGRectZero];
  subview.yoga.isEnabled = YES;
  [container addSubview:subview];

  XCTAssertFalse(container.yoga.isDirty);
  [container.yoga markDirty];
  XCTAssertFalse(container.yoga.isDirty);

  XCTAssertFalse(subview.yoga.isDirty);
  [subview.yoga markDirty];
  XCTAssertTrue(subview.yoga.isDirty);
}

- (void)testThatMarkingLeafsAsDirtyWillTriggerASizeRecalculation
{
  UIView *container = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 500, 50)];
  container.yoga.isEnabled = YES;
  container.yoga.flexDirection = BNDYGFlexDirectionRow;
  container.yoga.alignItems = BNDYGAlignFlexStart;

  UILabel *view = [[UILabel alloc] initWithFrame:CGRectZero];
  view.text = @"This is a short text.";
  view.numberOfLines = 1;
  view.yoga.isEnabled = YES;
  [container addSubview:view];

  [container.yoga applyLayoutPreservingOrigin:YES];
  CGSize const viewSizeAfterFirstPass = view.frame.size;

  view.text = @"This is a slightly longer text.";
  XCTAssertTrue(CGSizeEqualToSize(view.frame.size, viewSizeAfterFirstPass));

  [view.yoga markDirty];

  [container.yoga applyLayoutPreservingOrigin:YES];
  XCTAssertFalse(CGSizeEqualToSize(view.frame.size, viewSizeAfterFirstPass));
}

- (void)testFrameAndOriginPlacement
{
  const CGSize containerSize = CGSizeMake(320, 50);

  UIView *container = [[UIView alloc] initWithFrame:CGRectMake(0, 0, containerSize.width, containerSize.height)];
  container.yoga.isEnabled = YES;
  container.yoga.flexDirection = BNDYGFlexDirectionRow;

  UIView *subview1 = [[UIView alloc] initWithFrame:CGRectZero];
  subview1.yoga.isEnabled = YES;
  subview1.yoga.flexGrow = 1;
  [container addSubview:subview1];

  UIView *subview2 = [[UIView alloc] initWithFrame:CGRectZero];
  subview2.yoga.isEnabled = YES;
  subview2.yoga.flexGrow = 1;
  [container addSubview:subview2];

  UIView *subview3 = [[UIView alloc] initWithFrame:CGRectZero];
  subview3.yoga.isEnabled = YES;
  subview3.yoga.flexGrow = 1;
  [container addSubview:subview3];

  [container.yoga applyLayoutPreservingOrigin:YES];

  XCTAssertEqualWithAccuracy(subview2.frame.origin.x, CGRectGetMaxX(subview1.frame), FLT_EPSILON);
  XCTAssertEqualWithAccuracy(subview3.frame.origin.x, CGRectGetMaxX(subview2.frame), FLT_EPSILON);

  CGFloat totalWidth = 0;
  for (UIView *view in container.subviews) {
    totalWidth += view.bounds.size.width;
  }

  XCTAssertEqual(containerSize.width, totalWidth, @"The container's width is %.6f, the subviews take up %.6f", containerSize.width, totalWidth);
}

- (void)testThatLayoutIsCorrectWhenWeSwapViewOrder
{
  const CGSize containerSize = CGSizeMake(300, 50);

  UIView *container = [[UIView alloc] initWithFrame:CGRectMake(0, 0, containerSize.width, containerSize.height)];
  container.yoga.isEnabled = YES;
  container.yoga.flexDirection = BNDYGFlexDirectionRow;

  UIView *subview1 = [[UIView alloc] initWithFrame:CGRectZero];
  subview1.yoga.isEnabled = YES;
  subview1.yoga.flexGrow = 1;
  [container addSubview:subview1];

  UIView *subview2 = [[UIView alloc] initWithFrame:CGRectZero];
  subview2.yoga.isEnabled = YES;
  subview2.yoga.flexGrow = 1;
  [container addSubview:subview2];

  UIView *subview3 = [[UIView alloc] initWithFrame:CGRectZero];
  subview3.yoga.isEnabled = YES;
  subview3.yoga.flexGrow = 1;
  [container addSubview:subview3];

  [container.yoga applyLayoutPreservingOrigin:YES];

  XCTAssertTrue(CGRectEqualToRect(subview1.frame, CGRectMake(0, 0, 100, 50)));
  XCTAssertTrue(CGRectEqualToRect(subview2.frame, CGRectMake(100, 0, 100, 50)));
  XCTAssertTrue(CGRectEqualToRect(subview3.frame, CGRectMake(200, 0, 100, 50)));

  [container exchangeSubviewAtIndex:2 withSubviewAtIndex:0];
  subview2.yoga.isIncludedInLayout = NO;
  [container.yoga applyLayoutPreservingOrigin:YES];

  XCTAssertTrue(CGRectEqualToRect(subview3.frame, CGRectMake(0, 0, 150, 50)));
  XCTAssertTrue(CGRectEqualToRect(subview1.frame, CGRectMake(150, 0, 150, 50)));

  // this frame shouldn't have been modified since last time.
  XCTAssertTrue(CGRectEqualToRect(subview2.frame, CGRectMake(100, 0, 100, 50)));
}

- (void)testThatWeRespectIncludeInLayoutFlag
{
  const CGSize containerSize = CGSizeMake(300, 50);

  UIView *container = [[UIView alloc] initWithFrame:CGRectMake(0, 0, containerSize.width, containerSize.height)];
  container.yoga.isEnabled = YES;
  container.yoga.flexDirection = BNDYGFlexDirectionRow;

  UIView *subview1 = [[UIView alloc] initWithFrame:CGRectZero];
  subview1.yoga.isEnabled = YES;
  subview1.yoga.flexGrow = 1;
  [container addSubview:subview1];

  UIView *subview2 = [[UIView alloc] initWithFrame:CGRectZero];
  subview2.yoga.isEnabled = YES;
  subview2.yoga.flexGrow = 1;
  [container addSubview:subview2];

  UIView *subview3 = [[UIView alloc] initWithFrame:CGRectZero];
  subview3.yoga.isEnabled = YES;
  subview3.yoga.flexGrow = 1;
  [container addSubview:subview3];

  [container.yoga applyLayoutPreservingOrigin:YES];

  for (UIView *subview in container.subviews) {
    XCTAssertEqual(subview.bounds.size.width, 100);
  }

  subview3.yoga.isIncludedInLayout = NO;
  [container.yoga applyLayoutPreservingOrigin:YES];

  XCTAssertEqual(subview1.bounds.size.width, 150);
  XCTAssertEqual(subview2.bounds.size.width, 150);

  // We don't set the frame to zero, so, it should be set to what it was previously at.
  XCTAssertEqual(subview3.bounds.size.width, 100);
}

- (void)testThatNumberOfChildrenIsCorrectWhenWeIgnoreSubviews
{
  UIView *container = [[UIView alloc] initWithFrame:CGRectZero];
  container.yoga.isEnabled = YES;
  container.yoga.flexDirection = BNDYGFlexDirectionRow;

  UIView *subview1 = [[UIView alloc] initWithFrame:CGRectZero];
  subview1.yoga.isEnabled = YES;
  subview1.yoga.isIncludedInLayout = NO;
  [container addSubview:subview1];

  UIView *subview2 = [[UIView alloc] initWithFrame:CGRectZero];
  subview2.yoga.isEnabled = YES;
  subview2.yoga.isIncludedInLayout = NO;
  [container addSubview:subview2];

  UIView *subview3 = [[UIView alloc] initWithFrame:CGRectZero];
  subview3.yoga.isEnabled = YES;
  subview3.yoga.isIncludedInLayout = YES;
  [container addSubview:subview3];

  [container.yoga applyLayoutPreservingOrigin:YES];
  XCTAssertEqual(container.yoga.numberOfChildren, 1);

  subview2.yoga.isIncludedInLayout = YES;
  [container.yoga applyLayoutPreservingOrigin:YES];
  XCTAssertEqual(container.yoga.numberOfChildren, 2);
}

- (void)testThatViewNotIncludedInFirstLayoutPassAreIncludedInSecond
{
  UIView *container = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 300, 50)];
  container.yoga.isEnabled = YES;
  container.yoga.flexDirection = BNDYGFlexDirectionRow;

  UIView *subview1 = [[UIView alloc] initWithFrame:CGRectZero];
  subview1.yoga.isEnabled = YES;
  subview1.yoga.flexGrow = 1;
  [container addSubview:subview1];

  UIView *subview2 = [[UIView alloc] initWithFrame:CGRectZero];
  subview2.yoga.isEnabled = YES;
  subview2.yoga.flexGrow = 1;
  [container addSubview:subview2];

  UIView *subview3 = [[UIView alloc] initWithFrame:CGRectZero];
  subview3.yoga.isEnabled = YES;
  subview3.yoga.flexGrow = 1;
  subview3.yoga.isIncludedInLayout = NO;
  [container addSubview:subview3];

  [container.yoga applyLayoutPreservingOrigin:YES];

  XCTAssertEqual(subview1.bounds.size.width, 150);
  XCTAssertEqual(subview2.bounds.size.width, 150);
  XCTAssertEqual(subview3.bounds.size.width, 0);

  subview3.yoga.isIncludedInLayout = YES;
  [container.yoga applyLayoutPreservingOrigin:YES];

  XCTAssertEqual(subview1.bounds.size.width, 100);
  XCTAssertEqual(subview2.bounds.size.width, 100);
  XCTAssertEqual(subview3.bounds.size.width, 100);
}

- (void)testIsLeafFlag
{
  UIView *view = [[UIView alloc] initWithFrame:CGRectZero];
  XCTAssertTrue(view.yoga.isLeaf);

  for (int i=0; i<10; i++) {
    UIView *subview = [[UIView alloc] initWithFrame:CGRectZero];
    [view addSubview:subview];
  }
  XCTAssertTrue(view.yoga.isLeaf);

  view.yoga.isEnabled = YES;
  view.yoga.width = BNDYGPointValue(50);
  XCTAssertTrue(view.yoga.isLeaf);

  UIView *const subview = view.subviews[0];
  subview.yoga.isEnabled = YES;
  subview.yoga.width = BNDYGPointValue(50);
  XCTAssertFalse(view.yoga.isLeaf);
}

- (void)testThatWeCorrectlyAttachNestedViews
{
    UIView *container = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 300, 50)];
    container.yoga.isEnabled = YES;
    container.yoga.flexDirection = BNDYGFlexDirectionColumn;

    UIView *subview1 = [[UIView alloc] initWithFrame:CGRectZero];
    subview1.yoga.isEnabled = YES;
    subview1.yoga.width = BNDYGPointValue(100);
    subview1.yoga.flexGrow = 1;
    subview1.yoga.flexDirection = BNDYGFlexDirectionColumn;
    [container addSubview:subview1];

    UIView *subview2 = [[UIView alloc] initWithFrame:CGRectZero];
    subview2.yoga.isEnabled = YES;
    subview2.yoga.width = BNDYGPointValue(150);
    subview2.yoga.flexGrow = 1;
    subview2.yoga.flexDirection = BNDYGFlexDirectionColumn;
    [container addSubview:subview2];

    for (UIView *view in @[subview1, subview2]) {
        UIView *someView = [[UIView alloc] initWithFrame:CGRectZero];
        someView.yoga.isEnabled = YES;
        someView.yoga.flexGrow = 1;
        [view addSubview:someView];
    }
    [container.yoga applyLayoutPreservingOrigin:YES];

    // Add the same amount of new views, reapply layout.
    for (UIView *view in @[subview1, subview2]) {
        UIView *someView = [[UIView alloc] initWithFrame:CGRectZero];
        someView.yoga.isEnabled = YES;
        someView.yoga.flexGrow = 1;
        [view addSubview:someView];
    }
    [container.yoga applyLayoutPreservingOrigin:YES];

    XCTAssertEqual(subview1.bounds.size.width, 100);
    XCTAssertEqual(subview1.bounds.size.height, 25);
    for (UIView *subview in subview1.subviews) {
        const CGSize subviewSize = subview.bounds.size;
        XCTAssertNotEqual(subviewSize.width, 0);
        XCTAssertNotEqual(subviewSize.height, 0);
        XCTAssertFalse(isnan(subviewSize.height));
        XCTAssertFalse(isnan(subviewSize.width));
    }

    XCTAssertEqual(subview2.bounds.size.width, 150);
    XCTAssertEqual(subview2.bounds.size.height, 25);
    for (UIView *subview in subview2.subviews) {
        const CGSize subviewSize = subview.bounds.size;
        XCTAssertNotEqual(subviewSize.width, 0);
        XCTAssertNotEqual(subviewSize.height, 0);
        XCTAssertFalse(isnan(subviewSize.height));
        XCTAssertFalse(isnan(subviewSize.width));
    }
}

- (void)testThatANonLeafNodeCanBecomeALeafNode
{
    UIView *container = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 300, 50)];
    container.yoga.isEnabled = YES;

    UIView *subview1 = [[UIView alloc] initWithFrame:CGRectZero];
    subview1.yoga.isEnabled = YES;
    [container addSubview:subview1];

    UIView *subview2 = [[UIView alloc] initWithFrame:CGRectZero];
    subview2.yoga.isEnabled = YES;
    [subview1 addSubview:subview2];

    [container.yoga applyLayoutPreservingOrigin:YES];
    [subview2 removeFromSuperview];
    [container.yoga applyLayoutPreservingOrigin:YES];
}

- (void)testPointPercent
{
  XCTAssertEqual(BNDYGPointValue(1).value, 1);
  XCTAssertEqual(BNDYGPointValue(1).unit, BNDYGUnitPoint);
  XCTAssertEqual(BNDYGPercentValue(2).value, 2);
  XCTAssertEqual(BNDYGPercentValue(2).unit, BNDYGUnitPercent);
}

- (void)testPositionalPropertiesWork
{
  UIView *view = [[UIView alloc] initWithFrame:CGRectZero];

  view.yoga.left = BNDYGPointValue(1);
  XCTAssertEqual(view.yoga.left.value, 1);
  XCTAssertEqual(view.yoga.left.unit, BNDYGUnitPoint);
  view.yoga.left = BNDYGPercentValue(2);
  XCTAssertEqual(view.yoga.left.value, 2);
  XCTAssertEqual(view.yoga.left.unit, BNDYGUnitPercent);

  view.yoga.right = BNDYGPointValue(3);
  XCTAssertEqual(view.yoga.right.value, 3);
  XCTAssertEqual(view.yoga.right.unit, BNDYGUnitPoint);
  view.yoga.right = BNDYGPercentValue(4);
  XCTAssertEqual(view.yoga.right.value, 4);
  XCTAssertEqual(view.yoga.right.unit, BNDYGUnitPercent);

  view.yoga.top = BNDYGPointValue(5);
  XCTAssertEqual(view.yoga.top.value, 5);
  XCTAssertEqual(view.yoga.top.unit, BNDYGUnitPoint);
  view.yoga.top = BNDYGPercentValue(6);
  XCTAssertEqual(view.yoga.top.value, 6);
  XCTAssertEqual(view.yoga.top.unit, BNDYGUnitPercent);

  view.yoga.bottom = BNDYGPointValue(7);
  XCTAssertEqual(view.yoga.bottom.value, 7);
  XCTAssertEqual(view.yoga.bottom.unit, BNDYGUnitPoint);
  view.yoga.bottom = BNDYGPercentValue(8);
  XCTAssertEqual(view.yoga.bottom.value, 8);
  XCTAssertEqual(view.yoga.bottom.unit, BNDYGUnitPercent);

  view.yoga.start = BNDYGPointValue(9);
  XCTAssertEqual(view.yoga.start.value, 9);
  XCTAssertEqual(view.yoga.start.unit, BNDYGUnitPoint);
  view.yoga.start = BNDYGPercentValue(10);
  XCTAssertEqual(view.yoga.start.value, 10);
  XCTAssertEqual(view.yoga.start.unit, BNDYGUnitPercent);

  view.yoga.end = BNDYGPointValue(11);
  XCTAssertEqual(view.yoga.end.value, 11);
  XCTAssertEqual(view.yoga.end.unit, BNDYGUnitPoint);
  view.yoga.end = BNDYGPercentValue(12);
  XCTAssertEqual(view.yoga.end.value, 12);
  XCTAssertEqual(view.yoga.end.unit, BNDYGUnitPercent);
}

- (void)testMarginPropertiesWork
{
  UIView *view = [[UIView alloc] initWithFrame:CGRectZero];

  view.yoga.margin = BNDYGPointValue(1);
  XCTAssertEqual(view.yoga.margin.value, 1);
  XCTAssertEqual(view.yoga.margin.unit, BNDYGUnitPoint);
  view.yoga.margin = BNDYGPercentValue(2);
  XCTAssertEqual(view.yoga.margin.value, 2);
  XCTAssertEqual(view.yoga.margin.unit, BNDYGUnitPercent);

  view.yoga.marginHorizontal = BNDYGPointValue(3);
  XCTAssertEqual(view.yoga.marginHorizontal.value, 3);
  XCTAssertEqual(view.yoga.marginHorizontal.unit, BNDYGUnitPoint);
  view.yoga.marginHorizontal = BNDYGPercentValue(4);
  XCTAssertEqual(view.yoga.marginHorizontal.value, 4);
  XCTAssertEqual(view.yoga.marginHorizontal.unit, BNDYGUnitPercent);

  view.yoga.marginVertical = BNDYGPointValue(5);
  XCTAssertEqual(view.yoga.marginVertical.value, 5);
  XCTAssertEqual(view.yoga.marginVertical.unit, BNDYGUnitPoint);
  view.yoga.marginVertical = BNDYGPercentValue(6);
  XCTAssertEqual(view.yoga.marginVertical.value, 6);
  XCTAssertEqual(view.yoga.marginVertical.unit, BNDYGUnitPercent);

  view.yoga.marginLeft = BNDYGPointValue(7);
  XCTAssertEqual(view.yoga.marginLeft.value, 7);
  XCTAssertEqual(view.yoga.marginLeft.unit, BNDYGUnitPoint);
  view.yoga.marginLeft = BNDYGPercentValue(8);
  XCTAssertEqual(view.yoga.marginLeft.value, 8);
  XCTAssertEqual(view.yoga.marginLeft.unit, BNDYGUnitPercent);

  view.yoga.marginRight = BNDYGPointValue(9);
  XCTAssertEqual(view.yoga.marginRight.value, 9);
  XCTAssertEqual(view.yoga.marginRight.unit, BNDYGUnitPoint);
  view.yoga.marginRight = BNDYGPercentValue(10);
  XCTAssertEqual(view.yoga.marginRight.value, 10);
  XCTAssertEqual(view.yoga.marginRight.unit, BNDYGUnitPercent);

  view.yoga.marginTop = BNDYGPointValue(11);
  XCTAssertEqual(view.yoga.marginTop.value, 11);
  XCTAssertEqual(view.yoga.marginTop.unit, BNDYGUnitPoint);
  view.yoga.marginTop = BNDYGPercentValue(12);
  XCTAssertEqual(view.yoga.marginTop.value, 12);
  XCTAssertEqual(view.yoga.marginTop.unit, BNDYGUnitPercent);

  view.yoga.marginBottom = BNDYGPointValue(13);
  XCTAssertEqual(view.yoga.marginBottom.value, 13);
  XCTAssertEqual(view.yoga.marginBottom.unit, BNDYGUnitPoint);
  view.yoga.marginBottom = BNDYGPercentValue(14);
  XCTAssertEqual(view.yoga.marginBottom.value, 14);
  XCTAssertEqual(view.yoga.marginBottom.unit, BNDYGUnitPercent);

  view.yoga.marginStart = BNDYGPointValue(15);
  XCTAssertEqual(view.yoga.marginStart.value, 15);
  XCTAssertEqual(view.yoga.marginStart.unit, BNDYGUnitPoint);
  view.yoga.marginStart = BNDYGPercentValue(16);
  XCTAssertEqual(view.yoga.marginStart.value, 16);
  XCTAssertEqual(view.yoga.marginStart.unit, BNDYGUnitPercent);

  view.yoga.marginEnd = BNDYGPointValue(17);
  XCTAssertEqual(view.yoga.marginEnd.value, 17);
  XCTAssertEqual(view.yoga.marginEnd.unit, BNDYGUnitPoint);
  view.yoga.marginEnd = BNDYGPercentValue(18);
  XCTAssertEqual(view.yoga.marginEnd.value, 18);
  XCTAssertEqual(view.yoga.marginEnd.unit, BNDYGUnitPercent);
}

- (void)testPaddingPropertiesWork
{
  UIView *view = [[UIView alloc] initWithFrame:CGRectZero];

  view.yoga.padding = BNDYGPointValue(1);
  XCTAssertEqual(view.yoga.padding.value, 1);
  XCTAssertEqual(view.yoga.padding.unit, BNDYGUnitPoint);
  view.yoga.padding = BNDYGPercentValue(2);
  XCTAssertEqual(view.yoga.padding.value, 2);
  XCTAssertEqual(view.yoga.padding.unit, BNDYGUnitPercent);

  view.yoga.paddingHorizontal = BNDYGPointValue(3);
  XCTAssertEqual(view.yoga.paddingHorizontal.value, 3);
  XCTAssertEqual(view.yoga.paddingHorizontal.unit, BNDYGUnitPoint);
  view.yoga.paddingHorizontal = BNDYGPercentValue(4);
  XCTAssertEqual(view.yoga.paddingHorizontal.value, 4);
  XCTAssertEqual(view.yoga.paddingHorizontal.unit, BNDYGUnitPercent);

  view.yoga.paddingVertical = BNDYGPointValue(5);
  XCTAssertEqual(view.yoga.paddingVertical.value, 5);
  XCTAssertEqual(view.yoga.paddingVertical.unit, BNDYGUnitPoint);
  view.yoga.paddingVertical = BNDYGPercentValue(6);
  XCTAssertEqual(view.yoga.paddingVertical.value, 6);
  XCTAssertEqual(view.yoga.paddingVertical.unit, BNDYGUnitPercent);

  view.yoga.paddingLeft = BNDYGPointValue(7);
  XCTAssertEqual(view.yoga.paddingLeft.value, 7);
  XCTAssertEqual(view.yoga.paddingLeft.unit, BNDYGUnitPoint);
  view.yoga.paddingLeft = BNDYGPercentValue(8);
  XCTAssertEqual(view.yoga.paddingLeft.value, 8);
  XCTAssertEqual(view.yoga.paddingLeft.unit, BNDYGUnitPercent);

  view.yoga.paddingRight = BNDYGPointValue(9);
  XCTAssertEqual(view.yoga.paddingRight.value, 9);
  XCTAssertEqual(view.yoga.paddingRight.unit, BNDYGUnitPoint);
  view.yoga.paddingRight = BNDYGPercentValue(10);
  XCTAssertEqual(view.yoga.paddingRight.value, 10);
  XCTAssertEqual(view.yoga.paddingRight.unit, BNDYGUnitPercent);

  view.yoga.paddingTop = BNDYGPointValue(11);
  XCTAssertEqual(view.yoga.paddingTop.value, 11);
  XCTAssertEqual(view.yoga.paddingTop.unit, BNDYGUnitPoint);
  view.yoga.paddingTop = BNDYGPercentValue(12);
  XCTAssertEqual(view.yoga.paddingTop.value, 12);
  XCTAssertEqual(view.yoga.paddingTop.unit, BNDYGUnitPercent);

  view.yoga.paddingBottom = BNDYGPointValue(13);
  XCTAssertEqual(view.yoga.paddingBottom.value, 13);
  XCTAssertEqual(view.yoga.paddingBottom.unit, BNDYGUnitPoint);
  view.yoga.paddingBottom = BNDYGPercentValue(14);
  XCTAssertEqual(view.yoga.paddingBottom.value, 14);
  XCTAssertEqual(view.yoga.paddingBottom.unit, BNDYGUnitPercent);

  view.yoga.paddingStart = BNDYGPointValue(15);
  XCTAssertEqual(view.yoga.paddingStart.value, 15);
  XCTAssertEqual(view.yoga.paddingStart.unit, BNDYGUnitPoint);
  view.yoga.paddingStart = BNDYGPercentValue(16);
  XCTAssertEqual(view.yoga.paddingStart.value, 16);
  XCTAssertEqual(view.yoga.paddingStart.unit, BNDYGUnitPercent);

  view.yoga.paddingEnd = BNDYGPointValue(17);
  XCTAssertEqual(view.yoga.paddingEnd.value, 17);
  XCTAssertEqual(view.yoga.paddingEnd.unit, BNDYGUnitPoint);
  view.yoga.paddingEnd = BNDYGPercentValue(18);
  XCTAssertEqual(view.yoga.paddingEnd.value, 18);
  XCTAssertEqual(view.yoga.paddingEnd.unit, BNDYGUnitPercent);
}

- (void)testBorderWidthPropertiesWork
{
  UIView *view = [[UIView alloc] initWithFrame:CGRectZero];

  view.yoga.borderWidth = 1;
  XCTAssertEqual(view.yoga.borderWidth, 1);

  view.yoga.borderLeftWidth = 2;
  XCTAssertEqual(view.yoga.borderLeftWidth, 2);

  view.yoga.borderRightWidth = 3;
  XCTAssertEqual(view.yoga.borderRightWidth, 3);

  view.yoga.borderTopWidth = 4;
  XCTAssertEqual(view.yoga.borderTopWidth, 4);

  view.yoga.borderBottomWidth = 5;
  XCTAssertEqual(view.yoga.borderBottomWidth, 5);

  view.yoga.borderStartWidth = 6;
  XCTAssertEqual(view.yoga.borderStartWidth, 6);

  view.yoga.borderEndWidth = 7;
  XCTAssertEqual(view.yoga.borderEndWidth, 7);
}

@end
