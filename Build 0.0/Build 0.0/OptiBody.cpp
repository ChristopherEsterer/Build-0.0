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

// Some code is taken from Kinect.h for compatability

void OptiBody::saveJointVector(int JointType0, int JointType1, float X, float Y, float Z, int TrackingState, double time)
{
	JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].TrackingState = TrackingState;
	JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].X = X;
	JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].Y = Y;
	JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].Z = Z;
	JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].R = sqrt(X*X+Y*Y+Z*Z);
	JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].T = time;
		
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
	dX = (JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].X - JointVectorMap[!FrameCounter].JointMap[JointType0][JointType1].X)/interval;
	JointDerivativeMap.JointMap[JointType0][JointType1].X = dX;

	dY = (JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].Y - JointVectorMap[!FrameCounter].JointMap[JointType0][JointType1].Y)/interval;
	JointDerivativeMap.JointMap[JointType0][JointType1].Y = dY;

	dZ = (JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].Z - JointVectorMap[!FrameCounter].JointMap[JointType0][JointType1].Z)/interval;
	JointDerivativeMap.JointMap[JointType0][JointType1].Z = dZ;

	JointDerivativeMap.JointMap[JointType0][JointType1].R = sqrt(dX*dX + dY*dY + dZ*dZ);
	
	JointDerivativeMap.JointMap[JointType0][JointType1].T = JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].T;


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
}
double OptiBody::getJointVectorMapData(int JointType0, int JointType1, int Datatype)
{
	double TempData;
	switch (Datatype) {
	case 0:
		TempData = JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].X;
	case 1:
		TempData = JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].Y;
	case 2:
		TempData = JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].Z;
	case 3:
		TempData = JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].R;
	case 4:
		TempData = JointVectorMap[FrameCounter].JointMap[JointType0][JointType1].T;
	}
	return TempData;
}
double OptiBody::getJointDerivativeMapData(int JointType0, int JointType1, int Datatype)
{
	double TempData;
	switch (Datatype) {
	case 0:
		TempData = JointDerivativeMap.JointMap[JointType0][JointType1].X;
	case 1:
		TempData = JointDerivativeMap.JointMap[JointType0][JointType1].Y;
	case 2:
		TempData = JointDerivativeMap.JointMap[JointType0][JointType1].Z;
	case 3:
		TempData = JointDerivativeMap.JointMap[JointType0][JointType1].R;
	case 4:
		TempData = JointDerivativeMap.JointMap[JointType0][JointType1].T;
	}
	return TempData;
}
;
OptiBody::~OptiBody()
{
};

