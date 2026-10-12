#include "include/memory.h"
MemoryGroup memory_bus;
void write_to_memory_via_databus(word_type address, word_type value) {
    if (address<=0x00003FFF) {
        // WRAM - BIOS - System ROM
        memory_bus.general_internal_memory.SystemROM[address] = value;
    }
    else if (0x02000000 <= address && address<=0x0203FFFF) {
        // WRAM - WRAM - On-board Work RAM
        memory_bus.general_internal_memory.WorkRAM_on_board[address-0x02000000] = value;
    }
    else if (0x03000000 <= address && address<=0x03007FFF) {
        // WRAM - On-chip Work RAM
        memory_bus.general_internal_memory.WorkRAM_on_board[address-0x03000000] = value;
    }
    else if (0x04000000 <= address && address<=0x040003FE) {
        // I/O Registers
        memory_bus.general_internal_memory.WorkRAM_on_board[address-0x04000000] = value;
    }
    else if (0x05000000 <= address && address<=0x050003FF) {
        // BG/OBJ Palette RAM
        memory_bus.internal_display_memory.BG_OBJ_Pallete[address-0x05000000] = value;
    }
    else if (0x06000000 <= address && address<=0x06017FFF) {
        // VRAM - Video RAM
        memory_bus.internal_display_memory.VRAM[address-0x06000000] = value;
    }
    else if (0x07000000 <= address && address<=0x070003FF) {
        // OAM - OBJ Attributes
        memory_bus.internal_display_memory.OAM_obj_attributes[address-0x07000000] = value;
    }
    else if (0x08000000 <= address && address<=0x09FFFFFF) {
        // Game Pak ROM/FlashROM wait state 0
        memory_bus.game_pak_memory.game_pak_ROM_slot0[address-0x08000000] = value;
    }
    else if (0x0A000000 <= address && address<=0x0BFFFFFF) {
        // Game Pak ROM/FlashROM wait state 1
        memory_bus.game_pak_memory.game_pak_ROM_slot1[address-0x0A000000] = value;
    }
    else if (0x0C000000 <= address && address<=0x0DFFFFFF) {
        // Game Pak ROM/FlashROM wait state 2
        memory_bus.game_pak_memory.game_pak_ROM_slot2[address-0x0C000000] = value;
    }
    else if (0x0E000000 <= address && address<=0x0E00FFFF) {
        // Game Pak SRAM
        memory_bus.game_pak_memory.game_pak_SRAM[address-0x0E000000] = value;
    }
}

word_type read_from_memory_via_databus(word_type address) {
    if (address<=0x00003FFF) {
        // WRAM - BIOS - System ROM
        return memory_bus.general_internal_memory.SystemROM[address];
    }
    if (0x02000000 <= address && address<=0x0203FFFF) {
        // WRAM - WRAM - On-board Work RAM
        return memory_bus.general_internal_memory.WorkRAM_on_board[address-0x02000000];
    }
    if (0x03000000 <= address && address<=0x03007FFF) {
        // WRAM - On-chip Work RAM
        return memory_bus.general_internal_memory.WorkRAM_on_chip[address-0x03000000];
    }
    if (0x04000000 <= address && address<=0x040003FE) {
        // I/O Registers
        return memory_bus.general_internal_memory.IO_Registers[address-0x04000000];
    }
    if (0x05000000 <= address && address<=0x050003FF) {
        // BG/OBJ Palette RAM
        return memory_bus.internal_display_memory.BG_OBJ_Pallete[address-0x05000000];
    }
    if (0x06000000 <= address && address<=0x06017FFF) {
        // VRAM - Video RAM
        return memory_bus.internal_display_memory.VRAM[address-0x06000000];
    }
    if (0x07000000 <= address && address<=0x070003FF) {
        // OAM - OBJ Attributes
        return memory_bus.internal_display_memory.OAM_obj_attributes[address-0x07000000];
    }
    if (0x08000000 <= address && address<=0x09FFFFFF) {
        // Game Pak ROM/FlashROM wait state 0
        return memory_bus.game_pak_memory.game_pak_ROM_slot0[address-0x08000000];
    }
    if (0x0A000000 <= address && address<=0x0BFFFFFF) {
        // Game Pak ROM/FlashROM wait state 1
        return memory_bus.game_pak_memory.game_pak_ROM_slot1[address-0x0A000000];
    }
    if (0x0C000000 <= address && address<=0x0DFFFFFF) {
        // Game Pak ROM/FlashROM wait state 2
        return memory_bus.game_pak_memory.game_pak_ROM_slot2[address-0x0C000000];
    }
    if (0x0E000000 <= address && address<=0x0E00FFFF) {
        // Game Pak SRAM
        return memory_bus.game_pak_memory.game_pak_SRAM[address-0x0E000000];
    }
    return  0;
}
void startup_sequence_memory() {
    for (int i = 0; i < 0x0E00FFFF; i++) write_to_memory_via_databus(i,0);
}