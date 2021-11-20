#!/usr/bin/env bash

set -eu

gdb=""

while getopts ":d" opt; do
    case "${opt}" in
        d)
            gdb="gdb"
            ;;
        *)
            ;;
    esac
done

$gdb build/zatacka
