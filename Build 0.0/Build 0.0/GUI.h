#pragma once
#include<windows.h> // Windows API library
#include<GL/gl.h> // OpenGL Library
#include<GL/GLU.h> // GLU Library?
#include<GL/glut.h> // GLUT Library
#include "glui.h"
//#include "gui.h"
#include "Resource.h" // need for the menu IDs
#include <deque>
#include <fstream>

class GUIApp
{
	static const int		cSpinelineLenght = 100;
	static const int        cDataBufferSize = 100;
public:
	GUIApp();
	~GUIApp();

	
	void						setJointTypes(int J0, int J1);
	void						pushData(double data, double time);
	void						setOptiBodyClass(void * UBC);
	void						setWirelessClass(void* DUST);
	// graph
	DWORD						Run(HINSTANCE hInstance, int nCmdShow, HWND hDlg, int wmId);
	// spine
	DWORD						RunSpine(HINSTANCE hInstance, int nCmdShow, HWND hDlg, int wmId);
	// EMG
	DWORD						RunEMG(HINSTANCE hInstance, int nCmdShow, HWND hDlg, int wmId);
	//Force
	DWORD						RunForce(HINSTANCE hInstance, int nCmdShow, HWND hDlg, int wmId);
	int Datatype = 000; // for the getData() call to optibody
	//void						setOptiBodyClass(OptiBody& UBC);
private:
// class variables
	HWND                    m_hWnd, Kinect_hWnd, Wireless_hWnd; // Window Handels
	double                  m_nStartTime;	// Start Time of GUI
	INT64                   m_nLastCounter;	//
	double                  m_fFreq; // Timeing Variable
	INT64                   m_nNextStatusTime;
	DWORD                   m_nFramesSinceUpdate;
	int						windowID;	//
	int						JointType0; // JointType to be displayed on Graph
	int						JointType1; // second JointType to be displayed on Graph for limbs
	void*					m_UserBody; // Optibody Class pointer
	void*					m_WireLess; // Wireless class pointer
	ID2D1Factory*           m_pD2DFactory; // Drawing variables
	ID2D1HwndRenderTarget*  m_pRenderTarget;
	ID2D1SolidColorBrush*   m_pBrushGraphLine;
	ID2D1SolidColorBrush*   m_pBrushGraphLine2;
	ID2D1SolidColorBrush*   m_pBrushGraphLine3;
	ID2D1SolidColorBrush*   m_pBrushGraphLine4;
	ID2D1SolidColorBrush*   m_pBrushGraphLine5;

	std::ofstream Logfile; // Log file
	bool					f_dataSaved = false; // Logging Flag
	
	//bool                    SetStatusMessage(_In_z_ WCHAR* szMessage, DWORD nShowTimeMsec, bool bForce);

//Graph Private Functions
	void						Update(void);
	void						Display(void);
	HRESULT						EnsureDirect2DResources();
	std::array<std::deque<double>, 2>			dataBuffer; // 2 data points, one time one value

//Spine Private Funtions & Variables
	void						UpdateSpine(void);
	void						DisplaySpine(void);
	HRESULT						EnsureDirect2DResourcesSpine();
	std::array<std::array<std::deque<double>, 2>, 7> dataBufferSpine; //Array of 2 data points, one time one value	
	std::tuple<int,int,int> SpineGetMap(int i);
//Emg Private Functions & Variables
	void						UpdateEMG(void);
	void						DisplayEMG(void);
	HRESULT						EnsureDirect2DResourcesEMG();
	std::array<std::deque<double>, 2> dataBufferEMG; //  data time
//Force Private Functions & Variables
	void						UpdateForce(void);
	void						DisplayForce(void);
	HRESULT						EnsureDirect2DResourcesForce();
	std::array<std::deque<double>, 3> dataBufferForce; // left right time 
	
//Log functions
	void						SaveSpineBuffer(void);
	double						WristMin = 1000;
	double						WristMax = 1000;
	int liftState = 0;
	int LiftCount = 0;
	void CheckLiftPos(void);
// PoorForm Points
	int FormPoints = 0;
	int HipsYVHipAngV(void);
	int KneeAngVHipAngV(void);
	int KneeAngHipAng(void);
	int KneeAngV(void);
//	int WirstAForceEmg(void);
//Window Functions
	static LRESULT CALLBACK		MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK			DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool						SetStatusMessage(WCHAR * szMessage, DWORD nShowTimeMsec, bool bForce);
	void						DiscardDirect2DResources();
};
enum _SpineDataType
{
	SpineData_HipAngle = 0,
	SpineData_HipAngleVelo = 1,
	SpineData_KneeAngle = 2,
	SpineData_KneeAngleVelo = 3,
	SpineData_HipsYVelo = 4,
	SpineData_WristYAccel = 5,
	SpineData_WristYPos = 6
};
