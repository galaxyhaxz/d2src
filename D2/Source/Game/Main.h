#pragma once
#include "../Common/D2Gfx.h"
#include "../Common/D2MCPClient.h"
#include "../Common/D2Sound.h"
#include "../Common/D2Win.h"
#include "../Common/Fog.h"
#include "../Common/Storm.h"

/*
 *	Project definitions
 */
#define GAMEAPI				__fastcall
#define KEYHOOKAPI			__stdcall
#define QUERYINTAPI			__fastcall

#define MAX_REG_KEY			1024
#define NUM_CMD_ARGV		48
#define NUM_GAME_MOD		7

/*
 *	String definitions
 */
#define GAME_FILE_INI		"D2.ini"
#define GAME_OK				"DIABLO_II_OK"

#define SYS_NAME			"Diablo II Server"

#define SVC_NAME			"DIABLO2SRV"
#define SVC_DISPLAYNAME		"Diablo II Server"

#define INIT_NAME			"d2server"

#define REG_KEY_HOME		"Diablo II"
#define REG_VAL_RENDER		"Render"
#define REG_PATH_HOME		"SOFTWARE\\Blizzard Entertainment\\Diablo II"
#define REG_PATH_BETA		"SOFTWARE\\Blizzard Entertainment\\Diablo II Beta"
#define REG_PATH_VIDEO		"SOFTWARE\\Blizzard Entertainment\\Diablo II\\VideoConfig"

#define HMOD_KEYHOOK		"Keyhook.dll"
#define PROC_QUERYINT		"QueryInterface"
#define PROC_KEYHOOK		"InstallKeyboardHook"
#define PROC_UNKEYHOOK		"UninstallKeyboardHook"

#define ERRMSG_TITLE		"Diablo 2"
#define ERRMSG_LOADMOD		"Cannot load %s: Error %d"
#define ERRMSG_INSTALLSVC	"Unable to install or run service.  Verify that you have administrator access and that the program is located on a local drive."

#define CMDLINE_TOKEN		"-"
#define CMDLINE_SVC			"SvcCmdLine"
#define CMDLINE_USE			"UseCmdLine"
#define CMDLINE_BNET		"-skiptobnet"
#define CMDLINE_ADD_BNET	"%s -skiptobnet"
#define CMDLINE_INSTALL		"-install"
#define CMDLINE_SZ			"CmdLine"

/*
 *	Define module types
 */
enum
{
	MODULE_NONE,
	MODULE_CLIENT,
	MODULE_SERVER,
	MODULE_MULTIPLAYER,
	MODULE_LAUNCHER,
	MODULE_MULTILAN,
	MODULE_EXPAND
};

/*
 *	Define graphic render modes
 */
enum
{
	DISPLAYTYPE_NONE,
	DISPLAYTYPE_GDI,
	DISPLAYTYPE_UNUSED,
	DISPLAYTYPE_RAVE,
	DISPLAYTYPE_GLIDE,
	DISPLAYTYPE_OPENGL,
	DISPLAYTYPE_DIRECT3D
};

/*
 *	Define command line types
 */
enum
{
	CMD_BOOLEAN,
	CMD_INTEGER,
	CMD_STRING
};

/*
 *	Create structure for game configuration parameters
 *	888 bytes in size
 */
#pragma pack(push, 1)
struct D2GameCfgStrc
{
	BYTE bWindow;
	BYTE b3DFX;
	BYTE bOpenGL;
	BYTE bRave;
	BYTE bD3D;
	BYTE bPerspective;
	BYTE bQuality;
	DWORD dwGamma;
	BYTE bVSync;
	DWORD dwFramerate;
	DWORD dwGameType;
	WORD wJoinID;
	char szGameName[24];
	char szServerIP[24];
	char szBattleNetIP[24];
	char szMCPIP[24];
	BYTE _0076[4];
	BYTE bNoPK;
	BYTE bOpenC;
	BYTE bAmazon;
	BYTE bPaladin;
	BYTE bSorceress;
	BYTE bNecromancer;
	BYTE bBarbarian;
	BYTE bInvincible;
	BYTE _0082[48];
	char szName[24];
	BYTE _00CA[256];
	DWORD dwCTemp;
	BYTE bNoMonsters;
	DWORD dwMonsterClass;
	BYTE bMonsterInfo;
	DWORD dwMonsterDebug;
	BYTE bRare;
	BYTE bUnique;
	BYTE _01DA[2]; // Possibly Set/Magic
	DWORD dwAct;
	BYTE bNoPreload;
	BYTE bDirect;
	BYTE bLowEnd;
	BYTE bNoCompress;
	DWORD dwArena;
	BYTE _01E8[6]; // Related to Arena
	BYTE bLog;
	BYTE bMsgLog;
	BYTE bSafeMode;
	BYTE bNoSave;
	DWORD dwSeed;
	BYTE bCheats;
	BYTE bNoSound;
	BYTE bQuests;
	BYTE _01F9;
	DWORD dwComInt;
	BYTE _01FE[308]; // Related to ComInt
	BYTE bSkipToBNet;
	BYTE _0333[69];
};
#pragma pack(pop)

/*
 *	Create structure for command line arguments
 *	60 bytes in size
 */
struct D2CmdArgStrc
{
	char szSection[16];
	char szKey[16];
	char szCommand[16];
	DWORD dwType;
	DWORD dwIndex;
	DWORD dwDefault;
};

/*
 *	Prototypes
 */
int GAMEAPI ParseCmdLine(D2GameCfgStrc* pCfg, const char *argv);
void GAMEAPI stoLower(char *s);
int GAMEAPI GetCmdIndex(const char *s);
int GAMEAPI GetCmdData(char *s);
int GAMEAPI GameStart(HINSTANCE hInstance, D2GameCfgStrc* pCfg, int nModType);
int GAMEAPI SaveCmdLine(const char **argv);
int GAMEAPI GameInit(int argc, char *argv[]);
BOOL WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv);
VOID WINAPI service_ctrl(DWORD dwCtrlCode);
