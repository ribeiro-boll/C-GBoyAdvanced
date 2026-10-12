// byte_type get_bits_0_to_3(word_type instrucao) {
//
// Created by bolota on 27/08/2026.
//
#include "include/arm_opcode_handler.h"
#include "include/headers.h"

double_word_type cflag_verifier_left = 0x100000000;
double_word_type cflag_verifier_right = 0xfffffffff;


bool is_bit4_up(word_type instrucao) {
    return (instrucao & 0b00000000000000000000000000010000)? true : false;
}

bool is_bit7_up(word_type instrucao) {
    return (instrucao & 0b00000000000000000000000010000000)? true : false;
}

// STR & LDR: true = LDR | false = STR
bool is_bit_20_up(word_type instrucao) {
    return (instrucao & 0b00000000000100000000000000000000)? true : false;
}
// STR & LDR: true = write new address to base | false = no write back
bool is_bit_21_up(word_type instrucao) {
    return (instrucao & 0b00000000001000000000000000000000)? true : false;
}
bool is_bit_22_up(word_type instrucao) {
    return (instrucao & 0b00000000010000000000000000000000)? true : false;
}
// STR & LDR: true = add offset to base | false = subtract offset from base
bool is_bit_23_up(word_type instrucao) {
    return (instrucao & 0b00000000100000000000000000000000)? true : false;
}
// STR & LDR: true = add offset to base | false = subtract offset from base
bool is_bit_24_up(word_type instrucao) {
        return (instrucao & 0b00000001000000000000000000000000)? true : false;
}
bool is_bit_25_up(word_type instrucao) {
    return (instrucao & 0b00000010000000000000000000000000)? true : false;
}
byte_type get_cond_value(word_type instrucao) {
    return (instrucao & 0b11110000000000000000000000000000) >> 28;
}



byte_type get_DataProcessing_operand2(word_type instrucao) {
    return (instrucao & 0b00000000000000000000111111111111);
}
byte_type get_DataProcessing_2d_operand_register(word_type instrucao) {
    return (instrucao & 0b00000000000000000000000000001111);
}
byte_type get_DataProcessing_2d_operand_register_shift(word_type instrucao) {
    return (instrucao & 0b00000000000000000000111111110000) >> 4;
}
byte_type get_DataProcessing_2d_operand_register_shift_immediate_value(word_type instrucao) {
    return (instrucao & 0b00000000000000000000111110000000) >> 7;
}
byte_type get_DataProcessing_2d_operand_register_shift_register_value(word_type instrucao) {
    return (instrucao & 0b00000000000000000000111100000000) >> 8;
}

byte_type get_DataProcessing_2d_operand_immediate(word_type instrucao) {
    return (instrucao & 0b00000000000000000000000011111111);
}
byte_type get_DataProcessing_2d_operand_immediate_rotate_value(word_type instrucao) {
    return (instrucao & 0b00000000000000000000111100000000) >> 8;
}

byte_type get_DataProcessing_2d_operand_shift_type(word_type instrucao){
    return (instrucao & 0b00000000000000000000000001100000)>>5;
}

byte_type get_DataProcessing_RD(word_type instrucao) {
    return (instrucao & 0b00000000000000001111000000000000) >> 12;
}
byte_type get_DataProcessing_RN(word_type instrucao) {
    return (instrucao & 0b00000000000011110000000000000000) >> 16;
}
byte_type get_DataProcessing_opcode(word_type instrucao) {
    return (instrucao & 0b00000001111000000000000000000000) >> 21;
}

/*

0000 EQ Z set equal
0001 NE Z clear not equal
0010 CS C set unsigned higher or same
0011 CC C clear unsigned lower
0100 MI N set negative
0101 PL N clear positive or zero
0110 VS V set overflow
0111 VC V clear no overflow
1000 HI C set and Z clear unsigned higher
1001 LS C clear or Z set unsigned lower or same
1010 GE N equals V greater or equal
1011 LT N not equal to V less than
1100 GT Z clear AND (N equals V) greater than
1101 LE Z set OR (N not equal to V) less than or equal
1110 AL (ignored) always

*/
// se a condição for verdade, manda um true, se não false
bool instruction_condition_verifier(word_type instrucao) {
    switch (get_cond_value(instrucao)) {
        case 0b0000: //EQ Z set equal
            return (is_flag_Z_on());
        case 0b0001: //NE Z clear not equal
            return (!is_flag_Z_on());
        case 0b0010: //CS C set unsigned higher or same
            return (is_flag_C_on());
        case 0b0011: //CC C clear unsigned lower
            return (!is_flag_C_on());
        case 0b0100: //MI N set negative
            return (is_flag_N_on());
        case 0b0101: //PL N clear positive or zero
            return (!is_flag_N_on());
        case 0b0110: //VS V set overflow
            return (is_flag_V_on());
        case 0b0111: //VC V clear no overflow
            return (!is_flag_V_on());
        case 0b1000: //HI C set and Z clear unsigned higher
            return (is_flag_C_on() == true && is_flag_Z_on() == false)? true : false;
        case 0b1001: //LS C clear or Z set unsigned lower or same
            return (is_flag_C_on() == false || is_flag_Z_on() == true)? true : false;
        case 0b1010: //GE N equals V greater or equal
            return (is_flag_N_on() == is_flag_V_on())? true : false;
        case 0b1011: //LT N not equal to V less than
            return (is_flag_N_on() != is_flag_V_on())? true : false;
        case 0b1100: //GT Z clear AND (N equals V) greater than
            return (!is_flag_Z_on() && (is_flag_N_on()==is_flag_V_on()))? true : false;
        case 0b1101: //LE Z set OR (N not equal to V) less than or equal
            return (is_flag_Z_on() || (is_flag_N_on()!=is_flag_V_on()))? true : false;
        case 0b1110: //AL (ignored) always
            return true;
        default:
            return false;
    }
}


word_type handle_sencond_opperand_register(word_type instrucao) {
    byte_type reg_destination = get_DataProcessing_RD(instrucao), first_operand_register = get_DataProcessing_RN(instrucao);
    byte_type second_operand_register_shift, second_operand_immediate_rotate, rotate_value;
    byte_type second_register_shift_type = get_DataProcessing_2d_operand_shift_type(instrucao);
    word_type second_operand;
    bool is_bigger_than_31;
    bool is_register = false;
    byte_type second_operand_shift_ammount;
    word_type second_operand_after_shift;
    second_operand = read_register(get_DataProcessing_2d_operand_register(instrucao));
    if (is_bit4_up(instrucao) && !is_bit7_up(instrucao)) {
        second_operand_shift_ammount = read_register(get_DataProcessing_2d_operand_register_shift_register_value(instrucao));
        is_register = true;
    }
    else {
        second_operand_shift_ammount = get_DataProcessing_2d_operand_register_shift_immediate_value(instrucao);
    }
    switch (second_register_shift_type) {
        case 0b00:
            is_bigger_than_31 = (second_operand_shift_ammount>31);
            second_operand_after_shift = second_operand;
            if (is_bigger_than_31) {
                second_operand_after_shift = 0;
                // if (is_bit_20_up(instrucao))
                //     ((second_operand_shift_ammount == 32) && (second_operand & 1) )? flip_flag_C(true) : flip_flag_C(false);
            }
            else {
                __asm("SHL %b[input], %k[result]"
                    : [result] "+r" (second_operand_after_shift)
                    : [input] "c" (second_operand_shift_ammount));
                // if (is_bit_20_up(instrucao))
                //     if (second_operand_shift_ammount != 0 && second_operand_shift_ammount!=0)(second_operand  & (0x80000000 >> (second_operand_shift_ammount - 1)))? flip_flag_C(true) : flip_flag_C(false);

            }
            break;
            //printf("teste: 0x%x\n", second_operand_after_shift);

            // 0b00010 & 100000000
            // LEVANTA FLAG C APENAS SE TIVER O SUFIXO 'S'
            //logical left
        case 0b01:
            second_operand_after_shift = second_operand;
            //printf("numero antes: %x | nmr_shift: %d | ",second_operand_after_shift, second_operand_shift_ammount);
            if (second_operand_shift_ammount == 0 && !is_register) {
                // if (is_bit_20_up(instrucao)) {
                //     (second_operand&0x80000000)? flip_flag_C(true) : flip_flag_C(false);
                // }
                second_operand_after_shift = 0;
            }
            else {
                is_bigger_than_31 = (second_operand_shift_ammount>31);
                if (is_bigger_than_31) {
                    second_operand_after_shift = 0;
                    // if (is_bit_20_up(instrucao))
                    //     ((second_operand_shift_ammount == 32) && (second_operand & 0x80000000) )? flip_flag_C(true) : flip_flag_C(false);
                }
                else {
                    __asm("SHR %b[inputi], %k[result]"
                    : [result] "+r" (second_operand_after_shift)
                    : [inputi] "c" (second_operand_shift_ammount));
                    //printf("numero dps %x\n",second_operand_after_shift);
                    // if (is_bit_20_up(instrucao)) {
                    //     if (second_operand_shift_ammount != 0)(second_operand  & (1 << (second_operand_shift_ammount - 1)))? flip_flag_C(true) : flip_flag_C(false);
                    // }
                }
            }
            break;
            //logical right
        case 0b10:
            second_operand_after_shift = second_operand;
            if (second_operand_shift_ammount == 0 && !is_register) {
                // if (is_bit_20_up(instrucao)) {
                //     (second_operand&0x80000000)? flip_flag_C(true) : flip_flag_C(false);
                // }
                second_operand_after_shift = (second_operand&0x80000000)? 0xFFFFFFFF : 0;
            }
            else {
                is_bigger_than_31 = (second_operand_shift_ammount>31);
                if (is_bigger_than_31) {
                    second_operand_after_shift = (second_operand&0x80000000)? 0xFFFFFFFF : 0;
                    // if (is_bit_20_up(instrucao))
                    //     (((second_operand_shift_ammount == 32) && (second_operand & 0x80000000) || second_operand_after_shift == 0xFFFFFFFF))? flip_flag_C(true) : flip_flag_C(false);
                }
                else {
                    __asm ("SAR %b[inputii], %k[result]"
                    : [result] "+r" (second_operand_after_shift)
                    : [inputii] "c" (second_operand_shift_ammount));
                    // if (is_bit_20_up(instrucao)) {
                    //     if (second_operand_shift_ammount != 0)(second_operand  & (1 << (second_operand_shift_ammount - 1)))? flip_flag_C(true) : flip_flag_C(false);
                    // }
                }
            }
            break;
            //arithmetic right
        case 0b11:
            second_operand_after_shift = second_operand;
            if (second_operand_shift_ammount == 0 && !is_register) {
                second_operand_after_shift >>= 1;
                if (is_flag_C_on()) {
                     second_operand_after_shift |= 0x80000000;
                //     //if (is_bit_20_up(instrucao))flip_flag_C(false);
                }
               // if ((second_operand & 1) && is_bit_20_up(instrucao)) flip_flag_C(true);
            }
            else {
                is_bigger_than_31 = (second_operand_shift_ammount>31);
                __asm ("ROR %b[inputiii], %k[result]"
                : [result] "+r" (second_operand_after_shift)
                : [inputiii] "c" (second_operand_shift_ammount));
                    // if (is_bit_20_up(instrucao)) {
                    //     if (second_operand_shift_ammount!= 0) (second_operand  & (1 << (second_operand_shift_ammount - 1)))? flip_flag_C(true) : flip_flag_C(false);
                    // }
            }
            break;
            //rotate
    }
    // if (is_bit_20_up(instrucao)) {
    //     (second_operand_after_shift == 0)? flip_flag_Z(true) : flip_flag_Z(false);
    //     (second_operand_after_shift & 0x80000000)? flip_flag_N(true) : flip_flag_N(false);
    // }
    return second_operand_after_shift;
}

word_type handle_sencond_opperand(word_type instrucao) {
    byte_type reg_destination = get_DataProcessing_RD(instrucao), first_operand_register = get_DataProcessing_RN(instrucao);
    byte_type second_operand_register_shift, second_operand_immediate_rotate, rotate_value;
    byte_type second_register_shift_type = get_DataProcessing_2d_operand_shift_type(instrucao);
    word_type second_operand;
    bool is_bigger_than_31;
    bool is_register = false;
    byte_type second_operand_shift_ammount;
    word_type second_operand_after_shift;
    if (is_bit_25_up(instrucao)) {
        second_operand = get_DataProcessing_2d_operand_immediate(instrucao);
        rotate_value = get_DataProcessing_2d_operand_immediate_rotate_value(instrucao);
        word_type second_operand_after_shift_immediate = second_operand;
        __asm ("ROR %b[inputiiii], %k[result]"
            : [result] "+r" (second_operand_after_shift_immediate)
            : [inputiiii] "c" (rotate_value*2));

        if (is_bit_20_up(instrucao)) {
            if (rotate_value!=0)(second_operand  & (1 << (rotate_value*2 - 1)))? flip_flag_C(true) : flip_flag_C(false);
            (second_operand_after_shift_immediate == 0)? flip_flag_Z(true) : flip_flag_Z(false);
            (second_operand_after_shift_immediate & 0x80000000)? flip_flag_N(true): flip_flag_N(false);
        }
        return second_operand_after_shift_immediate;
    }
    else {
        second_operand = read_register(get_DataProcessing_2d_operand_register(instrucao));
        if (is_bit4_up(instrucao) && !is_bit7_up(instrucao)) {
            second_operand_shift_ammount = read_register(get_DataProcessing_2d_operand_register_shift_register_value(instrucao));
            is_register = true;
        }
        else {
            second_operand_shift_ammount = get_DataProcessing_2d_operand_register_shift_immediate_value(instrucao);
        }
        switch (second_register_shift_type) {
            case 0b00:
                is_bigger_than_31 = (second_operand_shift_ammount>31);
                second_operand_after_shift = second_operand;
                if (is_bigger_than_31) {
                    second_operand_after_shift = 0;
                    if (is_bit_20_up(instrucao))
                        ((second_operand_shift_ammount == 32) && (second_operand & 1) )? flip_flag_C(true) : flip_flag_C(false);
                }
                else {
                    __asm("SHL %b[input], %k[result]"
                        : [result] "+r" (second_operand_after_shift)
                        : [input] "c" (second_operand_shift_ammount));
                    if (is_bit_20_up(instrucao))
                        if (second_operand_shift_ammount != 0 && second_operand_shift_ammount!=0)(second_operand  & (0x80000000 >> (second_operand_shift_ammount - 1)))? flip_flag_C(true) : flip_flag_C(false);

                }
                break;
                //printf("teste: 0x%x\n", second_operand_after_shift);

                // 0b00010 & 100000000
                // LEVANTA FLAG C APENAS SE TIVER O SUFIXO 'S'
                //logical left
            case 0b01:
                second_operand_after_shift = second_operand;
                //printf("numero antes: %x | nmr_shift: %d | ",second_operand_after_shift, second_operand_shift_ammount);
                if (second_operand_shift_ammount == 0 && !is_register) {
                    if (is_bit_20_up(instrucao)) {
                        (second_operand&0x80000000)? flip_flag_C(true) : flip_flag_C(false);
                    }
                    second_operand_after_shift = 0;
                }
                else {
                    is_bigger_than_31 = (second_operand_shift_ammount>31);
                    if (is_bigger_than_31) {
                        second_operand_after_shift = 0;
                        if (is_bit_20_up(instrucao))
                            ((second_operand_shift_ammount == 32) && (second_operand & 0x80000000) )? flip_flag_C(true) : flip_flag_C(false);
                    }
                    else {
                        __asm("SHR %b[inputi], %k[result]"
                        : [result] "+r" (second_operand_after_shift)
                        : [inputi] "c" (second_operand_shift_ammount));
                        //printf("numero dps %x\n",second_operand_after_shift);
                        if (is_bit_20_up(instrucao)) {
                            if (second_operand_shift_ammount != 0)(second_operand  & (1 << (second_operand_shift_ammount - 1)))? flip_flag_C(true) : flip_flag_C(false);
                        }
                    }
                }
                break;
                //logical right
            case 0b10:
                second_operand_after_shift = second_operand;
                if (second_operand_shift_ammount == 0 && !is_register) {
                    if (is_bit_20_up(instrucao)) {
                        (second_operand&0x80000000)? flip_flag_C(true) : flip_flag_C(false);
                    }
                    second_operand_after_shift = (second_operand&0x80000000)? 0xFFFFFFFF : 0;
                }
                else {
                    is_bigger_than_31 = (second_operand_shift_ammount>31);
                    if (is_bigger_than_31) {
                        second_operand_after_shift = (second_operand&0x80000000)? 0xFFFFFFFF : 0;
                        if (is_bit_20_up(instrucao))
                            (((second_operand_shift_ammount == 32) && (second_operand & 0x80000000) || second_operand_after_shift == 0xFFFFFFFF))? flip_flag_C(true) : flip_flag_C(false);
                    }
                    else {
                        __asm ("SAR %b[inputii], %k[result]"
                        : [result] "+r" (second_operand_after_shift)
                        : [inputii] "c" (second_operand_shift_ammount));
                        if (is_bit_20_up(instrucao)) {
                            if (second_operand_shift_ammount != 0)(second_operand  & (1 << (second_operand_shift_ammount - 1)))? flip_flag_C(true) : flip_flag_C(false);
                        }
                    }
                }
                break;
                //arithmetic right
            case 0b11:
                second_operand_after_shift = second_operand;
                if (second_operand_shift_ammount == 0 && !is_register) {
                    second_operand_after_shift >>= 1;
                    if (is_flag_C_on()) {
                        second_operand_after_shift |= 0x80000000;
                        if (is_bit_20_up(instrucao))flip_flag_C(false);
                    }
                    if ((second_operand & 1) && is_bit_20_up(instrucao)) flip_flag_C(true);
                }
                else {
                    is_bigger_than_31 = (second_operand_shift_ammount>31);
                    __asm ("ROR %b[inputiii], %k[result]"
                    : [result] "+r" (second_operand_after_shift)
                    : [inputiii] "c" (second_operand_shift_ammount));
                    if (is_bit_20_up(instrucao)) {
                        if (second_operand_shift_ammount!= 0) (second_operand  & (1 << (second_operand_shift_ammount - 1)))? flip_flag_C(true) : flip_flag_C(false);
                    }
                }
                break;
                //rotate
        }
    }
    if (is_bit_20_up(instrucao)) {
        (second_operand_after_shift == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        (second_operand_after_shift & 0x80000000)? flip_flag_N(true) : flip_flag_N(false);
    }
    return second_operand_after_shift;
}

byte_type handle_AND(word_type instrucao) {
    word_type RN = read_register(get_DataProcessing_RN(instrucao)) & handle_sencond_opperand(instrucao), RD = get_DataProcessing_RD(instrucao);
    if (is_bit_20_up(instrucao)) {
        (RN == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        (RN & 0x80000000)? flip_flag_N(true) : flip_flag_N(false);
        if(RD == REG_PC) write_CPSR_to_SPSR();
    }
    write_to_register(RD, RN);
}

byte_type handle_EOR(word_type instrucao) {
    word_type RN = read_register(get_DataProcessing_RN(instrucao)) ^ (handle_sencond_opperand(instrucao));
    if (is_bit_20_up(instrucao)) {
        (RN == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        (RN & 0x80000000)? flip_flag_N(true) : flip_flag_N(false);
    }
    write_to_register(get_DataProcessing_RD(instrucao), RN);
}

byte_type handle_SUB(word_type instrucao) {
    word_type RN = read_register(get_DataProcessing_RN(instrucao)), sec_operand = (handle_sencond_opperand(instrucao));
    word_type RN_signed, sec_operand_signed,return_value_signed;
    if (is_bit_20_up(instrucao)) {
        RN_signed = (signed_word_type) RN;
        sec_operand_signed = (signed_word_type) sec_operand;
        return_value_signed = RN_signed - sec_operand_signed;
        ((sec_operand_signed & 0x80000000) != (RN_signed & 0x80000000)) & ((return_value_signed & 0x80000000)!=(RN_signed & 0x80000000))? flip_flag_V(true) : flip_flag_V(false);
        (return_value_signed == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        ((return_value_signed & 0x80000000))? flip_flag_N(true) : flip_flag_N(false);
        (RN_signed>=sec_operand_signed)? flip_flag_C(true) : flip_flag_C(false);
        write_to_register(get_DataProcessing_RD(instrucao), return_value_signed);
        return 0;
    }
    write_to_register(get_DataProcessing_RD(instrucao), RN - sec_operand);
}

byte_type handle_RSB(word_type instrucao) {

    word_type RN = read_register(get_DataProcessing_RN(instrucao)), sec_operand = (handle_sencond_opperand(instrucao));
    word_type RN_signed, sec_operand_signed,return_value_signed;

    if (is_bit_20_up(instrucao)) {
        RN_signed = (signed_word_type) RN;
        sec_operand_signed = (signed_word_type) sec_operand;
        return_value_signed =  sec_operand_signed - RN_signed;
        ((sec_operand_signed & 0x80000000) != (RN_signed & 0x80000000)) & ((return_value_signed & 0x80000000)!=(sec_operand_signed & 0x80000000))? flip_flag_V(true) : flip_flag_V(false);
        (return_value_signed == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        ((return_value_signed & 0x80000000))? flip_flag_N(true) : flip_flag_N(false);
        ((RN_signed)<sec_operand_signed)? flip_flag_C(true) : flip_flag_C(false);
        write_to_register(get_DataProcessing_RD(instrucao), return_value_signed);
        return 0;
    }
    write_to_register(get_DataProcessing_RD(instrucao), sec_operand - RN);
}

byte_type handle_ADD(word_type instrucao) {
    word_type RN = read_register(get_DataProcessing_RN(instrucao)), sec_operand = (handle_sencond_opperand(instrucao));
    word_type RN_signed, sec_operand_signed,return_value_signed;
    double_word_type tst = RN + sec_operand;
    //printf("tst: 0x%lx", tst);
    if (is_bit_20_up(instrucao)) {
        RN_signed = RN;
        sec_operand_signed = sec_operand;
        return_value_signed =  sec_operand_signed + RN_signed;
        (((sec_operand_signed & 0x80000000) != (return_value_signed & 0x80000000)) && ((RN_signed & 0x80000000) != (return_value_signed & 0x80000000)))? flip_flag_V(true) : flip_flag_V(false);
        (return_value_signed == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        (return_value_signed & 0x80000000)? flip_flag_N(true) : flip_flag_N(false);
        (((uint64_t)RN_signed + (uint64_t)sec_operand_signed) > 0xFFFFFFFF)? flip_flag_C(true) : flip_flag_C(false);
        write_to_register(get_DataProcessing_RD(instrucao), return_value_signed);
        return 0;
    }
    write_to_register(get_DataProcessing_RD(instrucao), sec_operand + RN);
}

byte_type handle_ADC(word_type instrucao) {
    bool old_Cflag = is_flag_C_on();
    word_type RN = read_register(get_DataProcessing_RN(instrucao)), sec_operand = (handle_sencond_opperand(instrucao));
    word_type RN_signed, sec_operand_signed,return_value_signed;
    if (is_bit_20_up(instrucao)) {
        RN_signed = RN;
        sec_operand_signed = sec_operand;
        return_value_signed =  sec_operand_signed + RN_signed + old_Cflag;
        (((sec_operand_signed & 0x80000000) != (return_value_signed & 0x80000000)) && ((RN_signed & 0x80000000) != (return_value_signed & 0x80000000)))? flip_flag_V(true) : flip_flag_V(false);
        (return_value_signed == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        ((return_value_signed & 0x80000000))? flip_flag_N(true) : flip_flag_N(false);
        (((int64_t)RN_signed + sec_operand_signed + old_Cflag) > 0xFFFFFFFF)? flip_flag_C(true) : flip_flag_C(false);
        write_to_register(get_DataProcessing_RD(instrucao), return_value_signed);
        return 0;
    }
    write_to_register(get_DataProcessing_RD(instrucao), sec_operand + RN + old_Cflag);
}

byte_type handle_SBC(word_type instrucao) {
    bool old_Cflag = is_flag_C_on();
    word_type RN = read_register(get_DataProcessing_RN(instrucao)), sec_operand = (handle_sencond_opperand(instrucao));
    word_type RN_signed, sec_operand_signed,return_value_signed;
    if (is_bit_20_up(instrucao)) {
        RN_signed = (signed_word_type) RN;
        sec_operand_signed = (signed_word_type) sec_operand;
        return_value_signed = RN_signed - sec_operand_signed - !old_Cflag;
        ((sec_operand_signed & 0x80000000) != (RN_signed & 0x80000000)) & ((return_value_signed & 0x80000000)!=(RN_signed & 0x80000000))? flip_flag_V(true) : flip_flag_V(false);
        (return_value_signed == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        (return_value_signed & 0x80000000)? flip_flag_N(true) : flip_flag_N(false);
        (((old_Cflag) & (RN_signed >= sec_operand_signed)) | ((!old_Cflag) & (RN_signed > sec_operand_signed)))? flip_flag_C(true) : flip_flag_C(false);
        write_to_register(get_DataProcessing_RD(instrucao), return_value_signed);
        return 0;
    }
    write_to_register(get_DataProcessing_RD(instrucao), RN - sec_operand + (old_Cflag - 1));
}

byte_type handle_RSC(word_type instrucao) {
    bool old_Cflag = is_flag_C_on();
    word_type RN = read_register(get_DataProcessing_RN(instrucao)), sec_operand = (handle_sencond_opperand(instrucao));
    word_type RN_signed, sec_operand_signed,return_value_signed;
    if (is_bit_20_up(instrucao)) {
        RN_signed = (signed_word_type) RN;
        sec_operand_signed = (signed_word_type) sec_operand;
        return_value_signed = sec_operand_signed - RN_signed + (old_Cflag - 1);
        ((sec_operand_signed & 0x80000000) != (RN_signed & 0x80000000)) & ((return_value_signed & 0x80000000)!=(sec_operand_signed & 0x80000000))? flip_flag_V(true) : flip_flag_V(false);
        ((return_value_signed & 0x80000000))? flip_flag_N(true) : flip_flag_N(false);
        (return_value_signed == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        (((old_Cflag) & (RN_signed <= sec_operand_signed)) | ((!old_Cflag) & (RN_signed < sec_operand_signed)) )? flip_flag_C(true) : flip_flag_C(false);
        write_to_register(get_DataProcessing_RD(instrucao), return_value_signed);
        return 0;
    }
    write_to_register(get_DataProcessing_RD(instrucao), sec_operand - RN + (old_Cflag - 1));
}

byte_type handle_TST(word_type instrucao) {
    word_type RN = read_register(get_DataProcessing_RN(instrucao)) & handle_sencond_opperand(instrucao);
    if (is_bit_20_up(instrucao)) {
        (RN == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        (RN & 0x80000000)? flip_flag_N(true) : flip_flag_N(false);
    }
}

byte_type handle_TEQ(word_type instrucao) {
    word_type RN = read_register(get_DataProcessing_RN(instrucao)) ^ (handle_sencond_opperand(instrucao));
    if (is_bit_20_up(instrucao)) {
        (RN == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        (RN & 0x80000000)? flip_flag_N(true) : flip_flag_N(false);
    }
}

byte_type handle_CMP(word_type instrucao) {
    //bool old_Cflag = is_flag_C_on();
    word_type RN = read_register(get_DataProcessing_RN(instrucao)), sec_operand = (handle_sencond_opperand(instrucao));
    word_type RN_signed, sec_operand_signed,return_value_signed;
    if (is_bit_20_up(instrucao)) {
        RN_signed = (signed_word_type) RN;
        sec_operand_signed = (signed_word_type) sec_operand;
        return_value_signed = RN_signed - sec_operand_signed;
        ((sec_operand_signed & 0x80000000) != (RN_signed & 0x80000000)) & ((return_value_signed & 0x80000000)!=(RN_signed & 0x80000000))? flip_flag_V(true) : flip_flag_V(false);
        (return_value_signed == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        (return_value_signed & 0x80000000)? flip_flag_N(true) : flip_flag_N(false);
        (RN_signed>=sec_operand_signed)? flip_flag_C(true) : flip_flag_C(false);
    }
}

byte_type handle_CMN(word_type instrucao) {
    word_type RN = read_register(get_DataProcessing_RN(instrucao)), sec_operand = (handle_sencond_opperand(instrucao));
    word_type RN_signed, sec_operand_signed,return_value_signed;
    double_word_type tst = RN + sec_operand;
    if (is_bit_20_up(instrucao)) {
        RN_signed = RN;
        sec_operand_signed = sec_operand;
        return_value_signed =  sec_operand_signed + RN_signed;
        (((sec_operand_signed & 0x80000000) != (return_value_signed & 0x80000000)) && ((RN_signed & 0x80000000) != (return_value_signed & 0x80000000)))? flip_flag_V(true) : flip_flag_V(false);
        (return_value_signed == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        (return_value_signed & 0x80000000)? flip_flag_N(true) : flip_flag_N(false);
        (((uint64_t)RN_signed + (uint64_t)sec_operand_signed) > 0xFFFFFFFF)? flip_flag_C(true) : flip_flag_C(false);
    }
}

byte_type handle_ORR(word_type instrucao) {
    //printf("ORR\n");
    //write_to_register();
    word_type RN = read_register(get_DataProcessing_RN(instrucao));
    RN |= (handle_sencond_opperand(instrucao));
    if (is_bit_20_up(instrucao)) {
        (RN == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        (RN & 0x80000000)? flip_flag_N(true) : flip_flag_N(false);
    }
    write_to_register(get_DataProcessing_RD(instrucao), RN);

}

byte_type handle_MOV(word_type instrucao) {
    write_to_register(get_DataProcessing_RD(instrucao), handle_sencond_opperand(instrucao));
    //printf("opcode passou por aqui: 0x%x\n\n\n", instrucao);
}
byte_type handle_BIC(word_type instrucao) {
    word_type RN = read_register((get_DataProcessing_RN(instrucao)));
    RN &= (~handle_sencond_opperand(instrucao));
    if (is_bit_20_up(instrucao)) {
        (RN == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        (RN & 0x80000000)? flip_flag_N(true) : flip_flag_N(false);
    }
    write_to_register(get_DataProcessing_RD(instrucao), RN);
    //printf("BIC\n");
}

byte_type handle_MVN(word_type instrucao) {
    //printf("MVN\n");
    word_type value =  ~handle_sencond_opperand(instrucao);
    if (is_bit_20_up(instrucao)) {
        (value == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        (value & 0x80000000)? flip_flag_N(true) : flip_flag_N(false);
    }
    write_to_register(get_DataProcessing_RD(instrucao), value);
}

// other encodings
byte_type handle_PSR_Immediate(word_type instrucao) {
    printf("PSR Immediate\n");
}

byte_type handle_PSR_Register(word_type instrucao) {
    printf("PSR Register\n");
}

byte_type handle_BranchEx(word_type instrucao) {
    printf("Branch Exchange\n");
}

byte_type handle_Data_Transfer_Unsigned_Halfword_Register(word_type instrucao) {
    printf("Data Transfer Unsigned Halfword Register\n");
}

byte_type handle_Data_Transfer_Signed_Byte_Register(word_type instrucao) {
    printf("Data Transfer Signed Byte Register\n");
}

byte_type handle_Data_Transfer_Signed_Halfword_Register(word_type instrucao) {
    printf("Data Transfer Signed Halfword Register\n");
}

byte_type handle_Load_Word_Register(word_type instrucao) {
    printf("Load Word Register\n");
}

byte_type handle_Load_Byte_Register(word_type instrucao) {
    printf("Load Byte Register\n");
}

byte_type handle_Load_Word_Immediate(word_type instrucao) {
    printf("Load Word Immediate\n");
}

byte_type handle_Load_Byte_Immediate(word_type instrucao) {
    printf("Load Byte Immediate\n");
}

byte_type handle_Immediate_Unsigned_Halfword_Data_Transfer(word_type instrucao) {
    printf("Immediate Unsigned Halfword Data Transfer\n");
}

byte_type handle_Immediate_Signed_Byte_Data_Transfer(word_type instrucao) {
    printf("Immediate Signed Byte Data Transfer\n");
}

byte_type handle_Immediate_Signed_Halfword_Data_Transfer(word_type instrucao) {
    printf("Immediate Signed Halfword Data Transfer\n");
}

byte_type handle_Swap_Byte(word_type instrucao) {
    printf("Swap Byte\n");
}

byte_type handle_Swap_Word(word_type instrucao) {
    printf("Swap Word\n");
}

byte_type handle_Undefined(word_type instrucao) {
    printf("Undefined\n");
}

byte_type handle_Block_Transfer(word_type instrucao) {
    printf("Block Transfer\n");
}

byte_type handle_Branch_Link(word_type instrucao) {
    printf("Branch Link\n");
}

byte_type handle_Sys_Interrupt(word_type instrucao) {
    printf("System Interrupt\n");
}
/*

0000 = AND - Rd:= Op1 AND Op2
0001 = EOR - Rd:= Op1 EOR Op2
0010 = SUB - Rd:= Op1 - Op2
0011 = RSB - Rd:= Op2 - Op1
0100 = ADD - Rd:= Op1 + Op2
0101 = ADC - Rd:= Op1 + Op2 + C
0110 = SBC - Rd:= Op1 - Op2 + C
0111 = RSC - Rd:= Op2 - Op1 + C
1000 = TST - set condition codes on Op1 AND Op2
1001 = TEQ - set condition codes on Op1 EOR Op2
1010 = CMP - set condition codes on Op1 - Op2
1011 = CMN - set condition codes on Op1 + Op2
1100 = ORR - Rd:= Op1 OR Op2
1101 = MOV - Rd:= Op2
1110 = BIC - Rd:= Op1 AND NOT Op2
1111 = MVN - Rd:= NOT Op2

*/

byte_type arm_instruction_data_processing_opcodes_bus(word_type instrucao) {
    switch (get_DataProcessing_opcode(instrucao)) {
        case 0b0000: //AND - Rd:= Op1 AND Op2
            return handle_AND(instrucao);
        case 0b0001: //EOR - Rd:= Op1 EOR Op2
            return handle_EOR(instrucao);
        case 0b0010: //SUB - Rd:= Op1 - Op2
            return handle_SUB(instrucao);
        case 0b0011: //RSB - Rd:= Op2 - Op1
            return handle_RSB(instrucao);
        case 0b0100: //ADD - Rd:= Op1 + Op2
            return handle_ADD(instrucao);
        case 0b0101: //ADC - Rd:= Op1 + Op2 + C
            return handle_ADC(instrucao);
        case 0b0110: //SBC - Rd:= Op1 - Op2 + C
            return handle_SBC(instrucao);
        case 0b0111: //RSC - Rd:= Op2 - Op1 + C
            return handle_RSC(instrucao);
        case 0b1000: //TST - set condition codes on Op1 AND Op2
            return handle_TST(instrucao);
        case 0b1001: //TEQ - set condition codes on Op1 EOR Op2
            return handle_TEQ(instrucao);
        case 0b1010: //CMP - set condition codes on Op1 - Op2
            return handle_CMP(instrucao);
        case 0b1011: //CMN - set condition codes on Op1 + Op2
            return handle_CMN(instrucao);
        case 0b1100: //ORR - Rd:= Op1 OR Op2
            return handle_ORR(instrucao);
        case 0b1101: //MOV - Rd:= Op2
            return handle_MOV(instrucao);
        case 0b1110: //BIC - Rd:= Op1 AND NOT Op2
            return handle_BIC(instrucao);
        case 0b1111: //MVN - Rd:= NOT Op2
            return handle_MVN(instrucao);
    }
}

void handle_multiply_common(word_type instrucao) {
    word_type RD = get_DataProcessing_RN(instrucao), RN = read_register(get_DataProcessing_RD(instrucao)), RS = read_register(get_DataProcessing_2d_operand_immediate_rotate_value(instrucao)), RM = read_register(get_DataProcessing_2d_operand_register(instrucao));
    double_word_type temporary = RM * RS;
    word_type return_value = temporary & 0xFFFFFFFF;
    if (is_bit_21_up(instrucao)) return_value+=RN;
    if (is_bit_20_up(instrucao)) {
        (return_value == 0)? flip_flag_Z(true) : flip_flag_Z(false);
        (return_value & 0x80000000)? flip_flag_N(true) : flip_flag_N(false);
    }
    write_to_register(RD, return_value);
}

void handle_multiply_long(word_type instrucao) {
    byte_type reg_high = get_DataProcessing_RN(instrucao), reg_low = get_DataProcessing_RD(instrucao);
    word_type RD_high = read_register(reg_high), RD_low = read_register(reg_low), RS = read_register(get_DataProcessing_2d_operand_immediate_rotate_value(instrucao)), RM = read_register(get_DataProcessing_2d_operand_register(instrucao));
    double_word_type unsigned_answer;
    signed_double_word_type signed_answer;
    double_word_type high_and_low= (((double_word_type)RD_high) << 32) | (RD_low);
    if (is_bit_22_up(instrucao)) {
        signed_answer = (signed_word_type)RM;
        signed_answer*= (signed_word_type)RS;
        if (is_bit_21_up(instrucao)) signed_answer += (unsigned) high_and_low;
        if (is_bit_20_up(instrucao)) {
            (signed_answer == 0)? flip_flag_Z(true) : flip_flag_Z(false);
            (signed_answer < 0)? flip_flag_N(true) : flip_flag_N(false);
        }
        //printf("\n\nsigned!!! -> RS: 0x%x | RM: 0x%x | rtrn_value: 0x%lx | high: 0x%x | low: 0x%x\n",RS, RM, signed_answer,(signed_answer & 0xFFFFFFFF00000000)>>32, signed_answer & 0xFFFFFFFF);
        write_to_register(reg_low,  (signed_answer  & 0x00000000FFFFFFFF));
        write_to_register(reg_high, ((signed_answer & 0xFFFFFFFF00000000) >> 32));

    }
    else {
        unsigned_answer = RS;
        unsigned_answer *=RM;
        if (is_bit_21_up(instrucao)) unsigned_answer+= high_and_low;
        if (is_bit_20_up(instrucao)) {
            (unsigned_answer == 0)? flip_flag_Z(true) : flip_flag_Z(false);
            (unsigned_answer & 0x8000000000000000)? flip_flag_N(true) : flip_flag_N(false);
        }
        //'printf("\n\nrtrn_value: 0x%lx | high: 0x%x | low: 0x%x\n", unsigned_answer,(unsigned_answer & 0xFFFFFFFF00000000)>>32, unsigned_answer & 0xFFFFFFFF);
        write_to_register(reg_low,  (unsigned_answer  & 0x00000000FFFFFFFF));
        write_to_register(reg_high, ((unsigned_answer & 0xFFFFFFFF00000000) >> 32));
    }
}

word_type align_word(word_type value, word_type address) {
    byte_type rotate_value = (address & 0b11)*8;
    word_type result = value;
    __asm ("ROR %b[rot], %[result]"
            : [result] "+r" (result)
            : [rot] "c" (rotate_value));
    return result;
}

void handle_STR(word_type instrucao) {
    word_type offset = (!is_bit_25_up(instrucao))? (instrucao & 0b00000000000000000000111111111111) : (handle_sencond_opperand_register(instrucao));
    word_type RN_base_register_value = read_register(get_DataProcessing_RN(instrucao)),
    RD_destination_register = read_register(get_DataProcessing_RD(instrucao)),
    address_com_offset;

    bool is_write_back = is_bit_21_up(instrucao),
    is_up = is_bit_23_up(instrucao),
    is_pre_indexing = is_bit_24_up(instrucao);
    address_com_offset = (is_up)? RN_base_register_value + offset : RN_base_register_value - offset;
    //printf("RN: 0x%x| RD: 0x%x | Offset: 0x%x| RN + Offset: : 0x%x|\n", RN_base_register_value,RD_destination_register, offset, address_com_offset);
    if (is_pre_indexing)
        RN_base_register_value = address_com_offset;
    word_type effective_base = (get_DataProcessing_RD(instrucao) == REG_PC)? (RN_base_register_value + 12) - ((RN_base_register_value + 12) % 4) : (RN_base_register_value) - (RN_base_register_value % 4);
    write_to_memory_via_databus(effective_base, RD_destination_register);
    RN_base_register_value = address_com_offset;
    if (is_write_back | !is_pre_indexing) {
        //printf("passei aq");
        write_to_register(get_DataProcessing_RN(instrucao), RN_base_register_value);
    }

    //if (is)
}

void handle_LDR(word_type instrucao) {
    half_word_type offset = (!is_bit_25_up(instrucao))? (half_word_type)(instrucao & 0b00000000000000000000111111111111) : (half_word_type)(handle_sencond_opperand_register(instrucao));
    word_type RN_base_register_value = read_register(get_DataProcessing_RN(instrucao)),
    RD_destination_register = read_register(get_DataProcessing_RD(instrucao)),
    address_com_offset;
    bool is_write_back = is_bit_21_up(instrucao),
    is_up = is_bit_23_up(instrucao),
    is_pre_indexing = is_bit_24_up(instrucao);
    address_com_offset = (is_up)? RN_base_register_value + offset : RN_base_register_value - offset;

    //printf("RN: 0x%x| RD: 0x%x | Offset: 0x%x| RN + Offset: : 0x%x|\n", RN_base_register_value,RD_destination_register, offset, address_com_offset);
    if (is_pre_indexing)
        RN_base_register_value = address_com_offset;
    word_type effective_base = RN_base_register_value - (RN_base_register_value % 4);
    write_to_register(get_DataProcessing_RD(instrucao),align_word(read_from_memory_via_databus(effective_base), RN_base_register_value));
    RD_destination_register = read_register(get_DataProcessing_RD(instrucao));
    //printf("| RD: 0x%x |\n",RD_destination_register);
    RN_base_register_value = address_com_offset;
    if (is_write_back | !is_pre_indexing)
        write_to_register(get_DataProcessing_RN(instrucao), RN_base_register_value);
}

void handle_STRB(word_type instrucao) {
    half_word_type offset = (!is_bit_25_up(instrucao))? instrucao & 0b00000000000000000000111111111111 : handle_sencond_opperand_register(instrucao);

    word_type RN_base_register_value = read_register(get_DataProcessing_RN(instrucao)),
    address_com_offset,
    effective_base,
    old_base = RN_base_register_value;

    byte_type RD_destination_register = (byte_type)(read_register(get_DataProcessing_RD(instrucao)) & 0x000000FF);
    bool is_write_back = is_bit_21_up(instrucao),
    is_up = is_bit_23_up(instrucao),
    is_pre_indexing = is_bit_24_up(instrucao);
    address_com_offset = (is_up)? RN_base_register_value + offset : RN_base_register_value - offset;
    effective_base = address_com_offset - (address_com_offset % 4);
    //printf("RN: 0x%x| RD: 0x%x | Offset: 0x%x| RN + Offset: : 0x%x|\n", RN_base_register_value,RD_destination_register, offset, address_com_offset);
    word_type old_memory_value = (get_DataProcessing_RD(instrucao) == REG_PC)? read_from_memory_via_databus(RN_base_register_value + 12) : read_from_memory_via_databus(RN_base_register_value);
    if (is_pre_indexing)
        RN_base_register_value = address_com_offset;
    word_type store_value;
    //printf("RD_destination_register & 0b00000000000000000000000000000011: 0x%2x", RD_destination_register & 0b00000000000000000000000000000011);
    switch (RN_base_register_value & 0b00000000000000000000000000000011) {
        case 0:
            store_value = (old_memory_value & 0xFFFFFF00) | (RD_destination_register);
            break;
        case 1:
            store_value = (old_memory_value & 0xFFFF00FF) | (((word_type)RD_destination_register) << 8);
            break;
        case 2:
            store_value = (old_memory_value & 0xFF00FFFF) | (((word_type)RD_destination_register) << 16);
            break;
        case 3:
            store_value = (old_memory_value & 0x00FFFFFF) | (((word_type)RD_destination_register) << 24);
            break;
    }

    (get_DataProcessing_RD(instrucao) == REG_PC)? write_to_memory_via_databus(effective_base + 12, store_value) : write_to_memory_via_databus(effective_base, store_value);
    RN_base_register_value = address_com_offset;
    if (is_write_back | !is_pre_indexing)
        write_to_register(get_DataProcessing_RN(instrucao), RN_base_register_value);
}

void handle_LDRB(word_type instrucao) {
    half_word_type offset = (!is_bit_25_up(instrucao))? instrucao & 0b00000000000000000000111111111111 : handle_sencond_opperand_register(instrucao);
    word_type address_com_offset;
    word_type RN_base_register_value = read_register(get_DataProcessing_RN(instrucao)), old_base = RN_base_register_value;
    bool is_write_back = is_bit_21_up(instrucao),
    is_up = is_bit_23_up(instrucao),
    is_pre_indexing = is_bit_24_up(instrucao);
    address_com_offset = (is_up)? RN_base_register_value + offset : RN_base_register_value - offset;
    word_type effective_base = address_com_offset - (address_com_offset % 4);
    //printf("RN: 0x%x| Offset: 0x%x| RN + Offset: : 0x%x| mem value: 0x%x\n\n", RN_base_register_value, offset, address_com_offset,read_from_memory_via_databus(address_com_offset));
    if (is_pre_indexing)
        RN_base_register_value = address_com_offset;
    word_type shift_value = (RN_base_register_value & 0b00000000000000000000000000000011) * 8;
    word_type used_value = ((read_from_memory_via_databus(effective_base) & (0x000000FF << shift_value)) >> (shift_value));
    write_to_register(get_DataProcessing_RD(instrucao),used_value);
    RN_base_register_value = address_com_offset;
    if (is_write_back | !is_pre_indexing)
        write_to_register(get_DataProcessing_RN(instrucao), RN_base_register_value);
}
void handle_STRHW(word_type instrucao) {
    byte_type offset = (!is_bit_25_up(instrucao))? (((instrucao & 0b00000000000000000000111100000000) >> 4) | (instrucao & 0b00000000000000000000000000001111)): handle_sencond_opperand_register(instrucao);
    word_type RN_base_register_value = read_register(get_DataProcessing_RN(instrucao)),
    address_com_offset,
    effective_base,
    old_base = RN_base_register_value;

    half_word_type RD_destination_register = (half_word_type)(read_register(get_DataProcessing_RD(instrucao)) & 0x0000FFFF);
    bool is_write_back = is_bit_21_up(instrucao),
    is_up = is_bit_23_up(instrucao),
    is_pre_indexing = is_bit_24_up(instrucao);
    address_com_offset = (is_up)? RN_base_register_value + offset : RN_base_register_value - offset;
    effective_base = RN_base_register_value;


    //printf("RN: 0x%x| RD: 0x%x | Offset: 0x%x| RN + Offset: : 0x%x| old_mem_val: 0x%x| RN+offset val: 0x%x\n", RN_base_register_value,RD_destination_register, offset, address_com_offset, 1,read_from_memory_via_databus(address_com_offset));
    if (is_pre_indexing) {
        RN_base_register_value = address_com_offset;
        effective_base = address_com_offset - (address_com_offset % 4);
    }
    word_type old_memory_value = (get_DataProcessing_RD(instrucao) == REG_PC)? read_from_memory_via_databus(effective_base + 12) : read_from_memory_via_databus(effective_base);
    word_type store_value;
    //printf("RD_destination_register & 0b00000000000000000000000000000011: 0x%2x", RD_destination_register & 0b00000000000000000000000000000011);

    switch ((RN_base_register_value & 0x00000003)) {
        case 0: case 1:
            store_value = (old_memory_value & 0xFFFF0000) | (RD_destination_register);
            break;
        case 2: case 3:
            store_value = (old_memory_value & 0x0000FFFF) | (((word_type)RD_destination_register) << 16);
            break;
    }
    //printf("Store value: 0x%x\n",store_value);
    (get_DataProcessing_RD(instrucao) == REG_PC)? write_to_memory_via_databus(effective_base + 12, store_value) : write_to_memory_via_databus(effective_base, store_value);
    RN_base_register_value = address_com_offset;
    if (is_write_back | !is_pre_indexing)
        write_to_register(get_DataProcessing_RN(instrucao), RN_base_register_value);
}

void handle_LDRHW(word_type instrucao) {
    byte_type offset = (!is_bit_25_up(instrucao))? (((instrucao & 0b00000000000000000000111100000000) >> 4) | (instrucao & 0b00000000000000000000000000001111)): handle_sencond_opperand_register(instrucao);
    word_type address_com_offset;
    word_type RN_base_register_value = read_register(get_DataProcessing_RN(instrucao)), old_base = RN_base_register_value;
    bool is_write_back = is_bit_21_up(instrucao),
    is_up = is_bit_23_up(instrucao),
    is_pre_indexing = is_bit_24_up(instrucao);
    address_com_offset = (is_up)? RN_base_register_value + offset : RN_base_register_value - offset;
    word_type effective_base = address_com_offset - (address_com_offset % 4);
    if (is_pre_indexing)
        RN_base_register_value = address_com_offset;
    //printf("RN: 0x%x| Offset: 0x%x| RN + Offset: : 0x%x| mem value: 0x%x | res value: 0x%x\n", RN_base_register_value, offset, address_com_offset,read_from_memory_via_databus(address_com_offset),;// arrumar um jeito de tranformar 0-1 em 0 e 2-3 em 1 // usar AND pra verificar a 2 casa de bit
    word_type valor_magico = ((((RN_base_register_value & 0x00000003) & 2)>>1))*4*4;
    //printf("valor_magicor value: 0x%x\n",valor_magico);
    write_to_register(get_DataProcessing_RD(instrucao),(read_from_memory_via_databus(effective_base) & (0x0000FFFF << (valor_magico))) >> (valor_magico));
    RN_base_register_value = address_com_offset;
    if (is_write_back | !is_pre_indexing)
        write_to_register(get_DataProcessing_RN(instrucao), RN_base_register_value);
}

void handle_LDR_HW_signed(word_type instrucao) {
/*
    half_word_type offset = (is_bit_25_up(instrucao))? (instrucao & 0b00000000000000000000111100000000>>4)|(instrucao & 0b00000000000000000000000000001111) : handle_sencond_opperand_register(instrucao);
    signed_half_word_type address_com_offset;
    word_type RN_base_register_value = read_register(get_DataProcessing_RN(instrucao));
    signed_half_word_type RN_base_signed = (RN_base_register_value & 0x80000000)? (signed_half_word_type)((signed_word_type)(RN_base_register_value & 0x7FFFFFFF)) * -1: (signed_half_word_type)RN_base_register_value;
    bool is_write_back = is_bit_21_up(instrucao),
    is_up = is_bit_23_up(instrucao),
    is_pre_indexing = is_bit_24_up(instrucao);
    address_com_offset = (is_up)? RN_base_signed + offset : RN_base_signed - offset;
    if (is_pre_indexing)
        RN_base_signed = address_com_offset;
    write_to_register(get_DataProcessing_RD(instrucao),read_from_memory_via_databus(RN_base_signed));
    RN_base_signed = address_com_offset;
    if (is_write_back | !is_pre_indexing)
        write_to_register(get_DataProcessing_RN(instrucao), RN_base_signed);
*/
    byte_type offset = (is_bit_22_up(instrucao))? (((instrucao & 0b00000000000000000000111100000000) >> 4) | (instrucao & 0b00000000000000000000000000001111)): read_register(instrucao & 0b1111) & 0xFF;
    //half_word_type offset = (!is_bit_25_up(instrucao))? (((instrucao & 0b00000000000000000000111100000000) >> 4) | (instrucao & 0b00000000000000000000000000001111)): handle_sencond_opperand_register(instrucao);
    word_type address_com_offset;
    word_type RN_base_register_value = read_register(get_DataProcessing_RN(instrucao)), old_base = RN_base_register_value;
    bool is_write_back = is_bit_21_up(instrucao),
    is_up = is_bit_23_up(instrucao),
    is_pre_indexing = is_bit_24_up(instrucao);
    address_com_offset = (is_up)? RN_base_register_value + offset : RN_base_register_value - offset;
    word_type effective_base = address_com_offset - (address_com_offset % 4);
    if (is_pre_indexing)
        RN_base_register_value = address_com_offset;
    //printf("RN: 0x%x| Offset: 0x%x| RN + Offset: : 0x%x| mem value: 0x%x | res value: 0x%x\n", RN_base_register_value, offset, address_com_offset,read_from_memory_via_databus(address_com_offset),;// arrumar um jeito de tranformar 0-1 em 0 e 2-3 em 1 // usar AND pra verificar a 2 casa de bit
    word_type valor_magico = ((((RN_base_register_value & 0x00000003) & 2)>>1))*4*4;
    //printf("valor_magicor value: 0x%x\n",valor_magico);
    word_type valor_a_ser_escrito = (read_from_memory_via_databus(effective_base) & (0x0000FFFF << (valor_magico))) >> (valor_magico);
    if (valor_a_ser_escrito&0x8000)
        valor_a_ser_escrito|= 0xFFFF0000;
    write_to_register(get_DataProcessing_RD(instrucao),valor_a_ser_escrito);
    RN_base_register_value = address_com_offset;
    if (is_write_back | !is_pre_indexing)
        write_to_register(get_DataProcessing_RN(instrucao), RN_base_register_value);
}
void handle_LDRB_signed(word_type instrucao) {
/*
    half_word_type offset = (is_bit_25_up(instrucao))? (instrucao & 0b00000000000000000000111100000000>>4)|(instrucao & 0b00000000000000000000000000001111) : handle_sencond_opperand_register(instrucao);
    signed_byte_type address_com_offset;
    word_type RN_base_register_value = read_register(get_DataProcessing_RN(instrucao));
    signed_byte_type RN_base_signed = (RN_base_register_value & 0x80000000)? (signed_byte_type)((signed_word_type)(RN_base_register_value & 0x7FFFFFFF)) * -1: (signed_byte_type)RN_base_register_value;
    bool is_write_back = is_bit_21_up(instrucao),
    is_up = is_bit_23_up(instrucao),
    is_pre_indexing = is_bit_24_up(instrucao);
    address_com_offset = (is_up)? RN_base_signed + offset : RN_base_signed - offset;
    if (is_pre_indexing)
        RN_base_signed = address_com_offset;
    write_to_register(get_DataProcessing_RD(instrucao),read_from_memory_via_databus(RN_base_signed));
    RN_base_signed = address_com_offset;
    if (is_write_back | !is_pre_indexing)
    write_to_register(get_DataProcessing_RN(instrucao), RN_base_signed);

*/
    byte_type offset = (is_bit_22_up(instrucao))? (((instrucao & 0b00000000000000000000111100000000) >> 4) | (instrucao & 0b00000000000000000000000000001111)): read_register(instrucao & 0b1111) & 0xFF;
    //printf("\n aqui: %d | offset: %d | bit25: %x", read_register(instrucao & 0b1111) & 0xFF, offset,is_bit_25_up(instrucao));
    word_type address_com_offset;
    word_type RN_base_register_value = read_register(get_DataProcessing_RN(instrucao)), old_base = RN_base_register_value;
    bool is_write_back = is_bit_21_up(instrucao),
    is_up = is_bit_23_up(instrucao),
    is_pre_indexing = is_bit_24_up(instrucao);
    address_com_offset = (is_up)? RN_base_register_value + offset : RN_base_register_value - offset;
    word_type effective_base = address_com_offset - (address_com_offset % 4);
    //printf("\nRN: 0x%x| RD: 0x%x | Offset: 0x%x| RN + Offset: : 0x%x| old_mem_val: 0x%x| RN+offset val: 0x%x | effective base: 0x%x | effective val: 0x%x|\n", RN_base_register_value,0, offset, address_com_offset, 1,read_from_memory_via_databus(address_com_offset), effective_base, read_from_memory_via_databus(effective_base));
    if (is_pre_indexing)
        RN_base_register_value = address_com_offset;
    word_type valor_a_ser_escrito = ((read_from_memory_via_databus(effective_base) & (0x000000FF << ((RN_base_register_value & 0b00000000000000000000000000000011) * 8)))) >> ((RN_base_register_value & 0b00000000000000000000000000000011) * 8);

    //printf("val: %x\n",valor_a_ser_escrito);
    if (valor_a_ser_escrito&0b10000000)
        valor_a_ser_escrito|= 0xFFFFFF00;
    write_to_register(get_DataProcessing_RD(instrucao),valor_a_ser_escrito);
    RN_base_register_value = address_com_offset;
    if (is_write_back | !is_pre_indexing)
        write_to_register(get_DataProcessing_RN(instrucao), RN_base_register_value);
}

void handle_SWP(word_type instrucao) {
    word_type temp_value = read_from_memory_via_databus(read_register(get_DataProcessing_RN(instrucao)));
    write_to_memory_via_databus(read_register(get_DataProcessing_RN(instrucao)), read_register(instrucao & 0b000000000000000000000000001111));
    write_to_register(get_DataProcessing_RD(instrucao), temp_value);
}
void handle_SWPB(word_type instrucao) {
    byte_type temp_value = read_from_memory_via_databus(read_register(get_DataProcessing_RN(instrucao)));
    write_to_memory_via_databus(read_register(get_DataProcessing_RN(instrucao)), (byte_type)read_register(instrucao & 0b000000000000000000000000001111));
    write_to_register(get_DataProcessing_RD(instrucao), temp_value);
}
byte_type arm_instruction_bus(word_type instrucao) {
    // MRS
    if ((instrucao & 0b00001111101111110000111111111111) == 0b00000001000011110000000000000000) {
        printf("MRS\n");
        return 0;
    }

    // MSR register
    if ((instrucao & 0b00001111101100001111111111110000) == 0b00000001001000001111000000000000) {
        printf("MSR register\n");
        return 0;
    }

    // MSR immediate
    if ((instrucao & 0b00001111101100001111000000000000) == 0b00000011001000001111000000000000) {
        printf("MSR immediate\n");
        return 0;
    }

    // BX
    if ((instrucao & 0b00001111111111111111111111110000) == 0b00000001001011111111111100010000) {
        printf("BX\n");
        return 0;
    }

    // MUL / MLA
    if ((instrucao & 0b00001111110000000000000011110000) == 0b00000000000000000000000010010000) {
        handle_multiply_common(instrucao);
        return 0;
    }

    // UMULL / UMLAL / SMULL / SMLAL
    if ((instrucao & 0b00001111100000000000000011110000) == 0b00000000100000000000000010010000) {
        handle_multiply_long(instrucao);
        return 0;
    }

    // STRH / LDRH - register offset
    if ((instrucao & 0b00001110010000000000111111110000) == 0b00000000000000000000000010110000) {
        (is_bit_20_up(instrucao))? handle_LDRHW(instrucao) : handle_STRHW(instrucao);
        return 0;
    }
    // LDRSB - register offset
    if ((instrucao & 0b00001110010100000000111111110000) == 0b00000000000100000000000011010000) {
        handle_LDRB_signed(instrucao);
        return 0;
    }

    // LDRSH - register offset
    if ((instrucao & 0b00001110010100000000111111110000) == 0b00000000000100000000000011110000) {
        handle_LDR_HW_signed(instrucao);
        return 0;
    }

    // STR / LDR - register offset
    if ((instrucao & 0b00001110010000000000000000010000) == 0b00000110000000000000000000000000) {
        (is_bit_20_up(instrucao))? handle_LDR(instrucao) : handle_STR(instrucao);
        return 0;
    }

    // STRB / LDRB - register offset
    if ((instrucao & 0b00001110010000000000000000010000) == 0b00000110010000000000000000000000) {
        (is_bit_20_up(instrucao))? handle_LDRB(instrucao) : handle_STRB(instrucao);
        return 0;
    }

    // STR / LDR - immediate offset
    if ((instrucao & 0b00001110010000000000000000000000) == 0b00000100000000000000000000000000) {
        (is_bit_20_up(instrucao))? handle_LDR(instrucao) : handle_STR(instrucao);
        return 0;
    }

    // STRB / LDRB - immediate offset
    if ((instrucao & 0b00001110010000000000000000000000) == 0b00000100010000000000000000000000) {
        (is_bit_20_up(instrucao))? handle_LDRB(instrucao) : handle_STRB(instrucao);
        return 0;
    }

    // STRH / LDRH - immediate offset
    if ((instrucao & 0b00001110010000000000000011110000) == 0b00000000010000000000000010110000) {
        (is_bit_20_up(instrucao))? handle_LDRHW(instrucao) : handle_STRHW(instrucao);
        return 0;
    }

    // LDRSB - immediate offset
    if ((instrucao & 0b00001110010100000000000011110000) == 0b00000000010100000000000011010000) {
        handle_LDRB_signed(instrucao);
        return 0;
    }

    // LDRSH - immediate offset
    if ((instrucao & 0b00001110010100000000000011110000) == 0b00000000010100000000000011110000) {
        handle_LDR_HW_signed(instrucao);
        return 0;
    }

    // SWP
    if ((instrucao & 0b00001111111100000000111111110000) == 0b00000001010000000000000010010000) {
        handle_SWPB(instrucao);
        return 0;
    }

    // SWP
    if (
        (instrucao & 0b00001111111100000000111111110000) == 0b00000001000000000000000010010000) {
        handle_SWP(instrucao);
        return 0;
    }

    // Undefined: classe 011 com bit4 = 1
    if ((instrucao & 0b00001110000000000000000000010000) == 0b00000110000000000000000000010000) {
        printf("undefined\n");
        return 0;
    }

    // STM / LDM
    if ((instrucao & 0b00001110000000000000000000000000) == 0b00001000000000000000000000000000) {
        printf("block transfer - STM/LDM\n");
        return 0;
    }

    // B / BL
    if ((instrucao & 0b00001110000000000000000000000000) == 0b00001010000000000000000000000000) {
        printf("branch - B/BL\n");
        return 0;
    }

    // LDC / STC
    if ((instrucao & 0b00001110000000000000000000000000) == 0b00001100000000000000000000000000) {
        printf("coprocessor data transfer -> undefined on GBA\n");
        return 0;
    }

    // CDP / MRC / MCR
    if ((instrucao & 0b00001111000000000000000000000000) == 0b00001110000000000000000000000000) {
        printf("coprocessor instruction -> undefined on GBA\n");
        return 0;
    }

    // SWI
    if ((instrucao & 0b00001111000000000000000000000000) == 0b00001111000000000000000000000000) {
        printf("SWI\n");
        return 0;
    }

    // Data Processing restante
    if ((instrucao & 0b00001100000000000000000000000000) == 0b00000000000000000000000000000000) {
        return arm_instruction_data_processing_opcodes_bus(instrucao);
    }

    printf("undefined / unhandled instruction: 0x%08X\n", instrucao);
    return 0;
}
void instruction_processing(word_type instruction) {
    if (instruction_condition_verifier(instruction)) {
        arm_instruction_bus(instruction);
    }
}
// -------------- debug start -------------

#ifndef ARM_TEST_PRINT_PASSES
#define ARM_TEST_PRINT_PASSES 0
#endif


#ifndef ARM_TEST_PAUSE_ON_FAIL
#define ARM_TEST_PAUSE_ON_FAIL 1
#endif

static void arm_test_pause_after_failure(void) {
    if (ARM_TEST_PAUSE_ON_FAIL) {
        printf("    [ENTER] continuar para o proximo teste...\n");
        getchar();
    }
}

// ============================================================
//              DATA PROCESSING TEST ECOSYSTEM
//
// Cobre os 16 opcodes:
//
// 0000 AND
// 0001 EOR
// 0010 SUB
// 0011 RSB
// 0100 ADD
// 0101 ADC
// 0110 SBC
// 0111 RSC
// 1000 TST
// 1001 TEQ
// 1010 CMP
// 1011 CMN
// 1100 ORR
// 1101 MOV
// 1110 BIC
// 1111 MVN
//
// Cobre também:
//   - instruções com e sem S
//   - N / Z / C / V
//   - carry / borrow
//   - signed overflow
//   - shifter carry x arithmetic carry
//   - carry de entrada de ADC/SBC/RSC
//   - shift imediato
//   - shift por registrador
//   - low 8 bits de Rs
//   - immediate rotated
//   - preservação de flags
//   - preservação de source registers
//
// NÃO cobre:
//   - S=1 + Rd=R15
//     porque aí entra SPSR/CPSR + processor modes/exceptions.
// ============================================================


#define DP_TEST_SENTINEL 0xDEADBEEF


// ============================================================
// OPCODES
// ============================================================

enum {
    DP_OP_AND = 0b0000,
    DP_OP_EOR = 0b0001,
    DP_OP_SUB = 0b0010,
    DP_OP_RSB = 0b0011,
    DP_OP_ADD = 0b0100,
    DP_OP_ADC = 0b0101,
    DP_OP_SBC = 0b0110,
    DP_OP_RSC = 0b0111,
    DP_OP_TST = 0b1000,
    DP_OP_TEQ = 0b1001,
    DP_OP_CMP = 0b1010,
    DP_OP_CMN = 0b1011,
    DP_OP_ORR = 0b1100,
    DP_OP_MOV = 0b1101,
    DP_OP_BIC = 0b1110,
    DP_OP_MVN = 0b1111
};


enum {
    DP_SHIFT_LSL = 0b00,
    DP_SHIFT_LSR = 0b01,
    DP_SHIFT_ASR = 0b10,
    DP_SHIFT_ROR = 0b11
};


// ============================================================
// OPCODE BUILDERS
// ============================================================


// ------------------------------------------------------------
// Data Processing
//
// Operand2 = Rm
//
// Na prática:
//
// Rm LSL #0
// ------------------------------------------------------------

#define ARM_DP_REG(OP, S, RN, RD, RM) ( \
    0xE0000000u |                      \
    (((OP) & 0xFu) << 21) |            \
    (((S)  & 0x1u) << 20) |            \
    (((RN) & 0xFu) << 16) |            \
    (((RD) & 0xFu) << 12) |            \
    ((RM) & 0xFu)                       \
)


// ------------------------------------------------------------
// Data Processing
//
// Operand2 = Rm shift #immediate
// ------------------------------------------------------------

#define ARM_DP_SHIFT_IMM(OP, S, RN, RD, RM, TYPE, AMOUNT) ( \
    0xE0000000u |                                           \
    (((OP)     & 0xFu)  << 21) |                            \
    (((S)      & 0x1u)  << 20) |                            \
    (((RN)     & 0xFu)  << 16) |                            \
    (((RD)     & 0xFu)  << 12) |                            \
    (((AMOUNT) & 0x1Fu) << 7) |                             \
    (((TYPE)   & 0x3u)  << 5) |                             \
    ((RM) & 0xFu)                                           \
)


// ------------------------------------------------------------
// Data Processing
//
// Operand2 = Rm shift Rs
//
// bit4 = 1
// bit7 = 0
// ------------------------------------------------------------

#define ARM_DP_SHIFT_REG(OP, S, RN, RD, RM, TYPE, RS) ( \
    0xE0000000u |                                      \
    (((OP)   & 0xFu) << 21) |                          \
    (((S)    & 0x1u) << 20) |                          \
    (((RN)   & 0xFu) << 16) |                          \
    (((RD)   & 0xFu) << 12) |                          \
    (((RS)   & 0xFu) << 8) |                           \
    (((TYPE) & 0x3u) << 5) |                           \
    (1u << 4) |                                        \
    ((RM) & 0xFu)                                      \
)


// ------------------------------------------------------------
// Data Processing immediate
//
// Operand2:
//
//     imm8 ROR (ROT * 2)
//
// ROT é o campo de 4 bits do opcode.
//
// Ex:
//
// ROT = 4
// rotate real = 8
// ------------------------------------------------------------

#define ARM_DP_IMM(OP, S, RN, RD, ROT, IMM8) ( \
    0xE2000000u |                              \
    (((OP)   & 0xFu) << 21) |                  \
    (((S)    & 0x1u) << 20) |                  \
    (((RN)   & 0xFu) << 16) |                  \
    (((RD)   & 0xFu) << 12) |                  \
    (((ROT)  & 0xFu) << 8) |                   \
    ((IMM8) & 0xFFu)                           \
)


// ============================================================
// TEST STATE HELPERS
// ============================================================

static void arm_test_reset_state(void) {

    for (byte_type i = 0; i < 16; i++) {
        write_to_register(i, 0);
    }

    flip_flag_N(false);
    flip_flag_Z(false);
    flip_flag_C(false);
    flip_flag_V(false);
}


static void arm_test_set_flags(
    bool N,
    bool Z,
    bool C,
    bool V
) {

    flip_flag_N(false);
    flip_flag_Z(false);
    flip_flag_C(false);
    flip_flag_V(false);

    if (N) {
        flip_flag_N(true);
    }

    if (Z) {
        flip_flag_Z(true);
    }

    if (C) {
        flip_flag_C(true);
    }

    if (V) {
        flip_flag_V(true);
    }
}


// ============================================================
// TEST STRUCT
// ============================================================

typedef struct {

    const char *name;

    word_type opcode;

    /*
     * Convenção de todos os testes:
     *
     * R0 = destino / sentinel
     * R1 = Rn
     * R2 = Rm
     * R3 = Rs
     */

    word_type r1;
    word_type r2;
    word_type r3;

    /*
     * Flags ANTES da instrução
     */
    bool N;
    bool Z;
    bool C;
    bool V;

    /*
     * Estado esperado DEPOIS
     */
    word_type expected_r0;

    bool expected_N;
    bool expected_Z;
    bool expected_C;
    bool expected_V;

} dp_torture_test_t;


// ============================================================
// TEST DATABASE
// ============================================================

static dp_torture_test_t dp_torture_tests[] = {

    // ========================================================
    // 00 - 11
    //
    // OPERAÇÕES SEM S
    //
    // Resultado deve estar correto e TODAS as flags devem
    // permanecer exatamente como estavam.
    // ========================================================

    {
        "AND sem S - resultado + flags preservadas",

        ARM_DP_REG(DP_OP_AND, 0, 1, 0, 2),

        0xF0F0F0F0,
        0x0FF00FF0,
        0,

        true, true, false, true,

        0x00F000F0,

        true, true, false, true
    },

    {
        "EOR sem S - resultado + flags preservadas",

        ARM_DP_REG(DP_OP_EOR, 0, 1, 0, 2),

        0xF0F0F0F0,
        0x0FF00FF0,
        0,

        true, true, false, true,

        0xFF00FF00,

        true, true, false, true
    },

    {
        "SUB sem S - 10 - 3",

        ARM_DP_REG(DP_OP_SUB, 0, 1, 0, 2),

        10,
        3,
        0,

        true, true, false, true,

        7,

        true, true, false, true
    },

    {
        "RSB sem S - 3 - 10",

        ARM_DP_REG(DP_OP_RSB, 0, 1, 0, 2),

        10,
        3,
        0,

        true, true, false, true,

        0xFFFFFFF9,

        true, true, false, true
    },

    {
        "ADD sem S - 10 + 3",

        ARM_DP_REG(DP_OP_ADD, 0, 1, 0, 2),

        10,
        3,
        0,

        true, true, false, true,

        13,

        true, true, false, true
    },

    {
        "ADC sem S - C=1",

        ARM_DP_REG(DP_OP_ADC, 0, 1, 0, 2),

        10,
        3,
        0,

        true, false, true, true,

        14,

        true, false, true, true
    },

    {
        "SBC sem S - C=1",

        ARM_DP_REG(DP_OP_SBC, 0, 1, 0, 2),

        10,
        3,
        0,

        true, false, true, true,

        7,

        true, false, true, true
    },

    {
        "RSC sem S - C=1",

        ARM_DP_REG(DP_OP_RSC, 0, 1, 0, 2),

        10,
        3,
        0,

        true, false, true, true,

        0xFFFFFFF9,

        true, false, true, true
    },

    {
        "ORR sem S",

        ARM_DP_REG(DP_OP_ORR, 0, 1, 0, 2),

        0xF000F000,
        0x0F0F00F0,
        0,

        true, true, false, true,

        0xFF0FF0F0,

        true, true, false, true
    },

    {
        "MOV sem S",

        ARM_DP_REG(DP_OP_MOV, 0, 0, 0, 2),

        0,
        0x12345678,
        0,

        true, true, false, true,

        0x12345678,

        true, true, false, true
    },

    {
        "BIC sem S",

        ARM_DP_REG(DP_OP_BIC, 0, 1, 0, 2),

        0xFFFF00FF,
        0x00FF0F0F,
        0,

        true, true, false, true,

        0xFF0000F0,

        true, true, false, true
    },

    {
        "MVN sem S",

        ARM_DP_REG(DP_OP_MVN, 0, 0, 0, 2),

        0,
        0x12345678,
        0,

        true, true, false, true,

        0xEDCBA987,

        true, true, false, true
    },


    // ========================================================
    // 12 - 19
    //
    // LÓGICAS COM S
    //
    // N = bit31 do resultado
    // Z = resultado == 0
    // C = carry produzido pelo barrel shifter
    // V = preservado
    // ========================================================

    {
        "ANDS - resultado zero - C preservado",

        ARM_DP_REG(DP_OP_AND, 1, 1, 0, 2),

        0x000000F0,
        0x0000000F,
        0,

        true, false, true, true,

        0x00000000,

        false, true, true, true
    },

    {
        "ANDS - LSL #1 gera N=1 C=1",

        ARM_DP_SHIFT_IMM(
            DP_OP_AND,
            1,
            1,
            0,
            2,
            DP_SHIFT_LSL,
            1
        ),

        0xFFFFFFFF,
        0xC0000000,
        0,

        false, false, false, true,

        0x80000000,

        true, false, true, true
    },

    {
        "EORS - resultado negativo",

        ARM_DP_REG(DP_OP_EOR, 1, 1, 0, 2),

        0xFFFFFFFF,
        0x7FFFFFFF,
        0,

        false, false, false, true,

        0x80000000,

        true, false, false, true
    },

    {
        "ORRS - ROR #1 - shifter C=1",

        ARM_DP_SHIFT_IMM(
            DP_OP_ORR,
            1,
            1,
            0,
            2,
            DP_SHIFT_ROR,
            1
        ),

        0x00000000,
        0x00000001,
        0,

        false, false, false, false,

        0x80000000,

        true, false, true, false
    },

    {
        "BICS - resultado zero",

        ARM_DP_REG(DP_OP_BIC, 1, 1, 0, 2),

        0xFFFFFFFF,
        0xFFFFFFFF,
        0,

        true, false, true, true,

        0x00000000,

        false, true, true, true
    },

    {
        "MVNS - resultado zero",

        ARM_DP_REG(DP_OP_MVN, 1, 0, 0, 2),

        0,
        0xFFFFFFFF,
        0,

        true, false, true, false,

        0x00000000,

        false, true, true, false
    },

    {
        "MOVS - zero",

        ARM_DP_REG(DP_OP_MOV, 1, 0, 0, 2),

        0,
        0x00000000,
        0,

        true, false, true, true,

        0x00000000,

        false, true, true, true
    },

    {
        "MOVS - negativo",

        ARM_DP_REG(DP_OP_MOV, 1, 0, 0, 2),

        0,
        0x80000000,
        0,

        false, true, false, true,

        0x80000000,

        true, false, false, true
    },


    // ========================================================
    // 20 - 23
    //
    // TST / TEQ
    //
    // NÃO escrevem Rd.
    //
    // Portanto R0 tem que continuar DEADBEEF.
    // ========================================================

    {
        "TST - resultado zero - nao escreve R0",

        ARM_DP_REG(DP_OP_TST, 1, 1, 0, 2),

        0x000000F0,
        0x0000000F,
        0,

        true, false, true, true,

        DP_TEST_SENTINEL,

        false, true, true, true
    },

    {
        "TST - LSL #1 - N=1 C=1",

        ARM_DP_SHIFT_IMM(
            DP_OP_TST,
            1,
            1,
            0,
            2,
            DP_SHIFT_LSL,
            1
        ),

        0xFFFFFFFF,
        0xC0000000,
        0,

        false, false, false, true,

        DP_TEST_SENTINEL,

        true, false, true, true
    },

    {
        "TEQ - iguais - Z=1",

        ARM_DP_REG(DP_OP_TEQ, 1, 1, 0, 2),

        0x12345678,
        0x12345678,
        0,

        true, false, false, true,

        DP_TEST_SENTINEL,

        false, true, false, true
    },

    {
        "TEQ - resultado negativo",

        ARM_DP_REG(DP_OP_TEQ, 1, 1, 0, 2),

        0xFFFFFFFF,
        0x7FFFFFFF,
        0,

        false, true, true, false,

        DP_TEST_SENTINEL,

        true, false, true, false
    },


    // ========================================================
    // 24 - 26
    //
    // ADD + FLAGS
    // ========================================================

    {
        "ADDS - unsigned carry + zero",

        ARM_DP_REG(DP_OP_ADD, 1, 1, 0, 2),

        0xFFFFFFFF,
        0x00000001,
        0,

        false, false, false, false,

        0x00000000,

        false, true, true, false
    },

    {
        "ADDS - signed overflow positivo",

        ARM_DP_REG(DP_OP_ADD, 1, 1, 0, 2),

        0x7FFFFFFF,
        0x00000001,
        0,

        false, false, true, false,

        0x80000000,

        true, false, false, true
    },

    {
        "ADDS - dois negativos - C=1 V=1",

        ARM_DP_REG(DP_OP_ADD, 1, 1, 0, 2),

        0x80000000,
        0x80000000,
        0,

        false, false, false, false,

        0x00000000,

        false, true, true, true
    },


    // ========================================================
    // 27 - 29
    //
    // SUB + FLAGS
    //
    // ARM em subtração:
    //
    // C=1 -> NÃO houve borrow
    // C=0 -> houve borrow
    // ========================================================

    {
        "SUBS - 5 - 3 - sem borrow",

        ARM_DP_REG(DP_OP_SUB, 1, 1, 0, 2),

        5,
        3,
        0,

        false, false, false, true,

        2,

        false, false, true, false
    },

    {
        "SUBS - 3 - 5 - borrow",

        ARM_DP_REG(DP_OP_SUB, 1, 1, 0, 2),

        3,
        5,
        0,

        false, false, true, true,

        0xFFFFFFFE,

        true, false, false, false
    },

    {
        "SUBS - signed overflow",

        ARM_DP_REG(DP_OP_SUB, 1, 1, 0, 2),

        0x80000000,
        0x00000001,
        0,

        false, false, false, false,

        0x7FFFFFFF,

        false, false, true, true
    },


    // ========================================================
    // 30 - 31
    //
    // RSB
    //
    // resultado = Operand2 - Rn
    // ========================================================

    {
        "RSBS - 3 - 5",

        ARM_DP_REG(DP_OP_RSB, 1, 1, 0, 2),

        5,
        3,
        0,

        false, false, true, true,

        0xFFFFFFFE,

        true, false, false, false
    },

    {
        "RSBS - signed overflow",

        ARM_DP_REG(DP_OP_RSB, 1, 1, 0, 2),

        0x80000000,
        0x7FFFFFFF,
        0,

        false, false, true, false,

        0xFFFFFFFF,

        true, false, false, true
    },


    // ========================================================
    // 32 - 35
    //
    // ADC
    //
    // resultado:
    //
    // Rn + Operand2 + CarryAntigo
    // ========================================================

    {
        "ADCS - C entrada 0",

        ARM_DP_REG(DP_OP_ADC, 1, 1, 0, 2),

        1,
        2,
        0,

        false, false, false, true,

        3,

        false, false, false, false
    },

    {
        "ADCS - C entrada 1",

        ARM_DP_REG(DP_OP_ADC, 1, 1, 0, 2),

        1,
        2,
        0,

        false, false, true, true,

        4,

        false, false, false, false
    },

    {
        "ADCS - carry out",

        ARM_DP_REG(DP_OP_ADC, 1, 1, 0, 2),

        0xFFFFFFFF,
        0x00000000,
        0,

        false, false, true, false,

        0x00000000,

        false, true, true, false
    },

    {
        "ADCS - overflow usando carry-in",

        ARM_DP_REG(DP_OP_ADC, 1, 1, 0, 2),

        0x7FFFFFFF,
        0x00000000,
        0,

        false, false, true, false,

        0x80000000,

        true, false, false, true
    },


    // ========================================================
    // 36 - 39
    //
    // SBC
    //
    // resultado:
    //
    // Rn - Operand2 - NOT(C)
    //
    // C=1:
    //      Rn - Operand2
    //
    // C=0:
    //      Rn - Operand2 - 1
    // ========================================================

    {
        "SBCS - C=1",

        ARM_DP_REG(DP_OP_SBC, 1, 1, 0, 2),

        5,
        3,
        0,

        false, false, true, true,

        2,

        false, false, true, false
    },

    {
        "SBCS - C=0 acrescenta borrow",

        ARM_DP_REG(DP_OP_SBC, 1, 1, 0, 2),

        5,
        3,
        0,

        false, false, false, true,

        1,

        false, false, true, false
    },

    {
        "SBCS - 0 - 0 com C=0",

        ARM_DP_REG(DP_OP_SBC, 1, 1, 0, 2),

        0,
        0,
        0,

        false, false, false, true,

        0xFFFFFFFF,

        true, false, false, false
    },

    {
        "SBCS - signed overflow",

        ARM_DP_REG(DP_OP_SBC, 1, 1, 0, 2),

        0x80000000,
        0x00000001,
        0,

        false, false, true, false,

        0x7FFFFFFF,

        false, false, true, true
    },


    // ========================================================
    // 40 - 43
    //
    // RSC
    //
    // resultado:
    //
    // Operand2 - Rn - NOT(C)
    // ========================================================

    {
        "RSCS - C=1",

        ARM_DP_REG(DP_OP_RSC, 1, 1, 0, 2),

        5,
        3,
        0,

        false, false, true, true,

        0xFFFFFFFE,

        true, false, false, false
    },

    {
        "RSCS - C=0",

        ARM_DP_REG(DP_OP_RSC, 1, 1, 0, 2),

        5,
        3,
        0,

        false, false, false, true,

        0xFFFFFFFD,

        true, false, false, false
    },

    {
        "RSCS - sem borrow",

        ARM_DP_REG(DP_OP_RSC, 1, 1, 0, 2),

        3,
        5,
        0,

        false, false, true, true,

        2,

        false, false, true, false
    },

    {
        "RSCS - signed overflow",

        ARM_DP_REG(DP_OP_RSC, 1, 1, 0, 2),

        0x80000000,
        0x7FFFFFFF,
        0,

        false, false, true, false,

        0xFFFFFFFF,

        true, false, false, true
    },


    // ========================================================
    // 44 - 46
    //
    // CMP
    //
    // Executa:
    //
    // Rn - Operand2
    //
    // SOMENTE para gerar flags.
    //
    // R0 continua sentinel.
    // ========================================================

    {
        "CMP - iguais",

        ARM_DP_REG(DP_OP_CMP, 1, 1, 0, 2),

        5,
        5,
        0,

        true, false, false, true,

        DP_TEST_SENTINEL,

        false, true, true, false
    },

    {
        "CMP - unsigned borrow",

        ARM_DP_REG(DP_OP_CMP, 1, 1, 0, 2),

        3,
        5,
        0,

        false, true, true, true,

        DP_TEST_SENTINEL,

        true, false, false, false
    },

    {
        "CMP - signed overflow",

        ARM_DP_REG(DP_OP_CMP, 1, 1, 0, 2),

        0x80000000,
        0x00000001,
        0,

        false, false, false, false,

        DP_TEST_SENTINEL,

        false, false, true, true
    },


    // ========================================================
    // 47 - 48
    //
    // CMN
    //
    // Executa:
    //
    // Rn + Operand2
    //
    // somente para flags.
    // ========================================================

    {
        "CMN - carry + zero",

        ARM_DP_REG(DP_OP_CMN, 1, 1, 0, 2),

        0xFFFFFFFF,
        0x00000001,
        0,

        true, false, false, true,

        DP_TEST_SENTINEL,

        false, true, true, false
    },

    {
        "CMN - signed overflow",

        ARM_DP_REG(DP_OP_CMN, 1, 1, 0, 2),

        0x7FFFFFFF,
        0x00000001,
        0,

        false, false, true, false,

        DP_TEST_SENTINEL,

        true, false, false, true
    },


    // ========================================================
    // 49
    //
    // ADC:
    //
    // C ANTIGO != SHIFTER C
    //
    // Isso pega uma armadilha importante:
    //
    // ADC precisa usar o Carry que existia ANTES da instrução.
    //
    // O barrel shifter também pode gerar um Carry.
    //
    // O Carry do shifter NÃO pode substituir o C antigo antes
    // de calcular o ADC.
    // ========================================================

    {
        "ADCS - usa C antigo, nao C produzido pelo LSR",

        ARM_DP_SHIFT_IMM(
            DP_OP_ADC,
            1,
            1,
            0,
            2,
            DP_SHIFT_LSR,
            1
        ),

        /*
         * Rn
         */
        0x00000000,

        /*
         * Rm
         *
         * 2 LSR 1 = 1
         *
         * shifter C = bit0 original = 0
         */
        0x00000002,

        0,

        /*
         * C antigo = 1
         */
        false, false, true, false,

        /*
         * ADC:
         *
         * 0 + 1 + CarryAntigo(1)
         *
         * = 2
         */
        0x00000002,

        false, false, false, false
    },


    // ========================================================
    // 50
    //
    // SBC:
    //
    // Mesmo problema ao contrário.
    //
    // C antigo = 0
    //
    // Operand2 gera shifter C = 1.
    //
    // O SBC ainda precisa usar o C ANTIGO = 0.
    // ========================================================

    {
        "SBCS - usa C antigo, nao C produzido pelo LSR",

        ARM_DP_SHIFT_IMM(
            DP_OP_SBC,
            1,
            1,
            0,
            2,
            DP_SHIFT_LSR,
            1
        ),

        /*
         * Rn = 5
         */
        0x00000005,

        /*
         * 1 LSR 1 = 0
         *
         * shifter C = 1
         */
        0x00000001,

        0,

        /*
         * Carry ANTIGO = 0
         */
        false, false, false, false,

        /*
         * SBC:
         *
         * 5 - 0 - NOT(0)
         *
         * 5 - 0 - 1
         *
         * = 4
         */
        0x00000004,

        false, false, true, false
    },


    // ========================================================
    // 51
    //
    // ARITHMETIC C x SHIFTER C
    //
    // ADD com S:
    //
    // Carry final vem da SOMA.
    //
    // NÃO vem do barrel shifter.
    // ========================================================

    {
        "ADDS immediate rotated - shifter C=1 mas arithmetic C=0",

        ARM_DP_IMM(
            DP_OP_ADD,
            1,
            1,
            0,
            4,
            0x80
        ),

        /*
         * Rn
         */
        0x00000000,

        0,
        0,

        false, false, false, false,

        /*
         * immediate:
         *
         * 0x80 ROR 8
         *
         * = 0x80000000
         *
         * shifter C = 1
         *
         * porém:
         *
         * 0 + 0x80000000
         *
         * não produz unsigned carry.
         *
         * C final = 0
         */
        0x80000000,

        true, false, false, false
    },


    // ========================================================
    // 52
    //
    // LOGICAL C x SHIFTER C
    //
    // Para ANDS:
    //
    // Carry final DEVE vir do shifter.
    // ========================================================

    {
        "ANDS immediate rotated - C vem do shifter",

        ARM_DP_IMM(
            DP_OP_AND,
            1,
            1,
            0,
            4,
            0x80
        ),

        0xFFFFFFFF,
        0,
        0,

        false, false, false, true,

        /*
         * Operand2:
         *
         * 0x80 ROR 8
         *
         * = 0x80000000
         *
         * shifter C = 1
         *
         * AND:
         *
         * FFFFFFFF & 80000000
         *
         * = 80000000
         */
        0x80000000,

        true, false, true, true
    },


    // ========================================================
    // 53
    //
    // CMP COM SHIFT
    //
    // Shifter gera C=1.
    //
    // CMP deve gerar o C da SUBTRAÇÃO.
    // ========================================================

    {
        "CMP com LSL - arithmetic C sobrescreve shifter C",

        ARM_DP_SHIFT_IMM(
            DP_OP_CMP,
            1,
            1,
            0,
            2,
            DP_SHIFT_LSL,
            1
        ),

        /*
         * Rn = 0
         */
        0x00000000,

        /*
         * C0000000 LSL 1
         *
         * = 80000000
         *
         * shifter C = 1
         */
        0xC0000000,

        0,

        false, false, false, false,

        /*
         * CMP:
         *
         * 00000000 - 80000000
         *
         * resultado interno = 80000000
         *
         * houve borrow:
         * C = 0
         *
         * signed overflow:
         * V = 1
         */
        DP_TEST_SENTINEL,

        true, false, false, true
    },


    // ========================================================
    // 54
    //
    // CMN COM SHIFT
    //
    // Mesmo conceito:
    //
    // Carry final vem da soma.
    // ========================================================

    {
        "CMN com LSL - arithmetic C sobrescreve shifter C",

        ARM_DP_SHIFT_IMM(
            DP_OP_CMN,
            1,
            1,
            0,
            2,
            DP_SHIFT_LSL,
            1
        ),

        0x00000000,

        /*
         * C0000000 LSL 1
         *
         * = 80000000
         *
         * shifter C = 1
         */
        0xC0000000,

        0,

        false, false, false, true,

        DP_TEST_SENTINEL,

        /*
         * 0 + 80000000
         *
         * N=1
         * Z=0
         * C=0
         * V=0
         */
        true, false, false, false
    },


    // ========================================================
    // 55
    //
    // SHIFT POR REGISTER USA APENAS LOW 8 BITS DE Rs
    //
    // R3 = 0x101
    //
    // amount efetivo = 0x01
    // ========================================================

    {
        "ANDS LSL R3 - usa low 8 bits de R3",

        ARM_DP_SHIFT_REG(
            DP_OP_AND,
            1,
            1,
            0,
            2,
            DP_SHIFT_LSL,
            3
        ),

        0xFFFFFFFF,

        /*
         * 80000001 LSL 1
         *
         * = 00000002
         *
         * carry = bit31 original = 1
         */
        0x80000001,

        /*
         * low 8 bits:
         *
         * 0x101 -> 0x01
         */
        0x00000101,

        false, false, false, true,

        0x00000002,

        false, false, true, true
    },


    // ========================================================
    // 56
    //
    // SEM S + SHIFT QUE PRODUZ C
    //
    // O barrel shifter pode calcular carry internamente,
    // mas CPSR NÃO deve ser alterado.
    // ========================================================

    {
        "AND sem S com LSL - shifter nao pode alterar flags",

        ARM_DP_SHIFT_IMM(
            DP_OP_AND,
            0,
            1,
            0,
            2,
            DP_SHIFT_LSL,
            1
        ),

        0xFFFFFFFF,
        0xC0000000,
        0,

        true, true, false, true,

        /*
         * C0000000 << 1
         *
         * = 80000000
         */
        0x80000000,

        /*
         * flags completamente preservadas
         */
        true, true, false, true
    },


    // ========================================================
    // 57
    //
    // IMMEDIATE ROTATE = 0
    //
    // Para operação lógica com S:
    //
    // se rotate == 0:
    //
    // shifter carry = Carry antigo.
    // ========================================================

    {
        "ANDS immediate rotate=0 - preserva C",

        ARM_DP_IMM(
            DP_OP_AND,
            1,
            1,
            0,
            0,
            0xF0
        ),

        0x000000FF,
        0,
        0,

        /*
         * C antigo = 1
         */
        false, false, true, false,

        /*
         * FF & F0 = F0
         */
        0x000000F0,

        false, false, true, false
    },


    // ========================================================
    // 58
    //
    // SHIFT REGISTER = 32 DENTRO DE UMA ARITMÉTICA
    //
    // Rm = 80000001
    // Rs = 32
    //
    // LSL 32:
    //
    // Operand2 = 0
    // shifter C = bit0 original = 1
    //
    // MAS ADDS deve usar arithmetic carry como C final.
    // ========================================================

    {
        "ADDS com LSL R3=32 - C final e arithmetic carry",

        ARM_DP_SHIFT_REG(
            DP_OP_ADD,
            1,
            1,
            0,
            2,
            DP_SHIFT_LSL,
            3
        ),

        /*
         * Rn = 1
         */
        0x00000001,

        /*
         * Rm
         */
        0x80000001,

        /*
         * Rs
         */
        32,

        false, false, false, false,

        /*
         * Operand2:
         *
         * 80000001 LSL 32
         *
         * = 00000000
         *
         * shifter C = 1
         *
         * ADD:
         *
         * 1 + 0 = 1
         *
         * arithmetic C = 0
         */
        0x00000001,

        false, false, false, false
    },
};


#define DP_TORTURE_TEST_COUNT \
    (sizeof(dp_torture_tests) / sizeof(dp_torture_tests[0]))


// ============================================================
// RUNNER
// ============================================================

void run_data_processing_torture_tests(void) {

    int passed = 0;
    int failed = 0;

    printf("\n");
    // printf("=============================================================\n");
    // printf("                DATA PROCESSING TORTURE TESTS\n");
    // printf("=============================================================\n\n");


    for (size_t i = 0; i < DP_TORTURE_TEST_COUNT; i++) {

        dp_torture_test_t *test = &dp_torture_tests[i];


        // ----------------------------------------------------
        // RESET
        // ----------------------------------------------------

        arm_test_reset_state();


        // ----------------------------------------------------
        // REGISTERS
        // ----------------------------------------------------

        write_to_register(0, DP_TEST_SENTINEL);
        write_to_register(1, test->r1);
        write_to_register(2, test->r2);
        write_to_register(3, test->r3);


        // ----------------------------------------------------
        // CPSR FLAGS
        // ----------------------------------------------------

        arm_test_set_flags(
            test->N,
            test->Z,
            test->C,
            test->V
        );


        // ----------------------------------------------------
        // EXECUÇÃO REAL
        //
        // condition verifier
        //      ->
        // instruction bus
        //      ->
        // DP bus
        //      ->
        // handler
        //      ->
        // Operand2
        // ----------------------------------------------------

        instruction_processing(test->opcode);


        // ----------------------------------------------------
        // READ RESULT
        // ----------------------------------------------------

        word_type got_r0 = read_register(0);
        word_type got_r1 = read_register(1);
        word_type got_r2 = read_register(2);
        word_type got_r3 = read_register(3);


        bool got_N = is_flag_N_on();
        bool got_Z = is_flag_Z_on();
        bool got_C = is_flag_C_on();
        bool got_V = is_flag_V_on();


        // ----------------------------------------------------
        // VERIFY DESTINATION
        // ----------------------------------------------------

        bool result_ok =
            got_r0 == test->expected_r0;


        // ----------------------------------------------------
        // VERIFY SOURCE REGISTERS
        //
        // Nenhum teste usa R1/R2/R3 como destino.
        //
        // Se algum deles mudou, alguma coisa fez merda.
        // ----------------------------------------------------

        bool source_registers_ok =
            got_r1 == test->r1 &&
            got_r2 == test->r2 &&
            got_r3 == test->r3;


        // ----------------------------------------------------
        // VERIFY FLAGS
        // ----------------------------------------------------

        bool flags_ok =
            got_N == test->expected_N &&
            got_Z == test->expected_Z &&
            got_C == test->expected_C &&
            got_V == test->expected_V;


        bool ok =
            result_ok &&
            source_registers_ok &&
            flags_ok;


        // ----------------------------------------------------
        // COUNTER
        // ----------------------------------------------------

        if (ok) {
            passed++;
            //continue;
        }
        else {
            failed++;
        }


        // ----------------------------------------------------
        // OUTPUT
        // ----------------------------------------------------

        if (!ok || ARM_TEST_PRINT_PASSES) {
            printf(
                "[%s] #%02zu - %s\n"
                "    opcode: 0x%08X\n"
                "\n"
                "    R1: 0x%08X\n"
                "    R2: 0x%08X\n"
                "    R3: 0x%08X\n"
                "\n"
                "    R0 esperado: 0x%08X\n"
                "    R0 recebido: 0x%08X\n"
                "\n"
                "    flags entrada:  N=%d Z=%d C=%d V=%d\n"
                "    flags esperada: N=%d Z=%d C=%d V=%d\n"
                "    flags recebida: N=%d Z=%d C=%d V=%d\n"
                "\n",

                ok ? "PASS" : "FAIL",

                i,
                test->name,

                test->opcode,

                test->r1,
                test->r2,
                test->r3,

                test->expected_r0,
                got_r0,

                test->N,
                test->Z,
                test->C,
                test->V,

                test->expected_N,
                test->expected_Z,
                test->expected_C,
                test->expected_V,

                got_N,
                got_Z,
                got_C,
                got_V
            );
        }


        // ----------------------------------------------------
        // SOURCE REGISTER CORRUPTION
        // ----------------------------------------------------

        if (!source_registers_ok) {

            printf(
                "    !!! SOURCE REGISTER ALTERADO !!!\n"
                "\n"
                "    R1 esperado: 0x%08X\n"
                "    R1 recebido: 0x%08X\n"
                "\n"
                "    R2 esperado: 0x%08X\n"
                "    R2 recebido: 0x%08X\n"
                "\n"
                "    R3 esperado: 0x%08X\n"
                "    R3 recebido: 0x%08X\n"
                "\n",

                test->r1,
                got_r1,

                test->r2,
                got_r2,

                test->r3,
                got_r3
            );
        }


        // ----------------------------------------------------
        // DETALHA O TIPO DE FALHA
        // ----------------------------------------------------

        if (!ok) {

            printf("    MOTIVO:");

            if (!result_ok) {
                printf(" [RESULT]");
            }

            if (!flags_ok) {
                printf(" [FLAGS]");
            }

            if (!source_registers_ok) {
                printf(" [SOURCE REGISTER]");
            }

            printf("\n\n");
            arm_test_pause_after_failure();
        }
    }


    // ========================================================
    // FINAL RESULT
    // ========================================================

    printf("=============================================================\n");
    printf("                   DATA PROCESSING TESTS\n");
    printf("=============================================================\n");
    printf("PASS:  %d |", passed);
    printf(" FAIL:  %d |", failed);
    printf(" TOTAL: %zu |\n", DP_TORTURE_TEST_COUNT);
    printf("=============================================================\n");
}



// ============================================================
//          CURRENT ARM IMPLEMENTATION TEST SUITE
//
// Cobertura SEMANTICA atual:
//   Data Processing:
//     AND EOR SUB RSB ADD ADC SBC RSC
//     TST TEQ CMP CMN ORR MOV BIC MVN
//
//   Multiply:
//     MUL MLA UMULL UMLAL SMULL SMLAL
//
// Todos os casos entram por instruction_processing(), portanto
// exercitam condition -> decoder -> handler -> resultado/flags.
//
// Para adicionar uma familia nova:
//   1) cria run_<familia>_tests()
//   2) adiciona uma chamada em run_arm_current_test_suite()
//
// PASS individual fica escondido por padrao.
// ============================================================


// ============================================================
// MULTIPLY ENCODERS
// ============================================================

#define ARM_TEST_MUL(COND, A, S, RD, RN, RS, RM) ( \
    (((word_type)(COND) & 0xFu) << 28) |            \
    (((word_type)(A)    & 0x1u) << 21) |            \
    (((word_type)(S)    & 0x1u) << 20) |            \
    (((word_type)(RD)   & 0xFu) << 16) |            \
    (((word_type)(RN)   & 0xFu) << 12) |            \
    (((word_type)(RS)   & 0xFu) << 8)  |            \
    0x00000090u |                                   \
    ((word_type)(RM) & 0xFu)                        \
)

#define ARM_TEST_MULL(COND, U, A, S, RDHI, RDLO, RS, RM) ( \
    (((word_type)(COND) & 0xFu) << 28) |                   \
    0x00800000u |                                          \
    (((word_type)(U)    & 0x1u) << 22) |                   \
    (((word_type)(A)    & 0x1u) << 21) |                   \
    (((word_type)(S)    & 0x1u) << 20) |                   \
    (((word_type)(RDHI) & 0xFu) << 16) |                   \
    (((word_type)(RDLO) & 0xFu) << 12) |                   \
    (((word_type)(RS)   & 0xFu) << 8)  |                   \
    0x00000090u |                                          \
    ((word_type)(RM) & 0xFu)                               \
)

typedef struct {
    int passed;
    int failed;
} arm_test_result_t;

static void arm_current_snapshot(word_type regs[15]) {
    for (byte_type i = 0; i < 15; i++) {
        regs[i] = read_register(i);
    }
}

static bool arm_current_other_regs_ok(
    const word_type before[15],
    const word_type after[15],
    int dst_a,
    int dst_b
) {
    for (int i = 0; i < 15; i++) {
        if (i == dst_a || i == dst_b) continue;
        if (before[i] != after[i]) return false;
    }
    return true;
}


// ============================================================
// MUL / MLA
// ============================================================

typedef struct {
    const char *name;
    word_type opcode;

    byte_type rd;
    byte_type rn;
    byte_type rs;
    byte_type rm;

    word_type rd_before;
    word_type rn_value;
    word_type rs_value;
    word_type rm_value;
    word_type expected_rd;

    bool N;
    bool Z;
    bool C;
    bool V;

    bool set_flags;
    bool should_execute;
} arm_mul_case_t;

static const arm_mul_case_t arm_mul_cases[] = {
    {
        "MUL basic 7*9",
        ARM_TEST_MUL(0xE, 0, 0, 4, 0, 2, 1),
        4, 0, 2, 1,
        0xDEADBEEF, 0, 9, 7,
        63,
        1, 0, 1, 1,
        0, 1
    },
    {
        "MUL truncation low32",
        ARM_TEST_MUL(0xE, 0, 0, 4, 0, 2, 1),
        4, 0, 2, 1,
        0xDEADBEEF, 0, 2, 0xFFFFFFFF,
        0xFFFFFFFE,
        0, 1, 0, 1,
        0, 1
    },
    {
        "MUL sem S preserves flags",
        ARM_TEST_MUL(0xE, 0, 0, 4, 0, 2, 1),
        4, 0, 2, 1,
        0xDEADBEEF, 0, 0, 0,
        0,
        1, 0, 1, 1,
        0, 1
    },
    {
        "MULS zero",
        ARM_TEST_MUL(0xE, 0, 1, 4, 0, 2, 1),
        4, 0, 2, 1,
        0xDEADBEEF, 0, 0x12345678, 0,
        0,
        1, 0, 0, 1,
        1, 1
    },
    {
        "MULS negative low32",
        ARM_TEST_MUL(0xE, 0, 1, 4, 0, 2, 1),
        4, 0, 2, 1,
        0xDEADBEEF, 0, 1, 0x80000000,
        0x80000000,
        0, 1, 1, 0,
        1, 1
    },
    {
        "MLA basic 7*9+5",
        ARM_TEST_MUL(0xE, 1, 0, 4, 3, 2, 1),
        4, 3, 2, 1,
        0xDEADBEEF, 5, 9, 7,
        68,
        1, 1, 0, 1,
        0, 1
    },
    {
        "MLA wraps to low32",
        ARM_TEST_MUL(0xE, 1, 0, 4, 3, 2, 1),
        4, 3, 2, 1,
        0xDEADBEEF, 5, 2, 0xFFFFFFFF,
        3,
        0, 1, 1, 0,
        0, 1
    },
    {
        "MLAS flags from final accumulated result",
        ARM_TEST_MUL(0xE, 1, 1, 4, 3, 2, 1),
        4, 3, 2, 1,
        0xDEADBEEF, 0xFFFFFFFE, 2, 1,
        0,
        1, 0, 0, 1,
        1, 1
    },
    {
        "MLA high register fields",
        ARM_TEST_MUL(0xE, 1, 0, 12, 10, 9, 7),
        12, 10, 9, 7,
        0xDEADBEEF, 5, 4, 3,
        17,
        0, 1, 1, 0,
        0, 1
    },
    {
        "MULEQ false condition changes nothing",
        ARM_TEST_MUL(0x0, 0, 0, 4, 0, 2, 1),
        4, 0, 2, 1,
        0xDEADBEEF, 0, 7, 6,
        0xDEADBEEF,
        1, 0, 1, 1,
        0, 0
    }
};

#define ARM_MUL_CASE_COUNT \
    (sizeof(arm_mul_cases) / sizeof(arm_mul_cases[0]))

static arm_test_result_t run_mul_mla_tests(void) {
    arm_test_result_t result = {0, 0};

    for (size_t i = 0; i < ARM_MUL_CASE_COUNT; i++) {
        const arm_mul_case_t *test = &arm_mul_cases[i];

        arm_test_reset_state();

        write_to_register(test->rd, test->rd_before);
        write_to_register(test->rm, test->rm_value);
        write_to_register(test->rs, test->rs_value);

        // Rn so e fonte real quando A=1. Os casos MLA usam R3/R10.
        if (test->rn != 0) {
            write_to_register(test->rn, test->rn_value);
        }

        arm_test_set_flags(test->N, test->Z, test->C, test->V);

        word_type before[15], after[15];
        arm_current_snapshot(before);

        instruction_processing(test->opcode);

        arm_current_snapshot(after);

        bool result_ok =
            after[test->rd] == test->expected_rd;

        bool other_regs_ok =
            arm_current_other_regs_ok(
                before,
                after,
                test->should_execute ? test->rd : -1,
                -1
            );

        bool got_N = is_flag_N_on();
        bool got_Z = is_flag_Z_on();
        bool got_C = is_flag_C_on();
        bool got_V = is_flag_V_on();

        bool flags_ok;

        if (!test->should_execute || !test->set_flags) {
            flags_ok =
                got_N == test->N &&
                got_Z == test->Z &&
                got_C == test->C &&
                got_V == test->V;
        }
        else {
            bool expected_N =
                (test->expected_rd & 0x80000000u) != 0;

            bool expected_Z =
                test->expected_rd == 0;

            // Em MULS/MLAS, o teste cobra N/Z.
            // C nao entra no criterio.
            // V deve permanecer preservado.
            flags_ok =
                got_N == expected_N &&
                got_Z == expected_Z &&
                got_V == test->V;
        }

        bool ok =
            result_ok &&
            other_regs_ok &&
            flags_ok;

        if (ok) result.passed++;
        else    result.failed++;

        if (!ok || ARM_TEST_PRINT_PASSES) {
            printf(
                "[%s][MUL/MLA] #%02zu - %s\n"
                "    opcode: 0x%08X\n"
                "    Rd esperado: 0x%08X\n"
                "    Rd recebido: 0x%08X\n"
                "    flags in:  N=%d Z=%d C=%d V=%d\n"
                "    flags out: N=%d Z=%d C=%d V=%d\n"
                "    outros registradores intactos: %s\n"
                "\n",
                ok ? "PASS" : "FAIL",
                i,
                test->name,
                test->opcode,
                test->expected_rd,
                after[test->rd],
                test->N, test->Z, test->C, test->V,
                got_N, got_Z, got_C, got_V,
                other_regs_ok ? "SIM" : "NAO"
            );
        }

        if (!ok) {
            arm_test_pause_after_failure();
        }
    }

    printf(
        "MUL/MLA: %d PASS | %d FAIL | %zu TOTAL\n",
        result.passed,
        result.failed,
        ARM_MUL_CASE_COUNT
    );

    return result;
}


// ============================================================
// UMULL / UMLAL / SMULL / SMLAL
// ============================================================

typedef struct {
    const char *name;
    word_type opcode;

    byte_type rd_hi;
    byte_type rd_lo;
    byte_type rs;
    byte_type rm;

    word_type hi_before;
    word_type lo_before;
    word_type rs_value;
    word_type rm_value;

    word_type expected_hi;
    word_type expected_lo;

    bool N;
    bool Z;
    bool C;
    bool V;

    bool set_flags;
    bool should_execute;
} arm_mull_case_t;

static const arm_mull_case_t arm_mull_cases[] = {
    {
        "UMULL FFFFFFFF*2",
        ARM_TEST_MULL(0xE, 0, 0, 0, 5, 4, 2, 1),
        5, 4, 2, 1,
        0, 0, 2, 0xFFFFFFFF,
        0x00000001, 0xFFFFFFFE,
        1, 0, 1, 1,
        0, 1
    },
    {
        "UMULL max*max",
        ARM_TEST_MULL(0xE, 0, 0, 0, 5, 4, 2, 1),
        5, 4, 2, 1,
        0, 0, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFE, 0x00000001,
        0, 1, 0, 1,
        0, 1
    },
    {
        "UMULLS zero",
        ARM_TEST_MULL(0xE, 0, 0, 1, 5, 4, 2, 1),
        5, 4, 2, 1,
        0, 0, 0xFFFFFFFF, 0,
        0, 0,
        1, 0, 1, 0,
        1, 1
    },
    {
        "UMULLS bit63 set",
        ARM_TEST_MULL(0xE, 0, 0, 1, 5, 4, 2, 1),
        5, 4, 2, 1,
        0, 0, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFE, 0x00000001,
        0, 1, 0, 1,
        1, 1
    },
    {
        "UMLAL carry low->high",
        ARM_TEST_MULL(0xE, 0, 1, 0, 5, 4, 2, 1),
        5, 4, 2, 1,
        0x00000000, 15, 1, 0xFFFFFFF1,
        0x00000001, 0x00000000,
        1, 1, 0, 1,
        0, 1
    },
    {
        "UMLAL keeps 64-bit accumulator",
        ARM_TEST_MULL(0xE, 0, 1, 0, 5, 4, 2, 1),
        5, 4, 2, 1,
        0x11223344, 2, 4, 3,
        0x11223344, 14,
        0, 1, 1, 0,
        0, 1
    },
    {
        "SMULL -1*2",
        ARM_TEST_MULL(0xE, 1, 0, 0, 5, 4, 2, 1),
        5, 4, 2, 1,
        0, 0, 2, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFE,
        1, 0, 1, 1,
        0, 1
    },
    {
        "SMULL -1*-1",
        ARM_TEST_MULL(0xE, 1, 0, 0, 5, 4, 2, 1),
        5, 4, 2, 1,
        0, 0, 0xFFFFFFFF, 0xFFFFFFFF,
        0x00000000, 0x00000001,
        1, 1, 0, 0,
        0, 1
    },
    {
        "SMULL INT_MIN*2",
        ARM_TEST_MULL(0xE, 1, 0, 0, 5, 4, 2, 1),
        5, 4, 2, 1,
        0, 0, 2, 0x80000000,
        0xFFFFFFFF, 0x00000000,
        0, 1, 0, 1,
        0, 1
    },
    {
        "SMLAL signed accumulate",
        ARM_TEST_MULL(0xE, 1, 1, 0, 5, 4, 2, 1),
        5, 4, 2, 1,
        0, 10, 3, 0xFFFFFFFE,
        0x00000000, 4,
        1, 0, 1, 1,
        0, 1
    },
    {
        "SMLALS cancellation to zero",
        ARM_TEST_MULL(0xE, 1, 1, 1, 5, 4, 2, 1),
        5, 4, 2, 1,
        0, 1, 1, 0xFFFFFFFF,
        0, 0,
        1, 0, 0, 1,
        1, 1
    },
    {
        "SMULL high register fields",
        ARM_TEST_MULL(0xE, 1, 0, 0, 12, 10, 9, 7),
        12, 10, 9, 7,
        0, 0, 3, 0xFFFFFFFE,
        0xFFFFFFFF, 0xFFFFFFFA,
        0, 1, 1, 0,
        0, 1
    },
    {
        "UMULLEQ false condition changes nothing",
        ARM_TEST_MULL(0x0, 0, 0, 0, 5, 4, 2, 1),
        5, 4, 2, 1,
        0x11223344, 7, 6, 5,
        0x11223344, 7,
        1, 0, 1, 1,
        0, 0
    }
};

#define ARM_MULL_CASE_COUNT \
    (sizeof(arm_mull_cases) / sizeof(arm_mull_cases[0]))

static arm_test_result_t run_multiply_long_tests(void) {
    arm_test_result_t result = {0, 0};

    for (size_t i = 0; i < ARM_MULL_CASE_COUNT; i++) {
        const arm_mull_case_t *test = &arm_mull_cases[i];

        arm_test_reset_state();

        write_to_register(test->rd_hi, test->hi_before);
        write_to_register(test->rd_lo, test->lo_before);
        write_to_register(test->rs, test->rs_value);
        write_to_register(test->rm, test->rm_value);

        arm_test_set_flags(test->N, test->Z, test->C, test->V);

        word_type before[15], after[15];
        arm_current_snapshot(before);

        instruction_processing(test->opcode);

        arm_current_snapshot(after);

        bool result_ok =
            after[test->rd_hi] == test->expected_hi &&
            after[test->rd_lo] == test->expected_lo;

        bool other_regs_ok =
            arm_current_other_regs_ok(
                before,
                after,
                test->should_execute ? test->rd_hi : -1,
                test->should_execute ? test->rd_lo : -1
            );

        bool got_N = is_flag_N_on();
        bool got_Z = is_flag_Z_on();
        bool got_C = is_flag_C_on();
        bool got_V = is_flag_V_on();

        bool flags_ok;

        if (!test->should_execute || !test->set_flags) {
            flags_ok =
                got_N == test->N &&
                got_Z == test->Z &&
                got_C == test->C &&
                got_V == test->V;
        }
        else {
            bool expected_N =
                (test->expected_hi & 0x80000000u) != 0;

            bool expected_Z =
                test->expected_hi == 0 &&
                test->expected_lo == 0;

            // Long multiply com S: o teste cobra N/Z.
            // C/V nao entram no criterio.
            flags_ok =
                got_N == expected_N &&
                got_Z == expected_Z;
        }

        bool ok =
            result_ok &&
            other_regs_ok &&
            flags_ok;

        if (ok) result.passed++;
        else    result.failed++;

        if (!ok || ARM_TEST_PRINT_PASSES) {
            printf(
                "[%s][LONG MUL] #%02zu - %s\n"
                "    opcode: 0x%08X\n"
                "    esperado: 0x%08X%08X\n"
                "    recebido: 0x%08X%08X\n"
                "    flags in:  N=%d Z=%d C=%d V=%d\n"
                "    flags out: N=%d Z=%d C=%d V=%d\n"
                "    outros registradores intactos: %s\n"
                "\n",
                ok ? "PASS" : "FAIL",
                i,
                test->name,
                test->opcode,
                test->expected_hi,
                test->expected_lo,
                after[test->rd_hi],
                after[test->rd_lo],
                test->N, test->Z, test->C, test->V,
                got_N, got_Z, got_C, got_V,
                other_regs_ok ? "SIM" : "NAO"
            );
        }

        if (!ok) {
            arm_test_pause_after_failure();
        }
    }

    printf(
        "LONG MUL: %d PASS | %d FAIL | %zu TOTAL\n",
        result.passed,
        result.failed,
        ARM_MULL_CASE_COUNT
    );

    return result;
}


// ============================================================
// PUBLIC FAMILY RUNNERS
// ============================================================

void run_data_processing_tests(void) {
    run_data_processing_torture_tests();
}

void run_multiply_tests(void) {
    printf("\n");
    printf("=============================================================\n");
    printf("                      MULTIPLY TESTS\n");
    printf("=============================================================\n");

    arm_test_result_t common = run_mul_mla_tests();
    arm_test_result_t long_mul = run_multiply_long_tests();

    printf("-------------------------------------------------------------\n");
    printf(
        "MULTIPLY: %d PASS | %d FAIL | %d TOTAL\n",
        common.passed + long_mul.passed,
        common.failed + long_mul.failed,
        common.passed + common.failed +
        long_mul.passed + long_mul.failed
    );
    printf("=============================================================\n");
}



// ============================================================
//              SINGLE DATA TRANSFER TESTS
//
// IMPORTANTE SOBRE O HARNESS:
//   write_to_memory_via_databus(address, value)
//
// O harness valida o próprio setup da memória ANTES de executar
// cada opcode. Se aparecer [SDT HARNESS] / [HSDT HARNESS],
// a instrução ARM nem chegou a rodar.
//
//
// Testa semanticamente:
//   STR / LDR / STRB / LDRB
//
// Caminho real:
//   instruction_processing()
//     -> arm_instruction_bus()
//     -> handler
//
// Cobre:
//   immediate offset12
//   register offset
//   LSL / LSR / ASR / ROR
//   pre-index / post-index
//   U add/sub
//   write-back
//   word / byte
//   load / store
//   flags preservadas
//   condição falsa
//   STRB = só low byte
//   LDRB = zero-extension
//
// Este bloco cobre a família SDT normal.
// Halfword/signed e word desalinhado são testados em blocos
// separados logo abaixo. R15 e SWP/SWPB ficam para depois.
// ============================================================

#define ARM_SDT_IMM(COND,P,U,B,W,L,RN,RD,IMM12) ( \
    (((word_type)(COND) & 0xFu) << 28) |           \
    0b00000100000000000000000000000000u |          \
    (((word_type)(P) & 1u) << 24) |                \
    (((word_type)(U) & 1u) << 23) |                \
    (((word_type)(B) & 1u) << 22) |                \
    (((word_type)(W) & 1u) << 21) |                \
    (((word_type)(L) & 1u) << 20) |                \
    (((word_type)(RN) & 0xFu) << 16) |             \
    (((word_type)(RD) & 0xFu) << 12) |             \
    ((word_type)(IMM12) & 0xFFFu)                  \
)

#define ARM_SDT_REG(COND,P,U,B,W,L,RN,RD,RM,TYPE,SHIFT) ( \
    (((word_type)(COND) & 0xFu) << 28) |                  \
    0b00000110000000000000000000000000u |                 \
    (((word_type)(P) & 1u) << 24) |                       \
    (((word_type)(U) & 1u) << 23) |                       \
    (((word_type)(B) & 1u) << 22) |                       \
    (((word_type)(W) & 1u) << 21) |                       \
    (((word_type)(L) & 1u) << 20) |                       \
    (((word_type)(RN) & 0xFu) << 16) |                    \
    (((word_type)(RD) & 0xFu) << 12) |                    \
    (((word_type)(SHIFT) & 0x1Fu) << 7) |                 \
    (((word_type)(TYPE) & 0x3u) << 5) |                   \
    ((word_type)(RM) & 0xFu)                              \
)

enum {
    SDT_LSL = 0b00,
    SDT_LSR = 0b01,
    SDT_ASR = 0b10,
    SDT_ROR = 0b11
};

#define SDT_TEST_BASE    0x02010000u
#define SDT_SLOT(N)      (SDT_TEST_BASE + ((word_type)(N) * 0x100u))
#define SDT_GUARD_BEFORE 0xA5A5A5A5u
#define SDT_GUARD_AFTER  0x5A5A5A5Au

typedef struct {
    const char *name;
    word_type opcode;

    // R0 = Rd, R1 = Rn, R2 = Rm
    word_type r0_before;
    word_type r1_before;
    word_type r2_before;

    word_type expected_r0;
    word_type expected_r1;
    word_type expected_r2;

    // Palavra alinhada usada para preparar/inspecionar memória.
    word_type memory_anchor;
    word_type memory_before;
    word_type memory_expected;

    bool condition_false;
} sdt_test_case_t;

static const sdt_test_case_t sdt_tests[] = {

    // ---------------- WORD / IMMEDIATE ----------------

    {
        "STR immediate offset 0",
        ARM_SDT_IMM(0xE,1,1,0,0,0,1,0,0x000),
        0x11223344, SDT_SLOT(0), 0,
        0x11223344, SDT_SLOT(0), 0,
        SDT_SLOT(0), 0xDEADBEEF, 0x11223344,
        false
    },

    {
        "LDR immediate offset 0",
        ARM_SDT_IMM(0xE,1,1,0,0,1,1,0,0x000),
        0xDEADBEEF, SDT_SLOT(1), 0,
        0x12345678, SDT_SLOT(1), 0,
        SDT_SLOT(1), 0x12345678, 0x12345678,
        false
    },

    {
        "STR pre +0x10 sem writeback",
        ARM_SDT_IMM(0xE,1,1,0,0,0,1,0,0x010),
        0xCAFEBABE, SDT_SLOT(2), 0,
        0xCAFEBABE, SDT_SLOT(2), 0,
        SDT_SLOT(2)+0x10, 0x0BADF00D, 0xCAFEBABE,
        false
    },

    {
        "LDR pre -0x20 sem writeback",
        ARM_SDT_IMM(0xE,1,0,0,0,1,1,0,0x020),
        0xDEADBEEF, SDT_SLOT(3)+0x20, 0,
        0x89ABCDEF, SDT_SLOT(3)+0x20, 0,
        SDT_SLOT(3), 0x89ABCDEF, 0x89ABCDEF,
        false
    },

    {
        "STR pre +0x10 com writeback",
        ARM_SDT_IMM(0xE,1,1,0,1,0,1,0,0x010),
        0x13579BDF, SDT_SLOT(4), 0,
        0x13579BDF, SDT_SLOT(4)+0x10, 0,
        SDT_SLOT(4)+0x10, 0xAAAAAAAA, 0x13579BDF,
        false
    },

    {
        "LDR pre -0x20 com writeback",
        ARM_SDT_IMM(0xE,1,0,0,1,1,1,0,0x020),
        0xDEADBEEF, SDT_SLOT(5)+0x20, 0,
        0x2468ACE0, SDT_SLOT(5), 0,
        SDT_SLOT(5), 0x2468ACE0, 0x2468ACE0,
        false
    },

    {
        "STR post +0x10 atualiza Rn",
        ARM_SDT_IMM(0xE,0,1,0,0,0,1,0,0x010),
        0x10203040, SDT_SLOT(6), 0,
        0x10203040, SDT_SLOT(6)+0x10, 0,
        SDT_SLOT(6), 0xFFFFFFFF, 0x10203040,
        false
    },

    {
        "LDR post -0x10 atualiza Rn",
        ARM_SDT_IMM(0xE,0,0,0,0,1,1,0,0x010),
        0xDEADBEEF, SDT_SLOT(7), 0,
        0x55667788, SDT_SLOT(7)-0x10, 0,
        SDT_SLOT(7), 0x55667788, 0x55667788,
        false
    },

    {
        "LDR usa os 12 bits do immediate",
        ARM_SDT_IMM(0xE,1,1,0,0,1,1,0,0xABC),
        0xDEADBEEF, SDT_SLOT(8), 0,
        0xA1B2C3D4, SDT_SLOT(8), 0,
        SDT_SLOT(8)+0xABC, 0xA1B2C3D4, 0xA1B2C3D4,
        false
    },

    {
        "STREQ false nao executa",
        ARM_SDT_IMM(0x0,1,1,0,1,0,1,0,0x010),
        0xFACEB00C, SDT_SLOT(9), 0,
        0xFACEB00C, SDT_SLOT(9), 0,
        SDT_SLOT(9)+0x10, 0x01020304, 0x01020304,
        true
    },

    // ---------------- WORD / REGISTER ----------------

    {
        "LDR register sem shift",
        ARM_SDT_REG(0xE,1,1,0,0,1,1,0,2,SDT_LSL,0),
        0xDEADBEEF, SDT_SLOT(10), 0x10,
        0x0F1E2D3C, SDT_SLOT(10), 0x10,
        SDT_SLOT(10)+0x10, 0x0F1E2D3C, 0x0F1E2D3C,
        false
    },

    {
        "STR register negativo sem shift",
        ARM_SDT_REG(0xE,1,0,0,0,0,1,0,2,SDT_LSL,0),
        0x76543210, SDT_SLOT(11)+0x20, 0x20,
        0x76543210, SDT_SLOT(11)+0x20, 0x20,
        SDT_SLOT(11), 0xDEADC0DE, 0x76543210,
        false
    },

    {
        "LDR register LSL #1 (bit7 variavel)",
        ARM_SDT_REG(0xE,1,1,0,0,1,1,0,2,SDT_LSL,1),
        0xDEADBEEF, SDT_SLOT(12), 0x08,
        0x1122AABB, SDT_SLOT(12), 0x08,
        SDT_SLOT(12)+0x10, 0x1122AABB, 0x1122AABB,
        false
    },

    {
        "STR register LSR #1",
        ARM_SDT_REG(0xE,1,1,0,0,0,1,0,2,SDT_LSR,1),
        0xABCDEF01, SDT_SLOT(13), 0x20,
        0xABCDEF01, SDT_SLOT(13), 0x20,
        SDT_SLOT(13)+0x10, 0x13572468, 0xABCDEF01,
        false
    },

    {
        "LDR register ASR #1",
        ARM_SDT_REG(0xE,1,1,0,0,1,1,0,2,SDT_ASR,1),
        0xDEADBEEF, SDT_SLOT(14), 0x20,
        0x31415926, SDT_SLOT(14), 0x20,
        SDT_SLOT(14)+0x10, 0x31415926, 0x31415926,
        false
    },

    {
        "STR register ROR #4",
        ARM_SDT_REG(0xE,1,1,0,0,0,1,0,2,SDT_ROR,4),
        0x27182818, SDT_SLOT(15), 0x00000100,
        0x27182818, SDT_SLOT(15), 0x00000100,
        SDT_SLOT(15)+0x10, 0xAAAAAAAA, 0x27182818,
        false
    },

    {
        "LDR register LSL #2 com writeback",
        ARM_SDT_REG(0xE,1,1,0,1,1,1,0,2,SDT_LSL,2),
        0xDEADBEEF, SDT_SLOT(16), 0x04,
        0xFEEDC0DE, SDT_SLOT(16)+0x10, 0x04,
        SDT_SLOT(16)+0x10, 0xFEEDC0DE, 0xFEEDC0DE,
        false
    },

    {
        "STR register post-index faz writeback",
        ARM_SDT_REG(0xE,0,1,0,0,0,1,0,2,SDT_LSL,0),
        0x44332211, SDT_SLOT(17), 0x10,
        0x44332211, SDT_SLOT(17)+0x10, 0x10,
        SDT_SLOT(17), 0x01010101, 0x44332211,
        false
    },

    // ---------------- BYTE / IMMEDIATE ----------------
    //
    // GBA little-endian:
    // AABBCCDD ->
    // +0 DD, +1 CC, +2 BB, +3 AA

    {
        "STRB grava somente low byte",
        ARM_SDT_IMM(0xE,1,1,1,0,0,1,0,0),
        0x12345678, SDT_SLOT(18), 0,
        0x12345678, SDT_SLOT(18), 0,
        SDT_SLOT(18), 0xAABBCCDD, 0xAABBCC78,
        false
    },

    {
        "LDRB zero-extend",
        ARM_SDT_IMM(0xE,1,1,1,0,1,1,0,0),
        0xDEADBEEF, SDT_SLOT(19), 0,
        0x000000DD, SDT_SLOT(19), 0,
        SDT_SLOT(19), 0xAABBCCDD, 0xAABBCCDD,
        false
    },

    {
        "STRB pre +1 preserva vizinhos",
        ARM_SDT_IMM(0xE,1,1,1,0,0,1,0,1),
        0x11223344, SDT_SLOT(20), 0,
        0x11223344, SDT_SLOT(20), 0,
        SDT_SLOT(20), 0xAABBCCDD, 0xAABB44DD,
        false
    },

    {
        "LDRB pre +2 pega terceiro byte",
        ARM_SDT_IMM(0xE,1,1,1,0,1,1,0,2),
        0xDEADBEEF, SDT_SLOT(21), 0,
        0x000000BB, SDT_SLOT(21), 0,
        SDT_SLOT(21), 0xAABBCCDD, 0xAABBCCDD,
        false
    },

    {
        "STRB pre +3 com writeback",
        ARM_SDT_IMM(0xE,1,1,1,1,0,1,0,3),
        0x00000011, SDT_SLOT(22), 0,
        0x00000011, SDT_SLOT(22)+3, 0,
        SDT_SLOT(22), 0xAABBCCDD, 0x11BBCCDD,
        false
    },

    {
        "LDRB post +1 atualiza Rn",
        ARM_SDT_IMM(0xE,0,1,1,0,1,1,0,1),
        0xDEADBEEF, SDT_SLOT(23), 0,
        0x000000DD, SDT_SLOT(23)+1, 0,
        SDT_SLOT(23), 0xAABBCCDD, 0xAABBCCDD,
        false
    },

    // ---------------- BYTE / REGISTER ----------------

    {
        "LDRB register LSL #1",
        ARM_SDT_REG(0xE,1,1,1,0,1,1,0,2,SDT_LSL,1),
        0xDEADBEEF, SDT_SLOT(24), 1,
        0x000000BB, SDT_SLOT(24), 1,
        SDT_SLOT(24), 0xAABBCCDD, 0xAABBCCDD,
        false
    },

    {
        "STRB register LSL #1",
        ARM_SDT_REG(0xE,1,1,1,0,0,1,0,2,SDT_LSL,1),
        0x00000066, SDT_SLOT(25), 1,
        0x00000066, SDT_SLOT(25), 1,
        SDT_SLOT(25), 0xAABBCCDD, 0xAA66CCDD,
        false
    },

    {
        "LDRBEQ false nao executa",
        ARM_SDT_IMM(0x0,1,1,1,1,1,1,0,1),
        0xDEADBEEF, SDT_SLOT(26), 0,
        0xDEADBEEF, SDT_SLOT(26), 0,
        SDT_SLOT(26), 0xAABBCCDD, 0xAABBCCDD,
        true
    }
};

#define SDT_TEST_COUNT (sizeof(sdt_tests) / sizeof(sdt_tests[0]))

void run_single_transfer_tests(void) {
    int passed = 0;
    int failed = 0;

    printf("\n");
    printf("=============================================================\n");
    printf("                SINGLE DATA TRANSFER TESTS\n");
    printf("=============================================================\n");

    for (size_t i = 0; i < SDT_TEST_COUNT; i++) {
        const sdt_test_case_t *test = &sdt_tests[i];

        arm_test_reset_state();

        bool input_N = (i & 0x1u) != 0;
        bool input_Z = (i & 0x2u) != 0;
        bool input_C = (i & 0x4u) != 0;
        bool input_V = (i & 0x8u) != 0;

        if (test->condition_false) {
            input_N = true;
            input_Z = false; // EQ deve falhar
            input_C = true;
            input_V = true;
        }

        arm_test_set_flags(input_N, input_Z, input_C, input_V);

        write_to_register(0, test->r0_before);
        write_to_register(1, test->r1_before);
        write_to_register(2, test->r2_before);

        /*
         * MEMORY SETUP DO HARNESS
         *
         * API do projeto:
         *   write_to_memory_via_databus(address, value)
         *
         * Antes de executar a instrução, o próprio harness confirma
         * que conseguiu preparar a memória. Assim um problema no
         * setup não aparece falsamente como bug de LDR/STR.
         */
        write_to_memory_via_databus(
            test->memory_anchor - 4,
            SDT_GUARD_BEFORE
        );

        write_to_memory_via_databus(
            test->memory_anchor,
            test->memory_before
        );

        write_to_memory_via_databus(
            test->memory_anchor + 4,
            SDT_GUARD_AFTER
        );

        bool memory_setup_ok =
            read_from_memory_via_databus(test->memory_anchor - 4) == SDT_GUARD_BEFORE &&
            read_from_memory_via_databus(test->memory_anchor)     == test->memory_before &&
            read_from_memory_via_databus(test->memory_anchor + 4) == SDT_GUARD_AFTER;

        if (!memory_setup_ok) {
            failed++;

            printf(
                "[FAIL][SDT HARNESS] #%02zu - %s\n"
                "    O teste NAO executou a instrucao.\n"
                "    Falha preparando a memoria antes do opcode.\n"
                "    anchor: 0x%08X\n"
                "    esperado no anchor: 0x%08X\n"
                "    recebido no anchor: 0x%08X\n\n",
                i,
                test->name,
                test->memory_anchor,
                test->memory_before,
                read_from_memory_via_databus(test->memory_anchor)
            );

            arm_test_pause_after_failure();
            continue;
        }

        instruction_processing(test->opcode);

        word_type got_r0 = read_register(0);
        word_type got_r1 = read_register(1);
        word_type got_r2 = read_register(2);

        word_type got_memory =
            read_from_memory_via_databus(test->memory_anchor);

        word_type got_guard_before =
            read_from_memory_via_databus(test->memory_anchor - 4);

        word_type got_guard_after =
            read_from_memory_via_databus(test->memory_anchor + 4);

        bool got_N = is_flag_N_on();
        bool got_Z = is_flag_Z_on();
        bool got_C = is_flag_C_on();
        bool got_V = is_flag_V_on();

        bool rd_ok = got_r0 == test->expected_r0;
        bool rn_ok = got_r1 == test->expected_r1;
        bool rm_ok = got_r2 == test->expected_r2;
        bool memory_ok = got_memory == test->memory_expected;

        bool guards_ok =
            got_guard_before == SDT_GUARD_BEFORE &&
            got_guard_after == SDT_GUARD_AFTER;

        bool flags_ok =
            got_N == input_N &&
            got_Z == input_Z &&
            got_C == input_C &&
            got_V == input_V;

        bool ok =
            rd_ok &&
            rn_ok &&
            rm_ok &&
            memory_ok &&
            guards_ok &&
            flags_ok;

        if (ok) passed++;
        else failed++;

        if (!ok || ARM_TEST_PRINT_PASSES) {
            printf(
                "[%s][SDT] #%02zu - %s\n"
                "    opcode: 0x%08X\n"
                "\n"
                "    R0 esperado: 0x%08X | recebido: 0x%08X\n"
                "    R1 esperado: 0x%08X | recebido: 0x%08X\n"
                "    R2 esperado: 0x%08X | recebido: 0x%08X\n"
                "\n"
                "    memoria @ 0x%08X\n"
                "    esperado: 0x%08X | recebido: 0x%08X\n"
                "\n"
                "    flags entrada: N=%d Z=%d C=%d V=%d\n"
                "    flags saida:   N=%d Z=%d C=%d V=%d\n"
                "    guards: before=%s after=%s\n",
                ok ? "PASS" : "FAIL",
                i,
                test->name,
                test->opcode,
                test->expected_r0, got_r0,
                test->expected_r1, got_r1,
                test->expected_r2, got_r2,
                test->memory_anchor,
                test->memory_expected, got_memory,
                input_N, input_Z, input_C, input_V,
                got_N, got_Z, got_C, got_V,
                got_guard_before == SDT_GUARD_BEFORE ? "OK" : "ALTERADO",
                got_guard_after == SDT_GUARD_AFTER ? "OK" : "ALTERADO"
            );

            if (!ok) {
                printf("    MOTIVO:");
                if (!rd_ok) printf(" [RD]");
                if (!rn_ok) printf(" [RN/WRITEBACK]");
                if (!rm_ok) printf(" [RM]");
                if (!memory_ok) printf(" [MEMORY]");
                if (!guards_ok) printf(" [MEMORY GUARD]");
                if (!flags_ok) printf(" [FLAGS]");
                printf("\n\n");
                arm_test_pause_after_failure();
            }
        }
    }

    printf("-------------------------------------------------------------\n");
    printf(
        "SINGLE DATA TRANSFER: %d PASS | %d FAIL | %zu TOTAL\n",
        passed,
        failed,
        SDT_TEST_COUNT
    );
    printf("=============================================================\n");
}




// ============================================================
//          HALFWORD + SIGNED TRANSFER TESTS
//
// Cobre:
//   STRH / LDRH
//   LDRSB
//   LDRSH
//
// Variantes:
//   - immediate offset de 8 bits (split 11:8 + 3:0)
//   - register offset
//   - P/U/W
//   - post-index write-back
//   - zero-extension de LDRH
//   - sign-extension de LDRSB/LDRSH
//   - condição falsa
//
// Não testa halfword desalinhado (bit0=1), porque no ARM7TDMI
// esse caso é arquiteturalmente UNPREDICTABLE.
// ============================================================


// ------------------------------------------------------------
// Halfword / signed transfer - REGISTER OFFSET
//
// Cond | 000 | P | U | 0 | W | L | Rn | Rd |
//        0000 | 1 | S | H | 1 | Rm
// ------------------------------------------------------------

#define ARM_HSDT_REG(COND, P, U, W, L, RN, RD, S, H, RM) ( \
    (((word_type)(COND) & 0xFu) << 28) |                    \
    (((word_type)(P)    & 0x1u) << 24) |                    \
    (((word_type)(U)    & 0x1u) << 23) |                    \
    (((word_type)(W)    & 0x1u) << 21) |                    \
    (((word_type)(L)    & 0x1u) << 20) |                    \
    (((word_type)(RN)   & 0xFu) << 16) |                    \
    (((word_type)(RD)   & 0xFu) << 12) |                    \
    (1u << 7) |                                             \
    (((word_type)(S)    & 0x1u) << 6) |                     \
    (((word_type)(H)    & 0x1u) << 5) |                     \
    (1u << 4) |                                             \
    ((word_type)(RM) & 0xFu)                                \
)


// ------------------------------------------------------------
// Halfword / signed transfer - IMMEDIATE OFFSET
//
// Cond | 000 | P | U | 1 | W | L | Rn | Rd |
//        offHi | 1 | S | H | 1 | offLo
//
// IMM8 é quebrado automaticamente entre 11:8 e 3:0.
// ------------------------------------------------------------

#define ARM_HSDT_IMM(COND, P, U, W, L, RN, RD, S, H, IMM8) ( \
    (((word_type)(COND) & 0xFu) << 28) |                      \
    (((word_type)(P)    & 0x1u) << 24) |                      \
    (((word_type)(U)    & 0x1u) << 23) |                      \
    (1u << 22) |                                              \
    (((word_type)(W)    & 0x1u) << 21) |                      \
    (((word_type)(L)    & 0x1u) << 20) |                      \
    (((word_type)(RN)   & 0xFu) << 16) |                      \
    (((word_type)(RD)   & 0xFu) << 12) |                      \
    ((((word_type)(IMM8) >> 4) & 0xFu) << 8) |                \
    (1u << 7) |                                               \
    (((word_type)(S)    & 0x1u) << 6) |                       \
    (((word_type)(H)    & 0x1u) << 5) |                       \
    (1u << 4) |                                               \
    ((word_type)(IMM8) & 0xFu)                                \
)


typedef struct {
    const char *name;
    word_type opcode;

    word_type r0_before;
    word_type r1_before;
    word_type r2_before;

    word_type expected_r0;
    word_type expected_r1;
    word_type expected_r2;

    /*
     * Sempre guardamos uma WORD alinhada como backing do teste.
     * Assim conseguimos conferir se STRH preservou os 16 bits
     * vizinhos e escolher bytes/halfwords específicos para loads.
     */
    word_type memory_anchor;
    word_type memory_before;
    word_type memory_expected;

    bool condition_false;

} hsdt_test_case_t;


static const hsdt_test_case_t hsdt_tests[] = {

    // ========================================================
    // UNSIGNED HALFWORD - IMMEDIATE
    // ========================================================

    {
        "STRH immediate offset 0 preserva halfword vizinho",
        ARM_HSDT_IMM(0xE, 1, 1, 0, 0, 1, 0, 0, 1, 0x00),

        0x12345678,
        SDT_SLOT(30),
        0,

        0x12345678,
        SDT_SLOT(30),
        0,

        SDT_SLOT(30),
        0xAABBCCDD,
        0xAABB5678,

        false
    },

    {
        "LDRH immediate offset 0 faz zero-extension",
        ARM_HSDT_IMM(0xE, 1, 1, 0, 1, 1, 0, 0, 1, 0x00),

        0xDEADBEEF,
        SDT_SLOT(31),
        0,

        0x0000CCDD,
        SDT_SLOT(31),
        0,

        SDT_SLOT(31),
        0xAABBCCDD,
        0xAABBCCDD,

        false
    },

    {
        "STRH immediate +2 escreve metade alta da word",
        ARM_HSDT_IMM(0xE, 1, 1, 0, 0, 1, 0, 0, 1, 0x02),

        0x12345678,
        SDT_SLOT(32),
        0,

        0x12345678,
        SDT_SLOT(32),
        0,

        SDT_SLOT(32),
        0xAABBCCDD,
        0x5678CCDD,

        false
    },

    {
        "LDRH immediate +2 seleciona metade alta",
        ARM_HSDT_IMM(0xE, 1, 1, 0, 1, 1, 0, 0, 1, 0x02),

        0xDEADBEEF,
        SDT_SLOT(33),
        0,

        0x0000AABB,
        SDT_SLOT(33),
        0,

        SDT_SLOT(33),
        0xAABBCCDD,
        0xAABBCCDD,

        false
    },

    {
        "LDRH immediate +0xA2 usa os 8 bits inteiros do offset",
        ARM_HSDT_IMM(0xE, 1, 1, 0, 1, 1, 0, 0, 1, 0xA2),

        0xDEADBEEF,
        SDT_SLOT(34),
        0,

        0x0000BEEF,
        SDT_SLOT(34),
        0,

        SDT_SLOT(34) + 0xA0,
        0xBEEF1234,
        0xBEEF1234,

        false
    },

    {
        "LDRH pre-index +2 com writeback",
        ARM_HSDT_IMM(0xE, 1, 1, 1, 1, 1, 0, 0, 1, 0x02),

        0xDEADBEEF,
        SDT_SLOT(35),
        0,

        0x0000CAFE,
        SDT_SLOT(35) + 2,
        0,

        SDT_SLOT(35),
        0xCAFE1234,
        0xCAFE1234,

        false
    },

    {
        "STRH post-index +2 sempre atualiza Rn",
        ARM_HSDT_IMM(0xE, 0, 1, 0, 0, 1, 0, 0, 1, 0x02),

        0x00001357,
        SDT_SLOT(36),
        0,

        0x00001357,
        SDT_SLOT(36) + 2,
        0,

        SDT_SLOT(36),
        0xAABBCCDD,
        0xAABB1357,

        false
    },


    // ========================================================
    // UNSIGNED HALFWORD - REGISTER OFFSET
    // ========================================================

    {
        "LDRH register offset +2",
        ARM_HSDT_REG(0xE, 1, 1, 0, 1, 1, 0, 0, 1, 2),

        0xDEADBEEF,
        SDT_SLOT(37),
        2,

        0x0000AABB,
        SDT_SLOT(37),
        2,

        SDT_SLOT(37),
        0xAABBCCDD,
        0xAABBCCDD,

        false
    },

    {
        "STRH register offset -2",
        ARM_HSDT_REG(0xE, 1, 0, 0, 0, 1, 0, 0, 1, 2),

        0x00002468,
        SDT_SLOT(38) + 2,
        2,

        0x00002468,
        SDT_SLOT(38) + 2,
        2,

        SDT_SLOT(38),
        0xAABBCCDD,
        0xAABB2468,

        false
    },


    // ========================================================
    // SIGNED BYTE
    // ========================================================

    {
        "LDRSB immediate positivo +0",
        ARM_HSDT_IMM(0xE, 1, 1, 0, 1, 1, 0, 1, 0, 0x00),

        0xDEADBEEF,
        SDT_SLOT(39),
        0,

        0x0000007F,
        SDT_SLOT(39),
        0,

        SDT_SLOT(39),
        0x1122337F,
        0x1122337F,

        false
    },

    {
        "LDRSB immediate negativo +1 faz sign-extension",
        ARM_HSDT_IMM(0xE, 1, 1, 0, 1, 1, 0, 1, 0, 0x01),

        0xDEADBEEF,
        SDT_SLOT(40),
        0,

        0xFFFFFF80,
        SDT_SLOT(40),
        0,

        SDT_SLOT(40),
        0x1122807F,
        0x1122807F,

        false
    },

    {
        "LDRSB immediate +0xA1 usa offset inteiro",
        ARM_HSDT_IMM(0xE, 1, 1, 0, 1, 1, 0, 1, 0, 0xA1),

        0xDEADBEEF,
        SDT_SLOT(41),
        0,

        0xFFFFFF80,
        SDT_SLOT(41),
        0,

        SDT_SLOT(41) + 0xA0,
        0x1122807F,
        0x1122807F,

        false
    },

    {
        "LDRSB register offset +3",
        ARM_HSDT_REG(0xE, 1, 1, 0, 1, 1, 0, 1, 0, 2),

        0xDEADBEEF,
        SDT_SLOT(42),
        3,

        0xFFFFFFAA,
        SDT_SLOT(42),
        3,

        SDT_SLOT(42),
        0xAABBCCDD,
        0xAABBCCDD,

        false
    },


    // ========================================================
    // SIGNED HALFWORD
    // ========================================================

    {
        "LDRSH immediate positivo +0",
        ARM_HSDT_IMM(0xE, 1, 1, 0, 1, 1, 0, 1, 1, 0x00),

        0xDEADBEEF,
        SDT_SLOT(43),
        0,

        0x00007FFF,
        SDT_SLOT(43),
        0,

        SDT_SLOT(43),
        0x80017FFF,
        0x80017FFF,

        false
    },

    {
        "LDRSH immediate negativo +2 faz sign-extension",
        ARM_HSDT_IMM(0xE, 1, 1, 0, 1, 1, 0, 1, 1, 0x02),

        0xDEADBEEF,
        SDT_SLOT(44),
        0,

        0xFFFF8001,
        SDT_SLOT(44),
        0,

        SDT_SLOT(44),
        0x80017FFF,
        0x80017FFF,

        false
    },

    {
        "LDRSH register offset +2",
        ARM_HSDT_REG(0xE, 1, 1, 0, 1, 1, 0, 1, 1, 2),

        0xDEADBEEF,
        SDT_SLOT(45),
        2,

        0xFFFF8001,
        SDT_SLOT(45),
        2,

        SDT_SLOT(45),
        0x80017FFF,
        0x80017FFF,

        false
    },

    {
        "LDRSBEQ false nao executa",
        ARM_HSDT_IMM(0x0, 1, 1, 0, 1, 1, 0, 1, 0, 0x01),

        0xDEADBEEF,
        SDT_SLOT(46),
        0,

        0xDEADBEEF,
        SDT_SLOT(46),
        0,

        SDT_SLOT(46),
        0x1122807F,
        0x1122807F,

        true
    }
};


#define HSDT_TEST_COUNT \
    (sizeof(hsdt_tests) / sizeof(hsdt_tests[0]))


void run_halfword_signed_transfer_tests(void) {

    int passed = 0;
    int failed = 0;

    printf("\n");
    printf("=============================================================\n");
    printf("              HALFWORD / SIGNED TRANSFER TESTS\n");
    printf("=============================================================\n");

    for (size_t i = 0; i < HSDT_TEST_COUNT; i++) {

        const hsdt_test_case_t *test = &hsdt_tests[i];

        arm_test_reset_state();

        bool input_N = (i & 0x1u) != 0;
        bool input_Z = (i & 0x2u) != 0;
        bool input_C = (i & 0x4u) != 0;
        bool input_V = (i & 0x8u) != 0;

        if (test->condition_false) {
            input_N = true;
            input_Z = false;
            input_C = true;
            input_V = true;
        }

        arm_test_set_flags(input_N, input_Z, input_C, input_V);

        write_to_register(0, test->r0_before);
        write_to_register(1, test->r1_before);
        write_to_register(2, test->r2_before);

        /*
         * Mesmo sanity-check do SDT:
         * primeiro prova que o harness preparou a memória.
         */
        write_to_memory_via_databus(
            test->memory_anchor - 4,
            SDT_GUARD_BEFORE
        );

        write_to_memory_via_databus(
            test->memory_anchor,
            test->memory_before
        );

        write_to_memory_via_databus(
            test->memory_anchor + 4,
            SDT_GUARD_AFTER
        );

        bool memory_setup_ok =
            read_from_memory_via_databus(test->memory_anchor - 4) == SDT_GUARD_BEFORE &&
            read_from_memory_via_databus(test->memory_anchor)     == test->memory_before &&
            read_from_memory_via_databus(test->memory_anchor + 4) == SDT_GUARD_AFTER;

        if (!memory_setup_ok) {
            failed++;

            printf(
                "[FAIL][HSDT HARNESS] #%02zu - %s\n"
                "    O teste NAO executou a instrucao.\n"
                "    Falha preparando a memoria antes do opcode.\n"
                "    anchor: 0x%08X\n"
                "    esperado no anchor: 0x%08X\n"
                "    recebido no anchor: 0x%08X\n\n",
                i,
                test->name,
                test->memory_anchor,
                test->memory_before,
                read_from_memory_via_databus(test->memory_anchor)
            );

            arm_test_pause_after_failure();
            continue;
        }

        instruction_processing(test->opcode);

        word_type got_r0 = read_register(0);
        word_type got_r1 = read_register(1);
        word_type got_r2 = read_register(2);

        word_type got_memory =
            read_from_memory_via_databus(test->memory_anchor);

        word_type got_guard_before =
            read_from_memory_via_databus(test->memory_anchor - 4);

        word_type got_guard_after =
            read_from_memory_via_databus(test->memory_anchor + 4);

        bool got_N = is_flag_N_on();
        bool got_Z = is_flag_Z_on();
        bool got_C = is_flag_C_on();
        bool got_V = is_flag_V_on();

        bool rd_ok = got_r0 == test->expected_r0;
        bool rn_ok = got_r1 == test->expected_r1;
        bool rm_ok = got_r2 == test->expected_r2;
        bool memory_ok = got_memory == test->memory_expected;

        bool guards_ok =
            got_guard_before == SDT_GUARD_BEFORE &&
            got_guard_after == SDT_GUARD_AFTER;

        bool flags_ok =
            got_N == input_N &&
            got_Z == input_Z &&
            got_C == input_C &&
            got_V == input_V;

        bool ok =
            rd_ok &&
            rn_ok &&
            rm_ok &&
            memory_ok &&
            guards_ok &&
            flags_ok;

        if (ok) passed++;
        else    failed++;

        if (!ok || ARM_TEST_PRINT_PASSES) {

            printf(
                "[%s][HSDT] #%02zu - %s\n"
                "    opcode: 0x%08X\n"
                "\n"
                "    R0 esperado: 0x%08X\n"
                "    R0 recebido: 0x%08X\n"
                "\n"
                "    R1 esperado: 0x%08X\n"
                "    R1 recebido: 0x%08X\n"
                "\n"
                "    R2 esperado: 0x%08X\n"
                "    R2 recebido: 0x%08X\n"
                "\n"
                "    memoria @ 0x%08X\n"
                "    esperado: 0x%08X\n"
                "    recebido: 0x%08X\n"
                "\n"
                "    flags entrada: N=%d Z=%d C=%d V=%d\n"
                "    flags saida:   N=%d Z=%d C=%d V=%d\n"
                "\n"
                "    guard before: %s\n"
                "    guard after:  %s\n",

                ok ? "PASS" : "FAIL",
                i,
                test->name,
                test->opcode,

                test->expected_r0,
                got_r0,

                test->expected_r1,
                got_r1,

                test->expected_r2,
                got_r2,

                test->memory_anchor,
                test->memory_expected,
                got_memory,

                input_N, input_Z, input_C, input_V,
                got_N, got_Z, got_C, got_V,

                got_guard_before == SDT_GUARD_BEFORE
                    ? "OK"
                    : "ALTERADO",

                got_guard_after == SDT_GUARD_AFTER
                    ? "OK"
                    : "ALTERADO"
            );

            if (!ok) {

                printf("    MOTIVO:");

                if (!rd_ok)
                    printf(" [RD / EXTENSION]");

                if (!rn_ok)
                    printf(" [RN/WRITEBACK]");

                if (!rm_ok)
                    printf(" [RM]");

                if (!memory_ok)
                    printf(" [MEMORY / WIDTH]");

                if (!guards_ok)
                    printf(" [MEMORY GUARD]");

                if (!flags_ok)
                    printf(" [FLAGS]");

                printf("\n\n");

                arm_test_pause_after_failure();
            }
        }
    }

    printf("-------------------------------------------------------------\n");
    printf(
        "HALFWORD / SIGNED: %d PASS | %d FAIL | %zu TOTAL\n",
        passed,
        failed,
        HSDT_TEST_COUNT
    );
    printf("=============================================================\n");
}


// ============================================================
//            MISALIGNED WORD TRANSFER TESTS
//
// ARM7TDMI / GBA:
//   LDR word em endereço não múltiplo de 4 faz uma leitura
//   alinhada e rotaciona a word para que o byte endereçado
//   apareça em bits 7:0.
//
// Em little endian, para AABBCCDD:
//
//   [A+0] -> AABBCCDD
//   [A+1] -> DDAABBCC
//   [A+2] -> CCDDAABB
//   [A+3] -> BBCCDDAA
//
// STR word desalinhado é forçado para o endereço alinhado
// no comportamento esperado do GBA.
//
// Halfword desalinhado NÃO entra: é UNPREDICTABLE no ARM7TDMI.
// ============================================================

typedef struct {
    const char *name;
    word_type opcode;

    word_type r0_before;
    word_type r1_before;

    word_type expected_r0;
    word_type expected_r1;

    word_type memory_anchor;
    word_type memory_before;
    word_type memory_expected;

} unaligned_word_test_t;


static const unaligned_word_test_t unaligned_word_tests[] = {

    {
        "LDR word desalinhado +1 -> ROR 8",
        ARM_SDT_IMM(0xE, 1, 1, 0, 0, 1, 1, 0, 0),

        0xDEADBEEF,
        SDT_SLOT(50) + 1,

        0xDDAABBCC,
        SDT_SLOT(50) + 1,

        SDT_SLOT(50),
        0xAABBCCDD,
        0xAABBCCDD
    },

    {
        "LDR word desalinhado +2 -> ROR 16",
        ARM_SDT_IMM(0xE, 1, 1, 0, 0, 1, 1, 0, 0),

        0xDEADBEEF,
        SDT_SLOT(51) + 2,

        0xCCDDAABB,
        SDT_SLOT(51) + 2,

        SDT_SLOT(51),
        0xAABBCCDD,
        0xAABBCCDD
    },

    {
        "LDR word desalinhado +3 -> ROR 24",
        ARM_SDT_IMM(0xE, 1, 1, 0, 0, 1, 1, 0, 0),

        0xDEADBEEF,
        SDT_SLOT(52) + 3,

        0xBBCCDDAA,
        SDT_SLOT(52) + 3,

        SDT_SLOT(52),
        0xAABBCCDD,
        0xAABBCCDD
    },

    {
        "STR word desalinhado +1 -> escrita word-aligned",
        ARM_SDT_IMM(0xE, 1, 1, 0, 0, 0, 1, 0, 0),

        0x12345678,
        SDT_SLOT(53) + 1,

        0x12345678,
        SDT_SLOT(53) + 1,

        SDT_SLOT(53),
        0xAABBCCDD,
        0x12345678
    }
};


#define UNALIGNED_WORD_TEST_COUNT \
    (sizeof(unaligned_word_tests) / sizeof(unaligned_word_tests[0]))


void run_unaligned_word_transfer_tests(void) {

    int passed = 0;
    int failed = 0;

    printf("\n");
    printf("=============================================================\n");
    printf("                MISALIGNED WORD TESTS\n");
    printf("=============================================================\n");

    for (size_t i = 0; i < UNALIGNED_WORD_TEST_COUNT; i++) {

        const unaligned_word_test_t *test =
            &unaligned_word_tests[i];

        arm_test_reset_state();

        bool input_N = true;
        bool input_Z = false;
        bool input_C = true;
        bool input_V = false;

        arm_test_set_flags(
            input_N,
            input_Z,
            input_C,
            input_V
        );

        write_to_register(0, test->r0_before);
        write_to_register(1, test->r1_before);

        write_to_memory_via_databus(
            test->memory_anchor,
            test->memory_before
        );

        bool memory_setup_ok =
            read_from_memory_via_databus(test->memory_anchor) ==
            test->memory_before;

        if (!memory_setup_ok) {
            failed++;

            printf(
                "[FAIL][MISALIGNED HARNESS] #%02zu - %s\n"
                "    O teste NAO executou a instrucao.\n"
                "    Falha preparando a memoria antes do opcode.\n"
                "    anchor: 0x%08X\n"
                "    esperado: 0x%08X\n"
                "    recebido: 0x%08X\n\n",
                i,
                test->name,
                test->memory_anchor,
                test->memory_before,
                read_from_memory_via_databus(test->memory_anchor)
            );

            arm_test_pause_after_failure();
            continue;
        }

        instruction_processing(test->opcode);

        word_type got_r0 = read_register(0);
        word_type got_r1 = read_register(1);

        word_type got_memory =
            read_from_memory_via_databus(test->memory_anchor);

        bool got_N = is_flag_N_on();
        bool got_Z = is_flag_Z_on();
        bool got_C = is_flag_C_on();
        bool got_V = is_flag_V_on();

        bool rd_ok =
            got_r0 == test->expected_r0;

        bool rn_ok =
            got_r1 == test->expected_r1;

        bool memory_ok =
            got_memory == test->memory_expected;

        bool flags_ok =
            got_N == input_N &&
            got_Z == input_Z &&
            got_C == input_C &&
            got_V == input_V;

        bool ok =
            rd_ok &&
            rn_ok &&
            memory_ok &&
            flags_ok;

        if (ok) passed++;
        else    failed++;

        if (!ok || ARM_TEST_PRINT_PASSES) {

            printf(
                "[%s][MISALIGNED WORD] #%02zu - %s\n"
                "    opcode: 0x%08X\n"
                "    R0 esperado: 0x%08X | recebido: 0x%08X\n"
                "    R1 esperado: 0x%08X | recebido: 0x%08X\n"
                "    MEM esperado: 0x%08X | recebido: 0x%08X\n"
                "    flags in:  N=%d Z=%d C=%d V=%d\n"
                "    flags out: N=%d Z=%d C=%d V=%d\n",

                ok ? "PASS" : "FAIL",
                i,
                test->name,
                test->opcode,

                test->expected_r0,
                got_r0,

                test->expected_r1,
                got_r1,

                test->memory_expected,
                got_memory,

                input_N, input_Z, input_C, input_V,
                got_N, got_Z, got_C, got_V
            );

            if (!ok) {
                printf("    [ENTER] depois de conferir o FAIL...\n\n");
                arm_test_pause_after_failure();
            }
        }
    }

    printf("-------------------------------------------------------------\n");
    printf(
        "MISALIGNED WORD: %d PASS | %d FAIL | %zu TOTAL\n",
        passed,
        failed,
        UNALIGNED_WORD_TEST_COUNT
    );
    printf("=============================================================\n");
}



// ============================================================
// MASTER - ADICIONA AS PROXIMAS FAMILIAS AQUI
// ============================================================

void run_arm_current_test_suite(void) {
    run_data_processing_tests();
    run_multiply_tests();

    // Load/store implementado ate aqui:
    run_single_transfer_tests();
    run_halfword_signed_transfer_tests();
    run_unaligned_word_transfer_tests();

    // futuro:
    // run_psr_tests();
    // run_bx_tests();
    // run_swap_tests();
    // run_block_transfer_tests();
    // run_branch_tests();
    // run_swi_tests();
}


// ============================================================
// DEMO TEMPORARIO
// ============================================================

void demo(void) {

    run_arm_current_test_suite();

}


// --------------- debug end ---------------
// TODO: implementar o Data Processing, item: 4.5       '       '