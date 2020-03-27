#!/usr/bin/env python3

import difflib
import subprocess
import sys

import _utils as utils


def check(path):
    with open(path) as f:
        original = f.read()
    formatted = subprocess.check_output(["clang-format", "-assume-filename", path], input=original, text=True)
    diff = difflib.unified_diff(
        original.splitlines(keepends=True), formatted.splitlines(keepends=True), path, "formatted"
    )
    patch = list(diff)

    def result(progress):
        if patch:
            print(f"\033[31m[{progress}] failed: {path}\033[0m")
            utils.print_patch(patch)
        else:
            print(f"\033[32m[{progress}] passed: {path}\033[0m")

        return not patch

    return result


def main():
    def sieve(path):
        return (path.endswith(".cpp") or path.endswith(".h")) and not path.startswith("cmake/")

    return utils.foreach_file(check, sieve)


if __name__ == "__main__":
    sys.exit(main())
