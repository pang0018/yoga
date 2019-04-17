/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

postfix operator %

extension Int {
    public static postfix func %(value: Int) -> BNDYGValue {
        return BNDYGValue(value: Float(value), unit: .percent)
    }
}

extension Float {
    public static postfix func %(value: Float) -> BNDYGValue {
        return BNDYGValue(value: value, unit: .percent)
    }
}

extension CGFloat {
    public static postfix func %(value: CGFloat) -> BNDYGValue {
        return BNDYGValue(value: Float(value), unit: .percent)
    }
}

extension BNDYGValue : ExpressibleByIntegerLiteral, ExpressibleByFloatLiteral {
    public init(integerLiteral value: Int) {
        self = BNDYGValue(value: Float(value), unit: .point)
    }
	
    public init(floatLiteral value: Float) {
        self = BNDYGValue(value: value, unit: .point)
    }
  
    public init(_ value: Float) {
        self = BNDYGValue(value: value, unit: .point)
    }
  
    public init(_ value: CGFloat) {
        self = BNDYGValue(value: Float(value), unit: .point)
    }
}
