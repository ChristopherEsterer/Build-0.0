#pragma once

#include "windows.h"
#include "stdafx.h"

//#include <array>
#ifndef _JointType_
#define _JointType_
typedef enum _JointType JointType;


enum _JointType
{
	JointType_SpineBase = 0,
	JointType_SpineMid = 1,
	JointType_Neck = 2,
	JointType_Head = 3,
	JointType_ShoulderLeft = 4,
	JointType_ElbowLeft = 5,
	JointType_WristLeft = 6,
	JointType_HandLeft = 7,
	JointType_ShoulderRight = 8,
	JointType_ElbowRight = 9,
	JointType_WristRight = 10,
	JointType_HandRight = 11,
	JointType_HipLeft = 12,
	JointType_KneeLeft = 13,
	JointType_AnkleLeft = 14,
	JointType_FootLeft = 15,
	JointType_HipRight = 16,
	JointType_KneeRight = 17,
	JointType_AnkleRight = 18,
	JointType_FootRight = 19,
	JointType_SpineShoulder = 20,
	JointType_HandTipLeft = 21,
	JointType_ThumbLeft = 22,
	JointType_HandTipRight = 23,
	JointType_ThumbRight = 24,
	JointType_Count = (JointType_ThumbRight + 1)
};
#endif // _JointType_

#ifndef _TrackingState_
#define _TrackingState_
typedef enum _TrackingState TrackingState;


enum _TrackingState
{
	TrackingState_NotTracked = 0,
	TrackingState_Inferred = 1,
	TrackingState_Tracked = 2
};
#endif // _TrackingState_

#ifndef _Vector4_
#define _Vector4
typedef struct _Vector4
{
	float x;
	float y;
	float z;
	float w;
} 	Vector4;

#endif // _Vector4_

#ifndef _JointOrientation_
#define _JointOrientation
typedef struct _JointOrientation
{
	JointType JointType;
	Vector4 Orientation;
} 	JointOrientation;

#endif // _JointOrientation_

#ifndef _JointDerivative_
#define _JointDerivative
typedef struct _JointDerivative
{
	JointType JointType;
	Vector4 DerivativeVector;
} ;

#endif // _JointDerivative_

#ifndef _JointSpace_
#define _JointSpace
typedef struct _JointSpace
{
	JointType JointType;
	Vector4 SpaceVector;
} ;

#endif // _JointSpace_

#ifndef _Skeleton_
#define _Skeleton


#endif // _Skeleton_

#ifndef _Body_
#define _Body
typedef struct _Body
{
	// two frames of body. n and n-1
	//static std::array<Skeleton, 2> BodyFrames;
	struct Skeleton
	{
		// Joint space for each bone
	//	std::array<JointSpace, JointType_Count> JointsXY;
		// Joint derivative for each bone
		//std::array<JointDerivative, JointType_Count> JointsD;
		//AJointsXY JointsXY;
		//AJointsD JointsD;

	
	
	} 	Skeleton[2] ;

	//Skeleton BodyFrames;
};

#endif // _Body_