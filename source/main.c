// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>
#include "minIni.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"

const char inifile[] = "/emummc/emummc.ini";
#define FILE_NAME "/emummc/emummc.ini"
#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))
bool can_reboot = true;
char str_Key_Main[40];

#define IRAM_PAYLOAD_MAX_SIZE 0x2F000
#define IRAM_PAYLOAD_BASE 0x40010000

static alignas(0x1000) u8 g_reboot_payload[IRAM_PAYLOAD_MAX_SIZE];
static alignas(0x1000) u8 g_ff_page[0x1000];
static alignas(0x1000) u8 g_work_page[0x1000];

void do_iram_dram_copy(void *buf, uintptr_t iram_addr, size_t size, int option) {
    memcpy(g_work_page, buf, size);
    
    SecmonArgs args = {0};
    args.X[0] = 0xF0000201;             
    args.X[1] = (uintptr_t)g_work_page;  
    args.X[2] = iram_addr;              
    args.X[3] = size;                   
    args.X[4] = option;                 
    svcCallSecureMonitor(&args);
    
    memcpy(buf, g_work_page, size);
}

void copy_to_iram(uintptr_t iram_addr, void *buf, size_t size) {
    do_iram_dram_copy(buf, iram_addr, size, 1);
}

void copy_from_iram(void *buf, uintptr_t iram_addr, size_t size) {
    do_iram_dram_copy(buf, iram_addr, size, 0);
}

static void clear_iram(void) {
    memset(g_ff_page, 0xFF, sizeof(g_ff_page));
    for (size_t i = 0; i < IRAM_PAYLOAD_MAX_SIZE; i += sizeof(g_ff_page)) {
        copy_to_iram(IRAM_PAYLOAD_BASE + i, g_ff_page, sizeof(g_ff_page));
    }
}

static void reboot_to_payload(void) {
    clear_iram();
    
    for (size_t i = 0; i < IRAM_PAYLOAD_MAX_SIZE; i += 0x1000) {
        copy_to_iram(IRAM_PAYLOAD_BASE + i, &g_reboot_payload[i], 0x1000);
    }
    
    splSetConfig((SplConfigItem)65001, 2);
}

void Read_Payload()
{
    Result rc = splInitialize();
    if (R_FAILED(rc)) 
    {
        printf("Failed to initialize spl: 0x%x\n\n\n", rc);
        can_reboot = false;
    } 
    else 
    {
        FILE *f = fopen("sdmc:/atmosphere/reboot_payload.bin", "rb");
        if (f == NULL) {
            printf("Failed to open /atmosphere/reboot_payload.bin!\n\n\n");
            can_reboot = false;
        } 
        else 
        {
            fread(g_reboot_payload, 1, sizeof(g_reboot_payload), f);
            fclose(f);
            sprintf(str_Key_Main, "Press (%s", ANSI_COLOR_YELLOW "Y" ANSI_COLOR_RESET);
            printf(str_Key_Main);
            printf(") Key To reboot to Payload\n\n\n");
        }
    }
}

void EmuMmcConfig_get()
{
    char str_emummc_enabled[100];
    long _emummc_enabled;
    _emummc_enabled = ini_gets("emummc", "emummc_enabled", "dummy", str_emummc_enabled, sizearray(str_emummc_enabled), inifile); 
    char str_emummc_sector[100];
    long _emummc_sector;
    _emummc_sector = ini_gets("emummc", "emummc_sector", "dummy", str_emummc_sector, sizearray(str_emummc_sector), inifile); 
    
    char str_toogle[40];
    char str_dev[40];
    char str_Key[40];
    char str_Key2[40];
    char str_Key3[40];

    printf( ANSI_COLOR_YELLOW "EMUMMC Config!\n\n\n" ANSI_COLOR_RESET);
        
        if(strcmp(str_emummc_enabled, "1") == 0) 
        {
            
            sprintf(str_toogle, "EMUMMC %s", ANSI_COLOR_GREEN "Enabled\n\n\n" ANSI_COLOR_RESET);
            printf(str_toogle);
            sprintf(str_Key, "Press (%s", ANSI_COLOR_CYAN "Minus" ANSI_COLOR_RESET);
            printf(str_Key);
            sprintf(str_Key2, ") Key To %s", ANSI_COLOR_RED "Disable" ANSI_COLOR_RESET);
            printf(str_Key2);
            printf(" EMUMMC\n\n\n");
        }
        else if(strcmp(str_emummc_enabled, "0") == 0) 
        {
            sprintf(str_toogle, "EMUMMC %s", ANSI_COLOR_RED "Disabled\n\n\n" ANSI_COLOR_RESET);
            printf(str_toogle);
            sprintf(str_Key, "Press (%s", ANSI_COLOR_CYAN "Minus" ANSI_COLOR_RESET);
            printf(str_Key);
            sprintf(str_Key2, ") Key To %s", ANSI_COLOR_GREEN "Enable" ANSI_COLOR_RESET);
            printf(str_Key2);
            printf(" EMUMMC\n\n\n");
        }
        else
        {
            printf(ANSI_COLOR_RED "emummc/emummc.ini is missing or wrong configured.\n\n\n" ANSI_COLOR_RESET);
        }

        if(strcmp(str_emummc_sector, "0x") == 0) 
        {
            //printf(str_emummc_sector);
        }
        sprintf(str_Key3, "Press (%s", ANSI_COLOR_MAGENTA "Plus" ANSI_COLOR_RESET);
        printf(str_Key3);
        printf(") Key To Exit\n\n\n");
        Read_Payload();
        sprintf(str_dev, "Developed by %s", ANSI_COLOR_GREEN "KranK/KuranKu\n\n\n" ANSI_COLOR_RESET);
        printf(str_dev);

        
}

void EmuMmcConfig_set()
{
    char str_emummc_enabled[100];
    long _emummc_enabled;
    _emummc_enabled = ini_gets("emummc", "emummc_enabled", "dummy", str_emummc_enabled, sizearray(str_emummc_enabled), inifile); 
    char LocalBuff[100];
    FILE *fp;
    fp = fopen("/emummc/emummc.ini","r+");
        
        if(strcmp(str_emummc_enabled, "1") == 0) 
        {
            if(fp != NULL)
            writesection(LocalBuff,"emummc", &fp);
            writekey(LocalBuff, "emummc_enabled ", " 0", &fp);
        }
        else if(strcmp(str_emummc_enabled, "0") == 0) 
        {
            if(fp != NULL)
            writesection(LocalBuff,"emummc", &fp);
            writekey(LocalBuff, "emummc_enabled ", " 1", &fp);
        }
        else
        {
            printf(ANSI_COLOR_RED "Oops Something went Wrong :(\n\n\n" ANSI_COLOR_RESET);
        }
    fclose(fp);
}

int main(int argc, char* argv[])
{
    consoleInit(NULL);
    EmuMmcConfig_get();
    
    
    while (appletMainLoop())
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break; 

        if (kDown & KEY_MINUS)
        {
            consoleInit(NULL);
            EmuMmcConfig_set();
            EmuMmcConfig_get();
        }
        if (can_reboot && kDown & KEY_Y) 
        {
            reboot_to_payload();
        }

        consoleUpdate(NULL);
    }

    if (can_reboot) 
    {
        splExit();
    }
    consoleExit(NULL);
    return 0;
}
