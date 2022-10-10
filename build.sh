#!/usr/bin/env bash

# Clone CUnit
echo "Clone CUnit"
wget https://sourceforge.net/projects/cunit/files/CUnit/2.1-3/CUnit-2.1-3.tar.bz2 -P framework/
tar xvfj framework/CUnit-2.1-3.tar.bz2 -C ./framework/
cp framework/CUnit-2.1-3/CUnit/Headers/CUnit.h.in framework/CUnit-2.1-3/CUnit/Headers/CUnit.h
