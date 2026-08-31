//
// Created by bolota on 27/08/2026.
//
#include "include/arm_opcode_handler.h"
#include "include/headers.h"
byte_type get_bits_0_to_3(word_type instrucao) {
    return (instrucao & 0b00000000000000000000000000001111) >> 0;
}
byte_type get_bits_4_to_7(word_type instrucao) {
    return (instrucao & 0b00000000000000000000000011110000) >> 4;
}
byte_type get_bits_8_to_11(word_type instrucao) {
    return (instrucao & 0b00000000000000000000111100000000) >> 8;
}
byte_type get_bits_12_to_15(word_type instrucao) {
    return (instrucao & 0b00000000000000001111000000000000) >> 12;
}
byte_type get_bits_16_to_19(word_type instrucao) {
    return (instrucao & 0b00000000000011110000000000000000) >> 16;
}
byte_type get_bits_21_to_24(word_type instrucao) {
    return (instrucao & 0b00000001111000000000000000000000) >> 21;
}
byte_type get_bits_24_to_27(word_type instrucao) {
    return (instrucao & 0b00001111000000000000000000000000) >> 25;
}
byte_type get_bits_22_to_27(word_type instrucao) {
    return (instrucao & 0b00001111110000000000000000000000) >> 23;
}
byte_type get_bits_23_to_27(word_type instrucao) {
    return (instrucao & 0b00001111100000000000000000000000) >> 23;
}
byte_type get_bits_25_to_27(word_type instrucao) {
    return (instrucao & 0b00001110000000000000000000000000) >> 25;
}
byte_type get_condition_code(word_type instrucao) {
    return (instrucao & 0b11110000000000000000000000000000) >> 28;
}

byte_type get_bits_0_to_7(word_type instrucao) {
    return instrucao & 0b00000000000000000000000011111111;
}
half_word_type get_bits_0_to_11(word_type instrucao) {
    return instrucao & 0b00000000000000000000111111111111;
}
half_word_type get_bits_7_to_11(word_type instrucao) {
    return instrucao & 0b00000000000000000000111110000000;
}
bool is_bit22_up(word_type instrucao) {
    return (instrucao & 0b10000000000000000000000)? true : false;
}
bool is_bit7_up(word_type instrucao) {
    return (instrucao & 0b10000000)? true : false;
}
bool is_bit4_up(word_type instrucao) {
    return (instrucao & 0b1000)? true : false;
}

void handle_branch_and_exchange() {
    //TODO: fazer isso funcionar 1 :)
}
void handle_halfword_data_transfer_register_offset() {
    //TODO: fazer isso funcionar 1 :)
}
void handle_halfword_data_transfer_immidiate_offset() {
    //TODO: fazer isso funcionar 1 :)
}
void handle_block_data_transfer() {
    //TODO: fazer isso funcionar 1 :)
}
void handle_branch() {
    //TODO: fazer isso funcionar 1 :)
}
void handle_software_interrupt() {
    //TODO: fazer isso funcionar 1 :)
}
void handle_multiply() {
    //TODO: fazer isso funcionar 1 :)
}
void handle_multiply_long() {
    //TODO: fazer isso funcionar 1 :)
}
// if in thumb mode, register occupies 3bit max
void arm_instruction_handler(word_type instrucao) {
    if ((instrucao & 0b0001001011111111111100010000) == 0b0001001011111111111100010000){
        handle_branch_and_exchange();
    }
    else if ((get_bits_25_to_27(instrucao) == 0) & (!is_bit22_up(instrucao)) & (get_bits_7_to_11(instrucao) == 0b00001) & is_bit4_up(instrucao)) {
        handle_halfword_data_transfer_register_offset();
    }
    else if ((get_bits_25_to_27(instrucao) == 0) & (is_bit22_up(instrucao)) & is_bit7_up(instrucao) & is_bit4_up(instrucao)) {
        handle_halfword_data_transfer_immidiate_offset();
    }
    else if (get_bits_25_to_27(instrucao) == 0b100) {
        handle_block_data_transfer();
    }
    else if (get_bits_25_to_27(instrucao) == 0b101) {
        handle_branch();
    }
    else if (get_bits_24_to_27(instrucao) == 0b1111) {
        handle_software_interrupt();
    }
    else if ((get_bits_22_to_27(instrucao) == 0) & (get_bits_4_to_7(instrucao) == 0b1001)) {
        handle_multiply();
    }
    else if ((get_bits_23_to_27(instrucao) == 0) & (get_bits_4_to_7(instrucao) == 0b1001)) {
        handle_multiply_long();
    }
}