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

#include "base.h"
#include "app.h"

#define DBG_PRE_PROMPT "APP"
#include "dbg.h"

#ifndef __ARMCC_VERSION
extern const app_desc_t __apps_start;
extern const app_desc_t __apps_end;

void apps_init(void)
{
    const app_desc_t *app = NULL;

    for (app = &__apps_start; app != &__apps_end; app++) {
        if (app->entry) {
            DBG("%s entry\n", app->name);
            app->entry();
        }
    }
}
#else
void apps_init(void)
{
}
#endif
