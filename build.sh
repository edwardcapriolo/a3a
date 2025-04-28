export SEA_DIR=~/seastar
export APP=`pwd`
cd build
cmake -DCMAKE_PREFIX_PATH="$SEA_DIR/build/release;$SEA_DIR/build/release/_cooking/installed" -DCMAKE_MODULE_PATH=$SEA_DIR/cmake $APP
make
