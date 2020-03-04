# Makefile for the Linux udc drivers.
# 10/27 2012, Michael xu, <mailto:michael.xu@spreadtrum.com>
# Rewritten to use lists instead of if-statements.

# Each configuration option enables a list of files.



subdir-ccflags-y += -Idrivers/video/adf/sprd/drv
subdir-ccflags-y += -Idrivers/video/adf/sprd/core/include/
subdir-ccflags-y  += -Idrivers/staging/android/ion/
subdir-ccflags-y += -Idrivers/phy
#subdir-ccflags-y += -Idrivers/udc/

obj-$(CONFIG_UDC)   += udc_base.o
obj-$(CONFIG_UDC_GPIO)		+= udc_gpio/
obj-$(CONFIG_UDC_I2C)		+= udc_i2c/
obj-$(CONFIG_UDC_LCD)		+= udc_lcd/
obj-$(CONFIG_UDC_TP)		+= udc_tp/
