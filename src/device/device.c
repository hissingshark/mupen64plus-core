/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - device.c                                                *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2016 Bobby Smiles                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "device.h"

#include "ai/ai_controller.h"
#include "memory/memory.h"
#include "pi/pi_controller.h"
#include "r4300/r4300_core.h"
#include "rdp/rdp_core.h"
#include "ri/ri_controller.h"
#include "rsp/rsp_core.h"
#include "si/si_controller.h"
#include "vi/vi_controller.h"


void init_device(struct device* dev,
    /* r4300 */
    unsigned int emumode,
    unsigned int count_per_op,
    int no_compiled_jump,
    /* ai */
    struct audio_out_backend* aout,
    /* pi */
    uint8_t* rom, size_t rom_size,
    struct storage_backend* flashram_storage,
    struct storage_backend* sram_storage,
    /* ri */
    uint32_t* dram, size_t dram_size,
    /* si */
    struct controller_input_backend* cins,
    struct storage_backend* mpk_storages,
    struct rumble_backend* rumbles,
    uint16_t eeprom_id, struct storage_backend* eeprom_storage,
    struct clock_backend* clock,
    /* vi */
    unsigned int vi_clock, unsigned int expected_refresh_rate, unsigned int count_per_scanline, unsigned int alternate_timing)
{
    init_r4300(&dev->r4300, emumode, count_per_op, no_compiled_jump);
    init_rdp(&dev->dp, &dev->r4300, &dev->sp, &dev->ri);
    init_rsp(&dev->sp, &dev->r4300, &dev->dp, &dev->ri);
    init_ai(&dev->ai, &dev->r4300, &dev->ri, &dev->vi, aout);
    init_pi(&dev->pi, rom, rom_size, flashram_storage, sram_storage, &dev->r4300, &dev->ri);
    init_ri(&dev->ri, dram, dram_size);
    init_si(&dev->si,
        cins,
        mpk_storages,
        rumbles,
        eeprom_id, eeprom_storage,
        clock,
        rom + 0x40,
        &dev->r4300, &dev->ri);
    init_vi(&dev->vi, vi_clock, expected_refresh_rate, count_per_scanline, alternate_timing, &dev->r4300);
}

void poweron_device(struct device* dev)
{
    poweron_r4300(&dev->r4300);
    poweron_rdp(&dev->dp);
    poweron_rsp(&dev->sp);
    poweron_ai(&dev->ai);
    poweron_pi(&dev->pi);
    poweron_ri(&dev->ri);
    poweron_si(&dev->si);
    poweron_vi(&dev->vi);
    poweron_memory(&dev->mem);
}
