#!/bin/bash
# Last modify date: 2022-12-12

SCRIPT_VERSION=v1.1
BOARD_IP=192.168.2.11
SWITCH_TTY=/dev/ttyUSB0

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


echo -e "$0 VER: ${SCRIPT_VERSION}"

#0, check root
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

if [ $# -gt 0 ]; then
    SWITCH_TTY=${1}
fi
echo -e "device: ${SWITCH_TTY}"
sudo ./uart_single -t -s 0 -p ${SWITCH_TTY}
sleep 1

all_cnt=0
while [ 1 ]
do
    echo -e "\n\n===========cnt: ${all_cnt}========\n\n"
    echo -e ">>>1, powerup, sleep 10s"
    sudo ./uart_single -t -s 1 -p ${SWITCH_TTY}
    sleep 10

    echo -e ">>>2, check ping"
    func_check_ping

    echo -e ">>>3, powerdown, sleep 10s"
    sudo ./uart_single -t -s 0 -p ${SWITCH_TTY}
    sleep 10
    let all_cnt+=1
done

