#ifndef C_GBOYADVANCED_CPU_H
#define C_GBOYADVANCED_CPU_H
#include <stdbool.h>
#include "glossary.h"

/*
System/User FIQ       Supervisor Abort     IRQ       Undefined
  --------------------------------------------------------------
  R0          R0        R0         R0        R0        R0
  R1          R1        R1         R1        R1        R1
  R2          R2        R2         R2        R2        R2
  R3          R3        R3         R3        R3        R3
  R4          R4        R4         R4        R4        R4
  R5          R5        R5         R5        R5        R5
  R6          R6        R6         R6        R6        R6
  R7          R7        R7         R7        R7        R7
  --------------------------------------------------------------
  R8          R8_fiq    R8         R8        R8        R8
  R9          R9_fiq    R9         R9        R9        R9
  R10         R10_fiq   R10        R10       R10       R10
  R11         R11_fiq   R11        R11       R11       R11
  R12         R12_fiq   R12        R12       R12       R12
  R13 (SP)    R13_fiq   R13_svc    R13_abt   R13_irq   R13_und
  R14 (LR)    R14_fiq   R14_svc    R14_abt   R14_irq   R14_und
  R15 (PC)    R15       R15        R15       R15       R15
  --------------------------------------------------------------
  CPSR        CPSR      CPSR       CPSR      CPSR      CPSR
  --          SPSR_fiq  SPSR_svc   SPSR_abt  SPSR_irq  SPSR_und

*/
typedef struct CPU_bus {
    bool thumb_mode; //inicia falso
    byte_type curr_mode; // 0- user | 1-FIQ | 2- supervisor | 3- abort | 4- IRQ | 5- undefined
    word_type CPSR;
    word_type curr_PC;
    word_type registradores[6][18];
    word_type SPSR[6];
}CPU_bus;

void startup_sequence_cpu();
void write_to_register(word_type variable, uint8_t nbr_reg);
void write_to_register_thumb(half_word_type variable, uint8_t nbr_reg);
word_type read_register(uint8_t nbr_reg);
half_word_type read_register_thumb(uint8_t nbr_reg);

//  on -> true | off -> false
void flip_flag_N(bool on_or_off);
void flip_flag_Z(bool on_or_off);
void flip_flag_C(bool on_or_off);
void flip_flag_V(bool on_or_off);


bool is_flag_N_on();
bool is_flag_Z_on();
bool is_flag_C_on();
bool is_flag_V_on();

#endif //C_GBOYADVANCED_CPU_H
