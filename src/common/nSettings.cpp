/******************************************************************************
***  n02 v0.3 winnt                                                         ***
**   Open Kaillera Client Core                                               **
***  For latest sources, visit http://sf.net/projects/okai                  ***
******************************************************************************/
#include "nSettings.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <map>
#include <string>

char file[5000];
char * subm;

char szKailleraHomePath[MAX_PATH] = "./.p2pkaillera";

#define INT_SETTING 0
#define STR_SETTING 1
typedef struct _SETTING
{
	unsigned int type;
	std::string sval;
	int dval;
	std::string desc;
} SETTING;

typedef std::map <std::string, SETTING> SETTINGS;
SETTINGS settings;

void nSettings::Initialize() {

	char *home = getenv("HOME");
	if(home) sprintf(szKailleraHomePath, "%s/.p2pkaillera", home);
	mkdir(szKailleraHomePath, 0777);
	if(!errno) {
		getcwd(szKailleraHomePath, MAX_PATH);
		strcat(szKailleraHomePath, "/.p2pkaillera");
		mkdir(szKailleraHomePath, 0777);
	}

	LoadCfg();
}

void nSettings::Terminate() {
//	WriteCfg();
}

void nSettings::DefaultCfg() {
	// force default settings
	SETTING set;
	settings.clear();
	
	set.type = STR_SETTING;
	set.sval = "user1";
	set.dval = 0;
	set.desc = "Username must be unique on kaillera server";
	settings["username"] = set;
	
	set.type = STR_SETTING;
	set.sval = "p2p";
	set.dval = 0;
	set.desc = "p2pKaillera library mode (p2p, client, playback)";
	settings["mode"] = set;
	
	set.type = STR_SETTING;
	set.sval = "192.168.1.1:27888";
	set.dval = 0;
	set.desc = "Address and port of server in kaillera client mode";
	settings["kaillera_server"] = set;
	
	set.type = INT_SETTING;
	set.sval = "";
	set.dval = 0;
	set.desc = "Connection speed from 60 packets/s to 10 packets/s; best value is 0, worse is 5";
	settings["kaillera_cns"] = set;

	set.type = INT_SETTING;
	set.sval = "";
	set.dval = 1;
	set.desc = "Does the unit host game in p2p mode? (value: 0 or 1)";
	settings["p2p_host"] = set;
	
	set.type = INT_SETTING;
	set.sval = "";
	set.dval = 27886;
	set.desc = "Port number for listening in p2p host mode";
	settings["p2p_port"] = set;
	
	set.type = STR_SETTING;
	set.sval = "10.1.2.1:27886";
	set.dval = 0;
	set.desc = "Address and port of server in p2p client mode";
	settings["p2p_server"] = set;
}

void nSettings::LoadCfg() {
	FILE *fp;
	char arg1[128];
	char arg2[255];
	int argd;
	char ligne[256];
	char filename[512];
	SETTING set;

	DefaultCfg();

	sprintf(filename, "%s/n02.cfg", szKailleraHomePath);

	// read config file
	if((fp = fopen(filename, "r")) != NULL) {
		while(fgets(ligne,sizeof(ligne),fp) != NULL) {
			sscanf(ligne, "%s %d", arg1, &argd);
			sscanf(ligne, "%s %s", arg1, arg2);
			if(strcmp(arg1, "#") != 0) {
				if (strcmp(arg1, "username") == 0 ||
						strcmp(arg1, "mode") == 0 ||
						strcmp(arg1, "kaillera_server") == 0 ||
						strcmp(arg1, "p2p_server") == 0) {
					set.type = STR_SETTING;
					set.sval = arg2;
					set.dval = 0;
					settings[arg1] = set;
				} else if (strcmp(arg1, "kaillera_cns") == 0 ||
						strcmp(arg1, "p2p_host") == 0 ||
						strcmp(arg1, "p2p_port") == 0) {
					set.type = INT_SETTING;
					set.sval = "";
					set.dval = argd;
					settings[arg1] = set;
				}
			}
		}
		fclose(fp);
	} else {
		WriteCfg();
	}


}

void nSettings::WriteCfg() {

	FILE *fp;
	char filename[512];
	char buf[1024];

	sprintf(filename, "%s/n02.cfg", szKailleraHomePath);

	fp = fopen(filename, "w");

	fputs("# p2pKaillera settings\n",fp);
	fputs("\n",fp);
	for (SETTINGS::iterator it = settings.begin(); it != settings.end(); it++) {
		SETTING set = (*it).second;
		if (set.type != INT_SETTING && set.type != STR_SETTING) {
			continue;
		}
		
		snprintf((char *)buf, 1024, "\n# %s\n", set.desc.c_str());
		fputs(buf, fp);
		
		if (set.type == INT_SETTING) {
			snprintf((char*)buf, 1024,  "%s %d\n", (*it).first.c_str(), set.dval);
		} else if (set.type == STR_SETTING) {
			snprintf((char*)buf, 1024,  "%s %s\n", (*it).first.c_str(), set.sval.c_str());
		}
		fputs(buf, fp);
	}

	fclose(fp);

}

int nSettings::get_int(char * key)
{
	if (settings.find(key) != settings.end() &&
		settings[key].type == INT_SETTING) {
		return settings[key].dval;
	}

	return 0;
}

char* nSettings::get_str(char * key, char * buf, size_t size)
{
	if (settings.find(key) != settings.end() &&
		settings[key].type == STR_SETTING) {
		strncpy(buf, settings[key].sval.c_str(), size);
		return buf;
	}

	return 0;
}

void nSettings::set_int(char * key, int val)
{
	settings[key].type = INT_SETTING;
	settings[key].dval = val;
	WriteCfg();
}

void nSettings::set_str(char * key, char * val)
{
	settings[key].type = STR_SETTING;
	settings[key].sval = val;
	WriteCfg();
}
