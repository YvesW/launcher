#include <Windows.h>
#include <shlobj.h>
#include <atlbase.h>
#include <jni.h>

extern "C" JNIEXPORT jboolean JNICALL Java_net_runelite_launcher_FilePermissionManager_isRunningElevated(JNIEnv *env, jclass clazz, jlong pid) {
	BOOL fRet = false;
	HANDLE hToken = NULL;
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
	// alternatively use GetCurrentProcess() instead if you'd only want to check the current process
	if (OpenProcessToken(handle, TOKEN_QUERY, &hToken)) {
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
			fRet = Elevation.TokenIsElevated;
		}
	}
	CloseHandle(handle);
	if (hToken) {
		CloseHandle(hToken);
	}
	bool result = fRet == 1 ? true : false;
	return result;
}

extern "C" JNIEXPORT void JNICALL Java_net_runelite_launcher_FilePermissionManager_elevate(JNIEnv *env, jclass clazz, jstring pathObj, jstring argsObj) {
	const char *pathString = env->GetStringUTFChars(pathObj, 0);
	const char *argsString = env->GetStringUTFChars(argsObj, 0);
	ShellExecuteA(NULL,		//HWND
			"runas",		//lpOperation: with elevated permissions
			pathString,		//lpFile
			argsString,		//lpParameters
			NULL,			//lpDirectory: default dir
			SW_SHOWNORMAL	//nShowCmd
		);
	env->ReleaseStringUTFChars(pathObj, pathString);
	env->ReleaseStringUTFChars(argsObj, argsString);
}

// https://devblogs.microsoft.com/oldnewthing/20130318-00/?p=4933
void FindDesktopFolderView(REFIID riid, void** ppv) {
	CComPtr<IShellWindows> spShellWindows;
	spShellWindows.CoCreateInstance(CLSID_ShellWindows);

	CComVariant vtLoc(CSIDL_DESKTOP);
	CComVariant vtEmpty;
	long lhwnd;
	CComPtr<IDispatch> spdisp;
	spShellWindows->FindWindowSW(
		&vtLoc, &vtEmpty,
		SWC_DESKTOP, &lhwnd,
		SWFO_NEEDDISPATCH, &spdisp);

	CComPtr<IShellBrowser> spBrowser;
	CComQIPtr<IServiceProvider>(spdisp)->
		QueryService(SID_STopLevelBrowser,
			IID_PPV_ARGS(&spBrowser));

	CComPtr<IShellView> spView;
	spBrowser->QueryActiveShellView(&spView);

	spView->QueryInterface(riid, ppv);
}

// https://devblogs.microsoft.com/oldnewthing/20131118-00/?p=2643
void GetDesktopAutomationObject(REFIID riid, void** ppv)
{
	CComPtr<IShellView> spsv;
	FindDesktopFolderView(IID_PPV_ARGS(&spsv));
	CComPtr<IDispatch> spdispView;
	spsv->GetItemObject(SVGIO_BACKGROUND, IID_PPV_ARGS(&spdispView));
	spdispView->QueryInterface(riid, ppv);
}

void ShellExecuteFromExplorer(LPCSTR pszFile,
	LPCSTR pszParameters = nullptr,
	LPCSTR pszDirectory = nullptr,
	LPCSTR pszOperation = nullptr,
	int nShowCmd = SW_SHOWNORMAL) {
	CComPtr<IShellFolderViewDual> spFolderView;
	GetDesktopAutomationObject(IID_PPV_ARGS(&spFolderView));
	CComPtr<IDispatch> spdispShell;
	spFolderView->get_Application(&spdispShell);
	CComQIPtr<IShellDispatch2>(spdispShell)
		->ShellExecute(CComBSTR(pszFile),
			CComVariant(pszParameters ? pszParameters : ""),
			CComVariant(pszDirectory ? pszDirectory : ""),
			CComVariant(pszOperation ? pszOperation : ""),
			CComVariant(nShowCmd));
}

// https://devblogs.microsoft.com/oldnewthing/20040520-00/?p=39243
class CCoInitialize {
public:
	CCoInitialize() : m_hr(CoInitialize(NULL)) { }
	~CCoInitialize() { if (SUCCEEDED(m_hr)) CoUninitialize(); }
	operator HRESULT() const { return m_hr; }
	HRESULT m_hr;
};

extern "C" JNIEXPORT void JNICALL Java_net_runelite_launcher_FilePermissionManager_unelevate(JNIEnv *env, jclass clazz, jstring pathObj, jstring argsObj) {
	const char *pathString = env->GetStringUTFChars(pathObj, 0);
	const char *argsString = env->GetStringUTFChars(argsObj, 0);

	CCoInitialize init;
	ShellExecuteFromExplorer(
		pathString,		// file
		argsString,		// parameters
		"",				// directory
		"",				// operation
		SW_SHOWNORMAL);	// nShowCmd
	env->ReleaseStringUTFChars(pathObj, pathString);
	env->ReleaseStringUTFChars(argsObj, argsString);
}