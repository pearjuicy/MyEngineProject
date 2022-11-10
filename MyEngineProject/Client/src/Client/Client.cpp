#include "ClientPCH.h"
#include "Client/WindowManager.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) 
{
	bool result;

	WindowManager* application = new WindowManager();
	
	application->Initialize();
	application->Loop();

	
	return 0;
}