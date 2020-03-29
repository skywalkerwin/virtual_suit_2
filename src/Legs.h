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
	void plotAvg(float savg[5][2], float avg[5][6], float xyzavg[6], float cur[5][6], int index);
	void start();
	void stop();
	void threadedFunction();

	int rockCheck1();
	int rockCheck2();

	Legs();
	~Legs();

	//ofFile legData;
	//Rock rock;
	//bool rockUp = 0;
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
	static const int histlength = 30;
	float mhist[2][9][histlength];
	float ihist[8][6][histlength];
	float lhist[5][6][histlength];
	float rhist[5][6][histlength];
	float nimu[12][6][nframes];
	float nmag[3][9][nframes];

	//current values for each leg
	float leftCur[5][6];
	float rightCur[5][6];
	float legCur[2][5][6];

	//histlength/nframes window averages
	//--------------------------------------------------------
	//the average x/y/z values of each sensor over some window accel=012 gyro=345
	float lavg[5][6];
	float ravg[5][6];
	float avgs[2][5][6];

	//the average values of both ACCEL/GYRO for each sensor over some window accel=0 gyro=1
	float lSavg[5][2];
	float rSavg[5][2];
	float savgs[2][5][2];

	//the average values of all individual axis readings on each sensor -- calculates using lavg/ravg/avgs
	//so the average x-accel from all sensors, or the average y-gyro from all sensors
	float lxyzavg[6];
	float rxyzavg[6];
	float xyzavgs[2][6];
	//--------------------------------------------------------

	float magadj[3] = { 176, 176, 165 };
	float ypltscales[3] = { 16, 2000, 1000 };
	//magnetic calibration variables
	float magmin[3][3] = { { 60000, 60000, 60000 }, { 60000, 60000, 60000 }, { 60000, 60000, 60000 } };
	float magmax[3][3] = { { -60000, -60000, -60000 }, { -60000, -60000, -60000 }, { -60000, -60000, -60000 } };
	float magbias[3][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
	float magscale[3][3] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };
	float scalebias[3][3] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };
	float avgrad[3] = { 1, 1, 1 };
	// sensor fusion stuff...quaternions
	float deltat = 0;
	static constexpr float GyroMeasError = PI * (50.0 / 180.0); // gyroscope measurement error in rads/s (start at 40 deg/s)
	static constexpr float GyroMeasDrift = PI * (10.0 / 180.0); // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)
	const float beta = sqrt(3.0 / 4.0) * GyroMeasError; // compute beta
	const float zeta = sqrt(3.0 / 4.0) * GyroMeasDrift;
	float q[8][4];//quaternions
	//scalar multipliers for sensors
	const float ascale = .000488;
	const float gscale = .061068;
	const int negcheck = 32767;
	int ttime = 0;
	//thread stuff
	std::condition_variable condition;


};

               