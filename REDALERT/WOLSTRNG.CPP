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

//	New character strings for wolapi integration.

//#ifdef WOLAPI_INTEGRATION
#include "function.h"
#include "WolStrng.h"

//#undef ENGLISH
//#define GERMAN

#ifdef ENGLISH
#pragma message( "...Building English version..." )

//	Menu choice for Internet game.
const char TXT_WOL_INTERNETBUTTON[]			= "Internet";
//	Generic error message, though implies that blame lies with Westwood Online.
const char TXT_WOL_ERRORMESSAGE[]			= "Unexpected error occurred communicating with Westwood Online.";
//	Connect button on login dialog.
const char TXT_WOL_CONNECT[]				= "Connect";
//	Title for login dialog.
const char TXT_WOL_LOGINDIALOG[]			= "Westwood Online Login";
//	Appears on login dialog - user login name field.
const char TXT_WOL_NAME[]					= "Nickname";
//	Appears on login dialog - user password field.
const char TXT_WOL_PASSWORD[]				= "Password";
//	Appears on login dialog - checkbox specifying whether nickname/password should be saved to disk.
const char TXT_WOL_SAVELOGIN[]				= "Save";
//	User hit the Escape button to cancel the logging in process.
const char TXT_WOL_LOGINCANCEL[]			= "Login cancelled.";

const char TXT_WOL_MISSINGNAME[]			= "Please enter your login nickname.";

const char TXT_WOL_MISSINGPASSWORD[]		= "Please enter your login password.";

const char TXT_WOL_CANTSAVENICK[]			= "Error saving nickname/password.";

const char TXT_WOL_NICKINUSE[]				= "That nickname is in use. Please select another.";

const char TXT_WOL_BADPASS[]				= "Invalid password for this nickname.";

const char TXT_WOL_TIMEOUT[]				= "Connection to Westwood Online timed out.";

const char TXT_WOL_CONNECTING[]				= "Connecting to Westwood Online...";

const char TXT_WOL_CANTCONNECT[]			= "Could not establish connection to Westwood Online.";
//	Appears while connecting and logging in to Westwood Online.
const char TXT_WOL_ATTEMPTLOGIN[]			= "Logging in...";
//	Appears while logging out and disconnecting from Westwood Online.
const char TXT_WOL_ATTEMPTLOGOUT[]			= "Logging out...";
//	Appears while logging out and disconnecting from Westwood Online after an error has occurred.
const char TXT_WOL_ERRORLOGOUT[]			= "Terminating connection with Westwood Online...";
//	Common "please wait" message.
const char TXT_WOL_WAIT[]					= "Please wait... communicating with Westwood Online...";
//	Title for the top WW Online level.
const char TXT_WOL_TOPLEVELTITLE[]			= "Westwood Online";
//	Title for the WW Online level where "official" chat channels are listed.
const char TXT_WOL_OFFICIALCHAT[]			= "Official Chat";
//	Title for the WW Online level where "user" (in other words, unofficial) chat channels are listed.
const char TXT_WOL_USERCHAT[]				= "User Chat";
//	Title for the WW Online level where game channels are listed.
const char TXT_WOL_GAMECHANNELS[]			= "Game Channels";
//	Title for the WW Online level where Red Alert game lobbies are listed.
const char TXT_WOL_REDALERTLOBBIES[]		= "Red Alert Lobbies";
//	Appears briefly while a list of channels is being downloaded.
const char TXT_WOL_CHANNELLISTLOADING[]		= "...downloading...";

const char TXT_WOL_YOURENOTINCHANNEL[]		= "You are not currently in a chat channel.";
//	"Action" button. Causes text entered by user to show up as if they were performing an action, as opposed to speaking.
const char TXT_WOL_ACTION[]					= "Action";
//	"Join" button. Allows user to join a channel, game, or WW Online level.
const char TXT_WOL_JOIN[]					= "Join";

const char TXT_WOL_CANTCREATEINCHANNEL[]	= "You can't create a new channel until you exit this channel.";
//	"New" button. Allows user to create a new chat channel or game.
const char TXT_WOL_NEWSOMETHING[]			= "New";
//	Title for chat channel creation dialog.
const char TXT_WOL_CREATECHANNELTITLE[]		= "Create Channel";

const char TXT_WOL_CREATECHANNELPROMPT[]	= "Channel Name: ";
//	Prompt for fields where the user must enter a password.
const char TXT_WOL_PASSPROMPT[]				= "Password: ";
//	Prompt for fields where the user may enter a password, but it is not required.
const char TXT_WOL_OPTIONALPASSPROMPT[]		= "Password (optional): ";
//	Appears in channel list, as top choice, which the user can use to go back to the top WW Online level.
const char TXT_WOL_CHANNEL_TOP[]			= ".. <back to top>";
//	Appears in channel list, as top choice, which the user can use to go back up one WW Online level.
const char TXT_WOL_CHANNEL_BACK[]			= ".. <back>";
//	%s is replaced by the name of a channel.
const char TXT_WOL_YOUJOINED[]				= "You have joined the %s channel.";
//	%s is replaced by the name of a user.
const char TXT_WOL_YOUJOINEDGAME[]			= "You have joined %s's game.";
//	Message confirming that user created a new game.
const char TXT_WOL_YOUCREATEDGAME[]			= "New game created.";
//	%s is replaced by the name of a lobby.
const char TXT_WOL_YOUJOINEDLOBBY[]			= "You have entered the %s lobby.";
//	%s is replaced by the name of a channel.
const char TXT_WOL_YOULEFT[]				= "You have left the %s channel.";
//	%s is replaced by the name of a lobby.
const char TXT_WOL_YOULEFTLOBBY[]			= "You have left the %s lobby.";
//	Title for dialog that prompts user for the password needed to enter a private channel.
const char TXT_WOL_JOINPRIVATETITLE[]		= "Join Private Channel";

const char TXT_WOL_JOINPRIVATEPROMPT[]		= "Enter Channel Password: ";

const char TXT_WOL_BADCHANKEY[]				= "Incorrect channel password.";
//	Title for the Page/Locate dialog. Page = send a user a message. Locate = find out where a user is.
const char TXT_WOL_PAGELOCATE[]				= "Page/Locate";
//	Appears on Page/Locate dialog.
const char TXT_WOL_USERNAMEPROMPT[]			= "User Name: ";
//	Text for Page button on dialog.
const char TXT_WOL_PAGE[]					= "Page";
//	Text for Locate button on dialog.
const char TXT_WOL_LOCATE[]					= "Locate";
//	%s is replaced with name of user being located.
const char TXT_WOL_LOCATING[]				= "Locating %s...";

const char TXT_WOL_FIND_NOTHERE[]			= "The specified user name does not exist.";

const char TXT_WOL_FIND_NOCHAN[]			= "The specified user is currently not in a channel.";

const char TXT_WOL_FIND_OFF[]				= "The specified user has disabled find capability.";
//	%s is replaced with name of user being located.
const char TXT_WOL_FOUNDIN[]				= "User found in the %s channel.";
//	Title for Page dialog.
const char TXT_WOL_PAGEMESSAGETITLE[]		= "Page User";
//	Prompt for field in which user enters the message that is to be sent to user.
const char TXT_WOL_PAGEMESSAGEPROMPT[]		= "Message to Send: ";
//	%s is replaced with name of user being paged.
const char TXT_WOL_PAGING[]					= "Paging %s...";

const char TXT_WOL_PAGE_NOTHERE[]			= "The specified user is not logged in.";

const char TXT_WOL_PAGE_OFF[]				= "The specified user has disabled page capability.";
//	First %s is replaced with user name, second %s with a text message.
const char TXT_WOL_ONPAGE[]					= "Page from %s: %s";
//	%s is replaced with name of user being paged.
const char TXT_WOL_WASPAGED[]				= "%s was successfully paged.";
//	%s is replaced with the name of a user that has just been squelched. (Currently unused.)
//const char TXT_WOL_USERISSQUELCHED[]		= "%s has been squelched.";
//	%s is replaced with the name of a user that has had squelch removed. (Currently unused.)
//const char TXT_WOL_USERISNOTSQUELCHED[]		= "%s is no longer squelched.";

const char TXT_WOL_ONLYOWNERCANKICK[]		= "Only the channel owner can kick users out.";
//	Both %s replaced with user names.
const char TXT_WOL_USERKICKEDUSER[]			= "%s kicked %s out of the channel.";
//	%s replaced with user name.
const char TXT_WOL_USERKICKEDYOU[]			= "You were kicked out of the channel by %s.";

const char TXT_WOL_NOONETOKICK[]			= "Select the user(s) you wish to kick out.";
//	%s replaced with user name.
const char TXT_WOL_USERWASBANNED[]			= "%s has been banned from the channel.";
//	Title for dialog in which user enters password for new game they are creating.
const char TXT_WOL_CREATEPRIVGAMETITLE[]	= "Create Private Game";

const char TXT_WOL_YOUREBANNED[]			= "You've been banned from entering this channel.";
//	%s replaced with user name.
const char TXT_WOL_PLAYERLEFTGAME[]			= "%s has left the game.";
//	%s replaced with user name.
const char TXT_WOL_PLAYERJOINEDGAME[]		= "%s has joined the game.";

const char TXT_WOL_YOUWEREKICKEDFROMGAME[]	= "You've been kicked out of the game.";
//	Shows user's ladder ranking and win/loss record. Appears above main chat area.
const char TXT_WOL_PERSONALWINLOSSRECORD[]	= "%s. Red Alert: Ranked %u. Won %u. Lost %u. Points %u.";
//	Shows user's ladder ranking and win/loss record. Appears above main chat area. Appended Aftermath ranking.
const char TXT_WOL_PERSONALWINLOSSRECORDAM[]= "%s. Aftermath: Ranked %u. Won %u. Lost %u. Points %u.";
//	Used to show brief user ladder ranking in user lists. Example: FredX (Rank 134)
const char TXT_WOL_USERRANK[]				= "%s (Rank %u)";
//	No need to translate.
const char TXT_WOL_USERHOUSE[]				= "%s <%s>";
//	"Rank" translates the same here as above.
const char TXT_WOL_USERRANKHOUSE[]			= "%s (Rank %u) <%s>";
//	Button host user presses to start a game they have created.
const char TXT_WOL_STARTBUTTON[]			= "Start";
//	Button that guests joining a game press to indicate that they agree to the game rules set up by the host.
const char TXT_WOL_ACCEPTBUTTON[]			= "Accept";
//	%s replaced with user name.
const char TXT_WOL_HOSTLEFTGAME[]			= "%s has cancelled the game.";
//	Appears when game is actually being started.
const char TXT_WOL_WAITINGTOSTART[]			= "Launching game...";
//	Tooltip help for WW Online button: disconnect.
const char TXT_WOL_TTIP_DISCON[]			= " Leave Westwood Online ";
//	Tooltip help for WW Online button: leave current channel.
const char TXT_WOL_TTIP_LEAVE[]				= " Leave the channel you are in ";
//	Tooltip help for WW Online button: refresh current list.
const char TXT_WOL_TTIP_REFRESH[]			= " Refresh current channel list ";
//	Tooltip help for WW Online button: squelch user(s).
const char TXT_WOL_TTIP_SQUELCH[]			= " Enable/disable incoming message from user(s) ";
//	Tooltip help for WW Online button: ban (and kick) user(s).
const char TXT_WOL_TTIP_BAN[]				= " Ban user(s) from channel ";
//	Tooltip help for WW Online button: kick user(s).
const char TXT_WOL_TTIP_KICK[]				= " Kick user(s) out of channel ";
//	Tooltip help for WW Online button: find/page.
const char TXT_WOL_TTIP_FINDPAGE[]			= " Find or page a user ";
//	Tooltip help for WW Online button: show options dialog.
const char TXT_WOL_TTIP_OPTIONS[]			= " Set Westwood Online options ";
//	Tooltip help for WW Online button: browse game ladder.
const char TXT_WOL_TTIP_LADDER[]			= " Browse Red Alert ladders ";
//	Tooltip help for WW Online button: show help.
const char TXT_WOL_TTIP_HELP[]				= " Show Westwood Online help ";
//	Tooltip help. Appears for button host presses to start a game.
const char TXT_WOL_TTIP_START[]				= " Start the game ";
//	Tooltip help. Appears for button guests press in order to agree to (accept) game rules set up by the host.
const char TXT_WOL_TTIP_ACCEPT[]			= " Accept the current game settings ";
//	Tooltip help. Appears for the small buttons that allow users to enlarge or diminish the size of channel/user lists.
const char TXT_WOL_TTIP_EXPANDLIST[]		= " Expand/contract list ";
//	Tooltip for Cancel button during game setup.
const char TXT_WOL_TTIP_CANCELGAME[]		= " Go back a level ";
//	Tooltip for Join button during chat.
const char TXT_WOL_TTIP_JOIN[]				= " Join a chat or game channel ";
//	Tooltip for Back button during chat.
const char TXT_WOL_TTIP_BACK[]				= " Go back a level ";
//	Tooltip for New button during chat.
const char TXT_WOL_TTIP_CREATE[]			= " Create a new chat/game channel ";
//	Tooltip for Action button.
const char TXT_WOL_TTIP_ACTION[]			= " Action message ";

const char TXT_WOL_OPTFIND[]				= "Let others FIND you.";

const char TXT_WOL_OPTPAGE[]				= "Let others PAGE you.";

const char TXT_WOL_OPTLANGUAGE[]			= "Filter out bad language.";
//	"Display just the games that were created by someone in the lobby you are currently in."
const char TXT_WOL_OPTGAMESCOPE[]			= "Show local lobby games only.";

const char TXT_WOL_CHANNELGONE[]			= "Channel no longer exists.";
//	Title for create new game dialog.
const char TXT_WOL_CG_TITLE[]				= "Create Game";
//	%i replaced by number of players allowed into game channel.
const char TXT_WOL_CG_PLAYERS[]				= "Players:  %i";
//	Marks field indicating whether or not this is a tournament game.
const char TXT_WOL_CG_TOURNAMENT[]			= "Tournament";
//	Marks field indicating whether or not this is a private game.
const char TXT_WOL_CG_PRIVACY[]				= "Private";

const char TXT_WOL_CG_RAGAME[]				= "Red Alert game";

const char TXT_WOL_CG_CSGAME[]				= "Counterstrike game";

const char TXT_WOL_CG_AMGAME[]				= "Aftermath game";

const char TXT_WOL_NEEDCOUNTERSTRIKE[]		= "Sorry, you must have Counterstrike installed to play this game.";

const char TXT_WOL_NEEDAFTERMATH[]			= "Sorry, you must have Aftermath installed to play this game.";
//	%s = name of channel, %i = number of people in channel.
const char TXT_WOL_TTIP_CHANLIST_CHAT[]		= " Doubleclick to join the '%s' channel (%i current users). ";
//	%s = name of lobby, %i = number of people in channel.
const char TXT_WOL_TTIP_CHANLIST_LOBBY[]	= " Doubleclick to join the '%s' lobby (%i current users). ";
//	Appears in tooltip help for a channel list item.
const char TXT_WOL_TTIP_REDALERT[]			= "Red Alert";
//	Appears in tooltip help for a channel list item.
const char TXT_WOL_TTIP_COUNTERSTRIKE[]		= "Counterstrike";
//	Appears in tooltip help for a channel list item.
const char TXT_WOL_TTIP_AFTERMATH[]			= "Aftermath";
//	%s = name of user, first %i = number of players in channel, second %i = maximum number of players allowed.
const char TXT_WOL_TTIP_CHANLIST_RAGAME[]	= " %s game (%i players of a maximum %i). ";
//	%s = name of user, %i = number of players in channel.
const char TXT_WOL_TTIP_CHANLIST_GAME[]		= " %s game (%i players). ";
//	Appears in tooltip help for a channel list item.
const char TXT_WOL_TTIP_PRIVATEGAME[]		= "(Private) ";
//	Appears in tooltip help for a channel list item.
const char TXT_WOL_TTIP_TOURNAMENTGAME[]	= "(Tournament) ";
//	%s is a kind of game, for example, "Dune 2000".
const char TXT_WOL_TTIP_CHANNELTYPE_GAMESOFTYPE[]	= " Doubleclick to view %s games. ";

const char TXT_WOL_TOURNAMENTPLAYERLIMIT[]	= "Tournament games must be two player games.";
//	Shows on game setup screen for private games. %s = password for game.
const char TXT_WOL_PRIVATEPASSWORD[]		= "Password: %s";
//	User cannot join game because either he or the game host has hacked the game.
const char TXT_WOL_RULESMISMATCH[]			= "Your game is incompatible with the host's!";
//	Message appears when game host presses start button but slow responses cause an automatic cancellation of game start.
const char TXT_WOL_STARTTIMEOUT[]			= "Timed out waiting for guest responses! Game start cancelled.";
//	Message appears for guests when automatic cancellation occurs.
const char TXT_WOL_STARTCANCELLED[]			= "Game start cancelled.";
//	Text of button on game setup screen that takes user out of the game channel.
const char TXT_WOL_CANCELGAME[]				= "Back";

const char TXT_WOL_PATCHQUESTION[]			= "An update patch is required for Internet play. Do you want to download it now?";
//	Title of patch download dialog. First %i = current file being downloaded, second %i = total # of files to download.
const char TXT_WOL_DOWNLOADING[]			= "Download file %i of %i";

const char TXT_WOL_DOWNLOADERROR[]			= "An error occurred during file download.";
//	Appears on patch download dialog. First %i = current # of bytes downloaded, second %i = total # of bytes to download.
const char TXT_WOL_DOWNLOADBYTES[]			= "Received %i bytes out of %i. (%i%%%%)";
//	Appears on patch download dialog. First %i = number of minutes left, second %i = number of seconds left.
const char TXT_WOL_DOWNLOADTIME[]			= "Time Remaining: %i min. %i secs.";
//	Appended to title of patch download dialog when resuming an interrupted download. %s is the regular title, as above.
const char TXT_WOL_DOWNLOADRESUMED[]		= "%s (Resumed after interruption.)";

const char TXT_WOL_DOWNLOADCONNECTING[]		= "Status: Connecting...";

const char TXT_WOL_DOWNLOADLOCATING[]		= "Status: Locating file...";

const char TXT_WOL_DOWNLOADDOWNLOADING[]	= "Status: Downloading...";

const char TXT_WOL_DOWNLOADEXITWARNING[]	= "Download complete! Red Alert will now restart in order to apply the update patch.";

const char TXT_WOL_HELPSHELL[]				= "Are you sure you want to launch the Internet browser for Westwood Online help?";

const char TXT_WOL_LADDERSHELL[]			= "Are you sure you want to launch the Internet browser for the Red Alert ladders?";

const char TXT_WOL_WEBREGISTRATIONSHELL[]	= "No saved usernames found. Would you like to register a new username for Westwood Online?";

const char TXT_WOL_GAMEADVERTSHELL[]		= "Are you sure you want to launch the Internet browser for information about %s?";
//	Appears above user list. %i = number of users in the current channel.
const char TXT_WOL_USERLIST[]				= "Users   %i";
//	Appears above user list to explain why no users are being listed: because the user is not currently in a chat channel.
const char TXT_WOL_NOUSERLIST[]				= "(not in a channel)";

const char TXT_WOL_CANTCREATEHERE[]			= "To start a game, you have to be in a Red Alert lobby.";
//	Appears inside game, when connection to WW Online is lost.
const char TXT_WOL_WOLAPIGONE[]				= "Connection to Westwood Online has been lost!";
//	Appears after game, when attempting to get back into WW Online.
const char TXT_WOL_WOLAPIREINIT[]			= "Connection to Westwood Online was lost. Reinitializing...";

const char TXT_WOL_NOTPAGED[]				= "Can't respond to page; no one has paged you.";
//	Appears briefly in the space for scenario name, in game setup dialog.
const char TXT_WOL_SCENARIONAMEWAIT[]		= "waiting for scenario...";
//	Text of button on chat screen that takes user out of a chat channel, or up one WW Online level.
const char TXT_WOL_BACK[]					= "Back";

const char TXT_WOL_AMDISCNEEDED[]			= "The Aftermath disk will be required for this game; please insert it now.";

const char TXT_WOL_CONFIRMLOGOUT[]			= "Are you sure you want to leave Westwood Online?";
//	"Propose a stalemate" button.
const char TXT_WOL_PROPOSE_DRAW[]			= "Propose a Draw";
//	Withdraw proposed stalemate button.
const char TXT_WOL_RETRACT_DRAW[]			= "Retract Draw Proposal";
//	Accept offered stalemate button.
const char TXT_WOL_ACCEPT_DRAW[]			= "Accept Proposed Draw";
//	User proposes that the game be declared a stalemate.
const char TXT_WOL_PROPOSE_DRAW_CONFIRM[]	= "Are you sure you want to propose a draw?";
//	User accepts the other's offer that the game be a tie.
const char TXT_WOL_ACCEPT_DRAW_CONFIRM[]	= "Are you sure you want to accept a draw?";

const char TXT_WOL_DRAW_PROPOSED_LOCAL[]	= "You have proposed that the game be declared a draw.";

const char TXT_WOL_DRAW_PROPOSED_OTHER[]	= "%s has proposed that the game be declared a draw.";

const char TXT_WOL_DRAW_RETRACTED_LOCAL[]	= "You have retracted your offer of a draw.";

const char TXT_WOL_DRAW_RETRACTED_OTHER[]	= "%s has retracted the offer of a draw.";
//	Message that appears in place of "Mission Accomplished" or "Mission Failed", when game is a draw.
const char TXT_WOL_DRAW[]					= "The Game is a Draw";
//	Error message that appears when user's web browser can't be automatically launched. %s is a web site URL.
const char TXT_WOL_CANTLAUNCHBROWSER[]		= "Can't launch web browser to open %s!";

const char TXT_WOL_CHANNELFULL[]			= "That chat/game channel is full.";

const char TXT_WOL_CHANNELTYPE_TOP[]			= " Doubleclick to go to the top level. ";

const char TXT_WOL_CHANNELTYPE_OFFICIALCHAT[]	= " Doubleclick to go to the official chat channels level. ";

const char TXT_WOL_CHANNELTYPE_USERCHAT[]		= " Doubleclick to go to the user chat channels level. ";

const char TXT_WOL_CHANNELTYPE_GAMES[]			= " Doubleclick to go to the game channels level. ";

const char TXT_WOL_CHANNELTYPE_LOADING[]		= " Loading list from Westwood Online, please wait... ";

const char TXT_WOL_CHANNELTYPE_LOBBIES[]		= " Doubleclick to go to the lobbies level. ";

const char TXT_WOL_FINDINGLOBBY[]				= "Connected - finding available lobby to enter...";

const char TXT_WOL_PRIVATETOMULTIPLE[]			= "<Private to multiple users>:";

const char TXT_WOL_PRIVATETO[]					= "Private to";

const char TXT_WOL_CS_MISSIONS[]				= "Counterstrike Missions";

const char TXT_WOL_AM_MISSIONS[]				= "Aftermath Missions";

const char TXT_WOL_CANTSQUELCHSELF[]			= "You cannot disable viewing of your own messages!";
//	Title of the WW Online options dialog.
const char TXT_WOL_OPTTITLE[]					= "Westwood Online Options";

const char TXT_WOL_SLOWUNITBUILD[]				= "Slow Unit Build";

const char TXT_WOL_THEGAMEHOST[]				= "The game host";

const char TXT_WOL_TTIP_RANKRA[]				= " Show Red Alert ladder rankings ";

const char TXT_WOL_TTIP_RANKAM[]				= " Show Aftermath ladder rankings ";

const char TXT_WOL_OPTRANKAM[]					= "Show Aftermath rankings (instead of Red Alert)";

const char TXT_WOL_CANCELMEANSFORFEIT[]			= " (AND FORFEIT THE GAME)";

const char TXT_WOL_DLLERROR_GETIE3[]			= "Your version of Windows is out of date. Please upgrade to Windows SP1, or install Internet Explorer 3.0 or higher.";

const char TXT_WOL_DLLERROR_CALLUS[]			= "An unexpected error has occurred. Please contact Westwood technical support.";

const char TXT_WOL_PRIVATE[]					= "<private>";

#else

#ifdef GERMAN
#pragma message( "...Building German version..." )

//	Replaced "·" with ascii 169 (in octal, 251) in cases where 8 point font is used (see 8point.lbm for why).
const char TXT_WOL_INTERNETBUTTON[]			= "Internet";
const char TXT_WOL_ERRORMESSAGE[]			= "Unerwarteter Fehler trat bei der Kommunikation mit Westwood Online auf.";
const char TXT_WOL_CONNECT[]				= "Verbinden";
const char TXT_WOL_LOGINDIALOG[]			= "Westwood-Online-Login";
const char TXT_WOL_NAME[]					= "Spitzname";
const char TXT_WOL_PASSWORD[]				= "Pa\251wort";
const char TXT_WOL_SAVELOGIN[]				= "Speichern";
const char TXT_WOL_LOGINCANCEL[]			= "Login abgebrochen.";
const char TXT_WOL_MISSINGNAME[]			= "Bitte geben Sie Ihren Login-Spitznamen ein.";
const char TXT_WOL_MISSINGPASSWORD[]		= "Bitte geben Sie Ihr Login-Pa\251wort ein.";
const char TXT_WOL_CANTSAVENICK[]			= "Fehler beim Speichern des Spitznamens/Pa\251worts";
const char TXT_WOL_NICKINUSE[]				= "Dieser Spitzname wird bereits verwendet. Bitte wÑhlen Sie einen anderen.";
const char TXT_WOL_BADPASS[]				= "UngÅltiges Pa\251wort fÅr diesen Spitznamen";
const char TXT_WOL_TIMEOUT[]				= "Verbindung zu Westwood Online unterbrochen";
const char TXT_WOL_CONNECTING[]				= "Verbinde zu Westwood Online...";
const char TXT_WOL_CANTCONNECT[]			= "Verbindung zu Westwood Online konnte nicht hergestellt werden.";
const char TXT_WOL_ATTEMPTLOGIN[]			= "Einloggen ... ";
const char TXT_WOL_ATTEMPTLOGOUT[]			= "Ausloggen ...";
const char TXT_WOL_ERRORLOGOUT[]			= "Verbindung zu Westwood Online beenden ...";
const char TXT_WOL_WAIT[]					= "Bitte warten ... Verbindung zu Westwood Online wird hergestellt ...";
const char TXT_WOL_TOPLEVELTITLE[]			= "Westwood Online";
const char TXT_WOL_OFFICIALCHAT[]			= "Offizieller Chat";
const char TXT_WOL_USERCHAT[]				= "User-Chat";
const char TXT_WOL_GAMECHANNELS[]			= "Game-Channels";
const char TXT_WOL_REDALERTLOBBIES[]		= "Alarmstufe-Rot-Lobbies";
const char TXT_WOL_CHANNELLISTLOADING[]		= "... Daten werden heruntergeladen ...";
const char TXT_WOL_YOURENOTINCHANNEL[]		= "Sie befinden sich zur Zeit nicht in einem Chat-Channel.";
const char TXT_WOL_ACTION[]					= "Action";
const char TXT_WOL_JOIN[]					= "Teilnehmen";
const char TXT_WOL_CANTCREATEINCHANNEL[]	= "Sie kînnen keinen neuen Channel erstellen, bevor Sie diesen Channel verlassen.";
const char TXT_WOL_NEWSOMETHING[]			= "Neu";
const char TXT_WOL_CREATECHANNELTITLE[]		= "Channel erstellen";
const char TXT_WOL_CREATECHANNELPROMPT[]	= "Channel-Name: ";
const char TXT_WOL_PASSPROMPT[]				= "Pa\251wort: ";
const char TXT_WOL_OPTIONALPASSPROMPT[]		= "Pa\251wort (optional): ";
const char TXT_WOL_CHANNEL_TOP[]			= ".. <zurÅck zum Anfang>";
const char TXT_WOL_CHANNEL_BACK[]			= ".. <zurÅck>";
const char TXT_WOL_YOUJOINED[]				= "Sie nehmen am %s-Channel teil.";
const char TXT_WOL_YOUJOINEDGAME[]			= "Sie nehmen an %ss Spiel teil.";
const char TXT_WOL_YOUCREATEDGAME[]			= "Neues Spiel erstellt.";
const char TXT_WOL_YOUJOINEDLOBBY[]			= "Sie haben die %s-Lobby betreten.";
const char TXT_WOL_YOULEFT[]				= "Sie haben den %s-Channel verlassen.";
const char TXT_WOL_YOULEFTLOBBY[]			= "Sie haben die %s-Lobby verlassen.";
const char TXT_WOL_JOINPRIVATETITLE[]		= "An privatem Channel teilnehmen";
const char TXT_WOL_JOINPRIVATEPROMPT[]		= "Channel-Pa\251wort eingeben: ";
const char TXT_WOL_BADCHANKEY[]				= "Falsches Channel-Pa\251wort.";
const char TXT_WOL_PAGELOCATE[]				= "Senden/Suchen";
const char TXT_WOL_USERNAMEPROMPT[]			= "User-Name: ";
const char TXT_WOL_PAGE[]					= "Senden";
const char TXT_WOL_LOCATE[]					= "Suchen";
const char TXT_WOL_LOCATING[]				= "Suche %s...";
const char TXT_WOL_FIND_NOTHERE[]			= "Der gesuchte User-Name existiert nicht.";
const char TXT_WOL_FIND_NOCHAN[]			= "Der genannte User befindet sich zur Zeit nicht in einem Channel.";
const char TXT_WOL_FIND_OFF[]				= "Der genannte User hat die Suchfunktion ausgeschaltet.";
const char TXT_WOL_FOUNDIN[]				= "User wurde im %s-Channel gefunden.";
const char TXT_WOL_PAGEMESSAGETITLE[]		= "Sender";
const char TXT_WOL_PAGEMESSAGEPROMPT[]		= "Zu sendende Nachricht: ";
const char TXT_WOL_PAGING[]					= "Sende an %s ...";
const char TXT_WOL_PAGE_NOTHERE[]			= "Der genannte User ist nicht eingeloggt.";
const char TXT_WOL_PAGE_OFF[]				= "Der genannte User hat die Empfangsfunktion ausgeschaltet.";
const char TXT_WOL_ONPAGE[]					= "Sende von %s: %s";
const char TXT_WOL_WASPAGED[]				= "Die Nachricht wurde %s erfolgreich zugestellt.";
//const char TXT_WOL_USERISSQUELCHED[]		= "%s ist zur Zeit nicht erreichbar.";
//const char TXT_WOL_USERISNOTSQUELCHED[]		= "%s ist jetzt wieder erreichbar.";
const char TXT_WOL_ONLYOWNERCANKICK[]		= "Nur der Channel-Besitzer kann andere User hinauswerfen.";
const char TXT_WOL_USERKICKEDUSER[]			= "%s hat %s aus dem Channel geworfen.";
const char TXT_WOL_USERKICKEDYOU[]			= "Sie wurden von %s aus dem Channel geworfen.";
const char TXT_WOL_NOONETOKICK[]			= "WÑhlen Sie den/die User, die Sie hinauswerfen mîchten.";
const char TXT_WOL_USERWASBANNED[]			= "%s hat keinen Zutritt mehr zu diesem Channel.";
const char TXT_WOL_CREATEPRIVGAMETITLE[]	= "Privates Spiel erstellen";
const char TXT_WOL_YOUREBANNED[]			= "Sie haben keinen Zutritt mehr zu diesem Channel.";
const char TXT_WOL_PLAYERLEFTGAME[]			= "%s hat das Spiel verlassen.";
const char TXT_WOL_PLAYERJOINEDGAME[]		= "%s hat an dem Spiel teilgenommen.";
const char TXT_WOL_YOUWEREKICKEDFROMGAME[]	= "Sie wurden aus dem Spiel geworfen.";
const char TXT_WOL_PERSONALWINLOSSRECORD[]	= "%s. Alarmstufe Rot: Pl %u. Siege %u. Niederl %u. Pkte %u.";
const char TXT_WOL_PERSONALWINLOSSRECORDAM[]= "%s. Vergeltungsschlag: Pl %u. Siege %u. Niederl %u. Pkte %u.";
const char TXT_WOL_USERRANK[]				= "%s (Platz %u)";
const char TXT_WOL_USERHOUSE[]				= "%s <%s>";
const char TXT_WOL_USERRANKHOUSE[]			= "%s (Platz %u) <%s>";
const char TXT_WOL_STARTBUTTON[]			= "Start";
const char TXT_WOL_ACCEPTBUTTON[]			= "BestÑtigen";
const char TXT_WOL_HOSTLEFTGAME[]			= "%s hat das Spiel abgebrochen.";
const char TXT_WOL_WAITINGTOSTART[]			= "Spiel wird gestartet ...";
const char TXT_WOL_TTIP_DISCON[]			= " Westwood Online verlassen";
const char TXT_WOL_TTIP_LEAVE[]				= " Derzeitigen Channel verlassen ";
const char TXT_WOL_TTIP_REFRESH[]			= " Channel-Liste aktualisieren ";
const char TXT_WOL_TTIP_SQUELCH[]			= " Nachrichteneingang von User(n) ein/ausschalten";
const char TXT_WOL_TTIP_BAN[]				= " User(n) Zutritt zum Channel verwehren ";
const char TXT_WOL_TTIP_KICK[]				= " User aus dem Channel werfen ";
const char TXT_WOL_TTIP_FINDPAGE[]			= " User suchen oder an User senden ";
const char TXT_WOL_TTIP_OPTIONS[]			= " Westwood-Online-Optionen einstellen ";
const char TXT_WOL_TTIP_LADDER[]			= " Alarmstufe-Rot-Tabelle durchsuchen ";
const char TXT_WOL_TTIP_HELP[]				= " Westwood-Online-Hilfe anzeigen ";
const char TXT_WOL_TTIP_START[]				= " Spiel starten ";
const char TXT_WOL_TTIP_ACCEPT[]			= " Aktuelle Spieleinstellungen bestÑtigen ";
const char TXT_WOL_TTIP_EXPANDLIST[]		= " Listen erweitern/verkleinern ";
const char TXT_WOL_TTIP_CANCELGAME[]		= " Einen Level zurÅck ";
const char TXT_WOL_TTIP_JOIN[]				= " An Chat- oder Game-Channel teilnehmen ";
const char TXT_WOL_TTIP_BACK[]				= " Einen Level zurÅck ";
const char TXT_WOL_TTIP_CREATE[]			= " Neuen Chat- oder Game-Level erstellen ";
const char TXT_WOL_TTIP_ACTION[]			= " Action-Nachricht ";
const char TXT_WOL_OPTFIND[]				= "Lassen Sie zu, da\251 andere Sie FINDEN.";
const char TXT_WOL_OPTPAGE[]				= "Lassen Sie zu, da\251 andere Ihnen Nachrichten SENDEN.";
const char TXT_WOL_OPTLANGUAGE[]			= "Unangemessene Sprache herausfiltern.";
const char TXT_WOL_OPTGAMESCOPE[]			= "Nur lokale Spiel-Lobby anzeigen.";
//const char TXT_WOL_OPTTITLE[]				= "Optionen";
const char TXT_WOL_CHANNELGONE[]			= "Channel existiert nicht mehr.";
const char TXT_WOL_CG_TITLE[]				= "Spiel erstellen";
const char TXT_WOL_CG_PLAYERS[]				= "Spieler:  %i";
const char TXT_WOL_CG_TOURNAMENT[]			= "Turnier";
const char TXT_WOL_CG_PRIVACY[]				= "Privat";
const char TXT_WOL_CG_RAGAME[]				= "Alarmstufe-Rot-Spiel";
const char TXT_WOL_CG_CSGAME[]				= "Gegenangriff-Spiel";
const char TXT_WOL_CG_AMGAME[]				= "Vergeltungsschlag-Spiel";
const char TXT_WOL_NEEDCOUNTERSTRIKE[]		= "Sie mÅssen 'Gegenangriff' installiert haben, um dieses Spiel spielen zu kînnen.";
const char TXT_WOL_NEEDAFTERMATH[]			= "Sie mÅssen 'Vergeltungsschlag' installiert haben, um dieses Spiel spielen zu kînnen.";
const char TXT_WOL_TTIP_CHANLIST_CHAT[]		= " Doppelklick zur Teilnahme am '%s'-Channel (z.Z. %i User). ";
const char TXT_WOL_TTIP_CHANLIST_LOBBY[]	= " Doppelklick zur Teilnahme an der '%s'-Lobby (z.Z. %i User). ";
const char TXT_WOL_TTIP_REDALERT[]			= "Alarmstufe Rot";
const char TXT_WOL_TTIP_COUNTERSTRIKE[]		= "Gegenangriff";
const char TXT_WOL_TTIP_AFTERMATH[]			= "Vergeltungsschlag";
const char TXT_WOL_TTIP_CHANLIST_RAGAME[]	= " %s-Spiel (%i Spieler von maximal %i). ";
const char TXT_WOL_TTIP_CHANLIST_GAME[]		= " %s-Spiel (%i Spieler). ";
const char TXT_WOL_TTIP_PRIVATEGAME[]		= "(Privat) ";
const char TXT_WOL_TTIP_TOURNAMENTGAME[]	= "(Turnier) ";
const char TXT_WOL_TTIP_CHANNELTYPE_GAMESOFTYPE[]	= " Doppelklicken Sie, um %s-Spiele anzusehen. ";
const char TXT_WOL_TOURNAMENTPLAYERLIMIT[]	= "Turnierspiele mÅssen von zwei Spieler gespielt werden.";
const char TXT_WOL_PRIVATEPASSWORD[]		= "Pa\251wort: %s";
const char TXT_WOL_RULESMISMATCH[]			= "Ihr Spiel ist mit dem des Host nicht kompatibel!";
const char TXT_WOL_STARTTIMEOUT[]			= "Keine Antworten von GÑsten! Spielstart abgebrochen.";
const char TXT_WOL_STARTCANCELLED[]			= "Spielstart abgebrochen.";
const char TXT_WOL_CANCELGAME[]				= "ZurÅck";
const char TXT_WOL_PATCHQUESTION[]			= "Ein Update-Patch wird fÅr Internet-Spiele benîtigt. Mîchten Sie es jetzt herunterladen?";
const char TXT_WOL_DOWNLOADING[]			= "Datei %i von %i herunterladen";
const char TXT_WOL_DOWNLOADERROR[]			= "Ein Fehler trat beim Herunterladen der Dateien auf.";
const char TXT_WOL_DOWNLOADBYTES[]			= "%i Bytes von %i erhalten. (%i%%%%)";
const char TXT_WOL_DOWNLOADTIME[]			= "Verbleibende Zeit: %i Min. %i Sek.";
const char TXT_WOL_DOWNLOADRESUMED[]		= "%s (Nach Unterbrechung wiederaufgenommen.)";
const char TXT_WOL_DOWNLOADCONNECTING[]		= "Status: Verbinde ...";
const char TXT_WOL_DOWNLOADLOCATING[]		= "Status: Suche Datei ...";
const char TXT_WOL_DOWNLOADDOWNLOADING[]	= "Status: Lade herunter ...";
const char TXT_WOL_DOWNLOADEXITWARNING[]	= "Herunterladen abgeschlossen! Alarmstufe Rot wird jetzt neugestartet, damit das Update-Patch angewendet werden kann.";
const char TXT_WOL_HELPSHELL[]				= "Sind Sie sicher, da\251 Sie den Internet-Browser fÅr die Westwood-Online-Hilfe starten mîchten?";
const char TXT_WOL_LADDERSHELL[]			= "Sind Sie sicher, da\251 Sie den Internet-Browser fÅr die Alarmstufe-Rot-Tabellen starten mîchten?";
const char TXT_WOL_WEBREGISTRATIONSHELL[]	= "Keine gespeicherten User-Namen gefunden. Mîchten Sie einen neuen User-Namen fÅr Westwood Online eintragen?";
const char TXT_WOL_GAMEADVERTSHELL[]		= "Sind Sie sicher, da\251 Sie den Internet-Browser fÅr Informationen Åber %s starten mîchten?";
const char TXT_WOL_USERLIST[]				= "User: %i";
const char TXT_WOL_NOUSERLIST[]				= "(nicht in einem Channel)";
const char TXT_WOL_CANTCREATEHERE[]			= "Um ein Spiel zu starten, mÅssen Sie in der Lobby Alarmstufe Rot sein.";
const char TXT_WOL_WOLAPIGONE[]				= "Verbindung zu Westwood Online verloren!";
const char TXT_WOL_WOLAPIREINIT[]			= "Verbindung zu Westwood Online verloren. Verbinde erneut ...";
const char TXT_WOL_NOTPAGED[]				= "Kann keine Antwort senden, niemand hat Ihnen geschrieben.";
const char TXT_WOL_SCENARIONAMEWAIT[]		= "Warte auf Szenario ...";
const char TXT_WOL_BACK[]					= "ZurÅck";
const char TXT_WOL_AMDISCNEEDED[]			= "Die CD 'Vergeltungsschlag' wird fÅr dieses Spiel benîtigt, bitte legen Sie sie jetzt ein.";
const char TXT_WOL_CONFIRMLOGOUT[]			= "Sind Sie sicher, da\251 Sie Westwood Online verlassen mîchten?";
const char TXT_WOL_PROPOSE_DRAW[]			= "Unentschieden vorschlagen";
const char TXT_WOL_RETRACT_DRAW[]			= "Unentschieden-Vorschlag zurÅckziehen";
const char TXT_WOL_ACCEPT_DRAW[]			= "Unentschieden-Vorschlag akzeptieren";
const char TXT_WOL_PROPOSE_DRAW_CONFIRM[]	= "Sind Sie sicher, da\251 Sie ein Unentschieden vorschlagen mîchten?";
const char TXT_WOL_ACCEPT_DRAW_CONFIRM[]	= "Sind Sie sicher, da\251 Sie ein Unentschieden akzeptieren mîchten?";
const char TXT_WOL_DRAW_PROPOSED_LOCAL[]	= "Sie haben vorgeschlagen, da\251 das Spiel fÅr unentschieden erklÑrt wird.";
const char TXT_WOL_DRAW_PROPOSED_OTHER[]	= "%s hat vorgeschlagen, da\251 das Spiel fÅr unentschieden erklÑrt wird.";
const char TXT_WOL_DRAW_RETRACTED_LOCAL[]	= "Sie haben Ihr Unentschieden-Angebot zurÅckgezogen.";
const char TXT_WOL_DRAW_RETRACTED_OTHER[]	= "%s hat das Unentschieden-Angebot zurÅckgezogen.";
const char TXT_WOL_DRAW[]					= "Das Spiel ist unentschieden";
const char TXT_WOL_CANTLAUNCHBROWSER[]		= "Web-Browser kann %s nicht îffnen!";
const char TXT_WOL_CHANNELFULL[]			= "Dieser Chat-/Game-Channel ist voll.";
const char TXT_WOL_CHANNELTYPE_TOP[]			= " Doppelklicken Sie, um zum obersten Level zu gelangen. ";
const char TXT_WOL_CHANNELTYPE_OFFICIALCHAT[]	= " Doppelklicken Sie, um zum offiziellen Chat-Channel-Level zu gelangen. ";
const char TXT_WOL_CHANNELTYPE_USERCHAT[]		= " Doppelklicken Sie, um zum User-Chat-Channel-Level zu gelangen. ";
const char TXT_WOL_CHANNELTYPE_GAMES[]			= " Doppelklicken Sie, um zum Game-Channel-Level zu gelangen. ";
const char TXT_WOL_CHANNELTYPE_LOADING[]		= " Liste von Westwood Online wird geladen, bitte warten... ";
const char TXT_WOL_CHANNELTYPE_LOBBIES[]		= " Doppelklicken Sie, um zum Lobby-Level zu gelangen. ";
const char TXT_WOL_FINDINGLOBBY[]				= "Verbunden - suche verfÅgbare Lobby...";
const char TXT_WOL_PRIVATETOMULTIPLE[]			= "<Privat an mehrere User>:";
const char TXT_WOL_PRIVATETO[]					= "Privat an";
const char TXT_WOL_CS_MISSIONS[]				= "Gegenangriff-Missionen";
const char TXT_WOL_AM_MISSIONS[]				= "Vergeltungsschlag-Missionen";
const char TXT_WOL_CANTSQUELCHSELF[]			= "Sie kînnen die Option zum Lesen Ihrer eigenen Nachrichten nicht ausschalten!";
const char TXT_WOL_OPTTITLE[]					= "Westwood Online-Optionen";
const char TXT_WOL_SLOWUNITBUILD[]				= "Einheitenbau verlangsamen";
const char TXT_WOL_THEGAMEHOST[]				= "Der Spiel-Host";
const char TXT_WOL_TTIP_RANKRA[]				= " Alarmstufe-Rot-Platz anzeigen ";
const char TXT_WOL_TTIP_RANKAM[]				= " Vergeltungsschlag-Platz anzeigen ";
const char TXT_WOL_OPTRANKAM[]					= "Vergeltungsschlag-Platz anzeigen.";
const char TXT_WOL_CANCELMEANSFORFEIT[]			= " (UND BÅSSEN SIE DAS SPIEL EIN)";
const char TXT_WOL_DLLERROR_GETIE3[]			= "Ihre Version der Windows ist veraltet. Bauen Sie bitte zu den Windows SP1, aus oder installieren Sie Internet Explorer 3,0 oder hîheres.";
const char TXT_WOL_DLLERROR_CALLUS[]			= "Ein unerwarteter Fehler ist aufgetreten. Bitte wenden Sie sich an den Technischen Kundendienst.";
const char TXT_WOL_PRIVATE[]					= "<privat>";

#else	//	FRENCH

#pragma message( "...Building French version..." )

const char TXT_WOL_INTERNETBUTTON[]			= "Internet";
const char TXT_WOL_ERRORMESSAGE[]			= "Erreur inattendue lors de la connexion Ö Westwood Online.";
const char TXT_WOL_CONNECT[]				= "Se connecter";
const char TXT_WOL_LOGINDIALOG[]			= "Identifiant Ö Westwood Online";
const char TXT_WOL_NAME[]					= "Pseudo";
const char TXT_WOL_PASSWORD[]				= "Mot de passe";
const char TXT_WOL_SAVELOGIN[]				= "Sauvegarder";
const char TXT_WOL_LOGINCANCEL[]			= "Ouverture de session annulÇe.";
const char TXT_WOL_MISSINGNAME[]			= "Veuillez entrer l'identifiant pour votre pseudo.";
const char TXT_WOL_MISSINGPASSWORD[]		= "Veuillez entrer l'identifiant pour votre mot de passe.";
const char TXT_WOL_CANTSAVENICK[]			= "Erreur lors de la sauvegarde du pseudo/mot de passe.";
const char TXT_WOL_NICKINUSE[]				= "Ce pseudo est dÇjÖ utilisÇ. SÇlectionnez-en un autre.";
const char TXT_WOL_BADPASS[]				= "Mot de passe invalide pour ce pseudo.";
const char TXT_WOL_TIMEOUT[]				= "Expiration du temps de connexion Ö Westwood Online.";
const char TXT_WOL_CONNECTING[]				= "Connexion Ö Westwood Online...";
const char TXT_WOL_CANTCONNECT[]			= "Impossible d'Çtablir la connexion Ö Westwood Online.";
const char TXT_WOL_ATTEMPTLOGIN[]			= "Ouverture de la session en cours...";
const char TXT_WOL_ATTEMPTLOGOUT[]			= "Fermeture de la session en cours...";
const char TXT_WOL_ERRORLOGOUT[]			= "Fin de connexion avec Westwood Online...";
const char TXT_WOL_WAIT[]					= "Attendez svp, en communication Ö Westwood Online...";
const char TXT_WOL_TOPLEVELTITLE[]			= "Westwood Online";
const char TXT_WOL_OFFICIALCHAT[]			= "Conversation officielle";
const char TXT_WOL_USERCHAT[]				= "Conversation utilisateur";
const char TXT_WOL_GAMECHANNELS[]			= "Canaux de jeu";
const char TXT_WOL_REDALERTLOBBIES[]		= "Salons d'Alerte Rouge";
const char TXT_WOL_CHANNELLISTLOADING[]		= "...En cours de tÇlÇchargement...";
const char TXT_WOL_YOURENOTINCHANNEL[]		= "Vous n'àtes pas dans un canal de conversation.";
const char TXT_WOL_ACTION[]					= "Action";
const char TXT_WOL_JOIN[]					= "Rejoindre";
const char TXT_WOL_CANTCREATEINCHANNEL[]	= "CrÇation d'un nouveau canal impossible tant que vous ne quittez pas ce canal.";
const char TXT_WOL_NEWSOMETHING[]			= "Nouveau";
const char TXT_WOL_CREATECHANNELTITLE[]		= "CrÇer un canal";
const char TXT_WOL_CREATECHANNELPROMPT[]	= "Nom du canal : ";
const char TXT_WOL_PASSPROMPT[]				= "Mot de passe : ";
const char TXT_WOL_OPTIONALPASSPROMPT[]		= "Mot de passe (en option): ";
const char TXT_WOL_CHANNEL_TOP[]			= ".. <retour Ö la page d'accueil>";
const char TXT_WOL_CHANNEL_BACK[]			= ".. <retour>";
const char TXT_WOL_YOUJOINED[]				= "Vous avez rejoint le canal %s.";
const char TXT_WOL_YOUJOINEDGAME[]			= "Vous rejoignez la partie de %s.";
const char TXT_WOL_YOUCREATEDGAME[]			= "CrÇation d'une nouvelle partie.";
const char TXT_WOL_YOUJOINEDLOBBY[]			= "Vous àtes entrÇ dans le salon %s.";
const char TXT_WOL_YOULEFT[]				= "Vous avez quittÇ le canal %s.";
const char TXT_WOL_YOULEFTLOBBY[]			= "Vous avez quittÇ le salon %s.";
const char TXT_WOL_JOINPRIVATETITLE[]		= "Rejoindre un canal privÇ";
const char TXT_WOL_JOINPRIVATEPROMPT[]		= "Entrer le mot de passe du canal : ";
const char TXT_WOL_BADCHANKEY[]				= "Mot de passe du canal incorrect.";
const char TXT_WOL_PAGELOCATE[]				= "Envoyer/Rechercher";
const char TXT_WOL_USERNAMEPROMPT[]			= "Nom de l'utilisateur : ";
const char TXT_WOL_PAGE[]					= "Envoyer";
const char TXT_WOL_LOCATE[]					= "Rechercher";
const char TXT_WOL_LOCATING[]				= "Recherche de %s en cours ...";
const char TXT_WOL_FIND_NOTHERE[]			= "Le nom de l'utilisateur spÇcifiÇ n'existe pas.";
const char TXT_WOL_FIND_NOCHAN[]			= "L'utilisateur spÇcifiÇ n'est pas sur le canal pour le moment.";
const char TXT_WOL_FIND_OFF[]				= "L'utilisateur spÇcifiÇ a dÇsactivÇ la fonction de recherche.";
const char TXT_WOL_FOUNDIN[]				= "Utilisateur trouvÇ dans le canal %s.";
const char TXT_WOL_PAGEMESSAGETITLE[]		= "Envoyer Ö l'utilisateur";
const char TXT_WOL_PAGEMESSAGEPROMPT[]		= "Message Ö envoyer : ";
const char TXT_WOL_PAGING[]					= "Envoi Ö %s en cours...";
const char TXT_WOL_PAGE_NOTHERE[]			= "L'utilisateur spÇcifiÇ n'a pas ouvert la session.";
const char TXT_WOL_PAGE_OFF[]				= "L'utilisateur spÇcifiÇ a dÇsactivÇ la fonction d'envoi de messages.";
const char TXT_WOL_ONPAGE[]					= "Envoi de %s : %s";
const char TXT_WOL_WASPAGED[]				= "Envoi Ö %s rÇussi.";
//const char TXT_WOL_USERISSQUELCHED[]		= "%s a ÇtÇ rejetÇ.";				//	ajw rejete really means squelched?
//const char TXT_WOL_USERISNOTSQUELCHED[]		= "%s n'est plus rejetÇ.";
const char TXT_WOL_ONLYOWNERCANKICK[]		= "Seul le responsable du canal peut expulser des utilisateurs.";
const char TXT_WOL_USERKICKEDUSER[]			= "%s expulse %s du canal.";
const char TXT_WOL_USERKICKEDYOU[]			= "Vous àtes expulsÇ du canal par %s.";
const char TXT_WOL_NOONETOKICK[]			= "SÇlectionnez l'(les) utilisateur(s) que vous voulez expulser.";
const char TXT_WOL_USERWASBANNED[]			= "%s est exclu du canal.";
const char TXT_WOL_CREATEPRIVGAMETITLE[]	= "CrÇer une partie privÇe";
const char TXT_WOL_YOUREBANNED[]			= "Vous n'àtes pas autorisÇ Ö entrer dans ce canal.";
const char TXT_WOL_PLAYERLEFTGAME[]			= "%s a quittÇ la partie.";
const char TXT_WOL_PLAYERJOINEDGAME[]		= "%s a rejoint la partie.";
const char TXT_WOL_YOUWEREKICKEDFROMGAME[]	= "Vous avez ÇtÇ expulsÇ de la partie.";
const char TXT_WOL_PERSONALWINLOSSRECORD[]	= "%s. Alerte Rouge: position %u. Vict. %u. DÇf. %u. Pts. %u.";
const char TXT_WOL_PERSONALWINLOSSRECORDAM[]= "%s. Missions M.A.D.: position %u. Vict. %u. DÇf. %u. Pts. %u.";
const char TXT_WOL_USERRANK[]				= "%s (Position %u)";
const char TXT_WOL_USERHOUSE[]				= "%s <%s>";
const char TXT_WOL_USERRANKHOUSE[]			= "%s (Position %u) <%s>";
const char TXT_WOL_STARTBUTTON[]			= "DÇmarrer";
const char TXT_WOL_ACCEPTBUTTON[]			= "Accepter";
const char TXT_WOL_HOSTLEFTGAME[]			= "%s a annulÇ la partie.";
const char TXT_WOL_WAITINGTOSTART[]			= "Lancement de la partie...";
const char TXT_WOL_TTIP_DISCON[]			= " Quitter Westwood Online ";
const char TXT_WOL_TTIP_LEAVE[]				= " Quitter le canal oó vous vous trouvez ";
const char TXT_WOL_TTIP_REFRESH[]			= " Rafraåchir la liste du canal ";
const char TXT_WOL_TTIP_SQUELCH[]			= " Activer/dÇsactiver les messages en provenance de(s) l'utilisateur(s) ";
const char TXT_WOL_TTIP_BAN[]				= " Exclure l'/les utilisateur(s)du canal ";
const char TXT_WOL_TTIP_KICK[]				= " Expulser l'/les utilisateurs du canal ";
const char TXT_WOL_TTIP_FINDPAGE[]			= " Rechercher ou envoyer un message Ö un utilisateur ";
const char TXT_WOL_TTIP_OPTIONS[]			= " RÇgler les options de Westwood Online ";
const char TXT_WOL_TTIP_LADDER[]			= " Parcourir les hiÇrarchies d'Alerte Rouge ";
const char TXT_WOL_TTIP_HELP[]				= " Afficher l'aide de Westwood Online ";
const char TXT_WOL_TTIP_START[]				= " DÇmarrer le jeu ";
const char TXT_WOL_TTIP_ACCEPT[]			= " Valider les paramätres actuels du jeu ";
const char TXT_WOL_TTIP_EXPANDLIST[]		= " ComplÇter/rÇduire la liste ";
const char TXT_WOL_TTIP_CANCELGAME[]		= " Retour au niveau prÇcÇdent ";
const char TXT_WOL_TTIP_JOIN[]				= " Rejoindre un canal de conversation/jeu ";
const char TXT_WOL_TTIP_BACK[]				= " Retour au niveau prÇcÇdent ";
const char TXT_WOL_TTIP_CREATE[]			= " CrÇer un nouveau canal de conversation/jeu ";
const char TXT_WOL_TTIP_ACTION[]			= " Message d'action ";
const char TXT_WOL_OPTFIND[]				= "Laisser les autres vous RECHERCHER.";
const char TXT_WOL_OPTPAGE[]				= "Laisser les autres vous ENVOYER des messages.";
const char TXT_WOL_OPTLANGUAGE[]			= "Filtrer les vulgaritÇs.";
const char TXT_WOL_OPTGAMESCOPE[]			= "Afficher seulement les parties en salons locaux.";
//const char TXT_WOL_OPTTITLE[]				= "Options";
const char TXT_WOL_CHANNELGONE[]			= "Ce canal n'existe plus.";
const char TXT_WOL_CG_TITLE[]				= "CrÇer une partie";
const char TXT_WOL_CG_PLAYERS[]				= "Joueurs :  %i";
const char TXT_WOL_CG_TOURNAMENT[]			= "Tournoi";
const char TXT_WOL_CG_PRIVACY[]				= "PrivÇe";
const char TXT_WOL_CG_RAGAME[]				= "Partie Alerte Rouge";
const char TXT_WOL_CG_CSGAME[]				= "Partie Missions Taãga";
const char TXT_WOL_CG_AMGAME[]				= "Partie Missions M.A.D.";
const char TXT_WOL_NEEDCOUNTERSTRIKE[]		= "DÇsolÇ, vous devez installer Missions Taãga pour jouer cette partie.";
const char TXT_WOL_NEEDAFTERMATH[]			= "DÇsolÇ, vous devez installer Missions M.A.D. pour jouer cette partie.";
const char TXT_WOL_TTIP_CHANLIST_CHAT[]		= " Double-clic pour rejoindre canal %s (%i utilisateurs). ";
const char TXT_WOL_TTIP_CHANLIST_LOBBY[]	= " Double-clic pour rejoindre salon %s (%i utilisateurs). ";
const char TXT_WOL_TTIP_REDALERT[]			= "Alerte Rouge";
const char TXT_WOL_TTIP_COUNTERSTRIKE[]		= "Missions Taãga";
const char TXT_WOL_TTIP_AFTERMATH[]			= "Missions M.A.D.";
const char TXT_WOL_TTIP_CHANLIST_RAGAME[]	= " Partie de %s (%i joueurs pour un max. de %i). ";
const char TXT_WOL_TTIP_CHANLIST_GAME[]		= " Partie de %s (%i joueurs). ";
const char TXT_WOL_TTIP_PRIVATEGAME[]		= "(PrivÇe) ";
const char TXT_WOL_TTIP_TOURNAMENTGAME[]	= "(Tournoi) ";
const char TXT_WOL_TTIP_CHANNELTYPE_GAMESOFTYPE[]	= " Double-clic pour afficher les parties %s. ";
const char TXT_WOL_TOURNAMENTPLAYERLIMIT[]	= "Les parties en tournoi doivent rassembler deux joueurs.";
const char TXT_WOL_PRIVATEPASSWORD[]		= "Mot de passe : %s";
const char TXT_WOL_RULESMISMATCH[]			= "Votre partie n'est pas compatible avec celle du serveur !";
const char TXT_WOL_STARTTIMEOUT[]			= "Expiration du temps de rÇponse des clients ! DÇmarrage du jeu annulÇ.";
const char TXT_WOL_STARTCANCELLED[]			= "DÇmarrage du jeu annulÇ.";
const char TXT_WOL_CANCELGAME[]				= "Retour";
const char TXT_WOL_PATCHQUESTION[]			= "Un patch mis Ö jour est nÇcessaire pour le jeu sur Internet. Voulez-vous le tÇlÇcharger maintenant ?";
const char TXT_WOL_DOWNLOADING[]			= "TÇlÇcharger %i fichier(s) sur %i.";
const char TXT_WOL_DOWNLOADERROR[]			= "Erreur lors du tÇlÇchargement du fichier.";
const char TXT_WOL_DOWNLOADBYTES[]			= "RÇception de %i octets sur %i. (%i%%%%).";
const char TXT_WOL_DOWNLOADTIME[]			= "Temps restant : %i min. %i secs.";
const char TXT_WOL_DOWNLOADRESUMED[]		= "%s (reprise apräs interruption.)";
const char TXT_WOL_DOWNLOADCONNECTING[]		= "Etat : en cours de connexion...";
const char TXT_WOL_DOWNLOADLOCATING[]		= "Etat : recherche du fichier...";
const char TXT_WOL_DOWNLOADDOWNLOADING[]	= "Etat : en cours de tÇlÇchargement...";
const char TXT_WOL_DOWNLOADEXITWARNING[]	= "TÇlÇchargement terminÇ ! Alerte Rouge est relancÇ pour que le nouveau patch soit pris en compte.";
const char TXT_WOL_HELPSHELL[]				= "Voulez-vous vraiment lancer le navigateur Internet pour obtenir l'aide Westwood Online ?";
const char TXT_WOL_LADDERSHELL[]			= "Voulez-vous vraiment lancer le navigateur Internet pour les hiÇrarchies d'Alerte Rouge ?";
const char TXT_WOL_WEBREGISTRATIONSHELL[]	= "Aucun nom d'utilisateur sauvegardÇ. Voulez-vous enregistrer un nouveau nom d'utilisateur pour Westwood Online ?";
const char TXT_WOL_GAMEADVERTSHELL[]		= "Voulez-vous vraiment lancer le navigateur Internet pour obtenir des informations sur %s ?";
const char TXT_WOL_USERLIST[]				= "Utilisateurs %i";
const char TXT_WOL_NOUSERLIST[]				= "(absent du canal)";
const char TXT_WOL_CANTCREATEHERE[]			= "Pour commencer une partie, vous devez àtre dans un salon d'Alerte Rouge.";
const char TXT_WOL_WOLAPIGONE[]				= "Perte de connexion avec Westwood Online !";
const char TXT_WOL_WOLAPIREINIT[]			= "Perte de connexion avec Westwood Online. RÇinitialisation en cours...";
const char TXT_WOL_NOTPAGED[]				= "Impossible de rÇpondre au message ; personne ne vous en a envoyÇ.";
const char TXT_WOL_SCENARIONAMEWAIT[]		= "En attente du scÇnario...";
const char TXT_WOL_BACK[]					= "Retour";
const char TXT_WOL_AMDISCNEEDED[]			= "Le CD de Missions M.A.D. est nÇcessaire pour cette partie ; insÇrez-le maintenant.";
const char TXT_WOL_CONFIRMLOGOUT[]			= "Voulez-vous vraiment quitter Westwood Online ?";
const char TXT_WOL_PROPOSE_DRAW[]			= "Proposer une fin avec ÇgalitÇ";
const char TXT_WOL_RETRACT_DRAW[]			= "Annuler la proposition de fin avec ÇgalitÇ";
const char TXT_WOL_ACCEPT_DRAW[]			= "Accepter la proposition de fin avec ÇgalitÇ";
const char TXT_WOL_PROPOSE_DRAW_CONFIRM[]	= "Voulez-vous vraiment proposer une fin avec ÇgalitÇ ?";
const char TXT_WOL_ACCEPT_DRAW_CONFIRM[]	= "Voulez-vous vraiment accepter une fin avec ÇgalitÇ ?" ;
const char TXT_WOL_DRAW_PROPOSED_LOCAL[]	= "Vous proposez de terminer la partie sans vainqueur ni perdant.";
const char TXT_WOL_DRAW_PROPOSED_OTHER[]	= "%s a proposÇ de terminer la partie sans vainqueur ni perdant.";
const char TXT_WOL_DRAW_RETRACTED_LOCAL[]	= "Vous avez annulÇ votre proposition de terminer la partie sans vainqueur ni perdant.";
const char TXT_WOL_DRAW_RETRACTED_OTHER[]	= "%s a annulÇ sa proposition de terminer la partie sans vainqueur ni perdant.";
const char TXT_WOL_DRAW[]					= "Match nul";
const char TXT_WOL_CANTLAUNCHBROWSER[]		= "Impossible de lancer le navigateur web pour ouvrir %s !";
const char TXT_WOL_CHANNELFULL[]			= "Ce canal de jeu/conversation est saturÇ.";
const char TXT_WOL_CHANNELTYPE_TOP[]			= " Double-clic pour retourner au premier niveau. ";
const char TXT_WOL_CHANNELTYPE_OFFICIALCHAT[]	= " Double-clic pour les canaux de conversation officiels. ";
const char TXT_WOL_CHANNELTYPE_USERCHAT[]		= " Double-clic pour les canaux d' utilisateur. ";
const char TXT_WOL_CHANNELTYPE_GAMES[]			= " Double-clic pour accÇder au niveau des canaux de jeu. ";
const char TXT_WOL_CHANNELTYPE_LOADING[]		= " Chargement de la liste depuis Westwood Online, veuillez patienter...";
const char TXT_WOL_CHANNELTYPE_LOBBIES[]		= " Double-clic pour accÇder au niveau des salons. ";
const char TXT_WOL_FINDINGLOBBY[]				= "Connection : recherche de salons disponibles...";
const char TXT_WOL_PRIVATETOMULTIPLE[]			= "<Message personnel adressÇ Ö divers utilisateurs> :";
const char TXT_WOL_PRIVATETO[]					= "Message personnel Ö";
const char TXT_WOL_CS_MISSIONS[]				= "Missions extraites de Missions Taãga";
const char TXT_WOL_AM_MISSIONS[]				= "Missions extraites de Missions M.A.D.";
const char TXT_WOL_CANTSQUELCHSELF[]			= "Vous ne pouvez pas dÇsactiver vos propres messages!";
const char TXT_WOL_OPTTITLE[]					= "Options de Westwood Online";
const char TXT_WOL_SLOWUNITBUILD[]				= "Ralentir la Construction";
const char TXT_WOL_THEGAMEHOST[]				= "Le serveur";
const char TXT_WOL_TTIP_RANKRA[]				= " Afficher les positions d'Alerte Rouge ";
const char TXT_WOL_TTIP_RANKAM[]				= " Afficher les positions de Missions M.A.D. ";
const char TXT_WOL_OPTRANKAM[]					= "Afficher les positions de Missions M.A.D.";
const char TXT_WOL_CANCELMEANSFORFEIT[]			= " (ET RENONCER AU JEU)";
const char TXT_WOL_DLLERROR_GETIE3[]			= "Votre version des Windows est dÇmodÇe. AmÇliorez s'il vous plait aux Windows SP1, ou installez l'Internet Explorer 3,0 ou plus haut.";
const char TXT_WOL_DLLERROR_CALLUS[]			= "Une erreur inattendue s'est produite. Veuillez contacter l'assistance technique de Electronic Arts.";
const char TXT_WOL_PRIVATE[]					= "<personnel>";

#endif

#endif


//#endif
