#!/bin/bash

# Matrix Vision USB Device Permission Script
# Only sets permissions on detected Matrix Vision devices

set -e

# Configuration
VENDOR_ID="164c"
PRODUCT_IDS=("0103" "0101")
TARGET_MODE="0664"
TARGET_GROUP="plugdev"

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

log_info() { echo -e "${GREEN}[INFO]${NC} $1"; }
log_warn() { echo -e "${YELLOW}[WARN]${NC} $1"; }
log_error() { echo -e "${RED}[ERROR]${NC} $1"; }

# Function to set permissions on a device
set_permissions() {
    local device=$1
    
    if [[ ! -e "$device" ]]; then
        log_warn "Device $device not found"
        return 1
    fi
    
    log_info "Setting permissions on $device"
    chmod "$TARGET_MODE" "$device"
    
    if getent group "$TARGET_GROUP" >/dev/null 2>&1; then
        chgrp "$TARGET_GROUP" "$device"
    fi
    
    ls -l "$device"
}

# Main script
log_info "Matrix Vision Device Permission Script"

devices_found=0

# Find Matrix Vision devices by checking sysfs
for sysfs_device in /sys/bus/usb/devices/*; do
    if [[ -d "$sysfs_device" && -f "$sysfs_device/idVendor" && -f "$sysfs_device/idProduct" ]]; then
        vendor=$(cat "$sysfs_device/idVendor" 2>/dev/null)
        product=$(cat "$sysfs_device/idProduct" 2>/dev/null)
        
        if [[ "$vendor" == "$VENDOR_ID" ]]; then
            for target_product in "${PRODUCT_IDS[@]}"; do
                if [[ "$product" == "$target_product" ]]; then
                    # Get the actual bus and device numbers from sysfs
                    if [[ -f "$sysfs_device/busnum" && -f "$sysfs_device/devnum" ]]; then
                        bus_num=$(cat "$sysfs_device/busnum" 2>/dev/null)
                        dev_num=$(cat "$sysfs_device/devnum" 2>/dev/null)
                        
                        if [[ -n "$bus_num" && -n "$dev_num" ]]; then
                            usb_device="/dev/bus/usb/$(printf '%03d/%03d' $bus_num $dev_num)"
                            log_info "Found Matrix Vision device: ${vendor}:${product} at $usb_device"
                            set_permissions "$usb_device"
                            devices_found=$((devices_found + 1))
                        fi
                    fi
                fi
            done
        fi
    fi
done

if [[ $devices_found -eq 0 ]]; then
    log_warn "No Matrix Vision devices found"
    log_info "Available USB devices:"
    lsusb | grep -v "Linux Foundation"
else
    log_info "Successfully processed $devices_found Matrix Vision device(s)"
fi