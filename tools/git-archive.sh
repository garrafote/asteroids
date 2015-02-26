#!/bin/sh

mkdir -p ./build/
git archive --format zip --output ./build/export.zip master
