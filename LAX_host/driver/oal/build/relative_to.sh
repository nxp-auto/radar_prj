#!/bin/bash
##############################################################################
#
# Copyright 2017-2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
##############################################################################

usage()
{
	script=$1
	echo "This script prints the resolved path relative to a file / directory"
	echo "Usage: $script <ref-dir> <file-path>"
	echo
	echo "The script will determine the relative path of <file-path> to <ref-dir>."
	echo "Its equivalent: realpath --relative-to=<ref-dir> <file-path>"
}

_common_subpath()
{
	local first="$1"
	local second="$2"
	local common="$3"
	local b_first="$(basename $first)"

	if [ "$b_first" = "$(basename $second)" ] &&
		[ "$(basename $first)" != "/" ]
	then
		_common_subpath "$(dirname $first)" "$(dirname $second)" "$b_first/$common"
	else
		echo "$(rev <<< $common)"
	fi
}

common_subpath()
{
	local first=$1
	local second=$2
	echo $(_common_subpath "$(rev <<< $first)" "$(rev <<< $second)" "")
}

relative_path()
{
	local relative_to=$1
	local file=$2

	local common=$(common_subpath "$relative_to" "$file")
	r=${relative_to##$common}
	# Keep only /
	r=${r//[^\/]/}
	# Replace / with ../
	r=${r//[\/]/../}
	r=$(dirname "$r")
	# dots (../../) + file without common part
	echo "$r${file##$common}"
}

if [ -z "$1" ] || [ -z "$2" ]
then
	usage $0
	exit 1
fi

folder=$1
shift

while test ${#} -gt 0
do
	relative_path "$folder" "$1"
	shift
done

#relative_path /work/workspaces/oal_test/tests/oal_timer/build-linux-kernel/ /work/workspaces/oal_test/oal/libs/kernel/linux-netlink/build-linux-kernel/liboal_kernel.a
