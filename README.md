# Uart Baremetal for the main domain r5f core of the Texas Instruments AM67A( Beagle Y AI).

## copying file to the firmware folder in the sd card
sudo cp j722s-main-r5f0_0-fw /media/admin1/rootfs/lib/firmware/j722s-main-r5f0_0-fw

## to start the core
sudo echo start > /dev/remoteproc/j7-main-r5f0/state

## to read the trace of the remoteproc4
sudo cat /sys/kernel/debug/remoteproc/remoteproc4/trace0

## to view the resource table of the remoteproc4
cat /sys/kernel/debug/remoteproc/remoteproc0/resource_table