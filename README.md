# README #

lagom is an ncruses based log viewer with regex filtering/search capabilities and configurable tokenization (broken right now).

## Getting started ##

Make sure that you have gcc 5.0+ in your shell environment.

**Clone repo**

git clone git@github.com:jolt80/lagom.git

**Prepare submodules**

*The repository contains a few submodules, these have to be intialized and updated.*

git submodule init    

git submodule update

*Build the external libraries*

make lib-deps

# Make rules

make realclean - really, really removes everything including the extrenal libraries

make clean     - removes all lagom objects files but keeps the extrenal compiled external libs

make all       - builds lagom release version

make test      - builds and runs unit-tests