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

/***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Westwood Keyboard Library                                    *
 *                                                                                             *
 *                    File Name : KEYBOARD.H                                                   *
 *                                                                                             *
 *                   Programmer : Philip W. Gorrow                                             *
 *                                                                                             *
 *                   Start Date : 10/16/95                                                     *
 *                                                                                             *
 *                  Last Update : October 16, 1995 [PWG]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef KEYBOARD_H
#define KEYBOARD_H
 
#include <WWSTD.H>

typedef enum {
	WWKEY_SHIFT_BIT	= 0x100,
	WWKEY_CTRL_BIT		= 0x200,
	WWKEY_ALT_BIT		= 0x400,
	WWKEY_RLS_BIT		= 0x800,
	WWKEY_VK_BIT		= 0x1000,
	WWKEY_DBL_BIT		= 0x2000,
	WWKEY_BTN_BIT		= 0x8000,
} WWKey_Type;

class WWKeyboardClass
{
	public:
		/*===================================================================*/
		/* Define the base constructor and destructors for the class			*/
		/*===================================================================*/
		WWKeyboardClass();

		/*===================================================================*/
		/* Define the functions which work with the Keyboard Class				*/
		/*===================================================================*/
		BOOL 	Check(void);															// checks keybuff for meta key
		int 	Get(void);																// gets a meta key from the keybuffer
		BOOL 	Put(int key);															// dumps a key into the keybuffer
		BOOL	Put_Key_Message(	UINT vk_key, BOOL release = FALSE, 		// handles keyboard related message
										BOOL dbl = FALSE);							//   and mouse clicks and dbl clicks
		int 	Check_Num(void);														// checks keybuff for a keynum key
		int 	Get_VK(void);															// gets keynum key from key buff
		int 	Check_ACII(void);														// checks keybuff for an ascii key
		int 	Get_ASCII(void);														// gets an ascii key from keybuff
		int 	Check_Bits(void);														// checks keybuff for key w/ bits
		int 	Get_Bits(void);														// get key from keybuff w/ bits
		int 	To_ASCII(int num);													// converts keynum to ascii value
		int 	Option_On(int option);												// turns specified option on
		int 	Option_Off(int option);												// turns specified option off
		void 	Clear(void);															// clears all keys from keybuffer
		int 	Down(int key);															// tests to see if a key is down
		void	AI(void);																// messaging logic for key manager

		/*===================================================================*/
		/* Define the main hook for the message processing loop.					*/
		/*===================================================================*/
		void Message_Handler(HWND hwnd, UINT message, UINT wParam, LONG lParam);

		/*===================================================================*/
		/* Define public routines which can be used on keys in general.		*/
		/*===================================================================*/
		VOID Split(int &key, int &shift, int &ctrl, int &alt, int &rls, int &dbl);
		BOOL Is_Mouse_Key(int key);


		/*===================================================================*/
		/* Define the public access variables which are used with the			*/
		/*   Keyboard Class.																	*/
		/*===================================================================*/
		int				MouseQX;
		int				MouseQY;

		unsigned char	VKRemap[256];		// gives vk for any ascii char
	private:
		/*===================================================================*/
		/* Define the private access functions which are used by keyboard		*/
		/*===================================================================*/
		int  Buff_Get(void);


		/*===================================================================*/
		/* Define the private access variables which are used with the			*/
		/*   Keyboard Class.																	*/
		/*===================================================================*/
		unsigned char	AsciiRemap[2048];	// remap for shft/ctrl/alt key combos
		unsigned short Buffer[256];		// buffer which holds actual keypresses
		unsigned char  ToggleKeys[256];	// determines toggles which affect key
		long				Head;					// the head position in keyboard buffer
		long				Tail;					// the tail position in keyboard buffer
		int				MState;
		int				Conditional;
		HANDLE			CurrentCursor;
};


#define	VK_NONE				  0x00
#define	VK_LBUTTON          0x01
#define	VK_RBUTTON          0x02
#define	VK_CANCEL           0x03
#define	VK_MBUTTON          0x04
#define	VK_NONE_05			  0x05
#define	VK_NONE_06			  0x06
#define	VK_NONE_07			  0x07
#define	VK_BACK             0x08
#define	VK_TAB              0x09
#define	VK_NONE_0A			  0x0A
#define	VK_NONE_0B			  0x0B
#define	VK_CLEAR            0x0C
#define	VK_RETURN           0x0D
#define	VK_NONE_0E			  0x0E
#define	VK_NONE_0F			  0x0F
#define	VK_SHIFT            0x10
#define	VK_CONTROL          0x11
#define	VK_MENU             0x12
#define	VK_PAUSE            0x13
#define	VK_CAPITAL          0x14
#define	VK_NONE_15			  0x15
#define	VK_NONE_16			  0x16
#define	VK_NONE_17			  0x17
#define	VK_NONE_18			  0x18
#define	VK_NONE_19			  0x19
#define	VK_NONE_1A			  0x1A
#define	VK_ESCAPE           0x1B
#define	VK_NONE_1C			  0x1C
#define	VK_NONE_1D			  0x1D
#define	VK_NONE_1E			  0x1E
#define	VK_NONE_1F			  0x1F
#define	VK_SPACE            0x20
#define	VK_PRIOR            0x21
#define	VK_NEXT             0x22
#define	VK_END              0x23
#define	VK_HOME             0x24
#define	VK_LEFT             0x25
#define	VK_UP               0x26
#define	VK_RIGHT            0x27
#define	VK_DOWN             0x28
#define	VK_SELECT           0x29
#define	VK_PRINT            0x2A
#define	VK_EXECUTE          0x2B
#define	VK_SNAPSHOT         0x2C
#define	VK_INSERT           0x2D
#define	VK_DELETE           0x2E
#define	VK_HELP             0x2F
#define	VK_0					  0x30
#define	VK_1					  0x31
#define	VK_2					  0x32
#define	VK_3					  0x33
#define	VK_4					  0x34
#define	VK_5					  0x35
#define	VK_6					  0x36
#define	VK_7					  0x37
#define	VK_8					  0x38
#define	VK_9					  0x39
#define	VK_NONE_3B			  0x3B
#define	VK_NONE_3C			  0x3C
#define	VK_NONE_3D			  0x3D
#define	VK_NONE_3E			  0x3E
#define	VK_NONE_3F			  0x3F
#define	VK_NONE_40			  0x40
#define	VK_A					  0x41
#define	VK_B					  0x42
#define	VK_C					  0x43
#define	VK_D					  0x44
#define	VK_E					  0x45
#define	VK_F					  0x46
#define	VK_G					  0x47
#define	VK_H					  0x48
#define	VK_I					  0x49
#define	VK_J					  0x4A
#define	VK_K					  0x4B
#define	VK_L					  0x4C
#define	VK_M					  0x4D
#define	VK_N					  0x4E
#define	VK_O					  0x4F
#define	VK_P					  0x50
#define	VK_Q					  0x51
#define	VK_R					  0x52
#define	VK_S					  0x53
#define	VK_T					  0x54
#define	VK_U					  0x55
#define	VK_V					  0x56
#define	VK_W					  0x57
#define	VK_X					  0x58
#define	VK_Y					  0x59
#define	VK_Z					  0x5A
#define	VK_NONE_5B			  0x5B
#define	VK_NONE_5C			  0x5C
#define	VK_NONE_5D			  0x5D
#define	VK_NONE_5E			  0x5E
#define	VK_NONE_5F			  0x5F
#define	VK_NUMPAD0          0x60
#define	VK_NUMPAD1          0x61
#define	VK_NUMPAD2          0x62
#define	VK_NUMPAD3          0x63
#define	VK_NUMPAD4          0x64
#define	VK_NUMPAD5          0x65
#define	VK_NUMPAD6          0x66
#define	VK_NUMPAD7          0x67
#define	VK_NUMPAD8          0x68
#define	VK_NUMPAD9          0x69
#define	VK_MULTIPLY         0x6A
#define	VK_ADD              0x6B
#define	VK_SEPARATOR        0x6C
#define	VK_SUBTRACT         0x6D
#define	VK_DECIMAL          0x6E
#define	VK_DIVIDE           0x6F
#define	VK_F1               0x70
#define	VK_F2               0x71
#define	VK_F3               0x72
#define	VK_F4               0x73
#define	VK_F5               0x74
#define	VK_F6               0x75
#define	VK_F7               0x76
#define	VK_F8               0x77
#define	VK_F9               0x78
#define	VK_F10              0x79
#define	VK_F11              0x7A
#define	VK_F12              0x7B
#define	VK_F13              0x7C
#define	VK_F14              0x7D
#define	VK_F15              0x7E
#define	VK_F16              0x7F
#define	VK_F17              0x80
#define	VK_F18              0x81
#define	VK_F19              0x82
#define	VK_F20              0x83
#define	VK_F21              0x84
#define	VK_F22              0x85
#define	VK_F23              0x86
#define	VK_F24              0x87
#define	VK_NONE_88          0x88
#define	VK_NONE_89          0x89
#define	VK_NONE_8A          0x8A
#define	VK_NONE_8B          0x8B
#define	VK_NONE_8C          0x8C
#define	VK_NONE_8D          0x8D
#define	VK_NONE_8E          0x8E
#define	VK_NONE_8F          0x8F
#define	VK_NUMLOCK          0x90
#define	VK_SCROLL           0x91
#define	VK_NONE_92			  0x92
#define	VK_NONE_93			  0x93
#define	VK_NONE_94			  0x94
#define	VK_NONE_95			  0x95
#define	VK_NONE_96			  0x96
#define	VK_NONE_97			  0x97
#define	VK_NONE_98			  0x98
#define	VK_NONE_99			  0x99
#define	VK_NONE_9A			  0x9A
#define	VK_NONE_9B			  0x9B
#define	VK_NONE_9C			  0x9C
#define	VK_NONE_9D			  0x9D
#define	VK_NONE_9E			  0x9E
#define	VK_NONE_9F			  0x9F
#define	VK_NONE_A0			  0xA0
#define	VK_NONE_A1			  0xA1
#define	VK_NONE_A2			  0xA2
#define	VK_NONE_A3			  0xA3
#define	VK_NONE_A4			  0xA4
#define	VK_NONE_A5			  0xA5
#define	VK_NONE_A6			  0xA6
#define	VK_NONE_A7			  0xA7
#define	VK_NONE_A8			  0xA8
#define	VK_NONE_A9			  0xA9
#define	VK_NONE_AA			  0xAA
#define	VK_NONE_AB			  0xAB
#define	VK_NONE_AC			  0xAC
#define	VK_NONE_AD			  0xAD
#define	VK_NONE_AE			  0xAE
#define	VK_NONE_AF			  0xAF
#define	VK_NONE_B0			  0xB0
#define	VK_NONE_B1			  0xB1
#define	VK_NONE_B2			  0xB2
#define	VK_NONE_B3			  0xB3
#define	VK_NONE_B4			  0xB4
#define	VK_NONE_B5			  0xB5
#define	VK_NONE_B6			  0xB6
#define	VK_NONE_B7			  0xB7
#define	VK_NONE_B8			  0xB8
#define	VK_NONE_B9			  0xB9
#define	VK_NONE_BA			  0xBA
#define	VK_NONE_BB			  0xBB
#define	VK_NONE_BC			  0xBC
#define	VK_NONE_BD			  0xBD
#define	VK_NONE_BE			  0xBE
#define	VK_NONE_BF			  0xBF
#define	VK_NONE_C0			  0xC0
#define	VK_NONE_C1			  0xC1
#define	VK_NONE_C2			  0xC2
#define	VK_NONE_C3			  0xC3
#define	VK_NONE_C4			  0xC4
#define	VK_NONE_C5			  0xC5
#define	VK_NONE_C6			  0xC6
#define	VK_NONE_C7			  0xC7
#define	VK_NONE_C8			  0xC8
#define	VK_NONE_C9			  0xC9
#define	VK_NONE_CA			  0xCA
#define	VK_NONE_CB			  0xCB
#define	VK_NONE_CC			  0xCC
#define	VK_NONE_CD			  0xCD
#define	VK_NONE_CE			  0xCE
#define	VK_NONE_CF			  0xCF
#define	VK_NONE_D0			  0xD0
#define	VK_NONE_D1			  0xD1
#define	VK_NONE_D2			  0xD2
#define	VK_NONE_D3			  0xD3
#define	VK_NONE_D4			  0xD4
#define	VK_NONE_D5			  0xD5
#define	VK_NONE_D6			  0xD6
#define	VK_NONE_D7			  0xD7
#define	VK_NONE_D8			  0xD8
#define	VK_NONE_D9			  0xD9
#define	VK_NONE_DA			  0xDA
#define	VK_NONE_DB			  0xDB
#define	VK_NONE_DC			  0xDC
#define	VK_NONE_DD			  0xDD
#define	VK_NONE_DE			  0xDE
#define	VK_NONE_DF			  0xDF
#define	VK_NONE_E0			  0xE0
#define	VK_NONE_E1			  0xE1
#define	VK_NONE_E2			  0xE2
#define	VK_NONE_E3			  0xE3
#define	VK_NONE_E4			  0xE4
#define	VK_NONE_E5			  0xE5
#define	VK_NONE_E6			  0xE6
#define	VK_NONE_E7			  0xE7
#define	VK_NONE_E8			  0xE8
#define	VK_NONE_E9			  0xE9
#define	VK_NONE_EA			  0xEA
#define	VK_NONE_EB			  0xEB
#define	VK_NONE_EC			  0xEC
#define	VK_NONE_ED			  0xED
#define	VK_NONE_EE			  0xEE
#define	VK_NONE_EF			  0xEF
#define	VK_NONE_F0			  0xF0
#define	VK_NONE_F1			  0xF1
#define	VK_NONE_F2			  0xF2
#define	VK_NONE_F3			  0xF3
#define	VK_NONE_F4			  0xF4
#define	VK_NONE_F5			  0xF5
#define	VK_NONE_F6			  0xF6
#define	VK_NONE_F7			  0xF7
#define	VK_NONE_F8			  0xF8
#define	VK_NONE_F9			  0xF9
#define	VK_NONE_FA			  0xFA
#define	VK_NONE_FB			  0xFB
#define	VK_NONE_FC			  0xFC
#define	VK_NONE_FD			  0xFD
#define	VK_NONE_FE			  0xFE
#define	VK_NONE_FF			  0xFF

#define	VK_UPLEFT			  VK_HOME
#define	VK_UPRIGHT			  VK_PRIOR
#define	VK_DOWNLEFT			  VK_END
#define	VK_DOWNRIGHT		  VK_NEXT
#define	VK_ALT				  VK_MENU

enum {
	//
	// Define all the KA types as variations of the VK types.  This is
	// so the KA functions will work properly under windows 95.
	//
	KA_NONE				= 0,
	KA_MORE 				= 1,
	KA_SETBKGDCOL 		= 2,
	KA_SETFORECOL 		= 6,
	KA_FORMFEED 		= 12,
	KA_SPCTAB 			= 20,
	KA_SETX 				= 25,
	KA_SETY 				= 26,

	KA_SPACE				= 32,					/*   */
	KA_EXCLAMATION,							/* ! */
	KA_DQUOTE,									/* " */
	KA_POUND,									/* # */
	KA_DOLLAR,									/* $ */
	KA_PERCENT,									/* % */
	KA_AMPER,									/* & */
	KA_SQUOTE,									/* ' */
	KA_LPAREN,									/* ( */
	KA_RPAREN,									/* ) */
	KA_ASTERISK,								/* * */
	KA_PLUS,										/* + */
	KA_COMMA,									/* , */
	KA_MINUS,									/* - */
	KA_PERIOD,									/* . */
	KA_SLASH,									/* / */

	KA_0, KA_1, KA_2, KA_3, KA_4, KA_5, KA_6, KA_7, KA_8, KA_9,
	KA_COLON,									/* : */
	KA_SEMICOLON,								/* ; */
	KA_LESS_THAN,								/* < */
	KA_EQUAL,									/* = */
	KA_GREATER_THAN,							/* > */
	KA_QUESTION,								/* ? */

	KA_AT,										/* @ */
	KA_A,											/* A */
	KA_B,											/* B */
	KA_C,											/* C */
	KA_D,											/* D */
	KA_E,											/* E */
	KA_F,											/* F */
	KA_G,											/* G */
	KA_H,											/* H */
	KA_I,											/* I */
	KA_J,											/* J */
	KA_K,											/* K */
	KA_L,											/* L */
	KA_M,											/* M */
	KA_N,											/* N */
	KA_O,											/* O */

	KA_P,											/* P */
	KA_Q,											/* Q */
	KA_R,											/* R */
	KA_S,											/* S */
	KA_T,											/* T */
	KA_U,											/* U */
	KA_V,											/* V */
	KA_W,											/* W */
	KA_X,											/* X */
	KA_Y,											/* Y */
	KA_Z,											/* Z */
	KA_LBRACKET,								/* [ */
	KA_BACKSLASH,								/* \ */
	KA_RBRACKET,								/* ] */
	KA_CARROT,									/* ^ */
	KA_UNDERLINE,								/* _ */

	KA_GRAVE,									/* ` */
	KA_a,											/* a */
	KA_b,											/* b */
	KA_c,											/* c */
	KA_d,											/* d */
	KA_e,											/* e */
	KA_f,											/* f */
	KA_g,											/* g */
	KA_h,											/* h */
	KA_i,											/* i */
	KA_j,											/* j */
	KA_k,											/* k */
	KA_l,											/* l */
	KA_m,											/* m */
	KA_n,											/* n */
	KA_o,											/* o */

	KA_p,											/* p */
	KA_q,											/* q */
	KA_r,											/* r */
	KA_s,											/* s */
	KA_t,											/* t */
	KA_u,											/* u */
	KA_v,											/* v */
	KA_w,											/* w */
	KA_x,											/* x */
	KA_y,											/* y */
	KA_z,											/* z */
	KA_LBRACE,									/* { */
	KA_BAR,										/* | */
	KA_RBRACE,									/* ] */
	KA_TILDA,									/* ~ */

	KA_ESC 				= VK_ESCAPE | WWKEY_VK_BIT,
	KA_EXTEND 			= VK_ESCAPE | WWKEY_VK_BIT,
	KA_RETURN 			= VK_RETURN | WWKEY_VK_BIT,
	KA_BACKSPACE 		= VK_BACK | WWKEY_VK_BIT,
	KA_TAB 				= VK_TAB  | WWKEY_VK_BIT,
	KA_DELETE			= VK_DELETE | WWKEY_VK_BIT,									/* <DELETE> */
	KA_INSERT			= VK_INSERT | WWKEY_VK_BIT,									/* <INSERT> */
	KA_PGDN				= VK_NEXT | WWKEY_VK_BIT,										/* <PAGE DOWN> */
	KA_DOWNRIGHT 		= VK_NEXT | WWKEY_VK_BIT,
	KA_DOWN				= VK_DOWN | WWKEY_VK_BIT,										/* <DOWN ARROW> */
	KA_END				= VK_END | WWKEY_VK_BIT,										/* <END> */
	KA_DOWNLEFT 		= VK_END | WWKEY_VK_BIT,
	KA_RIGHT				= VK_RIGHT | WWKEY_VK_BIT,									/* <RIGHT ARROW> */
	KA_KEYPAD5			= VK_SELECT | WWKEY_VK_BIT,									/* NUMERIC KEY PAD <5> */
	KA_LEFT				= VK_LEFT | WWKEY_VK_BIT,										/* <LEFT ARROW> */
	KA_PGUP				= VK_PRIOR | WWKEY_VK_BIT,										/* <PAGE UP> */
	KA_UPRIGHT 			= VK_PRIOR | WWKEY_VK_BIT,
	KA_UP					= VK_UP | WWKEY_VK_BIT,										/* <UP ARROW> */
	KA_HOME				= VK_HOME | WWKEY_VK_BIT,										/* <HOME> */
	KA_UPLEFT 			= VK_HOME | WWKEY_VK_BIT,
	KA_F12				= VK_F12 | WWKEY_VK_BIT,
	KA_F11				= VK_F11 | WWKEY_VK_BIT,
	KA_F10				= VK_F10 | WWKEY_VK_BIT,
	KA_F9 				= VK_F9 | WWKEY_VK_BIT,
	KA_F8					= VK_F8 | WWKEY_VK_BIT,
	KA_F7					= VK_F7 | WWKEY_VK_BIT,
	KA_F6					= VK_F6 | WWKEY_VK_BIT,
	KA_F5					= VK_F5 | WWKEY_VK_BIT,
	KA_F4					= VK_F4 | WWKEY_VK_BIT,
	KA_F3					= VK_F3 | WWKEY_VK_BIT,
	KA_F2					= VK_F2 | WWKEY_VK_BIT,
	KA_F1					= VK_F1 | WWKEY_VK_BIT,
	KA_LMOUSE 	 		= VK_LBUTTON | WWKEY_VK_BIT,
	KA_RMOUSE 	 		= VK_RBUTTON | WWKEY_VK_BIT,

	KA_SHIFT_BIT 		= WWKEY_SHIFT_BIT,
	KA_CTRL_BIT  		= WWKEY_CTRL_BIT,
	KA_ALT_BIT   		= WWKEY_ALT_BIT,
	KA_RLSE_BIT  		= WWKEY_RLS_BIT,

	//
	// Define all the KN types as variations of the KA types.  This is
	// so the KN functions will work properly under windows 95.
	//
	KN_NONE				= 0,
	KN_GRAVE 			= KA_GRAVE,
	KN_1 					= KA_1,
	KN_2 					= KA_2,
	KN_3 					= KA_3,
	KN_4 					= KA_4,
	KN_5 					= KA_5,
	KN_6 					= KA_6,
	KN_7 					= KA_7,
	KN_8 					= KA_8,
	KN_9 					= KA_9,
	KN_0 					= KA_0,
	KN_MINUS 			= KA_MINUS,									/* - */
	KN_EQUAL 			= KA_EQUAL,									/* = */

	KN_BACKSPACE		= KA_BACKSPACE,

	KN_TAB				= KA_TAB,										/* <TAB> */
	KN_Q 					= KA_q,
	KN_W 					= KA_w,
	KN_E 					= KA_e,
	KN_R 					= KA_r,
	KN_T 					= KA_t,
	KN_Y 					= KA_y,
	KN_U 					= KA_u,
	KN_I 					= KA_i,
	KN_O 					= KA_o,
	KN_P 					= KA_p,
	KN_LBRACKET			= KA_LBRACKET,								/* [ */
	KN_RBRACKET			= KA_RBRACKET,								/* ] */
	KN_BACKSLASH		= KA_BACKSLASH,								/* \ */


	KN_A 					= KA_a,
	KN_S 					= KA_s,
	KN_D 					= KA_d,
	KN_F 					= KA_f,
	KN_G 					= KA_g,
	KN_H 					= KA_h,
	KN_J 					= KA_j,
	KN_K 					= KA_k,
	KN_L 					= KA_l,
	KN_SEMICOLON 		= KA_SEMICOLON,								/* ; */
	KN_SQUOTE 			= KA_SQUOTE,									/* ' */
	KN_BACKSLASH2 		= KA_BACKSLASH,
	KN_RETURN 			= KA_RETURN,
	KN_Z 					= KA_z,
	KN_X 					= KA_x,
	KN_C 					= KA_c,
	KN_V 					= KA_v,
	KN_B 					= KA_b,
	KN_N 					= KA_n,
	KN_M 					= KA_m,
	KN_COMMA 			= KA_COMMA,									/* , */
	KN_PERIOD 			= KA_PERIOD,									/* . */
	KN_SLASH 			= KA_SLASH,									/* / */
	KN_SPACE 			= KA_SPACE,
	KN_LMOUSE			= KA_LMOUSE,
	KN_RMOUSE			= KA_RMOUSE,

	KN_HOME				= KA_HOME,										/* num key pad 7 */
	KN_UPLEFT			= KA_UPLEFT,
	KN_LEFT				= KA_LEFT,										/* num key pad 4 */
	KN_END				= KA_END,										/* num key pad 1 */
	KN_DOWNLEFT			= KA_DOWNLEFT,

	KN_KEYPAD_SLASH	= KA_SLASH,												/* num key pad / */
	KN_UP					= KA_UP,											/* num key pad 8 */
	KN_CENTER			= KA_KEYPAD5,									/* num key pad 5 */
	KN_DOWN				= KA_DOWN,										/* num key pad 2 */
	KN_INSERT			= KA_INSERT,									/* num key pad 0 */
	KN_KEYPAD_ASTERISK= KA_ASTERISK,									/* num key pad * */
	KN_PGUP				= KA_PGUP,										/* num key pad 9 */
	KN_UPRIGHT			= KA_UPRIGHT,
	KN_RIGHT				= KA_RIGHT,									/* num key pad 6 */
	KN_PGDN				= KA_PGDN,										/* num key pad 3 */
	KN_DOWNRIGHT		= KA_DOWNRIGHT,
	KN_DELETE			= KA_DELETE,									/* num key pad . */

	KN_KEYPAD_MINUS	= KA_MINUS,							/* num key pad - */
	KN_KEYPAD_PLUS		= KA_PLUS,							/* num key pad + */


	KN_KEYPAD_RETURN	= KA_RETURN,							/* num key pad <ENTER> */

	KN_ESC				= KA_ESC,
	KN_F1					= KA_F1,
	KN_F2					= KA_F2,
	KN_F3					= KA_F3,
	KN_F4					= KA_F4,
	KN_F5					= KA_F5,
	KN_F6					= KA_F6,
	KN_F7					= KA_F7,
	KN_F8					= KA_F8,
	KN_F9					= KA_F9,
	KN_F10				= KA_F10,
	KN_F11				= KA_F11,
	KN_F12				= KA_F12,

	KN_PRNTSCRN			= VK_PRINT | WWKEY_VK_BIT,
	KN_CAPSLOCK			= VK_CAPITAL | WWKEY_VK_BIT,
	KN_SCROLLLOCK		= VK_SCROLL | WWKEY_VK_BIT,								/* <SCROLL LOCK> */
	KN_PAUSE				= VK_PAUSE | WWKEY_VK_BIT,									/* <PAUSE> */
	KN_LSHIFT			= VK_SHIFT | WWKEY_VK_BIT,
	KN_RSHIFT			= VK_SHIFT | WWKEY_VK_BIT,
	KN_LCTRL				= VK_CONTROL | WWKEY_VK_BIT,
	KN_RCTRL				= VK_CONTROL | WWKEY_VK_BIT,
	KN_LALT				= VK_MENU | WWKEY_VK_BIT,
	KN_RALT				= VK_MENU | WWKEY_VK_BIT,
	KN_E_INSERT			= VK_INSERT | WWKEY_VK_BIT,
	KN_E_DELETE 		= VK_DELETE | WWKEY_VK_BIT,
	KN_E_LEFT			= VK_LEFT | WWKEY_VK_BIT,									/* extended <LEFT ARROW> */
	KN_E_HOME			= VK_HOME | WWKEY_VK_BIT,									/* extended <HOME> */
	KN_E_END				= VK_END | WWKEY_VK_BIT,									/* extended <END> */
	KN_E_UP				= VK_UP | WWKEY_VK_BIT,										/* extended <UP ARROW> */
	KN_E_DOWN			= VK_DOWN | WWKEY_VK_BIT,									/* extended <DOWN ARROW> */
	KN_E_PGUP			= VK_PRIOR | WWKEY_VK_BIT,									/* extended <PAGE UP> */
	KN_E_PGDN   		= VK_NEXT | WWKEY_VK_BIT,									/* extended <PAGE DOWN> */
	KN_E_RIGHT			= VK_RIGHT | WWKEY_VK_BIT,									/* extended <RIGHT ARROW> */
	KN_NUMLOCK			= VK_NUMLOCK | WWKEY_VK_BIT,									/* <NUM LOCK> */

	KN_SHIFT_BIT 		= WWKEY_SHIFT_BIT,
	KN_CTRL_BIT  		= WWKEY_CTRL_BIT | WWKEY_VK_BIT,
	KN_ALT_BIT   		= WWKEY_ALT_BIT | WWKEY_VK_BIT,
	KN_RLSE_BIT  		= WWKEY_RLS_BIT,
	KN_BUTTON    		= WWKEY_BTN_BIT,
};


extern WWKeyboardClass *_Kbd;


/*
** The following routines provide some compatability with the old westwood
** library.
*/
int Check_Key(void);
int Check_Key_Num(void);
int Get_Key_Num(void);
int Get_Key(void);
int KN_To_KA(int key);
void Clear_KeyBuffer(void);
int Key_Down(int key);
int KN_To_VK(int key);

#endif
