#include "stdafx.h"
#include "Wireless.h"

void executeBatch(void) {
	system("RunWireless.bat");
}

Wireless::Wireless()
{
}


Wireless::~Wireless()
{
}


// gets a line from  the file and saves to dataBuffer
void Wireless::GetLineEMG()
{
	do {
		DatafileEmg.open("EMG_Datafile.txt", std::ifstream::in);
	} while (!DatafileEmg.is_open());
	
	if(DatafileEmg.is_open()){
		std::string line;
		while (std::getline(DatafileEmg, line))
		{
				double ttime, temg;
				size_t pos = 0;
				

			//	std::getline(DatafileEmg, line);
				//DatafileEmg.close();
				pos = line.find(delimiter);
				ttime = std::stod(line.substr(0, pos));
				line.erase(0, pos + delimiter.length());

				pos = line.find(delimiter);
				temg = std::stod(line.substr(0, pos));
				line.erase(0, pos + delimiter.length());
			
				CompressNoise(temg);

				if (FirstTime) { StartTime = ttime; FirstTime = false; }
				if (ttime >= StartTime) {
					EMGDataMutex.lock();
					dataBufferEMG[0].push_back(ttime);
					dataBufferEMG[1].push_back(temg);
					EMGDataMutex.unlock();
					setNewDataFlagEMG(TRUE);
				}
			}
	
	DatafileEmg.close();
	}
//	DatafileEmg.close();
	return;
}

void Wireless::GetLineForce()
{
	do {
		DatafileForce.open("Force_Datafile.txt", std::ifstream::in);
	} while (!DatafileForce.is_open());

	if (DatafileForce.is_open()) {
		std::string line1;
		std::string line2;
		while (std::getline(DatafileForce, line1) && std::getline(DatafileForce, line2)) {
			double ttime, tfL, tfR;
			size_t pos = 0;
			size_t Lpos = -1;
			Lpos = line1.find(".");
			if (!Lpos) {
				line1.erase(0, Lpos + 1);
				pos = line1.find(delimiter);

				ttime = std::stod(line1.substr(0, pos));
				line1.erase(0, pos + delimiter.length());
				pos = line1.find(delimiter);
				tfL = std::stod(line1.substr(0, pos));
				line1.erase(0, pos + delimiter.length());

			}
			else {
				pos = line1.find(delimiter);
				line1.erase(0, pos + delimiter.length());
				pos = line1.find(delimiter);
				tfR = std::stod(line1.substr(0, pos));
				line1.erase(0, pos + delimiter.length());

			}

			Lpos = -1;
			Lpos = line2.find(".");
			if (!Lpos) {
				line2.erase(0, Lpos + 1);
				pos = line1.find(delimiter);

				ttime = std::stod(line2.substr(0, pos));
				line2.erase(0, pos + delimiter.length());
				pos = line2.find(delimiter);
				tfL = std::stod(line2.substr(0, pos));
				line2.erase(0, pos + delimiter.length());
			}
			else {
				pos = line2.find(delimiter);
				line2.erase(0, pos + delimiter.length());
				pos = line2.find(delimiter);
				tfR = std::stod(line2.substr(0, pos));
				line2.erase(0, pos + delimiter.length());
			}

			//std::getline(DatafileForce, line1);
			//std::getline(DatafileForce, line2);

			//		DatafileForce.close();

			/*pos = line1.find(delimiter);

			ttime = std::stod(line1.substr(0, pos));
			line1.erase(0, pos + delimiter.length());
			pos = line1.find(delimiter);
			tfL = std::stod(line1.substr(0, pos));
			line1.erase(0, pos + delimiter.length());

			pos = line2.find(delimiter);
			line2.erase(0, pos + delimiter.length());
			pos = line2.find(delimiter);
			tfR = std::stod(line2.substr(0, pos));
			line2.erase(0, pos + delimiter.length()); */
			if (FirstTimeForce) { StartTimeForce = ttime; FirstTimeForce = false;
			}
			if (ttime >= StartTimeForce) {
				ForceDataMutex.lock();
				dataBufferForce[0].push_back(ttime);
				dataBufferForce[1].push_back(tfL);
				dataBufferForce[2].push_back(tfR);
				setNewDataFlagForce(TRUE);
				ForceDataMutex.unlock();
			}
		}
		DatafileForce.close();
		
	}
//	DatafileForce.close();
	return;
}

void Wireless::CompressNoise(double & emgData)
{
	if (emgData < EMGNoiseMax && emgData > EMGNoiseMin) {
		emgData = EMGNoiseMiddle;
	}
}


// //getdata from buffer, C is which buffer
double Wireless::getData(int c)
{
	double td;
	switch (c) {
	case EMGTime_itr:
		EMGDataMutex.lock();
		td = dataBufferEMG[0].front();
		dataBufferEMG[0].pop_front();
		EMGDataMutex.unlock();
		break;
	case EMG_itr:
		EMGDataMutex.lock();
		td = dataBufferEMG[1].front();
		dataBufferEMG[1].pop_front();
		EMGDataMutex.unlock();
		break;
	case ForceTime_itr:
		ForceDataMutex.lock();
		td = dataBufferForce[0].front();
		dataBufferForce[0].pop_front();
		ForceDataMutex.unlock();
		break;
	case ForceL_itr:
		ForceDataMutex.lock();
		td = dataBufferForce[1].front();
		dataBufferForce[1].pop_front();
		ForceDataMutex.unlock();
		break;
	case ForceR_itr:
		ForceDataMutex.lock();
		td = dataBufferForce[2].front();
		dataBufferForce[2].pop_front();
		ForceDataMutex.unlock();
		break;
	};

	return td;
}

void Wireless::RunWireless(void) // Runs on a thread Populating dataBuffer;
{
	Sleep(2000);
	executeBatch();
	Sleep(1000);
	DatafileForce.open("Force_Datafile.txt", std::ifstream::in);
	DatafileEmg.open("EMG_Datafile.txt", std::ifstream::in);
	while (true) {
		GetLineEMG();
		GetLineForce();
		if (DatafileEmg.eof()) {
			remove("EMG_Datafile.txt");
		}
		if (DatafileForce.eof()) {
			remove("Force_Datafile.txt");
		}
	}
}
void Wireless::setNewDataFlagEMG(BOOL b)
{
	DataFlagEMGMutex.lock();
	NewDataEMGFlag = b;
	DataFlagEMGMutex.unlock();
}
BOOL Wireless::getNewDataFlagEMG(void)
{
	BOOL b;
	DataFlagEMGMutex.lock();
	b = NewDataEMGFlag;
	if (!dataBufferEMG[0].empty() && !dataBufferEMG[1].empty()) { b = TRUE; }
	else if (dataBufferEMG[0].empty() || dataBufferEMG[1].empty()) { b = FALSE; }
	DataFlagEMGMutex.unlock();
	return b;
}
;
void Wireless::setNewDataFlagForce(BOOL b)
{
	DataFlagForceMutex.lock();
	NewDataForceFlag = b;
	DataFlagForceMutex.unlock();
}
BOOL Wireless::getNewDataFlagForce(void)
{
	BOOL b;
	DataFlagForceMutex.lock();
	b = NewDataForceFlag;
	if (!dataBufferForce[0].empty() && !dataBufferForce[1].empty()) { b = TRUE; }
	else if (dataBufferForce[0].empty() || dataBufferForce[1].empty()) { b = FALSE; }
	DataFlagForceMutex.unlock();
	return b;
}

;