/*
 * Copyright (c) 2024 INESC-ID, Instituto Superior Técnico, Universidade de Lisboa
 * Created by Tiago Rocha <tiagolopesrocha@inesc-id.pt>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <unistd.h>

int main(int argc, char ** argv) {

  __asm__ __volatile__ (
    "csrwi 0x320, 0x8\n\t" // inhibit counters
    "csrwi 0xb03, 0\n\t" // mhpmcounter3 = 0x0, counter 3 reset
    "addi a6, x0, 0x11\n\t" // a6 = 0x11
    "slli a6, a6, 0x1b\n\t" // a6 = 0x11 << 0x1B = 0x8800_0000
    "csrw 0xbc0, a6\n\t" // mhpmmaddr = a6 = 0x8800_0000
    "addi a5, x0, 0xf\n\t" // a5 = 0xF
    "slli a5, a5, 0x20\n\t" // a5 = 0xF << 0x20 = 0xF000_0000
    "addi a5, a5, 0x1fd\n\t" // a5 = 0xF000_0000 + 0x1FD = 0xF000_01FD
    "csrw 0xbc1, a5\n\t" // mhpmebscfg = a5 = 0xF000_01FD, meaning, all 4 GPR and 8 counter values are to be stored
    "addi a5, x0, 0x271\n\t" // a5 = 0x271
    "slli a5, a5, 0x4\n\t" // a5 = 0x271 << 0x4 = 0x2710
    "csrw 0xb23, a5\n\t" // mhpmthreshold3 = a5 = 0x2710 = 0d10000
    "addi a5, x0, 0x2\n\t" // a5 = 0x2
    "csrw 0x323, a5\n\t" // mhpmevent3 = a5 = 0x2, L1D$-Miss
    "csrwi 0x320, 0x0\n\t" // restart counters
    :::
  );

  /* 1/40 L1$miss loop - every 40 instructions retired, 1 is a cache miss */
  __asm__ __volatile__ (
    "li a4, 0x64\n\t" // outside loop iterations = 0x64 = 0d100
    "li a3, 0x9\n\t" // a3 = 0x9
    "slli a3, a3, 0x1c\n\t" // vector address = a3 = 0x9 << 0x1c = 0x9000_0000
    
    "loop1:\n\t" // start of outside loop
    "li a1, 0x3e8\n\t" // inside loop iterations = 0x3e8 = 0d1000
    
    "loop0:\n\t" // start of inside loop

    "ld a2, 0(a3)\n\t" // load from a3, always a L1D$ miss
    
    /* start of 36 padding instructions */
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    "addi t0, t0, 0x1\n\t"
    
    "addi t0, t0, 0x1\n\t"
    /* end of 16 padding instructions */

    "addi a1, a1, -0x1\n\t" // decrement inside loop index
    "addi a3, a3, 0x10\n\t" // increment a3 memory pointer by 16B = L1D$ size

    "bnez a1, loop0\n\t" // end of inside loop
    
    "addi a4, a4, -0x1\n\t" // decrement outside loop index
    "bnez a4, loop1\n\t" // end of outside loop
    :::);

  return 0;
}
