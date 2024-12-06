export LD_LIBRARY_PATH=/usr/lib:/lib:/home/root:./.

echo "Using static libs"
./strong_first
./weak_first

echo "Using dynamic libs"
./strong_first_link_so
./weak_first_link_so