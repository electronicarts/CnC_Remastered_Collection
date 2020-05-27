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

/* $Header: /CounterStrike/EXPAND.CPP 7     3/17/97 1:05a Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : EXPAND.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 11/03/95                                                     *
 *                                                                                             *
 *                  Last Update : Mar 01, 1997 [V.Grippi]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   EListClass::Draw_Entry -- Draws entry for expansion scenario.                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

#ifdef FIXIT_VERSION_3
#include "WolStrng.h"
#endif

//#define CS_DEBUG

#define ARRAYOFFSET 20


/***********************************************************************************************
 * Expansion_CS_Present -- Is the Counterstrike expansion available?                           *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   true if counterstrike installed                                                   *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/5/97 1:59PM ST : Fixed to check for EXPAND.MIX                                         *
 *=============================================================================================*/
bool Expansion_CS_Present(void)
{
	//	ajw 9/29/98
	return Is_Counterstrike_Installed();
//	RawFileClass file("EXPAND.MIX");
//	return(file.Is_Available());
}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
/***********************************************************************************************
 * Expansion_AM_Present -- Is the Aftermath expansion available?                               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   true if AfterMath is installed                                                    *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    7/9/97 1:59PM BG : Fixed to check for EXPAND2.MIX                                        *
 *=============================================================================================*/
bool Expansion_AM_Present(void)
{
	//	ajw 9/29/98
	return Is_Aftermath_Installed();
//	RawFileClass file("EXPAND2.MIX");
//	return(file.Is_Available());
}
#endif


const char* ExpandNames[] = {
	"SCG20EA",
	"SCG21EA",
	"SCG22EA",
	"SCG23EA",
	"SCG24EA",
	"SCG26EA",
	"SCG27EA",
	"SCG28EA",
	"SCU31EA",
	"SCU32EA",
	"SCU33EA",
	"SCU34EA",
	"SCU35EA",
	"SCU36EA",
	"SCU37EA",
	"SCU38EA",
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	"SCG43EA",		// Harbor Reclamation
	"SCG41EA",		// In the nick of time
	"SCG40EA",		// Caught in the act
	"SCG42EA",		// Production Disruption
	"SCG47EA",		// Negotiations
	"SCG45EA",		// Monster Tank Madness
	"SCG44EA",		// Time Flies
	"SCG48EA",		// Absolut MADness
	"SCG46EA",		// Pawn

	"SCU43EA",		// Testing Grounds
	"SCU40EA",		// Shock Therapy
	"SCU42EA",		// Let's Make a Steal
	"SCU41EA",		// Test Drive
	"SCU45EA",		// Don't Drink The Water
	"SCU44EA",		// Situation Critical
	"SCU46EA",		// Brothers in Arms
	"SCU47EA",		// Deus Ex Machina
	"SCU48EA",		// Grunyev Revolution
#endif
	NULL
	};

const char* TestNames2[] = {
	"SCG01EA",
	"SCG02EA",
	"SCG03EA",
	"SCG04EA",
	"SCG05EA",
	"SCG06EA",
	"SCG07EA",
	"SCG08EA",
	"SCU01EA",
	"SCU02EA",
	"SCU03EA",
	"SCU04EA",
	"SCU05EA",
	"SCU06EA",
	"SCU07EA",
	"SCU08EA",
	"SCU09EA",
	NULL
};

#ifdef GERMAN
const char* XlatNames[] = {
	"Zusammenstoss",
	"Unter Tage",
	"Kontrollierte Verbrennung",
	"Griechenland 1 - Stavros",
	"Griechenland 2 - Evakuierung",
	"Sibirien 1 - Frische Spuren",
	"Sibirien 2 - In der Falle",
	"Sibirien 3 - Wildnis",
	"Das Feld der Ehre",
	"Belagerung",
	"Mausefalle",
	"Teslas Erbe",
	"Soldat Volkov",
	"Die Spitze der Welt",
	"Paradoxe Gleichung",
	"Nukleare Eskalation",
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	"Ein sicherer Hafen",			  //	"SCG43EA",		// Harbor Reclamation
	"Zeitkritische Routine",		  //	"SCG41EA",		// In the nick of time
	"Auf frischer Tat ertappt",	  //	"SCG40EA",		// Caught in the act
	"Drastischer Baustopp",			  //	"SCG42EA",		// Production Disruption
	"Harte Verhandlungen",			  //	"SCG47EA",		// Negotiations
	"Ferngelenktes Kriegsspielzeug",//	"SCG45EA",		// Monster Tank Madness
	"Licht aus",						  //	"SCG44EA",		// Time Flies
	"Molekulare Kriegsfhrung",	  //	"SCG48EA",		// Absolut MADness
	"Bauernopfer",						  //	"SCG46EA",		// Pawn

	"Testgel„nde",						  //	"SCU43EA",		// Testing Grounds
	"Schocktherapie",					  //	"SCU40EA",		// Shock Therapy
	"Der Letzte seiner Art",		  //	"SCU42EA",		// Let's Make a Steal
	"Probefahrt",						  //	"SCU41EA",		// Test Drive
	"Schlaftrunk",						  //	"SCU45EA",		// Don't Drink The Water
	"Der jngste Tag",				  //	"SCU44EA",		// Situation Critical
	"Waffenbrder",					  //	"SCU46EA",		// Brothers in Arms
	"Deus Ex Machina",				  //	"SCU47EA",		// Deus Ex Machina
	"Die Replikanten von Grunyev",  //	"SCU48EA",		// Grunyev Revolution

#endif
	NULL
};

#endif

#ifdef FRENCH
const char* XlatNames[] = {
	"Gaz Sarin 1: Ravitaillement Fatal",
	"Gaz Sarin 2: En Sous-sol",
	"Gaz Sarin 3: Attaque Chirurgicale",
	"GrŠce Occup‚e 1: Guerre Priv‚e",
	"GrŠce Occup‚e 2: Evacuation",
	"Conflit Sib‚rien 1: Traces FraŒches",
	"Conflit Sib‚rien 2: Pris au PiŠge",
	"Conflit Sib‚rien 3: Terres de Glace",
	"Mise … l'Epreuve",
	"Assi‚g‚s",
	"La SouriciŠre",
	"L'H‚ritage de Tesla",
	"Tandem de Choc",
	"Jusqu'au Sommet du Monde",
	"Effets Secondaires",
	"Intensification nucl‚aire",
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
"Le vieux port",				//	"SCG43EA",		// Harbor Reclamation
"Juste … temps",				//	"SCG41EA",		// In the nick of time
"La main dans le sac",		//	"SCG40EA",		// Caught in the act
"Production interrompue",	//	"SCG42EA",		// Production Disruption
"N‚gociations",				//	"SCG47EA",		// Negotiations
"Tanks en folie!",			//	"SCG45EA",		// Monster Tank Madness
"Le temps passe",				//	"SCG44EA",		// Time Flies
"D‚mence absolue",			//	"SCG48EA",		// Absolut MADness
"Le pion",						//	"SCG46EA",		// Pawn

"Terrains d'essais",			//	"SCU43EA",		// Testing Grounds
"Th‚rapie de choc",			//	"SCU40EA",		// Shock Therapy
"Au voleur!",					//	"SCU42EA",		// Let's Make a Steal
"Essai de conduite",			//	"SCU41EA",		// Test Drive
"Ne buvez pas la tasse",	//	"SCU45EA",		// Don't Drink The Water
"Situation critique",		//	"SCU44EA",		// Situation Critical
"FrŠres d'armes",				//	"SCU46EA",		// Brothers in Arms
"Deus Ex Machina",			//	"SCU47EA",		// Deus Ex Machina
"La R‚volution de Grunyev",//	"SCU48EA",		// Grunyev Revolution

#endif

	NULL,
};

#endif



#ifndef WIN32	 //VG

	#define	OPTION_WIDTH	236
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
#error Can never again build without WIN32 defined.
	#define	OPTION_HEIGHT	162
#else
	#define	OPTION_HEIGHT	162
#endif
	#define	OPTION_X			((320 - OPTION_WIDTH) / 2)
	#define	OPTION_Y			(200 - OPTION_HEIGHT) / 2

#else

	#define	OPTION_WIDTH	560
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	#define	OPTION_HEIGHT	332
#else
	#define	OPTION_HEIGHT	300
#endif
	#define	OPTION_X			((640 - OPTION_WIDTH) / 2)
	#define	OPTION_Y			(400 - OPTION_HEIGHT) / 2
#endif

struct EObjectClass
{
	HousesType House;
	int Scenario;
	char Name[128];
	char FullName[128];
};


/*
**	Derived from list class to handle expansion scenario listings. The listings
**	are recorded as EObjectClass objects. The data contained specifies the scenario
**	number, side, and text description.
*/
class EListClass : public ListClass
{
	public:
		EListClass(int id, int x, int y, int w, int h, TextPrintType flags, void const * up, void const * down) :
			ListClass(id, x, y, w, h, flags, up, down) {};

		virtual int Add_Object(EObjectClass * obj) {
			return(ListClass::Add_Item((char const *)obj));
		}
		virtual EObjectClass * Get_Object(int index) const {
			return((EObjectClass *)ListClass::Get_Item(index));
		}
		virtual EObjectClass * Current_Object(void) {
			return((EObjectClass *)ListClass::Current_Item());
		}

	protected:
		virtual void Draw_Entry(int index, int x, int y, int width, int selected);

	private:
		virtual int Add_Item(char const * text) {return(ListClass::Add_Item(text));};
		virtual int Add_Item(int text) {return(ListClass::Add_Item(text));};
		virtual char const * Current_Item(void) const {return(ListClass::Current_Item());};
		virtual char const * Get_Item(int index) const {return(ListClass::Get_Item(index));};
};


/***********************************************************************************************
 * EListClass::Draw_Entry -- Draws entry for expansion scenario.                               *
 *                                                                                             *
 *    This overrides the normal list class draw action so that the scenario name will be       *
 *    displayed along with the house name.                                                     *
 *                                                                                             *
 * INPUT:   index    -- The index of the entry that should be drawn.                           *
 *                                                                                             *
 *          x,y      -- Coordinate of upper left region to draw the entry into.                *
 *                                                                                             *
 *          width    -- Width of region (pixels) to draw the entry.                            *
 *                                                                                             *
 *          selected -- Is this entry considered selected?                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/17/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void EListClass::Draw_Entry(int index, int x, int y, int width, int selected)
{
	char buffer[128];
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	int text = TXT_NONE;
	if (Get_Object(index)->House == HOUSE_GOOD) {
		text = TXT_ALLIES;
	} else {
		text = TXT_SOVIET;
	}
	sprintf(buffer, "%s: %s", Text_String(text), Get_Object(index)->Name);

	TextPrintType flags = TextFlags;

	if (selected) {
		flags = flags | TPF_BRIGHT_COLOR;
		LogicPage->Fill_Rect (x, y, x + width - 1, y + LineHeight - 1, 1);
	} else {
		if (!(flags & TPF_USE_GRAD_PAL)) {
			flags = flags | TPF_MEDIUM_COLOR;
		}
	}

#ifndef WIN32
        Conquer_Clip_Text_Print(buffer, x, y, scheme, TBLACK, flags & ~(TPF_CENTER), width, Tabs);
#else
	Conquer_Clip_Text_Print(buffer, x + 100, y, scheme, TBLACK, flags & ~(TPF_CENTER), width, Tabs);
#endif
}

#ifdef FIXIT_VERSION_3
bool Expansion_Dialog( bool bCounterstrike )		//	If not bCounterstrike, then this was called for Aftermath.
#else
bool Expansion_Dialog(void)
#endif
{
	GadgetClass * buttons = NULL;

#ifndef WIN32
	TextButtonClass ok(200, TXT_OK, TPF_BUTTON, OPTION_X+40, OPTION_Y+OPTION_HEIGHT-15);
	TextButtonClass cancel(201, TXT_CANCEL, TPF_BUTTON, OPTION_X+OPTION_WIDTH-85, OPTION_Y+OPTION_HEIGHT-15);
#else
	TextButtonClass ok(200, TXT_OK, TPF_BUTTON, OPTION_X+40, OPTION_Y + OPTION_HEIGHT - 50 );
	TextButtonClass cancel(201, TXT_CANCEL, TPF_BUTTON, OPTION_X+OPTION_WIDTH-85, OPTION_Y + OPTION_HEIGHT - 50 );
#endif

#ifndef WIN32
	EListClass list(202, OPTION_X + 20, OPTION_Y+20, OPTION_WIDTH-40, OPTION_HEIGHT-40, TPF_BUTTON, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));
#else
	EListClass list(202, OPTION_X+35, OPTION_Y + 30, OPTION_WIDTH-70, OPTION_HEIGHT - 85, TPF_BUTTON, MFCD::Retrieve("BTN-UP.SHP"), MFCD::Retrieve("BTN-DN.SHP"));

#endif
	buttons = &ok;
	cancel.Add(*buttons);
	list.Add(*buttons);

	/*
	**	Add in all the expansion scenarios.
	*/
	CCFileClass file;
	char buffer[128], buffer2[128];
	char * sbuffer = (char*)_ShapeBuffer;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98 - Though disgusted.
	for (int index = 20; index < (36+18); index++) {
#else
	for (int index = 20; index < 36; index++) {
#endif

#ifndef CS_DEBUG
		strcpy(buffer,ExpandNames[index - 20]);
		strcpy(buffer2, ExpandNames[index - 20]);
#else
		strcpy(buffer, TestNames2[index]);
		strcpy(buffer2, TestNames2[index]);
#endif
		if(buffer[0] == NULL)
		   break;

     	strcat(buffer, ".INI");
		strcat(buffer2, ".INI");
		Scen.Set_Scenario_Name(buffer);
		Scen.Scenario = index;
		file.Set_Name(buffer);
#ifdef FIXIT_VERSION_3
		bool bOk;
		if( index < 36 )
			bOk = bCounterstrike;
		else
			bOk = !bCounterstrike;

		if( bOk && file.Is_Available() )
		{
#else	//	FIXIT_VERSION_3
		if (file.Is_Available()) {
#endif	//	FIXIT_VERSION_3
		    EObjectClass * obj = new EObjectClass;
            switch(buffer[2]){

		case 'G':
		case 'g':
			file.Read(sbuffer, 2000);
			sbuffer[2000] = '\r';
			sbuffer[2000+1] = '\n';
			sbuffer[2000+2] = '\0';
			WWGetPrivateProfileString("Basic", "Name", "x", buffer, sizeof(buffer), sbuffer);
#if defined(GERMAN) || defined(FRENCH)
			strcpy(obj->Name, XlatNames[index - ARRAYOFFSET]);
#else
			strcpy(obj->Name, buffer);
#endif
//			strcpy(obj->Name, buffer);
			strcpy(obj->FullName, buffer2);
			obj->House = HOUSE_GOOD;
			obj->Scenario = index;
			list.Add_Object(obj);
			break;

		case 'U':
		case 'u':
		  	file.Read(sbuffer, 2000);
		    	sbuffer[2000] = '\r';
		        sbuffer[2000+1] = '\n';
			sbuffer[2000+2] = '\0';
			WWGetPrivateProfileString("Basic", "Name", "x", buffer, sizeof(buffer), sbuffer);
#if defined(GERMAN) || defined(FRENCH)
			strcpy(obj->Name, XlatNames[index - ARRAYOFFSET]);
#else
			strcpy(obj->Name, buffer);
#endif
//		     	strcpy(obj->Name, buffer);
			strcpy(obj->FullName, buffer2);
			obj->House = HOUSE_BAD;
			obj->Scenario = index;
			list.Add_Object(obj);
	       		break;


		default:
			break;
		}
	    }
	}

	Set_Logic_Page(SeenBuff);
	bool recalc = true;
	bool display = true;
	bool process = true;
	bool okval = true;


	while (process) {

#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = true;
		}
#endif

		Call_Back();

		if (display) {
			display = false;

			Hide_Mouse();

			/*
			**	Load the background picture.
			*/
			Load_Title_Page();
			CCPalette.Set();

			Dialog_Box(OPTION_X, OPTION_Y, OPTION_WIDTH, OPTION_HEIGHT);
#ifdef FIXIT_VERSION_3
			if (bCounterstrike)
			{
				//PG Draw_Caption( TXT_WOL_CS_MISSIONS, OPTION_X, OPTION_Y, OPTION_WIDTH);
			}
			else
			{
				//PG Draw_Caption( TXT_WOL_AM_MISSIONS, OPTION_X, OPTION_Y, OPTION_WIDTH);
			}
#else
			Draw_Caption(TXT_NEW_MISSIONS, OPTION_X, OPTION_Y, OPTION_WIDTH);
#endif
			buttons->Draw_All();
			Show_Mouse();
		}

		KeyNumType input = buttons->Input();
		switch (input) {
			case 200|KN_BUTTON:
			      	Whom = list.Current_Object()->House;
				Scen.Scenario = list.Current_Object()->Scenario;
				strcpy(Scen.ScenarioName, list.Current_Object()->FullName);
				process = false;
				okval = true;
				break;

			case KN_ESC:
			case 201|KN_BUTTON:
				process = false;
				okval = false;
				break;

			case (KN_RETURN):
				Whom = list.Current_Object()->House;
				Scen.Scenario = list.Current_Object()->Scenario;
				strcpy(Scen.ScenarioName, list.Current_Object()->FullName);
				process = false;
				okval = true;
				break;

			default:
				break;
		}
	}

	/*
	**	Free up the allocations for the text lines in the list box.
	*/
 	for (int index = 0; index < list.Count(); index++) {
 		delete list.Get_Object(index);
 	}

	return(okval);

}

