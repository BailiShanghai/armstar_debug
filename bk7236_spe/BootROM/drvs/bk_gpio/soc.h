// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "bk_common_types.h"
#include "bk_err.h"
#include "reg.h"
#include "reg_base.h"
#include "os_port.h"
#include "ps_port.h"
#include "printf_port.h"
#include "bk_assert.h"
#include "bk_log.h"
#include "pal_string.h"
#include "hal_log.h"

#ifdef __cplusplus
extern "C" {
#endif

#define REG_GET_BIT(_r, _b)  ({\
		(*(volatile uint32_t*)(_r) & (_b));\
	})

#define REG_SET_BIT(_r, _b)  ({\
		(*(volatile uint32_t*)(_r) |= (_b));\
	})

#define REG_CLR_BIT(_r, _b)  ({\
		(*(volatile uint32_t*)(_r) &= ~(_b));\
	})

#define REG_SET_BITS(_r, _b, _m) ({\
		(*(volatile uint32_t*)(_r) = (*(volatile uint32_t*)(_r) & ~(_m)) | ((_b) & (_m)));\
	})

#define REG_GET_FIELD(_r, _f) ({\
		((REG_READ(_r) >> (_f##_S)) & (_f##_V));\
	})

#define REG_SET_FIELD(_r, _f, _v) ({\
		(REG_WRITE((_r),((REG_READ(_r) & ~((_f##_V) << (_f##_S)))|(((_v) & (_f##_V))<<(_f##_S)))));\
	})

#define REG_MCHAN_GET_FIELD(_ch, _r, _f) ({\
		((REG_READ(_r) >> (_f##_MS(_ch))) & (_f##_V));\
	})

#define REG_MCHAN_SET_FIELD(_ch, _r, _f, _v) ({\
		(REG_WRITE((_r), ((REG_READ(_r) & ~((_f##_V) << (_f##_MS(_ch))))|(((_v) & (_f##_V))<<(_f##_MS(_ch))))));\
	})

#define VALUE_GET_FIELD(_r, _f) (((_r) >> (_f##_S)) & (_f))

#define VALUE_GET_FIELD2(_r, _f) (((_r) & (_f))>> (_f##_S))

#define VALUE_SET_FIELD(_r, _f, _v) ((_r)=(((_r) & ~((_f) << (_f##_S)))|((_v)<<(_f##_S))))

#define VALUE_SET_FIELD2(_r, _f, _v) ((_r)=(((_r) & ~(_f))|((_v)<<(_f##_S))))

#define FIELD_TO_VALUE(_f, _v) (((_v)&(_f))<<_f##_S)

#define FIELD_TO_VALUE2(_f, _v) (((_v)<<_f##_S) & (_f))

#ifndef BIT
#define BIT(i) (1<<(i))
#endif

#ifndef BIT64
#define BIT64(i)                  (1LL << (i))
#endif

#ifdef __cplusplus
}
#endif
