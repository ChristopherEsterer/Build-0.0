// OptiBody.cpp by Chris Esterer
// This .cpp is used in a larger program.
// Function: To Get Joint Data from BodyBasics.cpp and analyse and save the data.
// Pretty much everything with Kinect Joints in here.
#include "stdafx.h"
#include "OptiBody.h"
//#include "OptiBodyStructs.cpp"
#define _JointType_
#include <array>
#include "windows.h"
#include "math.h"
//void OptiBody::OptiBody() {
// Initialize class, make new containters and pointers to them
	//PrevCounter = 0; // initilize the counter for the Body Frames
	//Body* TempUB = new Body;
//	Body = *TempUB;

//}
// Some code is taken from Kinect.h for compatability

//
//#ifndef _JointType
//#define _JointType_
//typedef enum _BJointType BJointType;
//
//enum _BJointType
//{
//	JointType_SpineBase = 0,
//	JointType_SpineMid = 1,
//	JointType_Neck = 2,
//	JointType_Head = 3,
//	JointType_ShoulderLeft = 4,
//	JointType_ElbowLeft = 5,
//	JointType_WristLeft = 6,
//	JointType_HandLeft = 7,
//	JointType_ShoulderRight = 8,
//	JointType_ElbowRight = 9,
//	JointType_WristRight = 10,
//	JointType_HandRight = 11,
//	JointType_HipLeft = 12,
//	JointType_KneeLeft = 13,
//	JointType_AnkleLeft = 14,
//	JointType_FootLeft = 15,
//	JointType_HipRight = 16,
//	JointType_KneeRight = 17,
//	JointType_AnkleRight = 18,
//	JointType_FootRight = 19,
//	JointType_SpineShoulder = 20,
//	JointType_HandTipLeft = 21,
//	JointType_ThumbLeft = 22,
//	JointType_HandTipRight = 23,
//	JointType_ThumbRight = 24,
//	JointType_Count = (JointType_ThumbRight + 1)
//};
//#endif // _JointType_
//
//#ifndef _TrackingState_
//#define _TrackingState_
//typedef enum _TrackingState TrackingState;
//
//
//enum _TrackingState
//{
//	TrackingState_NotTracked = 0,
//	TrackingState_Inferred = 1,
//	TrackingState_Tracked = 2
//};
//#endif // _TrackingState_
//
//#ifndef _BVector4
//#define _BVector4_
//typedef struct _BVector4
//{
//	float x;
//	float y;
//	float z;
//	float w;
//} 	BVector4;
//
//#endif // _Vector4_
//
//#ifndef _Joint_
//#define _Joint_
//typedef struct _Joint
//{
//	JointType JointType;
//	CameraSpacePoint Position;
//	TrackingState TrackingState;
//} 	Joint;
//
//#endif // _Joint_
//
//#ifndef _JointOrientation_
//#define _JointOrientation
//typedef struct _JointOrientation
//{
//	JointType JointType;
//	Vector4 Orientation;
//} 	JointOrientation;
//
//#endif // _JointOrientation_
//
//#ifndef _JointDerivative_
//#define _JointDerivative
//typedef struct _JointDerivative
//{
//	JointType JointType;
//	BVector4 DerivativeVector;
//} 	JointDerivative;
//
//#endif // _JointDerivative_
//
//#ifndef _JointSpace_
//#define _JointSpace
//typedef struct _JointSpace
//{
//	JointType JointType;
//	Vector4 SpaceVector;
//} 	JointSpace;
//
//#endif // _JointSpace_
//
//#ifndef _Skeleton_
//#define _Skeleton
//typedef struct _Skeleton
//{
//	// Joint space for each bone
//	std::array<JointSpace, JointType_Count> JointsXY;
//	// Joint derivative for each bone
//	std::array<JointDerivative, JointType_Count> JointsD;
//
//} 	Skeleton;
//
//#endif // _Skeleton_
//
//#ifndef _Body_
//#define _Body
//typedef struct _Body
//{
//	// two frames of body. n and n-1
//	std::array<Skeleton, 2> BodyFrames;
//
//} Body;
//
//#endif // _Body_
//
//#ifndef _CameraSpacePoint_
//#define _CameraSpacePoint_
//typedef struct _CameraSpacePoint
//{
//	float X;
//	float Y;
//	float Z;
//} 	CameraSpacePoint;
//
//#endif // _CameraSpacePoint_
//
//#ifndef _HandState_
//#define _HandState_
//typedef enum _HandState HandState;
//
//
//enum _HandState
//{
//	HandState_Unknown = 0,
//	HandState_NotTracked = 1,
//	HandState_Open = 2,
//	HandState_Closed = 3,
//	HandState_Lasso = 4
//};
//#endif // _HandState_

//class OptiBody
//{// Variables
//	static const int        cDepthWidth = 512;
//	static const int        cDepthHeight = 424;
//public: // Public functions for BodyBasic.cpp to call
//		// CompDervative
//		// GetDerivative
//		//void OptiBody();
//		//Body getBody();
//		//void saveBody(Body B);
//	void saveJointSpace(int JointType, float X, float Y, float Z, int TrackingState);
//private:
//	// Timing Variables
//	HWND                    m_hWnd;
//	INT64                   m_nStartTime;
//	INT64                   m_nLastCounter;
//	double                  m_fFreq;
//	INT64                   m_nNextStatusTime;
//	DWORD                   m_nFramesSinceUpdate;
//	// OptiBody Variables
//	BOOLEAN					PrevCounter;
//	Body					ClassBody;
//	//~OptiBody();
//
//	//float CompJointDerivative();
//};

void OptiBody::saveJointVector(int JointType0, int JointType1, float X, float Y, float Z, int TrackingState)
{
	JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].TrackingState = TrackingState;
	JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].X = X;
	JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].Y = Y;
	JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].Z = Z;
	JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].R = sqrt(X*X+Y*Y+Z*Z);
	
	
};


void OptiBody::getJointSpace(void)
{
}
void OptiBody::incFrameCounter(void)
{
	FrameCounter = !FrameCounter;
}
void OptiBody::compJointDerivative(int JointType0, int JointType1)
{
	//JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].TrackingState = TrackingState;
	float dX, dY, dZ;
	dX = JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].X - JointVectorMap[!FrameCounter].JointMap[JointType0][JointType1].X;
	JointDerivativeMap.JointMap[JointType0][JointType1].X = dX;

	dY = JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].Y - JointVectorMap[!FrameCounter].JointMap[JointType0][JointType1].Y;
	JointDerivativeMap.JointMap[JointType0][JointType1].Y = dY;

	dZ = JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].Z - JointVectorMap[!FrameCounter].JointMap[JointType0][JointType1].Z;
	JointDerivativeMap.JointMap[JointType0][JointType1].Z = dZ;

	JointDerivativeMap.JointMap[JointType0][JointType1].R = sqrt(dX*dX + dY*dY + dZ*dZ);

	if (JointDerivativeMap.JointMap[JointType0][JointType1].R > .5  || abs(dX) > .3 || abs(dY) > .3 || abs(dZ) > .3 ) {
		Sleep(1);
	}
}
OptiBody::OptiBody()
{
	m_hWnd = NULL;
	m_nStartTime = 0;
    m_nLastCounter = 0;
    m_fFreq = 0;
    m_nNextStatusTime = 0;
    m_nFramesSinceUpdate = NULL;
	// OptiBody Variables
	FrameCounter = 0;
	//JointMap JointVectorMap = new JointMap;
};
OptiBody::~OptiBody()
{
};
//extern Body UserBody; //announce that an instance is available with external linkage// 
//void saveJointSpace(JointType JointType, float X, float Y, float Z, TrackingState TrackingState)
//{
//	UserBody.BodyFrames[0].JointsXY[JointType].SpaceVector.x = X;
//	UserBody.BodyFrames[0].JointsXY[JointType].JointType = JointType;
//}
