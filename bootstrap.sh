#!/usr/bin/env bash

PROJECT_DIR=`pwd`

build_deps()
{
    clean_all

    # easyloggingpp
    cd ${PROJECT_DIR}/deps
    tar xf easyloggingpp-v9.96.7.tar.gz
    ln -s easyloggingpp-9.96.7 easyloggingpp

    # json
    cd ${PROJECT_DIR}/deps
    tar xf json-3.5.0.tar.gz
    ln -s json-3.5.0 json

    # mongoose
    cd ${PROJECT_DIR}/deps
    tar xf mongoose-v6.7.tar.gz
    ln -s mongoose-6.7 mongoose

    # libuv
    cd ${PROJECT_DIR}/deps
    tar xf libuv-v1.25.0.tar.gz
    cd libuv-v1.25.0
    mkdir build && cd build
    cmake -DCMAKE_INSTALL_PREFIX=${PROJECT_DIR}/deps/libuv ..
    make -j && make install

    # gtest
    cd ${PROJECT_DIR}/deps
    tar xf googletest-1.8.1.tar.gz
    cd googletest-1.8.1
    mkdir build && cd build
    cmake -DCMAKE_INSTALL_PREFIX=${PROJECT_DIR}/deps/gtest ..
    make -j && make install
}

build_all()
{
    rm -rf build && mkdir build && cd build && cmake .. && make -j
}

run_tests()
{
    cd build && make test
}

clean_all()
{
    rm -rf ${PROJECT_DIR}/build
    rm -rf ${PROJECT_DIR}/deps/gtest
    rm -rf ${PROJECT_DIR}/deps/googletest-1.8.1
    rm -rf ${PROJECT_DIR}/deps/libuv
    rm -rf ${PROJECT_DIR}/deps/libuv-v1.25.0
    rm -rf ${PROJECT_DIR}/deps/easyloggingpp
    rm -rf ${PROJECT_DIR}/deps/easyloggingpp-9.96.7
    rm -rf ${PROJECT_DIR}/deps/json
    rm -rf ${PROJECT_DIR}/deps/json-3.5.0
    rm -rf ${PROJECT_DIR}/deps/mongoose
    rm -rf ${PROJECT_DIR}/deps/mongoose-6.7
}

if [ $# == 1 ]; then
    if [ $1 = "clean" ]; then
        clean_all
    elif [ $1 = "deps" ]; then
        build_deps
    elif [ $1 = "build" ]; then
        build_all
    elif [ $1 = "test" ]; then
        run_tests
    fi
    exit 0
fi

printf "Usage:\n\t./bootstrap.sh deps  编译依赖\n\t./bootstrap.sh build 编译程序\n\t./bootstrap.sh test  运行测试\n\t./bootstrap.sh clean 清理\n\n"

