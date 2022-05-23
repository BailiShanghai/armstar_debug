/*
 * Copyright (C), 2018-2021, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#include "ce_lite_config.h"

/**
 * There are some TRNG examples for your reference.
 *
 * Example List:
 *  - Example 1: trng_dump_example.
 *               It shows you how to re-configure the TRNG and leverage the
 *               customized configurations to read specifed size of random
 *               data. And this example is active when you enable macro
 *               \c CE_LITE_TRNG and \c CE_LITE_TRNG_DUMP.
 *
 *  - Example 2: trng_read_example.
 *               It shows you how to read random data from TRNG module.
 *               And this example is active when you enable macro
 *               \c CE_LITE_TRNG.
 *
 */

#if defined(CE_LITE_TRNG)
#include "pal_log.h"
#include "ce_lite_trng.h"

#if defined(CE_LITE_TRNG_DUMP)

static void on_data(void *data, size_t len)
{
    /**
     *  deal with the random data here, and here just print out
     *  the random data for demo.
     */
    PAL_LOG_DUMP_BUF("Captured random Data:",
                            (const uint8_t *)data, len);
}

static void on_error(uint32_t err)
{
    /**
     * Check out the health check statistics here, and just print out
     * the error value for demo.
     */
    PAL_LOG_INFO("Health Check status:%X\n", err);
}

static int32_t trng_dump_example(void)
{
#define MAX_PACKET_SIZE     (128U)
    int32_t ret = 0;
    uint8_t buf[MAX_PACKET_SIZE] = {0};
    /**
     *  Set up the request.
     *  \p b_conf must be true if you want to override the configuration.
     *  when \p b_config is \c true, you must specify your customized
     *  configurations \p conf.
     *  Once you want TRNG to generate random data block by block in a call,
     *  you need to set up callbacks \p on_data for receiving the
     *  random data and \p buf for storing the random data block, also you
     *  must specify the block size and block number you want to read by
     *  setting up \p size and \p nmemb, then the total size is \p size *
     *  \p nmemb.
     */
    ce_trng_request_t req = {
        .b_conf = true,
        .conf = {
            .src = {
                .grp0_en = 1,
                .grp1_en = 1,
                .grp2_en = 1,
                .grp3_en = 1,
                .src_sel = CE_TRNG_ENTROPY_SRC_INTERNAL,
            },
            .sample = {
                .div = 100,
                .dly = 3,
            },
            .ro = {
                .grp0_taps_en = 4,
                .grp1_taps_en = 5,
                .grp2_taps_en = 6,
                .grp3_taps_en = 7,
            },
            .postproc = {
                .prng_bypass = 1,
                .vn_bypass = 0,
                .crng_bypass = 0,
                .rsvd = 0,
                .lfsr_drop_num = 0,
                .lfsr_sel = 0,
                .fast_mode = 1,
            },
            .eval = {
                .adap_tst_th = 589,
                .rep_tst_th = 11,
                .adap_tst_en = 1,
                .rep_tst_en = 1,
                .ac_tst_en = 1,
                .ac_tst_th = 10,
            },
            .thr = {
                .vn_err_th = 1,
                .crng_err_th = 1,
                .rep_err_th = 1,
                .adap_err_th = 1,
            }
        },
        .buf = buf,
        .size = MAX_PACKET_SIZE,
        .nmemb = 10,
        .on_data = on_data,
        .on_error = on_error,
    };

    ret = ce_trng_dump(&req);
    if (ret != 0) {
        PAL_LOG_ERR("ce_trng_dump FAILED(%X)\n", ret);
    } else {
        PAL_LOG_INFO("ce_trng_dump SUCCESS\n", ret);
    }
    return ret;
#undef MAX_PACKET_SIZE
}
#endif /* CE_LITE_TRNG_DUMP */

static int32_t trng_read_example(void)
{
    int32_t ret = 0;
    uint8_t buf[256] = {0};

    ret = ce_trng_read(buf, sizeof(buf));

    if (ret != 0) {
        PAL_LOG_ERR("ce_trng_read FAILED(%X)\n", ret);
    } else {
        PAL_LOG_INFO("ce_trng_read SUCCESS\n");
        PAL_LOG_DUMP_BUF("random data:", buf, sizeof(buf));
    }
    return ret;
}

int32_t trng_example_entry(void)
{
    int32_t ret = 0;

#if defined(CE_LITE_TRNG_DUMP)
    PAL_LOG_INFO("TRNG dump example...\n");
    ret = trng_dump_example();
    if (ret != 0) {
        goto fin;
    }
#endif /* CE_LITE_TRNG_DUMP */

    PAL_LOG_INFO("TRNG read example...\n");
    ret = trng_read_example();

fin:
    return ret;
}
#endif /* CE_LITE_TRNG */
