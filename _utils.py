#!/usr/bin/env python3

import logging
import os
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from typing import Callable


def init():
    logging.basicConfig(
        datefmt="%H:%M:%S",
        format="[%(asctime)s.%(msecs)d][%(levelname)s][%(thread)d] %(message)s",
        level=logging.DEBUG,
        stream=sys.stderr,
    )
    os.chdir(os.path.dirname(os.path.abspath(__file__)))


def foreach_file(action: Callable[[str], Callable[[str], None]], sieve=None):
    git_files = subprocess.check_output(["git", "ls-files"]).splitlines()
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
