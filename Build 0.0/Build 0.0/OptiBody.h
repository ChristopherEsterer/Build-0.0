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

	
	
	void compJointAngle(int jointA0);
	
	~OptiBody();
	OptiBody();
	double interval;

	double getData(int JointType0, int JointType1, int Datatype);
	double getJointData(int JointType, int Datatype); // remove?
	
	void postToGui(void);
	
	std::vector<double> times;
	HWND getHWnd(void);
	void setHWnd(HWND hWnd);
	void setNewDataFlag(BOOL b);
	BOOL getNewDataFlag(void);
private:

	// Timing Variables
	HWND                    m_hWnd;
	INT64                   m_nStartTime;
	INT64                   m_nLastCounter;
	double                  m_fFreq;
	INT64                   m_nNextStatusTime;
	DWORD                   m_nFramesSinceUpdate;
	BOOL NewDataFlag = FALSE;
	int misdatactr = 0;

	mutable std::mutex DataFlagMutex;
	mutable std::mutex PointDataMutex;
	mutable std::mutex LimbDataMutex;
	//mutable std::mutex EMGDataMutex;
	//mutable std::mutex ForceDataMutex;
	// OptiBody Variables
	std::array<BOOLEAN,6>	FrameCounters; // Each bit n or n+1 for frames

	void calculateAngle(double X1, double Y1, double Z1, double R1, double X2, double Y2, double Z2, double R2);
	void calculateAngle(int i1, int j1, int i2 , int j2, int C);
	//Limb [0]
	//Limb1Derivative [1]
	//Joint [2]
	//Joint1Derivative [3]
	//Angle [4]
	//Angle1Derivative [5]

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
			float interval;
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
			float interval; // *** implement
			double T; // TotalTime
		}JointPoint;
		std::array<JointPoint, 26> JointArray;
	}JointArray;
	std::array<JointArray, 2> Joint;// two frames n-1 and n
	std::array<JointArray, 2> Joint1Derivative;// two frames n-1 and n
	JointArray Joint2Derivative;// n of second derivative
	
	typedef struct AngleArray_
	{
		typedef struct AngleVector_
		{
			int TrackingState;
			float theta;
			float azimuth;
			float R; // vector lenght
			float interval;
			double T; // TotalTime
		}AngleVector;
		std::array<AngleVector, 26> AngleArray;
	}AngleArray;
	std::array<AngleArray, 2> Angle;
	std::array<AngleArray, 2> Angle1Derivative;
	AngleArray Angle2Derivative;// n of second derivative
};