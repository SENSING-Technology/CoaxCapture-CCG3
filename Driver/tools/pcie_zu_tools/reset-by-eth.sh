#!/bin/bash
DEST_IP=192.168.2.11
SSH_SCP_KEY="./pcie_zu_key"
SSH_SCP_FLAG="-o StrictHostKeyChecking=no -i ${SSH_SCP_KEY}"
SCRIPT_DIR=`dirname $0`

echo -e "1, reboot\n"
cd ${SCRIPT_DIR}
ssh ${SSH_SCP_FLAG} -t root@${DEST_IP} "/sbin/reboot"

echo -e "2, done\n"