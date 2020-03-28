#include "Rock.h"

void Rock::setupRock(vec3 v, float sz, int ind) {
	trailHistory.clear();
	currentTrail += 1;
	if (currentTrail == trailCount) {
		currentTrail = 0;
		rockTrail[currentTrail].clearVertices();
	}
	tmpColor = ofColor(ofRandom(255), ofRandom(255), ofRandom(255), 150);
	rockTrail[currentTrail].setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	rockSize = sz/1.5;
	rockJump = sz / 8;
	show = 1;
	rockState = 1;
	rockSphere.setResolution(res);
	rockSphere.setRadius(rockSize);
	rockSphere.setGlobalPosition(v);
	if (ind == 0) xDir = 1;
	else xDir = -1;
	rockX = rockSphere.getX() - xDir * 300 ;
	//rockY = rockSphere.getY() + 700;
	rockY = 3 * ofGetHeight() / 4;
	rockZ = rockSphere.getZ() - 300;
	xPull = 20;

}

void Rock::updateRock(int ind, float xv, float yv, float zv) {
	if(ind==0) xDir=1;
	else xDir = -1;

	xPull *= xDir;
	rockState = 3;
	xCenter = rockX;
	rockDX = xv*30*xDir;
	rockDY = yv*50;
	rockDZ = - zv*40;
}

int Rock::drawRock() {
	//EARTH BOULDER-------------------------------
	if (rockState != 0) {
		if (rockState == 1) {
			rockY -= rockJump;
			if (rockY <= ofGetHeight() / 2) {
				rockState = 2;
			}
		}
		if (rockState == 2) {
			//at this state, rock needs to either listen for launch commands from legs, or wait to time out and vanish.
			timeUp += 1;
			if (rockState != 3 && timeUp >= maxTimeUp) {
				rockState = 4;
			}
		}
		if (rockState == 3) {
			//trailHistory.push_back(rockSphere.getGlobalPosition());
			//rockTrail[currentTrail].addColor(tmpColor);
			//rockTrail[currentTrail].addVertex(rockSphere.getGlobalPosition());
			//rockTrail[currentTrail].addColor(tmpColor);
			//rockTrail[currentTrail].addVertex(rockSphere.getGlobalPosition() + vec3(0, 3 * ofGetHeight() / 4 - rockY, 0));
			//rockTrail[currentTrail].draw();
			if (rockY <= ofGetHeight() + rockSize / 2) {
				airTime += 1;
				rockX = ofGetWidth()/2-xDir * 300 - (rockDX * airTime) + (.5 * xPull*airTime*airTime);
				//rockY = ofGetHeight()/2 + 700 - (rockDY * airTime) + (.5 * GRAV*airTime*airTime);
				rockY = 3*ofGetHeight()/4 - (rockDY * airTime) + (.5 * GRAV*airTime*airTime);
				rockZ = -200 + rockDZ * airTime;
			}
			if (rockY >= ofGetHeight() +rockSize/2 || airTime >= 300) {
				groundTime += 1;
				if (groundTime >= maxGroudTime) {
					//vanish and reset rock state
					timeUp = 0;
					airTime = 0;
					groundTime = 0;
					show = 0;
					rockState = 0;
					rockX = ofGetWidth() * 2;
					rockY = ofGetHeight();
					rockZ = 0;
					return 0;
				}
			}

		}
		if (rockState == 4) {
			//reset rock State to initial settings after drop, and return 0
			timeUp += 1;
			rockY = rockY + GRAV*(timeUp-maxTimeUp);
			if (rockY >= ofGetHeight()) {
				timeUp = 0;
				show = 0;
				rockState = 0;
				rockY = ofGetHeight();
				return 0;
			}
		}
		if (rockState == 5) {
			//hit target, reset rock state
			timeUp = 0;
			show = 0;
			rockState = 0;
			rockY = ofGetHeight();
			return 0;
		}
		ofPushMatrix();
		ofEnableDepthTest();
		rockSphere.setGlobalPosition(rockX, rockY, rockZ);
		trailHistory.push_back(rockSphere.getGlobalPosition());
		//for (int i = 0; i < trailCount; i++) {
		//	rockTrail[i].draw();
		//}
		drawTrail();
		material.begin();
		float alpha = 150;
		if (rockState > 2) {
			alpha = 255;
		}
		ofSetColor(120, 50, 20, alpha);
		ofFill();
		rockSphere.draw();
		material.end();
		ofNoFill();
		ofSetColor(255, 255, 0);
		rockSphere.setScale(1.01f);
		rockSphere.drawWireframe();
		rockSphere.setScale(1.f);
		rockSphere.rotateRad(.008, vec3(1, 1, 1));
		ofPopMatrix();
		rockRotate += .005;
		ofDisableDepthTest();
	}
	//return 1 to indicate rock is still active
	return 1;
}

void Rock::drawTrail() {
	ofSetLineWidth(4);
	for (vec3 point : trailHistory) {
		vec3 y = point * vec3(0, 1, 0);
		ofSetColor(0, 255, 0);
		//not current rockY
		ofDrawLine(point, point + vec3(0,3 * ofGetHeight()/4,0) - y);
		ofSetColor(255, 0, 0);
		ofPoint(point);
		//ofDrawPoint(point, point + vec3(0, 10, 0);
	}
	ofSetLineWidth(1);

}

Rock::Rock()
{
}


Rock::~Rock()
{
}
