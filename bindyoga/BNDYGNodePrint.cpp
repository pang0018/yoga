/*
 * Copyright (c) 2017-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "BNDYGNodePrint.h"
#include <stdarg.h>
#include "BNDYGEnums.h"
#include "BNDYGNode.h"
#include "BindYoga-internal.h"

namespace facebook {
namespace bindyoga {
typedef std::string string;

static void indent(string* base, uint32_t level) {
  for (uint32_t i = 0; i < level; ++i) {
    base->append("  ");
  }
}

static bool areFourValuesEqual(const std::array<BNDYGValue, BNDYGEdgeCount>& four) {
  return BNDYGValueEqual(four[0], four[1]) && BNDYGValueEqual(four[0], four[2]) &&
      BNDYGValueEqual(four[0], four[3]);
}

static void appendFormatedString(string* str, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  va_list argsCopy;
  va_copy(argsCopy, args);
  std::vector<char> buf(1 + vsnprintf(NULL, 0, fmt, args));
  va_end(args);
  vsnprintf(buf.data(), buf.size(), fmt, argsCopy);
  va_end(argsCopy);
  string result = string(buf.begin(), buf.end() - 1);
  str->append(result);
}

static void appendFloatOptionalIfDefined(
    string* base,
    const string key,
    const BNDYGFloatOptional num) {
  if (!num.isUndefined()) {
    appendFormatedString(base, "%s: %g; ", key.c_str(), num.getValue());
  }
}

static void appendNumberIfNotUndefined(
    string* base,
    const string key,
    const BNDYGValue number) {
  if (number.unit != BNDYGUnitUndefined) {
    if (number.unit == BNDYGUnitAuto) {
      base->append(key + ": auto; ");
    } else {
      string unit = number.unit == BNDYGUnitPoint ? "px" : "%%";
      appendFormatedString(
          base, "%s: %g%s; ", key.c_str(), number.value, unit.c_str());
    }
  }
}

static void
appendNumberIfNotAuto(string* base, const string& key, const BNDYGValue number) {
  if (number.unit != BNDYGUnitAuto) {
    appendNumberIfNotUndefined(base, key, number);
  }
}

static void
appendNumberIfNotZero(string* base, const string& str, const BNDYGValue number) {

  if (number.unit == BNDYGUnitAuto) {
    base->append(str + ": auto; ");
  } else if (!BNDYGFloatsEqual(number.value, 0)) {
    appendNumberIfNotUndefined(base, str, number);
  }
}

static void appendEdges(
    string* base,
    const string& key,
    const std::array<BNDYGValue, BNDYGEdgeCount>& edges) {
  if (areFourValuesEqual(edges)) {
    appendNumberIfNotZero(base, key, edges[BNDYGEdgeLeft]);
  } else {
    for (int edge = BNDYGEdgeLeft; edge != BNDYGEdgeAll; ++edge) {
      string str = key + "-" + BNDYGEdgeToString(static_cast<BNDYGEdge>(edge));
      appendNumberIfNotZero(base, str, edges[edge]);
    }
  }
}

static void appendEdgeIfNotUndefined(
    string* base,
    const string& str,
    const std::array<BNDYGValue, BNDYGEdgeCount>& edges,
    const BNDYGEdge edge) {
  appendNumberIfNotUndefined(
      base, str, *BNDYGComputedEdgeValue(edges, edge, &BNDYGValueUndefined));
}

void BNDYGNodeToString(
    std::string* str,
    BNDYGNodeRef node,
    BNDYGPrintOptions options,
    uint32_t level) {
  indent(str, level);
  appendFormatedString(str, "<div ");
  if (node->getPrintFunc() != nullptr) {
    node->getPrintFunc()(node);
  }

  if (options & BNDYGPrintOptionsLayout) {
    appendFormatedString(str, "layout=\"");
    appendFormatedString(
        str, "width: %g; ", node->getLayout().dimensions[BNDYGDimensionWidth]);
    appendFormatedString(
        str, "height: %g; ", node->getLayout().dimensions[BNDYGDimensionHeight]);
    appendFormatedString(
        str, "top: %g; ", node->getLayout().position[BNDYGEdgeTop]);
    appendFormatedString(
        str, "left: %g;", node->getLayout().position[BNDYGEdgeLeft]);
    appendFormatedString(str, "\" ");
  }

  if (options & BNDYGPrintOptionsStyle) {
    appendFormatedString(str, "style=\"");
    if (node->getStyle().flexDirection != BNDYGNode().getStyle().flexDirection) {
      appendFormatedString(
          str,
          "flex-direction: %s; ",
          BNDYGFlexDirectionToString(node->getStyle().flexDirection));
    }
    if (node->getStyle().justifyContent != BNDYGNode().getStyle().justifyContent) {
      appendFormatedString(
          str,
          "justify-content: %s; ",
          BNDYGJustifyToString(node->getStyle().justifyContent));
    }
    if (node->getStyle().alignItems != BNDYGNode().getStyle().alignItems) {
      appendFormatedString(
          str,
          "align-items: %s; ",
          BNDYGAlignToString(node->getStyle().alignItems));
    }
    if (node->getStyle().alignContent != BNDYGNode().getStyle().alignContent) {
      appendFormatedString(
          str,
          "align-content: %s; ",
          BNDYGAlignToString(node->getStyle().alignContent));
    }
    if (node->getStyle().alignSelf != BNDYGNode().getStyle().alignSelf) {
      appendFormatedString(
          str, "align-self: %s; ", BNDYGAlignToString(node->getStyle().alignSelf));
    }
    appendFloatOptionalIfDefined(str, "flex-grow", node->getStyle().flexGrow);
    appendFloatOptionalIfDefined(
        str, "flex-shrink", node->getStyle().flexShrink);
    appendNumberIfNotAuto(str, "flex-basis", node->getStyle().flexBasis);
    appendFloatOptionalIfDefined(str, "flex", node->getStyle().flex);

    if (node->getStyle().flexWrap != BNDYGNode().getStyle().flexWrap) {
      appendFormatedString(
          str, "flexWrap: %s; ", BNDYGWrapToString(node->getStyle().flexWrap));
    }

    if (node->getStyle().overflow != BNDYGNode().getStyle().overflow) {
      appendFormatedString(
          str, "overflow: %s; ", BNDYGOverflowToString(node->getStyle().overflow));
    }

    if (node->getStyle().display != BNDYGNode().getStyle().display) {
      appendFormatedString(
          str, "display: %s; ", BNDYGDisplayToString(node->getStyle().display));
    }
    appendEdges(str, "margin", node->getStyle().margin);
    appendEdges(str, "padding", node->getStyle().padding);
    appendEdges(str, "border", node->getStyle().border);

    appendNumberIfNotAuto(
        str, "width", node->getStyle().dimensions[BNDYGDimensionWidth]);
    appendNumberIfNotAuto(
        str, "height", node->getStyle().dimensions[BNDYGDimensionHeight]);
    appendNumberIfNotAuto(
        str, "max-width", node->getStyle().maxDimensions[BNDYGDimensionWidth]);
    appendNumberIfNotAuto(
        str, "max-height", node->getStyle().maxDimensions[BNDYGDimensionHeight]);
    appendNumberIfNotAuto(
        str, "min-width", node->getStyle().minDimensions[BNDYGDimensionWidth]);
    appendNumberIfNotAuto(
        str, "min-height", node->getStyle().minDimensions[BNDYGDimensionHeight]);

    if (node->getStyle().positionType != BNDYGNode().getStyle().positionType) {
      appendFormatedString(
          str,
          "position: %s; ",
          BNDYGPositionTypeToString(node->getStyle().positionType));
    }

    appendEdgeIfNotUndefined(
        str, "left", node->getStyle().position, BNDYGEdgeLeft);
    appendEdgeIfNotUndefined(
        str, "right", node->getStyle().position, BNDYGEdgeRight);
    appendEdgeIfNotUndefined(str, "top", node->getStyle().position, BNDYGEdgeTop);
    appendEdgeIfNotUndefined(
        str, "bottom", node->getStyle().position, BNDYGEdgeBottom);
    appendFormatedString(str, "\" ");

    if (node->getMeasure() != nullptr) {
      appendFormatedString(str, "has-custom-measure=\"true\"");
    }
  }
  appendFormatedString(str, ">");

  const uint32_t childCount = static_cast<uint32_t>(node->getChildren().size());
  if (options & BNDYGPrintOptionsChildren && childCount > 0) {
    for (uint32_t i = 0; i < childCount; i++) {
      appendFormatedString(str, "\n");
      BNDYGNodeToString(str, BNDYGNodeGetChild(node, i), options, level + 1);
    }
    appendFormatedString(str, "\n");
    indent(str, level);
  }
  appendFormatedString(str, "</div>");
}
} // namespace bindyoga
} // namespace facebook
