/*
 * This test covers the four block move instructions LDI,
 * LDIR, LDD, and LDDR.
 *
 * The test creates a buffer full of random data, copies
 * the buffer using one of the move instructions, then
 * checks the target buffer to make sure it contains the
 * same data as the source buffer.
 */

#include "tv80_env.h"

#define BUF_SIZE 128

char bufa[BUF_SIZE];
char bufb[BUF_SIZE];

void rand_buf (char *buf) {
  int i;

  for (i=0; i<BUF_SIZE; i++) {
    buf[i] = randval;
  }
}

char test_ldi () {
  int  i;
  char rv;

  rand_buf (bufa);

  _asm
    ld  de, #_bufb
    ld  hl, #_bufa
    ld  bc, #128

test_ldi_loop:
    ldi 
    ld  a, #0x00
    cp  c
    jp  z, test_ldi_loop_exit
    cp  b
    jp  nz, test_ldi_loop

    test_ldi_loop_exit:
  _endasm;

  rv = 1;
  for (i=0; i<BUF_SIZE; i++) {
    if (bufa[i] != bufb[i]) rv = 0;
  }

  return rv;
}

char test_ldir () {
  int  i;
  char rv;

  rand_buf (bufa);

  _asm
    ld  de, #_bufb
    ld  hl, #_bufa
    ld  bc, #128
    dec bc

    ldir 

  _endasm;

  rv = 1;
  for (i=0; i<BUF_SIZE; i++) {
    if (bufa[i] != bufb[i]) rv = 0;
  }

  return rv;
}

char test_ldd () {
  int  i;
  char rv;

  rand_buf (bufa);

  _asm
    ld  hl, #_bufb
    ld  bc, #127
    add hl, bc
    ex  de, hl
    ld  hl, #_bufa
    add hl, bc
    inc bc

test_ldd_loop:
    ldd
    ld  a, #0x00
    cp  c
    jp  z, test_ldd_loop_exit
    cp  b
    jp  nz, test_ldd_loop

    test_ldd_loop_exit:
  _endasm;

  rv = 1;
  for (i=0; i<BUF_SIZE; i++) {
    if (bufa[i] != bufb[i]) rv = 0;
  }

  return rv;
}

char test_lddr () {
  int  i;
  char rv;

  rand_buf (bufa);

  _asm
    ld  hl, #_bufb
    ld  bc, #127
    add hl, bc
    ex  de, hl
    ld  hl, #_bufa
    add hl, bc

    lddr

  _endasm;

  rv = 1;
  for (i=0; i<BUF_SIZE; i++) {
    if (bufa[i] != bufb[i]) rv = 0;
  }

  return rv;
}

int main ()
{
  max_timeout_high = 0xff;
  
  // initialize source buffer
  print ("Checking LDI\n");
  if (!test_ldi())
    sim_ctl (SC_TEST_FAILED);

  print ("Checking LDIR\n");
  if (!test_ldir())
    sim_ctl (SC_TEST_FAILED);

  print ("Checking LDD\n");
  if (!test_ldd())
    sim_ctl (SC_TEST_FAILED);

  print ("Checking LDDR\n");
  if (!test_lddr())
    sim_ctl (SC_TEST_FAILED);
  else
    sim_ctl (SC_TEST_PASSED);

  return 0;
}
