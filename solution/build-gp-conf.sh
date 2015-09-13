#!/bin/bash

ROOT=${PWD}/../..
export PATH=${ROOT}/toolchains/aarch32/bin:$PATH

export TA_DEV_KIT_DIR=${ROOT}/optee_os/out/arm-plat-vexpress/export-user_ta
export TEEC_EXPORT=${ROOT}/optee_client/out/export
export PLATFORM=vexpress
export PLATFORM_FLAVOR=qemu_virt

cd ${ROOT}/gp_conf/solution
make O=${ROOT}/out/gp_conf_ta/solution \
                HOST_CROSS_COMPILE=arm-linux-gnueabihf- \
                TA_CROSS_COMPILE=arm-linux-gnueabihf- \
                $@
