#include "SettingsPage.hpp"
#include "../input.hpp"
#include "../ViewFunctions.hpp"
#include "../minIni.h"

using namespace std;
#include "stdio.h"
#include "dirent.h" /* NOT PRESENT on all systems */
#include "stdlib.h"
#include "sys/stat.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

const char inifile[] = "/emummc/emummc.ini";
#define FILE_NAME "/emummc/emummc.ini"
#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))
bool boolEmuMMC;
bool inifound;
string sectorEmuMMC;
string pathEmuMMC;
string idEmuMMC;
string nintendo_pathEmuMMC;

string RAW1global;
string RAW2global;
string SDraw;

int RAWcount = 0;
bool firststart = true;

std::string listdir(const char *path)
{
	ostringstream c_content;
    DIR *c_dir = NULL;
	int startline, startlinep, endline = 0,length = 10;
    c_dir = opendir (path);
    struct dirent *contents = NULL;
    if (c_dir == NULL)
    {
        //cout << "\nError: current directory could not be opened correctly";
		//pause();
        return;
    }

    while (contents = readdir (c_dir))
    {
        if (contents == NULL)
		{
			//cout << "\nError: current directory could not be read correctly";
			//pause();
			return;
		}
		c_content << contents->d_name << "\n";
	}
	closedir(c_dir);
	string cdir_parse = c_content.str();


	return cdir_parse;
}

std::string listdirs(const char *path)
{
	ostringstream c_content;
	struct dirent *contents;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    DIR *c_dir = opendir(path); 
  
    if (c_dir == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
        return 0; 
    } 
  
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
    // for readdir() 
    while ((contents = readdir(c_dir)) != NULL) 
	{
		printf("%s\n", contents->d_name); 
		if(contents->d_type == DT_DIR)
		{
			c_content << contents->d_name << "\n";
			//c_content << contents->d_name;
		}
		
	}
            
  
    string cdir_parse = c_content.str();
	closedir(c_dir); 
	return cdir_parse;
}

unsigned int swapByteOrder(unsigned int& ui)
{
    ui = (ui >> 24) |
         ((ui<<8) & 0x00FF0000) |
         ((ui>>8) & 0x0000FF00) |
         (ui << 24);

		 return ui;
}

uint32_t read_u32(std::istream& f)
{
    unsigned int val;
    char bytes[4];
    f.read(bytes, 4);

    val = bytes[3]<< 0 | (bytes[2] << 8) | (bytes[1] << 16) | (bytes[0] << 24);
    
    return swapByteOrder(val);
}

string GetRaw()
{
	string open = listdirs("/emuMMC");
	string get = "/emuMMC/" + open;
	string reopen = get + "/raw_based";  

	char buffer[1024];
	sprintf(buffer, "%s", reopen.c_str());
	std::ifstream f(buffer, std::ios::in | std::ios::binary);

	char Decimel[100];
	sprintf(Decimel, "0x%02x", read_u32(f));
	string WTF(Decimel);

	return WTF;
}

void Set_EmuMMC()
{
    char str_emummc_enabled[100];
    long _emummc_enabled;
    _emummc_enabled = ini_gets("emummc", "enabled", "not found", str_emummc_enabled, sizearray(str_emummc_enabled), inifile); 
    char LocalBuff[100];
    FILE *fp = fopen("/emummc/emummc.ini","r+");
	if(fp)
	{
		if(strcmp(str_emummc_enabled, "1") == 0) 
        {
            if(fp != NULL)
            writesection(LocalBuff,"emummc", &fp);
            writekey(LocalBuff, "enabled", "0", &fp);
        }
        else if(strcmp(str_emummc_enabled, "0") == 0) 
        {
            if(fp != NULL)
            writesection(LocalBuff,"emummc", &fp);
            writekey(LocalBuff, "enabled", "1", &fp);
        }
        else
        {
            //printf(ANSI_COLOR_RED "Oops Something went Wrong :(\n\n\n" ANSI_COLOR_RESET);
        }
	}
	else
	{
		/* code */
	}
	
	fclose(fp);
}

void Switch_EmuMMC()
{
    char str_emummc_enabled[100];
    long _emummc_enabled;
    _emummc_enabled = ini_gets("emummc", "enabled", "not found", str_emummc_enabled, sizearray(str_emummc_enabled), inifile); 

	char str_emummc_sector[100];
    long _emummc_sector;
    _emummc_sector = ini_gets("emummc", "sector", "not found", str_emummc_sector, sizearray(str_emummc_sector), inifile); 

	char str_emummc_path[100];
    long _emummc_path;
    _emummc_path = ini_gets("emummc", "path", "not found", str_emummc_path, sizearray(str_emummc_path), inifile); 

	char str_emummc_id[100];
    long _emummc_id;
    _emummc_id = ini_gets("emummc", "id", "not found", str_emummc_id, sizearray(str_emummc_id), inifile);

	char str_emummc_nintendo_path[200];
    long _emummc_nintendo_path;
    _emummc_nintendo_path = ini_gets("emummc", "nintendo_path", "not found", str_emummc_nintendo_path, sizearray(str_emummc_nintendo_path), inifile);

    char LocalBuff[1024];

    FILE *fp = fopen("/emummc/emummc.ini","r+");
	if(fp)
	{
		if(strcmp(str_emummc_enabled, "1") == 0) 
        {
            if(fp != NULL)
            writesection(LocalBuff,"emummc", &fp);
            writekey(LocalBuff, "enabled", "1", &fp);
        }
        else if(strcmp(str_emummc_enabled, "0") == 0) 
        {
            if(fp != NULL)
            writesection(LocalBuff,"emummc", &fp);
            writekey(LocalBuff, "enabled", "0", &fp);
        }

		if(strcmp(str_emummc_sector, RAW1global.c_str()) == 0) 
		{
			writekey(LocalBuff, "sector", RAW2global.c_str(), &fp);
		}
		else if(strcmp(str_emummc_sector, RAW2global.c_str()) == 0) 
		{
			writekey(LocalBuff, "sector", RAW1global.c_str(), &fp);
		}

		

		if(strcmp(str_emummc_path, "emuMMC/RAW1") == 0) 
		{
			writekey(LocalBuff, "path", "emuMMC/RAW2", &fp);
			SDraw = "SD RAW1";
		}
		else if(strcmp(str_emummc_path, "emuMMC/RAW2") == 0) 
		{
			writekey(LocalBuff, "path", "emuMMC/RAW1", &fp);
			SDraw = "SD RAW2";
		}

		if(strcmp(str_emummc_id, str_emummc_id) == 0) 
		{
			writekey(LocalBuff, "id ", str_emummc_id, &fp);
		}
		
		

		if(strcmp(str_emummc_nintendo_path, "emuMMC/RAW1/Nintendo") == 0) 
		{
			writekey(LocalBuff, "nintendo_path", "emuMMC/RAW2/Nintendo", &fp);
		}
		else if(strcmp(str_emummc_nintendo_path, "emuMMC/RAW2/Nintendo") == 0) 
		{
			writekey(LocalBuff, "nintendo_path", "emuMMC/RAW1/Nintendo", &fp);
		}

		;
	}
	else
	{
		/* code */
	}
	
	fclose(fp);
}

void Get_EmuMMC()
{
    char str_emummc_enabled[100];
    long _emummc_enabled;
    _emummc_enabled = ini_gets("emummc", "enabled", "not found", str_emummc_enabled, sizearray(str_emummc_enabled), inifile); 
    
	char char_emummc_sector[200];
    long _emummc_sector;
    _emummc_sector = ini_gets("emummc", "sector", "not found", char_emummc_sector, sizearray(char_emummc_sector), inifile);
    char buf_emummc_sector[300];
    sprintf(buf_emummc_sector, "Partition Sector: %s", char_emummc_sector);
    string string_emummc_sector(buf_emummc_sector, 100); 

	char char_emummc_path[200];
    long _emummc_path;
    _emummc_path = ini_gets("emummc", "path", "not found", char_emummc_path, sizearray(char_emummc_path), inifile);
    char buf_emummc_path[300];
    sprintf(buf_emummc_path, "Path: %s", char_emummc_path);
    string string_emummc_path(buf_emummc_path, 100); 

	char char_emummc_id[200];
    long _emummc_id;
    _emummc_id = ini_gets("emummc", "id", "not found", char_emummc_id, sizearray(char_emummc_id), inifile);
    char buf_emummc_id[300];
    sprintf(buf_emummc_id, "Id: %s", char_emummc_id);
    string string_emummc_id(buf_emummc_id, 100); 

	char char_emummc_nintendo_path[200];
    long _emummc_nintendo_path;
    _emummc_nintendo_path = ini_gets("emummc", "nintendo_path", "not found", char_emummc_nintendo_path, sizearray(char_emummc_nintendo_path), inifile);
    char buf_emummc_nintendo_path[300];
    sprintf(buf_emummc_nintendo_path, "Nintendo path: %s", char_emummc_nintendo_path);
    string string_emummc_nintendo_path(buf_emummc_nintendo_path, 100); 
	
	if(buf_emummc_sector != NULL) 
    {
            sectorEmuMMC = string_emummc_sector;
    } 
	if(buf_emummc_path != NULL) 
    {
            pathEmuMMC = string_emummc_path;
    }    
	if(buf_emummc_id != NULL) 
    {
            idEmuMMC = string_emummc_id;
    }
	if(buf_emummc_nintendo_path != NULL) 
    {
            nintendo_pathEmuMMC = string_emummc_nintendo_path;
    }

	if(strcmp(char_emummc_path, "emuMMC/RAW1") == 0) 
	{
		SDraw = "SD RAW1";
	}
	else if(strcmp(char_emummc_path, "emuMMC/RAW2") == 0) 
	{
		SDraw = "SD RAW2";
	}

    if (!boolEmuMMC)
    {
        if(strcmp(str_emummc_enabled, "1") == 0) 
        {
            boolEmuMMC = true;
        }
    }
    else
    {
        if(strcmp(str_emummc_enabled, "0") == 0) 
        {
            boolEmuMMC = false;
        }
    }
}

SettingsPage::SettingsPage() : 


nintendo_pathlabel("", WHITE, 880, font30),
idlabel("", WHITE, 880, font30),
Pathlabel("", WHITE, 880, font30),
Warninglabel("", {255, 215 , 0, 255}, 880, font30),
Warninglabel2("", {255, 215 , 0, 255}, 880, font30),
Warninglabel3("", {11, 255, 209, 255}, 880, font35),
Warninglabel4("", {255, 215 , 0, 255}, 880, font40),
EmuMmclabel("", WHITE, 880, font30),
Sectorlabel("", WHITE, 880, font30),
btnEmummcToogle("")
{
	Name = "EMUMMC";
	btnEmummcToogle.selected = false;
	
	UpdateAnimState();
	
	hasFocus = false;
	LoadShuffleState();
}




void SettingsPage::UpdateAnimState()
{	
	Get_EmuMMC();
	if (!boolEmuMMC)
    {
		btnEmummcToogle.SetBorderColor({0, 195, 227, 20});
	}
    else
    {
		btnEmummcToogle.SetBorderColor({255, 69, 84, 255});
	}
	btnEmummcToogle.SetString(boolEmuMMC ? "Disable" : "Enable");

	EmuMmclabel.SetString(boolEmuMMC ? "EMUMMC is: enabled" : "EMUMMC is: disabled");
	Sectorlabel.SetString(sectorEmuMMC);
	Pathlabel.SetString(pathEmuMMC);
	idlabel.SetString(idEmuMMC);
	nintendo_pathlabel.SetString(nintendo_pathEmuMMC);

	FILE *fp = fopen("/emummc/emummc.ini","r+");
	if(fp)
	{
		//Warninglabel.SetString("Information: \n\n You need to already have EMUMMC set up to use this app.\n You currently need to change sector manually to switch EMUMMC.\n multi EMUMMC support in Ver. 2.1.0 !");
		
        string open = listdirs("/emuMMC");
		//string get = "/emuMMC/" + open;

		string RAW1;
		char RAW1buffer[1024];
		char RAW1Decimel[100];

		string RAW2;
		char RAW2buffer[1024];
		char RAW2Decimel[100];

		string RAW3;
		char RAW3buffer[1024];
		char RAW3Decimel[100];
		
		string s0 = open;
		string s1 = "RAW1";
		string s2 = "RAW2";
		string s3 = "RAW3";
		
		
		if(strstr(s0.c_str(),s1.c_str()))
		{
			RAW1 = "/emuMMC/RAW1/raw_based";  
			sprintf(RAW1buffer, "%s", RAW1.c_str());
			std::ifstream f(RAW1buffer, std::ios::in | std::ios::binary);
			sprintf(RAW1Decimel, "0x%02x", read_u32(f));
			if (RAWcount < 2)
			{
				RAWcount += 1;
			}
		}
		if(strstr(s0.c_str(),s2.c_str()))
		{
			RAW2 = "/emuMMC/RAW2/raw_based";  
			sprintf(RAW2buffer, "%s", RAW2.c_str());
			std::ifstream f(RAW2buffer, std::ios::in | std::ios::binary);
			sprintf(RAW2Decimel, "0x%02x", read_u32(f));
			if (RAWcount < 2)
			{
				RAWcount += 1;
			}	
		}
		string RAW1Out(RAW1Decimel);
		RAW1global = RAW1Out;
		string RAW2Out(RAW2Decimel);
		RAW2global = RAW2Out;

		string RAWcountstr = "( " + to_string(RAWcount) + " )";

		string ListOfMMC = "EmuMMC Setups Found: " + RAWcountstr + "\n\n";
		Warninglabel.SetString(ListOfMMC + "1. " + s1 + "   /   " + RAW1Out);
		Warninglabel2.SetString("2. " + s2 + "   /   " + RAW2Out);
		Warninglabel3.SetString("Current Setup: " + SDraw);
		if (RAWcount > 1)
		{
			Warninglabel4.SetString("Hold  (  R  )  and Press  (  Y  )  to switch EmuMMC setup");
		}
		
		

		
		//Warninglabel.SetString(GetRaw());
		inifound = true;
	}
	else
	{
		Warninglabel.SetString("Warning: \n\n SD:/emuMMC/emummc.ini not found\n setup EMUMMC first");
		inifound = false;
	}
	fclose(fp);
	
	
}

void SettingsPage::Render(int X, int Y)
{
	int baseY = Y + 20;	
	Warninglabel.Render(X + 20 , baseY);
	baseY += Warninglabel.GetSize().h + 20;

	int baseY2 = baseY + 5;	
	Warninglabel2.Render(X + 20 , baseY2);
	baseY2 += Warninglabel2.GetSize().h + 20;

	Warninglabel3.Render(X + 120 , baseY2 + 65);
	Warninglabel4.Render(X + 130 , baseY2 + 390);
	if (inifound)
	{
		baseY2 += 5;
		EmuMmclabel.Render(X + 20 , baseY2);
		baseY2 += EmuMmclabel.GetSize().h + 20;

		btnEmummcToogle.Render(X + 20, baseY2);

	    baseY2 += 70;
		Sectorlabel.Render(X + 20 , baseY2);
		baseY2 += Sectorlabel.GetSize().h + 20;

		baseY2 += 5;
		Pathlabel.Render(X + 20 , baseY2);
		baseY2 += Pathlabel.GetSize().h + 20;

		baseY2 += 5;
		idlabel.Render(X + 20 , baseY2);
		baseY2 += idlabel.GetSize().h + 20;

		baseY2 += 5;
		nintendo_pathlabel.Render(X + 20 , baseY2);
		baseY2 += nintendo_pathlabel.GetSize().h + 20;
	}
	
	
}

void SettingsPage::LoadShuffleState()
{
	
}

void SettingsPage::WriteShuffleFlag(int i)
{

}

void SettingsPage::ClearBtnState()
{
	btnEmummcToogle.selected = false;
}

/*
Page layout:
~At times like this i regret not using imgui~

btnAnimEnable index = 0
v^
btnRandom (index = 1) <> btnCycle (index = 2)
v^
btnEmummcToogle (index = 3)
*/

void SettingsPage::Update()
{
			
			
			btnEmummcToogle.selected = true;
			if (kDown & KEY_A)
			{
				if (selectedIndex == 0)
				{
					boolEmuMMC = !boolEmuMMC;
					Set_EmuMMC();
					UpdateAnimState();
				}
			}
			else if (kDown & KEY_B || kDown & KEY_LEFT)
			{
				btnEmummcToogle.selected = false;
				Parent->PageLeaveFocus(this);
			}

			if (kHeld & KEY_R && kDown & KEY_Y)
			{
				Switch_EmuMMC();
				UpdateAnimState();
			}
}