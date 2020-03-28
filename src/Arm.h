#pragma once
#include "ofMain.h"
#include <atomic>

using namespace std;
using namespace glm;

class Arm : public ofThread
{
public:
	void armSetup(string com, ofNode &par, int num);
	void armUpdate();
	void armDraw(vec3 b);
	void handtilt();
	void handDraw();
	void jointsDraw();

	void Madgwick6(int i, float ax, float ay, float az, float gx, float gy, float gz);
	void Madgwick9(int i, float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
	void sensorfusion();
	void testimus();
	void plotimu(vec3 pos, float x, float y, int c);

	void start();
	void stop();
	void threadedFunction();

	Arm();
	~Arm();
	//arm models
	bool drawBool = 1;
	float ulen;
	float llen;
	float hlen;
	ofBoxPrimitive upperArm;
	ofBoxPrimitive lowerArm;
	ofBoxPrimitive hand;
	//ofBoxPrimitive handtip;
	ofNode body;
	//current matrix stuff
	mat4 modelMatrix;
	//mat4 handpoint;
	int hcount = 0;
	static const int histlength = 60;
	static const int nframes = 1;
	static const int maxLines = 60;
	int curLine = 0;
	vec3 shoulderpos;
	vec3 elbowpos;
	vec3 wristpos;
	vec3 handpos;
	//vector<vec3> handpoint;
	vec3 handpoint[histlength];
	//orientations
	quat handquat;

	//vec3 handpoint[histlength] = vec3[histlength];

	// serial communication variables
	int dir;
	ofSerial port;
	string coms;
	bool firstContact = false;
	unsigned char inBuffer[122];
	int off = 0;
	int updated = 0;
	// current sensor values
	float imu[6][6];
	float magno[3][9];
	// sensor history
	float mhist[3][9][histlength];
	float ihist[5][6][histlength];
	float nimu[12][6][nframes];
	float nmag[3][9][nframes];
	float magadj[3] = { 176, 176, 165 };
	float ypltscales[3] = { 16, 2000, 1000 };
	//magnetic calibration variables
	float magmin[3][3] = { { 60000, 60000, 60000 }, { 60000, 60000, 60000 }, { 60000, 60000, 60000 } };
	float magmax[3][3] = { { -60000, -60000, -60000 }, { -60000, -60000, -60000 }, { -60000, -60000, -60000 } };
	float magbias[3][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
	float magscale[3][3] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };
	float scalebias[3][3] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };
	float avgrad[3] = { 1, 1, 1 };
	// touch values
	int ttime = 0;
	int switchbinary = 0;
	int thumbPressure = 0;
	int switches[4] = { 0, 0, 0, 0 };
	int minpress = 1000;
	int maxpress = 0;
	int rangepress = 0;
	float normpress = 0;
	float presst[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double totalPress = 0;
	// sensor fusion stuff...quaternions
	float deltat = 0;
	static constexpr float GyroMeasError = PI * (40.0 / 180.0); // gyroscope measurement error in rads/s (start at 40 deg/s)
	static constexpr float GyroMeasDrift = PI * (0.0 / 180.0); // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)
	const float beta = sqrt(3.0 / 4.0) * GyroMeasError; // compute beta
	const float zeta = sqrt(3.0 / 4.0) * GyroMeasDrift;
	float q[8][4];//quaternions
	//scalar multipliers for sensors
	const float ascale = .000488;
	const float gscale = .061068;
	const int negcheck = 32767;
	// roll pitch yaw
	float roll[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	float pitch[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	float yaw[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	//float mx = 0;
	//float my = 0;
	//float mz = 0;
	//thread stuff
	std::condition_variable condition;

private:


};

