#ifndef __REG_BASE_H__ 
#define __REG_BASE_H__ 

/***********************************************************************************************************************************
* This file is generated from BK7256_ADDR Mapping_20211224_format_change_highlight_20220113_update.xlsm automatically                                
* Modify it manually is not recommended                                       
* CHIP ID:BK7256,GENARATE TIME:2022-01-17 15:34:58                                                 
************************************************************************************************************************************/

#define SOC_SYSTEM_REG_BASE  (0x54010000)

#define SOC_AON_PMU_REG_BASE  (0x54000000)   
#define SOC_CPU_PLIC_REG_BASE  (0xE4000000)   
#define SOC_AON_WDT_REG_BASE  (0x54000600)   

#define SOC_AON_GPIO_REG_BASE  (0x54000400)   
#define SOC_AON_RTC_REG_BASE  (0x54000200)   

#define SOC_FLASH_REG_BASE  (0x54030000)   
#define SOC_MBOX0_REG_BASE  (0xA0000000)   //TODO
#define SOC_MBOX1_REG_BASE  (0xA0008000)   //TODO
#define SOC_GENER_DMA_REG_BASE  (0x54020000)   
#define SOC_FFT_REG_BASE  (0x57000000)   
#define SOC_SBC_REG_BASE  (0x57010000)   

#define SOC_UART0_REG_BASE  (0x54820000)
#define SOC_UART1_REG_BASE  (0x55830000)
#define SOC_UART2_REG_BASE  (0x55840000)

#define SOC_SPI_REG_BASE  (0x54860000)   
#define SOC_WDT_REG_BASE  (0x54800000)   
#define SOC_SPI1_REG_BASE  (0x558C0000)   
#define SOC_I2C0_REG_BASE  (0x54850000)   
#define SOC_I2C1_REG_BASE  (0x55890000)   
#define SOC_TIMER0_REG_BASE  (0x54810000)   
#define SOC_TIMER1_REG_BASE  (0x55800000)   
#define SOC_PWM01_REG_BASE  (0x54840000)   
#define SOC_PWM23_REG_BASE  (0x54840040)   
#define SOC_PWM45_REG_BASE  (0x54840080)   
#define SOC_SADC_REG_BASE  (0x54870000)   
#define SOC_EFUSE_REG_BASE  (0x54880000)   
#define SOC_IRDA_REG_BASE  (0x54890000)   
#define SOC_I2S_REG_BASE  (0x57810000)   
#define SOC_TRNG_REG_BASE  (0x548A0000)   
#define SOC_XVR_REG_BASE  (0x5A800000)   
#define SOC_AUD_REG_BASE  (0x57800000)   
#define SOC_LA_REG_BASE  (0x55070000)   
#define SOC_JPEG_REG_BASE  (0x58000000)   
#define SOC_JPEG_DEC_REG_BASE  (0x58040000)   
#define SOC_EIP130_REG_BASE  (0x5b000000)   
#define SOC_EIP130OTP_REG_BASE  (0x5b004000)   
#define SOC_EIP130REGITER_REG_BASE  (0x5b008000)   
#define SOC_QSPI_REG_BASE  (0x56040000)   
#define SOC_LCD_DISP_REG_BASE  (0x58060000)   
#define SOC_SDIO_REG_BASE  (0x548B0000)   
#define SOC_USB_REG_BASE  (0x56002000)   
#define SOC_RC_REG_BASE  (0x5980C000)   
#define SOC_TRX_REG_BASE  (0x5980C200)   
#define SOC_POWTBL_REG_BASE  (0x5980C400)   
#define SOC_DPDTBL_REG_BASE  (0x5980C800)   
#define SOC_AGCMEM_REG_BASE  (0x5980A000)   
#define SOC_PEAKCWMEM_REG_BASE  (0x5980D000)   
#define SOC_PSRAM_REG_BASE  (0x56080000)
#define SOC_DMA2D_REG_BASE  (0x58020000)

#endif //__REG_BASE_H__ 
