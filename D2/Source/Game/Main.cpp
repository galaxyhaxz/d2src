#include <windows.h>
#include <stddef.h>
#include <stdio.h>

#include "Main.h"

/*
 *	Create table for command line arguments
 *	48 entries total
 */
#define cmdidx(m)	offsetof(D2GameCfgStrc, m)
D2CmdArgStrc CmdArgv[] = {
	{ "VIDEO",     "WINDOW",       "w",          CMD_BOOLEAN, cmdidx(bWindow),        0 },
	{ "VIDEO",     "3DFX",         "3dfx",       CMD_BOOLEAN, cmdidx(b3DFX),          0 },
	{ "VIDEO",     "OPENGL",       "opengl",     CMD_BOOLEAN, cmdidx(bOpenGL),        0 },
	{ "VIDEO",     "D3D",          "d3d",        CMD_BOOLEAN, cmdidx(bRave),          0 },
	{ "VIDEO",     "RAVE",         "rave",       CMD_BOOLEAN, cmdidx(bD3D),           0 },
	{ "VIDEO",     "PERSPECTIVE",  "per",        CMD_BOOLEAN, cmdidx(bPerspective),   0 },
	{ "VIDEO",     "QUALITY",      "lq",         CMD_BOOLEAN, cmdidx(bQuality),       0 },
	{ "VIDEO",     "GAMMA",        "gamma",      CMD_INTEGER, cmdidx(dwGamma),        0 },
	{ "VIDEO",     "VSYNC",        "vsync",      CMD_BOOLEAN, cmdidx(bVSync),         0 },
	{ "VIDEO",     "FRAMERATE",    "fr",         CMD_INTEGER, cmdidx(dwFramerate),    0 },
	{ "NETWORK",   "SERVERIP",     "s",          CMD_STRING,  cmdidx(szServerIP),     0 },
	{ "NETWORK",   "GAMETYPE",     "gametype",   CMD_INTEGER, cmdidx(dwGameType),     0 },
	{ "NETWORK",   "ARENA",        "arena",      CMD_INTEGER, cmdidx(dwArena),        0 },
	{ "NETWORK",   "JOINID",       "joinid",     CMD_INTEGER, cmdidx(wJoinID),        0 },
	{ "NETWORK",   "GAMENAME",     "gamename",   CMD_STRING,  cmdidx(szGameName),     0 },
	{ "NETWORK",   "BATTLENETIP",  "bn",         CMD_STRING,  cmdidx(szBattleNetIP),  0 },
	{ "NETWORK",   "MCPIP",        "mcpip",      CMD_STRING,  cmdidx(szMCPIP),        0 },
	{ "CHARACTER", "AMAZON",       "ama",        CMD_BOOLEAN, cmdidx(bAmazon),        1 },
	{ "CHARACTER", "PALADIN",      "pal",        CMD_BOOLEAN, cmdidx(bPaladin),       0 },
	{ "CHARACTER", "SORCERESS",    "sor",        CMD_BOOLEAN, cmdidx(bSorceress),     0 },
	{ "CHARACTER", "NECROMANCER",  "nec",        CMD_BOOLEAN, cmdidx(bNecromancer),   0 },
	{ "CHARACTER", "BARBARIAN",    "bar",        CMD_BOOLEAN, cmdidx(bBarbarian),     0 },
	{ "CHARACTER", "INVINCIBLE",   "i",          CMD_BOOLEAN, cmdidx(bInvincible),    0 },
	{ "CHARACTER", "NAME",         "name",       CMD_STRING,  cmdidx(szName),         0 },
	{ "CHARACTER", "CTEMP",        "ctemp",      CMD_INTEGER, cmdidx(dwCTemp),        0 },
	{ "MONSTER",   "NOMONSTERS",   "nm",         CMD_BOOLEAN, cmdidx(bNoMonsters),    0 },
	{ "MONSTER",   "MONSTERCLASS", "m",          CMD_INTEGER, cmdidx(dwMonsterClass), 0 },
	{ "MONSTER",   "MONSTERINFO",  "minfo",      CMD_BOOLEAN, cmdidx(bMonsterInfo),   0 },
	{ "MONSTER",   "MONSTERDEBUG", "md",         CMD_INTEGER, cmdidx(dwMonsterDebug), 0 },
	{ "ITEM",      "RARE",         "rare",       CMD_BOOLEAN, cmdidx(bRare),          0 },
	{ "ITEM",      "UNIQUE",       "unique",     CMD_BOOLEAN, cmdidx(bUnique),        0 },
	{ "INTERFACE", "ACT",          "act",        CMD_INTEGER, cmdidx(dwAct),          1 },
	{ "DEBUG",     "LOG",          "log",        CMD_BOOLEAN, cmdidx(bLog),           0 },
	{ "DEBUG",     "MSGLOG",       "msglog",     CMD_BOOLEAN, cmdidx(bMsgLog),        0 },
	{ "DEBUG",     "SAFEMODE",     "safe",       CMD_BOOLEAN, cmdidx(bSafeMode),      0 },
	{ "DEBUG",     "NOSAVE",       "nosave",     CMD_BOOLEAN, cmdidx(bNoSave),        0 },
	{ "DEBUG",     "SEED",         "seed",       CMD_INTEGER, cmdidx(dwSeed),         0 },
	{ "NETWORK",   "NOPK",         "nopk",       CMD_BOOLEAN, cmdidx(bNoPK),          0 },
	{ "DEBUG",     "CHEATS",       "cheats",     CMD_BOOLEAN, cmdidx(bCheats),        0 },
	{ "DEBUG",     "NOSOUND",      "ns",         CMD_BOOLEAN, cmdidx(bNoSound),       0 },
	{ "FILEIO",    "NOPREDLOAD",   "npl",        CMD_BOOLEAN, cmdidx(bNoPreload),     0 },
	{ "FILEIO",    "DIRECT",       "direct",     CMD_BOOLEAN, cmdidx(bDirect),        0 },
	{ "FILEIO",    "LOWEND",       "lem",        CMD_BOOLEAN, cmdidx(bLowEnd),        0 },
	{ "DEBUG",     "QuEsTs",       "questall",   CMD_BOOLEAN, cmdidx(bQuests),        0 },
	{ "NETWORK",   "COMINT",       "comint",     CMD_INTEGER, cmdidx(dwComInt),       0 },
	{ "NETWORK",   "SKIPTOBNET",   "skiptobnet", CMD_BOOLEAN, cmdidx(bSkipToBNet),    0 },
	{ "NETWORK",   "OPENC",        "openc",      CMD_BOOLEAN, cmdidx(bOpenC),         0 },
	{ "FILEIO",    "NOCOMPRESS",   "nocompress", CMD_BOOLEAN, cmdidx(bNoCompress),    0 }
};
#undef cmdidx

/*
 *	Module names loaded by main game
 */
char *lpszD2Module[] = {
	"none.dll",
	"D2Client.dll",
	"D2Server.dll",
	"D2Multi.dll",
	"D2Launch.dll",
	"D2MultiLAN.dll",
	"D2EClient.dll"
};

/*
 *	Internal name type for each module
 */
char *lpszModuleType[] = {
	"modstate0",
	"client",
	"server",
	"multiplayer",
	"launcher",
	"multilan",
	"expand"
};

/*
 *	Structure for service status and handler
 */
SERVICE_STATUS ssStatus = {
	SERVICE_WIN32_OWN_PROCESS,
	SERVICE_RUNNING,
	SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN,
	0,
	0,
	0,
	0
};

SERVICE_STATUS_HANDLE sshStatusHandle = NULL;
HINSTANCE hProcess = NULL;
BOOL bServerStopEvent = FALSE;
int nCmdReserved = 0;
char szSRegReadBuf[MAX_PATH];
char lpZero = '\0';
int nCurrentMod = 0;
int nProcFunc = 0;
HMODULE hModKeyhook = NULL;
BOOL bUseKeyhook = FALSE;

int GAMEAPI ParseCmdLine(D2GameCfgStrc* pCfg, const char *argv)
{
	D2CmdArgStrc* pArg;
	char *lpszArgv;
	char *lpArgPos;
	char *lpCfgPos;
	char szArg[24];
	char szBuf[48];
	char szPath[_MAX_DIR];
	BOOL bSearchIndex;
	int nResult;
	int nIndex;
	int nLen;
	int nArgLen;
	int i;
	int j;

	memset(pCfg, 0, sizeof(*pCfg) - 2);
	j = 0;

	if(GetCurrentDirectoryA(sizeof(szPath), szPath))
	{
		nLen = strlen(szPath);

		for(i = 0; i < nLen; i++)
		{
			if(i >= sizeof(szPath))
				break;
			else if(szPath[i] == '\\')
				j++;
		}

		if(j > 2)
			j = 2;

		for(i = 0; i < nLen; i++)
		{
			if(!j)
				break;
			else if(szPath[i] == '\\')
				j--;
		}

		szPath[i] = '\0';
		strcat(szPath, GAME_FILE_INI);
		pArg = CmdArgv;

		do
		{
			lpCfgPos = (char *)((int)pCfg + pArg->dwIndex);

			switch(pArg->dwType)
			{
				case CMD_INTEGER:
					*(int *)lpCfgPos = GetPrivateProfileIntA(pArg->szSection, pArg->szKey, pArg->dwDefault, szPath);
					break;
				case CMD_STRING:
					GetPrivateProfileStringA(pArg->szSection, pArg->szKey, &lpZero, lpCfgPos, 16, szPath);
					break;
				case CMD_BOOLEAN:
					*lpCfgPos = GetPrivateProfileIntA(pArg->szSection, pArg->szKey, pArg->dwDefault, szPath) != 0;
					break;
			}

			pArg++;
		} while((int)pArg < (int)&CmdArgv[NUM_CMD_ARGV].dwDefault);
	}

	nArgLen = strlen(argv);
	nLen = nArgLen;
	nResult = 0;

	if(nArgLen > 0)
	{
		lpszArgv = argv;
		lpArgPos = (char *)(szBuf - argv);

		do
		{
			if(*lpszArgv == '-')
			{
				lpArgPos--;
				lpszArgv++;
				j = nResult + 1;

				if(j < nLen)
				{
					i = 0;
					nIndex = -1;
					nLen = strlen(lpszArgv);

					if(nLen > 0)
					{
						while(i++ < nLen)
						{
							if(!*lpszArgv || *lpszArgv == '-')
								break;

							lpszArgv[(int)lpArgPos] = *lpszArgv++;
						}
					}

					szBuf[i] = '\0';
					szBuf[i + 1] = '\0';
					strcpy(szArg, szBuf);
					nLen = strlen(szArg);
					bSearchIndex = TRUE;
					stoLower(szArg);

					if(nLen)
					{
						while(bSearchIndex && nLen)
						{
							szArg[nLen--] = '\0';
							nIndex = GetCmdIndex(szArg);

							if(nIndex != -1)
								bSearchIndex = FALSE;
						}
					}

					i = 0;
					nLen = strlen(szArg);

					if(szBuf)
					{
						do
						{
							szArg[i++] = szBuf[i + nLen];
						} while(szBuf[i]);
					}

					szArg[i] = '\0';
					GetCmdData(szArg);

					if(nIndex != -1)
					{
						lpCfgPos = (char *)((int)pCfg + CmdArgv[nIndex].dwIndex);

						switch((char)CmdArgv[nIndex].dwType)
						{
							case CMD_INTEGER:
								*(int *)lpCfgPos = atoi(szArg);
								break;
							case CMD_STRING:
								strcpy(lpCfgPos, szArg);
								break;
							case CMD_BOOLEAN:
								*lpCfgPos = TRUE;
								break;
						}
					}
				}
			}

			nLen = nArgLen;
			nResult = j++;
			lpArgPos--;
			lpszArgv++;
		} while(j < nLen);
	}

	return 0;
}

void GAMEAPI stoLower(char *s)
{
	if(*s)
	{
		do
		{
			if(*s >= 'A' && *s <= 'Z')
				*s += 32;
		} while((s++)[1]);
	}
}

int GAMEAPI GetCmdIndex(const char *s)
{
	int nIndex;
	const char *pCmd;

	nIndex = 0;
	pCmd = CmdArgv->szCommand;

	while(strcmp(pCmd, s))
	{
		pCmd += sizeof(D2CmdArgStrc);
		nIndex++;

		if((int)pCmd >= (int)CmdArgv[NUM_CMD_ARGV].szCommand)
			return -1;
	}

	return nIndex;
}

int GAMEAPI GetCmdData(char *s)
{
	int nResult;
	int nLen;
	int nPos;
	int i;
	char bDelChar[4];
	char szBuf[24];

	memcpy(szBuf, s, strlen(s) + 1);
	nResult = 0;
	nLen = strlen(s);

	if(nLen > 0)
	{
		bDelChar[0] = ' ';
		bDelChar[1] = '\t';
		bDelChar[2] = '\n';
		bDelChar[3] = ':';

		while(nResult < nLen)
		{
			i = 0;

			while(bDelChar[i] != s[nResult])
			{
				if(i++ >= 4)
					goto DOCOPY;
			}

			nResult++;
		}
	}
DOCOPY:
	nPos = 0;

	if(nResult < nLen)
	{
		bDelChar[0] = ' ';
		bDelChar[1] = '\t';
		bDelChar[2] = '\n';
		bDelChar[3] = ':';
RETRY:
		i = 0;

		while(bDelChar[i] != szBuf[nResult])
		{
			if(i++ >= 4)
			{
				s[nPos++] = szBuf[nResult++];

				if(nResult < nLen)
					goto RETRY;

				break;
			}
		}
	}

	return 0;
}

int GAMEAPI GameStart(HINSTANCE hInstance, D2GameCfgStrc* pCfg, int nModType)
{
	DWORD dwRenderMode;
	FARPROC pFunc;
	const char *lpModName;
	HMODULE hModule;
	BOOL bSoundStarted;
	BOOL bProcessStarted;
	char szErrMsg[100];

	bProcessStarted = FALSE;
	bSoundStarted = FALSE;
	nCurrentMod = nModType;

	FogSetWorkingDirectory(pCfg->bDirect, 0);
	FogInitAsyncData(1, 0);
	Fog__Reserved();

	nModType = nCurrentMod;

	if(nCurrentMod != MODULE_SERVER)
	{
		if(!D2WinLoadArchives() || !D2WinLoadOtherArchives(D2WinPromptPlayDisc))
		{
			nModType = nCurrentMod;
			D2WinUnloadArchives();
			return 0;
		}
	}

	nModType = nCurrentMod;

	if(pCfg->b3DFX)
		dwRenderMode = DISPLAYTYPE_GLIDE;
	else if(pCfg->bWindow)
		dwRenderMode = DISPLAYTYPE_GDI;
	else if(pCfg->bOpenGL)
		dwRenderMode = DISPLAYTYPE_OPENGL;
	else
		dwRenderMode = pCfg->bD3D ? DISPLAYTYPE_DIRECT3D : DISPLAYTYPE_RAVE;

	if(nModType != MODULE_SERVER)
	{
		if(!D2WinCreateWindow(hInstance, dwRenderMode, pCfg->bWindow, !pCfg->bNoCompress))
			return 0;

		if(pCfg->bPerspective && dwRenderMode >= DISPLAYTYPE_GLIDE)
			D2GfxSetPerspectiveMode(TRUE);

		if(!D2WinInitSpriteCache(pCfg->bWindow, 0)
		{
			D2GfxDestroyWindow();
			return 0;
		}

		if(bUseKeyhook)
		{
			hModule = LoadLibraryA(HMOD_KEYHOOK);
			hModKeyhook = hModule;
		}
		else
			hModule = hModKeyhook;

		if(hModule)
		{
			pFunc = GetProcAddress(hModule, PROC_KEYHOOK);

			if(pFunc)
				((void (KEYHOOKAPI *)(int))pFunc)(D2GfxGetHWnd());
		}

		nModType = nCurrentMod;
		bProcessStarted = TRUE;
	}

	if(pCfg->bQuality)
	{
		D2GfxEnableLowQuality();
		nModType = nCurrentMod;
	}

	if(pCfg->dwGamma)
	{
		D2GfxSetGamma(pCfg->dwGamma);
		nModType = nCurrentMod;
	}

	if(pCfg->bVSync)
	{
		D2GfxEnableVSync();
		nModType = nCurrentMod;
	}

	if(!pCfg->bNoSound && nModType != MODULE_SERVER)
	{
		D2SoundInitSystem();
		nModType = nCurrentMod;
		bSoundStarted = TRUE;
	}

	while(nCurrentMod = nModType)
	{
		if(nModType == MODULE_SERVER)
		{
			if(bSoundStarted)
			{
				D2SoundCloseSystem();
				nModType = nCurrentMod;
				bSoundStarted = FALSE;
			}

			if(bProcessStarted)
			{
				D2WinCloseSpriteCache();
				D2GfxDestroyWindow();
				nModType = nCurrentMod;
				bProcessStarted = FALSE;
			}
		}

		if(nModType >= MODULE_NONE && nModType <= MODULE_EXPAND)
		{
			lpModName = lpszD2Module[nModType];
			hModule = LoadLibraryA(lpszD2Module[nModType]);

			if(!hModule)
			{
				sprintf(szErrMsg, ERRMSG_LOADMOD, lpModName, GetLastError());
				MessageBoxA(NULL, szErrMsg, ERRMSG_TITLE, MB_ICONERROR);
			}
			else
			{
				pFunc = GetProcAddress(hModule, PROC_QUERYINT);

				if(pFunc)
				{
					nProcFunc = pFunc;
					nModType = (*(int (QUERYINTAPI **)(int))nProcFunc)((int)pCfg);
					continue;
				}

				GetLastError();
			}
		}

		nModType = MODULE_NONE;
	}

	if(bSoundStarted)
		D2SoundCloseSystem();

	if(bProcessStarted)
	{
		D2WinCloseSpriteCache();
		D2GfxDestroyWindow();
	}

	D2WinUnloadArchives();

	if(hModKeyhook)
	{
		pFunc = GetProcAddress(hModKeyhook, PROC_UNKEYHOOK);

		if(pFunc)
			pFunc();

		FreeLibrary(hModKeyhook);
	}

	FogCloseAsyncData();
	D2MCPClientCloseMCP();

	if(pCfg->dwComInt)
		(*(void (**)(void))(pCfg->dwComInt + 12))();

	FogCloseSystem(NULL);

	return 0;
}

int GAMEAPI SaveCmdLine(const char **argv)
{
	BOOL bResult;
	BOOL bUseCmdLine;
	char szSRegWriteBuf[MAX_REG_KEY * 2];

	bUseCmdLine = FALSE;

	if(*argv && strlen(*argv))
	{
		sprintf(szSRegWriteBuf, CMDLINE_ADD_BNET, *argv);
		SRegSaveString(REG_KEY_HOME, CMDLINE_SZ, SREG_NONE, szSRegWriteBuf);
	}
	else
	{
		SRegLoadValue(REG_KEY_HOME, CMDLINE_USE, SREG_NONE, &bUseCmdLine);

		if(bUseCmdLine)
		{
			SRegLoadString(REG_KEY_HOME, CMDLINE_SZ, SREG_NONE, szSRegReadBuf, MAX_REG_KEY);
			*argv = szSRegReadBuf;
		}
		else
		{
			strcpy(szSRegWriteBuf, CMDLINE_BNET);
			SRegSaveString(REG_KEY_HOME, CMDLINE_SZ, SREG_NONE, szSRegWriteBuf);
		}
	}

	bUseCmdLine = FALSE;
	SRegSaveValue(REG_KEY_HOME, CMDLINE_USE, SREG_NONE, NULL);
	bResult = bServerStopEvent;

	if(bServerStopEvent)
	{
		bResult = SRegLoadString(REG_KEY_HOME, CMDLINE_SVC, SREG_NONE, szSRegReadBuf, MAX_REG_KEY);
		*argv = szSRegReadBuf;
	}

	return bResult;
}

int GAMEAPI GameInit(int argc, char *argv[])
{
	HANDLE hEvent;
	int nStatus;
	char *lpArgvDupe;
	char *lpArgvTokens;
	char **lpszModType;
	int i;
	DWORD dwRegType;
	DWORD cbData;
	HKEY phkResult;
	char *lpArgvCmd;
	HKEY hKey;
	DWORD cchValue;
	char szRegPathVid[sizeof(REG_PATH_VIDEO)];
	char szPath[MAX_PATH];
	D2GameCfgStrc pCfg;
	char szValue[MAX_PATH];
	BYTE bData[MAX_REG_KEY];

	lpArgvCmd = &lpZero;

	if(argc > 1)
		lpArgvCmd = *(char **)((char *)argv + 4 * argc - 4); // argv[1]

	FogInitSystem(SYS_NAME, NULL);
	hEvent = OpenEventA(EVENT_MODIFY_STATE, TRUE, GAME_OK);

	if(hEvent)
		SetEvent(hEvent);

	SaveCmdLine(&lpArgvCmd);

	if(!RegOpenKeyA(HKEY_LOCAL_MACHINE, REG_PATH_BETA, &phkResult))
	{
		RegCreateKeyA(HKEY_LOCAL_MACHINE, REG_PATH_HOME, &hKey);

		i = 0;
		cchValue = MAX_PATH;
		cbData = MAX_REG_KEY;

		do
		{
			nStatus = RegEnumValueA(phkResult, i++, szValue, &cchValue, NULL, &dwRegType, bData, &cbData);
		} while(!nStatus && !RegSetValueExA(hKey, szValue, 0, dwRegType, bData, cbData));

		RegCloseKey(hKey);
		RegCloseKey(phkResult);
		RegDeleteKeyA(HKEY_LOCAL_MACHINE, REG_PATH_BETA);
	}

	*szPath = lpZero;
	memset(szPath + 1, 0, MAX_PATH - 1);

	FogGetInstallPath((int)szPath, MAX_PATH);

	if(bServerStopEvent)
		SetCurrentDirectoryA(szPath);

	phkResult = (HKEY)MODULE_LAUNCHER; // Unsure why HKEY was used here
	lpArgvDupe = _strdup(lpArgvCmd);
	dwRegType = (DWORD)lpArgvDupe;
	lpArgvTokens = strtok(lpArgvDupe, CMDLINE_TOKEN);

	if(lpArgvTokens)
	{
		while(lpArgvTokens)
		{
			i = 0;
			lpszModType = (const char **)lpszModuleType;

			while((int)lpszModType < (int)&lpszModuleType[NUM_GAME_MOD])
			{
				if(!strncmp(lpArgvTokens, *lpszModType, strlen(*lpszModType)))
					phkResult = (HKEY)i;

				lpszModType++;
				i++;
			}

			lpArgvTokens = strtok(NULL, CMDLINE_TOKEN);
		}

		lpArgvDupe = (char *)dwRegType;
	}

	free(lpArgvDupe);
	ParseCmdLine(&pCfg, lpArgvCmd);

	if(!pCfg.b3DFX && !pCfg.bWindow && !pCfg.bOpenGL && !pCfg.bD3D)
	{
		cchValue = 4;
		hKey = (HKEY)4;
		memcpy(szRegPathVid, REG_PATH_VIDEO, sizeof(REG_PATH_VIDEO));

		if((hKey = (HKEY)!RegOpenKeyExA(HKEY_CURRENT_USER, szRegPathVid, 0, KEY_QUERY_VALUE, (PHKEY)&cbData)) ||
		   (hKey = (HKEY)!RegOpenKeyExA(HKEY_LOCAL_MACHINE, szRegPathVid, 0, KEY_QUERY_VALUE, (PHKEY)&cbData)) )
		{
			if(!RegQueryValueExA((HKEY)cbData, REG_VAL_RENDER, NULL, (LPDWORD)&hKey, (LPBYTE)&dwRegType, &cchValue))
			{
				switch(dwRegType)
				{
					case 1:
						pCfg.bD3D = TRUE;
						break;
					case 2:
						pCfg.bOpenGL = TRUE;
						break;
					case 3:
						pCfg.b3DFX = TRUE;
						break;
					case 4:
						pCfg.bWindow = TRUE;
						break;
					default: // Rave
						break;
				}

				RegCloseKey((HKEY)cbData);
			}
		}
	}

	return GameStart(hProcess, &pCfg, (int)phkResult);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, INT nCmdShow)
{
	SC_HANDLE schService;
	SC_HANDLE schSCManager;
	char *argv[2];
	char szPath[MAX_PATH];

	hProcess = hInstance;
	nCmdReserved = nCmdShow;

	if(!(GetVersion() & 0x80000000))
	{
		schSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);

		if(schSCManager)
		{
			schService = OpenServiceA(schSCManager, SVC_NAME, SERVICE_ALL_ACCESS);

			if(schService)
				CloseServiceHandle(schService);

			CloseServiceHandle(schSCManager);

			if(schService)
			{
				SERVICE_TABLE_ENTRYA DispatchTable[] = {
					{ SVC_NAME, (LPSERVICE_MAIN_FUNCTIONA)service_main },
					{ NULL, NULL }
				};

				if(!StartServiceCtrlDispatcherA(DispatchTable))
				{
					argv[0] = INIT_NAME;
					argv[1] = lpCmdLine;

					if(strstr(lpCmdLine, CMDLINE_INSTALL))
					{
						if(GetVersion() & 0x80000000 || !(schSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS)))
						{
							MessageBoxA(NULL, ERRMSG_INSTALLSVC, SVC_DISPLAYNAME, MB_ICONEXCLAMATION | MB_SETFOREGROUND);
							return 1;
						}
						else
						{
							schService = OpenServiceA(schSCManager, SVC_NAME, SERVICE_ALL_ACCESS);

							if(schService || (*szPath = 0,
											  GetModuleFileNameA(GetModuleHandleA(NULL), szPath, sizeof(szPath)),
											  schService = CreateServiceA(
												schSCManager,
												SVC_NAME,
												SVC_DISPLAYNAME,
												SERVICE_ALL_ACCESS,
												SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
												SERVICE_AUTO_START,
												SERVICE_ERROR_NORMAL,
												szPath,
												NULL,
												NULL,
												NULL,
												NULL,
												NULL) ) )
								CloseServiceHandle(schService);

							CloseServiceHandle(schSCManager);
						}
					}
					else
						GameInit(sizeof(argv) / sizeof(char **), argv);
				}
			}
		}
	}

	return 0;
}

BOOL WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv)
{
	BOOL bStatus;

	bServerStopEvent = TRUE;
	sshStatusHandle = RegisterServiceCtrlHandlerA(SVC_NAME, service_ctrl);
	SetServiceStatus(sshStatusHandle, &ssStatus);
	GameInit(dwArgc, lpszArgv);
	ssStatus.dwCurrentState = SERVICE_STOPPED;
	bStatus = SetServiceStatus(sshStatusHandle, &ssStatus);
	bServerStopEvent = FALSE;

	return bStatus;
}

VOID WINAPI service_ctrl(DWORD dwCtrlCode)
{
	switch(dwCtrlCode)
	{
		case SERVICE_CONTROL_STOP:
		case SERVICE_CONTROL_SHUTDOWN:
			ssStatus.dwCurrentState = SERVICE_STOP_PENDING;
			SetServiceStatus(sshStatusHandle, &ssStatus);
			bServerStopEvent = TRUE;
			return;

		case SERVICE_CONTROL_INTERROGATE:
			SetServiceStatus(sshStatusHandle, &ssStatus);
			return;

		default:
			break;
	}
}
