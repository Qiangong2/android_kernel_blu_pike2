common-objs := gen_scale_coef.o sin_cos.o cam_iommu.o getyuvinput.o cam_util.o

ifeq ($(CONFIG_SPRD_CAM_PW_DOMAIN_R5P0),y)
common-objs += r5p0/cam_pw_domain.o
else ifeq ($(CONFIG_SPRD_CAM_PW_DOMAIN_R4P0),y)
common-objs += r4p0/cam_pw_domain.o
endif

obj-y += common.o

