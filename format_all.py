#!/usr/bin/env python3

import multiprocessing
import os
import subprocess


def format_file(path):
    subprocess.check_call(["clang-format", "-i", path])


def main():
    os.chdir(os.path.dirname(os.path.abspath(__file__)))

    git_files = subprocess.check_output(["git", "ls-files"], text=True).splitlines()
    todo_files = [f for f in git_files if (f.endswith(".cpp") or f.endswith(".h")) and not f.startswith("cmake/")]
    with multiprocessing.Pool(os.cpu_count() + 2) as p:
        p.map(format_file, todo_files)
    print(f"formatted {len(todo_files)} files")


if __name__ == "__main__":
    main()
