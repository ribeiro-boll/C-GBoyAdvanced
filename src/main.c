//
// Created by bolota on 27/08/2026.
//
#include "include/headers.h"
#include <stdio.h>
int main(void) {
    startup_sequence();
    write_to_register(1337, 2);
    write_to_memory_via_databus(0x02000000, read_register(2));
    printf("teste: %d\n", read_from_memory_via_databus(0x02000000));
}