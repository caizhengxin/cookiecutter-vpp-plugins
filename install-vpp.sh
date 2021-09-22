#!/bin/bash
# @Author: jankincai
# @Date:   2021-09-22 14:01:47
# @Last Modified by:   jankincai
# @Last Modified time: 2021-09-22 14:03:07


deb [trusted=yes] https://packagecloud.io/fdio/master/ubuntu bionic main
curl -L https://packagecloud.io/fdio/master/gpgkey | sudo apt-key add -


sudo apt-get update
sudo apt-get install vpp vpp-plugin-core vpp-plugin-dpdk
sudo apt-get install python3-vpp-api vpp-dbg vpp-dev

