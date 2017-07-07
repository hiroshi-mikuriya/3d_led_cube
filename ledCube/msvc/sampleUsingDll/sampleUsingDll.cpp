// sampleUsingDll.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "sampleUsingDll.h"
#include "ledLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	struct Led
	{
		SetUrl_t * SetUrl;
		SetLed_t * SetLed;
		Clear_t * ClearLed;
		Show_t * Show;
		Wait_t * Wait;
	};

	Led LoadMethods(LPCTSTR dll)
	{
		Led m;
		HMODULE mod = LoadLibrary(dll);
		if (!mod){
			throw std::runtime_error("failed to load dll.");
		}
		m.SetUrl = reinterpret_cast<SetUrl_t*>(GetProcAddress(mod, "SetUrl"));
		m.SetLed = reinterpret_cast<SetLed_t*>(GetProcAddress(mod, "SetLed"));
		m.ClearLed = reinterpret_cast<Clear_t*>(GetProcAddress(mod, "Clear"));
		m.Show = reinterpret_cast<Show_t*>(GetProcAddress(mod, "Show"));
		m.Wait = reinterpret_cast<Wait_t*>(GetProcAddress(mod, "Wait"));
		return m;
	}

	void proc(int argc, TCHAR* argv[], TCHAR* envp[])
	{
		Led led = LoadMethods(_T("ledLib.dll"));
		for (int x = 0; x < LED_WIDTH; ++x){
			for (int y = 0; y < LED_HEIGHT; ++y){
				for (int z = 0; z < LED_DEPTH; ++z){
					int c = (x % 2) ? 0xFF0000 : 0x00FF00;
					led.SetLed(x, y, z, c);
					led.Show();
					led.Wait(10);
				}
			}
		}
	}
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// MFC を初期化して、エラーの場合は結果を印刷します。
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 必要に応じてエラー コードを変更してください。
			_tprintf(_T("致命的なエラー: MFC の初期化ができませんでした。\n"));
			nRetCode = 1;
		}
		else
		{
			proc(argc, argv, envp);
		}
	}
	else
	{
		// TODO: 必要に応じてエラー コードを変更してください。
		_tprintf(_T("致命的なエラー: GetModuleHandle が失敗しました\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
