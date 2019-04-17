# Copyright (c) 2014-present, Facebook, Inc.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
load("//:yoga_defs.bzl", "BASE_COMPILER_FLAGS", "GTEST_TARGET", "LIBRARY_COMPILER_FLAGS", "subdir_glob", "yoga_cxx_library", "yoga_cxx_test", "yoga_dep")

GMOCK_OVERRIDE_FLAGS = [
    # gmock does not mark mocked methods as override, ignore the warnings in tests
    "-Wno-inconsistent-missing-override",
]

COMPILER_FLAGS = LIBRARY_COMPILER_FLAGS + [
    "-std=c++1y",
]

TEST_COMPILER_FLAGS = BASE_COMPILER_FLAGS + GMOCK_OVERRIDE_FLAGS + [
    "-std=c++1y",
]

yoga_cxx_library(
    name = "bindyoga",
    srcs = glob(["bindyoga/*.cpp"]),
    header_namespace = "",
    exported_headers = subdir_glob([("", "bindyoga/*.h")]),
    compiler_flags = COMPILER_FLAGS,
    soname = "libyogacore.$(ext)",
    tests = [":BindYogaTests"],
    visibility = ["PUBLIC"],
    deps = [
        yoga_dep("lib/fb:ndklog"),
    ],
)

yoga_cxx_test(
    name = "YogaTests",
    srcs = glob(["tests/*.cpp"]),
    compiler_flags = TEST_COMPILER_FLAGS,
    contacts = ["emilsj@fb.com"],
    visibility = ["PUBLIC"],
    deps = [
        ":bindyoga",
        GTEST_TARGET,
    ],
)
