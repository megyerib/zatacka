#!/usr/bin/env bash
set -euo pipefail

cd $(dirname $0)
rm -f compile_commands.json
ln -s build/compile_commands.json .

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
