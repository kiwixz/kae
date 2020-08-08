#!/usr/bin/env python3

import difflib
import multiprocessing
import os
import subprocess
import sys


CLANG_FORMAT = os.getenv("CLANG_FORMAT", "clang-format")


def check_file(path):
    with open(path) as f:
        original = f.read()
    formatted = subprocess.check_output([CLANG_FORMAT, "-assume-filename", path], input=original, text=True)
    diff = difflib.unified_diff(original.splitlines(), formatted.splitlines(), path, "formatted", lineterm="")
    return path, list(diff)


def print_patch(patch):
    def colorize(line):
        if line[0] == "+":
            return f"\033[32m{line}\033[0m"
        elif line[0] == "-":
            return f"\033[31m{line}\033[0m"
        else:
            return line

    print("\n".join(patch[:3]))  # header
    print("\n".join(colorize(line) for line in patch[3:]))


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} directory")
        return 1

    os.chdir(sys.argv[1])

    git_files = subprocess.check_output(["git", "ls-files"], text=True).splitlines()
    todo_files = [f for f in git_files if (f.endswith(".cpp") or f.endswith(".h")) and not f.startswith("cmake/")]
    total = len(todo_files)
    done = 0
    failed = 0
    with multiprocessing.Pool(min(total, os.cpu_count() + 2)) as p:
        for path, patch in p.imap_unordered(check_file, todo_files):
            done += 1
            if patch:
                print(f"\033[31;1m[{done}/{total}] failed: {path}\033[0m")
                print_patch(patch)
                failed += 1
            else:
                print(f"[{done}/{total}] passed: {path}")

    if failed:
        print(f"\n\033[31;1m{failed} file(s) failed\033[0m")
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
