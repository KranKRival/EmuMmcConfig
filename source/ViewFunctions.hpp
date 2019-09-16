#pragma once
#include <iostream>
#include <string>
#include "UI/DialogPages.hpp"

const std::string VersionString = "2.1.0";
extern std::string SystemVer;

extern bool UseAnimations;

void PushPage(IUIControlObj* page); 
void PushPageBlocking(IUIControlObj* page);
void PopPage();
void ErrorFatal(const std::string &msg);
void Dialog(const std::string &msg);
void DialogBlocking(const std::string &msg);
void DisplayLoading(const std::string &msg);
void QuitApp();