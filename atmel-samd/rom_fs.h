/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Scott Shawcroft for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __MICROPY_INCLUDED_ATMEL_SAMD_ROM_FS_H__
#define __MICROPY_INCLUDED_ATMEL_SAMD_ROM_FS_H__

#include "asf/common/services/storage/ctrl_access/ctrl_access.h"


Ctrl_status rom_fs_test_unit_ready(void);
Ctrl_status rom_fs_read_capacity(uint32_t *u32_nb_sector);
bool        rom_fs_wr_protect(void);
bool        rom_fs_removal(void);
Ctrl_status rom_fs_usb_read_10(uint32_t addr, uint16_t nb_sector);
Ctrl_status rom_fs_usb_write_10(uint32_t addr, uint16_t nb_sector);

#endif  // __MICROPY_INCLUDED_ATMEL_SAMD_ROM_FS_H__
