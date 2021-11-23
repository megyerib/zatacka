#!/usr/bin/env bash

# simple build date if we are not in a Git repository
if [[ -z "$(git rev-parse --git-dir 2> /dev/null)" ]]; then
	echo -n "$(date +%Y-%m-%d)"
	exit
fi

tag=$(git describe --exact-match --tags 2>/dev/null)

# tag or branch/hash or hash
if [[ $? -eq 0 ]]; then
    echo -n "$tag"
else
    branch=$(git rev-parse --abbrev-ref HEAD)
    hash=$(git rev-parse --short HEAD)
    if [[ $branch == "HEAD" ]]; then
        echo -n "$hash"
    else
        echo -n "$branch/$hash"
    fi
fi

# other arguments such as 'debug'
if [[ ! -z "$*" ]]; then
    echo -n " $*"
fi

# 'unstable' if working directory is not clean
if [[ ! -z "$(git status --porcelain)" ]]; then 
    echo -n " unstable"
fi

# date - YYYY-MM-DD
echo -n " - $(date +%Y-%m-%d)"
