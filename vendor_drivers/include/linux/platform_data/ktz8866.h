/*
 * ktz8866.h - ktz8866 LEDs Driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __KTZ8866_H__
#define __KTZ8866_H__

struct device;

struct ktz8866_platform_data {
	struct device *fbdev;
	int hw_en_gpio;
	int enp_gpio;
	int enn_gpio;
	int panelid_gpio;
};

/* KTZ8866 backlight I2C driver */
#define KTZ8866_backlight_EN_MASK         0x40
#define KTZ8866_backlight_EN_SHIFT        6
#define KTZ8866_backlight_DISABLE         0
#define KTZ8866_backlight_ENABLE          1
#define KTZ8866_LCD_BIAS_ENP              130       /* GPIO for Enable pin for positive power (OUTP) */
#define KTZ8866_LCD_BIAS_ENN              131       /* GPIO for Enable pin for negative power (OUTN) */
#define KTZ8866_LCD_DRV_HW_EN             139       /* GPIO for Active high hardware enable pin */
#define KTZ8866_LCD_DRV_I2C_SCL           61       /* Clock of the I2C interface */
#define KTZ8866_LCD_DRV_I2C_SDA           60       /* Bi-directional data pin of the I2C interface */
#define KTZ8866_DISP_ID                   0x01
#define KTZ8866_DISP_BC1                  0x02
#define KTZ8866_DISP_BC2                  0x03
#define KTZ8866_DISP_BB_LSB               0x04
#define KTZ8866_DISP_BB_MSB               0x05
#define KTZ8866_DISP_BL_ENABLE            0x08
#define KTZ8866_DISP_BIAS_CONF1           0x09
#define KTZ8866_DISP_BIAS_CONF2           0x0a
#define KTZ8866_DISP_BIAS_CONF3           0x0b
#define KTZ8866_DISP_BIAS_BOOST           0x0c
#define KTZ8866_DISP_BIAS_VPOS            0x0d
#define KTZ8866_DISP_BIAS_VNEG            0x0e
#define KTZ8866_DISP_FLAGS                0x0f
#define KTZ8866_DISP_OPTION1              0x10
#define KTZ8866_DISP_OPTION2              0x11
#define KTZ8866_DISP_PTD_LSB              0x12
#define KTZ8866_DISP_PTD_MSB              0x13
#define KTZ8866_DISP_DIMMING              0x14
#define KTZ8866_DISP_FULL_CURRENT         0x15
#define BL_LEVEL_MAX 2047
#define BL_LEVEL_MAX_HBM 4095

struct ktz8866_reg {
	unsigned char reg;
	unsigned char value;
};

/**
 * struct ktz8866_led -
 * @lock:          Lock for reading/writing the device
 * @level:         Current backlight level
 * @ktz8866_status: Backlight on/off state
 * @dimming_status: Dimming enable/disable
 * @panel_id:      Panel ID read from GPIO
 * @HBMenable:     High brightness mode flag
 */
struct ktz8866_led {
	struct mutex lock;
	int level;
	bool ktz8866_status;
	bool dimming_status;
	int panel_id;
	bool HBMenable;
};

/* Declarations — definitions live in ktz8866.c to avoid multiple-definition linker errors */
extern struct ktz8866_reg ktz8866_regs_conf[];
extern const int bl_level_remap[];

#endif /* __KTZ8866_H__ */
