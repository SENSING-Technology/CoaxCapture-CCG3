#!/bin/bash
# Last modify date: 2023-5-4

SCRIPT_VERSION=v2.1-20230504
NET_DEV=eth0
BOARD_IP=192.168.2.11
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
if [ $# -lt 2 ]; then
  echo "Usage: $0 <net-dev> <board-ip> [ptp-mode]"
  echo "net-dev: net connect with board, check with ifconfig"
  echo "board-ip: e.g. 192.168.2.11"
  echo "ptp-mode: optional, E2E/P2P/gPTP, default E2E"
  echo " eg1: $0 enp0s31f6 192.168.2.11"
  echo " eg2: $0 enp0s31f6 192.168.2.11 P2P"
  exit 1;
fi

NET_DEV=${1}
echo -e "net-dev: ${1}"
BOARD_IP=${2}
echo -e "board-ip: ${2}"

if [ $# -gt 2 ]; then
  PTP_MODE=${3}
fi
echo -e "ptp-mode: ${PTP_MODE}"

#2, check ptp4l & clockdiff
func_ptp_exist
func_clockdiff_exist

#3, set time to 2023-01-01
killall ntpd
killall ptpd
killall ptp4l
date -s 2023-01-01

#4, run ptp4l
if [ x"${PTP_MODE}" == x"E2E" ]; then
  ptp4l -i ${NET_DEV} -E -S -s &
elif [ x"${PTP_MODE}" == x"P2P" ]; then
  ptp4l -i ${NET_DEV} -P -S -s &
elif [ x"${PTP_MODE}" == x"gPTP" ]; then
  ptp4l -i ${NET_DEV} -P -S -2 -f ${SCRIPT_DIR}/automotive-slave.cfg &
else
  echo -e "error ptp-mode: ${PTP_MODE}, exit!"
  exit 1
fi

#5, check timediff with ps
try_cnt=0
clockdiff ${BOARD_IP}
clockdelta=`clockdiff ${BOARD_IP} | awk '{print $3}'`
delta_ms=$(echo ${clockdelta} | sed s'/^[+-]//g')
echo "==${try_cnt}: ${clockdelta}, ${delta_ms}"
while [ ${delta_ms} -gt "1" ]
do
  sleep 2
  let try_cnt+=1
  clockdiff ${BOARD_IP}
  clockdelta=`clockdiff ${BOARD_IP} | awk '{print $3}'`
  delta_ms=$(echo ${clockdelta} | sed s'/^[+-]//g')
  echo "==${try_cnt}: ${clockdelta}, ${delta_ms}"
done
echo "timesync clockdelta: ${clockdelta}, accept!"

#6, all done
echo -e "\n> 6, all done"
