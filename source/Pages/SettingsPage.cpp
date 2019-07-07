#include "SettingsPage.hpp"
#include "../input.hpp"
#include "../ViewFunctions.hpp"
#include "../minIni.h"

using namespace std;

const char inifile[] = "/emummc/emummc.ini";
#define FILE_NAME "/emummc/emummc.ini"
#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))
bool boolEmuMMC;
bool inifound;
string sectorEmuMMC;
string pathEmuMMC;
string idEmuMMC;
string nintendo_pathEmuMMC;

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
            writekey(LocalBuff, "enabled ", " 0", &fp);
        }
        else if(strcmp(str_emummc_enabled, "0") == 0) 
        {
            if(fp != NULL)
            writesection(LocalBuff,"emummc", &fp);
            writekey(LocalBuff, "enabled ", " 1", &fp);
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


nintendo_pathlabel("", WHITE, 880, font25),
idlabel("", WHITE, 880, font25),
Pathlabel("", WHITE, 880, font25),
Warninglabel("", {255, 215 , 0, 255}, 880, font25),
EmuMmclabel("", WHITE, 880, font25),
Sectorlabel("", WHITE, 880, font25),
btnClear("")
{
	Name = "EMUMMC";
	btnClear.selected = false;
	
	UpdateAnimState();
	
	hasFocus = false;
	LoadShuffleState();
}


void SettingsPage::UpdateAnimState()
{	
	Get_EmuMMC();
	if (!boolEmuMMC)
    {
		btnClear.SetBorderColor({0, 195, 227, 20});
	}
    else
    {
		btnClear.SetBorderColor({255, 69, 84, 255});
	}
	btnClear.SetString(boolEmuMMC ? "Disable" : "Enable");
	EmuMmclabel.SetString(boolEmuMMC ? "EMUMMC is: enabled" : "EMUMMC is: disabled");
	Sectorlabel.SetString(sectorEmuMMC);
	Pathlabel.SetString(pathEmuMMC);
	idlabel.SetString(idEmuMMC);
	nintendo_pathlabel.SetString(nintendo_pathEmuMMC);

	FILE *fp = fopen("/emummc/emummc.ini","r+");
	if(fp)
	{
		Warninglabel.SetString("Information: \n\n You need to already have EMUMMC set up to use this app.\n You currently need to change sector manually to switch EMUMMC.\n multi EMUMMC support in Ver. 2.1.0 !");
		inifound = true;
	}
	else
	{
		Warninglabel.SetString("Warning: \n\n SD:/emuMMC/emummc.ini not found\n setup EMUMMC or place the ini manualy");
		inifound = false;
	}
	fclose(fp);
	
	
}

void SettingsPage::Render(int X, int Y)
{
	int baseY = Y + 20;	
	Warninglabel.Render(X + 20 , baseY);
	baseY += Warninglabel.GetSize().h + 20;

	if (inifound)
	{
		baseY += 20;
		EmuMmclabel.Render(X + 20 , baseY);
		baseY += EmuMmclabel.GetSize().h + 20;
		btnClear.Render(X + 20, baseY);

	    baseY += 80;
		Sectorlabel.Render(X + 20 , baseY);
		baseY += Sectorlabel.GetSize().h + 20;

		baseY += 10;
		Pathlabel.Render(X + 20 , baseY);
		baseY += Pathlabel.GetSize().h + 20;

		baseY += 10;
		idlabel.Render(X + 20 , baseY);
		baseY += idlabel.GetSize().h + 20;

		baseY += 10;
		nintendo_pathlabel.Render(X + 20 , baseY);
		baseY += nintendo_pathlabel.GetSize().h + 20;
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
	btnClear.selected = false;
}

/*
Page layout:
~At times like this i regret not using imgui~

btnAnimEnable index = 0
v^
btnRandom (index = 1) <> btnCycle (index = 2)
v^
btnClear (index = 3)
*/

void SettingsPage::Update()
{
			
			
			btnClear.selected = true;
			if (kDown & KEY_A)
			{
				if (selectedIndex == 0)
				{
					boolEmuMMC = !boolEmuMMC;
					Set_EmuMMC();
					UpdateAnimState();
				}
			}
			else if (kDown & KEY_B || kDown & KEY_LEFT){
				btnClear.selected = false;
				Parent->PageLeaveFocus(this);
			}
}