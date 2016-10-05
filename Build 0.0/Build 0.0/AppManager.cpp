#include "stdafx.h"
#include "AppManager.h"

void Application::sendNewData(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam)
{
}

double Application::dataReceive(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return 0.0;
}

void Application::setApplication(wchar_t AppNam[12], HINSTANCE AppInstanc, void * ClassPt, int GUITyp, int GUIgetNum, int CaseI)
{
	//AppName = AppNam;
	AppInstance = AppInstanc;
	ClassPtr = ClassPt;
	GUIType = GUITyp;
	GUIgetNumb = GUIgetNum;
	CaseID = CaseI;
}

void Application::getAppVar(HINSTANCE * AppInstanc, void ** ClassPt, int * GUITyp, int * GUIgetNum, int * CaseI)
{
	AppInstanc = &AppInstance;
	ClassPt = &ClassPtr;
	GUITyp = &GUIType;
	GUIgetNum = &GUIgetNumb;
	CaseI = &CaseID;
	return;
}

Application::Application()
{

}

Application::~Application()
{
}

void Application::StartGUI(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//int wmId = LOWORD(wParam); // get the message from the menu call so we know what kind of graph to make

	//HINSTANCE NEWhInstance = (HINSTANCE)GetModuleHandle(NULL);
	//GUIApp* NewGUI = new GUIApp;
	////GUIInst = NEWhInstance; // save incase we need it later
	////int wmId = LOWORD(wParam);
	//// Parse the menu selections:
	//switch (wmId)
	//{
	//case ID_HIPS_ANGLE:
	//	GuiApp[HIPSANGLE_PNUM].Datatype = 010;
	//	GuiApp[HIPSANGLE_PNUM].Run(hInst, Show, hDlg, wmId);

	//	GuiAppPtr[HIPSANGLE_PNUM] = NewGUI;

	//	ToClose[HIPSANGLE_PNUM] = std::this_thread::get_id();
	//	break;
	//case ID_WRISTS_ACCELLERATION:
	//	GuiApp[WRISTSACCELL_PNUM].Datatype = 120;
	//	GuiApp[WRISTSACCELL_PNUM].Run(hInst, Show, hDlg, wmId);

	//	ToClose[WRISTSACCELL_PNUM] = std::this_thread::get_id();
	//	break;
	//}


	////GuiApp[0].Run(hInst, Show, hDlg, wmId);

	//// this is where you save the ID to tell Build to remove the thread
	////ToClose[] = std::this_thread::get_id());

};

void Application::StartKinect(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

}
