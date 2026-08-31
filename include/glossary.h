#ifndef C_GBOYADVANCED_GLOSSARY_H
#define C_GBOYADVANCED_GLOSSARY_H
#include <stdint.h>

// GENERAL DEFINITIONS

#define byte_type uint8_t
#define half_word_type uint16_t
#define word_type uint32_t
#define bytes_nmbr 8
#define kbytes_nmbr 1024

// CPU MODES

#define USER 0
#define FIQ 1
#define SUPERVISOR 2
#define ABORT 3
#define IRQ 4
#define UNDEFINED 5

// REGISTERS DEFINITIONS

#define REG_SP 13
#define REG_LR 14
#define REG_PC 15
#define REG_CPSR 16
#define REG_SPSR 17

#endif //C_GBOYADVANCED_GLOSSARY_H
