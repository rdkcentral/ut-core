#/bin/bash

cd "$(dirname "$0")"
export LD_LIBRARY_PATH=/usr/lib:/lib:/home/root
./hal_test $@
