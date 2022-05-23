#include "base.h"
#include "reg.h"
#include "bk_common_types.h"
#include "bk_flash.h"

static const flash_config_t flash_config[] =
{
    {0x1C7016, 1, 0x400000, 2,  0, 2, 0x1F, 0x1F, 0x00, 0x16, 0x01B, 0, 0, 0xA5, 0x01}, //en_25qh32b
    {0x1C7015, 1, 0x200000, 2,  0, 2, 0x1F, 0x1F, 0x00, 0x0d, 0x0d,  0, 0, 0xA5, 0x01}, //en_25qh16b
    {0x0B4014, 2, 0x100000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0C, 0x101, 9, 1, 0xA0, 0x01}, //xtx_25f08b
    {0x0B4015, 2, 0x200000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0D, 0x101, 9, 1, 0xA0, 0x01}, //xtx_25f16b
    {0x0B4016, 2, 0x400000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0E, 0x101, 9, 1, 0xA0, 0x01}, //xtx_25f32b
    {0x0B4017, 2, 0x800000, 2, 14, 2, 0x1F, 0x05, 0x00, 0x0E, 0x109, 9, 1, 0xA0, 0x01}, //xtx_25f64b
    {0x0E4016, 2, 0x400000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0E, 0x101, 9, 1, 0xA0, 0x01}, //xtx_FT25H32
    {0xC84015, 2, 0x200000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0D, 0x101, 9, 1, 0xA0, 0x01}, //gd_25q16c
    {0xC84016, 1, 0x400000, 2,  0, 2, 0x1F, 0x1F, 0x00, 0x0E, 0x00E, 0, 0, 0xA0, 0x01}, //gd_25q32c
    {0xC86515, 2, 0x200000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0D, 0x101, 9, 1, 0xA0, 0x01}, //gd_25w16e
    {0xEF4016, 2, 0x400000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x00, 0x101, 9, 1, 0xA0, 0x01}, //w_25q32(bfj)
    {0x204016, 2, 0x400000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0E, 0x101, 9, 1, 0xA0, 0x01}, //xmc_25qh32b
    {0xC22315, 1, 0x200000, 2,  0, 2, 0x0F, 0x0F, 0x00, 0x0A, 0x00E, 6, 1, 0xA5, 0x01}, //mx_25v16b
    {0xEB6015, 2, 0x200000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0D, 0x101, 9, 1, 0xA0, 0x01}, //zg_th25q16b
    {0x000000, 2, 0x400000, 2,  0, 2, 0x1F, 0x00, 0x00, 0x00, 0x000, 0, 0, 0x00, 0x01}, //default
};

static const flash_config_t *flash_current_config = NULL;
static FUNC_2PARAM_CB flash_wr_sr_bypass_method_cd = NULL;
static UINT32 flash_id;

static void flash_get_current_flash_config(void)
{
    int i;
	
    for(i = 0; i < (sizeof(flash_config) / sizeof(flash_config_t) - 1); i++)
    {
        if(flash_id == flash_config[i].flash_id)
        {
            flash_current_config = &flash_config[i];
            break;
        }
    }
	
    if(i == (sizeof(flash_config) / sizeof(flash_config_t) - 1))
    {
        flash_current_config = &flash_config[i];
        bk_printf("don't config this flash, choose default config\r\n");
    }
}

void flash_disable_crc(void)
{
    UINT32 value;

    value = REG_READ(REG_FLASH_CONF);
    value &= ~(CRC_EN);
	
    REG_WRITE(REG_FLASH_CONF, value);
}

void flash_enable_crc(void)
{
    UINT32 value;

    value = REG_READ(REG_FLASH_CONF);
    value |= CRC_EN;
	
    REG_WRITE(REG_FLASH_CONF, value);
}

void flash_set_clk(UINT8 clk_conf)
{
    UINT32 value;

    value = REG_READ(REG_FLASH_CONF);
    value &= ~(FLASH_CLK_CONF_MASK << FLASH_CLK_CONF_POSI);
    value |= (clk_conf << FLASH_CLK_CONF_POSI);
	
    REG_WRITE(REG_FLASH_CONF, value);
}

void bk_flash_enable_cpu_data_wr(void)
{
    UINT32 value;

    value = REG_READ(REG_FLASH_CONF);
    value |= (CPU_DATA_WR_MASK << CPU_DATA_WR_POSI);
    REG_WRITE(REG_FLASH_CONF, value);
}

void bk_flash_disable_cpu_data_wr(void)
{
    UINT32 value;

    value = REG_READ(REG_FLASH_CONF);
    value &= (~(CPU_DATA_WR_MASK << CPU_DATA_WR_POSI));
	
    REG_WRITE(REG_FLASH_CONF, value);
}

static void flash_waiting_for_idle(void)
{	
    while(REG_READ(REG_FLASH_OPERATE_SW) & FIELD_BUSY_SW);
}

__attribute__((unused)) static void flash_finish_operation(int wp)
{
	UINT32 value;

	value = REG_READ(REG_FLASH_OP_CTRL);
	value &= (~FIELD_WP_VALUE);
	REG_WRITE(REG_FLASH_OP_CTRL, value | FIELD_OP_SW | (wp & FIELD_WP_VALUE));

	flash_waiting_for_idle();
}

static void flash_finish_operation_with_set_wp(void)
{
    UINT32 value;

	value = REG_READ(REG_FLASH_OP_CTRL);
	REG_WRITE(REG_FLASH_OP_CTRL, value | FIELD_OP_SW | FIELD_WP_VALUE);
	
	flash_waiting_for_idle();
}

static void flash_finish_operation_with_rsv_wp(void)
{
    UINT32 value;

	value = REG_READ(REG_FLASH_OP_CTRL);
	
	REG_WRITE(REG_FLASH_OP_CTRL, value | FIELD_OP_SW);
	
	flash_waiting_for_idle();
}

static void flash_write_enable(void)
{
    UINT32 value;

    value = (FLASH_OPCODE_WREN << OP_TYPE_SW_POSI);
    REG_WRITE(REG_FLASH_OPERATE_SW, value);

    flash_finish_operation_with_set_wp();
}

static void flash_write_disable(void)
{
    UINT32 value;

    value = (FLASH_OPCODE_WRDI << OP_TYPE_SW_POSI);
    REG_WRITE(REG_FLASH_OPERATE_SW, value);
    flash_finish_operation_with_set_wp();
}

static UINT16 flash_read_sr(UINT8 sr_width)
{
	UINT16 sr;
    UINT32 value;

    flash_waiting_for_idle();

    value = (FLASH_OPCODE_RDSR << OP_TYPE_SW_POSI);
    REG_WRITE(REG_FLASH_OPERATE_SW, value);
    flash_finish_operation_with_set_wp();

    value = REG_READ(REG_FLASH_SR_DATA_CRC_CNT);
    sr = value & 0x00FF;

	if(sr_width == 2)
	{
	    value = (FLASH_OPCODE_RDSR2 << OP_TYPE_SW_POSI);
	    REG_WRITE(REG_FLASH_OPERATE_SW, value);
	    flash_finish_operation_with_set_wp();

	    value = REG_READ(REG_FLASH_SR_DATA_CRC_CNT);
	    sr |= (value & 0x00FF) << 8;
	}

    return sr;
}

static void flash_write_sr(UINT8 sr_width,  UINT16 val)
{
    UINT32 value;

	if(flash_wr_sr_bypass_method_cd)
	{
		flash_wr_sr_bypass_method_cd((uint32_t)sr_width, (uint32_t)val);
		return;
	}

    flash_waiting_for_idle();

    value = REG_READ(REG_FLASH_CONF);
    value &= ~(WRSR_DATA_MASK << WRSR_DATA_POSI);

    value |= (val << WRSR_DATA_POSI);
    
    REG_WRITE(REG_FLASH_CONF, value);
    flash_waiting_for_idle();

    if(sr_width == 1)
    {
        value = (FLASH_OPCODE_WRSR << OP_TYPE_SW_POSI);
        REG_WRITE(REG_FLASH_OPERATE_SW, value);
    }
    else if(sr_width == 2)
    {
        value = (FLASH_OPCODE_WRSR2 << OP_TYPE_SW_POSI);
        REG_WRITE(REG_FLASH_OPERATE_SW, value);
    }

    flash_finish_operation_with_set_wp();
}

static UINT8 flash_read_qe(void)
{
    UINT8 temp;
    UINT32 value;

    if(1 == flash_current_config->sr_size)
    {
        value = (FLASH_OPCODE_RDSR << OP_TYPE_SW_POSI);
        REG_WRITE(REG_FLASH_OPERATE_SW, value);
    }
    else
    {
        value = (FLASH_OPCODE_RDSR2 << OP_TYPE_SW_POSI);
        REG_WRITE(REG_FLASH_OPERATE_SW, value);
    }

    flash_finish_operation_with_set_wp();

    value = REG_READ(REG_FLASH_SR_DATA_CRC_CNT);
    temp = (value & 0xFF);
    return temp;
}

static void flash_set_qe(void)
{
    UINT32 value, param;

    flash_waiting_for_idle();

    param = flash_read_sr(flash_current_config->sr_size);
    if((param & (flash_current_config->qe_bit << flash_current_config->qe_bit_post)))
    {
        return ;
    }
    value = REG_READ(REG_FLASH_CONF);
    value &= ~(WRSR_DATA_MASK << WRSR_DATA_POSI);
    value |= (((flash_current_config->qe_bit << flash_current_config->qe_bit_post)
        | param) << WRSR_DATA_POSI);
    REG_WRITE(REG_FLASH_CONF, value);

    value = REG_READ(REG_FLASH_OPERATE_SW);
    
    if(1 == flash_current_config->sr_size)
    {
        value = (value & (ADDR_SW_REG_MASK << ADDR_SW_REG_POSI))
                | (FLASH_OPCODE_WRSR << OP_TYPE_SW_POSI);
    }
    else
    {
        value = (value & (ADDR_SW_REG_MASK << ADDR_SW_REG_POSI))
                | (FLASH_OPCODE_WRSR2 << OP_TYPE_SW_POSI);
    }

    REG_WRITE(REG_FLASH_OPERATE_SW, value);

    flash_finish_operation_with_set_wp();
}

static void flash_set_qwfr(void)
{
    UINT32 value;

    value = REG_READ(REG_FLASH_CONF);
    value &= ~(FIELD_MODE_SEL_MASK << FIELD_MODE_SEL_POSI);
    value |= (flash_current_config->mode_sel << FIELD_MODE_SEL_POSI);
    REG_WRITE(REG_FLASH_CONF, value);
}

static void flash_clr_qwfr(void)
{
    UINT32 value;

    value = REG_READ(REG_FLASH_CONF);
    value &= ~(FIELD_MODE_SEL_MASK << FIELD_MODE_SEL_POSI);
    REG_WRITE(REG_FLASH_CONF, value);

    value = REG_READ(REG_FLASH_OPERATE_SW);
    value = ((0 << ADDR_SW_REG_POSI)
             | (FLASH_OPCODE_CRMR << OP_TYPE_SW_POSI));
    REG_WRITE(REG_FLASH_OPERATE_SW, value);

    flash_finish_operation_with_rsv_wp();
}

static void flash_set_wsr(UINT16 data)
{
    UINT32 value;

    flash_waiting_for_idle();

    value = REG_READ(REG_FLASH_CONF);
    value &= ~(WRSR_DATA_MASK << WRSR_DATA_POSI);
    value |= (data << WRSR_DATA_POSI);
    REG_WRITE(REG_FLASH_CONF, value);

    value = REG_READ(REG_FLASH_OPERATE_SW);
    value = (value & (ADDR_SW_REG_MASK << ADDR_SW_REG_POSI))
            | (FLASH_OPCODE_WRSR2 << OP_TYPE_SW_POSI);
    REG_WRITE(REG_FLASH_OPERATE_SW, value);

    flash_finish_operation_with_set_wp();
}

UINT8 flash_support_wide_voltage(void)
{
    return (flash_id == 0xC86515);
}

UINT8 flash_get_line_mode(void)
{
    return flash_current_config->line_mode;
}

void flash_set_line_mode(UINT8 mode)
{
    UINT32 value;
    
    if(1 == mode)
    {
        flash_clr_qwfr();
    }
    if(2 == mode)
    {
        flash_clr_qwfr();
        value = REG_READ(REG_FLASH_CONF);
        value &= ~(FIELD_MODE_SEL_MASK << FIELD_MODE_SEL_POSI);
        value |= ((MODE_DUAL & FIELD_MODE_SEL_MASK) << FIELD_MODE_SEL_POSI);
        REG_WRITE(REG_FLASH_CONF, value);
    }
    else if(4 == mode)
    {
        flash_clr_qwfr();
        value = REG_READ(REG_FLASH_SR_DATA_CRC_CNT);
        value &= ~(M_VALUE_MASK << M_VALUE_POST);
        value |= (flash_current_config->m_value<< M_VALUE_POST);
        
        REG_WRITE(REG_FLASH_SR_DATA_CRC_CNT, value);

        value = REG_READ(REG_FLASH_SR_DATA_CRC_CNT);

        if(1 == flash_current_config->qe_bit)
        {
            flash_set_qe();
        }

        flash_set_qwfr();
    }
}

static UINT32 flash_get_id(void)
{
    UINT32 value;

    value = (FLASH_OPCODE_RDID << OP_TYPE_SW_POSI);
    REG_WRITE(REG_FLASH_OPERATE_SW, value);

    flash_finish_operation_with_set_wp();
    flash_id = REG_READ(REG_FLASH_RDID_DATA_FLASH);
	
    return flash_id;
}

static UINT32 flash_get_size(void)
{
	if (NULL == flash_current_config)
	{
		return 0x200000;
	}

	return flash_current_config->flash_size;
}

UINT32 flash_is_xtx_type(void)
{
	return ((0x0B4014 == flash_id) || (0x0B4015 == flash_id)
			|| (0x0B4016 == flash_id)
			|| (0x0B4017 == flash_id)
			|| (0x0E4016 == flash_id));
}

UINT32 flash_is_support_0x50h_cmd(void)
{
	return flash_is_xtx_type();
}

UINT32 flash_register_bypass_cb(FUNC_2PARAM_CB cb)
{
	flash_wr_sr_bypass_method_cd = cb;

	return 0;
}

static UINT32 flash_read_mid(void)
{
    UINT32 value;
    UINT32 flash_id;

    flash_waiting_for_idle();

    value = REG_READ(REG_FLASH_OPERATE_SW);
    value = ((value & (ADDR_SW_REG_MASK << ADDR_SW_REG_POSI))
             | (FLASH_OPCODE_RDID << OP_TYPE_SW_POSI));
    REG_WRITE(REG_FLASH_OPERATE_SW, value);
    flash_finish_operation_with_rsv_wp();

    flash_id = REG_READ(REG_FLASH_RDID_DATA_FLASH);

    return flash_id;
}

PROTECT_TYPE bk_flash_get_protect(void)
{
	UINT16 sr_value, cmp, param, value, type;

	sr_value = flash_read_sr(flash_current_config->sr_size);
	param = (sr_value >> flash_current_config->protect_post) & flash_current_config->protect_mask;
	cmp = (sr_value >> flash_current_config->cmp_post) & 0x01;
	value = (cmp << 8) | param;
	
	if(value == flash_current_config->protect_all)
	{
		type = FLASH_PROTECT_ALL;
	}
	else if(value == flash_current_config->protect_none)
	{
		type = FLASH_PROTECT_NONE;
	}
	else if(value == flash_current_config->protect_half)
	{
		type = FLASH_PROTECT_HALF;
	}
	else if(value == flash_current_config->unprotect_last_block)
	{
		type = FLASH_UNPROTECT_LAST_BLOCK;
	}
	else
	{
		type = -1;
	}

	return type;
}

void bk_flash_set_protect(PROTECT_TYPE type)
{
    UINT32 param, value, cmp;
	
	switch (type)
	{
		case FLASH_PROTECT_NONE:
            param = flash_current_config->protect_none & 0xff;
            cmp = (flash_current_config->protect_none >> 8) & 0xff;
            break;
            
		case FLASH_PROTECT_ALL:
			param = flash_current_config->protect_all & 0xff;
			cmp = (flash_current_config->protect_all >> 8) & 0xff;
			break;

        case FLASH_PROTECT_HALF:
			param = flash_current_config->protect_half & 0xff;
			cmp = (flash_current_config->protect_half >> 8) & 0xff;
			break;

        case FLASH_UNPROTECT_LAST_BLOCK:
			param = flash_current_config->unprotect_last_block& 0xff;
			cmp = (flash_current_config->unprotect_last_block >> 8) & 0xff;
			break;
			
		default:
			param = flash_current_config->protect_all & 0xff;
            cmp = (flash_current_config->protect_all >> 8) & 0xff;
			break;
	}
    
    value = flash_read_sr(flash_current_config->sr_size);

	if(((param << flash_current_config->protect_post) != 
        (value & (flash_current_config->protect_mask << flash_current_config->protect_post)))
        || ((cmp << flash_current_config->cmp_post) !=
        (value & (0x01 << flash_current_config->cmp_post))))
	{
        value = (value & (~(flash_current_config->protect_mask 
			            << flash_current_config->protect_post))) 
			            | (param << flash_current_config->protect_post);
		value &= ~(1 << flash_current_config->cmp_post);
		value |= ((cmp & 0x01) << flash_current_config->cmp_post);
		
		bk_printf("--write status reg:%x,%x--\r\n", value, flash_current_config->sr_size);
		flash_write_sr(flash_current_config->sr_size, value);
	}
}

void bk_flash_erase_sector(UINT32 address)
{
    UINT32 value;
    UINT32 erase_addr = address & 0xFFF000;

    if(erase_addr >= flash_current_config->flash_size)
    {
        bk_printf("Erase error:invalid address0x%x\r\n", erase_addr);
        return;
    }

    flash_waiting_for_idle();
    value = REG_READ(REG_FLASH_OPERATE_SW);
    value = ((erase_addr << ADDR_SW_REG_POSI)
             | (FLASH_OPCODE_SE << OP_TYPE_SW_POSI));
    REG_WRITE(REG_FLASH_OPERATE_SW, value);
    flash_finish_operation_with_rsv_wp();
}

static void flash_set_hpm(void)
{
    UINT32 value;

    flash_waiting_for_idle();
    value = REG_READ(REG_FLASH_OPERATE_SW);
    value = ((0x0 << ADDR_SW_REG_POSI)
             | (FLASH_OPCODE_HPM << OP_TYPE_SW_POSI));
    REG_WRITE(REG_FLASH_OPERATE_SW, value);
    flash_finish_operation_with_rsv_wp();
}

static void flash_read_data(UINT8 *buffer, UINT32 address, UINT32 len)
{
    UINT32 i, reg_value;
    UINT32 addr = address & (~0x1F);
    UINT32 buf[8];
    UINT8 *pb = (UINT8 *)&buf[0];

    if(len == 0)
    {
        return;
    }

    flash_waiting_for_idle();
    while(len)
    {
        reg_value = REG_READ(REG_FLASH_OPERATE_SW);
        reg_value = ((addr << ADDR_SW_REG_POSI)
                     | (FLASH_OPCODE_READ << OP_TYPE_SW_POSI));
        REG_WRITE(REG_FLASH_OPERATE_SW, reg_value);
        flash_finish_operation_with_rsv_wp();
        addr += 32;

        for(i = 0; i < 8; i++)
        {
            buf[i] = REG_READ(REG_FLASH_DATA_FLASH_SW);
        }

        for(i = address % 32; i < 32; i++)
        {
            *buffer++ = pb[i];
            address++;
            len--;
            if(len == 0)
            {
                break;
            }
        }
    }
}

static void flash_write_data(UINT8 *buffer, UINT32 address, UINT32 len)
{
    UINT32 i, reg_value;
    UINT32 addr = address & (~0x1F);
    UINT32 buf[8];
    UINT8 *pb = (UINT8 *)&buf[0];

    if((addr >= flash_current_config->flash_size)
        || (len > flash_current_config->flash_size)
        || ((addr + len) > flash_current_config->flash_size))
    {
        bk_printf("Write error[addr:0x%x len:0x%x]\r\n", addr, len);
        return;
    }

    if(address % 32)
    {
        flash_read_data(pb, addr, 32);
    }
    else
    {
        memset(pb, 0xFF, 32);
    }

    flash_waiting_for_idle();
    while(len)
    {
        if(len < 32)
        {
            flash_read_data(pb, addr, 32);
            flash_waiting_for_idle();
        }
        for (i = address % 32; i < 32; i++)
        {
            pb[i] = *buffer++;
            address++;
            len--;
            if (len == 0)
                break;
        }

        for (i = 0; i < 8; i++)
        {
            REG_WRITE(REG_FLASH_DATA_SW_FLASH, buf[i]);
        }

        reg_value = REG_READ(REG_FLASH_OPERATE_SW);
        reg_value = ((addr << ADDR_SW_REG_POSI)
                     | (FLASH_OPCODE_PP << OP_TYPE_SW_POSI));
        REG_WRITE(REG_FLASH_OPERATE_SW, reg_value);
        flash_finish_operation_with_rsv_wp();
		
        addr += 32;
        memset(pb, 0xFF, 32);
    }
}

void flash_protection_op(UINT8 mode, PROTECT_TYPE type)
{
	bk_flash_set_protect(type);
}

void bk_flash_init(void)
{
    UINT32 id;

    flash_waiting_for_idle();
	
    id = flash_get_id();
    FLASH_PRT("[Flash]id:0x%x\r\n", id);
	(void)id;
    flash_get_current_flash_config();
	
	bk_flash_set_protect(FLASH_UNPROTECT_LAST_BLOCK);

	#if (0 == CFG_JTAG_ENABLE)
	bk_flash_disable_cpu_data_wr();
	#endif
	
    flash_set_line_mode(flash_current_config->line_mode);
    flash_enable_crc();
	
    flash_set_clk(5);  // 60M

    bk_printf("[Flash]init over\r\n");
}

void flash_exit(void)
{
}

UINT32 bk_flash_read(char *user_buf, UINT32 count, UINT32 address)
{
    flash_read_data((UINT8 *)user_buf, address, count);

    return FLASH_SUCCESS;
}

UINT32 bk_flash_write(char *user_buf, UINT32 count, UINT32 address)
{
    if(4 == flash_current_config->line_mode)
    {
        flash_set_line_mode(LINE_MODE_TWO);
    }

    flash_write_data((UINT8 *)user_buf, address, count);

    if(4 == flash_current_config->line_mode)
    {
        flash_set_line_mode(LINE_MODE_FOUR);
    }

    return FLASH_SUCCESS;
}


UINT32 bk_flash_ctrl(UINT32 cmd, void *parm)
{
    UINT8 clk;
    UINT16 wsr;
    UINT32 address;
    UINT32 reg;
    UINT32 ret = FLASH_SUCCESS;
    
    if(4 == flash_current_config->line_mode)
    {
        flash_set_line_mode(LINE_MODE_TWO);
    }
        
    switch(cmd)
    {
    case CMD_FLASH_SET_CLK:
        clk = (*(UINT8 *)parm);
        flash_set_clk(clk);
        break;

    case CMD_FLASH_SET_DPLL:
        reg = REG_READ(REG_FLASH_CONF);
        reg &= ~(FLASH_CLK_CONF_MASK << FLASH_CLK_CONF_POSI);
        reg = reg | (5 << FLASH_CLK_CONF_POSI);
        REG_WRITE(REG_FLASH_CONF, reg);
        break;

    case CMD_FLASH_SET_DCO:
        reg = REG_READ(REG_FLASH_CONF);
        reg &= ~(FLASH_CLK_CONF_MASK << FLASH_CLK_CONF_POSI);
        if (0) {
            reg = reg | (0xB << FLASH_CLK_CONF_POSI);
        } else {
            reg = reg | (9 << FLASH_CLK_CONF_POSI);
        }
        REG_WRITE(REG_FLASH_CONF, reg);
        break;

    case CMD_FLASH_WRITE_ENABLE:
        flash_write_enable();
        break;

    case CMD_FLASH_WRITE_DISABLE:
        flash_write_disable();
        break;

    case CMD_FLASH_READ_SR:
        (*(UINT16 *)parm) = flash_read_sr(2);
        break;

    case CMD_FLASH_WRITE_SR:
        flash_write_sr(*(unsigned long *)parm & 0x00FF, ((*(unsigned long *)parm) >> 8) & 0x00FFFF);
        break;

    case CMD_FLASH_READ_QE:
        (*(UINT8 *)parm) = flash_read_qe();
        break;

    case CMD_FLASH_SET_QE:
        if(flash_current_config->qe_bit)
        {
            flash_set_qe();
        }
        break;

    case CMD_FLASH_SET_QWFR:
        flash_set_qwfr();
        break;

    case CMD_FLASH_CLR_QWFR:
        flash_clr_qwfr();
        break;

    case CMD_FLASH_SET_WSR:
        wsr = (*(UINT16 *)parm);
        flash_set_wsr(wsr);
        break;

    case CMD_FLASH_GET_ID:
        (*(UINT32 *)parm) = flash_get_id();
        break;

    case CMD_FLASH_GET_SIZE:
        (*(UINT32 *)parm) = flash_get_size();
        break;

    case CMD_FLASH_READ_MID:
        (*(UINT32 *)parm) = flash_read_mid();
        break;
		
	case CMD_FLASH_GET_PROTECT:
		(*(UINT32 *)parm) = bk_flash_get_protect();
		break;

    case CMD_FLASH_ERASE_SECTOR:
        address = (*(UINT32 *)parm);
        bk_flash_erase_sector(address);
        break;

    case CMD_FLASH_SET_HPM:
        flash_set_hpm();
        break;
	
	case CMD_FLASH_SET_PROTECT:
		reg =  (*(UINT32 *)parm);
		flash_protection_op(FLASH_XTX_16M_SR_WRITE_DISABLE, reg);
		break;
		
    default:
        ret = FLASH_FAILURE;
        break;
    }
    
    if(4 == flash_current_config->line_mode)
    {        
        flash_set_line_mode(LINE_MODE_FOUR);
    }

    return ret;
}
// eof

