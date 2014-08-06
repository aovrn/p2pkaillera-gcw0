#include "player.h"
#include "kailleraclient.h"
#include <time.h>
bool player_playing = false;

class PlayBackBufferC {
public:
	char * buffer;
	char * ptr;
	char * end;
    
    void load_bytes(void * arg_0, unsigned int arg_4){
        if (ptr + 10 < end) {
            int p = min(arg_4, end - ptr);
            memcpy(arg_0, ptr, p);
            ptr += p;
        }
    }
    void load_str(char * arg_0, unsigned int arg_4){
		arg_4 = min(arg_4, strlen(ptr)+1);
        arg_4 = min(arg_4, end-ptr+1);
		load_bytes(arg_0, arg_4);
		arg_0[arg_4] = 0x00;
    }
	int load_int(){
        int x;
        load_bytes(&x,4);
        return x;
    }
    unsigned char load_char(){
        unsigned char x;
        load_bytes(&x,1);
        return x;
    }
    unsigned short load_short(){
        unsigned short x;
        load_bytes(&x,2);
        return x;
    }
	
} PlayBackBuffer;

//==============================================


void player_GUI(){
}

int player_MPV(void*values,int size){
	if (player_playing){
		if (PlayBackBuffer.ptr + 10 < PlayBackBuffer.end) {
			char b = PlayBackBuffer.load_char();
			if (b==0x12) {
				int l = PlayBackBuffer.load_short();
				if (l < 0) {
					player_playing = false;
					return -1;
				}
				if (l > 0)
					PlayBackBuffer.load_bytes((char*)values, l);//access error
				return l;
			}
			if (b==20) {
				char playernick[100];
				PlayBackBuffer.load_str(playernick, 100);
				int pn = PlayBackBuffer.load_int();
				infos.clientDroppedCallback(playernick, pn);
				return player_MPV(values, size);
			}
			if (b==8) {
				char nick[100];
				char msg[500];
				PlayBackBuffer.load_str(nick, 100);
				PlayBackBuffer.load_str(msg, 500);
				infos.chatReceivedCallback(nick, msg);
				return player_MPV(values, size);
			}
		} else player_playing = false;
	}
	return -1;
}
void player_EndGame(){
	player_playing =false;
	KSSDFA.input = KSSDFA_END_GAME;
	KSSDFA.state = 0;
}
bool player_SSDSTEP(){
	return false;
}
void player_ChatSend(char*){
	
}
bool player_RecordingEnabled(){
	return false;
}


