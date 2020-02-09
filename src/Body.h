#pragma once
#include "ofMain.h"
#include "Arm.h"
#include "Legs.h"
using namespace std;


class Body
{
public:
	Body();
	void bodySetup();
	void bodyUpdate(vec3 v);
	void legsSetup();
	void armsSetup();
	void bodyDraw();
	void torsoDraw();

	void drawHandPlots();
	void testimus();

	void checkMoves();


	~Body();

	string leftcom = "COM3";
	string rightcom = "COM4";
	string legComs = "COM5";

	Arm larm , rarm;
	Legs legs;

	//Rock rock;
	//bool rockUp = 0;

	ofBoxPrimitive torso;
	//LightCycle* lcycle;
	vec3 torsopos;
	static const int maxLines = 180;
	int hcount = 0;
	static const int histlength = 120;
	float ihist[5][6][histlength];
	int curLine = 0;
	//vec3 neckpos;


	float lval[6], rval[6];

};

