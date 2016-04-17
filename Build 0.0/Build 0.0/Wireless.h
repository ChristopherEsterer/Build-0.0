#pragma once
class Wireless
{
public:
	Wireless();
	~Wireless();
private:
	// gets a line from  the file and saves to dataBuffer
	void GetLineEMG();
	void GetLineForce();
	void CompressNoise(double& emgData);

	double EMGNoiseMax = 227;
	double EMGNoiseMiddle = 227;
	double EMGNoiseMin = 227;

	double StartTime;
	bool FirstTime = true;

	std::array<std::deque<double>,2> dataBufferEMG; // [0] Time [1]EMG 
	std::array<std::deque<double>, 3> dataBufferForce; // [0] Time [1]ForceL [2]ForceR
	std::ifstream DatafileEmg;
	std::ifstream DatafileForce;
	std::string delimiter = ",";
	mutable std::mutex EMGDataMutex;
	mutable std::mutex ForceDataMutex;
	
	mutable std::mutex DataFlagEMGMutex;
	BOOL NewDataEMGFlag = FALSE;
	mutable std::mutex DataFlagForceMutex;
	BOOL NewDataForceFlag = FALSE;

public:
	void setNewDataFlagEMG(BOOL b);
	BOOL getNewDataFlagEMG(void);
	void setNewDataFlagForce(BOOL b);
	BOOL getNewDataFlagForce(void);
	// //getdata from buffer, C is which buffer
	double getData(int c);
	void RunWireless(void);
};
enum DataIterator
{
	EMGTime_itr = 0,
	EMG_itr = 1,
	ForceTime_itr = 2,
	ForceL_itr = 3,
	ForceR_itr = 4
};
