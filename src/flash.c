//------------------------------------------------------------------------------
// Copyright (c) 2011 by Silicon Laboratories Inc.  All rights reserved.
// The program contained in this listing is proprietary to Silicon Laboratories,
// headquartered in Austin, Texas, U.S.A. and is subject to worldwide copyright
// protection, including protection under the United States Copyright Act of 1976
// as an unpublished work, pursuant to Section 104 and Section 408 of Title XVII
// of the United States code.  Unauthorized copying, adaptation, distribution,
// use, or display is prohibited by this law.
//
// Silicon Laboratories provides this software solely and exclusively
// for use on Silicon Laboratories' microcontroller products.
//
// This software is provided "as is".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// SILICON LABORATORIES SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
// INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
//------------------------------------------------------------------------------

//==============================================================================
// WARNING:
//
// This file is auto-generated by AppBuilder and should not be modified.
// Any hand modifications will be lost if the project is regenerated.
//==============================================================================
// library
#include <stdio.h>
// hal
#include <si32_device.h>
#include <SI32_CLKCTRL_A_Type.h>
#include <SI32_FLASHCTRL_A_Type.h>
#include <SI32_PBSTD_A_Type.h>
#include <SI32_PBCFG_A_Type.h>
#include <SI32_VMON_A_Type.h>
#include <SI32_RSTSRC_A_Type.h>

// application
#include "config.h"
#include "flash.h"

//==============================================================================
//SUPPORT FUNCTIONS
//==============================================================================

//==============================================================================
//MODE FUNCTIONS
//==============================================================================
void flash_erase_page(unsigned int addr)
{
   SI32_FLASHCTRL_A_exit_multi_byte_write_mode(SI32_FLASHCTRL_0);
   // 2. Write the address of the Flash page to WRADDR
   SI32_FLASHCTRL_A_write_wraddr(SI32_FLASHCTRL_0, addr);
   // 3. Enter Flash Erase Mode
   SI32_FLASHCTRL_A_enter_flash_erase_mode(SI32_FLASHCTRL_0);
   // 5. Write the inital unlock value to KEY
   SI32_FLASHCTRL_A_write_flash_key(SI32_FLASHCTRL_0, 0xA5);
   // 6. Write the single unlock value to KEY
   SI32_FLASHCTRL_A_write_flash_key(SI32_FLASHCTRL_0, 0xF1);
   // 7. Write any value to WRDATA in right-justified format to
   //    initiate the page erase
   SI32_FLASHCTRL_A_write_wrdata(SI32_FLASHCTRL_0, 0x0000);
   // 8. (optional) poll BUSYF if executing code from other than Flash Memory
   // We are executing code from Flash, so no need to poll.
   while(SI32_FLASHCTRL_A_is_buffer_full(SI32_FLASHCTRL_0));
   // 9. Enable Interrupts
}

//------------------------------------------------------------------------------
// myFLASHCTRL0_run_write_flash_mode()
//
//
//------------------------------------------------------------------------------
int flash_write_data(unsigned char * c,unsigned int addr, unsigned int num)
{
   int i;
   unsigned short *buf = (unsigned short*)c;
   // 2. Disable Flash Erase Operations
   SI32_FLASHCTRL_A_exit_flash_erase_mode(SI32_FLASHCTRL_0);
   // 3. Write the address of the half-word to WRADDR
   SI32_FLASHCTRL_A_write_wraddr(SI32_FLASHCTRL_0, addr);
   SI32_FLASHCTRL_A_enter_multi_byte_write_mode(SI32_FLASHCTRL_0);
   // 5. Write the inital unlock value to KEY
   SI32_FLASHCTRL_A_write_flash_key(SI32_FLASHCTRL_0, 0xA5);
   // 6. Write the multiple unlock value to KEY
   SI32_FLASHCTRL_A_write_flash_key(SI32_FLASHCTRL_0, 0xF2);
   // Write all the half-words in the array
   for(i = 0; i < num / 2; i++)
   {
      // 7. Write the data to WRDATA in right-justified format to
      //    initiate the write
      SI32_FLASHCTRL_A_write_wrdata(SI32_FLASHCTRL_0, buf[i]);
      while(SI32_FLASHCTRL_A_is_flash_busy(SI32_FLASHCTRL_0));
   }
   // 8. (optional) poll BUSYF if executing code from other than Flash Memory
   // We are executing code from Flash, so no need to poll.
   while(SI32_FLASHCTRL_A_is_buffer_full(SI32_FLASHCTRL_0));
   SI32_FLASHCTRL_A_write_flash_key(SI32_FLASHCTRL_0, 0x5A);
   return 0;
}


//------------------------------------------------------------------------------
void flash_initialize(void)
{
   // ENABLE FLASH CLOCK
   SI32_CLKCTRL_A_enable_apb_to_modules_0(SI32_CLKCTRL_0,
                                          SI32_CLKCTRL_A_APBCLKG0_FLCTRLCEN_ENABLED_U32);
   // 1. Enable VDD Supply Monitor and set as a reset source
   SI32_VMON_A_enable_vdd_supply_monitor(SI32_VMON_0);
   SI32_RSTSRC_A_enable_vdd_monitor_reset_source(SI32_RSTSRC_0);
}

//-eof--------------------------------------------------------------------------
