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

void OptiBody::saveJointPoint(int JointType, float X, float Y, float Z, int TrackingState, double time)
{
	PointDataMutex.lock();
	Joint[FrameCounters[2]].JointArray[JointType].TrackingState = TrackingState;
	Joint[FrameCounters[2]].JointArray[JointType].X = X;
	Joint[FrameCounters[2]].JointArray[JointType].Y = Y;
	Joint[FrameCounters[2]].JointArray[JointType].Z = Z;
	Joint[FrameCounters[2]].JointArray[JointType].R = sqrt(X*X + Y*Y + Z*Z);
	Joint[FrameCounters[2]].JointArray[JointType].T = time;
	PointDataMutex.unlock();
}

void OptiBody::saveLimbVector(int JointType0, int JointType1, float X, float Y, float Z, int TrackingState, double time)
{
	LimbDataMutex.lock();
	Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].TrackingState = TrackingState;
	Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].X = X;
	Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].Y = Y;
	Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].Z = Z;
	Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].R = sqrt(X*X+Y*Y+Z*Z);
	Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].T = time;
	LimbDataMutex.unlock();
};

void OptiBody::getJointSpace(void)
{
}
void OptiBody::incFrameCounter(void)
{
	for (int i = 0; i < FrameCounters.size(); i++) {
		incFrameCounters(i);
	}
	//FrameCounter = !FrameCounter;
}
void OptiBody::incFrameCounters(int i)
{
	FrameCounters[i] = !FrameCounters[i];
}
void OptiBody::compDerivative(int JointType0, int JointType1, int c)
{// C is case for which dervative to calculate
 // 0 Limb1st, 1 Limb2nd
 // 2 Joint1st, 3 Joint2nd
// if Joint, only JointType0 is used.

double dX, dY, dZ;
	switch (c) {
	case 0:
		interval = (Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].T - Limb[!FrameCounters[0]].LimbMap[JointType0][JointType1].T);

		dX = ((Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].X - Limb[!FrameCounters[0]].LimbMap[JointType0][JointType1].X) / interval);
		Limb1Derivative[FrameCounters[1]].LimbMap[JointType0][JointType1].X = dX;
		dY = ((Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].Y - Limb[!FrameCounters[0]].LimbMap[JointType0][JointType1].Y) / interval);
		Limb1Derivative[FrameCounters[1]].LimbMap[JointType0][JointType1].Y = dY;
		dZ = ((Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].Z - Limb[!FrameCounters[0]].LimbMap[JointType0][JointType1].Z) / interval);
		Limb1Derivative[FrameCounters[1]].LimbMap[JointType0][JointType1].Z = dZ;
		Limb1Derivative[FrameCounters[1]].LimbMap[JointType0][JointType1].T = Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].T ;
		break;
	case 1:
		interval = (Limb1Derivative[FrameCounters[1]].LimbMap[JointType0][JointType1].T - Limb1Derivative[!FrameCounters[1]].LimbMap[JointType0][JointType1].T);

		dX = ((Limb1Derivative[FrameCounters[1]].LimbMap[JointType0][JointType1].X - Limb1Derivative[!FrameCounters[1]].LimbMap[JointType0][JointType1].X) / interval);
		Limb2Derivative.LimbMap[JointType0][JointType1].X = dX;
		dY = ((Limb1Derivative[FrameCounters[1]].LimbMap[JointType0][JointType1].Y - Limb1Derivative[!FrameCounters[1]].LimbMap[JointType0][JointType1].Y) / interval);
		Limb2Derivative.LimbMap[JointType0][JointType1].Y = dY;
		dZ = ((Limb1Derivative[FrameCounters[1]].LimbMap[JointType0][JointType1].Z - Limb1Derivative[!FrameCounters[1]].LimbMap[JointType0][JointType1].Z) / interval);
		Limb2Derivative.LimbMap[JointType0][JointType1].Z = dZ;
		Limb2Derivative.LimbMap[JointType0][JointType1].T = Limb1Derivative[FrameCounters[1]].LimbMap[JointType0][JointType1].T;
		break;
	case 2:
		interval = (Joint[FrameCounters[2]].JointArray[JointType0].T - Joint[(!FrameCounters[2])].JointArray[JointType0].T);

		
		dX = ((Joint[FrameCounters[2]].JointArray[JointType0].X - Joint[(!FrameCounters[2])].JointArray[JointType0].X) / interval);
		Joint1Derivative[FrameCounters[3]].JointArray[JointType0].X = dX;
		dY = ((Joint[FrameCounters[2]].JointArray[JointType0].Y - Joint[(!FrameCounters[2])].JointArray[JointType0].Y) / interval);
		Joint1Derivative[FrameCounters[3]].JointArray[JointType0].Y = dY;
		dZ = ((Joint[FrameCounters[2]].JointArray[JointType0].Z - Joint[(!FrameCounters[2])].JointArray[JointType0].Z) / interval);
		Joint1Derivative[FrameCounters[3]].JointArray[JointType0].Z = dZ;
		Joint1Derivative[FrameCounters[3]].JointArray[JointType0].T = Joint[(FrameCounters[2])].JointArray[JointType0].T;
		break;
	case 3:
		interval = (Joint1Derivative[FrameCounters[3]].JointArray[JointType0].T - Joint1Derivative[!FrameCounters[3]].JointArray[JointType0].T);

	
		dX = ((Joint1Derivative[FrameCounters[3]].JointArray[JointType0].X - Joint1Derivative[!FrameCounters[3]].JointArray[JointType0].X) / interval);
		Joint2Derivative.JointArray[JointType0].X = dX;
		dY = ((Joint1Derivative[FrameCounters[3]].JointArray[JointType0].Y - Joint1Derivative[!FrameCounters[3]].JointArray[JointType0].Y) / interval);
		Joint2Derivative.JointArray[JointType0].Y = dY;
		dZ = ((Joint1Derivative[FrameCounters[3]].JointArray[JointType0].Z - Joint1Derivative[!FrameCounters[3]].JointArray[JointType0].Z) / interval);
		Joint2Derivative.JointArray[JointType0].Z = dZ;
		Joint2Derivative.JointArray[JointType0].T = Joint[FrameCounters[3]].JointArray[JointType0].T;
		break;
	}
	return;
}
float OptiBody::calculateAngle(double X1, double Y1, double Z1, double R1, double X2, double Y2, double Z2, double R2)
{
	double Dot = (X1*X2 + Y1*Y2 + Z1*Z2);
	float R = (Dot / (R1 * R2));
	float angle = ((180 / 3.14159265359)*acos(R));
	return (float) angle;
};
OptiBody::OptiBody()
{
	m_hWnd = NULL;
	m_nStartTime = 0;
    m_nLastCounter = 0;
    m_fFreq = 0;
    m_nNextStatusTime = 0;
    m_nFramesSinceUpdate = NULL;
	// OptiBody Variables
	//FrameCounters = NULL;
	//JointMap JointVectorMap = new JointMap;
}
double OptiBody::getData(int JointType0, int JointType1, int Datatype)
{// 3 digit dataType case:
//	first digit Limb, Joint, or angle {0,1,2}
//	Second Digit F,F',F" {0,1,2}
//	X,Y,Z,R,T {0,1,2,3,4}.
	

	double TempData;
	switch (Datatype) {
// Limbs
	case 000:
		LimbDataMutex.lock();
		TempData = Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].X;
		LimbDataMutex.unlock();
		break;
	case 001:
		LimbDataMutex.lock();
		TempData = Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].Y;
		LimbDataMutex.unlock();
		break;
	case 002:
		LimbDataMutex.lock();
		TempData = Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].Z;
		LimbDataMutex.unlock();
		break;
	case 003:
		LimbDataMutex.lock();
		TempData = Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].R;
		LimbDataMutex.unlock();
		break;
	case 004:
		LimbDataMutex.lock();
		TempData = Limb[FrameCounters[0]].LimbMap[JointType0][JointType1].T;
		LimbDataMutex.unlock();
		break;
	case 010:
		LimbDataMutex.lock();
		TempData = Limb1Derivative[FrameCounters[0]].LimbMap[JointType0][JointType1].X;
		LimbDataMutex.unlock();
		break;
	case 011:
		LimbDataMutex.lock();
		TempData = Limb1Derivative[FrameCounters[0]].LimbMap[JointType0][JointType1].Y;
		LimbDataMutex.unlock();
		break;
	case 012:
		LimbDataMutex.lock();
		TempData = Limb1Derivative[FrameCounters[0]].LimbMap[JointType0][JointType1].Z;
		LimbDataMutex.unlock();
		break;
	case 013:
		LimbDataMutex.lock();
		TempData = Limb1Derivative[FrameCounters[0]].LimbMap[JointType0][JointType1].R;
		LimbDataMutex.unlock();
		break;
	case 014:
		LimbDataMutex.lock();
		TempData = Limb1Derivative[FrameCounters[0]].LimbMap[JointType0][JointType1].T;
		LimbDataMutex.unlock();
		break;
	case 020:
		LimbDataMutex.lock();
		TempData = Limb2Derivative.LimbMap[JointType0][JointType1].X;
		LimbDataMutex.unlock();
		break;
	case 021:
		LimbDataMutex.lock();
		TempData = Limb2Derivative.LimbMap[JointType0][JointType1].Y;
		LimbDataMutex.unlock();
		break;
	case 022:
		LimbDataMutex.lock();
		TempData = Limb2Derivative.LimbMap[JointType0][JointType1].Z;
		LimbDataMutex.unlock();
		break;
	case 023:
		LimbDataMutex.lock();
		TempData = Limb2Derivative.LimbMap[JointType0][JointType1].R;
		LimbDataMutex.unlock();
		break;
	case 024:
		LimbDataMutex.lock();
		TempData = Limb2Derivative.LimbMap[JointType0][JointType1].T;
		LimbDataMutex.unlock();
		break;
// Joints
	case 100:
		PointDataMutex.lock();
		TempData = Joint[FrameCounters[2]].JointArray[JointType0].X;
		PointDataMutex.unlock();
		break;
	case 101:
		PointDataMutex.lock();
		TempData = Joint[FrameCounters[2]].JointArray[JointType0].Y;
		PointDataMutex.unlock();
		break;
	case 102:
		PointDataMutex.lock();
		TempData = Joint[FrameCounters[2]].JointArray[JointType0].Z;
		PointDataMutex.unlock();
		break;
	case 103:
		PointDataMutex.lock();
		TempData = Joint[FrameCounters[2]].JointArray[JointType0].R;
		PointDataMutex.unlock();
		break;
	case 104:
		PointDataMutex.lock();
		TempData = Joint[FrameCounters[2]].JointArray[JointType0].T;
		PointDataMutex.unlock();
		break;
	case 110:
		PointDataMutex.lock();
		TempData = Joint1Derivative[FrameCounters[3]].JointArray[JointType0].X;
		PointDataMutex.unlock();
		break;
	case 111:
		PointDataMutex.lock();
		TempData = Joint1Derivative[FrameCounters[3]].JointArray[JointType0].Y;
		PointDataMutex.unlock();
		break;
	case 112:
		PointDataMutex.lock();
		TempData = Joint1Derivative[FrameCounters[3]].JointArray[JointType0].Z;
		PointDataMutex.unlock();
		break;
	case 113:
		PointDataMutex.lock();
		TempData = Joint1Derivative[FrameCounters[3]].JointArray[JointType0].R;
		PointDataMutex.unlock();
		break;
	case 114:
		PointDataMutex.lock();
		TempData = Joint1Derivative[FrameCounters[3]].JointArray[JointType0].T;
		PointDataMutex.unlock();
		break;
	case 120:
		PointDataMutex.lock();
		TempData = Joint2Derivative.JointArray[JointType0].X;
		PointDataMutex.unlock();
		break;
	case 121:
		PointDataMutex.lock();
		TempData = Joint2Derivative.JointArray[JointType0].Y;
		PointDataMutex.unlock();
		break;
	case 122:
		PointDataMutex.lock();
		TempData = Joint2Derivative.JointArray[JointType0].Z;
		PointDataMutex.unlock();
		break;
	case 123:
		PointDataMutex.lock();
		TempData = Joint2Derivative.JointArray[JointType0].R;
		PointDataMutex.unlock();
		break;
	case 124:
		PointDataMutex.lock();
		TempData = Joint2Derivative.JointArray[JointType0].T;
		PointDataMutex.unlock();
		break;
	case 200:
		PointDataMutex.lock();
		TempData = JointAngles[JointType0];
		PointDataMutex.unlock();
		break;
	case 204:
		PointDataMutex.lock();
		TempData = Joint[FrameCounters[2]].JointArray[JointType0].T;
		PointDataMutex.unlock();
		break;
	}
	return TempData;
}
double OptiBody::getJointData(int JointType0, int Datatype)
{
	double TempData;/*
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
	}*/
	return TempData;
}
void OptiBody::postToGui(void)
{

}
HWND OptiBody::getHWnd(void)
{
	return (HWND)m_hWnd;
}
void OptiBody::setHWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}
void OptiBody::setNewDataFlag(BOOL b)
{
	DataFlagMutex.lock();
	NewDataFlag = b;
	DataFlagMutex.unlock();
}
BOOL OptiBody::getNewDataFlag(void)
{
	BOOL b;
	DataFlagMutex.lock();
	b = NewDataFlag;
	DataFlagMutex.unlock();
	return b;
}
;
void OptiBody::compJointAngle(int joint0)
{
	int i1, j1, i2, j2;
	switch (joint0) {
	case JointType_SpineMid:
		i1 = JointType_SpineShoulder;
		j1 = JointType_SpineMid;
		i2 = JointType_SpineMid;
		j2 = JointType_SpineBase;
		break;
	}
	float tempAngle = calculateAngle(
		Limb[FrameCounters[0]].LimbMap[i1][j1].X,
		Limb[FrameCounters[0]].LimbMap[i1][j1].Y,
		Limb[FrameCounters[0]].LimbMap[i1][j1].Z,
		Limb[FrameCounters[0]].LimbMap[i1][j1].R,
		Limb[FrameCounters[0]].LimbMap[i2][j2].X,
		Limb[FrameCounters[0]].LimbMap[i2][j2].Y,
		Limb[FrameCounters[0]].LimbMap[i2][j2].Z,
		Limb[FrameCounters[0]].LimbMap[i2][j2].R
		);
	JointAngles[joint0] = tempAngle;
}
OptiBody::~OptiBody()
{
};

