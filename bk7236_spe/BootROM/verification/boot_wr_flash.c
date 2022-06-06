#include "base.h"
#include "bk_uart.h"
#include "bk_flash.h"
#include "boot_wr_flash.h"
#include "flash_operation.h"

/**
 ****************************************************************************************
 * @brief Count ones.
 * @param[in] val Value to count the number of ones.
 * @return Number of ones when value is written as 32 bits.
 ****************************************************************************************
 */
uint32_t boot_count_one_num(uint32_t n)
{
	uint32_t count = 0;

	while (n)
	{
		n &= n-1;
		count ++;
	}
	
	return count;
}

/**
 ****************************************************************************************
 * @brief Get block index
 * @param[in] null.
 * @return fixed-size blcok index, which includes current field.
 ****************************************************************************************
 */
uint32_t boot_get_chunk_id(SEC_OBJ_T *sec_obj)
{
	int i, cnt = 0;
	hal_ret_t ret;
	uint32_t id = 0, addr, id_field_len;
	uint32_t id_field[1 + (DATA_CHUNK_MAX_IDX >> 5)] = {0};

	/* id0 is a management block id*/
	addr = FLASH_ADDR_FROM_ID(sec_obj, id);
	id_field_len = DATA_CHUNK_MAX_IDX % 32 ? sizeof(id_field) : (DATA_CHUNK_MAX_IDX >> 5);
	ret = flash_op_read(addr, (uint8_t *)id_field, id_field_len << 2);

	/*count zero-value bit*/
	for(i = 0; i < id_field_len; i ++){
		if(0xFFFFFFFF == id_field[i]){
			break;
		}else{
			cnt += 32 - boot_count_one_num(id_field[i]);
		}
	}

	(void)ret;
	
	return cnt;
}


/**
 ****************************************************************************************
 * @brief Set block index. The flash default bit value is 1, afer erasing; the bit value:0
 *        means the corresponding block's position
 * @param[in] index value.
 * @return fixed-size blcok index, which includes current field.
 ****************************************************************************************
 */
uint32_t boot_set_chunk_id(SEC_OBJ_T *sec_obj, uint32_t index)
{
	hal_ret_t ret;
	uint32_t round, mod;
	uint32_t id = 0, addr;
	uint32_t original_val;

	if(0 == index)
		return BRET_INVALID;
	
	round = index >> 5;
	mod = index % 32;
	
	addr = FLASH_ADDR_FROM_ID(sec_obj, id);
	addr += round << 2;
	ret = flash_op_read(addr, (uint8_t *)&original_val, sizeof(original_val));
	
	original_val &= (~(1 << mod));
	ret = flash_op_write(addr, (uint8_t *)&original_val, sizeof(original_val));
	
	return ret;
}

/**
 ****************************************************************************************
 * @brief Get next chunk's information
 * @param[in] sec_obj: flash sector object, it has the base address;
 * @param[out] is_erase: check whether the sector shall be erased, before writing next chunk;
 * @param[out] next_index: next chunk index;
 * @return operation status.
 ****************************************************************************************
 */
uint32_t boot_get_next_chunk_info(SEC_OBJ_T *sec_obj, uint32_t *is_erase, uint32_t *next_index)
{
	uint32_t id, next_id;
	uint32_t erase_flag = 0;
	
	id = boot_get_chunk_id(sec_obj);	
	next_id = id + 1;
	
	if(DATA_CHUNK_MAX_IDX == next_id)
	{
		next_id = 1; /* index:0 has reserved for management*/
		erase_flag = 1;
	}

	*is_erase = erase_flag;
	*next_index = next_id;

	return 0;
}

/**
 ****************************************************************************************
 * @brief Write chunk content, based on chunk index of management block
 * @param[in] sec_obj: flash sector object, it has the base address;
 * @param[in] chunk_ptr: write buffer;
 * @param[in] chunk_size: write size;
 * @return operation status.
 ****************************************************************************************
 */
hal_ret_t boot_write_chunk(SEC_OBJ_T *sec_obj, void *chunk_ptr, uint32_t chunk_size)
{
	hal_ret_t ret;
	uint32_t next_id, addr;
	uint32_t is_erase_flag = 0;

	boot_get_next_chunk_info(sec_obj, &is_erase_flag, &next_id);
	addr = FLASH_ADDR_FROM_ID(sec_obj, next_id);
	PAL_ASSERT(RW_DATA_CHUNK_SIZE >= chunk_size);

	if(is_erase_flag)
	{
		ret = flash_op_erase(FLASH_ADDR_FROM_ID(sec_obj, 0), ERASE_SECTOR_SIZE);
	}
	
	ret = flash_op_write(addr, (uint8_t *)chunk_ptr, chunk_size);
	boot_set_chunk_id(sec_obj, next_id);
	
	return ret;
}

/**
 ****************************************************************************************
 * @brief Read chunk content, based on chunk index of management block
 * @param[in] sec_obj: flash sector object, it has the base address;
 * @param[out] chunk_ptr: read buffer;
 * @param[in] chunk_size: read size;
 * @return operation status.
 ****************************************************************************************
 */
hal_ret_t boot_read_chunk(SEC_OBJ_T *sec_obj, void *chunk_ptr, uint32_t chunk_size)
{
	hal_ret_t ret;
	uint32_t id, addr;

	id = boot_get_chunk_id(sec_obj);
	if(0 == id){
		return HAL_ERR_GENERIC;
	}
	
	addr = FLASH_ADDR_FROM_ID(sec_obj, id);
	PAL_ASSERT(RW_DATA_CHUNK_SIZE >= chunk_size);
	
	ret = flash_op_read(addr, (uint8_t *)chunk_ptr, chunk_size);
	
	return ret;
}
// eof

