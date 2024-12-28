#!/bin/bash
# Last modify date: 2022-12-8

SCRIPT_VERSION=v1.0
BOARD_IP=192.168.2.11
OTA_PACKAGE_0=/home/neousys/peta_prj/fw/pcie_zu_fw-1.3.42-20221209.tar.gz
OTA_PACKAGE_1=/home/neousys/peta_prj/fw/pcie_zu_fw-1.3.43-20221209-RC1.tar.gz
PACKAGE_INDEX=0

func_check_ping() {
    try_cnt=0
    ping -c 1 ${BOARD_IP}
    ret=$?
    echo "ping check[${try_cnt}], ret: ${ret}"
    while [ ${ret} -ne 0 ]
    do
        sleep 2
        let try_cnt+=1
        ping -c 1 ${BOARD_IP}
        ret=$?
        echo "ping check[${try_cnt}], ret: ${ret}"
    done
    echo "ping ok!"
}

func_otaupdate() {
    package_path=/home
    if [ ${PACKAGE_INDEX} -eq 0 ]; then
        package_path=${OTA_PACKAGE_0}
        PACKAGE_INDEX=1
    else
        package_path=${OTA_PACKAGE_1}
        PACKAGE_INDEX=0
    fi

    echo "package_path: ${package_path}"
    ota_cnt=0
    ./pcie_zu-update.sh ${package_path}
    ret=$?
    echo "otaupdate[${ota_cnt}], ret: ${ret}"
    while [ ${ret} -ne 0 ]
    do
        sleep 5
        let ota_cnt+=1
        ./pcie_zu-update.sh ${package_path}
        ret=$?
        echo "otaupdate[${ota_cnt}], ret: ${ret}"
    done
    echo "otaupdate ok!"
}

echo -e "$0 VER: ${SCRIPT_VERSION}"

all_cnt=0
while [ 1 ]
do
    echo -e "\n\n===========cnt: ${all_cnt}========\n\n"
    func_check_ping
    func_otaupdate
    ./reset-by-eth.sh
    echo "wait for 15s..."
    sleep 15
    let all_cnt+=1
done

