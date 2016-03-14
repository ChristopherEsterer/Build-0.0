#pragma once
#include "windows.h"
#include "stdafx.h"
//#include "OptiBodyStructs.cpp"
#include <array>
// Some code is taken from Kinect.h for compatability
//struct JointMap;
class OptiBody
{// Variables
//static const int        cDepthWidth = 512;
//	static const int        cDepthHeight = 424;
public: // Public functions for BodyBasic.cpp to call
		// CompDervative
		// GetDerivative
	//void OptiBody();
	//Body getBody();
	//void saveBody(Body B);
	void saveJointPoint(int JointType, float X, float Y, float Z, int TrackingState, double time);
	void saveLimbVector(int JointType0, int JointType1, float X, float Y, float Z, int TrackingState, double time);
	void getJointSpace(void);
	void incFrameCounter(void);
	void incFrameCounters(int i);
	void compDerivative(int JointType0, int JointType1, int c);
	~OptiBody();
	OptiBody();
	double interval;

	double getData(int JointType0, int JointType1, int Datatype);
	double getJointData(int JointType, int Datatype); // remove?
private:
	// Timing Variables
	HWND                    m_hWnd;
	INT64                   m_nStartTime;
	INT64                   m_nLastCounter;
	double                  m_fFreq;
	INT64                   m_nNextStatusTime;
	DWORD                   m_nFramesSinceUpdate;
	// OptiBody Variables
	std::array<BOOLEAN,4>	FrameCounters; // Each bit n or n+1 for frames
	//Limb [0]
	//Limb1Derivative [1]
	//Joint [2]
	//Joint1Derivative [3]

	// Joint Points and Limb vector Data Structures
	typedef struct LimbMap_
	{
		typedef struct LimbVector_
		{
			int TrackingState;
			float X;
			float Y;
			float Z;
			float R; // vector lenght
			double T; // TotalTime
		}LimbVector;
		std::array<std::array <LimbVector, 26>, 26> LimbMap; // 26X26 matrix
	}LimbMap;
	std::array<LimbMap,2> Limb; // two frames n-1 and n
	std::array<LimbMap, 2> Limb1Derivative; // two frames n-1 and n
	LimbMap Limb2Derivative; // n of second derivative
	
	typedef struct PointArray_
	{
		typedef struct JointPoint_
		{
			int TrackingState;
			float X;
			float Y;
			float Z;
			float R; // vector lenght
			double T; // TotalTime
		}JointPoint;
		std::array<JointPoint, 26> JointArray;
	}JointArray;
	std::array<JointArray, 2> Joint;// two frames n-1 and n
	std::array<JointArray, 2> Joint1Derivative;// two frames n-1 and n
	JointArray Joint2Derivative;// n of second derivative
	
};