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

/* $Header:   F:\projects\c&c\vcs\code\cdata.cpv   2.18   16 Oct 1995 16:50:22   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CDATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 16, 1994                                                 *
 *                                                                                             *
 *                  Last Update : July 29, 1994   [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TemplateTypeClass::Create_And_Place -- Creates and places a template object on the map.   *
 *   TemplateTypeClass::Create_One_Of -- Creates an object of this template type.              *
 *   TemplateTypeClass::Display -- Displays a generic representation of template.              *
 *   TemplateTypeClass::From_Name -- Determine template from ASCII name.                       *
 *   TemplateTypeClass::Init -- Loads graphic data for templates.                              *
 *   TemplateTypeClass::Occupy_List -- Determines occupation list.                             *
 *   TemplateTypeClass::One_Time -- Performs one-time initialization                           *
 *   TemplateTypeClass::Prep_For_Add -- Prepares to add template to scenario.                  *
 *   TemplateTypeClass::TemplateTypeClass -- Constructor for template type objects.            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


static char const _slope00000001[] = {7,-1};
static char const _slope000000101[] = {6,8,-1};
static char const _slope00000011[] = {6,7,-1};
static char const _slope0000001[] = {6,-1};
static char const _slope000001001[] = {5,8,-1};
static char const _slope000001[] = {5,-1};
static char const _slope000101[] = {3,5,-1};
static char const _slope00011010000100000001000011[] = {3,4,6,11,19,25,25,-1};
static char const _slope00011010010100100001000011[] = {3,4,6,9,11,14,19,24,25,-1};
static char const _slope0001[] = {3,-1};
static char const _slope001001001[] = {2,5,8,-1};
static char const _slope00110000000011[] = {2,3,12,13,-1};
static char const _slope00110010010011[] = {2,3,6,9,12,13,-1};
static char const _slope001111001[] = {2,3,4,5,8,-1};
static char const _slope0011[] = {2,3,-1};
static char const _slope001[] = {2,-1};
static char const _slope01000000000000000000001[] = {1,22,-1};
static char const _slope01000000100000010000001[] = {1,8,15,22,-1};
static char const _slope0111[] = {1,2,3,-1};
static char const _slope01[] = {1,-1};
static char const _slope1001001[] = {0,3,6,-1};
static char const _slope1001[] = {0,3,-1};
static char const _slope1100000000000000001100011[] = {0,1,18,19,23,24,-1};
static char const _slope1100001000001000001100011[] = {0,1,6,12,18,19,23,24,-1};
static char const _slope1101101[] = {0,1,3,4,6,-1};
static char const _slope1101[] = {0,1,3,-1};
static char const _slope111[] = {0,1,2,-1};
static char const _slope111010011[] = {0,1,2,4,7,8,-1};
static char const _slope11101[] = {0,1,2,4,-1};
static char const _slope111111011[] = {0,1,2,3,4,5,7,8,-1};
static char const _slope11111111[] = {0,1,2,3,4,5,6,7,-1};
static char const _slope111111[] = {0,1,2,3,4,5,-1};
static char const _slope1[] = {0,-1};

static TemplateTypeClass const Empty(
	TEMPLATE_CLEAR1,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE|THEATERF_JUNGLE,
	"CLEAR1",
	TXT_CLEAR,
	LAND_CLEAR,
	1,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Clear(
	TEMPLATE_CLEAR1,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE|THEATERF_JUNGLE,
	"CLEAR1",
	TXT_CLEAR,
	LAND_CLEAR,
	1,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road1(
	TEMPLATE_ROAD1,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D01",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road2(
	TEMPLATE_ROAD2,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D02",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road3(
	TEMPLATE_ROAD3,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D03",
	TXT_ROAD,
	LAND_CLEAR,
	1,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road4(
	TEMPLATE_ROAD4,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D04",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road5(
	TEMPLATE_ROAD5,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D05",
	TXT_ROAD,
	LAND_CLEAR,
	3,4,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road6(
	TEMPLATE_ROAD6,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D06",
	TXT_ROAD,
	LAND_CLEAR,
	2,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road7(
	TEMPLATE_ROAD7,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D07",
	TXT_ROAD,
	LAND_CLEAR,
	3,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road8(
	TEMPLATE_ROAD8,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D08",
	TXT_ROAD,
	LAND_CLEAR,
	3,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road9(
	TEMPLATE_ROAD9,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D09",
	TXT_ROAD,
	LAND_CLEAR,
	4,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road10(
	TEMPLATE_ROAD10,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D10",
	TXT_ROAD,
	LAND_CLEAR,
	4,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road11(
	TEMPLATE_ROAD11,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D11",
	TXT_ROAD,
	LAND_CLEAR,
	2,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road12(
	TEMPLATE_ROAD12,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D12",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road13(
	TEMPLATE_ROAD13,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D13",
	TXT_ROAD,
	LAND_CLEAR,
	4,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road14(
	TEMPLATE_ROAD14,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D14",
	TXT_ROAD,
	LAND_CLEAR,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road15(
	TEMPLATE_ROAD15,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D15",
	TXT_ROAD,
	LAND_CLEAR,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road16(
	TEMPLATE_ROAD16,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D16",
	TXT_ROAD,
	LAND_CLEAR,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road17(
	TEMPLATE_ROAD17,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D17",
	TXT_ROAD,
	LAND_CLEAR,
	3,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road18(
	TEMPLATE_ROAD18,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D18",
	TXT_ROAD,
	LAND_CLEAR,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road19(
	TEMPLATE_ROAD19,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D19",
	TXT_ROAD,
	LAND_CLEAR,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road20(
	TEMPLATE_ROAD20,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D20",
	TXT_ROAD,
	LAND_CLEAR,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road21(
	TEMPLATE_ROAD21,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D21",
	TXT_ROAD,
	LAND_CLEAR,
	3,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road22(
	TEMPLATE_ROAD22,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D22",
	TXT_ROAD,
	LAND_CLEAR,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road23(
	TEMPLATE_ROAD23,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D23",
	TXT_ROAD,
	LAND_CLEAR,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road24(
	TEMPLATE_ROAD24,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D24",
	TXT_ROAD,
	LAND_CLEAR,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road25(
	TEMPLATE_ROAD25,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D25",
	TXT_ROAD,
	LAND_CLEAR,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road26(
	TEMPLATE_ROAD26,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D26",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road27(
	TEMPLATE_ROAD27,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D27",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road28(
	TEMPLATE_ROAD28,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D28",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road29(
	TEMPLATE_ROAD29,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D29",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road30(
	TEMPLATE_ROAD30,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D30",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road31(
	TEMPLATE_ROAD31,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D31",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road32(
	TEMPLATE_ROAD32,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D32",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road33(
	TEMPLATE_ROAD33,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D33",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road34(
	TEMPLATE_ROAD34,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D34",
	TXT_ROAD,
	LAND_CLEAR,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road35(
	TEMPLATE_ROAD35,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D35",
	TXT_ROAD,
	LAND_CLEAR,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road36(
	TEMPLATE_ROAD36,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D36",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road37(
	TEMPLATE_ROAD37,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D37",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road38(
	TEMPLATE_ROAD38,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D38",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road39(
	TEMPLATE_ROAD39,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D39",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road40(
	TEMPLATE_ROAD40,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D40",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road41(
	TEMPLATE_ROAD41,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D41",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road42(
	TEMPLATE_ROAD42,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D42",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Road43(
	TEMPLATE_ROAD43,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"D43",
	TXT_ROAD,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Water(
	TEMPLATE_WATER,
	THEATERF_WINTER|THEATERF_TEMPERATE|THEATERF_DESERT,
	"W1",
	TXT_WATER,
	LAND_WATER,
	1,1,
	LAND_WATER,
	NULL
);
static TemplateTypeClass const Water2(
	TEMPLATE_WATER2,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"W2",
	TXT_WATER,
	LAND_WATER,
	2,2,
	LAND_WATER,
	NULL
);
static TemplateTypeClass const Shore1(
	TEMPLATE_SHORE1,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH1",
	TXT_WATER,
	LAND_WATER,
	3,3,
	LAND_BEACH,
	(char const *)_slope111111
);
static TemplateTypeClass const Shore2(
	TEMPLATE_SHORE2,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH2",
	TXT_WATER,
	LAND_ROCK,
	3,3,
	LAND_BEACH,
	(char const *)_slope111
);
static TemplateTypeClass const Shore3(
	TEMPLATE_SHORE3,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH3",
	TXT_WATER,
	LAND_ROCK,
	1,1,
	LAND_WATER,
	NULL
);
static TemplateTypeClass const Shore4(
	TEMPLATE_SHORE4,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH4",
	TXT_WATER,
	LAND_ROCK,
	2,1,
	LAND_WATER,
	NULL
);
static TemplateTypeClass const Shore5(
	TEMPLATE_SHORE5,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH5",
	TXT_WATER,
	LAND_WATER,
	3,3,
	LAND_BEACH,
	(char const *)_slope111111
);
static TemplateTypeClass const Shore6(
	TEMPLATE_SHORE6,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH6",
	TXT_WATER,
	LAND_WATER,
	3,3,
	LAND_BEACH,
	(char const *)_slope111111
);
static TemplateTypeClass const Shore7(
	TEMPLATE_SHORE7,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH7",
	TXT_WATER,
	LAND_WATER,
	2,2,
	LAND_BEACH,
	(char const *)_slope1
);
static TemplateTypeClass const Shore8(
	TEMPLATE_SHORE8,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH8",
	TXT_WATER,
	LAND_WATER,
	3,3,
	LAND_BEACH,
	(char const *)_slope11111111
);
static TemplateTypeClass const Shore9(
	TEMPLATE_SHORE9,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH9",
	TXT_WATER,
	LAND_WATER,
	3,3,
	LAND_BEACH,
	(char const *)_slope111111011
);
static TemplateTypeClass const Shore10(
	TEMPLATE_SHORE10,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH10",
	TXT_WATER,
	LAND_WATER,
	2,2,
	LAND_BEACH,
	(char const *)_slope01
);
static TemplateTypeClass const Shore11(
	TEMPLATE_SHORE11,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH11",
	TXT_WATER,
	LAND_WATER,
	3,3,
	LAND_BEACH,
	(char const *)_slope1001
);
static TemplateTypeClass const Shore12(
	TEMPLATE_SHORE12,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH12",
	TXT_WATER,
	LAND_WATER,
	3,3,
	LAND_BEACH,
	(char const *)_slope000001001
);
static TemplateTypeClass const Shore13(
	TEMPLATE_SHORE13,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH13",
	TXT_WATER,
	LAND_WATER,
	3,3,
	LAND_BEACH,
	(char const *)_slope0000001
);
static TemplateTypeClass const Shore14(
	TEMPLATE_SHORE14,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH14",
	TXT_WATER,
	LAND_ROCK,
	3,3,
	LAND_BEACH,
	(char const *)_slope00000011
);
static TemplateTypeClass const Shore15(
	TEMPLATE_SHORE15,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH15",
	TXT_WATER,
	LAND_ROCK,
	3,3,
	LAND_BEACH,
	(char const *)_slope000000101
);
static TemplateTypeClass const Shore16(
	TEMPLATE_SHORE16,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"SH16",
	TXT_WATER,
	LAND_ROCK,
	3,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Shore17(
	TEMPLATE_SHORE17,
	THEATERF_WINTER|THEATERF_TEMPERATE|THEATERF_DESERT,
	"SH17",
	TXT_WATER,
	LAND_WATER,
	2,2,
	LAND_WATER,
	NULL
);
static TemplateTypeClass const Shore18(
	TEMPLATE_SHORE18,
	THEATERF_WINTER|THEATERF_TEMPERATE|THEATERF_DESERT,
	"SH18",
	TXT_WATER,
	LAND_WATER,
	2,2,
	LAND_WATER,
	NULL
);
static TemplateTypeClass const Shore19(
	TEMPLATE_SHORE19,
	THEATERF_DESERT,
	"SH19",
	TXT_WATER,
	LAND_ROCK,
	3,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Shore20(
	TEMPLATE_SHORE20,
	THEATERF_DESERT,
	"SH20",
	TXT_WATER,
	LAND_ROCK,
	4,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Shore21(
	TEMPLATE_SHORE21,
	THEATERF_DESERT,
	"SH21",
	TXT_WATER,
	LAND_ROCK,
	3,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Shore22(
	TEMPLATE_SHORE22,
	THEATERF_DESERT,
	"SH22",
	TXT_WATER,
	LAND_ROCK,
	6,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Shore23(
	TEMPLATE_SHORE23,
	THEATERF_DESERT,
	"SH23",
	TXT_WATER,
	LAND_ROCK,
	2,2,
	LAND_CLEAR,
	(char const *)_slope01
);
static TemplateTypeClass const Shore24(
	TEMPLATE_SHORE24,
	THEATERF_DESERT,
	"SH24",
	TXT_WATER,
	LAND_ROCK,
	3,3,
	LAND_CLEAR,
	(char const *)_slope000001
);
static TemplateTypeClass const Shore25(
	TEMPLATE_SHORE25,
	THEATERF_DESERT,
	"SH25",
	TXT_WATER,
	LAND_ROCK,
	3,2,
	LAND_CLEAR,
	(char const *)_slope0001
);
static TemplateTypeClass const Shore26(
	TEMPLATE_SHORE26,
	THEATERF_DESERT,
	"SH26",
	TXT_WATER,
	LAND_ROCK,
	3,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore27(
	TEMPLATE_SHORE27,
	THEATERF_DESERT,
	"SH27",
	TXT_WATER,
	LAND_ROCK,
	4,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore28(
	TEMPLATE_SHORE28,
	THEATERF_DESERT,
	"SH28",
	TXT_WATER,
	LAND_ROCK,
	3,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore29(
	TEMPLATE_SHORE29,
	THEATERF_DESERT,
	"SH29",
	TXT_WATER,
	LAND_ROCK,
	6,2,
	LAND_CLEAR,
	(char const *)_slope00000001
);
static TemplateTypeClass const Shore30(
	TEMPLATE_SHORE30,
	THEATERF_DESERT,
	"SH30",
	TXT_WATER,
	LAND_ROCK,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore31(
	TEMPLATE_SHORE31,
	THEATERF_DESERT,
	"SH31",
	TXT_WATER,
	LAND_ROCK,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore32(
	TEMPLATE_SHORE32,
	THEATERF_TEMPERATE|THEATERF_WINTER,
	"SH32",
	TXT_WATER,
	LAND_CLEAR,
	3,3,
	LAND_WATER,
	(char const *)_slope1
);
static TemplateTypeClass const Shore33(
	TEMPLATE_SHORE33,
	THEATERF_TEMPERATE|THEATERF_WINTER,
	"SH33",
	TXT_WATER,
	LAND_CLEAR,
	3,3,
	LAND_WATER,
	(char const *)_slope001
);
static TemplateTypeClass const Shore34(
	TEMPLATE_SHORE34,
	THEATERF_TEMPERATE|THEATERF_WINTER,
	"SH34",
	TXT_WATER,
	LAND_CLEAR,
	3,3,
	LAND_WATER,
	(char const *)_slope001001001
);
static TemplateTypeClass const Shore35(
	TEMPLATE_SHORE35,
	THEATERF_TEMPERATE|THEATERF_WINTER,
	"SH35",
	TXT_WATER,
	LAND_CLEAR,
	3,3,
	LAND_WATER,
	(char const *)_slope1001001
);
static TemplateTypeClass const Shore36(
	TEMPLATE_SHORE36,
	THEATERF_DESERT,
	"SH36",
	TXT_WATER,
	LAND_CLEAR,
	1,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore37(
	TEMPLATE_SHORE37,
	THEATERF_DESERT,
	"SH37",
	TXT_WATER,
	LAND_CLEAR,
	1,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore38(
	TEMPLATE_SHORE38,
	THEATERF_DESERT,
	"SH38",
	TXT_WATER,
	LAND_CLEAR,
	1,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore39(
	TEMPLATE_SHORE39,
	THEATERF_DESERT,
	"SH39",
	TXT_WATER,
	LAND_CLEAR,
	1,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore40(
	TEMPLATE_SHORE40,
	THEATERF_DESERT,
	"SH40",
	TXT_WATER,
	LAND_WATER,
	3,3,
	LAND_CLEAR,
	(char const *)_slope1
);
static TemplateTypeClass const Shore41(
	TEMPLATE_SHORE41,
	THEATERF_DESERT,
	"SH41",
	TXT_WATER,
	LAND_CLEAR,
	3,3,
	LAND_WATER,
	(char const *)_slope1101101
);
static TemplateTypeClass const Shore42(
	TEMPLATE_SHORE42,
	THEATERF_DESERT,
	"SH42",
	TXT_WATER,
	LAND_WATER,
	1,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore43(
	TEMPLATE_SHORE43,
	THEATERF_DESERT,
	"SH43",
	TXT_WATER,
	LAND_WATER,
	1,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore44(
	TEMPLATE_SHORE44,
	THEATERF_DESERT,
	"SH44",
	TXT_WATER,
	LAND_WATER,
	1,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore45(
	TEMPLATE_SHORE45,
	THEATERF_DESERT,
	"SH45",
	TXT_WATER,
	LAND_WATER,
	1,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore46(
	TEMPLATE_SHORE46,
	THEATERF_DESERT,
	"SH46",
	TXT_WATER,
	LAND_WATER,
	3,3,
	LAND_CLEAR,
	(char const *)_slope1101
);
static TemplateTypeClass const Shore47(
	TEMPLATE_SHORE47,
	THEATERF_DESERT,
	"SH47",
	TXT_WATER,
	LAND_WATER,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore48(
	TEMPLATE_SHORE48,
	THEATERF_DESERT,
	"SH48",
	TXT_WATER,
	LAND_WATER,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore49(
	TEMPLATE_SHORE49,
	THEATERF_DESERT,
	"SH49",
	TXT_WATER,
	LAND_WATER,
	3,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore50(
	TEMPLATE_SHORE50,
	THEATERF_DESERT,
	"SH50",
	TXT_WATER,
	LAND_WATER,
	4,3,
	LAND_CLEAR,
	(char const *)_slope00000001
);
static TemplateTypeClass const Shore51(
	TEMPLATE_SHORE51,
	THEATERF_DESERT,
	"SH51",
	TXT_WATER,
	LAND_WATER,
	4,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore52(
	TEMPLATE_SHORE52,
	THEATERF_DESERT,
	"SH52",
	TXT_WATER,
	LAND_WATER,
	4,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore53(
	TEMPLATE_SHORE53,
	THEATERF_DESERT,
	"SH53",
	TXT_WATER,
	LAND_WATER,
	4,3,
	LAND_CLEAR,
	(char const *)_slope11101
);
static TemplateTypeClass const Shore54(
	TEMPLATE_SHORE54,
	THEATERF_DESERT,
	"SH54",
	TXT_WATER,
	LAND_WATER,
	3,2,
	LAND_CLEAR,
	(char const *)_slope1
);
static TemplateTypeClass const Shore55(
	TEMPLATE_SHORE55,
	THEATERF_DESERT,
	"SH55",
	TXT_WATER,
	LAND_WATER,
	3,2,
	LAND_CLEAR,
	(char const *)_slope001
);
static TemplateTypeClass const Shore56(
	TEMPLATE_SHORE56,
	THEATERF_DESERT,
	"SH56",
	TXT_WATER,
	LAND_WATER,
	3,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore57(
	TEMPLATE_SHORE57,
	THEATERF_DESERT,
	"SH57",
	TXT_WATER,
	LAND_WATER,
	3,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore58(
	TEMPLATE_SHORE58,
	THEATERF_DESERT,
	"SH58",
	TXT_WATER,
	LAND_WATER,
	2,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore59(
	TEMPLATE_SHORE59,
	THEATERF_DESERT,
	"SH59",
	TXT_WATER,
	LAND_WATER,
	2,3,
	LAND_CLEAR,
	(char const *)_slope1
);
static TemplateTypeClass const Shore60(
	TEMPLATE_SHORE60,
	THEATERF_DESERT,
	"SH60",
	TXT_WATER,
	LAND_WATER,
	2,3,
	LAND_CLEAR,
	(char const *)_slope000101
);
static TemplateTypeClass const Shore61(
	TEMPLATE_SHORE61,
	THEATERF_DESERT,
	"SH61",
	TXT_WATER,
	LAND_WATER,
	2,3,
	LAND_CLEAR,
	(char const *)_slope01
);
static TemplateTypeClass const Shore62(
	TEMPLATE_SHORE62,
	THEATERF_DESERT,
	"SH62",
	TXT_WATER,
	LAND_WATER,
	6,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Shore63(
	TEMPLATE_SHORE63,
	THEATERF_DESERT,
	"SH63",
	TXT_WATER,
	LAND_WATER,
	4,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Boulder1(
	TEMPLATE_BOULDER1,
	THEATERF_WINTER|THEATERF_TEMPERATE|THEATERF_DESERT,
	"B1",
	TXT_SLOPE,
	LAND_ROCK,
	1,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Boulder2(
	TEMPLATE_BOULDER2,
	THEATERF_WINTER|THEATERF_TEMPERATE|THEATERF_DESERT,
	"B2",
	TXT_SLOPE,
	LAND_ROCK,
	2,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Boulder3(
	TEMPLATE_BOULDER3,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"B3",
	TXT_SLOPE,
	LAND_ROCK,
	3,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Boulder4(
	TEMPLATE_BOULDER4,
	THEATERF_TEMPERATE,
	"B4",
	TXT_SLOPE,
	LAND_ROCK,
	1,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Boulder5(
	TEMPLATE_BOULDER5,
	THEATERF_TEMPERATE,
	"B5",
	TXT_SLOPE,
	LAND_ROCK,
	1,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Boulder6(
	TEMPLATE_BOULDER6,
	THEATERF_TEMPERATE,
	"B6",
	TXT_SLOPE,
	LAND_ROCK,
	1,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope1(
	TEMPLATE_SLOPE1,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S01",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_CLEAR,
	(char const *)_slope001
);
static TemplateTypeClass const Slope2(
	TEMPLATE_SLOPE2,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S02",
	TXT_SLOPE,
	LAND_ROCK,
	2,3,
	LAND_CLEAR,
	(char const *)_slope01
);
static TemplateTypeClass const Slope3(
	TEMPLATE_SLOPE3,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S03",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope4(
	TEMPLATE_SLOPE4,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S04",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope5(
	TEMPLATE_SLOPE5,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S05",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope6(
	TEMPLATE_SLOPE6,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S06",
	TXT_SLOPE,
	LAND_ROCK,
	2,3,
	LAND_CLEAR,
	(char const *)_slope1
);
static TemplateTypeClass const Slope7(
	TEMPLATE_SLOPE7,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S07",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope8(
	TEMPLATE_SLOPE8,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S08",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_CLEAR,
	(char const *)_slope01
);
static TemplateTypeClass const Slope9(
	TEMPLATE_SLOPE9,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S09",
	TXT_SLOPE,
	LAND_ROCK,
	3,2,
	LAND_CLEAR,
	(char const *)_slope0001
);
static TemplateTypeClass const Slope10(
	TEMPLATE_SLOPE10,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S10",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope11(
	TEMPLATE_SLOPE11,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S11",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope12(
	TEMPLATE_SLOPE12,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S12",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope13(
	TEMPLATE_SLOPE13,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S13",
	TXT_SLOPE,
	LAND_ROCK,
	3,2,
	LAND_CLEAR,
	(char const *)_slope000001
);
static TemplateTypeClass const Slope14(
	TEMPLATE_SLOPE14,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S14",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_CLEAR,
	(char const *)_slope0111
);
static TemplateTypeClass const Slope15(
	TEMPLATE_SLOPE15,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S15",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_CLEAR,
	(char const *)_slope01
);
static TemplateTypeClass const Slope16(
	TEMPLATE_SLOPE16,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S16",
	TXT_SLOPE,
	LAND_ROCK,
	2,3,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope17(
	TEMPLATE_SLOPE17,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S17",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope18(
	TEMPLATE_SLOPE18,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S18",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope19(
	TEMPLATE_SLOPE19,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S19",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope20(
	TEMPLATE_SLOPE20,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S20",
	TXT_SLOPE,
	LAND_ROCK,
	2,3,
	LAND_CLEAR,
	(char const *)_slope000001
);
static TemplateTypeClass const Slope21(
	TEMPLATE_SLOPE21,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S21",
	TXT_SLOPE,
	LAND_ROCK,
	1,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope22(
	TEMPLATE_SLOPE22,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S22",
	TXT_SLOPE,
	LAND_ROCK,
	2,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope23(
	TEMPLATE_SLOPE23,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S23",
	TXT_SLOPE,
	LAND_ROCK,
	3,2,
	LAND_CLEAR,
	(char const *)_slope000001
);
static TemplateTypeClass const Slope24(
	TEMPLATE_SLOPE24,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S24",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope25(
	TEMPLATE_SLOPE25,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S25",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope26(
	TEMPLATE_SLOPE26,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S26",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope27(
	TEMPLATE_SLOPE27,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S27",
	TXT_SLOPE,
	LAND_ROCK,
	3,2,
	LAND_CLEAR,
	(char const *)_slope0011
);
static TemplateTypeClass const Slope28(
	TEMPLATE_SLOPE28,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S28",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope29(
	TEMPLATE_SLOPE29,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S29",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope30(
	TEMPLATE_SLOPE30,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S30",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope31(
	TEMPLATE_SLOPE31,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S31",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope32(
	TEMPLATE_SLOPE32,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S32",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope33(
	TEMPLATE_SLOPE33,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S33",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope34(
	TEMPLATE_SLOPE34,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S34",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope35(
	TEMPLATE_SLOPE35,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S35",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope36(
	TEMPLATE_SLOPE36,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S36",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope37(
	TEMPLATE_SLOPE37,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S37",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Slope38(
	TEMPLATE_SLOPE38,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"S38",
	TXT_SLOPE,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Brush1(
	TEMPLATE_BRUSH1,
	THEATERF_DESERT,
	"BR1",
	TXT_BRUSH,
	LAND_ROCK,
	1,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Brush2(
	TEMPLATE_BRUSH2,
	THEATERF_DESERT,
	"BR2",
	TXT_BRUSH,
	LAND_ROCK,
	1,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Brush3(
	TEMPLATE_BRUSH3,
	THEATERF_DESERT,
	"BR3",
	TXT_BRUSH,
	LAND_ROCK,
	1,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Brush4(
	TEMPLATE_BRUSH4,
	THEATERF_DESERT,
	"BR4",
	TXT_BRUSH,
	LAND_ROCK,
	1,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Brush5(
	TEMPLATE_BRUSH5,
	THEATERF_DESERT,
	"BR5",
	TXT_BRUSH,
	LAND_ROCK,
	1,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Brush6(
	TEMPLATE_BRUSH6,
	THEATERF_DESERT,
	"BR6",
	TXT_BRUSH,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Brush7(
	TEMPLATE_BRUSH7,
	THEATERF_DESERT,
	"BR7",
	TXT_BRUSH,
	LAND_ROCK,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Brush8(
	TEMPLATE_BRUSH8,
	THEATERF_DESERT,
	"BR8",
	TXT_BRUSH,
	LAND_ROCK,
	3,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Brush9(
	TEMPLATE_BRUSH9,
	THEATERF_DESERT,
	"BR9",
	TXT_BRUSH,
	LAND_ROCK,
	3,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Brush10(
	TEMPLATE_BRUSH10,
	THEATERF_DESERT,
	"BR10",
	TXT_BRUSH,
	LAND_ROCK,
	2,1,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Patch1(
	TEMPLATE_PATCH1,
	THEATERF_TEMPERATE|THEATERF_DESERT,
	"P01",
	TXT_PATCH,
	LAND_CLEAR,
	1,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch2(
	TEMPLATE_PATCH2,
	THEATERF_TEMPERATE|THEATERF_DESERT,
	"P02",
	TXT_PATCH,
	LAND_ROCK,
	1,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch3(
	TEMPLATE_PATCH3,
	THEATERF_TEMPERATE|THEATERF_DESERT,
	"P03",
	TXT_PATCH,
	LAND_CLEAR,
	1,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch4(
	TEMPLATE_PATCH4,
	THEATERF_TEMPERATE|THEATERF_DESERT,
	"P04",
	TXT_PATCH,
	LAND_ROCK,
	1,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch5(
	TEMPLATE_PATCH5,
	THEATERF_DESERT,
	"P05",
	TXT_PATCH,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch6(
	TEMPLATE_PATCH6,
	THEATERF_DESERT,
	"P06",
	TXT_PATCH,
	LAND_CLEAR,
	6,4,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch7(
	TEMPLATE_PATCH7,
	THEATERF_WINTER|THEATERF_TEMPERATE|THEATERF_DESERT,
	"P07",
	TXT_PATCH,
	LAND_CLEAR,
	4,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch8(
	TEMPLATE_PATCH8,
	THEATERF_WINTER|THEATERF_TEMPERATE|THEATERF_DESERT,
	"P08",
	TXT_PATCH,
	LAND_CLEAR,
	3,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch13(
	TEMPLATE_PATCH13,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"P13",
	TXT_PATCH,
	LAND_CLEAR,
	3,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch14(
	TEMPLATE_PATCH14,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"P14",
	TXT_PATCH,
	LAND_CLEAR,
	2,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch15(
	TEMPLATE_PATCH15,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"P15",
	TXT_PATCH,
	LAND_CLEAR,
	1,1,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch16(
	TEMPLATE_PATCH16,
	THEATERF_WINTER,
	"P16",
	TXT_PATCH,
	LAND_CLEAR,
	2,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch17(
	TEMPLATE_PATCH17,
	THEATERF_WINTER,
	"P17",
	TXT_PATCH,
	LAND_CLEAR,
	4,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch18(
	TEMPLATE_PATCH18,
	THEATERF_WINTER,
	"P18",
	TXT_PATCH,
	LAND_CLEAR,
	4,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch19(
	TEMPLATE_PATCH19,
	THEATERF_WINTER,
	"P19",
	TXT_PATCH,
	LAND_CLEAR,
	4,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Patch20(
	TEMPLATE_PATCH20,
	THEATERF_WINTER,
	"P20",
	TXT_PATCH,
	LAND_CLEAR,
	4,3,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const River1(
	TEMPLATE_RIVER1,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"RV01",
	TXT_RIVER,
	LAND_WATER,
	5,4,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const River2(
	TEMPLATE_RIVER2,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"RV02",
	TXT_RIVER,
	LAND_WATER,
	5,3,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River3(
	TEMPLATE_RIVER3,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"RV03",
	TXT_RIVER,
	LAND_WATER,
	4,4,
	LAND_CLEAR,
	(char const *)_slope00000001
);
static TemplateTypeClass const River4(
	TEMPLATE_RIVER4,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"RV04",
	TXT_RIVER,
	LAND_WATER,
	4,4,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River5(
	TEMPLATE_RIVER5,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"RV05",
	TXT_RIVER,
	LAND_WATER,
	3,3,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River6(
	TEMPLATE_RIVER6,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"RV06",
	TXT_RIVER,
	LAND_WATER,
	3,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River7(
	TEMPLATE_RIVER7,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"RV07",
	TXT_RIVER,
	LAND_WATER,
	3,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River8(
	TEMPLATE_RIVER8,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"RV08",
	TXT_RIVER,
	LAND_WATER,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River9(
	TEMPLATE_RIVER9,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"RV09",
	TXT_RIVER,
	LAND_WATER,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River10(
	TEMPLATE_RIVER10,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"RV10",
	TXT_RIVER,
	LAND_WATER,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River11(
	TEMPLATE_RIVER11,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"RV11",
	TXT_RIVER,
	LAND_WATER,
	2,2,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River12(
	TEMPLATE_RIVER12,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"RV12",
	TXT_RIVER,
	LAND_WATER,
	3,4,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River13(
	TEMPLATE_RIVER13,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"RV13",
	TXT_RIVER,
	LAND_WATER,
	4,4,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River14(
	TEMPLATE_RIVER14,
	THEATERF_DESERT,
	"RV14",
	TXT_RIVER,
	LAND_WATER,
	4,3,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River15(
	TEMPLATE_RIVER15,
	THEATERF_DESERT,
	"RV15",
	TXT_RIVER,
	LAND_WATER,
	4,3,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River16(
	TEMPLATE_RIVER16,
	THEATERF_DESERT,
	"RV16",
	TXT_RIVER,
	LAND_WATER,
	6,4,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River17(
	TEMPLATE_RIVER17,
	THEATERF_DESERT,
	"RV17",
	TXT_RIVER,
	LAND_WATER,
	6,5,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River18(
	TEMPLATE_RIVER18,
	THEATERF_DESERT,
	"RV18",
	TXT_RIVER,
	LAND_WATER,
	4,4,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River19(
	TEMPLATE_RIVER19,
	THEATERF_DESERT,
	"RV19",
	TXT_RIVER,
	LAND_WATER,
	4,4,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River20(
	TEMPLATE_RIVER20,
	THEATERF_DESERT,
	"RV20",
	TXT_RIVER,
	LAND_WATER,
	6,8,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River21(
	TEMPLATE_RIVER21,
	THEATERF_DESERT,
	"RV21",
	TXT_RIVER,
	LAND_WATER,
	5,8,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River22(
	TEMPLATE_RIVER22,
	THEATERF_DESERT,
	"RV22",
	TXT_RIVER,
	LAND_WATER,
	3,3,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River23(
	TEMPLATE_RIVER23,
	THEATERF_DESERT,
	"RV23",
	TXT_RIVER,
	LAND_WATER,
	3,3,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River24(
	TEMPLATE_RIVER24,
	THEATERF_DESERT,
	"RV24",
	TXT_RIVER,
	LAND_WATER,
	3,3,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const River25(
	TEMPLATE_RIVER25,
	THEATERF_DESERT,
	"RV25",
	TXT_RIVER,
	LAND_WATER,
	3,3,
	LAND_ROCK,
	NULL
);
static TemplateTypeClass const Ford1(
	TEMPLATE_FORD1,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"FORD1",
	TXT_RIVER,
	LAND_WATER,
	3,3,
	LAND_CLEAR,
	(char const *)_slope001111001
);
static TemplateTypeClass const Ford2(
	TEMPLATE_FORD2,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"FORD2",
	TXT_RIVER,
	LAND_WATER,
	3,3,
	LAND_CLEAR,
	(char const *)_slope111010011
);
static TemplateTypeClass const Falls1(
	TEMPLATE_FALLS1,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"FALLS1",
	TXT_RIVER,
	LAND_WATER,
	3,3,
	LAND_CLEAR,
	(char const *)_slope1
);
static TemplateTypeClass const Falls2(
	TEMPLATE_FALLS2,
	THEATERF_WINTER|THEATERF_DESERT|THEATERF_TEMPERATE,
	"FALLS2",
	TXT_RIVER,
	LAND_WATER,
	3,2,
	LAND_CLEAR,
	NULL
);
static TemplateTypeClass const Bridge1(
	TEMPLATE_BRIDGE1,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"BRIDGE1",
	TXT_RIVER,
	LAND_WATER,
	4,4,
	LAND_CLEAR,
	(char const *)_slope00110010010011
);
static TemplateTypeClass const Bridge1d(
	TEMPLATE_BRIDGE1D,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"BRIDGE1D",
	TXT_RIVER,
	LAND_WATER,
	4,4,
	LAND_CLEAR,
	(char const *)_slope00110000000011
);
static TemplateTypeClass const Bridge2(
	TEMPLATE_BRIDGE2,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"BRIDGE2",
	TXT_RIVER,
	LAND_WATER,
	5,5,
	LAND_CLEAR,
	(char const *)_slope1100001000001000001100011
);
static TemplateTypeClass const Bridge2d(
	TEMPLATE_BRIDGE2D,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	"BRIDGE2D",
	TXT_RIVER,
	LAND_WATER,
	5,5,
	LAND_CLEAR,
	(char const *)_slope1100000000000000001100011
);
static TemplateTypeClass const Bridge3(
	TEMPLATE_BRIDGE3,
	THEATERF_DESERT,
	"BRIDGE3",
	TXT_RIVER,
	LAND_WATER,
	6,5,
	LAND_CLEAR,
	(char const *)_slope00011010010100100001000011
);
static TemplateTypeClass const Bridge3d(
	TEMPLATE_BRIDGE3D,
	THEATERF_DESERT,
	"BRIDGE3D",
	TXT_RIVER,
	LAND_WATER,
	6,5,
	LAND_CLEAR,
	(char const *)_slope00011010000100000001000011
);
static TemplateTypeClass const Bridge4(
	TEMPLATE_BRIDGE4,
	THEATERF_DESERT,
	"BRIDGE4",
	TXT_RIVER,
	LAND_WATER,
	6,4,
	LAND_CLEAR,
	(char const *)_slope01000000100000010000001
);
static TemplateTypeClass const Bridge4d(
	TEMPLATE_BRIDGE4D,
	THEATERF_DESERT,
	"BRIDGE4D",
	TXT_RIVER,
	LAND_WATER,
	6,4,
	LAND_CLEAR,
	(char const *)_slope01000000000000000000001
);

TemplateTypeClass const * const TemplateTypeClass::Pointers[TEMPLATE_COUNT] = {
	&Clear,			// TEMPLATE_CLEAR1
	&Water,			// TEMPLATE_WATER
	&Water2,			// TEMPLATE_WATER2
	&Shore1,			// TEMPLATE_SHORE1
	&Shore2,			// TEMPLATE_SHORE2
	&Shore3,			// TEMPLATE_SHORE3
	&Shore4,			// TEMPLATE_SHORE4
	&Shore5,			// TEMPLATE_SHORE5
	&Shore11,		//	TEMPLATE_SHORE11
	&Shore12,		// TEMPLATE_SHORE12
	&Shore13,		// TEMPLATE_SHORE13
	&Shore14,		// TEMPLATE_SHORE14
	&Shore15,		// TEMPLATE_SHORE15
	&Slope1,			//	TEMPLATE_SLOPE1
	&Slope2,			//	TEMPLATE_SLOPE2
	&Slope3,			//	TEMPLATE_SLOPE3
	&Slope4,			//	TEMPLATE_SLOPE4
	&Slope5,			//	TEMPLATE_SLOPE5
	&Slope6,			//	TEMPLATE_SLOPE6
	&Slope7,			//	TEMPLATE_SLOPE7
	&Slope8,			//	TEMPLATE_SLOPE8
	&Slope9,			//	TEMPLATE_SLOPE9
	&Slope10,		//	TEMPLATE_SLOPE10
	&Slope11,		//	TEMPLATE_SLOPE11
	&Slope12,		//	TEMPLATE_SLOPE12
	&Slope13,		//	TEMPLATE_SLOPE13
	&Slope14,		//	TEMPLATE_SLOPE14
	&Slope15,		//	TEMPLATE_SLOPE15
	&Slope16,		//	TEMPLATE_SLOPE16
	&Slope17,		//	TEMPLATE_SLOPE17
	&Slope18,		//	TEMPLATE_SLOPE18
	&Slope19,		//	TEMPLATE_SLOPE19
	&Slope20,		//	TEMPLATE_SLOPE20
	&Slope21,		//	TEMPLATE_SLOPE21
	&Slope22,		//	TEMPLATE_SLOPE22
	&Slope23,		//	TEMPLATE_SLOPE23
	&Slope24,		//	TEMPLATE_SLOPE24
	&Slope25,		//	TEMPLATE_SLOPE25
	&Slope26,		//	TEMPLATE_SLOPE26
	&Slope27,		//	TEMPLATE_SLOPE27
	&Slope28,		//	TEMPLATE_SLOPE28
	&Slope29,		//	TEMPLATE_SLOPE29
	&Slope30,		//	TEMPLATE_SLOPE30
	&Slope31,		//	TEMPLATE_SLOPE31
	&Slope32,		//	TEMPLATE_SLOPE32
	&Slope33,		//	TEMPLATE_SLOPE33
	&Slope34,		//	TEMPLATE_SLOPE34
	&Slope35,		//	TEMPLATE_SLOPE35
	&Slope36,		//	TEMPLATE_SLOPE36
	&Slope37,		//	TEMPLATE_SLOPE37
	&Slope38,		//	TEMPLATE_SLOPE38
	&Shore32,		// TEMPLATE_SHORE32
	&Shore33,		// TEMPLATE_SHORE33
	&Shore20,		// TEMPLATE_SHORE20
	&Shore21,		// TEMPLATE_SHORE21
	&Shore22,		//	TEMPLATE_SHORE22
	&Shore23,		// TEMPLATE_SHORE23
	&Brush1,			//	TEMPLATE_BRUSH1
	&Brush2,			//	TEMPLATE_BRUSH2
	&Brush3,			//	TEMPLATE_BRUSH3
	&Brush4,			//	TEMPLATE_BRUSH4
	&Brush5,			//	TEMPLATE_BRUSH5
	&Brush6,			//	TEMPLATE_BRUSH6
	&Brush7,			//	TEMPLATE_BRUSH7
	&Brush8,			//	TEMPLATE_BRUSH8
	&Brush9,			//	TEMPLATE_BRUSH9
	&Brush10,		//	TEMPLATE_BRUSH10
	&Patch1,			//	TEMPLATE_PATCH1
	&Patch2,			//	TEMPLATE_PATCH2
	&Patch3,			//	TEMPLATE_PATCH3
	&Patch4,			//	TEMPLATE_PATCH4
	&Patch5,			//	TEMPLATE_PATCH5
	&Patch6,			//	TEMPLATE_PATCH6
	&Patch7,			//	TEMPLATE_PATCH7
	&Patch8,			//	TEMPLATE_PATCH8
	&Shore16,		//	TEMPLATE_SHORE16
	&Shore17,		//	TEMPLATE_SHORE17
	&Shore18,		//	TEMPLATE_SHORE18
	&Shore19,		// TEMPLATE_SHORE19
	&Patch13,		//	TEMPLATE_PATCH13
	&Patch14,		//	TEMPLATE_PATCH14
	&Patch15,		//	TEMPLATE_PATCH15
	&Boulder1,		//	TEMPLATE_BOULDER1
	&Boulder2,		//	TEMPLATE_BOULDER2
	&Boulder3,		//	TEMPLATE_BOULDER3
	&Boulder4,		// TEMPLATE_BOULDER4
	&Boulder5,		//	TEMPLATE_BOULDER5
	&Boulder6,		//	TEMPLATE_BOULDER6
	&Shore6,			// TEMPLATE_SHORE6
	&Shore7,			// TEMPLATE_SHORE7
	&Shore8,			// TEMPLATE_SHORE8
	&Shore9,			// TEMPLATE_SHORE9
	&Shore10,		// TEMPLATE_SHORE10

	&Road1,			//	TEMPLATE_ROAD1
	&Road2,			//	TEMPLATE_ROAD2
	&Road3,			//	TEMPLATE_ROAD3
	&Road4,			//	TEMPLATE_ROAD4
	&Road5,			//	TEMPLATE_ROAD5
	&Road6,			//	TEMPLATE_ROAD6
	&Road7,			//	TEMPLATE_ROAD7
	&Road8,			//	TEMPLATE_ROAD8
	&Road9,			//	TEMPLATE_ROAD9
	&Road10,			//	TEMPLATE_ROAD10
	&Road11,			//	TEMPLATE_ROAD11
	&Road12,			//	TEMPLATE_ROAD12
	&Road13,			//	TEMPLATE_ROAD13
	&Road14,			//	TEMPLATE_ROAD14
	&Road15,			//	TEMPLATE_ROAD15
	&Road16,			//	TEMPLATE_ROAD16
	&Road17,			//	TEMPLATE_ROAD17
	&Road18,			//	TEMPLATE_ROAD18
	&Road19,			//	TEMPLATE_ROAD19
	&Road20,			//	TEMPLATE_ROAD20
	&Road21,			//	TEMPLATE_ROAD21
	&Road22,			//	TEMPLATE_ROAD22
	&Road23,			//	TEMPLATE_ROAD23
	&Road24,			//	TEMPLATE_ROAD24
	&Road25,			//	TEMPLATE_ROAD25
	&Road26,			//	TEMPLATE_ROAD26
	&Road27,			//	TEMPLATE_ROAD27
	&Road28,			//	TEMPLATE_ROAD28
	&Road29,			//	TEMPLATE_ROAD29
	&Road30,			//	TEMPLATE_ROAD30
	&Road31,			//	TEMPLATE_ROAD31
	&Road32,			//	TEMPLATE_ROAD32
	&Road33,			//	TEMPLATE_ROAD33
	&Road34,			//	TEMPLATE_ROAD34
	&Road35,			//	TEMPLATE_ROAD35
	&Road36,			//	TEMPLATE_ROAD36
	&Road37,			//	TEMPLATE_ROAD37
	&Road38,			//	TEMPLATE_ROAD38
	&Road39,			//	TEMPLATE_ROAD39
	&Road40,			//	TEMPLATE_ROAD40
	&Road41,			//	TEMPLATE_ROAD41
	&Road42,			//	TEMPLATE_ROAD42
	&Road43,			//	TEMPLATE_ROAD43

	&River1,			//	TEMPLATE_RIVER1
	&River2,			//	TEMPLATE_RIVER2
	&River3,			//	TEMPLATE_RIVER3
	&River4,			//	TEMPLATE_RIVER4
	&River5,			//	TEMPLATE_RIVER5
	&River6,			//	TEMPLATE_RIVER6
	&River7,			//	TEMPLATE_RIVER7
	&River8,			//	TEMPLATE_RIVER8
	&River9,			//	TEMPLATE_RIVER9
	&River10,		//	TEMPLATE_RIVER10
	&River11,		//	TEMPLATE_RIVER11
	&River12,		//	TEMPLATE_RIVER12
	&River13,		//	TEMPLATE_RIVER13
	&River14,		//	TEMPLATE_RIVER14
	&River15,		//	TEMPLATE_RIVER15
	&River16,		//	TEMPLATE_RIVER16
	&River17,		//	TEMPLATE_RIVER17
	&River18,		//	TEMPLATE_RIVER18
	&River19,		//	TEMPLATE_RIVER19
	&River20,		//	TEMPLATE_RIVER20
	&River21,		//	TEMPLATE_RIVER21
	&River22,		//	TEMPLATE_RIVER22
	&River23,		//	TEMPLATE_RIVER23
	&River24,		//	TEMPLATE_RIVER24
	&River25,		//	TEMPLATE_RIVER25
	&Ford1,			//	TEMPLATE_FORD1
	&Ford2,			//	TEMPLATE_FORD2
	&Falls1,			//	TEMPLATE_FALLS1
	&Falls2,			//	TEMPLATE_FALLS2
	&Bridge1,		//	TEMPLATE_BRIDGE1
	&Bridge1d,		//	TEMPLATE_BRIDGE1D
	&Bridge2,		//	TEMPLATE_BRIDGE2
	&Bridge2d,		//	TEMPLATE_BRIDGE2D
	&Bridge3,		//	TEMPLATE_BRIDGE3
	&Bridge3d,		//	TEMPLATE_BRIDGE3D
	&Bridge4,		//	TEMPLATE_BRIDGE4
	&Bridge4d,		//	TEMPLATE_BRIDGE4D

	&Shore24,		//	TEMPLATE_SHORE24
	&Shore25,		//	TEMPLATE_SHORE25
	&Shore26,		//	TEMPLATE_SHORE26
	&Shore27,		//	TEMPLATE_SHORE27
	&Shore28,		//	TEMPLATE_SHORE28
	&Shore29,		//	TEMPLATE_SHORE29
	&Shore30,		//	TEMPLATE_SHORE30
	&Shore31,		//	TEMPLATE_SHORE31

	&Patch16,		//	TEMPLATE_PATCH16
	&Patch17,		//	TEMPLATE_PATCH17
	&Patch18,		//	TEMPLATE_PATCH18
	&Patch19,		//	TEMPLATE_PATCH19
	&Patch20,		//	TEMPLATE_PATCH20

	&Shore34,		//	TEMPLATE_SHORE34
	&Shore35,		//	TEMPLATE_SHORE35
	&Shore36,		//	TEMPLATE_SHORE36
	&Shore37,		//	TEMPLATE_SHORE37
	&Shore38,		//	TEMPLATE_SHORE38
	&Shore39,		//	TEMPLATE_SHORE39
	&Shore40,		//	TEMPLATE_SHORE40
	&Shore41,		//	TEMPLATE_SHORE41
	&Shore42,		//	TEMPLATE_SHORE42
	&Shore43,		//	TEMPLATE_SHORE43
	&Shore44,		//	TEMPLATE_SHORE44
	&Shore45,		//	TEMPLATE_SHORE45

	&Shore46,		//	TEMPLATE_SHORE46
	&Shore47,		//	TEMPLATE_SHORE47
	&Shore48,		//	TEMPLATE_SHORE48
	&Shore49,		//	TEMPLATE_SHORE49
	&Shore50,		//	TEMPLATE_SHORE50
	&Shore51,		//	TEMPLATE_SHORE51
	&Shore52,		//	TEMPLATE_SHORE52
	&Shore53,		//	TEMPLATE_SHORE53
	&Shore54,		//	TEMPLATE_SHORE54
	&Shore55,		//	TEMPLATE_SHORE55
	&Shore56,		//	TEMPLATE_SHORE56
	&Shore57,		//	TEMPLATE_SHORE57
	&Shore58,		//	TEMPLATE_SHORE58
	&Shore59,		//	TEMPLATE_SHORE59
	&Shore60,		//	TEMPLATE_SHORE60
	&Shore61,		//	TEMPLATE_SHORE61

	&Shore62,		//	TEMPLATE_SHORE62
	&Shore63,		//	TEMPLATE_SHORE63
};


/***********************************************************************************************
 * TemplateTypeClass::TemplateTypeClass -- Constructor for template type objects.              *
 *                                                                                             *
 *    This is the constructor for the template types.                                          *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TemplateTypeClass::TemplateTypeClass(TemplateType iconset, int theater,
	char const *ininame, int fullname, LandType land,
	int width, int height, LandType altland, char const *alticons ) :
		ObjectTypeClass(false, false, false, true, false, false, true, true, fullname, ininame, ARMOR_NONE, 0)
{
	Theater = theater;
	AltIcons = alticons;
	AltLand = altland;
	Type = iconset;
	Land = land;
	Width = width;
	Height = height;
}


/***********************************************************************************************
 * TemplateTypeClass::From_Name -- Determine template from ASCII name.                         *
 *                                                                                             *
 *    This routine is used to determine the template number given only                         *
 *    an ASCII representation. The scenario loader uses this routine                           *
 *    to construct the map from the INI control file.                                          *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the ASCII name of the template.                                *
 *                                                                                             *
 * OUTPUT:  Returns with the template number. If the name had no match,                        *
 *          then returns with TEMPLATE_NONE.                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TemplateType TemplateTypeClass::From_Name(char const *name)
{
	if (name) {
		for (TemplateType index = TEMPLATE_FIRST; index < TEMPLATE_COUNT; index++) {
			if (stricmp(As_Reference(index).IniName, name) == 0) {
				return(index);
			}
		}
	}
	return(TEMPLATE_NONE);
}


/***********************************************************************************************
 * TemplateTypeClass::Occupy_List -- Determines occupation list.                               *
 *                                                                                             *
 *    This routine is used to examine the template map and build an                            *
 *    occupation list. This list is used to render a template cursor as                        *
 *    well as placement of icon numbers.                                                       *
 *                                                                                             *
 * INPUT:   placement   -- Is this for placement legality checking only? The normal condition  *
 *                         is for marking occupation flags.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the template occupation list.                            *
 *                                                                                             *
 * WARNINGS:   The return pointer is valid only until the next time that                       *
 *             this routine is called.                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * TemplateTypeClass::Occupy_List(bool) const
{
	static short _occupy[13*8+5];
	unsigned char	map[13*8];
	short	*ptr;
	int	index;

	Mem_Copy(Get_Icon_Set_Map(Get_Image_Data()), map, Width*Height);

	ptr = &_occupy[0];
	for (index = 0; index < Width*Height; index++) {
		if (map[index] != 0xFF) {
			*ptr++ = (index % Width) + ((index / Width)*MAP_CELL_W);
		}
	}
	*ptr = REFRESH_EOL;

	return((short const *)&_occupy[0]);
}


/***********************************************************************************************
 * TemplateTypeClass::Init -- Loads graphic data for templates.                                *
 *                                                                                             *
 *    This routine loads the template graphic data for all the template                        *
 *    type supported for the specified theater. This routine is called                         *
 *    whenever the theater for the scenario is first determined.                               *
 *                                                                                             *
 * INPUT:   theater  -- The theater that the template data is to be                            *
 *                      loaded for.                                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine goes to disk!                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *   06/02/1994 JLB : Only handles iconset loading now (as it should).                         *
 *=============================================================================================*/
void TemplateTypeClass::Init(TheaterType theater)
{
	//if (theater != LastTheater){
		char fullname[_MAX_FNAME+_MAX_EXT];	// Fully constructed iconset name.
		void const * ptr;		// Working loaded iconset pointer.

		for (TemplateType index = TEMPLATE_FIRST; index < TEMPLATE_COUNT; index++) {
			TemplateTypeClass	const & tplate = As_Reference(index);

			((void const *&)tplate.ImageData) = NULL;
			if (tplate.Theater & (1<<theater)) {
				_makepath(fullname, NULL, NULL, tplate.IniName, Theaters[theater].Suffix);
				ptr = MixFileClass::Retrieve(fullname);
				((void const *&)tplate.ImageData) = ptr;
// No need for icon caching now. ST - 12/19/2018 11:48AM				
//				Register_Icon_Set((void*)ptr,TRUE);		//Register icon set for video memory caching
			}
		}
	//}
}


#ifdef SCENARIO_EDITOR
/***********************************************************************************************
 * TemplateTypeClass::Display -- Displays a generic representation of template.                *
 *                                                                                             *
 *    This routine is used to display a generic view of the template                           *
 *    object. This is necessary for selection in the scenario editor.                          *
 *                                                                                             *
 * INPUT:   x,y   -- The coordinates to center the display about.                              *
 *                                                                                             *
 *          window-- The window to base the coordinates upon.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TemplateTypeClass::Display(int x, int y, WindowNumberType window, HousesType ) const
{
	int	w,h;
	int	index;
	unsigned char map[13*8];
	bool	scale;		// Should the template be half sized?

	w = Bound(Width, 1, 13);
	h = Bound(Height, 1, 8);
	scale = (w > 3 || h > 3);
	if (scale) {
		x -= (w/2) * (ICON_PIXEL_W/2);
		y -= (h/2) * (ICON_PIXEL_H/2);
	} else {
		x -= (w/2) * ICON_PIXEL_W;
		y -= (h/2) * ICON_PIXEL_H;
	}
	x += WindowList[window][WINDOWX]<<3;
	y += WindowList[window][WINDOWY];

	Mem_Copy(Get_Icon_Set_Map(Get_Image_Data()), map, Width*Height);

	for (index = 0; index < w*h; index++) {
		if (map[index] != 0xFF) {
			HidPage.Draw_Stamp(Get_Image_Data(), index, 0, 0, NULL, WINDOW_MAIN);
			if (scale) {

				HidPage.Scale((*LogicPage), 0, 0,
					x + ((index % w)*(ICON_PIXEL_W/2)),
					y + ((index / w)*(ICON_PIXEL_H/2)),
					ICON_PIXEL_W, ICON_PIXEL_H,
					ICON_PIXEL_W/2, ICON_PIXEL_H/2, (char *)NULL);

			} else {
				HidPage.Blit((*LogicPage), 0, 0, x + ((index % w)*(ICON_PIXEL_W)),
					y + ((index / w)*(ICON_PIXEL_H)), ICON_PIXEL_W, ICON_PIXEL_H);
			}
		}
	}
}


/***********************************************************************************************
 * TemplateTypeClass::Prep_For_Add -- Prepares to add template to scenario.                    *
 *                                                                                             *
 *    This routine prepares a list of template objects so that the                             *
 *    scenario editor can use this list to display a dialog box. The                           *
 *    selection of a template object will allow its placement upon the                         *
 *    map.                                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *   05/28/1994 JLB : Only handles real templates now.                                         *
 *   06/04/1994 JLB : Uses map editing interface functions.                                    *
 *=============================================================================================*/
void TemplateTypeClass::Prep_For_Add(void)
{
	for (TemplateType index = TEMPLATE_CLEAR1; index < TEMPLATE_COUNT; index++) {
		if (As_Reference(index).Get_Image_Data()) {
			Map.Add_To_List(&As_Reference(index));
		}
	}
}
#endif


/***********************************************************************************************
 * TemplateTypeClass::Create_And_Place -- Creates and places a template object on the map.     *
 *                                                                                             *
 *    This support routine is used by the scenario editor to add a template object to the map  *
 *    and to the game.                                                                         *
 *                                                                                             *
 * INPUT:   cell  -- The cell to place the template object.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the template object placed successfully?                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TemplateTypeClass::Create_And_Place(CELL cell, HousesType ) const
{
	if (new TemplateClass(Type, cell)) {
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TemplateTypeClass::Create_One_Of -- Creates an object of this template type.                *
 *                                                                                             *
 *    This routine will create an object of this type. For certain template objects, such      *
 *    as walls, it is actually created as a building. The "building" wall is converted into    *
 *    a template at the moment of placing down on the map.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the appropriate object for this template type.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * TemplateTypeClass::Create_One_Of(HouseClass *) const
{
	return(new TemplateClass(Type, -1));
}


/***********************************************************************************************
 * TemplateTypeClass::One_Time -- Performs one-time initialization                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TemplateTypeClass::One_Time(void)
{
}



