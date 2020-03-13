#!/usr/bin/env python3

import logging
import subprocess
import sys

import _utils as utils


def format_file(path):
    subprocess.check_call(["clang-format", "-i", path])

    def result(progress):
        logging.info("[%s] formatted: %s", progress, path)
        return True

    return result


def main():
    utils.init()

    def sieve(path):
        return (path.endswith(".cpp") or path.endswith(".h")) and not path.startswith("cmake/")

    return utils.foreach_file(format_file, sieve)


if __name__ == "__main__":
    sys.exit(main())
