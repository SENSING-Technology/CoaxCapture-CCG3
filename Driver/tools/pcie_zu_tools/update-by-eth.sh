#!/bin/bash
DEST_IP=192.168.2.11
SSH_SCP_KEY="./pcie_zu_key"
SSH_SCP_FLAG="-o StrictHostKeyChecking=no -i ${SSH_SCP_KEY}"

echo -e "1, copy new boot to /data\n"
cd ../images/linux/
scp ${SSH_SCP_FLAG} BOOT.BIN boot.scr image.ub root@${DEST_IP}:/data

echo -e "2, remount /boot and mv from /data to /boot\n"
ssh ${SSH_SCP_FLAG} -t root@${DEST_IP} "
	umount /boot;
	/sbin/mkfs.vfat /dev/mmcblk0p1;
	fsck.fat -a /dev/mmcblk0p1;
	mount /dev/mmcblk0p1 /boot;
	cd /data;
	rm -rf /boot/*
	mv BOOT.BIN boot.scr image.ub /boot;
	mount -o remount,ro /boot;
	sync;"
ssh ${SSH_SCP_FLAG} -t root@${DEST_IP} "md5sum /boot/*"
md5sum ./BOOT.BIN boot.scr image.ub

echo -e "3, done!\n"
