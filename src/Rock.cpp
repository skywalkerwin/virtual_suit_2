#include "Rock.h"

void Rock::setupRock(float sz) {
	rockSize = sz/1.5;
	rockSpeed = sz / 10;
	show = 1;
	rockState = 1;

}

void Rock::updateRock() {


}

int Rock::drawRock() {
	//EARTH BOULDER-------------------------------
	if (rockState != 0) {
		ofPushMatrix();
		ofTranslate(ofGetWidth() *2/ 5, rockHeight);
		//ofSetColor(120, 50, 20, 150);
		ofSetColor(120, 50, 20);
		ofFill();
		ofRotateYRad(rockRotate);
		ofSetSphereResolution(res);
		ofDrawSphere(rockSize);
		ofSetColor(0);
		ofSetColor(50, 255, 100);
		ofNoFill();
		ofDrawSphere(rockSize);
		ofPopMatrix();
		rockRotate += .005;
		rockHeight -= rockSpeed;
		if (rockHeight <= ofGetHeight() / 2) {
			rockState = 2;
			//at this state, rock needs to either listen for launch commands from legs, or wait to time out and vanish.
			rockSpeed = 0;
			timeUp += 1;
			if (timeUp >= 300) {
				//reset rock State to initial settings
				timeUp = 0;
				rockHeight = ofGetHeight();
				show = 0;
				rockState = 0;
				return 0;
			}
		}
		//if (rockHeight >= ofGetHeight() + rockSize) {
		//	//delete this;
		//}
	}
	return 1;
}

Rock::Rock()
{
}


Rock::~Rock()
{
}
