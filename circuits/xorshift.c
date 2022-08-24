/* This code was generated by Usuba.
   See https://github.com/DadaIsCrazy/usuba.
   From the file "/home/christopher/workspace/usuba/examples/samples/usuba/XorShift.ua" (included below). */

#include <stdint.h>

/* Do NOT change the order of those define/include */

/* #ifndef BITS_PER_REG */
/* #define BITS_PER_REG 64 */
/* #endif */
/* /1* including the architecture specific .h *1/ */
#include "../arch/DATATYPE.h"

/* auxiliary functions */


/* main function */
void xor_shift__ (/*inputs*/ DATATYPE x__[64], /*outputs*/ DATATYPE z__[64]) {

  // Variables declaration
  ;

  // Instructions (body)
  x__[0] = XOR(x__[0],x__[13]);
  x__[1] = XOR(x__[1],x__[14]);
  x__[2] = XOR(x__[2],x__[15]);
  x__[3] = XOR(x__[3],x__[16]);
  x__[4] = XOR(x__[4],x__[17]);
  x__[5] = XOR(x__[5],x__[18]);
  x__[6] = XOR(x__[6],x__[19]);
  x__[7] = XOR(x__[7],x__[20]);
  x__[8] = XOR(x__[8],x__[21]);
  x__[9] = XOR(x__[9],x__[22]);
  x__[10] = XOR(x__[10],x__[23]);
  x__[11] = XOR(x__[11],x__[24]);
  x__[12] = XOR(x__[12],x__[25]);
  x__[13] = XOR(x__[13],x__[26]);
  x__[14] = XOR(x__[14],x__[27]);
  x__[15] = XOR(x__[15],x__[28]);
  x__[16] = XOR(x__[16],x__[29]);
  x__[17] = XOR(x__[17],x__[30]);
  x__[18] = XOR(x__[18],x__[31]);
  x__[19] = XOR(x__[19],x__[32]);
  x__[20] = XOR(x__[20],x__[33]);
  x__[21] = XOR(x__[21],x__[34]);
  x__[22] = XOR(x__[22],x__[35]);
  x__[23] = XOR(x__[23],x__[36]);
  x__[24] = XOR(x__[24],x__[37]);
  x__[25] = XOR(x__[25],x__[38]);
  x__[26] = XOR(x__[26],x__[39]);
  x__[27] = XOR(x__[27],x__[40]);
  x__[28] = XOR(x__[28],x__[41]);
  x__[29] = XOR(x__[29],x__[42]);
  x__[30] = XOR(x__[30],x__[43]);
  x__[31] = XOR(x__[31],x__[44]);
  x__[32] = XOR(x__[32],x__[45]);
  x__[33] = XOR(x__[33],x__[46]);
  x__[34] = XOR(x__[34],x__[47]);
  x__[35] = XOR(x__[35],x__[48]);
  x__[36] = XOR(x__[36],x__[49]);
  x__[37] = XOR(x__[37],x__[50]);
  x__[38] = XOR(x__[38],x__[51]);
  x__[39] = XOR(x__[39],x__[52]);
  x__[40] = XOR(x__[40],x__[53]);
  x__[41] = XOR(x__[41],x__[54]);
  x__[42] = XOR(x__[42],x__[55]);
  x__[43] = XOR(x__[43],x__[56]);
  x__[44] = XOR(x__[44],x__[57]);
  x__[45] = XOR(x__[45],x__[58]);
  x__[46] = XOR(x__[46],x__[59]);
  x__[47] = XOR(x__[47],x__[60]);
  x__[48] = XOR(x__[48],x__[61]);
  x__[49] = XOR(x__[49],x__[62]);
  x__[50] = XOR(x__[50],x__[63]);
  x__[7] = XOR(x__[7],x__[0]);
  x__[8] = XOR(x__[8],x__[1]);
  x__[9] = XOR(x__[9],x__[2]);
  x__[10] = XOR(x__[10],x__[3]);
  x__[11] = XOR(x__[11],x__[4]);
  x__[12] = XOR(x__[12],x__[5]);
  x__[13] = XOR(x__[13],x__[6]);
  x__[14] = XOR(x__[14],x__[7]);
  x__[15] = XOR(x__[15],x__[8]);
  x__[16] = XOR(x__[16],x__[9]);
  x__[17] = XOR(x__[17],x__[10]);
  x__[18] = XOR(x__[18],x__[11]);
  x__[19] = XOR(x__[19],x__[12]);
  x__[20] = XOR(x__[20],x__[13]);
  x__[21] = XOR(x__[21],x__[14]);
  x__[22] = XOR(x__[22],x__[15]);
  x__[23] = XOR(x__[23],x__[16]);
  x__[24] = XOR(x__[24],x__[17]);
  x__[25] = XOR(x__[25],x__[18]);
  x__[26] = XOR(x__[26],x__[19]);
  x__[27] = XOR(x__[27],x__[20]);
  x__[28] = XOR(x__[28],x__[21]);
  x__[29] = XOR(x__[29],x__[22]);
  x__[30] = XOR(x__[30],x__[23]);
  x__[31] = XOR(x__[31],x__[24]);
  x__[32] = XOR(x__[32],x__[25]);
  x__[33] = XOR(x__[33],x__[26]);
  x__[34] = XOR(x__[34],x__[27]);
  x__[35] = XOR(x__[35],x__[28]);
  x__[36] = XOR(x__[36],x__[29]);
  x__[37] = XOR(x__[37],x__[30]);
  x__[38] = XOR(x__[38],x__[31]);
  x__[39] = XOR(x__[39],x__[32]);
  x__[40] = XOR(x__[40],x__[33]);
  x__[41] = XOR(x__[41],x__[34]);
  x__[42] = XOR(x__[42],x__[35]);
  x__[43] = XOR(x__[43],x__[36]);
  x__[44] = XOR(x__[44],x__[37]);
  x__[45] = XOR(x__[45],x__[38]);
  x__[46] = XOR(x__[46],x__[39]);
  x__[47] = XOR(x__[47],x__[40]);
  x__[48] = XOR(x__[48],x__[41]);
  x__[49] = XOR(x__[49],x__[42]);
  x__[50] = XOR(x__[50],x__[43]);
  x__[51] = XOR(x__[51],x__[44]);
  x__[52] = XOR(x__[52],x__[45]);
  x__[53] = XOR(x__[53],x__[46]);
  x__[54] = XOR(x__[54],x__[47]);
  x__[55] = XOR(x__[55],x__[48]);
  x__[56] = XOR(x__[56],x__[49]);
  x__[57] = XOR(x__[57],x__[50]);
  x__[58] = XOR(x__[58],x__[51]);
  x__[59] = XOR(x__[59],x__[52]);
  x__[60] = XOR(x__[60],x__[53]);
  x__[61] = XOR(x__[61],x__[54]);
  x__[62] = XOR(x__[62],x__[55]);
  x__[63] = XOR(x__[63],x__[56]);
  x__[0] = XOR(x__[0],x__[17]);
  x__[1] = XOR(x__[1],x__[18]);
  x__[2] = XOR(x__[2],x__[19]);
  x__[3] = XOR(x__[3],x__[20]);
  x__[4] = XOR(x__[4],x__[21]);
  x__[5] = XOR(x__[5],x__[22]);
  x__[6] = XOR(x__[6],x__[23]);
  x__[7] = XOR(x__[7],x__[24]);
  x__[8] = XOR(x__[8],x__[25]);
  x__[9] = XOR(x__[9],x__[26]);
  x__[10] = XOR(x__[10],x__[27]);
  x__[11] = XOR(x__[11],x__[28]);
  x__[12] = XOR(x__[12],x__[29]);
  x__[13] = XOR(x__[13],x__[30]);
  x__[14] = XOR(x__[14],x__[31]);
  x__[15] = XOR(x__[15],x__[32]);
  x__[16] = XOR(x__[16],x__[33]);
  x__[17] = XOR(x__[17],x__[34]);
  x__[18] = XOR(x__[18],x__[35]);
  x__[19] = XOR(x__[19],x__[36]);
  x__[20] = XOR(x__[20],x__[37]);
  x__[21] = XOR(x__[21],x__[38]);
  x__[22] = XOR(x__[22],x__[39]);
  x__[23] = XOR(x__[23],x__[40]);
  x__[24] = XOR(x__[24],x__[41]);
  x__[25] = XOR(x__[25],x__[42]);
  x__[26] = XOR(x__[26],x__[43]);
  x__[27] = XOR(x__[27],x__[44]);
  x__[28] = XOR(x__[28],x__[45]);
  x__[29] = XOR(x__[29],x__[46]);
  x__[30] = XOR(x__[30],x__[47]);
  x__[31] = XOR(x__[31],x__[48]);
  x__[32] = XOR(x__[32],x__[49]);
  x__[33] = XOR(x__[33],x__[50]);
  x__[34] = XOR(x__[34],x__[51]);
  x__[35] = XOR(x__[35],x__[52]);
  x__[36] = XOR(x__[36],x__[53]);
  x__[37] = XOR(x__[37],x__[54]);
  x__[38] = XOR(x__[38],x__[55]);
  x__[39] = XOR(x__[39],x__[56]);
  x__[40] = XOR(x__[40],x__[57]);
  x__[41] = XOR(x__[41],x__[58]);
  x__[42] = XOR(x__[42],x__[59]);
  x__[43] = XOR(x__[43],x__[60]);
  x__[44] = XOR(x__[44],x__[61]);
  x__[45] = XOR(x__[45],x__[62]);
  x__[46] = XOR(x__[46],x__[63]);
  z__[0] = x__[0];
  z__[1] = x__[1];
  z__[2] = x__[2];
  z__[3] = x__[3];
  z__[4] = x__[4];
  z__[5] = x__[5];
  z__[6] = x__[6];
  z__[7] = x__[7];
  z__[8] = x__[8];
  z__[9] = x__[9];
  z__[10] = x__[10];
  z__[11] = x__[11];
  z__[12] = x__[12];
  z__[13] = x__[13];
  z__[14] = x__[14];
  z__[15] = x__[15];
  z__[16] = x__[16];
  z__[17] = x__[17];
  z__[18] = x__[18];
  z__[19] = x__[19];
  z__[20] = x__[20];
  z__[21] = x__[21];
  z__[22] = x__[22];
  z__[23] = x__[23];
  z__[24] = x__[24];
  z__[25] = x__[25];
  z__[26] = x__[26];
  z__[27] = x__[27];
  z__[28] = x__[28];
  z__[29] = x__[29];
  z__[30] = x__[30];
  z__[31] = x__[31];
  z__[32] = x__[32];
  z__[33] = x__[33];
  z__[34] = x__[34];
  z__[35] = x__[35];
  z__[36] = x__[36];
  z__[37] = x__[37];
  z__[38] = x__[38];
  z__[39] = x__[39];
  z__[40] = x__[40];
  z__[41] = x__[41];
  z__[42] = x__[42];
  z__[43] = x__[43];
  z__[44] = x__[44];
  z__[45] = x__[45];
  z__[46] = x__[46];
  z__[47] = x__[47];
  z__[48] = x__[48];
  z__[49] = x__[49];
  z__[50] = x__[50];
  z__[51] = x__[51];
  z__[52] = x__[52];
  z__[53] = x__[53];
  z__[54] = x__[54];
  z__[55] = x__[55];
  z__[56] = x__[56];
  z__[57] = x__[57];
  z__[58] = x__[58];
  z__[59] = x__[59];
  z__[60] = x__[60];
  z__[61] = x__[61];
  z__[62] = x__[62];
  z__[63] = x__[63];

}

/* Additional functions */


/* **************************************************************** */
/*                            Usuba source                          */
/*                                                                  */
/*


node xor_shift(x :  u64) returns z :  u64
  let
    (x) = (x ^ (x << 13));
    (x) = (x ^ (x >> 7));
    (x) = (x ^ (x << 17));
    (z) = x
  tel


*/
 
