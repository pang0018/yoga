/**
 * Copyright (c) 2014-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>

#include <bindyoga/BNDBNDYGNode.h>
#include <bindyoga/BindYoga.h>

static std::vector<BNDYGNodeRef> getChildren(BNDYGNodeRef const node)
{
  const uint32_t count = BNDYGNodeGetChildCount(node);
  std::vector<BNDYGNodeRef> children;
  children.reserve(count);
  for (uint32_t i = 0 ; i < count ; i++) {
    children.push_back(BNDYGNodeGetChild(node, i));
  }
  return children;
}

TEST(YogaTest, set_children_adds_children_to_parent) {
  BNDYGNodeRef const root = BNDYGNodeNew();
  BNDYGNodeRef const root_child0 = BNDYGNodeNew();
  BNDYGNodeRef const root_child1 = BNDYGNodeNew();

  BNDYGNodeSetChildren(root, {root_child0, root_child1});

  const std::vector<BNDYGNodeRef> children = getChildren(root);
  const std::vector<BNDYGNodeRef> expectedChildren = {root_child0, root_child1};
  ASSERT_EQ(children, expectedChildren);

  const std::vector<BNDYGNodeRef> owners = {BNDYGNodeGetOwner(root_child0), BNDYGNodeGetOwner(root_child1)};
  const std::vector<BNDYGNodeRef> expectedOwners = {root, root};
  ASSERT_EQ(owners, expectedOwners);

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, set_children_to_empty_removes_old_children) {
  BNDYGNodeRef const root = BNDYGNodeNew();
  BNDYGNodeRef const root_child0 = BNDYGNodeNew();
  BNDYGNodeRef const root_child1 = BNDYGNodeNew();

  BNDYGNodeSetChildren(root, {root_child0, root_child1});
  BNDYGNodeSetChildren(root, {});

  const std::vector<BNDYGNodeRef> children = getChildren(root);
  const std::vector<BNDYGNodeRef> expectedChildren = {};
  ASSERT_EQ(children, expectedChildren);

  const std::vector<BNDYGNodeRef> owners = {BNDYGNodeGetOwner(root_child0), BNDYGNodeGetOwner(root_child1)};
  const std::vector<BNDYGNodeRef> expectedOwners = {nullptr, nullptr};
  ASSERT_EQ(owners, expectedOwners);

  BNDYGNodeFreeRecursive(root);
}

TEST(YogaTest, set_children_replaces_non_common_children) {
  BNDYGNodeRef const root = BNDYGNodeNew();
  BNDYGNodeRef const root_child0 = BNDYGNodeNew();
  BNDYGNodeRef const root_child1 = BNDYGNodeNew();

  BNDYGNodeSetChildren(root, {root_child0, root_child1});

  BNDYGNodeRef const root_child2 = BNDYGNodeNew();
  BNDYGNodeRef const root_child3 = BNDYGNodeNew();

  BNDYGNodeSetChildren(root, {root_child2, root_child3});

  const std::vector<BNDYGNodeRef> children = getChildren(root);
  const std::vector<BNDYGNodeRef> expectedChildren = {root_child2, root_child3};
  ASSERT_EQ(children, expectedChildren);

  const std::vector<BNDYGNodeRef> owners = {BNDYGNodeGetOwner(root_child0), BNDYGNodeGetOwner(root_child1)};
  const std::vector<BNDYGNodeRef> expectedOwners = {nullptr, nullptr};
  ASSERT_EQ(owners, expectedOwners);

  BNDYGNodeFreeRecursive(root);
  BNDYGNodeFree(root_child0);
  BNDYGNodeFree(root_child1);
}

TEST(YogaTest, set_children_keeps_and_reorders_common_children) {
  BNDYGNodeRef const root = BNDYGNodeNew();
  BNDYGNodeRef const root_child0 = BNDYGNodeNew();
  BNDYGNodeRef const root_child1 = BNDYGNodeNew();
  BNDYGNodeRef const root_child2 = BNDYGNodeNew();

  BNDYGNodeSetChildren(root, {root_child0, root_child1, root_child2});

  BNDYGNodeRef const root_child3 = BNDYGNodeNew();

  BNDYGNodeSetChildren(root, {root_child2, root_child1, root_child3});

  const std::vector<BNDYGNodeRef> children = getChildren(root);
  const std::vector<BNDYGNodeRef> expectedChildren = {root_child2, root_child1, root_child3};
  ASSERT_EQ(children, expectedChildren);

  const std::vector<BNDYGNodeRef> owners = {
    BNDYGNodeGetOwner(root_child0),
    BNDYGNodeGetOwner(root_child1),
    BNDYGNodeGetOwner(root_child2),
    BNDYGNodeGetOwner(root_child3)
  };
  const std::vector<BNDYGNodeRef> expectedOwners = {nullptr, root, root, root};
  ASSERT_EQ(owners, expectedOwners);

  BNDYGNodeFreeRecursive(root);
  BNDYGNodeFree(root_child0);
}
