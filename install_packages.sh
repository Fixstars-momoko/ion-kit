#!/bin/sh -e

# build external packages
conan create external/halide-10
conan create external/opencv

# build ion-core
conan create ion-core

# export base package
conan export ion-bb-base

# export all BBs
for name in ion-bb/*; do
    conan export $name
done
