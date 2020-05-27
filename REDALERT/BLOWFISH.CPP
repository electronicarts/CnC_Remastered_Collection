//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection

/* $Header: /CounterStrike/BLOWFISH.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BLOWFISH.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 04/14/96                                                     *
 *                                                                                             *
 *                  Last Update : July 8, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   BlowfishEngine::Decrypt -- Decrypts data using blowfish algorithm.                        *
 *   BlowfishEngine::Encrypt -- Encrypt an arbitrary block of data.                            *
 *   BlowfishEngine::Process_Block -- Process a block of data using Blowfish algorithm.        *
 *   BlowfishEngine::Sub_Key_Encrypt -- Encrypts a block for use in S-Box processing.          *
 *   BlowfishEngine::Submit_Key -- Submit a key that will allow data processing.               *
 *   BlowfishEngine::~BlowfishEngine -- Destructor for the Blowfish engine.                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "blowfish.h"
#include	<string.h>
#include	<assert.h>


/*
**	Byte order controlled long integer. This integer is constructed
**	so that character 0 (C0) is the most significant byte of the
**	integer. This is biased toward big endian architecture, but that
**	just happens to be how the Blowfish algorithm was designed.
*/
typedef union {
	unsigned long Long;
	struct {
		unsigned char C3;
		unsigned char C2;
		unsigned char C1;
		unsigned char C0;
	} Char;
} Int;


/***********************************************************************************************
 * BlowfishEngine::~BlowfishEngine -- Destructor for the Blowfish engine.                      *
 *                                                                                             *
 *    This destructor will clear out the s-box tables so that even if the memory for the       *
 *    class remains, it will contain no compromising data.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
BlowfishEngine::~BlowfishEngine(void)
{
	if (IsKeyed) {
		Submit_Key(NULL, 0);
	}
}


/***********************************************************************************************
 * BlowfishEngine::Submit_Key -- Submit a key that will allow data processing.                 *
 *                                                                                             *
 *    This routine must be called before any data can be encrypted or decrypted. This routine  *
 *    need only be called when the key is to be changed or set for the first time. Once the    *
 *    key has been set, the engine may be used to encrypt, decrypt, or both operations         *
 *    indefinitely. The key must be 56 bytes or less in length. This is necessary because      *
 *    any keys longer than that will not correctly affect the encryption process.              *
 *                                                                                             *
 *    If the key pointer is NULL, then the S-Box tables are reset to identity. This will       *
 *    mask the previous key setting. Use this method to clear the engine after processing in   *
 *    order to gain a measure of security.                                                     *
 *                                                                                             *
 * INPUT:   key      -- Pointer to the key data block.                                         *
 *                                                                                             *
 *          length   -- The length of the submitted key.                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This is a time consuming process.                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BlowfishEngine::Submit_Key(void const * key, int length)
{
	assert(length <= MAX_KEY_LENGTH);

	/*
	**	Initialize the permutation and S-Box tables to a known
	**	constant value.
	*/
	memcpy(P_Encrypt, P_Init, sizeof(P_Init));
	memcpy(P_Decrypt, P_Init, sizeof(P_Init));
	memcpy(bf_S, S_Init, sizeof(S_Init));

	/*
	**	Validate parameters.
	*/
	if (key == 0 || length == 0) {
		IsKeyed = false;
		return;
	}

	/*
	**	Combine the key with the permutation table. Wrap the key
	**	as many times as necessary to ensure that the entire
	**	permutation table has been modified. The key is lifted
	**	into a long by using endian independent means.
	*/
	int j = 0;
	unsigned char const * key_ptr = (unsigned char const *)key;
	unsigned long * p_ptr = &P_Encrypt[0];
	for (int index = 0; index < ROUNDS+2; index++) {
		unsigned long data = 0;

		data = (data << CHAR_BIT) | key_ptr[j++ % length];
		data = (data << CHAR_BIT) | key_ptr[j++ % length];
		data = (data << CHAR_BIT) | key_ptr[j++ % length];
		data = (data << CHAR_BIT) | key_ptr[j++ % length];

		*p_ptr++ ^= data;
	}

	/*
	**	The permutation table must be scrambled by means of the key. This
	**	is how the key is factored into the encryption -- by merely altering
	**	the permutation (and S-Box) tables. Because this transformation alters
	**	the table data WHILE it is using the table data, the tables are
	**	thoroughly obfuscated by this process.
	*/
	unsigned long left = 0x00000000L;
	unsigned long right = 0x00000000L;
	unsigned long * p_en = &P_Encrypt[0];			// Encryption table.
	unsigned long * p_de = &P_Decrypt[ROUNDS+1];	// Decryption table.
	for (int p_index = 0; p_index < ROUNDS+2; p_index += 2) {
		Sub_Key_Encrypt(left, right);

		*p_en++ = left;
		*p_en++ = right;

		*p_de-- = left;
		*p_de-- = right;
	}

	/*
	**	Perform a similar transmutation to the S-Box tables. Also notice that the
	**	working 64 bit number is carried into this process from the previous
	**	operation.
	*/
	for (int sbox_index = 0; sbox_index < 4; sbox_index++) {
		for (int ss_index = 0; ss_index < UCHAR_MAX+1; ss_index += 2) {
			Sub_Key_Encrypt(left, right);
			bf_S[sbox_index][ss_index] = left;
			bf_S[sbox_index][ss_index + 1] = right;
		}
	}

	IsKeyed = true;
}


/***********************************************************************************************
 * BlowfishEngine::Encrypt -- Encrypt an arbitrary block of data.                              *
 *                                                                                             *
 *    Use this routine to encrypt an arbitrary block of data. The block must be an even        *
 *    multiple of 8 bytes. Any bytes left over will not be encrypted. The 8 byte requirement   *
 *    is necessary because the underlying algorithm processes blocks in 8 byte chunks.         *
 *    Partial blocks are unrecoverable and useless.                                            *
 *                                                                                             *
 * INPUT:   plaintext-- Pointer to the data block to be encrypted.                             *
 *                                                                                             *
 *          length   -- The length of the data block.                                          *
 *                                                                                             *
 *          cyphertext- Pointer to the output buffer that will hold the encrypted data.        *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of bytes encrypted.                                 *
 *                                                                                             *
 * WARNINGS:   You must submit the key before calling this routine. This will only encrypt     *
 *             the plaintext in 8 byte increments. Modulo bytes left over are not processed.   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int BlowfishEngine::Encrypt(void const * plaintext, int length, void * cyphertext)
{
	if (plaintext == 0 || length == 0) {
		return(0);
	}
	if (cyphertext == 0) cyphertext = (void *)plaintext;

	if (IsKeyed) {

		/*
		**	Validate parameters.
		*/
		int blocks = length / BYTES_PER_BLOCK;

		/*
		**	Process the buffer in 64 bit chunks.
		*/
		for (int index = 0; index < blocks; index++) {
			Process_Block(plaintext, cyphertext, P_Encrypt);
			plaintext = ((char *)plaintext) + BYTES_PER_BLOCK;
			cyphertext = ((char *)cyphertext) + BYTES_PER_BLOCK;
		}
		int encrypted = blocks * BYTES_PER_BLOCK;

		/*
		**	Copy over any trailing left over appendix bytes.
		*/
		if (encrypted < length) {
			memmove(cyphertext, plaintext, length - encrypted);
		}

		return(encrypted);
	}

	/*
	**	Non-keyed processing merely copies the data.
	*/
	if (plaintext != cyphertext) {
		memmove(cyphertext, plaintext, length);
	}
	return(length);
}


/***********************************************************************************************
 * BlowfishEngine::Decrypt -- Decrypt an arbitrary block of data.                              *
 *                                                                                             *
 *    Use this routine to decrypt an arbitrary block of data. The block must be an even        *
 *    multiple of 8 bytes. Any bytes left over will not be decrypted. The 8 byte requirement   *
 *    is necessary because the underlying algorithm processes blocks in 8 byte chunks.         *
 *    Partial blocks are unrecoverable and useless.                                            *
 *                                                                                             *
 * INPUT:   cyphertext- Pointer to the data block to be decrypted.                             *
 *                                                                                             *
 *          length   -- The length of the data block.                                          *
 *                                                                                             *
 *          plaintext-- Pointer to the output buffer that will hold the decrypted data.        *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of bytes decrypted.                                 *
 *                                                                                             *
 * WARNINGS:   You must submit the key before calling this routine. This will only decrypt     *
 *             the cyphertext in 8 byte increments. Modulo bytes left over are not processed.  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int BlowfishEngine::Decrypt(void const * cyphertext, int length, void * plaintext)
{
	if (cyphertext == 0 || length == 0) {
		return(0);
	}
	if (plaintext == 0) plaintext = (void *)cyphertext;

	if (IsKeyed) {

		/*
		**	Validate parameters.
		*/
		int blocks = length / BYTES_PER_BLOCK;

		/*
		**	Process the buffer in 64 bit chunks.
		*/
		for (int index = 0; index < blocks; index++) {
			Process_Block(cyphertext, plaintext, P_Decrypt);
			cyphertext = ((char *)cyphertext) + BYTES_PER_BLOCK;
			plaintext = ((char *)plaintext) + BYTES_PER_BLOCK;
		}
		int encrypted = blocks * BYTES_PER_BLOCK;

		/*
		**	Copy over any trailing left over appendix bytes.
		*/
		if (encrypted < length) {
			memmove(plaintext, cyphertext, length - encrypted);
		}

		return(encrypted);
	}

	/*
	**	Non-keyed processing merely copies the data.
	*/
	if (plaintext != cyphertext) {
		memmove(plaintext, cyphertext, length);
	}
	return(length);
}


/***********************************************************************************************
 * BlowfishEngine::Process_Block -- Process a block of data using Blowfish algorithm.          *
 *                                                                                             *
 *    This is the main processing routine for encryption and decryption. The algorithm         *
 *    consists of a 16 round Feistal network and uses mathematics from different algebraic     *
 *    groups (strengthens against differential cryptanalysis). The large S-Boxes and the       *
 *    rounds strengthen it against linear cryptanalysis.                                       *
 *                                                                                             *
 * INPUT:   plaintext   -- Pointer to the source text (it actually might be a pointer to       *
 *                         the cyphertext if this is called as a decryption process).          *
 *                                                                                             *
 *          cyphertext  -- Pointer to the output buffer that will hold the processed block.    *
 *                                                                                             *
 *          ptable      -- Pointer to the permutation table. This algorithm will encrypt       *
 *                         and decrypt using the same S-Box tables. The encryption control     *
 *                         is handled by the permutation table.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The source and destination buffers must be 8 bytes long.                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BlowfishEngine::Process_Block(void const * plaintext, void * cyphertext, unsigned long const * ptable)
{
	/*
	**	Input the left and right halves of the source block such that
	**	the byte order is constant regardless of the endian
	**	persuasion of the current processor. The blowfish algorithm is
	**	biased toward "big endian" architecture and some optimizations
	**	could be done for big endian processors in that case.
	*/
	unsigned char const * source = (unsigned char const *)plaintext;
	Int left;
	left.Char.C0 = *source++;
	left.Char.C1 = *source++;
	left.Char.C2 = *source++;
	left.Char.C3 = *source++;

	Int right;
	right.Char.C0 = *source++;
	right.Char.C1 = *source++;
	right.Char.C2 = *source++;
	right.Char.C3 = *source;

	/*
	**	Perform all Feistal rounds on the block. This is the encryption/decryption
	**	process. Since there is an exchange that occurs after each round, two
	**	rounds are combined in this loop to avoid unnecessary exchanging.
	*/
	for (int index = 0; index < ROUNDS/2; index++) {
		left.Long ^= *ptable++;
		right.Long ^= ((( bf_S[0][left.Char.C0] + bf_S[1][left.Char.C1]) ^ bf_S[2][left.Char.C2]) + bf_S[3][left.Char.C3]);
		right.Long ^= *ptable++;
		left.Long ^= ((( bf_S[0][right.Char.C0] + bf_S[1][right.Char.C1]) ^ bf_S[2][right.Char.C2]) + bf_S[3][right.Char.C3]);
	}

	/*
	**	The final two longs in the permutation table are processed into the block.
	**	The left and right halves are still reversed as a side effect of the last
	**	round.
	*/
	left.Long ^= *ptable++;
	right.Long ^= *ptable;

	/*
	**	The final block data is output in endian architecture
	**	independent format. Notice that the blocks are output as
	**	right first and left second. This is to counteract the final
	**	superfluous exchange that occurs as a side effect of the
	**	encryption rounds.
	*/
	unsigned char * out = (unsigned char *)cyphertext;
	*out++ = right.Char.C0;
	*out++ = right.Char.C1;
	*out++ = right.Char.C2;
	*out++ = right.Char.C3;

	*out++ = left.Char.C0;
	*out++ = left.Char.C1;
	*out++ = left.Char.C2;
	*out = left.Char.C3;
}


/***********************************************************************************************
 * BlowfishEngine::Sub_Key_Encrypt -- Encrypts a block for use in S-Box processing.            *
 *                                                                                             *
 *    This is the same as the normal process block function but it doesn't have the endian     *
 *    fixup logic. Since this routine is only called for S-Box table generation and it is      *
 *    known that the S-Box initial data is already in local machine endian format, the         *
 *    byte order fixups are not needed. This also has a tendency to speed up S-Box generation  *
 *    as well.                                                                                 *
 *                                                                                             *
 * INPUT:   left  -- The left half of the data block.                                          *
 *                                                                                             *
 *          right -- The right half of the data block.                                         *
 *                                                                                             *
 * OUTPUT:  none, but the processed block is stored back into the left and right half          *
 *          integers.                                                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BlowfishEngine::Sub_Key_Encrypt(unsigned long & left, unsigned long & right)
{
	Int l;
	l.Long = left;

	Int r;
	r.Long = right;

	for (int index = 0; index < ROUNDS; index += 2) {
		l.Long ^= P_Encrypt[index];
		r.Long ^= ((( bf_S[0][l.Char.C0] + bf_S[1][l.Char.C1]) ^ bf_S[2][l.Char.C2]) + bf_S[3][l.Char.C3]);
		r.Long ^= P_Encrypt[index+1];
		l.Long ^= ((( bf_S[0][r.Char.C0] + bf_S[1][r.Char.C1]) ^ bf_S[2][r.Char.C2]) + bf_S[3][r.Char.C3]);
	}
	left = r.Long ^ P_Encrypt[ROUNDS+1];
	right = l.Long ^ P_Encrypt[ROUNDS];
}


/*
**	These tables have the bytes stored in machine endian format. Because of this,
**	a special block cypher routine is needed when the sub-keys are generated.
**	This is kludgier than it otherwise should be. However, storing these
**	integers in machine independent format would be even more painful.
*/

unsigned long const BlowfishEngine::P_Init[BlowfishEngine::ROUNDS+2] = {
	0x243F6A88U,0x85A308D3U,0x13198A2EU,0x03707344U,0xA4093822U,0x299F31D0U,0x082EFA98U,0xEC4E6C89U,
	0x452821E6U,0x38D01377U,0xBE5466CFU,0x34E90C6CU,0xC0AC29B7U,0xC97C50DDU,0x3F84D5B5U,0xB5470917U,
	0x9216D5D9U,0x8979FB1BU
} ;

unsigned long const BlowfishEngine::S_Init[4][UCHAR_MAX+1] = {
	{
		0xD1310BA6U,0x98DFB5ACU,0x2FFD72DBU,0xD01ADFB7U,0xB8E1AFEDU,0x6A267E96U,0xBA7C9045U,0xF12C7F99U,
		0x24A19947U,0xB3916CF7U,0x0801F2E2U,0x858EFC16U,0x636920D8U,0x71574E69U,0xA458FEA3U,0xF4933D7EU,
		0x0D95748FU,0x728EB658U,0x718BCD58U,0x82154AEEU,0x7B54A41DU,0xC25A59B5U,0x9C30D539U,0x2AF26013U,
		0xC5D1B023U,0x286085F0U,0xCA417918U,0xB8DB38EFU,0x8E79DCB0U,0x603A180EU,0x6C9E0E8BU,0xB01E8A3EU,
		0xD71577C1U,0xBD314B27U,0x78AF2FDAU,0x55605C60U,0xE65525F3U,0xAA55AB94U,0x57489862U,0x63E81440U,
		0x55CA396AU,0x2AAB10B6U,0xB4CC5C34U,0x1141E8CEU,0xA15486AFU,0x7C72E993U,0xB3EE1411U,0x636FBC2AU,
		0x2BA9C55DU,0x741831F6U,0xCE5C3E16U,0x9B87931EU,0xAFD6BA33U,0x6C24CF5CU,0x7A325381U,0x28958677U,
		0x3B8F4898U,0x6B4BB9AFU,0xC4BFE81BU,0x66282193U,0x61D809CCU,0xFB21A991U,0x487CAC60U,0x5DEC8032U,
		0xEF845D5DU,0xE98575B1U,0xDC262302U,0xEB651B88U,0x23893E81U,0xD396ACC5U,0x0F6D6FF3U,0x83F44239U,
		0x2E0B4482U,0xA4842004U,0x69C8F04AU,0x9E1F9B5EU,0x21C66842U,0xF6E96C9AU,0x670C9C61U,0xABD388F0U,
		0x6A51A0D2U,0xD8542F68U,0x960FA728U,0xAB5133A3U,0x6EEF0B6CU,0x137A3BE4U,0xBA3BF050U,0x7EFB2A98U,
		0xA1F1651DU,0x39AF0176U,0x66CA593EU,0x82430E88U,0x8CEE8619U,0x456F9FB4U,0x7D84A5C3U,0x3B8B5EBEU,
		0xE06F75D8U,0x85C12073U,0x401A449FU,0x56C16AA6U,0x4ED3AA62U,0x363F7706U,0x1BFEDF72U,0x429B023DU,
		0x37D0D724U,0xD00A1248U,0xDB0FEAD3U,0x49F1C09BU,0x075372C9U,0x80991B7BU,0x25D479D8U,0xF6E8DEF7U,
		0xE3FE501AU,0xB6794C3BU,0x976CE0BDU,0x04C006BAU,0xC1A94FB6U,0x409F60C4U,0x5E5C9EC2U,0x196A2463U,
		0x68FB6FAFU,0x3E6C53B5U,0x1339B2EBU,0x3B52EC6FU,0x6DFC511FU,0x9B30952CU,0xCC814544U,0xAF5EBD09U,
		0xBEE3D004U,0xDE334AFDU,0x660F2807U,0x192E4BB3U,0xC0CBA857U,0x45C8740FU,0xD20B5F39U,0xB9D3FBDBU,
		0x5579C0BDU,0x1A60320AU,0xD6A100C6U,0x402C7279U,0x679F25FEU,0xFB1FA3CCU,0x8EA5E9F8U,0xDB3222F8U,
		0x3C7516DFU,0xFD616B15U,0x2F501EC8U,0xAD0552ABU,0x323DB5FAU,0xFD238760U,0x53317B48U,0x3E00DF82U,
		0x9E5C57BBU,0xCA6F8CA0U,0x1A87562EU,0xDF1769DBU,0xD542A8F6U,0x287EFFC3U,0xAC6732C6U,0x8C4F5573U,
		0x695B27B0U,0xBBCA58C8U,0xE1FFA35DU,0xB8F011A0U,0x10FA3D98U,0xFD2183B8U,0x4AFCB56CU,0x2DD1D35BU,
		0x9A53E479U,0xB6F84565U,0xD28E49BCU,0x4BFB9790U,0xE1DDF2DAU,0xA4CB7E33U,0x62FB1341U,0xCEE4C6E8U,
		0xEF20CADAU,0x36774C01U,0xD07E9EFEU,0x2BF11FB4U,0x95DBDA4DU,0xAE909198U,0xEAAD8E71U,0x6B93D5A0U,
		0xD08ED1D0U,0xAFC725E0U,0x8E3C5B2FU,0x8E7594B7U,0x8FF6E2FBU,0xF2122B64U,0x8888B812U,0x900DF01CU,
		0x4FAD5EA0U,0x688FC31CU,0xD1CFF191U,0xB3A8C1ADU,0x2F2F2218U,0xBE0E1777U,0xEA752DFEU,0x8B021FA1U,
		0xE5A0CC0FU,0xB56F74E8U,0x18ACF3D6U,0xCE89E299U,0xB4A84FE0U,0xFD13E0B7U,0x7CC43B81U,0xD2ADA8D9U,
		0x165FA266U,0x80957705U,0x93CC7314U,0x211A1477U,0xE6AD2065U,0x77B5FA86U,0xC75442F5U,0xFB9D35CFU,
		0xEBCDAF0CU,0x7B3E89A0U,0xD6411BD3U,0xAE1E7E49U,0x00250E2DU,0x2071B35EU,0x226800BBU,0x57B8E0AFU,
		0x2464369BU,0xF009B91EU,0x5563911DU,0x59DFA6AAU,0x78C14389U,0xD95A537FU,0x207D5BA2U,0x02E5B9C5U,
		0x83260376U,0x6295CFA9U,0x11C81968U,0x4E734A41U,0xB3472DCAU,0x7B14A94AU,0x1B510052U,0x9A532915U,
		0xD60F573FU,0xBC9BC6E4U,0x2B60A476U,0x81E67400U,0x08BA6FB5U,0x571BE91FU,0xF296EC6BU,0x2A0DD915U,
		0xB6636521U,0xE7B9F9B6U,0xFF34052EU,0xC5855664U,0x53B02D5DU,0xA99F8FA1U,0x08BA4799U,0x6E85076AU,
	},{
		0x4B7A70E9U,0xB5B32944U,0xDB75092EU,0xC4192623U,0xAD6EA6B0U,0x49A7DF7DU,0x9CEE60B8U,0x8FEDB266U,
		0xECAA8C71U,0x699A17FFU,0x5664526CU,0xC2B19EE1U,0x193602A5U,0x75094C29U,0xA0591340U,0xE4183A3EU,
		0x3F54989AU,0x5B429D65U,0x6B8FE4D6U,0x99F73FD6U,0xA1D29C07U,0xEFE830F5U,0x4D2D38E6U,0xF0255DC1U,
		0x4CDD2086U,0x8470EB26U,0x6382E9C6U,0x021ECC5EU,0x09686B3FU,0x3EBAEFC9U,0x3C971814U,0x6B6A70A1U,
		0x687F3584U,0x52A0E286U,0xB79C5305U,0xAA500737U,0x3E07841CU,0x7FDEAE5CU,0x8E7D44ECU,0x5716F2B8U,
		0xB03ADA37U,0xF0500C0DU,0xF01C1F04U,0x0200B3FFU,0xAE0CF51AU,0x3CB574B2U,0x25837A58U,0xDC0921BDU,
		0xD19113F9U,0x7CA92FF6U,0x94324773U,0x22F54701U,0x3AE5E581U,0x37C2DADCU,0xC8B57634U,0x9AF3DDA7U,
		0xA9446146U,0x0FD0030EU,0xECC8C73EU,0xA4751E41U,0xE238CD99U,0x3BEA0E2FU,0x3280BBA1U,0x183EB331U,
		0x4E548B38U,0x4F6DB908U,0x6F420D03U,0xF60A04BFU,0x2CB81290U,0x24977C79U,0x5679B072U,0xBCAF89AFU,
		0xDE9A771FU,0xD9930810U,0xB38BAE12U,0xDCCF3F2EU,0x5512721FU,0x2E6B7124U,0x501ADDE6U,0x9F84CD87U,
		0x7A584718U,0x7408DA17U,0xBC9F9ABCU,0xE94B7D8CU,0xEC7AEC3AU,0xDB851DFAU,0x63094366U,0xC464C3D2U,
		0xEF1C1847U,0x3215D908U,0xDD433B37U,0x24C2BA16U,0x12A14D43U,0x2A65C451U,0x50940002U,0x133AE4DDU,
		0x71DFF89EU,0x10314E55U,0x81AC77D6U,0x5F11199BU,0x043556F1U,0xD7A3C76BU,0x3C11183BU,0x5924A509U,
		0xF28FE6EDU,0x97F1FBFAU,0x9EBABF2CU,0x1E153C6EU,0x86E34570U,0xEAE96FB1U,0x860E5E0AU,0x5A3E2AB3U,
		0x771FE71CU,0x4E3D06FAU,0x2965DCB9U,0x99E71D0FU,0x803E89D6U,0x5266C825U,0x2E4CC978U,0x9C10B36AU,
		0xC6150EBAU,0x94E2EA78U,0xA5FC3C53U,0x1E0A2DF4U,0xF2F74EA7U,0x361D2B3DU,0x1939260FU,0x19C27960U,
		0x5223A708U,0xF71312B6U,0xEBADFE6EU,0xEAC31F66U,0xE3BC4595U,0xA67BC883U,0xB17F37D1U,0x018CFF28U,
		0xC332DDEFU,0xBE6C5AA5U,0x65582185U,0x68AB9802U,0xEECEA50FU,0xDB2F953BU,0x2AEF7DADU,0x5B6E2F84U,
		0x1521B628U,0x29076170U,0xECDD4775U,0x619F1510U,0x13CCA830U,0xEB61BD96U,0x0334FE1EU,0xAA0363CFU,
		0xB5735C90U,0x4C70A239U,0xD59E9E0BU,0xCBAADE14U,0xEECC86BCU,0x60622CA7U,0x9CAB5CABU,0xB2F3846EU,
		0x648B1EAFU,0x19BDF0CAU,0xA02369B9U,0x655ABB50U,0x40685A32U,0x3C2AB4B3U,0x319EE9D5U,0xC021B8F7U,
		0x9B540B19U,0x875FA099U,0x95F7997EU,0x623D7DA8U,0xF837889AU,0x97E32D77U,0x11ED935FU,0x16681281U,
		0x0E358829U,0xC7E61FD6U,0x96DEDFA1U,0x7858BA99U,0x57F584A5U,0x1B227263U,0x9B83C3FFU,0x1AC24696U,
		0xCDB30AEBU,0x532E3054U,0x8FD948E4U,0x6DBC3128U,0x58EBF2EFU,0x34C6FFEAU,0xFE28ED61U,0xEE7C3C73U,
		0x5D4A14D9U,0xE864B7E3U,0x42105D14U,0x203E13E0U,0x45EEE2B6U,0xA3AAABEAU,0xDB6C4F15U,0xFACB4FD0U,
		0xC742F442U,0xEF6ABBB5U,0x654F3B1DU,0x41CD2105U,0xD81E799EU,0x86854DC7U,0xE44B476AU,0x3D816250U,
		0xCF62A1F2U,0x5B8D2646U,0xFC8883A0U,0xC1C7B6A3U,0x7F1524C3U,0x69CB7492U,0x47848A0BU,0x5692B285U,
		0x095BBF00U,0xAD19489DU,0x1462B174U,0x23820E00U,0x58428D2AU,0x0C55F5EAU,0x1DADF43EU,0x233F7061U,
		0x3372F092U,0x8D937E41U,0xD65FECF1U,0x6C223BDBU,0x7CDE3759U,0xCBEE7460U,0x4085F2A7U,0xCE77326EU,
		0xA6078084U,0x19F8509EU,0xE8EFD855U,0x61D99735U,0xA969A7AAU,0xC50C06C2U,0x5A04ABFCU,0x800BCADCU,
		0x9E447A2EU,0xC3453484U,0xFDD56705U,0x0E1E9EC9U,0xDB73DBD3U,0x105588CDU,0x675FDA79U,0xE3674340U,
		0xC5C43465U,0x713E38D8U,0x3D28F89EU,0xF16DFF20U,0x153E21E7U,0x8FB03D4AU,0xE6E39F2BU,0xDB83ADF7U,
	},{
		0xE93D5A68U,0x948140F7U,0xF64C261CU,0x94692934U,0x411520F7U,0x7602D4F7U,0xBCF46B2EU,0xD4A20068U,
		0xD4082471U,0x3320F46AU,0x43B7D4B7U,0x500061AFU,0x1E39F62EU,0x97244546U,0x14214F74U,0xBF8B8840U,
		0x4D95FC1DU,0x96B591AFU,0x70F4DDD3U,0x66A02F45U,0xBFBC09ECU,0x03BD9785U,0x7FAC6DD0U,0x31CB8504U,
		0x96EB27B3U,0x55FD3941U,0xDA2547E6U,0xABCA0A9AU,0x28507825U,0x530429F4U,0x0A2C86DAU,0xE9B66DFBU,
		0x68DC1462U,0xD7486900U,0x680EC0A4U,0x27A18DEEU,0x4F3FFEA2U,0xE887AD8CU,0xB58CE006U,0x7AF4D6B6U,
		0xAACE1E7CU,0xD3375FECU,0xCE78A399U,0x406B2A42U,0x20FE9E35U,0xD9F385B9U,0xEE39D7ABU,0x3B124E8BU,
		0x1DC9FAF7U,0x4B6D1856U,0x26A36631U,0xEAE397B2U,0x3A6EFA74U,0xDD5B4332U,0x6841E7F7U,0xCA7820FBU,
		0xFB0AF54EU,0xD8FEB397U,0x454056ACU,0xBA489527U,0x55533A3AU,0x20838D87U,0xFE6BA9B7U,0xD096954BU,
		0x55A867BCU,0xA1159A58U,0xCCA92963U,0x99E1DB33U,0xA62A4A56U,0x3F3125F9U,0x5EF47E1CU,0x9029317CU,
		0xFDF8E802U,0x04272F70U,0x80BB155CU,0x05282CE3U,0x95C11548U,0xE4C66D22U,0x48C1133FU,0xC70F86DCU,
		0x07F9C9EEU,0x41041F0FU,0x404779A4U,0x5D886E17U,0x325F51EBU,0xD59BC0D1U,0xF2BCC18FU,0x41113564U,
		0x257B7834U,0x602A9C60U,0xDFF8E8A3U,0x1F636C1BU,0x0E12B4C2U,0x02E1329EU,0xAF664FD1U,0xCAD18115U,
		0x6B2395E0U,0x333E92E1U,0x3B240B62U,0xEEBEB922U,0x85B2A20EU,0xE6BA0D99U,0xDE720C8CU,0x2DA2F728U,
		0xD0127845U,0x95B794FDU,0x647D0862U,0xE7CCF5F0U,0x5449A36FU,0x877D48FAU,0xC39DFD27U,0xF33E8D1EU,
		0x0A476341U,0x992EFF74U,0x3A6F6EABU,0xF4F8FD37U,0xA812DC60U,0xA1EBDDF8U,0x991BE14CU,0xDB6E6B0DU,
		0xC67B5510U,0x6D672C37U,0x2765D43BU,0xDCD0E804U,0xF1290DC7U,0xCC00FFA3U,0xB5390F92U,0x690FED0BU,
		0x667B9FFBU,0xCEDB7D9CU,0xA091CF0BU,0xD9155EA3U,0xBB132F88U,0x515BAD24U,0x7B9479BFU,0x763BD6EBU,
		0x37392EB3U,0xCC115979U,0x8026E297U,0xF42E312DU,0x6842ADA7U,0xC66A2B3BU,0x12754CCCU,0x782EF11CU,
		0x6A124237U,0xB79251E7U,0x06A1BBE6U,0x4BFB6350U,0x1A6B1018U,0x11CAEDFAU,0x3D25BDD8U,0xE2E1C3C9U,
		0x44421659U,0x0A121386U,0xD90CEC6EU,0xD5ABEA2AU,0x64AF674EU,0xDA86A85FU,0xBEBFE988U,0x64E4C3FEU,
		0x9DBC8057U,0xF0F7C086U,0x60787BF8U,0x6003604DU,0xD1FD8346U,0xF6381FB0U,0x7745AE04U,0xD736FCCCU,
		0x83426B33U,0xF01EAB71U,0xB0804187U,0x3C005E5FU,0x77A057BEU,0xBDE8AE24U,0x55464299U,0xBF582E61U,
		0x4E58F48FU,0xF2DDFDA2U,0xF474EF38U,0x8789BDC2U,0x5366F9C3U,0xC8B38E74U,0xB475F255U,0x46FCD9B9U,
		0x7AEB2661U,0x8B1DDF84U,0x846A0E79U,0x915F95E2U,0x466E598EU,0x20B45770U,0x8CD55591U,0xC902DE4CU,
		0xB90BACE1U,0xBB8205D0U,0x11A86248U,0x7574A99EU,0xB77F19B6U,0xE0A9DC09U,0x662D09A1U,0xC4324633U,
		0xE85A1F02U,0x09F0BE8CU,0x4A99A025U,0x1D6EFE10U,0x1AB93D1DU,0x0BA5A4DFU,0xA186F20FU,0x2868F169U,
		0xDCB7DA83U,0x573906FEU,0xA1E2CE9BU,0x4FCD7F52U,0x50115E01U,0xA70683FAU,0xA002B5C4U,0x0DE6D027U,
		0x9AF88C27U,0x773F8641U,0xC3604C06U,0x61A806B5U,0xF0177A28U,0xC0F586E0U,0x006058AAU,0x30DC7D62U,
		0x11E69ED7U,0x2338EA63U,0x53C2DD94U,0xC2C21634U,0xBBCBEE56U,0x90BCB6DEU,0xEBFC7DA1U,0xCE591D76U,
		0x6F05E409U,0x4B7C0188U,0x39720A3DU,0x7C927C24U,0x86E3725FU,0x724D9DB9U,0x1AC15BB4U,0xD39EB8FCU,
		0xED545578U,0x08FCA5B5U,0xD83D7CD3U,0x4DAD0FC4U,0x1E50EF5EU,0xB161E6F8U,0xA28514D9U,0x6C51133CU,
		0x6FD5C7E7U,0x56E14EC4U,0x362ABFCEU,0xDDC6C837U,0xD79A3234U,0x92638212U,0x670EFA8EU,0x406000E0U,
	},{
		0x3A39CE37U,0xD3FAF5CFU,0xABC27737U,0x5AC52D1BU,0x5CB0679EU,0x4FA33742U,0xD3822740U,0x99BC9BBEU,
		0xD5118E9DU,0xBF0F7315U,0xD62D1C7EU,0xC700C47BU,0xB78C1B6BU,0x21A19045U,0xB26EB1BEU,0x6A366EB4U,
		0x5748AB2FU,0xBC946E79U,0xC6A376D2U,0x6549C2C8U,0x530FF8EEU,0x468DDE7DU,0xD5730A1DU,0x4CD04DC6U,
		0x2939BBDBU,0xA9BA4650U,0xAC9526E8U,0xBE5EE304U,0xA1FAD5F0U,0x6A2D519AU,0x63EF8CE2U,0x9A86EE22U,
		0xC089C2B8U,0x43242EF6U,0xA51E03AAU,0x9CF2D0A4U,0x83C061BAU,0x9BE96A4DU,0x8FE51550U,0xBA645BD6U,
		0x2826A2F9U,0xA73A3AE1U,0x4BA99586U,0xEF5562E9U,0xC72FEFD3U,0xF752F7DAU,0x3F046F69U,0x77FA0A59U,
		0x80E4A915U,0x87B08601U,0x9B09E6ADU,0x3B3EE593U,0xE990FD5AU,0x9E34D797U,0x2CF0B7D9U,0x022B8B51U,
		0x96D5AC3AU,0x017DA67DU,0xD1CF3ED6U,0x7C7D2D28U,0x1F9F25CFU,0xADF2B89BU,0x5AD6B472U,0x5A88F54CU,
		0xE029AC71U,0xE019A5E6U,0x47B0ACFDU,0xED93FA9BU,0xE8D3C48DU,0x283B57CCU,0xF8D56629U,0x79132E28U,
		0x785F0191U,0xED756055U,0xF7960E44U,0xE3D35E8CU,0x15056DD4U,0x88F46DBAU,0x03A16125U,0x0564F0BDU,
		0xC3EB9E15U,0x3C9057A2U,0x97271AECU,0xA93A072AU,0x1B3F6D9BU,0x1E6321F5U,0xF59C66FBU,0x26DCF319U,
		0x7533D928U,0xB155FDF5U,0x03563482U,0x8ABA3CBBU,0x28517711U,0xC20AD9F8U,0xABCC5167U,0xCCAD925FU,
		0x4DE81751U,0x3830DC8EU,0x379D5862U,0x9320F991U,0xEA7A90C2U,0xFB3E7BCEU,0x5121CE64U,0x774FBE32U,
		0xA8B6E37EU,0xC3293D46U,0x48DE5369U,0x6413E680U,0xA2AE0810U,0xDD6DB224U,0x69852DFDU,0x09072166U,
		0xB39A460AU,0x6445C0DDU,0x586CDECFU,0x1C20C8AEU,0x5BBEF7DDU,0x1B588D40U,0xCCD2017FU,0x6BB4E3BBU,
		0xDDA26A7EU,0x3A59FF45U,0x3E350A44U,0xBCB4CDD5U,0x72EACEA8U,0xFA6484BBU,0x8D6612AEU,0xBF3C6F47U,
		0xD29BE463U,0x542F5D9EU,0xAEC2771BU,0xF64E6370U,0x740E0D8DU,0xE75B1357U,0xF8721671U,0xAF537D5DU,
		0x4040CB08U,0x4EB4E2CCU,0x34D2466AU,0x0115AF84U,0xE1B00428U,0x95983A1DU,0x06B89FB4U,0xCE6EA048U,
		0x6F3F3B82U,0x3520AB82U,0x011A1D4BU,0x277227F8U,0x611560B1U,0xE7933FDCU,0xBB3A792BU,0x344525BDU,
		0xA08839E1U,0x51CE794BU,0x2F32C9B7U,0xA01FBAC9U,0xE01CC87EU,0xBCC7D1F6U,0xCF0111C3U,0xA1E8AAC7U,
		0x1A908749U,0xD44FBD9AU,0xD0DADECBU,0xD50ADA38U,0x0339C32AU,0xC6913667U,0x8DF9317CU,0xE0B12B4FU,
		0xF79E59B7U,0x43F5BB3AU,0xF2D519FFU,0x27D9459CU,0xBF97222CU,0x15E6FC2AU,0x0F91FC71U,0x9B941525U,
		0xFAE59361U,0xCEB69CEBU,0xC2A86459U,0x12BAA8D1U,0xB6C1075EU,0xE3056A0CU,0x10D25065U,0xCB03A442U,
		0xE0EC6E0EU,0x1698DB3BU,0x4C98A0BEU,0x3278E964U,0x9F1F9532U,0xE0D392DFU,0xD3A0342BU,0x8971F21EU,
		0x1B0A7441U,0x4BA3348CU,0xC5BE7120U,0xC37632D8U,0xDF359F8DU,0x9B992F2EU,0xE60B6F47U,0x0FE3F11DU,
		0xE54CDA54U,0x1EDAD891U,0xCE6279CFU,0xCD3E7E6FU,0x1618B166U,0xFD2C1D05U,0x848FD2C5U,0xF6FB2299U,
		0xF523F357U,0xA6327623U,0x93A83531U,0x56CCCD02U,0xACF08162U,0x5A75EBB5U,0x6E163697U,0x88D273CCU,
		0xDE966292U,0x81B949D0U,0x4C50901BU,0x71C65614U,0xE6C6C7BDU,0x327A140AU,0x45E1D006U,0xC3F27B9AU,
		0xC9AA53FDU,0x62A80F00U,0xBB25BFE2U,0x35BDD2F6U,0x71126905U,0xB2040222U,0xB6CBCF7CU,0xCD769C2BU,
		0x53113EC0U,0x1640E3D3U,0x38ABBD60U,0x2547ADF0U,0xBA38209CU,0xF746CE76U,0x77AFA1C5U,0x20756060U,
		0x85CBFE4EU,0x8AE88DD8U,0x7AAAF9B0U,0x4CF9AA7EU,0x1948C25CU,0x02FB8A8CU,0x01C36AE4U,0xD6EBE1F9U,
		0x90D4F869U,0xA65CDEA0U,0x3F09252DU,0xC208E69FU,0xB74E6132U,0xCE77E25BU,0x578FDFE3U,0x3AC372E6U
	}
};

