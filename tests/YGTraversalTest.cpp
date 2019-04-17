/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>

#include <bindyoga/BNDBNDYGNode.h>
#include <bindyoga/BindYoga.h>

TEST(YogaTest, pre_order_traversal) {
  BNDYGNodeRef const root = BNDYGNodeNew();
  BNDYGNodeRef const root_child0 = BNDYGNodeNew();
  BNDYGNodeRef const root_child1 = BNDYGNodeNew();
  BNDYGNodeRef const root_child0_child0 = BNDYGNodeNew();
  
  BNDYGNodeSetChildren(root, {root_child0, root_child1});
  BNDYGNodeInsertChild(root_child0, root_child0_child0, 0);
  
  std::vector<BNDYGNodeRef> visited;
  BNDYGTraversePreOrder(root, [&visited](BNDYGNodeRef node) {
    visited.push_back(node);
  });
  
  const std::vector<BNDYGNodeRef> expected = {
    root,
    root_child0,
    root_child0_child0,
    root_child1
  };
  ASSERT_EQ(visited, expected);

  BNDYGNodeFreeRecursive(root);
}
