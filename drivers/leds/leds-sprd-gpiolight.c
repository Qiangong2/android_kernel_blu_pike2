/*
 * Spreadtrum OSC support
 *
 * Copyright (C) 2018 spreadtrum, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/vmalloc.h>
#include <linux/sprd_otp.h>
#include <linux/mfd/sprd/pmic_glb_reg.h>
#include <linux/slab.h>

struct torch_device {
	struct regmap *reg_map;
	struct device *dev;
	spinlock_t slock;
	int curris_open;
	int gpio;
};

static struct class *torch_class;
static struct device *torch_dev;
struct torch_device *drv_d;

#define GPIO_OPEN_TORCH 	0
#define GPIO_CLOSE_TORCH 	1


static int sprd_leds_gpio_val_torch(struct torch_device * tor, int val)
{
	int ret = -EINVAL;

	ret = gpio_direction_output(tor->gpio, val);
	if (ret) {
		pr_err("torch gpio output err\n");
		return ret;
	}
	return 0;
}

static ssize_t leds_gpio_show(struct device *dev,
			struct device_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%u\n", drv_d->curris_open);
}

static ssize_t leds_gpio_store(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t count)
{

	int ret = -EPERM;
	int val = -2;

	if (!dev)
		goto exit;

	ret = kstrtouint(buf, 10, &val);
	if (ret)
		goto exit;

	switch (val) {
	case 1:
		sprd_leds_gpio_val_torch(drv_d,
			GPIO_CLOSE_TORCH);
		break;
	case 0:
		sprd_leds_gpio_val_torch(drv_d,
			GPIO_OPEN_TORCH);
		break;
	default:
		val = 0;
		break;
	}

	drv_d->curris_open = val;
	return 10;
exit:
	return ret;
}

static struct device_attribute dev_attr_enable = {
	.attr = {
		.name = "enable",
		.mode = 0666,
	},
	.show = leds_gpio_show,
	.store = leds_gpio_store,
};

#if 1
static int init_torch(struct torch_device *dat)
{

	int ret = 0;
	struct device_node *np = dat->dev->of_node;

	dat->gpio = of_get_named_gpio(np, "torchio", 0);
	if (gpio_is_valid(dat->gpio)) {

		ret = devm_gpio_request(dat->dev, dat->gpio, "torch");
		if (ret) {
			pr_err("torch gpio get err\n");
			return ret;
		}
	} else
      ret = -EINVAL;

    return ret;
}
#else
static int init_torch(struct torch_device *dat)
{
    int ret = 0;

    ret = gpio_request(49, "torch");
    if (ret < 0) {
        pr_err("torch gpio request failed\n");
        return ret;
    }

    dat->gpio = ret;
    return ret;
}
#endif

static int leds_torch_probe(struct platform_device *pdev)
{
	int err = 0;

	if (pdev == NULL)
		return -EINVAL;

	torch_class = class_create(THIS_MODULE,
			"torch_dev");
	if (torch_class == NULL)
		goto out1;

	torch_dev = device_create(torch_class, NULL, 0, (void *)drv_d, "torch");
	if (torch_dev == NULL)
		goto out2;

	err = device_create_file(torch_dev, &dev_attr_enable);
	if (err < 0)
		goto out3;

	drv_d = devm_kzalloc(&pdev->dev,
			sizeof(struct torch_device),
			GFP_KERNEL);
	if (!drv_d)
		goto out3;
	spin_lock_init(&drv_d->slock);

	spin_lock(&drv_d->slock);
	drv_d->dev = &pdev->dev;
	platform_set_drvdata(pdev, (void *)drv_d);
	spin_unlock(&drv_d->slock);

//	drv_d->reg_map = dev_get_regmap(pdev->dev.parent, NULL);
//	if (drv_d->reg_map == NULL)
//		goto out4;

	err = init_torch(drv_d);
	if (err < 0) {
		pr_err("torch init failed\n");
		goto out4;
	}

	return 0;
out4:
	devm_kfree(&pdev->dev, drv_d);
out3:
	device_remove_file(torch_dev,
		&dev_attr_enable);
out2:
	class_destroy(torch_class);
out1:
	return err;
}

static int leds_torch_suspend(struct platform_device *dev, pm_message_t state)
{
	pr_info("entry leds_suspend\n");
	return 0;
}

static int leds_torch_resume(struct platform_device *dev)
{
	pr_info("entry leds_resume\n");
	return 0;
}


static int leds_torch_remove(struct platform_device *pdev)
{
	devm_kfree(&pdev->dev,pdev->dev.driver_data);
	device_remove_file(torch_dev,
			&dev_attr_enable);
	class_destroy(torch_class);
	return 0;
}


static const struct of_device_id leds_torch_of_match[] = {
	{ .compatible = "sprd,gpio-torch", },
	{},
};

static struct platform_driver led_light_gpio_drv = {
	.probe = leds_torch_probe,
	.remove = leds_torch_remove,
	.suspend = leds_torch_suspend,
	.resume  = leds_torch_resume,
	.driver = {
		.name = "gpio-torch",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(leds_torch_of_match),
	},
};

module_platform_driver(led_light_gpio_drv);

MODULE_AUTHOR("spreadtrum.com");
MODULE_DESCRIPTION("GPIO LED driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:leds-torch");

