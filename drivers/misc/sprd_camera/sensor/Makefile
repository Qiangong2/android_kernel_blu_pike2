sprd_sensor-objs += sprd_sensor_drv.o sprd_sensor_core.o

ifeq ($(CONFIG_64BIT),y)
sprd_sensor-objs += compat_sensor_drv.o
endif

obj-y += sprd_sensor.o
obj-y += power/ csi2/

ifeq ($(CONFIG_SPRD_CAM_PW_DOMAIN_R5P0),y)
ccflags-y += -I$(srctree)/drivers/misc/sprd_camera/common/r5p0
else ifeq ($(CONFIG_SPRD_CAM_PW_DOMAIN_R4P0),y)
ccflags-y += -I$(srctree)/drivers/misc/sprd_camera/common/r4p0
endif

ifeq ($(CONFIG_SPRD_CAMERA_CSI_R2P0V2),y)
ccflags-y += -I$(src)/csi2/csi_sprd/r2p0v2
else ifeq ($(CONFIG_SPRD_CAMERA_CSI_R2P0),y)
ccflags-y += -I$(src)/csi2/csi_sprd/r2p0
endif
