#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
#elif defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#else
//#include <mach/mt_gpio.h>
//#include <linux/xlog.h>
//#include <mach/mt_pm_ldo.h>
#endif


// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1440)

#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0xFFF   // END OF REGISTERS MARKER

#define LCM_ID_ILI9881C	0x98811C

#define LCM_DSI_CMD_MODE									0

#ifndef TRUE
    #define   TRUE     1
#endif
 
#ifndef FALSE
    #define   FALSE    0
#endif

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

 struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] = {
	

	//PWU6_BPZN_P1 haifei ili9881p panda 1440*720
	{0xFF,3,{0x98,0x81,0x05}},
	{0xB2,1,{0x70}},    //panda timing clr delay 1 frame
	{0x03,1,{0x00}},    //VCM1[8]
	{0x04,1,{0x24}},    //VCM1[7:0]
	{0x30,1,{0xF7}},     //-3/3X 
	{0x29,1,{0x00}},
	{0x2A,1,{0x12}},     //sleep VCM1-old
	{0x38,1,{0xA8}},    //IOVCC LVD 1.45
	{0x1A,1,{0x50}},    //debounce 32us
	{0x52,1,{0x5F}},    //VGHO=VGH
	{0x54,1,{0x28}},    //VGH CLP=12.03 
	{0x55,1,{0x25}},    //VGL CLP=-12.03

	{0x26,1,{0x02}},     //Auto 1/2 VCOM-new
	{0x3D,1,{0xA1}},     //default:E1 enable VCI/VSP LVD
	{0x1B,1,{0x01}},     //11, keep LVD function

	{0xFF,3,{0x98,0x81,0x02}},
	{0x42,1,{0x2F}},    //SDT=3us 
	{0x01,1,{0x50}},    //timeout black
	{0x15,1,{0x10}},    //2/3-power mode
	//{0x33,1,{0x01}},    //BIST EN     

	{0x57,1,{0x00}},
	{0x58,1,{0x1A}},
	{0x59,1,{0x29}},
	{0x5A,1,{0x15}},
	{0x5B,1,{0x18}},
	{0x5C,1,{0x2B}},
	{0x5D,1,{0x1F}},
	{0x5E,1,{0x20}},
	{0x5F,1,{0x98}},
	{0x60,1,{0x1E}},
	{0x61,1,{0x2A}},
	{0x62,1,{0x85}},
	{0x63,1,{0x1A}},
	{0x64,1,{0x19}},
	{0x65,1,{0x4E}},
	{0x66,1,{0x23}},
	{0x67,1,{0x29}},
	{0x68,1,{0x56}},
	{0x69,1,{0x60}},
	{0x6A,1,{0x25}},

	{0x6B,1,{0x00}},
	{0x6C,1,{0x1A}},
	{0x6D,1,{0x29}},
	{0x6E,1,{0x15}},
	{0x6F,1,{0x18}},
	{0x70,1,{0x2B}},
	{0x71,1,{0x1F}},
	{0x72,1,{0x20}},
	{0x73,1,{0x98}},
	{0x74,1,{0x1E}},
	{0x75,1,{0x2A}},
	{0x76,1,{0x85}},
	{0x77,1,{0x1A}},
	{0x78,1,{0x19}},
	{0x79,1,{0x4E}},
	{0x7A,1,{0x23}},
	{0x7B,1,{0x29}},
	{0x7C,1,{0x56}},
	{0x7D,1,{0x60}},
	{0x7E,1,{0x25}},


	{0xFF,3,{0x98,0x81,0x01}},    //GIP 
	{0x01,1,{0x00}},  //
	{0x02,1,{0x00}},
	{0x03,1,{0x56}},  //STV_A rise[9:8]=01, phase_stv_a=2, overlap_stv_a=6H
	{0x04,1,{0x13}},  //phase_stv_b=2, overlap_stv_b=4H
	{0x05,1,{0x13}},  //phase_stv_c=2, overlap_stv_c=4H
	{0x06,1,{0x0a}},  //STV_A rise[7:0]
	{0x07,1,{0x05}},  //STV_B rise[7:0]
	{0x08,1,{0x05}},  //STV_C rise[7:0]
	{0x09,1,{0x1D}},  //fti_1_rise:STV
	{0x0a,1,{0x01}},  //fti_2_rise
	{0x0b,1,{0x00}},  //fti_3_rise
	{0x0c,1,{0x3F}},  //fti_1_fall
	{0x0d,1,{0x29}},  //fti_2_fall
	{0x0e,1,{0x29}},  //fti_3_fall
	{0x0f,1,{0x1D}},  //clw_1_rise  29
	{0x10,1,{0x1D}},  //clw_2_rise  29
	{0x11,1,{0x00}},  //clwb_1_rise
	{0x12,1,{0x00}},  //clwb_2_rise
	{0x13,1,{0x08}},  //clw_x_fall
	{0x14,1,{0x08}},  //clwb_x_fall
	{0x15,1,{0x00}},
	{0x16,1,{0x00}},
	{0x17,1,{0x00}},
	{0x18,1,{0x00}},
	{0x19,1,{0x00}},
	{0x1a,1,{0x00}},
	{0x1b,1,{0x00}},
	{0x1c,1,{0x00}},
	{0x1d,1,{0x00}},
	{0x1e,1,{0x40}},  //clk_a_rise[10:8]
	{0x1f,1,{0x88}},  //panda lvd_option, clk_b_rise[10:8], panda_back_vgh_option
	{0x20,1,{0x08}},  //clk_a_rise[7:0]
	{0x21,1,{0x01}},  //clk_a_fall[7:0]
	{0x22,1,{0x00}},  //clk_b_rise[7:0]
	{0x23,1,{0x00}},  //clk_b_fall[7:0]
	{0x24,1,{0x00}},  //clk_keep_pos1[7:0]
	{0x25,1,{0x00}},  //clk_keep_pos2[7:0]
	{0x26,1,{0x00}},  //clkb_keep_pos1[7:0]
	{0x27,1,{0x00}},  //clkb_keep_pos2[7:0]
	{0x28,1,{0x33}},  //clk_x_num[2:0]=8, phase_clk[2:0]=8
	{0x29,1,{0x03}},  //overlap_clk[3:0]=4H
	{0x2a,1,{0x00}},
	{0x2b,1,{0x00}},
	{0x2c,1,{0x00}},
	{0x2d,1,{0x00}},
	{0x2e,1,{0x00}},
	{0x2f,1,{0x00}},
	{0x30,1,{0x00}},
	{0x31,1,{0x00}},
	{0x32,1,{0x00}},
	{0x33,1,{0x00}},
	{0x34,1,{0x00}},
	{0x35,1,{0x00}},
	{0x36,1,{0x00}},
	{0x37,1,{0x00}},
	{0x38,1,{0x00}},
	{0x39,1,{0x0f}},  //post_timing_en, gip_sr_vgl[0], gip_sr_vgh_[1:0]
	{0x3a,1,{0x2a}},
	{0x3b,1,{0xc0}},
	{0x3c,1,{0x00}},
	{0x3d,1,{0x00}},
	{0x3e,1,{0x00}},
	{0x3f,1,{0x00}},
	{0x40,1,{0x00}},
	{0x41,1,{0xe0}},
	{0x42,1,{0x40}},
	{0x43,1,{0x0f}},
	{0x44,1,{0x31}},  //panda timing, default=15
	{0x45,1,{0xa8}},
	{0x46,1,{0x00}},
	{0x47,1,{0x08}},
	{0x48,1,{0x00}},
	{0x49,1,{0x00}},
	{0x4a,1,{0x00}},
	{0x4b,1,{0x00}},
		                                   
	// ==== GOUT_BW_L[3:0] ====
	{0x4c,1,{0xb2}},  //STVB2
	{0x4d,1,{0x22}},
	{0x4e,1,{0x01}},  //VGL/VGH
	{0x4f,1,{0xf7}},  //STVC2/STVA2
	{0x50,1,{0x29}},  //L9=19=CK8=CLK6
	{0x51,1,{0x72}},  //L12=17=CK6=CLK4
	{0x52,1,{0x25}},  //L13=15=CK4=CLK2
	{0x53,1,{0xb2}},  //L16=1B=CK2=CLK8
	{0x54,1,{0x22}},
	{0x55,1,{0x22}},
	{0x56,1,{0x22}},
		                                   
	// ==== GOUT_BW_R[3:0] ====
	{0x57,1,{0xa2}},  //STVB1
	{0x58,1,{0x22}},
	{0x59,1,{0x01}},
	{0x5a,1,{0xe6}},  //STVC1/STVA1
	{0x5b,1,{0x28}},  //R9=CK7=CLK5
	{0x5c,1,{0x62}},  //R12=CK5=CLK3
	{0x5d,1,{0x24}},  //R13=CK3=CLK1
	{0x5e,1,{0xa2}},  //R16=CK1=CLK7
	{0x5f,1,{0x22}},
	{0x60,1,{0x22}},
	{0x61,1,{0x22}},

	{0x62,1,{0xee}},
		                                   
	// ==== GOUT_FW_L ====
	{0x63,1,{0x02}},
	{0x64,1,{0x0b}},
	{0x65,1,{0x02}},
	{0x66,1,{0x02}},
	{0x67,1,{0x01}},
	{0x68,1,{0x00}},
	{0x69,1,{0x0f}},
	{0x6a,1,{0x07}},
	{0x6b,1,{0x55}},  //L9=CK8=CLK2
	{0x6c,1,{0x02}},
	{0x6d,1,{0x02}},
	{0x6e,1,{0x5b}},  //L12=CK6=CLK8
	{0x6f,1,{0x59}},  //L13=CK4=CLK6
	{0x70,1,{0x02}},
	{0x71,1,{0x02}},
	{0x72,1,{0x57}},  //L16=CK2=CLK4
	{0x73,1,{0x02}},
	{0x74,1,{0x02}},
	{0x75,1,{0x02}},
	{0x76,1,{0x02}},
	{0x77,1,{0x02}},
	{0x78,1,{0x02}},
		                                   
	// ==== GOUT_FW_R ====
	{0x79,1,{0x02}},
	{0x7a,1,{0x0a}},
	{0x7b,1,{0x02}},
	{0x7c,1,{0x02}},
	{0x7d,1,{0x01}},
	{0x7e,1,{0x00}},
	{0x7f,1,{0x0e}},
	{0x80,1,{0x06}},
	{0x81,1,{0x54}},  //R9=CK7=CLK1
	{0x82,1,{0x02}},
	{0x83,1,{0x02}},
	{0x84,1,{0x5a}},  //R12=CK5=CLK7
	{0x85,1,{0x58}},  //R13=CK3=CLK5
	{0x86,1,{0x02}},
	{0x87,1,{0x02}},
	{0x88,1,{0x56}},  //R16=CK1=CLK3
	{0x89,1,{0x02}},
	{0x8a,1,{0x02}},
	{0x8b,1,{0x02}},
	{0x8c,1,{0x02}},
	{0x8d,1,{0x02}},
	{0x8e,1,{0x02}},
		                                   
	{0x8f,1,{0x44}},
	{0x90,1,{0x44}},

	{0xFF,3,{0x98,0x81,0x06}},
	{0x01,1,{0x03}},     //LEDPWM/SDO hi-z  
	{0x04,1,{0x71}},     //70¡êo4lane, 71¡êo3lane,
	{0x2B,1,{0x0A}},     //BGR_PANEL+SS_PANEL BW:09   
	   
	{0xC0,1,{0xCF}},       //720*1440
	{0xC1,1,{0x2A}},       //720*1440

	{0xFF,3,{0x98,0x81,0x00}},
	{0x35,1,{0x00}},		
	{0x55,1,{0xB0}},     
	{0x11,1,{0x00}},       
	{REGFLAG_DELAY, 150, {}},
	// Display ON            
	{0x29, 1, {0x00}},       
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

/*

static struct LCM_setting_table lcm_set_window[] = {
	{0x2A,	4,	{0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
	{0x2B,	4,	{0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};



static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
	{0x29, 1, {0x00}},
	{REGFLAG_DELAY, 50, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
*/

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 1, {0x00}},
	{REGFLAG_DELAY, 50, {}},

    // Sleep Mode On
	{0x10, 1, {0x00}},
	{REGFLAG_DELAY, 120, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

/*

static struct LCM_setting_table lcm_backlight_level_setting[] = {
	{0x51, 1, {0xFF}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
*/

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

    for(i = 0; i < count; i++) {
		
        unsigned cmd;
        cmd = table[i].cmd;
		
        switch (cmd) {
			
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
				
            case REGFLAG_END_OF_TABLE :
                break;
				
            default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
       	}
    }
	
}


// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;
	//	params->density = 320;//LCM_DENSITY;

#if LCM_DSI_CMD_MODE
		params->dsi.mode   = CMD_MODE;
#else
		params->dsi.mode   = BURST_VDO_MODE;
#endif
		params->dsi.vertical_sync_active = 10;
		params->dsi.vertical_backporch = 20;
		params->dsi.vertical_frontporch = 20;
		params->dsi.vertical_active_line = FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 40;	
		params->dsi.horizontal_backporch				= 50;
		params->dsi.horizontal_frontporch				= 30;  //80
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

		params->dsi.LANE_NUM = LCM_THREE_LANE;
		
		params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
		params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
		params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;
		
		
		params->dsi.packet_size=256;
		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		// Video mode setting		
		params->dsi.intermediat_buffer_num = 2;


		// params->dsi.HS_TRAIL=20; 
		params->dsi.PLL_CLOCK = 340;  //240=4db  230=1.5db
/***********************    esd  check   ***************************/
#ifndef BUILD_LK
		params->dsi.esd_check_enable = 1;
		params->dsi.customization_esd_check_enable = 1;
		params->dsi.lcm_esd_check_table[0].cmd          = 0x0a;
		params->dsi.lcm_esd_check_table[0].count        = 1;
		params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9C;
#endif

}

#define AUXADC_COMPARE_ID          0
#define AUXADC_LCM_VOLTAGE_CHANNEL (2)
#define MIN_VOLTAGE (0)
#define MAX_VOLTAGE (600)

//extern int IMM_GetOneChannelValue(int dwChannel, int data[4], int* rawdata);

static unsigned int lcm_compare_id(void)
{
    int res = 0;
	int i=0;
    int lcm_vol = 0;
    int rawdata = 0;
    int data[4] = {0,0,0,0};
	int avg = 3;

	res = IMM_GetOneChannelValue(AUXADC_LCM_VOLTAGE_CHANNEL, data, &rawdata);


	printf("zgj:[cpu-adc_uboot]: lcm_vol = %d==XXXXX",lcm_vol);

	
    if(res < 0)
    {
#ifdef BUILD_LK
        printf("cjx:[adc_uboot]: res = %d, get data error\n",res);
#else
        printk("cjx:[adc_kernel]: get data error\n");
#endif
        return 0;
    }

	 lcm_vol = data[0]*1000+data[1]*10;
#ifdef BUILD_LK
    printf("zgj:[adc_uboot]: lcm_vol= %d , file : %s, line : %d\n", lcm_vol, __FILE__, __LINE__);
#else
    printk("zgj:[adc_kernel]: lcm_vol= %d , file : %s, line : %d\n", lcm_vol, __FILE__, __LINE__);
#endif

    if (lcm_vol >= MIN_VOLTAGE && lcm_vol <= MAX_VOLTAGE)
    {
		printf("zgj:lcm->ili9881d_hd1440_dsi_vdo_hsd_hz\n");
        return 1;
    }
    return 0;
}

static void lcm_init(void)
{
	//display_ldo28_enable(1);
	//display_ldo18_enable(1);
	//display_bias_enable();
	
    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(120);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{

	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	SET_RESET_PIN(1);
	MDELAY(1);
	SET_RESET_PIN(0);
	MDELAY(1);
	SET_RESET_PIN(1);
	MDELAY(120);
	
	//display_bias_disable();
	//display_ldo18_enable(0);
	//`nnnndisplay_ldo28_enable(0);
}


static void lcm_resume(void)
{
//	lcm_initialization_setting[141].para_list[0] = lcm_initialization_setting[141].para_list[0] + 1; 
	lcm_init();
	
//	push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
}

/*
static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0>>8)&0xFF);
	unsigned char x0_LSB = (x0&0xFF);
	unsigned char x1_MSB = ((x1>>8)&0xFF);
	unsigned char x1_LSB = (x1&0xFF);
	unsigned char y0_MSB = ((y0>>8)&0xFF);
	unsigned char y0_LSB = (y0&0xFF);
	unsigned char y1_MSB = ((y1>>8)&0xFF);
	unsigned char y1_LSB = (y1&0xFF);

	unsigned int data_array[16];

	data_array[0]= 0x00053902;
	data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
	data_array[2]= (x1_LSB);
	data_array[3]= 0x00053902;
	data_array[4]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[5]= (y1_LSB);
	data_array[6]= 0x002c3909;

	dsi_set_cmdq(data_array, 7, 0);

}


static void lcm_setbacklight(unsigned int level)
{
	unsigned int default_level = 145;
	unsigned int mapped_level = 0;

	//for LGE backlight IC mapping table
	if(level > 255) 
			level = 255;

	if(level >0) 
			mapped_level = default_level+(level)*(255-default_level)/(255);
	else
			mapped_level=0;

	// Refresh value of backlight level.
	lcm_backlight_level_setting[0].para_list[0] = mapped_level;

	push_table(lcm_backlight_level_setting, sizeof(lcm_backlight_level_setting) / sizeof(struct LCM_setting_table), 1);
}

*/

//prize-chenxiaowen-20161013-compatibility-(ili9881c_hd720_dsi_vdo_yin vs ili9881c_hd720_dsi_vdo_cmi)-start
#if defined(ILI9881C_HD720_DSI_VDO_YIN) && defined(ILI9881C_HD720_DSI_VDO_CMI)

#define MIN_VOLTAGE (600)
#define MAX_VOLTAGE (1200)
#endif
//prize-chenxiaowen-20161013-compatibility-(ili9881c_hd720_dsi_vdo_yin vs ili9881c_hd720_dsi_vdo_cmi)-end

#ifndef BUILD_LK
extern atomic_t ESDCheck_byCPU;
#endif



//prize-chenxiaowen-20161013-compatibility-(ili9881c_hd720_dsi_vdo_yin vs ili9881c_hd720_dsi_vdo_cmi)-start
#if defined(ILI9881C_HD720_DSI_VDO_YIN) && defined(ILI9881C_HD720_DSI_VDO_CMI)
static unsigned int ili_lcm_compare_id(void)
{

 int data[4] = {0,0,0,0};
 int res = 0;
 int rawdata = 0;
 int lcm_vol = 0;

 #ifdef AUXADC_LCM_VOLTAGE_CHANNEL
 res = IMM_GetOneChannelValue(AUXADC_LCM_VOLTAGE_CHANNEL,data,&rawdata);
 if(res < 0)
 {
	 #ifdef BUILD_LK
	 dprintf(0,"lixf lcd [adc_uboot]: get data error\n");
	 #endif
	 return 0;

 }
 #endif

 lcm_vol = data[0]*1000+data[1]*10;

 #ifdef BUILD_LK
 dprintf(0,"lk: lcm_vol= %d , file : %s, line : %d\n",lcm_vol, __FILE__, __LINE__);
 #else
 printk("kernel: lcm_vol= %d , file : %s, line : %d\n",lcm_vol, __FILE__, __LINE__);
 #endif

 if (lcm_vol>=MIN_VOLTAGE &&lcm_vol <= MAX_VOLTAGE &&lcm_compare_id())
 {
 		return 1;
 }

 return 0;
}
#endif
//prize-chenxiaowen-20161013-compatibility-(ili9881c_hd720_dsi_vdo_yin vs ili9881c_hd720_dsi_vdo_cmi)-end


LCM_DRIVER ili9881c_hdplus_dsi_vdo_cmi_ykl_lcm_drv = {
	.name		= "ili9881c_hdplus_dsi_vdo_cmi_ykl",
    	//prize-lixuefeng-20150512-start
	#if defined(CONFIG_PRIZE_HARDWARE_INFO) && !defined (BUILD_LK)
	.lcm_info = {
		.chip	= "ili9881p",
		.vendor	= "yikuailai",
		.id		= "0x9881",
		.more	= "lcm_1440*720",
	},
	#endif
	//prize-lixuefeng-20150512-end	
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	//prize-chenxiaowen-20161013-compatibility-(ili9881c_hd720_dsi_vdo_yin vs ili9881c_hd720_dsi_vdo_cmi)-start
  #if defined(ILI9881C_HD720_DSI_VDO_YIN) && defined(ILI9881C_HD720_DSI_VDO_CMI)
  .compare_id = ili_lcm_compare_id,
  #else
  .compare_id = lcm_compare_id,
  #endif
	//prize-chenxiaowen-20161013-compatibility-(ili9881c_hd720_dsi_vdo_yin vs ili9881c_hd720_dsi_vdo_cmi)-end
#if (LCM_DSI_CMD_MODE)
	.set_backlight	= lcm_setbacklight,
    .update         = lcm_update,
#endif
};

