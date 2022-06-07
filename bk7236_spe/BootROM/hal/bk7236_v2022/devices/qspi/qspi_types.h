#ifndef _QSPI_TYPES_H_
#define _QSPI_TYPES_H_

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

enum {
	QSPI_STATE_SECURE = 0,
	QSPI_STATE_NON_SECURE
};

enum {
	QSPI_PORT_0 = 0,
	QSPI_PORT_1,
	QSPI_PORT_MAX
};

#ifdef __cplusplus
extern "C" }
#endif

#endif
