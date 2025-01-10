#!/bin/bash
# mipi disable

tool_path=../tools
std_out=init_card.log
#std_out=/dev/NULL
date  > $std_out


# set device
device_array[0]=/dev/xdma0_user 
device_array[1]=/dev/xdma1_user
device_array[2]=/dev/xdma2_user
device_array[3]=/dev/xdma3_user
dev=${device_array[0]}

function set_card () {
    device_num=$1
    #echo $device_num
    if [ $device_num -eq 0 ] 
    then
        dev=${device_array[0]}
    elif [ $device_num -eq 1 ]
    then
        dev=${device_array[1]}
    elif [ $device_num -eq 2 ]
    then
        dev=${device_array[2]}
    else
        dev=${device_array[3]}
    fi
}

result="0x00000000"
function wait_pl_ps_sync () {
	#result="0x00000000"
	loop_count=1

	$tool_path/reg_rw $dev 0x40024 wp >output
	result=`awk 'NR==4{print $8}' output`
		
	while [ $(($result >> 3)) -ne 15 ]	
	do
		sleep 0.1
		$tool_path/reg_rw $dev 0x40024 wp >output
		result=`awk 'NR==4{print $8}' output`
		loop_count=`expr $loop_count + 1`
		if [ $loop_count -gt 100 ]
			then
			break
		fi
	done
	rm ./output
}

function ptp_timesync () {
    wait_pl_ps_sync
    if [ $(($result >> 3)) -eq 15 ]	
        then
        echo "pl and ps has sync !"
        ./timesync_pc_ptp.sh $1
        else
        echo "pl ps sync failed!"
    fi
}

function mipi_disable () {
    $tool_path/serdes_cfg $dev 100 "i2ctransfer -f -y 1 w3@0x48 0x03 0x13 0x00"  >> $std_out
    $tool_path/serdes_cfg $dev 100 "i2ctransfer -f -y 1 w3@0x48 0x03 0x13 0x00"  >> $std_out
    $tool_path/serdes_cfg $dev 100 "i2ctransfer -f -y 2 w3@0x48 0x03 0x13 0x00" >> $std_out
    $tool_path/serdes_cfg $dev 100 "i2ctransfer -f -y 3 w3@0x48 0x03 0x13 0x00" >> $std_out
    $tool_path/serdes_cfg $dev 100 "i2ctransfer -f -y 4 w3@0x48 0x03 0x13 0x00" >> $std_out
    $tool_path/serdes_cfg $dev 100 "i2ctransfer -f -y 5 w3@0x48 0x03 0x13 0x00" >> $std_out
    $tool_path/serdes_cfg $dev 100 "i2ctransfer -f -y 6 w3@0x48 0x03 0x13 0x00" >> $std_out
    $tool_path/serdes_cfg $dev 100 "i2ctransfer -f -y 7 w3@0x48 0x03 0x13 0x00" >> $std_out
    $tool_path/serdes_cfg $dev 100 "i2ctransfer -f -y 8 w3@0x48 0x03 0x13 0x00" >> $std_out
    sleep 0.1

    #9296 power down
    $tool_path/reg_rw $dev 0x30140 b 0x00 >> $std_out 
    sleep 0.5
    # PCIE reset
    $tool_path/reg_rw $dev 0x30004 w 0xff >> $std_out 
    sleep 0.5
    #9296 power on
    $tool_path/reg_rw $dev 0x30140 b 0xff >> $std_out 
    sleep 1
}

function camera_anc_set () {
    format_vaule=$8
    format_vaule=`expr $7 + $(($format_vaule << 4))`
    format_vaule=`expr $6 + $(($format_vaule << 4))`
    format_vaule=`expr $5 + $(($format_vaule << 4))`
    format_vaule=`expr $4 + $(($format_vaule << 4))`
    format_vaule=`expr $3 + $(($format_vaule << 4))`
    format_vaule=`expr $2 + $(($format_vaule << 4))`
    format_vaule=`expr $1 + $(($format_vaule << 4))`
    #echo $format_vaule
    $tool_path/reg_rw $dev 0x30010 wp $format_vaule >> $std_out 
}

function image_input_format () {
    input_type=$1
    $tool_path/reg_rw $dev 0x30008 w $input_type >> $std_out 
}

function camera_format_set () {
    format_vaule=$8
    format_vaule=`expr $7 + $(($format_vaule << 4))`
    format_vaule=`expr $6 + $(($format_vaule << 4))`
    format_vaule=`expr $5 + $(($format_vaule << 4))`
    format_vaule=`expr $4 + $(($format_vaule << 4))`
    format_vaule=`expr $3 + $(($format_vaule << 4))`
    format_vaule=`expr $2 + $(($format_vaule << 4))`
    format_vaule=`expr $1 + $(($format_vaule << 4))`
    #echo $format_vaule
    $tool_path/reg_rw $dev 0x30008 wp $format_vaule >> $std_out 
}

function video_output_yuv_format () {
    device=/dev/video$1
    type=$2
    $tool_path/video_rw $device "$type" >> $std_out
}

function camera_pps_ptpmode () {
    pps_ptpmode=$1
    case $pps_ptpmode in
    "0")
    mode_value=0x00000000
    ;;
    "1")
    mode_value=0x00000001
    ;;
    "2")
    mode_value=0x00000002
    ;;
    "3")
    mode_value=0x00000003
    ;;
    *)
    echo "camera_pps_ptpmode Invalid parameters"
    exit 1
    ;;
    esac
    echo $mode_value
    $tool_path/reg_rw $dev 0x30014 w $mode_value >> $std_out
    #sleep 0.5
}

function timestamp_src () {
    stamp_src=$1
    case $stamp_src in
    "0")
    mode_value=0x00000000
    ;;
    "1")
    mode_value=0x00000001
    ;;
    "2")
    mode_value=0x00000002
    ;;
    *)
    echo "timestamp_src Invalid parameters"
    exit 1
    ;;
    esac
    #echo $mode_value
    $tool_path/reg_rw $dev 0x3016c w $mode_value >> $std_out
    #sleep 0.5
}

function card_trigger_signal_mode () {
    mode_config=$1
    case $mode_config in
    "0")
    mode_value=0x00000000
    ;;
    "1")
    mode_value=0x11111111
    ;;
    "2")
    mode_value=0x22222222
    ;;
    "3")
    mode_value=0x33333333
    ;;
    *)
    echo "card_trigger_signal_mode Invalid parameters"
    exit 1
    ;;
    esac
    #echo $mode_value
    $tool_path/reg_rw $dev 0x30020 w $mode_value >> $std_out
    sleep 0.5
} 

function card_external_signal_input_fps () {
    ext_trigger_value=`expr 300000000 / $1`
    $tool_path/reg_rw $dev 0x30164 w $ext_trigger_value >> $std_out 
} 

function camera_inner_output_fps () {
    inner_fps=`expr 300000000 / $1`
    $tool_path/reg_rw $dev 0x30024 w $inner_fps >> $std_out 
    $tool_path/reg_rw $dev 0x30028 w $inner_fps >> $std_out 
    $tool_path/reg_rw $dev 0x3002c w $inner_fps >> $std_out 
    $tool_path/reg_rw $dev 0x30030 w $inner_fps >> $std_out 
    $tool_path/reg_rw $dev 0x30034 w $inner_fps >> $std_out 
    $tool_path/reg_rw $dev 0x30038 w $inner_fps >> $std_out 
    $tool_path/reg_rw $dev 0x3003c w $inner_fps >> $std_out 
    $tool_path/reg_rw $dev 0x30040 w $inner_fps >> $std_out 
}

function camera_external_output_fps () {
    external_fps=`expr 300000000 / $1`
    $tool_path/reg_rw $dev 0x30144 w $external_fps >> $std_out 
    $tool_path/reg_rw $dev 0x30148 w $external_fps >> $std_out 
    $tool_path/reg_rw $dev 0x3014c w $external_fps >> $std_out 
    $tool_path/reg_rw $dev 0x30150 w $external_fps >> $std_out 
    $tool_path/reg_rw $dev 0x30154 w $external_fps >> $std_out 
    $tool_path/reg_rw $dev 0x30158 w $external_fps >> $std_out 
    $tool_path/reg_rw $dev 0x3015c w $external_fps >> $std_out 
    $tool_path/reg_rw $dev 0x30160 w $external_fps >> $std_out 
} 

function trigger_pulse () {
    $tool_path/reg_rw $dev 0x30064 w 1000 >> $std_out 
    $tool_path/reg_rw $dev 0x30068 w 1000 >> $std_out 
    $tool_path/reg_rw $dev 0x3006c w 1000 >> $std_out 
    $tool_path/reg_rw $dev 0x30070 w 1000 >> $std_out 
    $tool_path/reg_rw $dev 0x30074 w 1000 >> $std_out 
    $tool_path/reg_rw $dev 0x30078 w 1000 >> $std_out 
    $tool_path/reg_rw $dev 0x3007c w 1000 >> $std_out 
    $tool_path/reg_rw $dev 0x30080 w 1000 >> $std_out 
}

function trigger_delay () {
    $tool_path/reg_rw $dev 0x30044 w $1 >> $std_out 
    $tool_path/reg_rw $dev 0x30048 w $2 >> $std_out 
    $tool_path/reg_rw $dev 0x3004c w $3 >> $std_out 
    $tool_path/reg_rw $dev 0x30050 w $4 >> $std_out 
    $tool_path/reg_rw $dev 0x30054 w $5 >> $std_out 
    $tool_path/reg_rw $dev 0x30058 w $6 >> $std_out 
    $tool_path/reg_rw $dev 0x3005c w $7 >> $std_out 
    $tool_path/reg_rw $dev 0x30060 w $8 >> $std_out 
}

function camera_resolution () {
    channel=`expr $1 + 1`
    #vertical horizontal
    horizontal=$2
    vertical=$3
    case $channel in
        "1")
        #echo "$tool_path/reg_rw $dev 0x30100 w $horizontal"
        #echo "$tool_path/reg_rw $dev 0x30104 w $vertical"  
        $tool_path/reg_rw $dev 0x30100 w $horizontal
        $tool_path/reg_rw $dev 0x30104 w $vertical
        ;;
        "2")
        $tool_path/reg_rw $dev 0x30108 w $horizontal
        $tool_path/reg_rw $dev 0x3010c w $vertical
        ;;
        "3")
        $tool_path/reg_rw $dev 0x30110 w $horizontal
        $tool_path/reg_rw $dev 0x30114 w $vertical
        ;;
        "4")
        $tool_path/reg_rw $dev 0x30118 w $horizontal
        $tool_path/reg_rw $dev 0x3011c w $vertical
        ;;
        "5")
        $tool_path/reg_rw $dev 0x30120 w $horizontal
        $tool_path/reg_rw $dev 0x30124 w $vertical
        ;;
        "6")
        $tool_path/reg_rw $dev 0x30128 w $horizontal
        $tool_path/reg_rw $dev 0x3012c w $vertical
        ;;
        "7")
        $tool_path/reg_rw $dev 0x30130 w $horizontal
        $tool_path/reg_rw $dev 0x30134 w $vertical
        ;;
        "8")
        $tool_path/reg_rw $dev 0x30138 w $horizontal
        $tool_path/reg_rw $dev 0x3013c w $vertical
        ;;
        *)
        echo "default"
    esac
}

function i2ctransfer () {
    channel=$1
    value=$2
    #echo $channel
    if [ $value -eq 0 ] 
        then
        #echo "i2ctransfer -f -y $channel w3@0x48 0x03 0x13 0x42"
        $tool_path/serdes_cfg $dev 100 "i2ctransfer -f -y $channel w3@0x48 0x03 0x13 0x42" >> $std_out
        else
        #echo "i2ctransfer -f -y $channel w3@0x48 0x03 0x13 0x2"
        $tool_path/serdes_cfg $dev 100 "i2ctransfer -f -y $channel w3@0x48 0x03 0x13 0x2" >> $std_out
    fi
}

function camera_serdes_cfg () {
    $tool_path/serdes_cfg $dev 0 $1  >> $std_out
    echo "Serdes 0 Params Init Processed!"
    $tool_path/serdes_cfg $dev 1 $2 >> $std_out
    echo "Serdes 1 Params Init Processed!"
    $tool_path/serdes_cfg $dev 2 $3  >> $std_out
    echo "Serdes 2 Params Init Processed!"
    $tool_path/serdes_cfg $dev 3 $4  >> $std_out
    echo "Serdes 3 Params Init Processed!"
    $tool_path/serdes_cfg $dev 4 $5 >> $std_out
    echo "Serdes 4 Params Init Processed!"
    $tool_path/serdes_cfg $dev 5 $6 >> $std_out
    echo "Serdes 5 Params Init Processed!"
    $tool_path/serdes_cfg $dev 6 $7 >> $std_out
    echo "Serdes 6 Params Init Processed!"
    $tool_path/serdes_cfg $dev 7 $8 >> $std_out
    echo "Serdes 7 Params Init Processed!"
    sleep 1s

    #echo "Trigger Config!"
    #$tool_path/reg_rw $dev 0x30020 w 0x00002222 >> $std_out 
    #sleep 0.2

    # PCIE reset release
    $tool_path/reg_rw $dev 0x30004 w 0x00 >> $std_out 
    sleep 0.5
    i2ctransfer 1 $1
    i2ctransfer 2 $2
    i2ctransfer 3 $3
    i2ctransfer 4 $4
    i2ctransfer 5 $5
    i2ctransfer 6 $6
    i2ctransfer 7 $7
    i2ctransfer 8 $8
}

#for test
function i2ctransfer_test () {
    channel=$1
    value=$2
    echo $channel
    if [ $value -eq 1 ] 
        then
        echo "i2ctransfer -f -y $channel w3@0x48 0x03 0x13 0x42"
        else
        echo "i2ctransfer -f -y $channel w3@0x48 0x03 0x13 0x02"
    fi
}

function display () {
    i2ctransfer_test 0 $1
    i2ctransfer_test 1 $2
}
#ptp_timesync enp0s31f6
