/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __APP_H__
#define __APP_H__

void apps_init(void);

typedef void (*app_entry)(void);

typedef struct _app_desc_t {
    const char *name;
    app_entry   entry;
} app_desc_t;

#define APP_START(_name)                                                       \
    static const app_desc_t _app_##_name __attribute__((__used__))             \
        __attribute__((aligned(sizeof(void *))))                               \
            __attribute__((__section__(".apps_data"))) = {.name = #_name,

#define APP_END                                                                \
    }                                                                          \
    ;

#endif /* __APP_H__ */
