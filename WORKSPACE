load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "io_bazel_rules_go",
    sha256 = "207fad3e6689135c5d8713e5a17ba9d1290238f47b9ba545b63d9303406209c6",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_go/releases/download/v0.24.7/rules_go-v0.24.7.tar.gz",
        "https://github.com/bazelbuild/rules_go/releases/download/v0.24.7/rules_go-v0.24.7.tar.gz",
    ],
)

http_archive(
    name = "bazel_gazelle",
    sha256 = "b85f48fa105c4403326e9525ad2b2cc437babaa6e15a3fc0b1dbab0ab064bc7c",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-gazelle/releases/download/v0.22.2/bazel-gazelle-v0.22.2.tar.gz",
        "https://github.com/bazelbuild/bazel-gazelle/releases/download/v0.22.2/bazel-gazelle-v0.22.2.tar.gz",
    ],
)

load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")
load("@bazel_gazelle//:deps.bzl", "gazelle_dependencies", "go_repository")

go_rules_dependencies()

go_register_toolchains()

gazelle_dependencies()

load("@com_github_bazelbuild_buildtools//buildifier:deps.bzl", "buildifier_dependencies")

buildifier_dependencies()

git_repository(
    name = "rules_cc",
    commit = "02becfef8bc97bda4f9bb64e153f1b0671aec4ba",
    remote = "https://github.com/bazelbuild/rules_cc",
    shallow_since = "1597773444 -0700",
)

git_repository(
    name = "gtest",
    commit = "13a433a94dd9c7e55907d7a9b75f44ff82f309eb",
    remote = "https://github.com/google/googletest",
    shallow_since = "1591825059 -0400",
)

git_repository(
    name = "avr_tools",
    commit = "997c1096f95a12385addc9ecce7a6aae33ae933b",
    remote = "https://github.com/mihaigalos/bazel-avr-tools",
    shallow_since = "1609321613 +0100",
)

new_local_repository(
    name = "system_libs",
    build_file_content = """
cc_library(
    name = "dl",
    srcs = ["libdl.so.2"],
    visibility = ["//visibility:public"],
)
""",
    # pkg-config --variable=libdir x11
    path = "/lib/x86_64-linux-gnu/",
)

git_repository(
    name = "avr-bootloader-common",
    commit = "b9a639de3f4a731a5044026fd1093c8d25971685",
    remote = "https://github.com/mihaigalos/avr-bootloader-common",
    shallow_since = "1611476679 +0100",
)

go_repository(
    name = "go-ihex",
    commit = "ae68d14160fa206bb12d722cde0179356421ac8f",
    importpath = "github.com/mihaigalos/go-ihex",
)

go_repository(
    name = "go-serial",
    commit = "d70dd9211da4fb7faaf61aa700215828010c68e3",
    importpath = "github.com/mihaigalos/go-serial",
)

go_repository(
    name = "go-bar",
    commit = "441e5080442f7d749bc26e61b9ff876ec64704d0",
    importpath = "github.com/mihaigalos/go-bar",
)
