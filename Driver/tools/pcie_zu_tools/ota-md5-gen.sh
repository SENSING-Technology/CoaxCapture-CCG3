#!/bin/bash

SCRIPT_VERSION=v1.0
FW_DIR=`pwd`/../image/linux

#1, check param
if [ $# -lt 1 ] ; then
    echo "Usage: $0 <version> [firmware_path]"
    echo " e.g.: $0 v1.13-12"
    exit 1;
fi

echo -e "$0 VER: $SCRIPT_VERSION"

if [ $# -ge 2 ] ; then
    FW_DIR=$2
fi

FW_PATH_OUTPUT=${FW_DIR}/pcie_zu_ota.json
FW_PATH_BOOT=${FW_DIR}/BOOT.BIN
FW_PATH_SCR=${FW_DIR}/boot.scr
FW_PATH_IMG=${FW_DIR}/image.ub

MD5_BOOT=`md5sum ${FW_PATH_BOOT} | cut -d ' ' -f 1`
MD5_SCR=`md5sum ${FW_PATH_SCR} | cut -d ' ' -f 1`
MD5_IMG=`md5sum ${FW_PATH_IMG} | cut -d ' ' -f 1`


# DATE=20200117
DATE=$(date +%F | sed 's/-//g')

echo "Start generate ota json..."

echo -e '{
    "fw": "'$1'",
    "BOOT_BIN": {
        "md5": "'${MD5_BOOT}'",
        "date": "'${DATE}'"
    },
    "boot_scr": {
        "md5": "'${MD5_SCR}'",
        "date": "'${DATE}'"
    },
    "image_ub": {
        "md5": "'${MD5_IMG}'",
        "date": "'${DATE}'"
    }
}' > ${FW_PATH_OUTPUT}


