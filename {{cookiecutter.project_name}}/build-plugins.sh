#!/bin/bash
# @Author: jankincai
# @Date:   2021-09-03 09:22:57
# @Last Modified by:   jankincai
# @Last Modified time: 2021-09-22 11:43:55
set -e

CURDIR=$(cd "$(dirname "$0")"; pwd)
vpp_dir=~/vpp

if [ ! -d $vpp_dir ]; then
    echo "[DEBUG]: VPP项目不存在，正在clone vpp代码"
    git clone https://gerrit.fd.io/r/vpp ~/vpp
fi

cp -r $CURDIR $vpp_dir/src/plugins/


if [ ! -d $vpp_dir/src/build ]; then
    mkdir $vpp_dir/src/build
fi

cd $vpp_dir/src/build

cmake ..
make -j 3

if [ ! -f /usr/lib/x86_64-linux-gnu/vpp_plugins/"${CURDIR##*/}"_plugin.so ]; then
    sudo ln -s $vpp_dir/src/build/lib/vpp_api_test_plugins/"${CURDIR##*/}"_test_plugin.so /usr/lib/x86_64-linux-gnu/vpp_api_test_plugins
    sudo ln -s $vpp_dir/src/build/lib/vpp_plugins/"${CURDIR##*/}"_plugin.so /usr/lib/x86_64-linux-gnu/vpp_plugins/
fi
