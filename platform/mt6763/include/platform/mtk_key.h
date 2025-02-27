#ifndef __MTK_KEY_H__
#define __MTK_KEY_H__

#include <platform/mt_reg_base.h>

#define KP_STA      (KP_BASE + 0x0000)
#define KP_MEM1     (KP_BASE + 0x0004)
#define KP_MEM2     (KP_BASE + 0x0008)
#define KP_MEM3     (KP_BASE + 0x000c)
#define KP_MEM4     (KP_BASE + 0x0010)
#define KP_MEM5     (KP_BASE + 0x0014)
#define KP_DEBOUNCE (KP_BASE + 0x0018)
#define KP_SCAN_TIMING  (KP_BASE + 0x001C)
#define KP_SEL      (KP_BASE + 0x0020)
#define KP_EN       (KP_BASE + 0x0024)

#define KPD_NUM_MEMS    5
#define KPD_MEM5_BITS   8

#define KPD_NUM_KEYS    72  /* 4 * 16 + KPD_MEM5_BITS */
typedef unsigned int        u32;
struct keypad_dts_data {
	u32 kpd_sw_pwrkey;
	u32 kpd_hw_pwrkey;
	u32 kpd_sw_rstkey;
	u32 kpd_hw_rstkey;
	u32 kpd_use_extend_type;
	u32 kpd_hw_map_num;
	u32 kpd_hw_init_map[KPD_NUM_KEYS];
	u32 kpd_hw_recovery_key;
	u32 kpd_hw_factory_key;
};

void set_kpd_pmic_mode(void);
void disable_PMIC_kpd_clock(void);
void enable_PMIC_kpd_clock(void);
bool mtk_detect_key(unsigned short key);
bool mtk_detect_pmic_just_rst(void);

#endif /* __MTK_KEY_H__ */
