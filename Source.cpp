#include <windows.h>

#ifdef _WIN64
#pragma comment (linker, "/INCLUDE:_tls_used")
#pragma comment (linker, "/INCLUDE:_tls_callback")
#else
#pragma comment (linker, "/INCLUDE:__tls_used")
#pragma comment (linker, "/INCLUDE:__tls_callback")
#endif

void NTAPI __stdcall TlsCallBack(PVOID DllHandle, DWORD dwReason, PVOID Reserved);

EXTERN_C
#ifdef _WIN64
#pragma const_seg (".CRT$XLB")
const PIMAGE_TLS_CALLBACK _tls_callback = TlsCallBack;
#pragma const_seg ()
#else
#pragma data_seg (".CRT$XLB")
PIMAGE_TLS_CALLBACK _tls_callback = TlsCallBack;
#pragma data_seg ()
#endif

void NTAPI __stdcall TlsCallBack(PVOID DllHandle, DWORD dwReason, PVOID Reserved)
{
	DWORD dwNumberOfBytesWritten;
	LPCSTR szText = "Unknown\n";
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH: // Windows 7の場合、User32.dllを静的ロードしないとDLL_PROCESS_ATTACHが呼ばれない？
		szText = "process attach\n";
		break;
	case DLL_PROCESS_DETACH:
		szText = "process detach\n";
		break;
	case DLL_THREAD_ATTACH:
		szText = "thread attach\n";
		break;
	case DLL_THREAD_DETACH:
		szText = "thread detach\n";
		break;
	}
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), szText, lstrlenA(szText), &dwNumberOfBytesWritten, 0);
}

EXTERN_C void
#ifdef _DEBUG
main()
#else
mainCRTStartup()
#endif
{
	//MessageBox(0, 0, 0, 0); // Windows 7の場合、User32.dllを静的ロードしないとDLL_PROCESS_ATTACHが呼ばれない？
	DWORD dwNumberOfBytesWritten;
	LPCSTR szText = __FUNCTION__ "\n";
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), szText, lstrlenA(szText), &dwNumberOfBytesWritten, 0);
	ExitProcess(0);
}
