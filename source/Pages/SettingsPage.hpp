#pragma once
#include <switch.h>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "../SwitchThemesCommon/SwitchThemesCommon.hpp"
#include "../UI/UI.hpp"
#include "../fs.hpp"

class SettingsPage : public IPage
{
	public:
		SettingsPage();	
		
		void Render(int X, int Y) override;
		void Update() override;
	private:	
		void LoadShuffleState();
		void UpdateAnimState();
		void WriteShuffleFlag(int i);
		
		void ClearBtnState();	
		
		Label nintendo_pathlabel;
		Label idlabel;
		Label Pathlabel;
		Label Warninglabel;
		Label Warninglabel2;
		Label Warninglabel3;
		Label Warninglabel4;
		Label EmuMmclabel;
		Label Sectorlabel;
		Button btnEmummcToogle;
		
		int selectedIndex = 0;
		
		bool hasFocus = true;
};
