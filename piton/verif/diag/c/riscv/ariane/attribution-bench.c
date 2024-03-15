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

int main(int argc, char **argv) {

  __asm__ __volatile__ (
    "csrwi 0x320, 0x8\n\t" // inhibit counters
    "csrwi 0xb03, 0\n\t" // mhpmcounter3 = 0x0, counter 3 reset
    "addi a6, x0, 0x11\n\t" // a6 = 0x11
    "slli a6, a6, 0x1b\n\t" // a6 = 0x11 << 0x1B = 0x8800_0000
    "csrw 0xbc0, a6\n\t" // mhpmmaddr = a6 = 0x8800_0000
    "addi a5, x0, 0x8\n\t" // a5 = 0x8
    "csrw 0xbc1, a5\n\t" // mhpmebscfg = a5 = 0x8, meaning, mhpmcounter3 value is stored
    "addi a5, x0, 0x65\n\t" // a5 = 0x65
    "csrw 0xb23, a5\n\t" // mhpmthreshold3 = a5 = 0x65 = 0d101
    "addi a5, x0, 0x6\n\t" // a5 = 0x6
    "csrw 0x323, a5\n\t" // mhpmevent3 = a5 = 0x6, Store Accesses
    "csrwi 0x320, 0x0\n\t" // restart counters
    :::
  );

  __asm__ __volatile__ (
    "li a1, 0x186a0\n\t" // loop iterations = 0x186a0 = 0d100000
    "li a3, 0x9\n\t" // a3 = 0x9
    "slli a3, a3, 0x1c\n\t" // memory address = a3 = 0x9 << 0x1c = 0x9000_0000
    
    "loop:\n\t" // start of loop

    "sb x0, 0(a3)\n\t" // store instruction 1
    "sb x0, 0(a3)\n\t" // store instruction 2
    "sb x0, 0(a3)\n\t" // store instruction 3
    "sb x0, 0(a3)\n\t" // store instruction 4
    
    "addi a1, a1, -0x1\n\t" // decrement loop index
    "bnez a1, loop\n\t" // end of loop
    :::
  );

  return 0;
}
