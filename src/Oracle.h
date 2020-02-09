#pragma once
#include "ofMain.h"
using namespace glm;

class Oracle
{
public:

	void setupOracle();
	void getLegData(float avgl[5][6], float avgr[5][6], float savgl[5][2], float savgr[5][2], float xyzavgl[6], float xyzavgr[6]);
	void getArmData();

	void test();
	Oracle();
	~Oracle();

	float lavg[5][6];
	float ravg[5][6];

	float lSavg[5][2];
	float rSavg[5][2];

	float lxyzavg[6];
	float rxyzavg[6];
};

