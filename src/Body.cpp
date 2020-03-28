#include "Body.h"


void Body::bodySetup() {
	torso.set(200);
	legs.legsSetup(legComs);
	//larm.armSetup(leftcom, torso, -1);
	//rarm.armSetup(rightcom, torso, 1);
	torso.setGlobalPosition(vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0));
	//*lcycle = c;
	//torso.setParent(lcycle->core);
	//vec3 offset(0, 0, -120);
	//torso.setGlobalPosition(v + offset);

}

void Body::checkMoves(vec3 v) {
	//either expand this function, or flesh out the oracle to check for incoming data.
	//ofEnableDepthTest();
	//ofSetColor(0, 255, 0);
	//torso.draw();
	//ofSetColor(0);
	//torso.drawWireframe();
	//ofDisableDepthTest();
	bodyDraw();

	if (earth.moveState == 0 && rarm.switches[0] == 1) {
		int ind = legs.rockCheck1();
		if (ind != -1) {
			float legVal = abs(legs.xyzavgs[ind][3]);
			if (earth.rockUp == 0 && legVal > 200) {
				ofLog() << "ROCK JUMP" << endl;
				earth.rock.setupRock(v, legVal, ind);
				earth.rockUp = 1;
				earth.moveState = 1;
			}
		}
	}
	//check for rock propel state
	if (earth.moveState == 1 && rarm.switches[0] == 1 && earth.rock.rockState==2) {
		int ind = legs.rockCheck2();
		if (ind != -1) {
			float xacc = (legs.avgs[ind][0][0]);
			float yacc = abs(legs.avgs[ind][0][1]);
			float zacc = (legs.avgs[ind][0][2]);
			if (zacc > 3) {
				//trigger rock kick
				ofLog() << "ROCK KICK" << endl;
				earth.rock.updateRock(ind, xacc, yacc, zacc);
			}
		}
	}

	if (!earth.rock.drawRock()) {
		//if drawRock returns 0, it is non active so reset rock settings
		ofLog() << "RESET ROCK" << endl;
		earth.rockUp = 0;
		earth.moveState = 0;
	}
}

void Body::legsSetup() {
	legs.legsSetup(legComs);
}

void Body::armsSetup() {
	//torso.set(50);
	larm.armSetup(leftcom, torso, -1);
	rarm.armSetup(rightcom, torso, 1);
	//*lcycle = c;
	//torso.setParent(lcycle->core);
	//vec3 offset(0, 0, -120);
	//torso.setGlobalPosition(v + offset);

}

void Body::bodyDraw() {
	//vec3 offset(0, 0, -120);
	//torso.setGlobalPosition(lcycle->getGlobalPosition()+offset);
	//torsopos = torso.getGlobalPosition();
	////neckpos = torso.getGlobalPosition();
	//larm.armDraw(torsopos);
	//rarm.armDraw(torsopos);
	//ofSetColor(255,0,0);
	////ofSetLineWidth(5);
	//ofDrawLine(larm.handpos, rarm.handpos);
	////ofSetColor(255);
	//torso.draw();
	//torso.resetTransform();

	ofSetLineWidth(1);

	ofPushMatrix();
	ofEnableDepthTest();
	ofTranslate(torso.getGlobalPosition());
	ofColor curColor = ofColor(255, 255, 0);
	//ofNoFill();
	//ofDrawBox(300, 700, 150);
	ofTranslate(0, -100, 0);
	ofFill();
	ofSetColor(255, 0, 0);
	ofSetColor(curColor);
	ofDrawBox(200, 280, 80);
	ofSetColor(255);
	ofSetColor(0);
	ofNoFill();
	ofSetLineWidth(4);
	ofDrawBox(201, 281, 81);
	ofSetLineWidth(1);
	//legs
	ofPushMatrix();
	ofTranslate(-50, 270, 0);
	ofFill();
	ofSetColor(255, 0, 0);
	ofSetColor(curColor);
	ofDrawBox(70, 350, 50);
	ofSetColor(255);
	ofSetColor(0);
	ofNoFill();
	ofSetLineWidth(4);
	ofDrawBox(71, 351, 51);
	ofSetLineWidth(1);

	ofTranslate(100, 0, 0);
	ofFill();
	ofSetColor(255, 0, 0);
	ofSetColor(curColor);
	ofDrawBox(70, 350, 50);
	ofSetColor(255);
	ofSetColor(0);
	ofNoFill();
	ofSetLineWidth(4);
	ofDrawBox(71, 351, 51);
	ofSetLineWidth(1);
	ofPopMatrix();

	//arms
	ofPushMatrix();
	ofTranslate(-100, 40, 0);
	ofFill();
	ofSetColor(255, 0, 0);
	ofSetColor(curColor);
	ofDrawBox(100, 310, 40);
	ofSetColor(255);
	ofSetColor(0);
	ofNoFill();
	ofSetLineWidth(4);
	ofDrawBox(101, 311, 41);
	ofSetLineWidth(1);

	ofTranslate(200, 0, 0);
	ofFill();
	ofSetColor(255, 0, 0);
	ofSetColor(curColor);
	ofDrawBox(100, 310, 40);
	ofSetColor(255);
	ofSetColor(0);
	ofNoFill();
	ofSetLineWidth(4);
	ofDrawBox(101, 311, 41);
	ofSetLineWidth(1);
	ofPopMatrix();

	//head
	ofPushMatrix();
	ofTranslate(0, -190, 0);
	ofFill();
	ofSetColor(255, 0, 0);
	ofSetColor(curColor);
	ofDrawBox(80, 120, 50);
	ofSetColor(255);
	ofSetColor(0);
	ofNoFill();
	ofSetLineWidth(4);
	ofDrawBox(91, 121, 51);
	ofSetLineWidth(1);
	ofPopMatrix();

	ofDisableDepthTest();
	ofPopMatrix();

}

void Body::bodyUpdate(vec3 v) {
	//larm.armUpdate();
	//rarm.armUpdate();
	curLine++;
	if (curLine == maxLines) {
		curLine = 0;
	}
	//auto velVector = torso.getYAxis() * vel;
	//torso.move(velVector);
	torso.setGlobalPosition(v);
}
void Body::torsoDraw() {
	vec3 origin(0, 0, 0);
	torso.setPosition(origin);
}

void Body::testimus() {
	//ofPushMatrix();
	//ofTranslate(1 * ofGetWidth() / 4, 0, 0);
	//larm.testimus();
	//ofPopMatrix();
	//ofPushMatrix();
	//ofTranslate(3 * ofGetWidth() / 4, 0, 0);
	//rarm.testimus();
	//ofPopMatrix();
	ofSetColor(255);
	ofDrawBitmapString(rarm.pitch[3], ofGetWidth() - 100, 20);
	ofDrawBitmapString(rarm.pitch[4], ofGetWidth() - 100, 40);
	ofDrawBitmapString(rarm.pitch[5], ofGetWidth() - 100, 60);
	ofDrawBitmapString(rarm.pitch[6], ofGetWidth() - 100, 80);
	ofDrawBitmapString(rarm.pitch[7], ofGetWidth() - 100, 100);

	ofDrawBitmapString(rarm.roll[3], ofGetWidth() - 100, 140);
	ofDrawBitmapString(rarm.roll[5], ofGetWidth() - 100, 160);
	ofDrawBitmapString(rarm.roll[4], ofGetWidth() - 100, 180);
	ofDrawBitmapString(rarm.roll[6], ofGetWidth() - 100, 200);
	ofDrawBitmapString(rarm.roll[7], ofGetWidth() - 100, 220);

}

void Body::drawHandPlots() {
	testimus();
	larm.plotimu(vec3(0, 0, 0), ofGetWidth(), ofGetHeight(), 0);
	rarm.plotimu(vec3(0, 0, 0), ofGetWidth(), ofGetHeight(), 0);
}



Body::Body()
{
}


Body::~Body()
{
	//larm.~Arm();
	//rarm.~Arm();
	//delete &larm;
	//delete &rarm;
}
