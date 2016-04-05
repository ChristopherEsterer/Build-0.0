#pragma once
class Wireless
{
public:
	Wireless();
	~Wireless();
private:
	// gets a line from  the file and saves to dataBuffer
	void GetLine();
	std::array<std::deque<double>,4> dataBuffer;
	std::ifstream Datafile;
	std::string delimiter = ",";
public:
	// //getdata from buffer, C is which buffer
	double getData(int c);
};

