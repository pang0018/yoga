/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "BNDYGEnums.h"

const char *BNDYGAlignToString(const BNDYGAlign value){
  switch(value){
    case BNDYGAlignAuto:
      return "auto";
    case BNDYGAlignFlexStart:
      return "flex-start";
    case BNDYGAlignCenter:
      return "center";
    case BNDYGAlignFlexEnd:
      return "flex-end";
    case BNDYGAlignStretch:
      return "stretch";
    case BNDYGAlignBaseline:
      return "baseline";
    case BNDYGAlignSpaceBetween:
      return "space-between";
    case BNDYGAlignSpaceAround:
      return "space-around";
  }
  return "unknown";
}

const char *BNDYGDimensionToString(const BNDYGDimension value){
  switch(value){
    case BNDYGDimensionWidth:
      return "width";
    case BNDYGDimensionHeight:
      return "height";
  }
  return "unknown";
}

const char *BNDYGDirectionToString(const BNDYGDirection value){
  switch(value){
    case BNDYGDirectionInherit:
      return "inherit";
    case BNDYGDirectionLTR:
      return "ltr";
    case BNDYGDirectionRTL:
      return "rtl";
  }
  return "unknown";
}

const char *BNDYGDisplayToString(const BNDYGDisplay value){
  switch(value){
    case BNDYGDisplayFlex:
      return "flex";
    case BNDYGDisplayNone:
      return "none";
  }
  return "unknown";
}

const char *BNDYGEdgeToString(const BNDYGEdge value){
  switch(value){
    case BNDYGEdgeLeft:
      return "left";
    case BNDYGEdgeTop:
      return "top";
    case BNDYGEdgeRight:
      return "right";
    case BNDYGEdgeBottom:
      return "bottom";
    case BNDYGEdgeStart:
      return "start";
    case BNDYGEdgeEnd:
      return "end";
    case BNDYGEdgeHorizontal:
      return "horizontal";
    case BNDYGEdgeVertical:
      return "vertical";
    case BNDYGEdgeAll:
      return "all";
  }
  return "unknown";
}

const char *BNDYGExperimentalFeatureToString(const BNDYGExperimentalFeature value){
  switch(value){
    case BNDYGExperimentalFeatureWebFlexBasis:
      return "web-flex-basis";
  }
  return "unknown";
}

const char *BNDYGFlexDirectionToString(const BNDYGFlexDirection value){
  switch(value){
    case BNDYGFlexDirectionColumn:
      return "column";
    case BNDYGFlexDirectionColumnReverse:
      return "column-reverse";
    case BNDYGFlexDirectionRow:
      return "row";
    case BNDYGFlexDirectionRowReverse:
      return "row-reverse";
  }
  return "unknown";
}

const char *BNDYGJustifyToString(const BNDYGJustify value){
  switch(value){
    case BNDYGJustifyFlexStart:
      return "flex-start";
    case BNDYGJustifyCenter:
      return "center";
    case BNDYGJustifyFlexEnd:
      return "flex-end";
    case BNDYGJustifySpaceBetween:
      return "space-between";
    case BNDYGJustifySpaceAround:
      return "space-around";
    case BNDYGJustifySpaceEvenly:
      return "space-evenly";
  }
  return "unknown";
}

const char *BNDYGLogLevelToString(const BNDYGLogLevel value){
  switch(value){
    case BNDYGLogLevelError:
      return "error";
    case BNDYGLogLevelWarn:
      return "warn";
    case BNDYGLogLevelInfo:
      return "info";
    case BNDYGLogLevelDebug:
      return "debug";
    case BNDYGLogLevelVerbose:
      return "verbose";
    case BNDYGLogLevelFatal:
      return "fatal";
  }
  return "unknown";
}

const char *BNDYGMeasureModeToString(const BNDYGMeasureMode value){
  switch(value){
    case BNDYGMeasureModeUndefined:
      return "undefined";
    case BNDYGMeasureModeExactly:
      return "exactly";
    case BNDYGMeasureModeAtMost:
      return "at-most";
  }
  return "unknown";
}

const char *BNDYGNodeTypeToString(const BNDYGNodeType value){
  switch(value){
    case BNDYGNodeTypeDefault:
      return "default";
    case BNDYGNodeTypeText:
      return "text";
  }
  return "unknown";
}

const char *BNDYGOverflowToString(const BNDYGOverflow value){
  switch(value){
    case BNDYGOverflowVisible:
      return "visible";
    case BNDYGOverflowHidden:
      return "hidden";
    case BNDYGOverflowScroll:
      return "scroll";
  }
  return "unknown";
}

const char *BNDYGPositionTypeToString(const BNDYGPositionType value){
  switch(value){
    case BNDYGPositionTypeRelative:
      return "relative";
    case BNDYGPositionTypeAbsolute:
      return "absolute";
  }
  return "unknown";
}

const char *BNDYGPrintOptionsToString(const BNDYGPrintOptions value){
  switch(value){
    case BNDYGPrintOptionsLayout:
      return "layout";
    case BNDYGPrintOptionsStyle:
      return "style";
    case BNDYGPrintOptionsChildren:
      return "children";
  }
  return "unknown";
}

const char *BNDYGUnitToString(const BNDYGUnit value){
  switch(value){
    case BNDYGUnitUndefined:
      return "undefined";
    case BNDYGUnitPoint:
      return "point";
    case BNDYGUnitPercent:
      return "percent";
    case BNDYGUnitAuto:
      return "auto";
  }
  return "unknown";
}

const char *BNDYGWrapToString(const BNDYGWrap value){
  switch(value){
    case BNDYGWrapNoWrap:
      return "no-wrap";
    case BNDYGWrapWrap:
      return "wrap";
    case BNDYGWrapWrapReverse:
      return "wrap-reverse";
  }
  return "unknown";
}
