#!/bin/bash
# Last modify date: 2022-12-12

SCRIPT_VERSION=v1.2
BOARD_IP=192.168.2.11
SWITCH_TTY=/dev/ttyUSB0
PERIOD_TIME_S=0.1
PERIOD_CNT=5

func_switch_on() {
    sudo ./uart_single -t -s 1 -p ${SWITCH_TTY} > /dev/null
    # sudo ./uart_single -t -s 1 -p ${SWITCH_TTY} > /dev/null
    # sudo ./uart_single -t -s 1 -p ${SWITCH_TTY} > /dev/null
}

func_switch_off() {
    sudo ./uart_single -t -s 0 -p ${SWITCH_TTY} > /dev/null
    # sudo ./uart_single -t -s 0 -p ${SWITCH_TTY} > /dev/null
    # sudo ./uart_single -t -s 0 -p ${SWITCH_TTY} > /dev/null
}


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
echo -e "PERIOD_TIME_S: ${PERIOD_TIME_S}"
echo -e "PERIOD_CNT: ${PERIOD_CNT}"

func_switch_off
sleep 1

all_cnt=0
while [ 1 ]
do
    echo -e "\n\n===========cnt: ${all_cnt}========\n\n"

    echo -e ">>>1, powerup"
    power_cnt=1
    while [ ${power_cnt} -ne ${PERIOD_CNT} ]
    do
        echo -e "\n>a, powerup[${power_cnt}], sleep ${PERIOD_TIME_S}s"
        func_switch_on
        sleep ${PERIOD_TIME_S}
        func_switch_off
        let power_cnt+=1
    done

    sleep ${PERIOD_TIME_S}
    echo -e "\n>b, powerup sleep 10s"
    func_switch_on
    sleep 10

    echo -e "\n>>>2, check ping"
    func_check_ping

    echo -e "\n>>>3, powerdown, sleep 10s"
    func_switch_off
    sleep 10
    let all_cnt+=1
done

