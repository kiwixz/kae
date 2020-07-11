#!/usr/bin/env python3

import multiprocessing
import os
import subprocess
import sys


def check_file(path):
    try:
        subprocess.check_output(
            ["clang-tidy", "-quiet", "-warnings-as-errors=*", "-p", "build", path], stderr=subprocess.STDOUT, text=True,
        )
        return path, None
    except subprocess.CalledProcessError as ex:
        return path, ex.output


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} directory")
        return 1

    os.chdir(sys.argv[1])

    if not os.path.exists("build/compile_commands.json"):
        print("error: could not find compile_commands.json")
        return 1

    git_files = subprocess.check_output(["git", "ls-files"], text=True).splitlines()
    todo_files = [f for f in git_files if f.endswith(".cpp") and not f.startswith("cmake/")]

    total = len(todo_files)
    done = 0
    failed = 0
    with multiprocessing.Pool(os.cpu_count() + 2) as p:
        for path, errors in p.imap_unordered(check_file, todo_files):
            done += 1
            if errors:
                print(f"\033[31;1m[{done}/{total}] failed: {path}\033[0m")
                print(errors)
                failed += 1
            else:
                print(f"[{done}/{total}] passed: {path}")

    if failed:
        print(f"\n\033[31;1m{failed} file(s) failed\033[0m")
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
