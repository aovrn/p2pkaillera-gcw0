#include "kailleraclient.h"
#include "kaillera_ui.h"

#include "common/linux.h"

#include "common/nSTL.h"
#include "common/k_socket.h"
#include "common/nThread.h"
#include "common/nSettings.h"

bool KAILLERA_CORE_INITIALIZED = false;

bool kaillera_SelectServerDlgStep() {
	if (KAILLERA_CORE_INITIALIZED) {
		kaillera_step();
		return true;
	}
	Sleep(100);
	return true;
}

char * CONNECTION_TYPES [] = 
{
	"",
	"LAN",
	"Exc",
	"Good",
	"Avg",
	"Low",
	"Bad"
};

char * USER_STATUS [] =
{
	"Playing",
	"Idle",
	"Playing"
};

char * GAME_STATUS [] = 
{
	"Waiting",
	"Playing",
	"Playing",
};

#define CHAT_NONE	0
#define CHAT_SERVER	1
#define CHAT_GAME	2

//===========================================================================
//=======================================================================

bool kaillera_RecordingEnabled() {
    return false;
}

void sdl_outp(char * line, int chat_type, unsigned int color) {
	char buf[1024];
	snprintf(buf, 1024, chat_type == CHAT_NONE ?  "%s" : (chat_type == CHAT_SERVER ? "SCHAT: %s" : "GCHAT: %s"), line);
	kprintf(buf);
}

// Server chat
void kaillera_outp(char * line){
	sdl_outp(line, CHAT_SERVER, 0);
}

// Game chat
void kaillera_goutp(char * line){
	sdl_outp(line, CHAT_GAME, 0);
}

void __cdecl kaillera_gdebug(char * arg_0, ...) {
	char V8[1024];
	char V88[2084];
	sprintf_s(V8, 1021, "%s\r\n", arg_0);
	va_list args;
	va_start (args, arg_0);
	vsnprintf_s(V88, 2084, 2082, V8, args);
	va_end (args);
	kaillera_goutp(V88);
}

void __cdecl kaillera_core_debug(char * arg_0, ...) {
	char V8[1024];
	char V88[2084];
	sprintf_s(V8, 1021, "%s\r\n", arg_0);
	va_list args;
	va_start (args, arg_0);
	vsnprintf_s(V88, 2084, 2082, V8, args);
	va_end (args);

	sdl_outp(V88, CHAT_SERVER, 0x333333);
}
void __cdecl kaillera_ui_motd(char * arg_0, ...) {
	char V8[1024];
	char V88[2084];
	sprintf_s(V8, 1021, "%s\r\n", arg_0);
	va_list args;
	va_start (args, arg_0);
	vsnprintf_s(V88, sizeof(V88), 2083, V8, args);
	va_end (args);

	sdl_outp(V88, CHAT_SERVER, 0x00336633);
}
void __cdecl kaillera_error_callback(char * arg_0, ...) {
	char V8[1024];
	char V88[2084];
	sprintf_s(V8, 1021, "%s\r\n", arg_0);
	va_list args;
	va_start (args, arg_0);
	vsnprintf_s(V88, 2084, 2082, V8, args);
	va_end (args);

	sdl_outp(V88, CHAT_SERVER, 0x000000FF);
}

void __cdecl kaillera_ui_debug(char * arg_0, ...) {
	char V8[1024];
	char V88[2084];
	sprintf_s(V8, 1021, "%s\r\n", arg_0);
	va_list args;
	va_start (args, arg_0);
	vsnprintf_s(V88, 2084, 2082, V8, args);
	va_end (args);

	sdl_outp(V88, CHAT_SERVER, 0x00777777);
}

void __cdecl kaillera_outpf(char * arg_0, ...) {
	char V8[1024];
	char V88[2084];
	sprintf_s(V8, 1021, "%s\r\n", arg_0);
	va_list args;
	va_start (args, arg_0);
	vsnprintf_s(V88, 2084, 2082, V8, args);
	va_end (args);

	sdl_outp(V88, CHAT_SERVER, 0x00000000);
}

#define NOTIMP(m) \
	kaillera_ui_debug("NOT IMPLEMENTED: %s; %s:%d", m, __FILE__, __LINE__);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void kaillera_user_add_callback(char*name, int ping, int status, unsigned short id, char conn){
	NOTIMP("kaillera_user_add_callback");
//	char bfx[500];
//	int x;
//	kaillera_sdlg_userslv.AddRow(name, id);
//	x = kaillera_sdlg_userslv.Find(id);
//	wsprintf(bfx, "%i", ping);
//	kaillera_sdlg_userslv.FillRow(bfx, 1, x);
//	kaillera_sdlg_userslv.FillRow(CONNECTION_TYPES[conn], 2, x);
//	kaillera_sdlg_userslv.FillRow(USER_STATUS[status], 3, x);
}
void kaillera_game_add_callback(char*gname, unsigned int id, char*emulator, char*owner, char*users, char status){
	NOTIMP("kaillera_game_add_callback");
//	int x;
//	
//	kaillera_sdlg_gameslv.AddRow(gname, id);
//	x = kaillera_sdlg_gameslv.Find(id);
//	
//	kaillera_sdlg_gameslv.FillRow(emulator, 1, x);
//	kaillera_sdlg_gameslv.FillRow(owner, 2, x);
//	kaillera_sdlg_gameslv.FillRow(GAME_STATUS[status], 3, x);
//	kaillera_sdlg_gameslv.FillRow(users, 4, x);
//	kaillera_sdlg_gameslvReSort();
}
void kaillera_game_create_callback(char*gname, unsigned int id, char*emulator, char*owner){
	NOTIMP("kaillera_game_create_callback");
//	kaillera_game_add_callback(gname, id, emulator, owner, "1/2", 0);
}

void kaillera_chat_callback(char*name, char * msg){
	NOTIMP("kaillera_chat_callback");
	kaillera_outpf("<%s> %s", name, msg);
}
void kaillera_game_chat_callback(char*name, char * msg){
	NOTIMP("kaillera_game_chat_callback");
//	kaillera_gdebug("<%s> %s", name, msg);
//	if (KSSDFA.state==2 && infos.chatReceivedCallback) {
//		infos.chatReceivedCallback(name, msg);
//		//l
//	}
}
void kaillera_motd_callback(char*name, char * msg){
	kaillera_ui_motd("- %s", msg);
}
void kaillera_user_join_callback(char*name, int ping, unsigned short id, char conn){
	NOTIMP("kaillera_user_join_callback");
//	kaillera_user_add_callback(name, ping, 1, id, conn);
	kaillera_ui_debug("* Joins: %s", name);
//	kaillera_sdlg_userslvReSort();
}
void kaillera_user_leave_callback(char*name, char*quitmsg, unsigned short id){
	NOTIMP("kaillera_user_leave_callback");
//	kaillera_sdlg_userslv.DeleteRow(kaillera_sdlg_userslv.Find(id));
	kaillera_ui_debug("* Parts: %s (%s)", name, quitmsg);
}
void kaillera_game_close_callback(unsigned int id){
	NOTIMP("kaillera_game_close_callback");
//	kaillera_sdlg_gameslv.DeleteRow(kaillera_sdlg_gameslv.Find(id));
}
void kaillera_game_status_change_callback(unsigned int id, char status, int players, int maxplayers){
	NOTIMP("kaillera_game_status_change_callback");
//	char * GAME_STATUS [] = 
//	{
//		"Waiting",
//		"Playing",
//		"Playing",
//	};
//	int x = kaillera_sdlg_gameslv.Find(id);
//	kaillera_sdlg_gameslv.FillRow(GAME_STATUS[status], 3, x);
//	char users [32];
//	wsprintf(users, "%i/%i", players, maxplayers);
//	kaillera_sdlg_gameslv.FillRow(users, 4, x);
//	kaillera_sdlg_gameslvReSort();
}

void kaillera_user_game_create_callback(){
	NOTIMP("kaillera_user_game_create_callback");
//	kaillera_sdlgGameMode();
//	kaillera_sdlg_LV_GULIST.DeleteAllRows();
//	SetWindowText(kaillera_sdlg_RE_GCHAT, "");
//	EnableWindow(kaillera_sdlg_BTN_KICK, TRUE);
//	EnableWindow(kaillera_sdlg_BTN_START, TRUE);
}
void kaillera_user_game_closed_callback(){
	NOTIMP("kaillera_user_game_closed_callback");
//	kaillera_sdlgNormalMode();
}

void kaillera_user_game_joined_callback(){
	NOTIMP("kaillera_user_game_joined_callback");
//	kaillera_sdlgGameMode();
//	kaillera_sdlg_LV_GULIST.DeleteAllRows();
//	SetWindowText(kaillera_sdlg_RE_GCHAT, "");
//	EnableWindow(kaillera_sdlg_BTN_KICK, FALSE);
//	EnableWindow(kaillera_sdlg_BTN_START, FALSE);
}

void kaillera_player_add_callback(char *name, int ping, unsigned short id, char conn){
	NOTIMP("kaillera_player_add_callback");
//	char bfx[32];
//	kaillera_sdlg_LV_GULIST.AddRow(name, id);
//	int x = kaillera_sdlg_LV_GULIST.Find(id);
//	wsprintf(bfx, "%i", ping);
//	kaillera_sdlg_LV_GULIST.FillRow(bfx, 1, x);	
//	kaillera_sdlg_LV_GULIST.FillRow(CONNECTION_TYPES[conn], 2, x);
//	int thrp = (ping * 60 / 1000 / conn) + 2;
//	wsprintf(bfx, "%i frames", thrp * conn - 1);
//	kaillera_sdlg_LV_GULIST.FillRow(bfx, 3, x);
}
void kaillera_player_joined_callback(char * username, int ping, unsigned short uid, char connset){
	NOTIMP("kaillera_player_joined_callback");
	kaillera_gdebug("* Joins: %s", username);
//	kaillera_player_add_callback(username, ping, uid, connset);
}
void kaillera_player_left_callback(char * user, unsigned short id){
	NOTIMP("kaillera_player_left_callback");
	kaillera_gdebug("* Parts: %s", user);
//	kaillera_sdlg_LV_GULIST.DeleteRow (kaillera_sdlg_LV_GULIST.Find(id));
}
void kaillera_user_kicked_callback(){
	NOTIMP("kaillera_user_kicked_callback");
//	kaillera_error_callback("* You have been kicked out of the game");
//	KSSDFA.input = KSSDFA_END_GAME;
//	KSSDFA.state = 0;
//	kaillera_sdlgNormalMode();
}
void kaillera_login_stat_callback(char*lsmsg){
	NOTIMP("kaillera_login_stat_callback");
	kaillera_core_debug("* %s", lsmsg);
}
void kaillera_player_dropped_callback(char * user, int gdpl){
	NOTIMP("kaillera_player_dropped_callback");
	kaillera_gdebug("* Dropped: %s (Player %i)", user, gdpl);
//	if (infos.clientDroppedCallback)
//		infos.clientDroppedCallback(user,gdpl);
//	if (gdpl == playerno) {
//		KSSDFA.input = KSSDFA_END_GAME;
//		KSSDFA.state = 0;
//	}
}
void kaillera_game_callback(char * game, char player, char players){
	NOTIMP("kaillera_game_callback");
//	if (game!= 0)
//		strcpy(GAME, game);
//	playerno = player;
//	numplayers = players;
//	kaillera_gdebug("kaillera_game_callback(%s, %i, %i)", GAME, playerno, numplayers);
//	kaillera_gdebug("press \"Drop\" if your emulator fails to load the game");
//	KSSDFA.input = KSSDFA_START_GAME;
}
void kaillera_game_netsync_wait_callback(int tx){
	NOTIMP("kaillera_game_netsync_wait_callback");
//	SetWindowText(kaillera_sdlg_ST_SPEED, "waiting for others");
//	int secs = tx / 1000;
//	int ssecs = (tx % 1000) / 100;
//	char xxx[32];
//	wsprintf(xxx,"%03i.%is", secs, ssecs);
//	SetWindowText(kaillera_sdlg_ST_DELAY, xxx);
//	kaillera_sdlg_delay = -1;
}
void kaillera_end_game_callback(){
	NOTIMP("kaillera_end_game_callback");
	KSSDFA.input = KSSDFA_END_GAME;
	KSSDFA.state = 0;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

typedef struct {
	char servname[32];
	char hostname[128];
	int port;
}KLSNST;

KLSNST KLSNST_temp;

char kaillera_sdlg_NAME[128];

void ParseServerAddress(const char * address) {
	char * host = KLSNST_temp.hostname;
	strncpy(host, address, 128);
	int port = 27888;
	while (*++host != ':' && *host != 0);
	if (*host == ':') {
		*host++ = 0x00;
		port = atoi(host);
		port = port==0?27888:port;
	}
	KLSNST_temp.port = port;
}

// Start gui handler
void kaillera_GUI(){
	char kaillera_server[128];

	nSettings::get_str("kaillera_server", kaillera_server, 128);
	ParseServerAddress(kaillera_server);

	KAILLERA_CORE_INITIALIZED = true;
	strcpy(kaillera_sdlg_NAME, KLSNST_temp.hostname);

	char un[32];
	nSettings::get_str("username", un, 32);

	char conset = nSettings::get_int("kaillera_cns") + 1;

	if (kaillera_core_initialize(0, APP, un, conset)) {
		Sleep(150);
	        
		kaillera_core_connect(KLSNST_temp.hostname, KLSNST_temp.port);

		while (1) {
			Sleep(100);
			if (kaillera_is_connected()) {
				//kaillera_gdebug("Connected to %s (%i users & %i games)", );
				break;
			}
		}

		while (1) Sleep(100);
		
		//disconnect
		char quitmsg[128];
		//GetWindowText(GetDlgItem(kaillera_ssdlg, IDC_QUITMSG), quitmsg, 128);
		kaillera_disconnect(quitmsg);
		kaillera_core_cleanup();
		
		KSSDFA.state = 0;
		KSSDFA.input = KSSDFA_END_GAME;
	} else {
		//MessageBox(pDlg, "Core Initialization Failed", 0, 0);
	}
}
