#!/usr/bin/bash
cloc --hide-rate --vcs=git --quiet --fullpath --exclude-dir=lib --not-match-f=.clang-format .
