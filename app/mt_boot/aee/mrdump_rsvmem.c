/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2016. All rights reserved.
*
* BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
* THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
* AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
* NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
* SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
* SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
* THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
* THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
* CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
* SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
* STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
* AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
* OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
* MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*/

#include <memory_layout.h>
#include <block_generic_interface.h>
#include <malloc.h>
#include <printf.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <video.h>
#include <bootimg.h>
#include <libfdt.h>
#include <load_vfy_boot.h>

#include <arch/ops.h>

#ifdef MTK_PARTITION_COMMON
#include <env.h>
#else
#include <platform/env.h>
#endif
#include <platform/mtk_key.h>
#include <platform/mtk_wdt.h>
#include <platform/mt_gpt.h>
#include <platform/boot_mode.h>
#include <platform/ram_console.h>
#include <platform.h>

#include <target/cust_key.h>
#include <dev/mrdump.h>
#include "aee.h"
#include "kdump.h"

#define MRDUMP_CB_BUF_LEN 64

void mrdump_reserve_memory(void)
{
#ifndef DUMMY_MEMORY_LAYOUT
#if defined(MBLOCK_LIB_SUPPORT)
#if	defined(MBLOCK_LIB_SUPPORT) \
	&& (MBLOCK_EXPAND(MBLOCK_LIB_SUPPORT)\
	== MBLOCK_EXPAND(2))
#ifndef MTK_MRDUMP_SUPPORT
	int result;

	if (mblock_create(&g_boot_arg->mblock_info,
		&g_boot_arg->orig_dram_info,
		PL_BOOTARG_BASE, PL_BOOTARG_MAX_SIZE)) {
		dprintf(CRITICAL, "%s: free PL_BOOT_ARG failed\n", __func__);
		assert(0);
	}

#endif
#endif
#endif
#endif
}

void mrdump_setup_cblock(void)
{
#ifdef MTK_MRDUMP_SRAM_CB
        char *mrdump_cb_buf = malloc(MRDUMP_CB_BUF_LEN);
        if (mrdump_cb_buf) {
                snprintf(mrdump_cb_buf, MRDUMP_CB_BUF_LEN, "mrdump_cb=0x%x,0x%x", MRDUMP_CB_ADDR, MRDUMP_CB_SIZE);
                cmdline_append(mrdump_cb_buf);
        }
        free(mrdump_cb_buf);
#endif
}
