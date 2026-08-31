#include "include/core_cpu.h"

CPU_bus cpu;

void startup_sequence() {
    cpu.curr_mode=0;
    cpu.CPSR = 0;
    cpu.curr_PC = 0;
    cpu.thumb_mode = false;
    for (int i = 0; i< 6;i++) {
        cpu.SPSR[i] = 0;
        for (int j = 0; j<17; j++) {
            cpu.registradores[i][j] = 0;
        }
    }
}
void write_to_register(word_type variable, uint8_t nbr_reg) {
    cpu.registradores[cpu.curr_mode][nbr_reg] = variable;
}
word_type read_register(uint8_t nbr_reg) {
    return cpu.registradores[cpu.curr_mode][nbr_reg];
}
void write_to_register_thumb(half_word_type variable, uint8_t nbr_reg) {
    cpu.registradores[cpu.curr_mode][nbr_reg] = variable;
}
half_word_type read_register_thumb(uint8_t nbr_reg) {
    return (half_word_type)cpu.registradores[cpu.curr_mode][nbr_reg];
}

void transfer_registers_on_mode_change(uint8_t new_selected_mode) {
    for (int i = 0; i < 8; i++) {
        cpu.registradores[new_selected_mode][i] = cpu.registradores[cpu.curr_mode][i] ;
    }
    if (cpu.curr_mode != FIQ && new_selected_mode != FIQ) {
        for (int i = 8; i < 13; i++) {
            cpu.registradores[new_selected_mode][i] = cpu.registradores[cpu.curr_mode][i] ;
        }
    }
    if (cpu.curr_mode != USER) {
        cpu.registradores[new_selected_mode][REG_CPSR] = cpu.registradores[cpu.curr_mode][REG_SPSR];
    }
    else {
        cpu.registradores[new_selected_mode][REG_SPSR] = cpu.registradores[new_selected_mode][REG_CPSR];
    }
}

void flip_flag_N(bool on_or_off) {
    //0x      ff       ff        ff       ff
    //0b   10000000  00000000  00000000  00000000
    word_type temp = cpu.CPSR;
    cpu.CPSR = (on_or_off)? temp | 0b10000000000000000000000000000000 : temp & 0b01111111111111111111111111111111;
}
void flip_flag_Z(bool on_or_off) {
    word_type temp = cpu.CPSR;
    cpu.CPSR = (on_or_off)? temp | 0b01000000000000000000000000000000 : temp & 0b10111111111111111111111111111111;
}
void flip_flag_C(bool on_or_off) {
    word_type temp = cpu.CPSR;
    cpu.CPSR = (on_or_off)? temp | 0b00100000000000000000000000000000 : temp & 0b00011111111111111111111111111111;
}
void flip_flag_V(bool on_or_off) {
    word_type temp = cpu.CPSR;
    cpu.CPSR = (on_or_off)? temp | 0b00010000000000000000000000000000 : temp & 0b11101111111111111111111111111111;
}

bool is_flag_N_on() {
    return (cpu.CPSR & 4194304)? true : false;
}
bool is_flag_Z_on() {
    return (cpu.CPSR & 2097152)? true : false;
}
bool is_flag_C_on() {
    return (cpu.CPSR & 1048576)? true : false;
}
bool is_flag_V_on() {
    return (cpu.CPSR & 524288)? true : false;
}
