#pragma once
#include "ofMain.h"
#include "Rock.h"
#include <atomic>

using namespace std;
using namespace glm;

class Legs : public ofThread
{
public:
	void legsSetup(string com);
	void legsUpdate();
	void calcAvgs();
	void calcSavgs();
	void calcXYZavgs();
	void plotimu(float x, float y, int c, int h);
	void plotimu2(float x, float y);
	void plotimu3(float x, float y, bool exclu[5]);
	void plotAvgs();
	void plotAvg(float savg[5][2], float avg[5][6], float xyzavg[6], int index);
	void start();
	void stop();
	void threadedFunction();

	void check();

	//float *getAvgs();
	//float *getSavgs();
	//float *getXYZavgs();

	Legs();
	~Legs();

	//ofFile legData;
	Rock rock;
	bool rockUp = 0;
	ofSerial port;
	string coms;
	bool firstContact = false;
	unsigned char inBuffer[134];
	int off = 0;
	// current sensor values
	float imu[8][6];
	float magno[2][9];
	// sensor history
	int hcount = 0;
	static const int nframes = 30;
	static const int histlength = 20;
	float mhist[2][9][histlength];
	float ihist[8][6][histlength];
	float lhist[5][6][histlength];
	float rhist[5][6][histlength];
	float nimu[12][6][nframes];
	float nmag[3][9][nframes];
	//histlength/nframes window averages
	float lavg[5][6];
	float ravg[5][6];
	float avgs[2][5][6];

	float lSavg[5][2];
	float rSavg[5][2];
	float savgs[2][5][2];

	float lxyzavg[6];
	float rxyzavg[6];
	float xyzavgs[2][6];

	float magadj[3] = { 176, 176, 165 };
	float ypltscales[3] = { 16, 2000, 1000 };
	//magnetic calibration variables
	float magmin[3][3] = { { 60000, 60000, 60000 }, { 60000, 60000, 60000 }, { 60000, 60000, 60000 } };
	float magmax[3][3] = { { -60000, -60000, -60000 }, { -60000, -60000, -60000 }, { -60000, -60000, -60000 } };
	float magbias[3][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
	float magscale[3][3] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };
	float scalebias[3][3] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };
	float avgrad[3] = { 1, 1, 1 };
	//scalar multipliers for sensors
	const float ascale = .000488;
	const float gscale = .061068;
	const int negcheck = 32767;
	int ttime = 0;
	//thread stuff
	std::condition_variable condition;


};

               