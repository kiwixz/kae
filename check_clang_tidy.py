#!/usr/bin/env python3

import logging
import subprocess
import sys
from pathlib import Path

import _utils as utils


def check(build_dir, path):
    errors = None
    try:
        subprocess.check_output(
            ["clang-tidy", "-quiet", "-p", build_dir, "-warnings-as-errors", "*", path],
            stderr=subprocess.STDOUT,
            text=True,
        )
    except subprocess.CalledProcessError as ex:
        errors = ex.output

    def result(progress):
        if errors:
            print(f"\033[31m[{progress}] errors found for file: {path}\033[0m")
            print(errors)
        else:
            print(f"\033[32m[{progress}] no errors in file: {path}\033[0m")

        return not errors

    return result


def main():
    build_dir = sys.argv[1] if len(sys.argv) > 1 else "build"
    if not (Path(build_dir) / "compile_commands.json").exists():
        logging.critical("compile_commands.json not found, did you pass the correct build directory ?")
        return 2

    return utils.foreach_file(lambda path: check(build_dir, path), lambda path: path.endswith(".cpp"))


if __name__ == "__main__":
    sys.exit(main())
