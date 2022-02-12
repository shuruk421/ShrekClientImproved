#include "functions.h"
#include <string>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>
#include <msclr\marshal_cppstd.h>
/*
BOOL WINAPI DllMain(
_In_ HINSTANCE hinstDLL,
_In_ DWORD     fdwReason,
_In_ LPVOID    lpvReserved
);
*/

typedef jint(*hJNI_GetCreatedJavaVMs)(JavaVM** vmBuf, jsize bufLen, jsize* nVMs);

hJNI_GetCreatedJavaVMs oJNI_GetCreatedJavaVMs;

HMODULE jvmHandle;
FARPROC func_JNI_GetCreatedJavaVMs;
JavaVM *jvm;
JNIEnv *jenv;

using namespace std;
using namespace System;
int minClick = 11;
int maxClick = 14;
double minReach = 3;
double maxReach = 4;
bool selfDestruct = false;
bool reachT = false;
bool clickerT = false;
bool inventoryClickT = false;
LPCTSTR WindowName = "Minecraft 1.7.10";
POINT pt;
HMODULE MODULE;
jobject getMouseOver(float p_78473_1_, JNIEnv* env);
extern int UIMain();
DWORD WINAPI Main_thread(LPVOID lpParam) {
	UIMain();
	return 1;
}

DWORD WINAPI LoopFunction(LPVOID lpParam)
{
	bool keep = false;
	jvmHandle = GetModuleHandleA("jvm.dll");
	func_JNI_GetCreatedJavaVMs = GetProcAddress(jvmHandle, "JNI_GetCreatedJavaVMs");
	oJNI_GetCreatedJavaVMs = (hJNI_GetCreatedJavaVMs)func_JNI_GetCreatedJavaVMs;
	jint returnOF = oJNI_GetCreatedJavaVMs(&jvm, 1, NULL);
	jint returnOf1 = jvm->AttachCurrentThread((void **)&jenv, NULL);
	if (jenv != nullptr)
	{
		postPreInit(jenv);
		while (1)
		{	
				HWND hwnd = FindWindowA(NULL, WindowName);
				if (selfDestruct || (GetKeyState(VK_F12) & 0x8000)) {
					FreeLibraryAndExitThread(MODULE, 0);
				}
				HWND currentWindow = GetForegroundWindow();
				while (GetAsyncKeyState(VK_UP)) {
					thePlayer = getPlayer(jenv);
					theWorld = getWorld(jenv);
					renderViewEntity = jenv->GetObjectField(mc, renderViewEntityf);
				}
				bool attacked2 = false;
				while (GetAsyncKeyState(VK_LBUTTON) && !attacked2) {
					if (currentWindow == hwnd) {
						
						cout << (rand() % (maxClick - minClick) + minClick) << endl;
						int delay = 1000/((rand() % ((maxClick - minClick)*10)/10 + minClick)*2);
						GetCursorPos(&pt);
						bool attacked = false;
						if (currentScreen == NULL && allowUserInput)
							if (theWorld != NULL && reachT) {
								if (thePlayer != NULL)
									if (jenv->CallBooleanMethod(thePlayer, isUsingItemf) == false) {
										if (!keep)
											if (jenv->GetObjectField(jenv->GetObjectField(mc, objectMouseOverf), entityHitf) != NULL)
												keep = true;
										if (minReach < maxReach) {
											double reach = (double)(rand() % (int)((maxReach - minReach) * 100)) / 100 + minReach;
											cout << reach << endl;
											if (keep) {
												jobject enemy = getMouseOver(jenv->GetFloatField(jenv->GetObjectField(mc, timerf), renderPartialTicksf), jenv, reach);
												if (enemy != NULL) {
													jobject playerController = jenv->GetObjectField(mc, playerControllerf);
													jenv->CallVoidMethod(thePlayer, swingItemf);
													jenv->CallVoidMethod(playerController, attackEntityf, thePlayer, enemy);
													attacked = true;
												}
											}
										}
									}
							} else if (attacked) {
								attacked = false;
							}
						currentScreen = jenv->GetObjectField(mc, currentScreenf);
						if(currentScreen != NULL)
							allowUserInput = jenv->GetBooleanField(currentScreen, allowUserInputf);
						if(inventoryClickT && allowUserInput){
							if (!attacked && clickerT) {
								SendMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
								Sleep(delay);
								SendMessage(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(pt.x, pt.y));
								Sleep(delay);
							}
							else if (clickerT) {
								Sleep(delay * 2);
							}
						} else if (currentScreen == NULL) {
							if (!attacked && clickerT) {
								SendMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
								Sleep(delay);
								SendMessage(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(pt.x, pt.y));
								Sleep(delay);
							}
							else if (clickerT) {
								Sleep(delay * 2);
							}
						}
						if (!clickerT)
							attacked2 = true;
					}
				}
				while (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000) && attacked2)
					attacked2 = false;
			}
		}
	Sleep(200);
	cleanup(jenv);

	if (jenv->ExceptionCheck())
	{
		jenv->ExceptionDescribe();
	}
	jvm->DetachCurrentThread();
	//some CPU relief
	Sleep(200);
	return 0;
}

void init()
{
	CreateThread(NULL, 0, &LoopFunction, NULL, 0, NULL);
	CreateThread(NULL, 0x1000, &Main_thread, 0, 0, NULL);
}

std::string exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);
	string mbid = exec("wmic baseboard get serialnumber").substr(19, 10);
	long hMBID = stol(mbid) * 3 + 2179;
	if (hMBID == 820113462) {
		switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:
			init();
			//case DLL_PROCESS_DETACH:
			//case DLL_THREAD_ATTACH:
			//case DLL_THREAD_DETACH:
		}
	}
	return true;
}