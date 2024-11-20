#!/bin/bash
# Last modify date: 2022-9-5 09:31:56

SCRIPT_VERSION=v1.2
DEST_IP=192.168.2.11
SSH_SCP_KEY="./pcie_zu_key"
SSH_SCP_FLAG="-o StrictHostKeyChecking=no -i ${SSH_SCP_KEY}"
# SCRIPT_INFO1="pcie_zu name&passwd is root"
SCRIPT_DIR=`dirname $0`

echo -e "$0 VER: ${SCRIPT_VERSION}"
FILE_PATH=`realpath $1`
# echo -e "$FILE_PATH"

#1, check param
if [ $# -lt 1 ] || [ ! -f ${FILE_PATH} ]; then
  echo "$0, ver: ${SCRIPT_VERSION}"
  echo "Usage: $0 <path-to-ota-package> [board-ip]"
  echo " eg1: $0 pcie_zu_fw-xxx.tar.gz"
  echo " eg2: $0 pcie_zu_fw-xxx.tar.gz 192.168.2.11"
  exit 1;
fi
echo "ota package: ${FILE_PATH}"
cd ${SCRIPT_DIR}

if [ $# -gt 1 ]; then
  DEST_IP=$2
  echo "DEST_IP: ${DEST_IP}"
fi

if [ -f ${SSH_SCP_KEY} ]; then
    chmod 600 ${SSH_SCP_KEY}
else
    echo -e "warn: can not find ${SSH_SCP_KEY}!!"
fi

# 2, check if exist only one ota file
echo -e "\n> 2, check if exist only one ota file"
OTA_FILE_NUM=`ls ${FILE_PATH} | grep "pcie_zu_fw.*.tar.gz" | wc -l`
if [ ${OTA_FILE_NUM} -ne 1 ]; then
    echo "have detect ${OTA_FILE_NUM} ota files, should only be 1, pls check! !"
    exit 1
fi

# 3, copy update file to /ldc partition
echo -e "\n> 3, copy update file to /ldc partition"
scp ${SSH_SCP_FLAG} ${FILE_PATH} root@${DEST_IP}:/ldc

# 4, run update script in pcie_zu
echo -e "\n> 4, run update script in pcie_zu"
ssh ${SSH_SCP_FLAG} root@${DEST_IP} /etc/common/ota-update.sh
ret=$?
echo -e "\npcie_zu ota ret code: $ret"

# 5, all done
if [ $ret -ne 0 ]; then
    echo -e "\e[31m pcie_zu ota failed, pls check \e[0m"
else
    echo -e "\e[32m pcie_zu ota success \e[0m"
fi

echo -e "\n> 5, all done"
exit $ret
