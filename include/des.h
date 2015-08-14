/******************************************
** DES-Implementation
** Author: B-Con (b-con@b-con.us)
** Copyright/Restrictions: GNU GPL
** Disclaimer: This code is presented "as is" without any garuentees; said author holds
               liability for no problems rendered by the use of this code.
** Details: This code is the implementation of the AES algorithm, as specified by the
            NIST in in publication FIPS PUB 197, availible on the NIST website at
            http://csrc.nist.gov/publications/fips/fips46-3/fips46-3.pdf .
******************************************/

#define uchar unsigned char
/*#define uint unsigned int*/

void key_schedule(uchar key[], uchar schedule[][6], unsigned int mode);

// Initial (Inv)Permutation step
void IP(unsigned int state[], uchar in[]);

void InvIP(unsigned int state[], uchar in[]);

unsigned int f(unsigned int state, uchar key[]);

void des_crypt(uchar in[], uchar out[], uchar key[][6]);

/**************************************
             3DES functions
**************************************/

void three_des_key_schedule(uchar key[], uchar schedule[][16][6], unsigned int mode);

void three_des_crypt(uchar in[], uchar out[], uchar key[][16][6]);

/************************************/
