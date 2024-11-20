# SENSING GMSL Video Capture Card Firmware Programming UserGuide

This directory contains the firmware files for the SENSING GMSL Video Capture Card. The firmware is required to enable the card to interface with the cameras and to output images in various formats.

The firmware files are categorized based on the desired image format:

- **YUYV / UYVY Format**: Used for standard color video output.

  - `pcie_zu_fw-xxxx-YUV.tar.gz`
- **RAW12 Format**: Used for raw image output with 12-bit color depth.

  - `pcie_zu_fw-xxxxx-RAW12.tar.gz`
- **RAW10 Format**: Used for raw image output with 10-bit color depth.

  - `pcie_zu_fw-xxxxx-RAW10.tar.gz`

## Firmware Update Instructions

To update the firmware on your Capture Card, please follow these steps:

1. #### Download the required firmware package

   Place the downloaded firmware `.tar.gz` file into the `/Driver/tools/pcie_zu_tools` folder located in the `driver/` package directory. Example path:<Driver Package Directory>/tools/pcie_zu_tools/

   ![](..\imagesource\Copyfirmware.PNG)
2. #### Load firmware package

    In the terminal, run the following command to execute the firmware update script:

    ```
    // Replace `xxxxx.tar.gz` with the name of the firmware update file. Wait for the firmware to be loaded.
    ./pcie_zu-update.sh xxxxx.tar.gz
    ```
    Firmware update result will as follow image show:  
    ![](..\imagesource\Decryptfirmware.PNG)

3. #### Restart the Host Machine

    After the firmware update script has executed successfully, **power off the machine** to complete the update. It is essential to power off the machine for the firmware to take effect.

4. #### Connect the Capture Card

    After rebooting, connect the network cable to the capture card and ensure that you can ping the capture card's IP address (`192.168.2.11`).

    ```
    ping 192.168.2.11
    ```

5. #### SSH into the Capture Card

    Use SSH to log into the capture card. The default username is `root`, and the password is `root`.

    Once logged in, execute the following command to start the firmware upgrade:

    ```
    /etc/common/deploy_qspi.sh /boot/
    ```
    as follow image show:  
![](..\imagesource\begintoupDate.png)

6. #### Firmware Upgrade Complete

    Once the firmware upgrade is complete, you should see a success message:  
    as follow image show:  
![](..\imagesource\updatSuccess.png)

7. #### Power Off and Restart the Host Machine Again
8. #### Check the Current Firmware Type
    After the update is complete,**power off and restart** the machine again to ensure the firmware is correctly applied.

    To verify that the firmware has been successfully updated, use the following command to check the current firmware version and type:

    ```
    env-get.sh
    ```

    this command will output the firmware type of the Capture Card. The following are common firmware types:

    - `XXX-YUV`: Firmware for cameras using YUYV and UYVY formats.
    - `XXX-RAW`: Firmware for cameras using RAW12 format.
    - `XXX-RAW10`: Firmware for cameras using RAW10 format.

    If the firmware type matches the required one, the firmware update has been successfully completed.

    ![](..\imagesource\yuvformat.png)

    ![](..\imagesource\raw12Format.png)
