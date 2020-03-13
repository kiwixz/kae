#!/usr/bin/env python3

import os
import subprocess
from concurrent.futures import ThreadPoolExecutor
from typing import Callable, List


def foreach_file(action: Callable[[str], Callable[[str], None]], sieve: Callable[[str], bool] = None):
    git_files = subprocess.check_output(["git", "ls-files"], text=True).splitlines()
    with ThreadPoolExecutor((os.cpu_count() or 0) + 2) as executor:
        futures = []
        count = 0
        for path in git_files:
            if sieve is not None and not sieve(path):
                continue
            futures.append(executor.submit(action, path))
            count += 1

        total = count
        count = 1
        failures = 0
        for f in futures:
            if not f.result()(f"{count}/{total}"):
                failures += 1
            count += 1
        return failures


def print_patch(patch: List[str]):
    def colorize(line: str):
        if line[0] == "+":
            return f"\033[32m{line}\033[0m"

        if line[0] == "-":
            return f"\033[31m{line}\033[0m"

        return line

    print("".join(patch[:3]))  # header
    print("".join(colorize(line) for line in patch[3:]))
