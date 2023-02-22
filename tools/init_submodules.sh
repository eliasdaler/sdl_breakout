#!/bin/bash
git -c submodule."external/libjxl".update=none submodule update --init --recursive
