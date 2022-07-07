#!/bin/bash

find . -type f -iname \*.cpp -o -iname \*.hpp | xargs clang-format -i
