#include "stdafx.h"
#include "Wireless.h"


Wireless::Wireless()
{
}


Wireless::~Wireless()
{
}


// gets a line from  the file and saves to dataBuffer
void Wireless::GetLine()
{
	do {
		Datafile.open("Datafile.txt", std::ifstream::in);
	} while (!Datafile.is_open());
	
	if(Datafile.is_open()){
		double ttime, temg, tfL, tfr;
		size_t pos = 0;
		std::string line;
		std::string SS;
		
		std::getline(Datafile,line);
	Datafile.close();
		pos = SS.find(delimiter);
		ttime =	std::stod(SS.substr(0, pos));
		SS.erase(0, pos + delimiter.length());

		pos = SS.find(delimiter);
		temg = std::stod(SS.substr(0, pos));
		SS.erase(0, pos + delimiter.length());
		
		pos = SS.find(delimiter);
		tfL = std::stod(SS.substr(0, pos));
		SS.erase(0, pos + delimiter.length());
		
		pos = SS.find(delimiter);
		tfr = std::stod(SS.substr(0, pos));
		SS.erase(0, pos + delimiter.length());
		
		dataBuffer[0].push_back(ttime);
		dataBuffer[1].push_back(temg);
		dataBuffer[2].push_back(tfL);
		dataBuffer[3].push_back(tfr);
	//Datafile.close();
	}
	return;
}

// //getdata from buffer, C is which buffer
double Wireless::getData(int c)
{
	double td;
	switch (c) {
	case 0:
		td = dataBuffer[0].back();
		return td;
	case 1:
		td = dataBuffer[0].back();
		return td;
	};
}
