#pragma once

#ifdef __cplusplus
extern "C" {
#endif

enum
{
	STATE_SECURE = 0,
	STATE_NON_SECURE
};

#define REG_GAP_LOCATION_NUM    (((0x50 - 0x14) >> 2) - 1)

typedef volatile struct {
	/* REG_0x00 ITCMCR */
	union {
		struct {
			uint32_t en:1; /**< bit[0] TCM enable */
			uint32_t res0: 2; /**< bit[2:1] Reserved */
			uint32_t size: 4; /**< bit[6:3] tcm size*/
			uint32_t res1: 25; /**< bit[31:7] reserved */
		};
		uint32_t v;
	} itcm_ctrl;
	
	/* REG_0x01 DTCMCR */
	union {
		struct {
			uint32_t en:1; /**< bit[0] TCM enable */
			uint32_t res0: 2; /**< bit[2:1] Reserved */
			uint32_t size: 4; /**< bit[6:3] tcm size*/
			uint32_t res1: 25; /**< bit[31:7] reserved */
		};
		uint32_t v;
	} dtcm_ctrl;

	uint32_t reg_gap[REG_GAP_LOCATION_NUM];
	
	/* REG_0x01 DTCMCR */
	union {
		struct {
			uint32_t en:1; /**< bit[0] TCM enable */
			uint32_t res0: 2; /**< bit[2:1] Reserved */
			uint32_t size: 4; /**< bit[6:3] tcm size*/
			uint32_t res1: 25; /**< bit[31:7] reserved */
		};
		uint32_t v;
	} mem_model;
} tcm_hw_t;

#define TGU_LOOKUP_TABLE_MAX_CNT   (16)
#define TGU_RESV_CNT (((0x10 - 0x04) >> 2) - 1)

typedef volatile struct {
	/* REG_0x00 */
	union {
		struct {
			uint32_t data_busfault_en: 1;
			uint32_t ahbt_err_resp: 1; 
			uint32_t reserved: 30;
		};
		uint32_t v;
	} tgu_ctrl;
	
	/* REG_0x01 */
	union {
		struct {
			uint32_t blk_size: 4;
			uint32_t resv0: 4; 
			uint32_t num_of_blks: 4;
			uint32_t reserved: 19;
			uint32_t tgu_present: 1;
		};
		uint32_t v;
	} tgu_cfg;
	
	uint32_t resv[TGU_RESV_CNT];
	
	/* REG_0x01 */
	uint32_t sec_lookup_tab[TGU_LOOKUP_TABLE_MAX_CNT];
} tgu_hw_t;

#ifdef __cplusplus
}
#endif
// eof


