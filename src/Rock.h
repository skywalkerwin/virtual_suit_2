#pragma once
#include "ofMain.h"
using namespace glm;

class Rock
{
public:
	
	void setupRock(vec3 v, float sz, int ind);
	void updateRock(int ind, float xv, float yv, float zv);
	int drawRock();
	void drawTrail();
	Rock();
	~Rock();

	ofSpherePrimitive rock;

	bool show = 0;
	//timeUp is how long the rock has been floating from state 2
	int timeUp = 0;
	int maxTimeUp = 600;

	int airTime = 0;
	//the peakTime is the time(frames) at which the rock is at its peak during launch state 3
	int peakTime = 0;

	//the ground time is how long the rock has been on the ground after state 3
	int groundTime = 0;
	int maxGroudTime = 80;

	float rockSize=40;
	float rockJump=1;
	int res = 8;
	//float rockRad = 200;
	float rockRotate = 0;

	float rockX = ofGetWidth() * 2;
	float rockY = ofGetHeight();
	float rockZ = 0;

	float rockDX = 0;
	float rockDY = 0;
	float rockDZ = 0;

	int xDir = 1;
	float xPull = 0;
	float xCenter = 0;
	const float GRAV = 10;



	int rockState = 0;
	//0 - inactive
	//1 - initial jump
	//2 = hovering
	//3 = launching
	//4 - dropping from hover
	//5 - hit target

	//vector<ofMeshFace> triangles;
	ofMaterial material;
	ofSpherePrimitive rockSphere;
	static const int trailCount = 4;
	ofMesh rockTrail[trailCount];
	int currentTrail = 0;
	ofColor tmpColor;
	vector<vec3> trailHistory;

	//ofSpherePrimitive wire;

};

