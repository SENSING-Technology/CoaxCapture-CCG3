#!/bin/bash
# Last modify date: 2023-4-1

SCRIPT_VERSION=v1.2
BOARD_IP=192.168.2.11

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
if [ $# -lt 1 ]; then
  echo "Usage: $0 <net-dev> [board-ip]"
  echo "net-dev: net connect with board, check with ifconfig"
  echo "board-ip: optional, default 192.168.2.11"
  echo " eg1: $0 enp0s31f6"
  echo " eg2: $0 enp0s31f6 192.168.2.11"
  exit 1;
fi

if [ $# -gt 1 ]; then
    BOARD_IP=${2}
    echo -e "board-ip: ${2}"
fi

#2, check ptp4l & clockdiff
func_ptp_exist
func_clockdiff_exist

#3, set time to 2000-01-01
killall ntpd
killall ptpd
killall ptp4l
timedatectl set-ntp false
date -s 2000-01-01

#4, run ptp4l
ptp4l -i ${1} -S -s &

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
