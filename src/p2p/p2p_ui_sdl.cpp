#include "kailleraclient.h"

#include "linux.h"

#include "common/nSettings.h"
#include "p2p_ui.h"
#include "common/nSTL.h"


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void outp(char * line);
void __cdecl outpf(char * arg_0, ...) {
	char V8[1024];
	char V88[2084];
	sprintf(V8, "%s", arg_0);
	va_list args;
	va_start (args, arg_0);
	vsprintf (V88, V8, args);
	va_end (args);
	outp(V88);
}
void __cdecl p2p_core_debug(char * arg_0, ...) {
#ifdef DEBUG
	char V8[1024];
	char V88[2084];
	sprintf(V8, "%s", arg_0);
	va_list args;
	va_start (args, arg_0);
	vsprintf (V88, V8, args);
	va_end (args);
	outp(V88);
#endif
}

void p2p_outc(char * nick, char * msg);
void outp(char * line) {
	kprintf(line);
#ifdef DEBUG
	p2p_outc(NULL, line);
#endif
}

#define NOTIMP(m) \
	outpf("NOT IMPLEMENTED: %s; %s:%d", m, __FILE__, __LINE__);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int PACKETLOSSCOUNT;
int PACKETMISOTDERCOUNT;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool HOST;
int PORT;
char GAME[128];
char APP[128];
char IP[128];
char USERNAME[128];
int playerno;
int numplayers;

bool COREINIT = false;
bool DROPPED = false;

void p2p_outc(char * nick, char * msg) {
	if (KSSDFA.state==2 && infos.chatReceivedCallback) {
		infos.chatReceivedCallback(nick != NULL ? nick : USERNAME, msg);
	}
}

void p2p_ui_pcs(){
	
	outpf("============= Core status begin ===============");

	p2p_print_core_status();

	unsigned int sta =  KSSDFA.state;
	unsigned int inp = KSSDFA.input;
	outpf("KSSDFA { state: %i, input: %i }", sta, inp);
	outpf("PACKETLOSSCOUNT=%u", PACKETLOSSCOUNT);
	outpf("PACKETMISOTDERCOUNT=%u", PACKETMISOTDERCOUNT);
	outpf("============ Core status end =================");
}




void p2p_ping_callback(int PING){
	NOTIMP("p2p_ping_callback");
	char buf[200];
	sprintf(buf, "ping: %i ms pl: %i", PING, PACKETLOSSCOUNT);
	outpf(buf);
}



void p2p_chat_callback(char * nick, char * msg){
	outpf("CHAT: <%s> %s",nick, msg);
#ifndef DEBUG // In debug outpf() sends all messsages to emulator
	p2p_outc(nick, msg);
#endif
}

bool p2p_add_delay_callback(){
	NOTIMP("p2p_add_delay_callback");
	return true;
}
void p2p_game_callback(char * game, int playernop, int maxplayersp){
	strcpy(GAME, game);
	playerno = playernop;
	numplayers = maxplayersp;
	KSSDFA.input = KSSDFA_START_GAME;
}

void p2p_end_game_callback(){
	KSSDFA.input = KSSDFA_END_GAME;
	KSSDFA.state = 0;
	DROPPED = true;
}

void p2p_client_dropped_callback(char * nick, int playerno){
	if (infos.clientDroppedCallback) {
		infos.clientDroppedCallback(nick, playerno);
	}
}


void p2p_ui_chat_send(char * xxx){
	if (strcmp(xxx, ":pcs")==0) {
		p2p_ui_pcs();
		return;
	}
	if (p2p_is_connected()) {
		if (*xxx == ':') {
			xxx++;
			if (strcmp(xxx, "ping")==0) {
				p2p_ping();
			} else if (strcmp(xxx, "retr")==0) {
				p2p_retransmit();
			} else {
				outpf("Unknown command \"%s\"", xxx);
				p2p_send_chat(xxx-1);
			}
		} else {
			p2p_send_chat(xxx);
		}
	}
}

void p2p_EndGame(){
	outpf("dropping game");
	p2p_drop_game();
}
bool p2p_SelectServerDlgStep(){
	if (COREINIT) {
		p2p_step();
		return true;
	}
	return false;
}



bool p2p_RecordingEnabled(){
	NOTIMP("p2p_RecordingEnabled");
//	return SendMessage(GetDlgItem(p2p_ui_connection_dlg, CHK_REC), BM_GETCHECK, 0, 0)==BST_CHECKED;
}



/////////////////////////////////////////////////

void p2p_hosted_game_callback(char * game){
	// In this implementation gamelist contains one game which was selected in emulator menu.
	// After game start emulator must check that game contains in their list
	return;
#if 0
	if (gamelist != 0) {
		char * xx = gamelist;
		int p;
		while ((p=strlen(xx))!= 0){
			if (strcmp(game, xx)==0) {
				return;
			}
			xx += p+ 1;
		}
	}
	
	outpf("ERROR: Game not found on your local list");
#endif
}

//#define KAILLERA_FILE // TODO: remove from release

void p2p_GUI(){
#ifdef KAILLERA_FILE
	FILE * f = fopen("./host_kaillera", "r");
	HOST = (f == NULL);
#else
	HOST = nSettings::get_int("p2p_host");
#endif
	PORT = HOST ? nSettings::get_int("p2p_port") : 0;
	strcpy(GAME, gamelist);
	nSettings::get_str("username", USERNAME, sizeof(USERNAME) / sizeof(USERNAME[0]));

#ifdef KAILLERA_FILE
	if (HOST) {
		f = fopen("./host_kaillera", "w");
	}
	if (f != NULL) {
		fclose(f);
	}
		strcpy(IP, "127.0.0.1");
#else
	nSettings::get_str("p2p_server", IP, sizeof(IP) / sizeof(IP[0]));
#endif

	if (p2p_core_initialize(HOST, PORT, APP, GAME, USERNAME)){
		
		if (HOST) {
			outpf("Hosting %s on port %i", GAME, p2p_core_get_port());
			outpf("WARNING: Hosting requires hosting ports to be forwarded and enabled in firewalls.");
		} else {

			char * host;
			host = IP;
			int port = 27886;
			while (*++host != ':' && *host != 0);
			if (*host == ':') {
				*host++ = 0x00;
				port = atoi(host);
				port = port==0?27886:port;
			}
			host = IP;
//			76.81.211.10:27886
			outpf("Connecting to %s:%i", host, port);

			if (!p2p_core_connect(host, port)){

				//MessageBox(hDlg, "Error connecting to specified host/port", host, 0);
				//EndDialog(hDlg, 0);
				return;
			}
		}
		
		COREINIT = true;

		Sleep(2);
		p2p_set_ready(true);

		while(1) {
			Sleep(100);
			if (DROPPED)
			{
				p2p_set_ready(false);
				KSSDFA.state = 3;
				DROPPED = false;
				if (p2p_disconnect())
				{
					p2p_core_cleanup();
				}
				break;
			}
		}

	} else outpf("Error initializing sockets");
}

