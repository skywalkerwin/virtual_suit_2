#include "Sword.h"


void Sword::setupSword() {
	startpos = vec3(ofRandom(-1000, 1000), ofRandom(500, 1500), ofGetHeight()/2);
	startrot = vec3(ofRandom(-180, 180), ofRandom(-180, 180), ofRandom(-180, 180));
	handle.set(10, 120, 20, 2, 2, true);
	handle.setCylinderColor(220);
	blade.set(8, maxBladeLen, 20, 2, 2, true);
	blade.setCylinderColor(ofColor(20, 255, 50));
	blade.setParent(handle);
	handle.setGlobalPosition(startpos);
	handle.setOrientation(startrot);
	handlepos = handle.getGlobalPosition();
	saberOn.load("sounds/saberOn.mp3");
	saberOff.load("sounds/saberOff.mp3");
	saberOn.setVolume(0.5f);
	saberOff.setVolume(0.5f);
	saberOn.setMultiPlay(false);
	saberOff.setMultiPlay(false);
	saberOn.setSpeed(1.2);
	saberOff.setSpeed(1.5);
}

void Sword::drawSword(quat q, vec3 handv, int sb) {
	//startpos = vec3(0, ofRandom(0, 1000), ofGetHeight()/3);
	blade.setCylinderColor(ofColor(20, 255, 50));
	if (inGround == 1) {
		handle.setGlobalPosition(startpos);
		handle.setOrientation(startrot);
		handle.draw();
		if (sb == 4) {
			inAir = 1;
			inGround = 0;
			travelv = (handv - handlepos) / 30;
		}
	}

	if (inAir == 1) {
		handlepos = handlepos + travelv;
		handle.setGlobalPosition(handlepos);
		handle.draw();
		float dis = glm::distance(handlepos, handv);
		if (dis < 50) {
			inHand = 1;
			inAir = 0;
		}
	}
	if (inHand == 1) {
		if (sb == 1) {
			if (isOn == 0 && playSounds == 1) {
				saberOn.play();
			}
			isOn = 1;
		}
		if (sb == 2) {
			if (isOn == 1 && playSounds == 1) {
				saberOff.play();
			}
			isOn = 0;
		}
		if (isOn == 1 && bladeLen < maxBladeLen) {
			bladeLen += maxBladeLen / 48;
		}
		if (isOn == 0 && bladeLen > 0) {
			bladeLen -= maxBladeLen / 48;
		}
		handle.setGlobalPosition(handv);
		handle.setGlobalOrientation(q);
		handle.rotateDeg(90, handle.getXAxis());
		handle.draw();
		blade.setHeight(bladeLen);
		blade.boom(-bladeLen/2);
		blade.draw();
	}
	handlepos = handle.getGlobalPosition();
	handle.resetTransform();
	blade.resetTransform();
}

Sword::Sword()
{
}

Sword::~Sword()
{
}
