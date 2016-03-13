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
	void saveJointVector(int JointType0, int JointType1, float X, float Y, float Z, int TrackingState, double time);
	void getJointSpace(void);
	void incFrameCounter(void);
	void compJointDerivative(int JointType0, int JointType1);
	~OptiBody();
	OptiBody();
	double interval;

	double getJointVectorMapData(int JointType0, int JointType1, int Datatype);
	double getJointDerivativeMapData(int JointType0, int JointType1, int Datatype);
private:
	// Timing Variables
	HWND                    m_hWnd;
	INT64                   m_nStartTime;
	INT64                   m_nLastCounter;
	double                  m_fFreq;
	INT64                   m_nNextStatusTime;
	DWORD                   m_nFramesSinceUpdate;
	// OptiBody Variables
	BOOLEAN					FrameCounter;
	//Body*					ClassBody;
//	Body ClassBody;
	//float CompJointDerivative();
	typedef struct JointMap_
	{
		typedef struct JointVector_
		{
			int TrackingState;
			float X;
			float Y;
			float Z;
			float R; // vector lenght
			double T; // TotalTime
		}JointVector;
		std::array<std::array <JointVector, 26>, 26> JointMap; // 26X26 matrix
	}JointMap;
	std::array<JointMap,2> JointVectorMap;
	JointMap JointDerivativeMap;
};