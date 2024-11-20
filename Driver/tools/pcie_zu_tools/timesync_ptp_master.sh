#!/bin/bash
# Last modify date: 2023-5-4

SCRIPT_VERSION=v2.0-20230504
NET_DEV=eth0
PTP_MODE=E2E
SCRIPT_DIR=`dirname $0`

func_clockdiff_exist() {
    which "clockdiff" > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "clockdiff not exist, install"
        apt-get install iputils-clockdiff
    fi
}

func_ptp_exist() {
    which "ptpd" > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "ptpd not exist, install"
        apt-get install ptpd
    fi

    which "ptp4l" > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "ptp4l not exist, install"
        apt-get install linuxptp
    fi
}

echo -e "$0 VER: ${SCRIPT_VERSION}"

#0, check root
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

#1, check param
if [ $# -lt 1 ]; then
  echo "Usage: $0 <net-dev> [ptp-mode]"
  echo "net-dev: net connect with board, check with ifconfig"
  echo "ptp-mode: optional, E2E/P2P/gPTP, default E2E"
  echo " eg1: $0 enp0s31f6"
  echo " eg2: $0 enp0s31f6 gPTP"
  exit 1;
fi

NET_DEV=${1}
echo -e "net-dev: ${1}"

if [ $# -gt 1 ]; then
  PTP_MODE=${2}
fi
echo -e "ptp-mode: ${PTP_MODE}"

#2, check ptp4l & clockdiff
func_ptp_exist
func_clockdiff_exist

#3, clear time app in system
killall ntpd
killall ptpd
killall ptp4l

#4, run ptp
if [ x"${PTP_MODE}" == x"E2E" ]; then
    # ptp4l -i ${1} -E -S &
    ptpd -C -E -M -i ${1} &
elif [ x"${PTP_MODE}" == x"P2P" ]; then
    # ptp4l -i ${1} -E -S &
    ptpd -C -P -M -i ${1} &
elif [ x"${PTP_MODE}" == x"gPTP" ]; then
  ptp4l -i ${NET_DEV} -P -S -2 -f ${SCRIPT_DIR}/automotive-master.cfg &
else
  echo -e "error ptp-mode: ${PTP_MODE}, exit!"
  exit 1
fi

#6, all done
echo -e "\n> 6, all done"
