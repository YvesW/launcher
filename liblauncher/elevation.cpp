#include <Windows.h>
#include <jni.h>

extern "C" JNIEXPORT jint JNICALL Java_net_runelite_launcher_Launcher_isRunningElevated(JNIEnv *env, jclass clazz) {
    BOOL fRet = false;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
    	TOKEN_ELEVATION Elevation;
    	DWORD cbSize = sizeof(TOKEN_ELEVATION);
    	if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
    		fRet = Elevation.TokenIsElevated;
    	}
    }
    if (hToken) {
    	CloseHandle(hToken);
    }
    bool result = fRet == 1 ? true : false;
    return result;
}