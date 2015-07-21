/**
 * @file   TZ01_console.h
 * @brief  Serial console library for Cerevo CDP-TZ01B.
 *
 * @author Cerevo Inc.
 */

/*
Copyright 2015 Cerevo Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _TZ01_CONSOLE_H_
#define _TZ01_CONSOLE_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Console Initialize
 *
 * @return Initialize result.
 * @retval true : Success.
 * @retval false: Failed.
 */
bool TZ01_console_init(void);

/**
 * @brief Get character.
 *
 * @param[out] c Got character.
 * @return  Result
 * @retval true : Success.
 * @retval false: Failed.
 */
bool TZ01_console_getc(uint8_t *c);

/**
 * @brief Put character.
 *
 + @param[in] c Put character.
 * @return Result.
 * @retval true : Success.
 * @retval false: Failed.
 */
bool TZ01_console_putc(uint8_t c);

/**
 * @brief Get string.
 *
 * @param buf String buffer.
 * @param max_len Max string length.
 * @return Number of characters actually read.
 */
int TZ01_console_gets(char *buf, int max_len);

/**
 * @brief Put String.
 *
 * @param buf String buffer.
 *  `buf' should have been null terminated.
 * @return Number of byte written.
 */
int TZ01_console_puts(char *buf);

/**
 * @brief Read byte data.
 *
 * @param buf Buffer pointer.
 * @param len Read length.
 * @return Number of bytes actually read.
 */
int TZ01_console_read(uint8_t *buf, int len);

/**
 * @brief Write byte data.
 *
 * @param buf Buffer pointer.
 * @param len Write data length.
 * @return Number of byte written.
 */
int TZ01_console_write(uint8_t *buf, int len);

#endif  //_TZ01_CONSOLE_H_
