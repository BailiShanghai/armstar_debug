/**
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

#ifndef __SECURE_BOOT_CONFIG_H__
#define __SECURE_BOOT_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \def     SECBOOT_ENABLE_EXT_PROG
 * \brief   Enable or disable processing extended program.
 *          When this is disabled, an manifest with the extended program will be
 *          treated as invalid manifest, and \c HAL_ERR_GENERIC is returned.
 *          When this is enabled, the extended program in manifest will be
 *          verified and executed.
 */
////#define SECBOOT_ENABLE_EXT_PROG		////Disable

/**
 * \def     SECBOOT_ENABLE_IMG_ENC
 * \brief   Enable or disable image encryption.
 *          When this is disabled, the image encryption feature will be totally
 *          removed from secure boot core, and when detecting an encrypted image
 *          in manifest, secure boot core will return
 *          \c HAL_ERR_GENERIC.
 *          Also, all cipher related configurations in HAL will be ignored.
 *          When this is enabled, image encryption feature is enabled, and one
 *          of the cipher scheme must be enabled too.
 */
////#define SECBOOT_ENABLE_IMG_ENC		////Disable, Not support

/**
 * \def     SECBOOT_ENABLE_XIP
 * \brief   Enable or disable XIP image.
 *          When this is disabled, XIP feature will be totally removed from
 *          secure boot core. And \c HAL_ERR_GENERIC is returned if
 *          one XIP image is detected in manifest.
 *          When this is enabled, XIP feature is enabled.
 */
#define SECBOOT_ENABLE_XIP			////Enalbe,

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURE_BOOT_CONFIG_H__ */

/*************************** The End Of File*****************************/
