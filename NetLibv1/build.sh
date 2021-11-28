#!/bin/sh
###
 # @Author: your name
 # @Date: 2021-11-15 20:41:19
 # @LastEditTime: 2021-11-15 23:02:28
 # @LastEditors: Please set LastEditors
 # @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 # @FilePath: /NetLib/build.sh
### 

set -x
# set -x 表示指令结束后， 会显示以下指令

SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-./build}
BUILD_TYPE=${BUILD_TYPE:-release}
INSTALL_DIR=${INSTALL_DIR:-${BUILD_DIR}/${BUILD_TYPE}-install-cpp11}
# INSTALL_DIR="/usr/"
CXX=${CXX:-g++}
echo ${BUILD_DIR}
echo ${BUILD_TYPE}
echo ${INSTALL_DIR}
ln -sf $BUILD_DIR/$BUILD_TYPE-cpp11/compile_commands.json

mkdir -p $BUILD_DIR/$BUILD_TYPE-cpp11 \
  && cd $BUILD_DIR/$BUILD_TYPE-cpp11 \
  && cmake ..\
           -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
           -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
           -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           $SOURCE_DIR \
  && make $*

# INSTALL_DIR 