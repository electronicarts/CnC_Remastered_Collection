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


/*************************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S                  **
 *************************************************************************************
 *                                                                                   *
 *                 Project Name : Command & Conquer - Red Alert                      *
 *                                                                                   *
 *                    File Name : SENDFILE.CPP                                       *
 *                                                                                   *
 *                   Programmer : Steve Tall                                         *
 *                                                                                   *
 *                   Start Date : Audust 20th, 1996                                  *
 *                                                                                   *
 *                  Last Update : August 20th, 1996 [ST]                             *
 *                                                                                   *
 *-----------------------------------------------------------------------------------*
 * Overview:                                                                         *
 *                                                                                   *
 *  Functions for scenario file transfer between machines                            *
 *                                                                                   *
 *-----------------------------------------------------------------------------------*
 * Functions:                                                             	 			*
 *                                                                         				*
 *                                                                         				*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#if (0)//PG
#include "function.h"

//#include "WolDebug.h"

#ifdef WINSOCK_IPX
#include "WSProto.h"
#else

#ifdef WIN32
#include	"tcpip.h"
#else
#include	"fakesock.h"
#endif	//WIN32

#endif	//WINSOCK_IPX

bool Receive_Remote_File ( char *file_name, unsigned int file_length, int gametype);
bool Send_Remote_File ( char *file_name, int gametype );

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
extern bool Is_Mission_Counterstrike (char *file_name);
extern bool Is_Mission_Aftermath (char *file_name);
#endif

#define RESPONSE_TIMEOUT	60*60

#ifdef WOLAPI_INTEGRATION
#include "WolapiOb.h"
extern WolapiObject* pWolapi;
#endif

/***********************************************************************************************
 * Get_Scenario_File_From_Host -- Initiates download of scenario file from game host           *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to buffer to copy file name into                                              *
 *           game type - 0 for modem/null modem, 1 otherwise                                   *
 *                                                                                             *
 * OUTPUT:   true if file sucessfully downloaded                                               *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/22/96 3:06PM ST : Created                                                              *
 *=============================================================================================*/
bool Get_Scenario_File_From_Host(char *return_name, int gametype)
{
	//WWDebugString ("RA95 - In Get_Scenario_From_Host\n");

	unsigned int file_length;

	SerialPacketType 	send_packet;
	SerialPacketType 	receive_packet;
	GlobalPacketType	net_send_packet;
	GlobalPacketType	net_receive_packet;
	unsigned int		packet_len;
	unsigned short		product_id;

	IPXAddressClass	sender_address;

	CDTimerClass<SystemTimerClass> response_timer;		// timeout timer for waiting for responses

	/*
	** Send the scenario request using guaranteed delivery.
	*/
	if (!gametype) {
		memset ((void*)&send_packet, 0, sizeof (send_packet));
		send_packet.Command = SERIAL_REQ_SCENARIO;
		NullModem.Send_Message (&send_packet, sizeof(send_packet), 1);
	} else {
		memset ((void*)&net_send_packet, 0, sizeof (net_send_packet));
		net_send_packet.Command = NET_REQ_SCENARIO;
		Ipx.Send_Global_Message (&net_send_packet, sizeof (net_send_packet),
			1, &(Session.HostAddress) );
	}



	//WWDebugString ("RA95 - Waiting for response from host\n");

	/*
	** Wait for host to respond with a file info packet
	*/
	response_timer = RESPONSE_TIMEOUT;
	if (!gametype){
		do {
			NullModem.Service();

			if (NullModem.Get_Message ((void*)&receive_packet, (int*)&packet_len) > 0) {

				if (receive_packet.Command == SERIAL_FILE_INFO){
					strcpy (return_name, receive_packet.ScenarioInfo.ShortFileName);
					file_length = receive_packet.ScenarioInfo.FileLength;
					break;
				}
			}
		} while ( response_timer );
	}else{
		do {
			Ipx.Service();
			int receive_packet_length = sizeof (net_receive_packet);
			if (Ipx.Get_Global_Message (&net_receive_packet, &receive_packet_length,
				&sender_address, &product_id)){

//WWDebugString ("RA95 - Got packet from host\n");
#ifdef WINSOCK_IPX
				if (net_receive_packet.Command == NET_FILE_INFO && sender_address == Session.HostAddress) {
#else	//WINSOCK_IPX
				if (net_receive_packet.Command == NET_FILE_INFO &&
						(Winsock.Get_Connected() || sender_address == Session.HostAddress)){
#endif	//WINSOCK_IPX
					strcpy (return_name, net_receive_packet.ScenarioInfo.ShortFileName);
					file_length = net_receive_packet.ScenarioInfo.FileLength;
//WWDebugString ("RA95 - Got file info packet from host\n");
					break;
				}
			}

#ifdef WOLAPI_INTEGRATION
			if( Session.Type == GAME_INTERNET && pWolapi && ( ::timeGetTime() > pWolapi->dwTimeNextWolapiPump ) )
			{
				pWolapi->pChat->PumpMessages();
				pWolapi->dwTimeNextWolapiPump = ::timeGetTime() + WOLAPIPUMPWAIT;
			}
#endif
		} while ( response_timer );
	}

//char rt[80];
//sprintf (rt, "RA95 - response_timer = %d\n", response_timer );
//WWDebugString (rt);

	/*
	** If we timed out then something horrible has happened to the other player so just
	** return failure.
	*/
	if (!response_timer) return (false);

//	debugprint( "about to download '%s'\n", return_name );

	/*
	** Receive the file from the host
	*/
	return (Receive_Remote_File ( return_name, file_length, gametype));
}







/***********************************************************************************************
 * Receive_Remote_File -- Handles incoming file download packets from the game host            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    file name to save as                                                              *
 *           length of file to expect                                                          *
 *           game type - 0 for modem/null modem, 1 otherwise                                   *
 *                                                                                             *
 * OUTPUT:   true if file downloaded was completed                                             *
 *                                                                                             *
 * WARNINGS: This fuction can modify the file name passed in                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   8/22/96 3:07PM ST : Created                                                               *
 *=============================================================================================*/
bool Receive_Remote_File ( char *file_name, unsigned int file_length, int gametype)
{

	//WWDebugString ("RA95 - In Receive_Remote_File\n");
	unsigned short		product_id;
	IPXAddressClass	sender_address;

	/*
	** Dialog & button dimensions
	*/
	int d_dialog_w = 200 *RESFACTOR;											// dialog width
	int d_dialog_h = 90*RESFACTOR;											// dialog height
	int d_dialog_x = ((320*RESFACTOR - d_dialog_w) / 2);				// dialog x-coord
	int d_dialog_y = ((200*RESFACTOR - d_dialog_h) / 2);				// centered y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_margin1=10;
	int d_txt6_h=15;

#if (GERMAN | FRENCH)
	int d_cancel_w = 50*RESFACTOR;
#else
	int d_cancel_w = 40*RESFACTOR;
#endif
	int d_cancel_h = 9*RESFACTOR;
	int d_cancel_x = d_dialog_cx - d_cancel_w / 2;
	int d_cancel_y = d_dialog_y + d_dialog_h - 20*RESFACTOR;

	int d_progress_w = 100*RESFACTOR;
	int d_progress_h = 10*RESFACTOR;
	int d_progress_x = (SeenBuff.Get_Width()/2) - d_progress_w/2;
	int d_progress_y = d_dialog_y + 45*RESFACTOR;

	int	width;
	int	height;

	char *info_string = (char*)Text_String (TXT_RECEIVING_SCENARIO);

	Fancy_Text_Print(TXT_NONE, 0, 0, GadgetClass::Get_Color_Scheme(),
		TBLACK, TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);

	Format_Window_String(info_string, SeenBuff.Get_Height(), width, height);


	/*
	** Button Enumerations
	*/
	enum {
		BUTTON_CANCEL = 100,
		BUTTON_PROGRESS
	};

	/*
	** Buttons
	*/
	//TextButtonClass *buttons;										// button list

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
#if (GERMAN | FRENCH)
		d_cancel_x, d_cancel_y);
#else
		d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);
#endif

	GaugeClass progress_meter (BUTTON_PROGRESS,
		d_progress_x, d_progress_y, d_progress_w, d_progress_h);

	Fancy_Text_Print(TXT_NONE, 0, 0, GadgetClass::Get_Color_Scheme(),
		TBLACK, TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);


	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_PROGRESS,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;


	bool 			process = true;
	RedrawType 	display = REDRAW_ALL;		// redraw level
	KeyNumType 	input;
	GadgetClass *commands;					// button list
	bool			return_code;
	int			update_time = 0;



	RemoteFileTransferType receive_packet;

	int 				last_received_block = -1;		//No blocks received yet
	unsigned int 	total_length = 0;
	unsigned int	packet_len;

	/*
	** If the file name is already in use, use the temp file name
	*/
	CCFileClass test_file (file_name);
	if (test_file.Is_Available()){
		strcpy (file_name, "DOWNLOAD.TMP");
	}

	RawFileClass save_file (file_name);

	/*
	** If the file already exists then delete it and re-create it.
	*/
	if (save_file.Is_Available()) save_file.Delete();

	/*
	** Open the file for write
	*/
	save_file.Open ( WRITE );

	commands = &cancelbtn;
	commands->Add_Tail (progress_meter);

	progress_meter.Set_Maximum(100);		// Max is 100%
	progress_meter.Set_Value(0);			// Current is 0%

	/*
	** Wait for all the blocks to arrive
	*/

	do	{
#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = REDRAW_ALL;
		}
#endif

#ifdef WOLAPI_INTEGRATION
		if( Session.Type == GAME_INTERNET && pWolapi && ( ::timeGetTime() > pWolapi->dwTimeNextWolapiPump ) )
		{
			pWolapi->pChat->PumpMessages();
			pWolapi->dwTimeNextWolapiPump = ::timeGetTime() + WOLAPIPUMPWAIT;
		}
#endif

		if (display){

			if (display >= REDRAW_BACKGROUND){

				Hide_Mouse();
				/*
				** Redraw backgound & dialog box
				*/
				Load_Title_Page(true);
				Set_Palette(CCPalette);

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				/*
				** Dialog & Field labels
				*/
				Draw_Caption (TXT_NONE, d_dialog_x, d_dialog_y, d_dialog_w);

				Fancy_Text_Print(info_string, d_dialog_cx-width/2, d_dialog_y + 25*RESFACTOR,
									GadgetClass::Get_Color_Scheme(), TBLACK,
									TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Show_Mouse();

			}

			if (display >= REDRAW_BUTTONS){

				commands->Draw_All();

			}

			if (display >= REDRAW_PROGRESS){
				progress_meter.Draw_Me(true);
			}

			display = REDRAW_NONE;
		}


		if (!gametype){
			NullModem.Service();

			if (NullModem.Get_Message ((void*)&receive_packet, (int*)&packet_len) > 0) {

				if (receive_packet.Command == NET_FILE_CHUNK){

					if (receive_packet.BlockNumber == last_received_block + 1){

						save_file.Write ( receive_packet.RawData, receive_packet.BlockLength );
						total_length += receive_packet.BlockLength;
						last_received_block ++;

						update_time++;
						if (update_time >7){
							progress_meter.Set_Value ( (total_length*100) / file_length );
							display = REDRAW_PROGRESS;
							update_time = 0;;
						}

						if (total_length >= file_length){
							process = false;
							return_code = true;
							progress_meter.Set_Value ( 100 );
							progress_meter.Draw_Me(true);
						}
					}
				}
			}
		}else{
			Ipx.Service();

			int receive_packet_len = sizeof (receive_packet);
			if (Ipx.Get_Global_Message (&receive_packet, &receive_packet_len,
				&sender_address, &product_id)){

#ifdef WINSOCK_IPX
				if (receive_packet.Command == NET_FILE_CHUNK && sender_address == Session.HostAddress){
#else	//WINSOCK_IPX
				if (receive_packet.Command == NET_FILE_CHUNK &&
							(Winsock.Get_Connected() || sender_address == Session.HostAddress)){
#endif	//WINSOCK_IPX

					if (receive_packet.BlockNumber == last_received_block + 1){

						save_file.Write ( receive_packet.RawData, receive_packet.BlockLength );
						total_length += receive_packet.BlockLength;
						last_received_block ++;

						update_time++;
						if (update_time >7){
							progress_meter.Set_Value ( (total_length*100) / file_length );
							display = REDRAW_PROGRESS;
							update_time = 0;;
						}

						if (total_length >= file_length){
							process = false;
							return_code = true;
							progress_meter.Set_Value ( 100 );
							progress_meter.Draw_Me(true);
						}
					}
				}
			}
		}


		if (process){
			input = cancelbtn.Input();

			/*
			---------------------------- Process input ----------------------------
			*/
			switch (input) {

				/*
				** Cancel. Just return to the main menu
				*/
				case (KN_ESC):
				case (BUTTON_CANCEL | KN_BUTTON):
					process = false;
					return_code = false;
					break;
			}
		}


	} while ( process );

	save_file.Close();

	/*
	** Update the internal list of scenarios to include the downloaded one so we know about it
	**  for the next game.
	*/
	Session.Read_Scenario_Descriptions();

	return (return_code);
}






/***********************************************************************************************
 * Send_Remote_File -- Sends a file to game clients                                            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    File name                                                                         *
 *                                                                                             *
 * OUTPUT:   true if file transfer was successfully completed                                  *
 *           game type - 0 for modem/null modem, 1 otherwise                                   *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/22/96 3:09PM ST : Created                                                              *
 *=============================================================================================*/
bool Send_Remote_File ( char *file_name, int gametype )
{
	//WWDebugString ("RA95 - In Send_Remote_File\n");

	/*
	** Dialog & button dimensions
	*/
	int factor			= (SeenBuff.Get_Width() == 320) ? 1 : 2;

	int d_dialog_w = 240 *factor;											// dialog width
	int d_dialog_h = 90*factor;											// dialog height
	int d_dialog_x = ((320*factor - d_dialog_w) / 2);				// dialog x-coord
	int d_dialog_y = ((200*factor - d_dialog_h) / 2);				// centered y-coord
	int d_dialog_cx = d_dialog_x + (d_dialog_w / 2);		// center x-coord

	int d_margin1=10;
	int d_txt6_h=15;

#if (GERMAN | FRENCH)
	int d_cancel_w = 50*factor;
#else
	int d_cancel_w = 40*factor;
#endif
	int d_cancel_h = 9*factor;
	int d_cancel_x = d_dialog_cx - d_cancel_w / 2;
	int d_cancel_y = d_dialog_y + d_dialog_h - 20*factor;

	int d_progress_w = 100*factor;
	int d_progress_h = 10*factor;
	int d_progress_x = (SeenBuff.Get_Width()/2) - d_progress_w/2;
	int d_progress_y = d_dialog_y + 45*factor;

	int	width;
	int	height;

	char *info_string = (char*)Text_String (TXT_SENDING_SCENARIO);

	CDTimerClass<SystemTimerClass> response_timer;		// timeout timer for waiting for responses

	Fancy_Text_Print(TXT_NONE, 0, 0, GadgetClass::Get_Color_Scheme(),
		TBLACK, TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);

	Format_Window_String(info_string, SeenBuff.Get_Height(), width, height);


	/*
	** Button Enumerations
	*/
	enum {
		BUTTON_CANCEL = 100,
		BUTTON_PROGRESS
	};

	/*
	** Buttons
	*/
	//TextButtonClass *buttons;										// button list

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
#if (GERMAN | FRENCH)
		d_cancel_x, d_cancel_y);
#else
		d_cancel_x, d_cancel_y, d_cancel_w, d_cancel_h);
#endif

	GaugeClass progress_meter (BUTTON_PROGRESS,
		d_progress_x, d_progress_y, d_progress_w, d_progress_h);

	Fancy_Text_Print(TXT_NONE, 0, 0, GadgetClass::Get_Color_Scheme(),
		TBLACK, TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);


	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_PROGRESS,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;


	bool 			process = true;
	RedrawType 	display = REDRAW_ALL;		// redraw level
	KeyNumType 	input;
	GadgetClass *commands;					// button list
	bool			return_code;
	int			update_time = 0;


	int file_length;
	int block_number;
	int max_chunk_size;
	int total_blocks;
	int bytes_left;

	void *read_ptr;

	RemoteFileTransferType	send_packet;
	SerialPacketType			file_info;
	GlobalPacketType			net_file_info;


	CCFileClass send_file (file_name);

	if ( !send_file.Is_Available() ){
		//WWDebugString ("RA95 - Error - could not find file to send to client\n");
//		debugprint("RA95 - Error - could not find file to send to client\n");
		return (false);
	}
	file_length = send_file.Size();

	response_timer = RESPONSE_TIMEOUT;

	/*
	** Send the file info to the remote machine(s)
	*/
	if (!gametype){
		file_info.Command = SERIAL_FILE_INFO;
		strcpy (&file_info.ScenarioInfo.ShortFileName[0], file_name);
#ifdef FIXIT_VERSION_3
		//	If we're sending an official map, always send it to 'download.tmp'.
		if( Is_Mission_Counterstrike( file_name ) || Is_Mission_Aftermath( file_name ) ) {
			strcpy (&file_info.ScenarioInfo.ShortFileName[0], "DOWNLOAD.TMP");
		}
#else
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
// If we're sending an Aftermath map, always send it to 'download.tmp'.
		if (Is_Mission_Aftermath(file_name)) {
			strcpy (&file_info.ScenarioInfo.ShortFileName[0], "DOWNLOAD.TMP");
		}
#endif
#endif
		file_info.ScenarioInfo.FileLength = file_length;
		NullModem.Send_Message (&file_info, sizeof (file_info), 1);
		while (NullModem.Num_Send() > 0 && response_timer){
			NullModem.Service();
		}
	} else {
		net_file_info.Command = NET_FILE_INFO;
		strcpy (&net_file_info.ScenarioInfo.ShortFileName[0], file_name);
//		debugprint( "Uploading '%s'\n", file_name );
#ifdef FIXIT_VERSION_3
		//	If we're sending an official map, always send it to 'download.tmp'.
		if( Is_Mission_Counterstrike( file_name ) || Is_Mission_Aftermath( file_name ) ) {
			strcpy (&net_file_info.ScenarioInfo.ShortFileName[0], "DOWNLOAD.TMP");
		}
#else
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
// If we're sending an Aftermath map, always send it to 'download.tmp'.
		if (Is_Mission_Aftermath(file_name)) {
			strcpy (&file_info.ScenarioInfo.ShortFileName[0], "DOWNLOAD.TMP");
			//	There was a bug here: s/b net_file_info. This means that players that don't have Aftermath could have been 
			//	accumulating Aftermath maps all this time!!! (File wasn't getting renamed to "DOWNLOAD.TMP".)
		}
#endif
#endif
//		debugprint( "ShortFileName is '%s'\n", net_file_info.ScenarioInfo.ShortFileName );
		net_file_info.ScenarioInfo.FileLength = file_length;

		for (int i=0 ; i<Session.RequestCount ; i++){
			Ipx.Send_Global_Message (&net_file_info, sizeof (GlobalPacketType),
				1, &(Session.Players[Session.ScenarioRequests[i]]->Address) );
		}

		while (Ipx.Global_Num_Send() > 0 && response_timer) {
			Ipx.Service();
		}
	}


	max_chunk_size = MAX_SEND_FILE_PACKET_SIZE;
	total_blocks = (file_length + max_chunk_size-1) / max_chunk_size;
	bytes_left = file_length;

	send_file.Open ( READ );


	commands = &cancelbtn;
	commands->Add_Tail (progress_meter);

	progress_meter.Set_Maximum(100);		// Max is 100%
	progress_meter.Set_Value(0);			// Current is 0%


	block_number = 0;

	while ( process ){

#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = REDRAW_ALL;
		}
#endif

#ifdef WOLAPI_INTEGRATION
		if( Session.Type == GAME_INTERNET && pWolapi && ( ::timeGetTime() > pWolapi->dwTimeNextWolapiPump ) )
		{
			pWolapi->pChat->PumpMessages();
			pWolapi->dwTimeNextWolapiPump = ::timeGetTime() + WOLAPIPUMPWAIT;
		}
#endif

		if (display){

			if (display >= REDRAW_BACKGROUND){

				Hide_Mouse();
				/*
				** Redraw backgound & dialog box
				*/
				Load_Title_Page(true);
				Set_Palette(CCPalette);

				Dialog_Box(d_dialog_x, d_dialog_y, d_dialog_w, d_dialog_h);

				/*
				** Dialog & Field labels
				*/
				Draw_Caption (TXT_NONE, d_dialog_x, d_dialog_y, d_dialog_w);

				Fancy_Text_Print(info_string, d_dialog_cx-width/2, d_dialog_y + 25*factor,
									GadgetClass::Get_Color_Scheme(), TBLACK,
									TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Show_Mouse();

			}

			if (display >= REDRAW_BUTTONS){

				commands->Draw_All();

			}

			if (display >= REDRAW_PROGRESS){
				progress_meter.Draw_Me(true);
			}

			display = REDRAW_NONE;
		}


		if (!gametype){
			NullModem.Service();


			if (block_number < total_blocks){

				if ( NullModem.Num_Send() <2 ){

					send_packet.Command = SERIAL_FILE_CHUNK;
					send_packet.BlockNumber = block_number;
					send_packet.BlockLength = MIN (file_length, max_chunk_size);

					file_length -= send_packet.BlockLength;

					read_ptr = &send_packet.RawData[0];

					if (send_file.Read (read_ptr , send_packet.BlockLength) == send_packet.BlockLength){
						NullModem.Send_Message ((void*)&send_packet, sizeof(send_packet), 1);
					}

					block_number++;

					update_time++;
					if (update_time >7){
						progress_meter.Set_Value ( (block_number*100) / total_blocks );
						display = REDRAW_PROGRESS;
						update_time = 0;;
					}

				}
			}else{
				if (NullModem.Num_Send() == 0){
					process = false;
					return_code = true;
					progress_meter.Set_Value ( 100 );
					progress_meter.Draw_Me(true);

				}
			}


		}else{
			Ipx.Service();

			if (block_number < total_blocks){

				if ( Ipx.Global_Num_Send() == 0 ){

					send_packet.Command = SERIAL_FILE_CHUNK;
					send_packet.BlockNumber = block_number;
					send_packet.BlockLength = MIN (file_length, max_chunk_size);

					file_length -= send_packet.BlockLength;

					read_ptr = &send_packet.RawData[0];

					if (send_file.Read (read_ptr , send_packet.BlockLength) == send_packet.BlockLength){
						for (int i=0 ; i<Session.RequestCount ; i++){
							Ipx.Send_Global_Message (&send_packet, sizeof (send_packet),
								1, &(Session.Players[Session.ScenarioRequests[i]]->Address) );
						}
					}

					block_number++;

					update_time++;
					if (update_time >7){
						progress_meter.Set_Value ( (block_number*100) / total_blocks );
						display = REDRAW_PROGRESS;
						update_time = 0;;
					}

				}
			}else{
				if (Ipx.Global_Num_Send() == 0){
					process = false;
					return_code = true;
					progress_meter.Set_Value ( 100 );
					progress_meter.Draw_Me(true);
				}
			}



		}


		if (process){
			input = cancelbtn.Input();

			/*
			---------------------------- Process input ----------------------------
			*/
			switch (input) {

				/*
				** Cancel. Just return to the main menu
				*/
				case (KN_ESC):
				case (BUTTON_CANCEL | KN_BUTTON):
					process = false;
					return_code = false;
					break;
			}
		}

	}

	return (return_code);
}
#endif