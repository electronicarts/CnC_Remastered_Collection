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

/****************************************************************************
*  Error Constants, in case of values above 10 hexadecimal please call
*  Marx International, Inc.
****************************************************************************/

#define PARAM_ERR       0x01
#define BOX_NOT_FOUND   0x02

#define BOX_CRYPT_ERR   0x03
#define UNI_CRYPT_ERR   0x04
#define READ_RAM_ERR    0x05
#define WRITE_RAM_ERR   0x06
#define RAM_COUNT_ERR   0x07
#define BOX_READY_ERR   0x08


/****************************************************************************
*  Constants
****************************************************************************/

#define CRYPTLENG       32
#define RAM1LENG        50
#define RAM2LENG       433
#define IDEALENG        32

/****************************************************************************
*  User Functions
****************************************************************************/
extern "C" {

extern short CbN_BoxReady(unsigned short iPortNr, unsigned char *pcBoxName);

extern short CbN_ReadID1(unsigned short iPortNr, unsigned char *pcSCodeId,
                     unsigned long *plIdReturn);

extern short CbN_ReadID2(unsigned short iPortNr, unsigned char *pcSCodeId,
                     unsigned long *plIdReturn);

extern short CbN_ReadID3(unsigned short iPortNr, unsigned char *pcSCodeId,
                     unsigned long *plIdReturn);

extern short CbN_ReadID4(unsigned short iPortNr, unsigned char *pcSCodeId,
                     unsigned long *plIdReturn);

extern short CbN_ReadID5(unsigned short iPortNr, unsigned char *pcSCodeId,
                     unsigned long *plIdReturn);

extern short CbN_ReadID6(unsigned short iPortNr, unsigned char *pcSCodeId,
                     unsigned long *plIdReturn);

extern short CbN_ReadID7(unsigned short iPortNr, unsigned char *pcSCodeId,
                     unsigned long *plIdReturn);

extern short CbN_ReadID8(unsigned short iPortNr, unsigned char *pcSCodeId,
                     unsigned long *plIdReturn);

extern short CbN_ReadSER(unsigned short iPortNr, unsigned char *pcSCodeSer,
                     unsigned  long *plSerNum);

extern short CbN_ReadRAM1(unsigned short iPortNr,unsigned short iIdNr,unsigned char
                      *pcSCodeId,unsigned char *pcPasswRam1,unsigned short
                      iStartAdr,unsigned short iLength, unsigned char *pcOutData);

extern short CbN_ReadRAM2(unsigned short iPortNr,unsigned short iIdNr,unsigned char
                      *pcSCodeId,unsigned char *pcPasswRam2,unsigned short
                      iStartAdr,unsigned short iLength, unsigned char *pcOutData);

extern short CbN_WriteRAM1(unsigned short iPortNr,unsigned short iIdNr,unsigned char
                      *pcSCodeId,unsigned char *pcPasswRam1,unsigned short
                      iStartAdr,unsigned short iLength,unsigned char *pcOutData);

extern short CbN_WriteRAM2(unsigned short iPortNr,unsigned short iIdNr,unsigned char
                      *pcSCodeId,unsigned char *pcPasswRam2,unsigned short
                      iStartAdr,unsigned short iLength,unsigned char *pcOutData);

extern short CbN_IncRAM1(unsigned short iPortNr, unsigned short iIdNr, unsigned char
                      *pcSCodeId, unsigned char *pcPasswRam1, unsigned short
                      iCounterAdr, unsigned short *piNewCount);

extern short CbN_DecRAM1(unsigned short iPortNr, unsigned short iIdNr, unsigned char
                      *pcSCodeId, unsigned char *pcPasswRam1, unsigned short
                      iCounterAdr, unsigned short *piNewCount);

extern short CbN_IncRAM2(unsigned short iPortNr, unsigned short iIdNr, unsigned char
                      *pcSCodeId, unsigned char *pcPasswRam2, unsigned short
                      iCounterAdr, unsigned short *piNewCount);

extern short CbN_DecRAM2(unsigned short iPortNr, unsigned short iIdNr, unsigned char
                      *pcSCodeId, unsigned char *pcPasswRam2, unsigned short
                      iCounterAdr, unsigned short *piNewCount);


extern short CbN_Decrypt(unsigned short iPortNr,unsigned short iIdNr,unsigned char
                      *pcSCodeId,unsigned short iSeed, unsigned short iLength,
                      unsigned char *pcOutData);

extern short CbN_Encrypt(unsigned short iPortNr,unsigned short iIdNr,unsigned char
                      *pcSCodeId,unsigned short iSeed,unsigned short iLength,
                      unsigned char *pcOutData);

extern short CbN_IDEA_Encrypt(unsigned short iPortNr, unsigned short iIdNr,
                           unsigned char *pcSCodeId, unsigned char *pcBuff,
                           unsigned long lLength);

extern short CbN_IDEA_Decrypt(unsigned short iPortNr, unsigned short iIdNr,
                           unsigned char *pcSCodeId, unsigned char *pcBuff,
                           unsigned long lLength);

extern short CbN_IDEA_EncryptP(unsigned short iPortNr, unsigned short iIdNr,
                           unsigned char *pcSCodeId, unsigned char *pcBuff,
                           unsigned long lLength, unsigned long lIdeaKey);

extern short CbN_IDEA_DecryptP(unsigned short iPortNr, unsigned short iIdNr,
                           unsigned char *pcSCodeId, unsigned char *pcBuff,
                           unsigned long lLength, unsigned long lIdeaKey);

extern short CbN_SetCounterRAM1(unsigned short iPortNr, unsigned short iIdNr,
                              unsigned char *pcSCodeId, unsigned char
                              *pcPassRam, unsigned short iAdrCount, unsigned short
                              iNewCount);

extern short CbN_ReadCounterRAM1(unsigned short iPortNr, unsigned short iIdNr,
                               unsigned char *pcSCodeId, unsigned char
                               *pcPassRam, unsigned short iAdrCount, unsigned short
                               *piCurrentCount);

extern short CbN_SetCounterRAM2(unsigned short iPortNr, unsigned short iIdNr,
                              unsigned char *pcSCodeId, unsigned char
                              *pcPassRam, unsigned short iAdrCount, unsigned short
                              iNewCount);

extern short CbN_ReadCounterRAM2(unsigned short iPortNr, unsigned short iIdNr,
                               unsigned char *pcSCodeId, unsigned char
                               *pcPassRam, unsigned short iAdrCount, unsigned short
                               *piCurrentCount);

}
