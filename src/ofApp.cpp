#include "ofApp.h"
using namespace glm;

//--------------------------------------------------------------
void ofApp::setup() {
	
	oracle.setupOracle();
	camWidth = 1920;  // try to grab at this size.
	camHeight = 1080;

	//get back a list of devices.
	vector<ofVideoDevice> devices = vidGrabber.listDevices();

	for (size_t i = 0; i < devices.size(); i++) {
		if (devices[i].bAvailable) {
			//log the device
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
		}
		else {
			//log the device and note it as unavailable
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
		}
	}

	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(120);
	vidGrabber.initGrabber(camWidth, camHeight);

	ofSetVerticalSync(false);
	ofSetFrameRate(120);
	//ofSetVerticalSync(true);
	//ofSetFrameRate(60);
	ofSetColor(0, 255, 0);
	stepClock.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	stepClock2.setMode(OF_PRIMITIVE_TRIANGLE_FAN);

	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	//ofLog() << serial.listDevices() << endl;
	//ofLog() << deviceList.size() << endl;

	mybody.bodySetup();

	//mybody.legsSetup();
	for (int i = 0; i < dummyCount; i++) {
		//dummy[i].setupDummy(vec3(ofGetWidth() / 2, ofGetHeight() / 2, ofRandom(-5000, -2000)));
		dummy[i].setupDummy(vec3(ofGetWidth() / 2 + ofRandom(-4000, 4000), ofGetHeight() / 2, ofRandom(-6000, -2000)));
	}

	cam.setFov(120);
	cam.setFarClip(500000);
	//cam.setGlobalPosition(ofGetWidth()/2, ofGetHeight()/2, 200);
}

//--------------------------------------------------------------
void ofApp::update() {
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());

	vidGrabber.update();
	if (vidGrabber.isFrameNew()) {
		ofPixels & pixels = vidGrabber.getPixels();
	}
	for (int i = 0; i < dummyCount; i++) {
		dummy[i].updateDummy();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	//cam.setGlobalPosition(ofGetWidth() / 2 + camX, 2000 + camY, 1000 + camZ);
	//cam.setGlobalPosition(ofGetWidth() / 2 + camX, -400 + camY, -100 + camZ);
	cam.setGlobalPosition(mybody.torso.getGlobalPosition()+vec3(0,-600,700));
	cam.begin();
	ofScale(1, -1, 1);
	ofBackground(100);
	ofDrawBitmapString(ofGetWidth() / 2 + camX, 20, 20);
	ofDrawBitmapString(-500 + camY, 20, 40);
	ofDrawBitmapString(800 + camZ, 20, 60);
	if (showCam == 1) {
		ofSetHexColor(0xffffff);
		ofPushMatrix();
		//ofTranslate(0, 420, -900);
		//vidGrabber.draw(2560, 0, -2560, -1440);
		vidGrabber.draw(2450, -50, -2350, -1330);
		ofPopMatrix();
	}
	ofEnableDepthTest();
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, 3 * ofGetHeight() / 4, 0);
	ofRotateZDeg(-90);
	ofSetLineWidth(5);
	ofSetColor(24, 202, 230);
	//ofDrawGridPlane(100, 250);
	//ofSetColor(254, 1, 154);
	//ofTranslate(1, 0, 1);
	//ofDrawGridPlane(100, 200);
	ofSetLineWidth(1);
	ofPopMatrix();

	for (int i = 0; i < dummyCount; i++) {
		dummy[i].drawDummy();
		//for (int j = 0; j < dummyCount; j++) {
		//	ofSetColor(255);
		//	ofDrawLine(dummy[i].hitbox.getGlobalPosition(), dummy[j].hitbox.getGlobalPosition());
		//}
	}
	ofDisableDepthTest();

	//for (int i = 0; i < mybody.earth.rock.trailCount; i++) {
	//	mybody.earth.rock.rockTrail[i].draw();
	//}

	//--rock stuff--
	//cam.lookAt(mybody.torso);

	mybody.checkMoves(mybody.torso.getGlobalPosition());
	for (int i = 0; i < dummyCount; i++) {
		if (mybody.earth.rock.rockState == 3 && distance(mybody.earth.rock.rockSphere.getGlobalPosition(), dummy[i].hitbox.getGlobalPosition()) < 500) {
			ofSetColor(255, 255, 0);
			ofFill();
			mybody.earth.rock.rockState = 5;
			dummy[i].updateDummyHP(abs(mybody.earth.rock.rockDZ / 10));
		}
	}

	ofPopMatrix();
	cam.end();
	cam.resetTransform();
	ofScale(1, 1, 1);
	ofSetColor(255);
	if (showPlots) {
		ofPushMatrix();
		//mybody.drawHandPlots();
		mybody.legs.plotimu3(ofGetWidth(), ofGetHeight(), exclu);
		mybody.legs.plotAvgs();
		ofPopMatrix();
	}
	//ofPushMatrix();
	//ofTranslate(3 * ofGetWidth() / 4, ofGetHeight() / 4);
	//ofEnableDepthTest();
	////ofRotateYDeg(mybody.rarm.yaw[2]);
	//ofRotateXDeg(mybody.rarm.pitch[2]);
	//ofRotateZDeg(-mybody.rarm.roll[2]);
	//ofSetColor(255);
	//ofFill();
	//ofDrawBox(200);
	//ofNoFill();
	//ofSetColor(0);
	//ofDrawBox(200);
	//ofFill();
	//ofDisableDepthTest();
	//ofPopMatrix();
	//frameClock();
}


void ofApp::frameClock() {

	// ----------------------------------------------------------------- FRAME CLOCK -------------------------------------------------
	ofPushMatrix();
	ofTranslate(19 * ofGetWidth() / 20, ofGetHeight() / 10, 0);
	if (framecount == 0) {
		stepClock.addColor(ofColor(255));
		stepClock.addVertex(vec3(0, 0, 0));
		stepClock.addColor(ofColor(255));
		stepClock.addVertex(vec3(30, 0, 0));
	}
	ofRotateZRad(-PI / 2);
	stepClock.addColor(ofColor(255));
	stepClock.addVertex(vec3(30 * cos(stepClockAngle), 30 * sin(stepClockAngle), 0));
	stepClock.draw();
	framecount += 1;
	stepClockAngle = framecount * (2 * PI / cycleFrames);
	if (framecount == cycleFrames) {
		stepClock.addColor(ofColor(255));
		stepClock.addVertex(vec3(30, 0, 0));
		stepClock.draw();
		framecount = 0;
		stepClock.clearVertices();
	}
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(17 * ofGetWidth() / 20, ofGetHeight() / 10, 0);
	if (framecount2 == 0) {
		stepClock2.addColor(ofColor(255));
		stepClock2.addVertex(vec3(0, 0, 0));
		stepClock2.addColor(ofColor(255));
		stepClock2.addVertex(vec3(30, 0, 0));
	}
	ofRotateZRad(-PI / 2);
	stepClock2.addColor(ofColor(255));
	stepClock2.addVertex(vec3(30 * cos(stepClockAngle2), 30 * sin(stepClockAngle2), 0));
	stepClock2.draw();
	framecount2 += 1;
	stepClockAngle2 = framecount2 * (2 * PI / cycleFrames2);
	if (framecount2 == cycleFrames2) {
		stepClock2.addColor(ofColor(255));
		stepClock2.addVertex(vec3(30, 0, 0));
		stepClock2.draw();
		framecount2 = 0;
		stepClock2.clearVertices();
	}
	ofPopMatrix();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	vec3 v;
	int move = 50;
	switch (key) {
	case 'c':
		showCam = !showCam;
		break;
	case '1':
		exclu[0] = !exclu[0];
		break;
	case '2':
		exclu[1] = !exclu[1];
		break;
	case '3':
		exclu[2] = !exclu[2];
		break;
	case '4':
		exclu[3] = !exclu[3];
		break;
	case '5':
		exclu[4] = !exclu[4];
		break;
	case 'a':
		camY -= 10;
		break;
	case 'q':
		camY += 10;
		break;
	case 'd':
		v = vec3(move, 0, 0);
		cam.setGlobalPosition(cam.getGlobalPosition() + v);
		break;
	case 'e':
		v = vec3(0, -move, 0);
		cam.setGlobalPosition(cam.getGlobalPosition() + v);
		break;
	case 's':
		camZ -= 10;
		break;
	case 'w':
		camZ += 10;
		break;
	case 'l':
		//v = vec3(0, 0, -move);
		cam.lookAt(cycle.core);
		break;
	case 'p':
		showPlots = !showPlots;
		break;
	case 'x':
		res++;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::exit() {
	//mybody.~Body();
}
