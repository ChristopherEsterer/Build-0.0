//------------------------------------------------------------------------------
// <copyright file="BodyBasics.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <strsafe.h>
#include "Resource.h"
//#include "BodyBasics.h"
//#include "OptiBody.h"

static const float c_JointThickness = 3.0f;
static const float c_TrackedBoneThickness = 6.0f;
static const float c_InferredBoneThickness = 1.0f;
static const float c_HandSize = 30.0f;

/// <summary>
/// Entry point for the application
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="hPrevInstance">always 0</param>
/// <param name="lpCmdLine">command line arguments</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
/// <returns>status</returns>

OptiBody UserBody;

int APIENTRY KinectMain(    
	_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	
    CBodyBasics application;
  //  application.Run(hInstance, nShowCmd);
	return 0;
}
int KMain(HINSTANCE hInstance, int nShowCmd)
{

	CBodyBasics application;
	HINSTANCE NEWhInstance = (HINSTANCE)GetModuleHandle(NULL);
//	application.Run(NEWhInstance, nShowCmd);
	return 0;
}
//*** 
/// <summary>
/// Constructor
/// </summary>
CBodyBasics::CBodyBasics() :
    m_hWnd(NULL),
    m_nStartTime(0),
    m_nLastCounter(0),
    m_nFramesSinceUpdate(0),
    m_fFreq(0),
    m_nNextStatusTime(0LL),
    m_pKinectSensor(NULL),
    m_pCoordinateMapper(NULL),
    m_pBodyFrameReader(NULL),
    m_pD2DFactory(NULL),
    m_pRenderTarget(NULL),
    m_pBrushJointTracked(NULL),
    m_pBrushJointInferred(NULL),
    m_pBrushBoneTracked(NULL),
    m_pBrushBoneInferred(NULL),
    m_pBrushHandClosed(NULL),
    m_pBrushHandOpen(NULL),
    m_pBrushHandLasso(NULL)
{
    LARGE_INTEGER qpf = {0};
    if (QueryPerformanceFrequency(&qpf))
    {
        m_fFreq = double(qpf.QuadPart);
    }
}
  

/// <summary>
/// Destructor
/// </summary>
CBodyBasics::~CBodyBasics()
{
    DiscardDirect2DResources();

    // clean up Direct2D
    SafeRelease(m_pD2DFactory);

    // done with body frame reader
    SafeRelease(m_pBodyFrameReader);

    // done with coordinate mapper
    SafeRelease(m_pCoordinateMapper);

    // close the Kinect Sensor
    if (m_pKinectSensor)
    {
        m_pKinectSensor->Close();
    }

    SafeRelease(m_pKinectSensor);
}

/// <summary>
/// Creates the main window and begins processing
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
DWORD CBodyBasics::Run(HINSTANCE hInstance, int nCmdShow, HWND hDlg)
{
    MSG       msg = {0};
    WNDCLASS  wc;

    // Dialog custom window class
    ZeroMemory(&wc, sizeof(wc));
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.cbWndExtra    = DLGWINDOWEXTRA;
    wc.hCursor       = LoadCursorW(NULL, IDC_ARROW);
    wc.hIcon         = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_APP));//***
//	wc.hIcon = LoadIconW(NULL, NULL);//***
//	wc.hIcon = LoadIconW(hInstance, nCmdShow);//***
    wc.lpfnWndProc   = DefDlgProcW;
    wc.lpszClassName = L"BodyBasicsAppDlgWndClass";

    if (!RegisterClassW(&wc))
    {
        return 0;
    }

    // Create main application window
   /* HWND hWndApp = CreateDialogParamW(
        NULL,
        MAKEINTRESOURCE(IDD_APP),
        NULL,
        (DLGPROC)CBodyBasics::MessageRouter, 
        reinterpret_cast<LPARAM>(this)); */
	HWND hWndApp = CreateDialogParamW(
		NULL,
		MAKEINTRESOURCE(IDD_APP),
		hDlg,
		(DLGPROC)CBodyBasics::MessageRouter,
		reinterpret_cast<LPARAM>(this));


	DWORD TEMPE = GetLastError();
    // Show window
    ShowWindow(hWndApp, nCmdShow);

    // Main message loop
    while (WM_QUIT != msg.message)
    {
        Update();

        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // If a dialog message will be taken care of by the dialog proc
            if (hWndApp && IsDialogMessageW(hWndApp, &msg))
            {
                continue;
            }

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

	// Window is closeing, deregister the class.
	if (!UnregisterClassW(wc.lpszClassName, wc.hInstance))
	{
		return 0;
	}

    return static_cast<int>(msg.wParam);
}

void* CBodyBasics::GetUserBody(void)
{
	return &UserBody;
}



/// <summary>
/// Main processing function
/// </summary>
void CBodyBasics::Update()
{
    if (!m_pBodyFrameReader)
    {
        return;
    }

    IBodyFrame* pBodyFrame = NULL;

    HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

    if (SUCCEEDED(hr))
    {
        INT64 nTime = 0;

        hr = pBodyFrame->get_RelativeTime(&nTime);

        IBody* ppBodies[BODY_COUNT] = {0};

        if (SUCCEEDED(hr))
        {
            hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
        }

        if (SUCCEEDED(hr))
        {
            ProcessBody(nTime, BODY_COUNT, ppBodies);
        }

        for (int i = 0; i < _countof(ppBodies); ++i)
        {
            SafeRelease(ppBodies[i]);
        }
    }

    SafeRelease(pBodyFrame);
}

/// <summary>
/// Handles window messages, passes most to the class instance to handle
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK CBodyBasics::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CBodyBasics* pThis = NULL;
    
    if (WM_INITDIALOG == uMsg)
    {
        pThis = reinterpret_cast<CBodyBasics*>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else
    {
        pThis = reinterpret_cast<CBodyBasics*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis)
    {
        return pThis->DlgProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

/// <summary>
/// Handle windows messages for the class instance
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK CBodyBasics::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
        case WM_INITDIALOG:
        {
            // Bind application window handle
            m_hWnd = hWnd;
			UserBody.setHWnd(hWnd);
            // Init Direct2D
            D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

            // Get and initialize the default Kinect sensor
            InitializeDefaultSensor();
        }
        break;

        // If the titlebar X is clicked, destroy app
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;

        case WM_DESTROY:
            // Quit the main message pump
            PostQuitMessage(0);
            break;
    }

    return FALSE;
}

/// <summary>
/// Initializes the default Kinect sensor
/// </summary>
/// <returns>indicates success or failure</returns>
HRESULT CBodyBasics::InitializeDefaultSensor()
{
    HRESULT hr;

    hr = GetDefaultKinectSensor(&m_pKinectSensor);
    if (FAILED(hr))
    {
        return hr;
    }

    if (m_pKinectSensor)
    {
        // Initialize the Kinect and get coordinate mapper and the body reader
        IBodyFrameSource* pBodyFrameSource = NULL;
		//OptiBody* InitBody = new OptiBody;
		//OBody = InitBody;

        hr = m_pKinectSensor->Open();

        if (SUCCEEDED(hr))
        {
            hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
        }

        if (SUCCEEDED(hr))
        {
            hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
        }

        if (SUCCEEDED(hr))
        {
            hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
        }

        SafeRelease(pBodyFrameSource);
    }

    if (!m_pKinectSensor || FAILED(hr))
    {
        SetStatusMessage(L"No ready Kinect found!", 10000, true);
        return E_FAIL;
    }

    return hr;
}

/// <summary>
/// Handle new body data
/// <param name="nTime">timestamp of frame</param>
/// <param name="nBodyCount">body data count</param>
/// <param name="ppBodies">body data in frame</param>
/// </summary>
void CBodyBasics::ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies)
{
    if (m_hWnd)
    {
        HRESULT hr = EnsureDirect2DResources();

        if (SUCCEEDED(hr) && m_pRenderTarget && m_pCoordinateMapper)
        {

            m_pRenderTarget->BeginDraw();
            m_pRenderTarget->Clear();

            RECT rct;
            GetClientRect(GetDlgItem(m_hWnd, IDC_VIDEOVIEW), &rct);
            int width = rct.right;
            int height = rct.bottom;

            for (int i = 0; i < nBodyCount; ++i)
            {
                IBody* pBody = ppBodies[i];
                if (pBody)
                {
                    BOOLEAN bTracked = false;
                    hr = pBody->get_IsTracked(&bTracked);

                    if (SUCCEEDED(hr) && bTracked)
                    {
                        Joint joints[JointType_Count]; 
                        D2D1_POINT_2F jointPoints[JointType_Count];
                        HandState leftHandState = HandState_Unknown;
                        HandState rightHandState = HandState_Unknown;

                        pBody->get_HandLeftState(&leftHandState);
                        pBody->get_HandRightState(&rightHandState);

                        hr = pBody->GetJoints(_countof(joints), joints);
						// Copy the joints object to analyse
						// Save the joint data as well.

                        if (SUCCEEDED(hr))
                        {
                            for (int j = 0; j < _countof(joints); ++j)
                            {
								//saveJointSpace(joints[i]);
								//saveJointSpace(Type, X, Y, Z, State, Delta T);
							//	UserBody.saveJointSpace((int)joints[i].JointType,(float) joints[i].Position.X ,(float) joints[i].Position.Y,(float) joints[i].Position.Z, (int) joints[i].TrackingState);
                                jointPoints[j] = BodyToScreen(joints[j].Position, width, height);
                            }

                            DrawBody(joints, jointPoints); //* Draw the joints
							SaveBody(joints, jointPoints, (nTime - m_nStartTime)); //*** New
								double interval = 0.0;
								LARGE_INTEGER qpcNow = { 0 };
								if (m_fFreq)
								{
									if (QueryPerformanceCounter(&qpcNow))
									{
										if (m_nLastCounter)
										{
											interval = (double(qpcNow.QuadPart - m_nLastCounter)) / (m_fFreq*m_nFramesSinceUpdate);;
										}
									}
								}

							AnalyseBody( joints, jointPoints, interval);
                            DrawHand(leftHandState, jointPoints[JointType_HandLeft]); //* Draw the Left Hand
                            DrawHand(rightHandState, jointPoints[JointType_HandRight]); //* Draw the Right Hand
                        }
                    }
                }
            }
			
            hr = m_pRenderTarget->EndDraw();

            // Device lost, need to recreate the render target
            // We'll dispose it now and retry drawing
            if (D2DERR_RECREATE_TARGET == hr)
            {
                hr = S_OK;
                DiscardDirect2DResources();
            }

		}

		//UINT msg = 0x8001;

		//if (!PostMessageW(ParentWindow, msg, 0, 0))
		//{
		//	Sleep(1);
		//}
		

        if (!m_nStartTime)
        {
            m_nStartTime = nTime;
        }
        double fps = 0.0;
		double interval = 0.0;
	//	Sleep(1000);
        LARGE_INTEGER qpcNow = {0};
        if (m_fFreq)
        {
            if (QueryPerformanceCounter(&qpcNow))
            {
                if (m_nLastCounter)
                {
                    m_nFramesSinceUpdate++;
                    fps = m_fFreq * m_nFramesSinceUpdate / double(qpcNow.QuadPart - m_nLastCounter);
					
					interval =  (double(qpcNow.QuadPart - m_nLastCounter) )/ (m_fFreq*m_nFramesSinceUpdate);
                }
            }
        }

        WCHAR szStatusMessage[64];
        StringCchPrintf(szStatusMessage, _countof(szStatusMessage), L" FPS = %0.2f	Time = %I64d	Interval = %0.5f	FramesSkiped = %u", fps, (nTime - m_nStartTime), interval, (unsigned int)m_nFramesSinceUpdate);

        if (SetStatusMessage(szStatusMessage, 1000, false))
        {
            m_nLastCounter = qpcNow.QuadPart;
            m_nFramesSinceUpdate = 0;
        }
    }
}

/// <summary>
/// Set the status bar message
/// </summary>
/// <param name="szMessage">message to display</param>
/// <param name="showTimeMsec">time in milliseconds to ignore future status messages</param>
/// <param name="bForce">force status update</param>
bool CBodyBasics::SetStatusMessage(_In_z_ WCHAR* szMessage, DWORD nShowTimeMsec, bool bForce)
{
    INT64 now = GetTickCount64();

    if (m_hWnd && (bForce || (m_nNextStatusTime <= now)))
    {
        SetDlgItemText(m_hWnd, IDC_STATUS, szMessage);
        m_nNextStatusTime = now + nShowTimeMsec;

        return true;
    }

    return false;
}

/// <summary>
/// Ensure necessary Direct2d resources are created
/// </summary>
/// <returns>S_OK if successful, otherwise an error code</returns>
HRESULT CBodyBasics::EnsureDirect2DResources()
{
    HRESULT hr = S_OK;

    if (m_pD2DFactory && !m_pRenderTarget)
    {
        RECT rc;
        GetWindowRect(GetDlgItem(m_hWnd, IDC_VIDEOVIEW), &rc);  

        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;
        D2D1_SIZE_U size = D2D1::SizeU(width, height);
        D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
        rtProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
        rtProps.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;

        // Create a Hwnd render target, in order to render to the window set in initialize
        hr = m_pD2DFactory->CreateHwndRenderTarget(
            rtProps,
            D2D1::HwndRenderTargetProperties(GetDlgItem(m_hWnd, IDC_VIDEOVIEW), size),
            &m_pRenderTarget
        );

        if (FAILED(hr))
        {
            SetStatusMessage(L"Couldn't create Direct2D render target!", 10000, true);
            return hr;
        }

        // light green
        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.27f, 0.75f, 0.27f), &m_pBrushJointTracked);

        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 1.0f), &m_pBrushJointInferred);
        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 1.0f), &m_pBrushBoneTracked);
        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray, 1.0f), &m_pBrushBoneInferred);

        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.5f), &m_pBrushHandClosed);
        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 0.5f), &m_pBrushHandOpen);
        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushHandLasso);
    }

    return hr;
}

/// <summary>
/// Dispose Direct2d resources 
/// </summary>
void CBodyBasics::DiscardDirect2DResources()
{
    SafeRelease(m_pRenderTarget);

    SafeRelease(m_pBrushJointTracked);
    SafeRelease(m_pBrushJointInferred);
    SafeRelease(m_pBrushBoneTracked);
    SafeRelease(m_pBrushBoneInferred);

    SafeRelease(m_pBrushHandClosed);
    SafeRelease(m_pBrushHandOpen);
    SafeRelease(m_pBrushHandLasso);
}

/// <summary>
/// Converts a body point to screen space
/// </summary>
/// <param name="bodyPoint">body point to tranform</param>
/// <param name="width">width (in pixels) of output buffer</param>
/// <param name="height">height (in pixels) of output buffer</param>
/// <returns>point in screen-space</returns>
D2D1_POINT_2F CBodyBasics::BodyToScreen(const CameraSpacePoint& bodyPoint, int width, int height)
{
    // Calculate the body's position on the screen
    DepthSpacePoint depthPoint = {0};
    m_pCoordinateMapper->MapCameraPointToDepthSpace(bodyPoint, &depthPoint);

    float screenPointX = static_cast<float>(depthPoint.X * width) / cDepthWidth;
    float screenPointY = static_cast<float>(depthPoint.Y * height) / cDepthHeight;

    return D2D1::Point2F(screenPointX, screenPointY);
}

/// <summary>
/// Draws a body 
/// </summary>
/// <param name="pJoints">joint data</param>
/// <param name="pJointPoints">joint positions converted to screen space</param>
void CBodyBasics::DrawBody(const Joint* pJoints, const D2D1_POINT_2F* pJointPoints)
{
    // Draw the bones

    // Torso
    DrawBone(pJoints, pJointPoints, JointType_Head, JointType_Neck);
    DrawBone(pJoints, pJointPoints, JointType_Neck, JointType_SpineShoulder);
    DrawBone(pJoints, pJointPoints, JointType_SpineShoulder, JointType_SpineMid);
    DrawBone(pJoints, pJointPoints, JointType_SpineMid, JointType_SpineBase);
    DrawBone(pJoints, pJointPoints, JointType_SpineShoulder, JointType_ShoulderRight);
    DrawBone(pJoints, pJointPoints, JointType_SpineShoulder, JointType_ShoulderLeft);
    DrawBone(pJoints, pJointPoints, JointType_SpineBase, JointType_HipRight);
    DrawBone(pJoints, pJointPoints, JointType_SpineBase, JointType_HipLeft);
    
    // Right Arm    
    DrawBone(pJoints, pJointPoints, JointType_ShoulderRight, JointType_ElbowRight);
    DrawBone(pJoints, pJointPoints, JointType_ElbowRight, JointType_WristRight);
    DrawBone(pJoints, pJointPoints, JointType_WristRight, JointType_HandRight);
    DrawBone(pJoints, pJointPoints, JointType_HandRight, JointType_HandTipRight);
    DrawBone(pJoints, pJointPoints, JointType_WristRight, JointType_ThumbRight);

    // Left Arm
    DrawBone(pJoints, pJointPoints, JointType_ShoulderLeft, JointType_ElbowLeft);
    DrawBone(pJoints, pJointPoints, JointType_ElbowLeft, JointType_WristLeft);
    DrawBone(pJoints, pJointPoints, JointType_WristLeft, JointType_HandLeft);
    DrawBone(pJoints, pJointPoints, JointType_HandLeft, JointType_HandTipLeft);
    DrawBone(pJoints, pJointPoints, JointType_WristLeft, JointType_ThumbLeft);

    // Right Leg
    DrawBone(pJoints, pJointPoints, JointType_HipRight, JointType_KneeRight);
    DrawBone(pJoints, pJointPoints, JointType_KneeRight, JointType_AnkleRight);
    DrawBone(pJoints, pJointPoints, JointType_AnkleRight, JointType_FootRight);

    // Left Leg
    DrawBone(pJoints, pJointPoints, JointType_HipLeft, JointType_KneeLeft);
    DrawBone(pJoints, pJointPoints, JointType_KneeLeft, JointType_AnkleLeft);
    DrawBone(pJoints, pJointPoints, JointType_AnkleLeft, JointType_FootLeft);

    // Draw the joints
    for (int i = 0; i < JointType_Count; ++i)
    {
        D2D1_ELLIPSE ellipse = D2D1::Ellipse(pJointPoints[i], c_JointThickness, c_JointThickness);

        if (pJoints[i].TrackingState == TrackingState_Inferred)
        {
            m_pRenderTarget->FillEllipse(ellipse, m_pBrushJointInferred);
        }
        else if (pJoints[i].TrackingState == TrackingState_Tracked)
        {
            m_pRenderTarget->FillEllipse(ellipse, m_pBrushJointTracked);
        }
    }
}
//
// *** Added FUNCTIONS ***
//
void CBodyBasics::SaveBody(const Joint* pJoints, const D2D1_POINT_2F* pJointPoints, double time)
{ //*** Needs work, Like not even started
	// Draw the bones
	//Save Joints
	SaveJointPoint(pJoints, pJointPoints, time, JointType_Head);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_Neck);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_SpineShoulder);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_SpineMid);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_SpineBase);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_ShoulderRight);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_ShoulderLeft);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_SpineBase);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_HipRight);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_HipLeft);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_ElbowRight);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_WristRight);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_HandRight);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_HandTipRight);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_ThumbRight);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_ElbowLeft);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_WristLeft);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_HandLeft);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_HandTipLeft);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_ThumbLeft);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_KneeRight);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_AnkleRight);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_FootRight);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_KneeLeft);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_AnkleLeft);
	SaveJointPoint(pJoints, pJointPoints, time, JointType_FootLeft);

	//SaveLimbs
		// Torso
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_Head, JointType_Neck);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_Neck, JointType_SpineShoulder);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_SpineShoulder, JointType_SpineMid);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_SpineMid, JointType_SpineBase);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_SpineShoulder, JointType_SpineBase); // ***
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_SpineShoulder, JointType_ShoulderRight);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_SpineShoulder, JointType_ShoulderLeft);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_SpineBase, JointType_HipRight);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_SpineBase, JointType_HipLeft);

	// Right Arm    
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_ShoulderRight, JointType_ElbowRight);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_ElbowRight, JointType_WristRight);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_WristRight, JointType_HandRight);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_HandRight, JointType_HandTipRight);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_WristRight, JointType_ThumbRight);

	// Left Arm
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_ShoulderLeft, JointType_ElbowLeft);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_ElbowLeft, JointType_WristLeft);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_WristLeft, JointType_HandLeft);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_HandLeft, JointType_HandTipLeft);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_WristLeft, JointType_ThumbLeft);

	// Right Leg
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_HipRight, JointType_KneeRight);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_KneeRight, JointType_AnkleRight);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_AnkleRight, JointType_FootRight);

	// Left Leg
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_HipLeft, JointType_KneeLeft);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_KneeLeft, JointType_AnkleLeft);
	ComputeLimbVector(pJoints, pJointPoints, time, JointType_AnkleLeft, JointType_FootLeft);
	
	UserBody.times.push_back(time);
	
}
void CBodyBasics::AnalyseBody(const Joint* pJoints, const D2D1_POINT_2F* pJointPoints, double& interval)
{
	// Analyse the bones
	UserBody.interval = interval;

	//ComputeJointDerivative(pJoints, pJointPoints, JointType_Head);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_Neck);
	ComputeJointDerivative(pJoints, pJointPoints, JointType_SpineShoulder);
	ComputeJointDerivative(pJoints, pJointPoints, JointType_SpineMid);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_SpineBase);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_ShoulderRight);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_ShoulderLeft);
	ComputeJointDerivative(pJoints, pJointPoints, JointType_SpineBase);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_HipRight);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_HipLeft);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_ElbowRight);
	ComputeJointDerivative(pJoints, pJointPoints, JointType_WristRight);
	ComputeJointDerivative(pJoints, pJointPoints, JointType_HandRight);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_HandTipRight);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_ThumbRight);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_ElbowLeft);
	ComputeJointDerivative(pJoints, pJointPoints, JointType_WristLeft);
	ComputeJointDerivative(pJoints, pJointPoints, JointType_HandLeft);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_HandTipLeft);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_ThumbLeft);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_KneeRight);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_AnkleRight);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_FootRight);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_KneeLeft);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_AnkleLeft);
	//ComputeJointDerivative(pJoints, pJointPoints, JointType_FootLeft);

	UserBody.compJointAngle(JointType_SpineMid);
	ComputeAngleDerivative(pJoints, pJointPoints, JointType_SpineMid);
	UserBody.compJointAngle(JointType_SpineBase);
	ComputeAngleDerivative(pJoints, pJointPoints, JointType_SpineBase);
	UserBody.compJointAngle(JointType_KneeLeft);
	ComputeAngleDerivative(pJoints, pJointPoints, JointType_KneeLeft);
	UserBody.compJointAngle(JointType_KneeRight);
	ComputeAngleDerivative(pJoints, pJointPoints, JointType_KneeRight);
	UserBody.compJointAngle(JointType_SpineShoulder);
	ComputeAngleDerivative(pJoints, pJointPoints, JointType_SpineShoulder);

	// Torso
	//ComputeLimbDerivative(pJoints, pJointPoints, JointType_Head, JointType_Neck);
	//ComputeLimbDerivative(pJoints, pJointPoints, JointType_Neck, JointType_SpineShoulder);
	//ComputeLimbDerivative(pJoints, pJointPoints, JointType_SpineShoulder, JointType_SpineMid);
	//ComputeLimbDerivative(pJoints, pJointPoints, JointType_SpineMid, JointType_SpineBase);
	//ComputeLimbDerivative(pJoints, pJointPoints, JointType_SpineShoulder, JointType_ShoulderRight);
	//ComputeLimbDerivative(pJoints, pJointPoints, JointType_SpineShoulder, JointType_ShoulderLeft);
	//ComputeLimbDerivative(pJoints, pJointPoints, JointType_SpineBase, JointType_HipRight);
	//ComputeLimbDerivative(pJoints, pJointPoints, JointType_SpineBase, JointType_HipLeft);

	// Right Arm    
	/*ComputeLimbDerivative(pJoints, pJointPoints, JointType_ShoulderRight, JointType_ElbowRight);
	ComputeLimbDerivative(pJoints, pJointPoints, JointType_ElbowRight, JointType_WristRight);
	ComputeLimbDerivative(pJoints, pJointPoints, JointType_WristRight, JointType_HandRight);
	ComputeLimbDerivative(pJoints, pJointPoints, JointType_HandRight, JointType_HandTipRight);
	ComputeLimbDerivative(pJoints, pJointPoints, JointType_WristRight, JointType_ThumbRight);*/

	// Left Arm
	/*ComputeLimbDerivative(pJoints, pJointPoints, JointType_ShoulderLeft, JointType_ElbowLeft);
	ComputeLimbDerivative(pJoints, pJointPoints, JointType_ElbowLeft, JointType_WristLeft);
	ComputeLimbDerivative(pJoints, pJointPoints, JointType_WristLeft, JointType_HandLeft);
	ComputeLimbDerivative(pJoints, pJointPoints, JointType_HandLeft, JointType_HandTipLeft);
	ComputeLimbDerivative(pJoints, pJointPoints, JointType_WristLeft, JointType_ThumbLeft);*/

	// Right Leg
	//ComputeLimbDerivative(pJoints, pJointPoints, JointType_HipRight, JointType_KneeRight);
	////ComputeLimbDerivative(pJoints, pJointPoints, JointType_KneeRight, JointType_AnkleRight);
	////ComputeLimbDerivative(pJoints, pJointPoints, JointType_AnkleRight, JointType_FootRight);

	//// Left Leg
	//ComputeLimbDerivative(pJoints, pJointPoints, JointType_HipLeft, JointType_KneeLeft);
	////ComputeLimbDerivative(pJoints, pJointPoints, JointType_KneeLeft, JointType_AnkleLeft);
	//ComputeLimbDerivative(pJoints, pJointPoints, JointType_AnkleLeft, JointType_FootLeft);
	
	UserBody.incFrameCounter();
	UserBody.setNewDataFlag(TRUE);
}
void CBodyBasics::ComputeLimbVector(const Joint* pJoints, const D2D1_POINT_2F* pJointPoints, double time, JointType joint0, JointType joint1)
{ // Code from DrawBone()
	TrackingState joint0State = pJoints[joint0].TrackingState;
	TrackingState joint1State = pJoints[joint1].TrackingState;

	// If we can't find either of these joints, exit
	if ((joint0State == TrackingState_NotTracked) || (joint1State == TrackingState_NotTracked))
	{ // We need to flag this for derivative. so we dont get crazy peak accel
		return;
	}

	// Don't draw if both points are inferred
	if ((joint0State == TrackingState_Inferred) && (joint1State == TrackingState_Inferred))
	{// We need to flag this for derivative. so we dont get crazy peak accel
		return;
	}

	// We assume all drawn bones are inferred unless BOTH joints are tracked
	if ((joint0State == TrackingState_Tracked) && (joint1State == TrackingState_Tracked))
	{// Put the Functoin call here // *** ***
		float Tx = (pJoints[joint0].Position.X) - (pJoints[joint1].Position.X);
		float Ty = (pJoints[joint0].Position.Y) - (pJoints[joint1].Position.Y);
		float Tz = (pJoints[joint0].Position.Z) - (pJoints[joint1].Position.Z);
		UserBody.saveLimbVector(joint0, joint1, Tx, Ty, Tz, TrackingState_Tracked, time);
	}
	else
	{
		float Tx = (pJoints[joint0].Position.X) - (pJoints[joint1].Position.X);
		float Ty = (pJoints[joint0].Position.Y) - (pJoints[joint1].Position.Y);
		float Tz = (pJoints[joint0].Position.Z) - (pJoints[joint1].Position.Z);
		UserBody.saveLimbVector(joint0, joint1, Tx, Ty, Tz, TrackingState_Inferred, time);
	}
}
void CBodyBasics::ComputeLimbDerivative(const Joint* pJoints, const D2D1_POINT_2F* pJointPoints, JointType joint0, JointType joint1)
{ // Code from DrawBone()
	TrackingState joint0State = pJoints[joint0].TrackingState;
	TrackingState joint1State = pJoints[joint1].TrackingState;

	// If we can't find either of these joints, exit
	if ((joint0State == TrackingState_NotTracked) || (joint1State == TrackingState_NotTracked))
	{ // We need to flag this for derivative. so we dont get crazy peak accel
		return;
	}

	// Don't draw if both points are inferred
	if ((joint0State == TrackingState_Inferred) && (joint1State == TrackingState_Inferred))
	{// We need to flag this for derivative. so we dont get crazy peak accel
		return;
	}

	// We assume all drawn bones are inferred unless BOTH joints are tracked
	if ((joint0State == TrackingState_Tracked) && (joint1State == TrackingState_Tracked))
	{// Put the Functoin call here // *** ***
		UserBody.compDerivative(joint0, joint1,0);
		UserBody.compDerivative(joint0, joint1, 1);
	}
	else
	{
		UserBody.compDerivative(joint0, joint1,0);
		UserBody.compDerivative(joint0, joint1, 1);
	}
}

void CBodyBasics::ComputeJointDerivative(const Joint * pJoints, const D2D1_POINT_2F * pJointPoints, JointType joint0)
{
	TrackingState joint0State = pJoints[joint0].TrackingState;


	// If we can't find either of these joints, exit
	if (joint0State == TrackingState_NotTracked)
	{ // We need to flag this for derivative. so we dont get crazy peak accel
		return;
	}

	// Don't draw if both points are inferred
	if (joint0State == TrackingState_Inferred)
	{// We need to flag this for derivative. so we dont get crazy peak accel
		return;
	}

	// We assume all drawn bones are inferred unless BOTH joints are tracked
	if (joint0State == TrackingState_Tracked)
	{// Put the Functoin call here // *** ***
		UserBody.compDerivative(joint0, joint0, 2);
		UserBody.compDerivative(joint0, joint0, 3);
	}
	else
	{
		UserBody.compDerivative(joint0, joint0, 2);
		UserBody.compDerivative(joint0, joint0, 3);
	}

}
void CBodyBasics::ComputeAngleDerivative(const Joint * pJoints, const D2D1_POINT_2F * pJointPoints, JointType joint0)
{
	TrackingState joint0State = pJoints[joint0].TrackingState;


	// If we can't find either of these joints, exit
	if (joint0State == TrackingState_NotTracked)
	{ // We need to flag this for derivative. so we dont get crazy peak accel
		return;
	}

	// Don't draw if both points are inferred
	if (joint0State == TrackingState_Inferred)
	{// We need to flag this for derivative. so we dont get crazy peak accel
		return;
	}

	// We assume all drawn bones are inferred unless BOTH joints are tracked
	if (joint0State == TrackingState_Tracked)
	{// Put the Functoin call here // *** ***
		UserBody.compDerivative(joint0, joint0, 4);
		UserBody.compDerivative(joint0, joint0, 5);
	}
	else
	{
		UserBody.compDerivative(joint0, joint0, 4);
		UserBody.compDerivative(joint0, joint0, 5);
	}

}

void CBodyBasics::SaveJointPoint(const Joint * pJoints, const D2D1_POINT_2F * pJointPoints, double time, JointType joint0)
{
	TrackingState joint0State = pJoints[joint0].TrackingState;


	// Don't draw if both points are inferred
	if (joint0State == TrackingState_Inferred)
	{// We need to flag this for derivative. so we dont get crazy peak accel
		float Tx = (pJoints[joint0].Position.X);
		float Ty = (pJoints[joint0].Position.Y);
		float Tz = (pJoints[joint0].Position.Z);
		UserBody.saveJointPoint(joint0, Tx, Ty, Tz, TrackingState_Inferred, time);
	}

	// We assume all drawn bones are inferred unless BOTH joints are tracked
	if (joint0State == TrackingState_Tracked)
	{// Put the Functoin call here // *** ***
		float Tx = (pJoints[joint0].Position.X);
		float Ty = (pJoints[joint0].Position.Y);
		float Tz = (pJoints[joint0].Position.Z);
		UserBody.saveJointPoint(joint0, Tx, Ty, Tz, TrackingState_Tracked, time);
	}
	/*else
	{
		float Tx = (pJoints[joint0].Position.X) - (pJoints[joint1].Position.X);
		float Ty = (pJoints[joint0].Position.Y) - (pJoints[joint1].Position.Y);
		float Tz = (pJoints[joint0].Position.Z) - (pJoints[joint1].Position.Z);
		UserBody.saveLimbVector(joint0, joint1, Tx, Ty, Tz, TrackingState_Inferred, time);
	}*/
}

// *** End
//
/// <summary>
/// Draws one bone of a body (joint to joint)
/// </summary>
/// <param name="pJoints">joint data</param>
/// <param name="pJointPoints">joint positions converted to screen space</param>
/// <param name="pJointPoints">joint positions converted to screen space</param>
/// <param name="joint0">one joint of the bone to draw</param>
/// <param name="joint1">other joint of the bone to draw</param>
void CBodyBasics::DrawBone(const Joint* pJoints, const D2D1_POINT_2F* pJointPoints, JointType joint0, JointType joint1)
{
    TrackingState joint0State = pJoints[joint0].TrackingState;
    TrackingState joint1State = pJoints[joint1].TrackingState;

    // If we can't find either of these joints, exit
    if ((joint0State == TrackingState_NotTracked) || (joint1State == TrackingState_NotTracked))
    {
        return;
    }

    // Don't draw if both points are inferred
    if ((joint0State == TrackingState_Inferred) && (joint1State == TrackingState_Inferred))
    {
        return;
    }

    // We assume all drawn bones are inferred unless BOTH joints are tracked
    if ((joint0State == TrackingState_Tracked) && (joint1State == TrackingState_Tracked))
    {
        m_pRenderTarget->DrawLine(pJointPoints[joint0], pJointPoints[joint1], m_pBrushBoneTracked, c_TrackedBoneThickness);
    }
    else
    {
        m_pRenderTarget->DrawLine(pJointPoints[joint0], pJointPoints[joint1], m_pBrushBoneInferred, c_InferredBoneThickness);
    }
}

/// <summary>
/// Draws a hand symbol if the hand is tracked: red circle = closed, green circle = opened; blue circle = lasso
/// </summary>
/// <param name="handState">state of the hand</param>
/// <param name="handPosition">position of the hand</param>
void CBodyBasics::DrawHand(HandState handState, const D2D1_POINT_2F& handPosition)
{
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(handPosition, c_HandSize, c_HandSize);

    switch (handState)
    {
        case HandState_Closed:
            m_pRenderTarget->FillEllipse(ellipse, m_pBrushHandClosed);
            break;

        case HandState_Open:
            m_pRenderTarget->FillEllipse(ellipse, m_pBrushHandOpen);
            break;

        case HandState_Lasso:
            m_pRenderTarget->FillEllipse(ellipse, m_pBrushHandLasso);
            break;
    }
}
