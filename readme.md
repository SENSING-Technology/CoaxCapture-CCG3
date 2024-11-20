# SENSING GMSL Video Capture Card UserGuide

This repository contains the necessary resources for configuring and operating the Sensing Capture cards (models CCG3-8H and CCG3-4H) to work with connected cameras. These cards are designed to interface with coaxial cables and require firmware updates to function properly. The repository includes firmware files, driver source code, and usage instructions.

![](.\imagesource\sensingPCIECard.jpeg)

## Models

- **CCG3-8H**: This model supports up to 8 camera connections via coaxial cables.
- **CCG3-4H**: This model supports up to 4 camera connections via coaxial cables.

The two models share the same firmware and driver resources, with differences in the number of interfaces.

## Directory Structure

- **FirmwareResources/**: Contains firmware files for both models. The firmware updates the card for different image formats (YUYV, UYVY, RAW12, RAW10).
- **Driver/**: Contains the driver source code for the Capture cards. The current version is compatible with Ubuntu 18.04 and Ubuntu 20.04.

## How to Use
**Firmware programming**:  
The Capture card has been programmed with firmware to support YUV or RAW10, RAW12 modes when it leaves the factory.  
If you need to switch to other modes, follow the instructions in the respective `README.md` files under the `FirmwareResources/` to reprogram the firmware of the Capture card.  
- For YUYV/UYVY image formats, use the firmware `pcie_zu_fw-xxxx-YUV.tar.gz`.
- For RAW12 image format, use the firmware `pcie_zu_fw-xxxx-RAW12.tar.gz`.
- For RAW10 image format, use the firmware `pcie_zu_fw-xxxx-RAW10.tar.gz`.

**Driver Installation**:  
Follow the instructions in the respective `README.md` files under the `Driver/` directory to build and install the driver on your system.

## Supported Platforms

- Ubuntu 18.04
- Ubuntu 20.04
