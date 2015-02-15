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
#define CHAT_TRACE	3

//===========================================================================
//=======================================================================

bool kaillera_RecordingEnabled() {
    return false;
}

void sdl_outp(char * line, int chat_type, unsigned int color) {
}

void console_outp(char * line, int chat_type) {
	char buf[1024];
	snprintf(buf, 1024, 
		(chat_type == CHAT_NONE ?  "%s" :
		(chat_type == CHAT_SERVER ? "SCHAT: %s" : 
		(chat_type == CHAT_GAME ? "GCHAT: %s" : 
			"TRACE: %s"))), line);
	kprintf(buf);
}

void emu_outp(char * nick, char * msg) {
	if (KSSDFA.state==2 && infos.chatReceivedCallback) {
		infos.chatReceivedCallback(nick, msg);
	}
}

void kaillera_outp(int chat_type, unsigned int color, bool send, char * arg_0, va_list args) {
	char V8[1024];
	char V88[2084];
	sprintf_s(V8, 1021, "%s", arg_0);
	vsnprintf_s(V88, 2084, 2082, V8, args);
	
	sdl_outp(V88, chat_type, color);
	console_outp(V88, chat_type);
	if (send) {
		emu_outp(NULL, V88);
	}
}

void kaillera_outp(int chat_type, unsigned int color, char * arg_0, va_list args) {
	kaillera_outp(chat_type, color, false, arg_0, args);
}

void __cdecl kaillera_game_debug(char * arg_0, ...) {
	va_list args;
	va_start (args, arg_0);
	kaillera_outp(CHAT_GAME, 0x00000000, true, arg_0, args);
	va_end (args);
}

void __cdecl kaillera_core_debug(char * arg_0, ...) {
	va_list args;
	va_start (args, arg_0);
	kaillera_outp(CHAT_SERVER, 0x00333333, arg_0, args);
	va_end (args);
}

void __cdecl kaillera_ui_debug(char * arg_0, ...) {
	va_list args;
	va_start (args, arg_0);
	kaillera_outp(CHAT_SERVER, 0x00777777, arg_0, args);
	va_end (args);
}

void __cdecl kaillera_ui_trace(char * arg_0, ...) {
#ifdef DEBUG
	va_list args;
	va_start (args, arg_0);
	kaillera_outp(CHAT_TRACE, 0x00777777, arg_0, args);
	va_end (args);
#endif
}

void __cdecl kaillera_ui_motd(char * arg_0, ...) {
	va_list args;
	va_start (args, arg_0);
	kaillera_outp(CHAT_SERVER, 0x00336633, arg_0, args);
	va_end (args);
}
void __cdecl kaillera_error_callback(char * arg_0, ...) {
	va_list args;
	va_start (args, arg_0);
	kaillera_outp(CHAT_SERVER, 0x000000FF, arg_0, args);
	va_end (args);
}

void __cdecl kaillera_soutpf(char * arg_0, ...) {
	va_list args;
	va_start (args, arg_0);
	kaillera_outp(CHAT_SERVER, 0x00000000, arg_0, args);
	va_end (args);
}

void __cdecl kaillera_goutpf(char * arg_0, ...) {
	va_list args;
	va_start (args, arg_0);
	kaillera_outp(CHAT_GAME, 0x00000000, arg_0, args);
	va_end (args);
}

#define NOTIMP(m) \
	kaillera_ui_debug("NOT IMPLEMENTED: %s; %s:%d", m, __FILE__, __LINE__);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void kaillera_user_add_callback(char*name, int ping, int status, unsigned short id, char conn){
	kaillera_ui_trace("kaillera_user_add_callback(name=%s; ping=%d, status=%d, id=%d, conn=%d)", name, ping, status, id, conn);
//	Add row to user list
//	char bfx[500];
//	int x;
//	kaillera_sdlg_userslv.AddRow(name, id);
//	x = kaillera_sdlg_userslv.Find(id);
//	wsprintf(bfx, "%i", ping);
//	kaillera_sdlg_userslv.FillRow(bfx, 1, x);
//	kaillera_sdlg_userslv.FillRow(CONNECTION_TYPES[conn], 2, x);
//	kaillera_sdlg_userslv.FillRow(USER_STATUS[status], 3, x);
}
int CURRENT_ID = -1;
void kaillera_game_add_callback(char*gname, unsigned int id, char*emulator, char*owner, char*users, char status){
	kaillera_ui_trace("kaillera_game_add_callback(gname=%s; id=%d, emulator=%s, owner=%s, users=%s, status=%d)",
		gname, id, emulator, owner, users, status);

	if (strcmp(GAME, gname) != 0) {
		CURRENT_ID = id;
		strcpy(GAME, gname);
	}

//	Add row to game list
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
	kaillera_ui_trace("kaillera_game_create_callback(gname=%s; id=%d, emulator=%s, owner=%s)", gname, id, emulator, owner);
	kaillera_game_add_callback(gname, id, emulator, owner, "1/2", 0);
}
void kaillera_chat_callback(char*name, char * msg){
	kaillera_ui_trace("kaillera_chat_callback(name=%s, msg=%s)", name, msg);
	kaillera_soutpf("<%s> %s", name, msg);
}
void kaillera_game_chat_callback(char*name, char * msg){
	kaillera_ui_trace("kaillera_game_chat_callback(name=%s, msg=%s)", name, msg);
	kaillera_goutpf("<%s> %s", name, msg);
	emu_outp(name, msg);
}
void kaillera_motd_callback(char*name, char * msg){
	kaillera_ui_trace("kaillera_motd_callback(name=%s, msg=%s)", name, msg);
	kaillera_ui_motd("- %s", msg);
}
void kaillera_user_join_callback(char*name, int ping, unsigned short id, char conn){
	kaillera_ui_trace("kaillera_user_join_callback(name=%s, ping=%d, id=%d, conn=%d)", name, ping, id, conn);
	kaillera_user_add_callback(name, ping, 1, id, conn);
	kaillera_ui_debug("* Joins: %s; ping = %i", name, ping);
//	TODO: sort user list
}
void kaillera_user_leave_callback(char*name, char*quitmsg, unsigned short id){
	kaillera_ui_trace("kaillera_user_leave_callback(name=%s, quitmsg=%s, id=%d)", name, quitmsg, id);
//	kaillera_sdlg_userslv.DeleteRow(kaillera_sdlg_userslv.Find(id));
	kaillera_ui_debug("* Left: %s (%s)", name, quitmsg);
}
void kaillera_game_close_callback(unsigned int id){
	kaillera_ui_trace("kaillera_game_close_callback(id=%d)", id);
//	kaillera_sdlg_gameslv.DeleteRow(kaillera_sdlg_gameslv.Find(id));
}
void kaillera_game_status_change_callback(unsigned int id, char status, int players, int maxplayers){
	kaillera_ui_trace("kaillera_game_status_change_callback(id=%d, status=%d, players=%d, maxplayers=%d)", id, status, players, maxplayers);
	if (status == 0 && players == 2 && CURRENT_ID == -1) {
		kaillera_start_game();
	}
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
	kaillera_ui_trace("kaillera_user_game_create_callback()");
//	kaillera_sdlgGameMode();
//	kaillera_sdlg_LV_GULIST.DeleteAllRows();
//	SetWindowText(kaillera_sdlg_RE_GCHAT, "");
//	EnableWindow(kaillera_sdlg_BTN_KICK, TRUE);
//	EnableWindow(kaillera_sdlg_BTN_START, TRUE);
}
void kaillera_user_game_closed_callback(){
	kaillera_ui_trace("kaillera_user_game_closed_callback()");
//	kaillera_sdlgNormalMode();
}

void kaillera_user_game_joined_callback(){
	kaillera_ui_trace("kaillera_user_game_joined_callback()");
//	kaillera_sdlgGameMode();
//	kaillera_sdlg_LV_GULIST.DeleteAllRows();
//	SetWindowText(kaillera_sdlg_RE_GCHAT, "");
//	EnableWindow(kaillera_sdlg_BTN_KICK, FALSE);
//	EnableWindow(kaillera_sdlg_BTN_START, FALSE);
}

void kaillera_player_add_callback(char *name, int ping, unsigned short id, char conn){
	kaillera_ui_trace("kaillera_player_add_callback(name=%s, ping=%d, id=%d, conn=%d)", name, ping, id, conn);
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
	kaillera_ui_trace("kaillera_player_joined_callback(username=%s, ping=%d, uid=%d, connset=%d)", username, ping, uid, connset);
	kaillera_game_debug("* Joins: %s", username);
	kaillera_player_add_callback(username, ping, uid, connset);
}
void kaillera_player_left_callback(char * user, unsigned short id){
	kaillera_ui_trace("kaillera_player_left_callback(user=%s, id=%d)", user, id);
	kaillera_game_debug("* Left: %s", user);
//	kaillera_sdlg_LV_GULIST.DeleteRow (kaillera_sdlg_LV_GULIST.Find(id));
}
bool PDROPPED = false;
void kaillera_user_kicked_callback(){
	kaillera_ui_trace("kaillera_user_kicked_callback()");
	kaillera_error_callback("* You have been kicked out of the game");
	KSSDFA.input = KSSDFA_END_GAME;
	KSSDFA.state = 0;
	PDROPPED = true;
//	kaillera_sdlgNormalMode();
}
void kaillera_login_stat_callback(char*lsmsg){
	kaillera_ui_trace("kaillera_login_stat_callback()");
	kaillera_ui_debug("* %s", lsmsg);
}
void kaillera_player_dropped_callback(char * user, int gdpl){
	kaillera_ui_trace("kaillera_player_dropped_callback(user=%s, gdpl=%d)", user, gdpl);
	kaillera_game_debug("* Dropped: %s (Player %i)", user, gdpl);
	if (infos.clientDroppedCallback)
		infos.clientDroppedCallback(user,gdpl);
	if (gdpl == playerno) {
		KSSDFA.input = KSSDFA_END_GAME;
		KSSDFA.state = 0;
		PDROPPED = true;
	}
}
void kaillera_game_callback(char * game, char player, char players){
	kaillera_ui_trace("kaillera_game_callback(game=%s, player=%i, players=%i)", game, player, players);
	if (game!= 0)
		strcpy(GAME, game);
	playerno = player;
	numplayers = players;
	//kaillera_game_debug("press \"Drop\" if your emulator fails to load the game");
	KSSDFA.input = KSSDFA_START_GAME;
}
void kaillera_game_netsync_wait_callback(int tx){
	kaillera_ui_trace("kaillera_game_netsync_wait_callback(tx=%i)", tx);
//	SetWindowText(kaillera_sdlg_ST_SPEED, "waiting for others");
//	int secs = tx / 1000;
//	int ssecs = (tx % 1000) / 100;
//	char xxx[32];
//	wsprintf(xxx,"%03i.%is", secs, ssecs);
//	SetWindowText(kaillera_sdlg_ST_DELAY, xxx);
//	kaillera_sdlg_delay = -1;
}
void kaillera_end_game_callback(){
	kaillera_ui_trace("kaillera_end_game_callback()");
	KSSDFA.input = KSSDFA_END_GAME;
	KSSDFA.state = 0;
	PDROPPED = true;
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
				//kaillera_game_debug("Connected to %s (%i users & %i games)", );
				kaillera_game_debug("Connected to ...");
				break;
			}
		}

		// Emulate activity
		Sleep(3000);
		if (CURRENT_ID != -1) {
			kaillera_join_game(CURRENT_ID);
		}
		else {
			strcpy(GAME, gamelist);
			kaillera_create_game(GAME);
		}


		while (!PDROPPED) Sleep(100);
		
		//disconnect
		char quitmsg[128];
		//GetWindowText(GetDlgItem(kaillera_ssdlg, IDC_QUITMSG), quitmsg, 128);
		kaillera_disconnect(quitmsg);
		kaillera_core_cleanup();
		
		KSSDFA.state = 0;
		KSSDFA.input = KSSDFA_END_GAME;

		PDROPPED = false;
		CURRENT_ID = -1;
		GAME[0] = 0;
	} else {
		//MessageBox(pDlg, "Core Initialization Failed", 0, 0);
	}
}
