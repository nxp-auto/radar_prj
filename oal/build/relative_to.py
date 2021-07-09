#!/usr/bin/python
##############################################################################
#
# Copyright 2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
##############################################################################

"""
 Simple script to be used by build system that determines
 the relative path of a set of files
 run -h for usage details
"""

import argparse
from os.path import relpath


ARGPARSER = argparse.ArgumentParser()
ARGPARSER.add_argument("relative_to", help="Resolve file name relative to this folder", type=str)
ARGPARSER.add_argument("file", help="File to resolve", type=str, nargs='+')

ARGS = ARGPARSER.parse_args()

print (" ".join([relpath(file_path, ARGS.relative_to) for file_path in ARGS.file]))
