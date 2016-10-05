// Coded by Chris Esterer March 2016

#pragma once
#include "windows.h"
#include <process.h>
#include "stdafx.h"

class Application
{
public:
	//Functions
	void sendNewData(DWORD idThread, UINT  Msg, WPARAM wParam, LPARAM lParam);
	double dataReceive(DWORD idThread, UINT  Msg, WPARAM wParam, LPARAM lParam);
	//int messageSend( )
	//int messageReceive( )
	// Function for Optibody to send a message when new data is available
	// Function send data to GUI application.
	// Function Receive Data
	void setApplication(wchar_t AppName[12], HINSTANCE AppInstance,void* ClassPtr,int GUIType,int GUIgetNumb,int CaseID);
	
	void getAppVar(HINSTANCE* AppInstance, void** ClassPtr, int* GUIType, int* GUIgetNumb, int* CaseID);
	//Setters
	//Getters
	//New
	Application();
	//Destroy
	~Application();

//Variables
	// Applicatoin
	wchar_t AppName[12];// String to hold app name
	HINSTANCE AppInstance;
	HINSTANCE ParentInstance;
	void* ClassPtr;
	int GUIType; // Type of Graph to be displayed
	int GUIgetNumb; // 3 Digit for get Case in Optibody
	int CaseID; // ID int for menu case;
	//string ClassName
	//Threading
	HANDLE Thread;
	DWORD ThreadID;
	std::deque<DWORD> MessageQue;

	//Windowing
	HWND hWnd;
	HINSTANCE WindowInstance;

	//ApplicationStarts
	void StartGUI(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void StartKinect(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);





};