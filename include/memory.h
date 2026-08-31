#ifndef C_GBOYADVANCED_MEMORY_H
#define C_GBOYADVANCED_MEMORY_H
#include "glossary.h"

typedef struct GeneralInternalMemory {
    word_type SystemROM [16 * kbytes_nmbr];
    word_type WorkRAM_on_board[256 * kbytes_nmbr];
    word_type WorkRAM_on_chip[32 * kbytes_nmbr];
    word_type IO_Registers[0x3FF];
}GeneralInternalMemory;

typedef struct InternalDisplayMemory {
    word_type BG_OBJ_Pallete[kbytes_nmbr];
    word_type VRAM[96 * kbytes_nmbr];
    word_type OAM_obj_attributes[kbytes_nmbr];
}InternalDisplayMemory;

typedef struct GamePakMemory {
    word_type game_pak_ROM_slot0[0x09FFFFFF - 0x08000000];
    word_type game_pak_ROM_slot1[0x09FFFFFF - 0x08000000];
    word_type game_pak_ROM_slot2[0x09FFFFFF - 0x08000000];
    word_type game_pak_SRAM[0x10000];
}GamePakMemory;

/*

General Internal Memory

  00000000-00003FFF   BIOS - System ROM         (16 KBytes)
  00004000-01FFFFFF   Not used
  02000000-0203FFFF   WRAM - On-board Work RAM  (256 KBytes) 2 Wait
  02040000-02FFFFFF   Not used
  03000000-03007FFF   WRAM - On-chip Work RAM   (32 KBytes)
  03008000-03FFFFFF   Not used
  04000000-040003FE   I/O Registers
  04000400-04FFFFFF   Not used

Internal Display Memory

  05000000-050003FF   BG/OBJ Palette RAM        (1 Kbyte)
  05000400-05FFFFFF   Not used
  06000000-06017FFF   VRAM - Video RAM          (96 KBytes)
  06018000-06FFFFFF   Not used
  07000000-070003FF   OAM - OBJ Attributes      (1 Kbyte)
  07000400-07FFFFFF   Not used

External Memory (Game Pak)

  08000000-09FFFFFF   Game Pak ROM/FlashROM (max 32MB) - Wait State 0
  0A000000-0BFFFFFF   Game Pak ROM/FlashROM (max 32MB) - Wait State 1
  0C000000-0DFFFFFF   Game Pak ROM/FlashROM (max 32MB) - Wait State 2
  0E000000-0E00FFFF   Game Pak SRAM    (max 64 KBytes) - 8bit Bus width
  0E010000-0FFFFFFF   Not used

Unused Memory Area
  10000000-FFFFFFFF   Not used (upper 4bits of address bus unused)

*/
typedef struct MemoryGroup{
    GeneralInternalMemory general_internal_memory;
    InternalDisplayMemory internal_display_memory;
    GamePakMemory game_pak_memory;
} MemoryGroup;

void write_to_memory_via_databus(word_type address, word_type value);
word_type read_from_memory_via_databus(word_type address);
void startup_sequence_memory();
#endif //C_GBOYADVANCED_MEMORY_H