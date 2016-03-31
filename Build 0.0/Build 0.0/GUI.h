#pragma once
#include<windows.h> // Windows API library
#include<GL/gl.h> // OpenGL Library
#include<GL/GLU.h> // GLU Library?
#include<GL/glut.h> // GLUT Library
#include "glui.h"
//#include "gui.h"
#include "Resource.h" // need for the menu IDs
#include <deque>

class GUIApp
{
	static const int        cDataBufferSize = 100;
public:
	GUIApp();
	~GUIApp();
	static LRESULT CALLBACK		MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK			DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	DWORD						Run(HINSTANCE hInstance, int nCmdShow, HWND hDlg, int wmId);
	void						Display(void);
	bool						SetStatusMessage(WCHAR * szMessage, DWORD nShowTimeMsec, bool bForce);
	HRESULT						EnsureDirect2DResources();
	void						DiscardDirect2DResources();
	void						Update(void);
	void						setJointTypes(int J0, int J1);
	void						pushData(double data, double time);
	void						setOptiBodyClass(void * UBC);
	
	int Datatype = 000; // for the getData() call to optibody
	//void						setOptiBodyClass(OptiBody& UBC);
private:
	HWND                    m_hWnd, Kinect_hWnd;

	double                  m_nStartTime;
	INT64                   m_nLastCounter;
	double                  m_fFreq;
	INT64                   m_nNextStatusTime;
	DWORD                   m_nFramesSinceUpdate;
	int						windowID;
	
	int						JointType0;
	int						JointType1;
	
	std::array<std::deque<double>,2>			dataBuffer; // 2 data points, one time one value


	void*					m_UserBody;

	ID2D1Factory*           m_pD2DFactory;
	ID2D1HwndRenderTarget*  m_pRenderTarget;
	ID2D1SolidColorBrush*   m_pBrushGraphLine;
	ID2D1SolidColorBrush*   m_pBrushGraphLine2;
	ID2D1SolidColorBrush*   m_pBrushGraphLine3;
	ID2D1SolidColorBrush*   m_pBrushGraphLine4;
	ID2D1SolidColorBrush*   m_pBrushGraphLine5;

	//bool                    SetStatusMessage(_In_z_ WCHAR* szMessage, DWORD nShowTimeMsec, bool bForce);


};
