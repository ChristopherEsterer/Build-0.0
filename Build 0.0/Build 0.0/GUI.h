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
	// graph
	DWORD						Run(HINSTANCE hInstance, int nCmdShow, HWND hDlg, int wmId);

	// spine
	DWORD						RunSpine(HINSTANCE hInstance, int nCmdShow, HWND hDlg, int wmId);

	int Datatype = 000; // for the getData() call to optibody
	//void						setOptiBodyClass(OptiBody& UBC);
private:
// class variables
	HWND                    m_hWnd, Kinect_hWnd;
	double                  m_nStartTime;
	INT64                   m_nLastCounter;
	double                  m_fFreq;
	INT64                   m_nNextStatusTime;
	DWORD                   m_nFramesSinceUpdate;
	int						windowID;	
	int						JointType0;
	int						JointType1;
	void*					m_UserBody;
	ID2D1Factory*           m_pD2DFactory;
	ID2D1HwndRenderTarget*  m_pRenderTarget;
	ID2D1SolidColorBrush*   m_pBrushGraphLine;
	ID2D1SolidColorBrush*   m_pBrushGraphLine2;
	ID2D1SolidColorBrush*   m_pBrushGraphLine3;
	ID2D1SolidColorBrush*   m_pBrushGraphLine4;
	ID2D1SolidColorBrush*   m_pBrushGraphLine5;

	std::ofstream Logfile;

	bool					f_dataSaved = false;
	
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
