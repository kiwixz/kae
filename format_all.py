#!/usr/bin/env python3

import subprocess
import sys

import _utils as utils


def format_file(path):
    subprocess.check_call(["clang-format", "-i", path])

    def result(progress):
        print(f"[{progress}] formatted file: {path}")
        return True

    return result


def main():
    def sieve(path):
        return (path.endswith(".cpp") or path.endswith(".h")) and not path.startswith("cmake/")

    return utils.foreach_file(format_file, sieve)


if __name__ == "__main__":
    sys.exit(main())
