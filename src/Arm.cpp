#include "Arm.h"

void Arm::armSetup(string c, ofNode &p, int n) {
	//ofSetLogLevel(OF_LOG_VERBOSE);
	drawBool = 1;
	dir = n;
	coms = c;
	string sidename = " ";
	if (n == -1) sidename = "Left"; 
	else sidename = "Right";
	string tname = c + " " + sidename;
	setThreadName(tname);
	//ofLog() << "THREAD NAME  "<<getThreadName() << endl;
	//ofLog() << "THREAD ID  " << getThreadId() << endl;
	//ulen = ofGetHeight() / 4;
	//llen = ofGetHeight() / 5;
	//hlen = ofGetHeight() / 10;
	ulen = 200;
	llen = 150;
	hlen = 50;
	body = p;
	upperArm.setParent(p);
	upperArm.set(50, ulen, 50);
	lowerArm.setParent(upperArm);
	lowerArm.set(40, llen, 40);
	hand.setParent(lowerArm);
	hand.set(30, hlen, 30);

	for (int i = 0; i < histlength; i++) {
		//handpoint.push_back(vec3(0, 0, 0));
		//handpoint.assign(histlength, vec3(0, 0, 0));
		handpoint[i] = vec3(0, 0, 0);
	}
	int reso = 2;
	upperArm.setResolution(reso);
	lowerArm.setResolution(reso);
	hand.setResolution(reso);
	ofLog() << sidename << "  parent:  " << upperArm.getParent() << endl;
	port.setup(c, 12000000);
	startThread();

}

void Arm::armDraw(vec3 b) {
	upperArm.setGlobalPosition(b);
	vec3 pos(dir * 100, 0, -215);
	upperArm.move(pos);
	shoulderpos = upperArm.getGlobalPosition();
	upperArm.rotateDeg(yaw[0], upperArm.getZAxis());
	upperArm.rotateDeg(pitch[0], upperArm.getXAxis());
	upperArm.rotateDeg(roll[0], upperArm.getYAxis());
	upperArm.boom(ulen / 2);
	//ofSetColor(0, 255, 0);
	//ofSetColor(75,0,130);
	//ofSetColor(255, 0, 255);
	ofSetColor(80);
	if(drawBool==1){
		upperArm.draw();
		ofSetColor(0);
		upperArm.drawWireframe();
	}
	upperArm.boom(ulen / 2);
	elbowpos = upperArm.getGlobalPosition();

	lowerArm.setGlobalPosition(elbowpos);
	lowerArm.rotateDeg(yaw[1], lowerArm.getZAxis());
	lowerArm.rotateDeg(pitch[1], lowerArm.getXAxis());
	lowerArm.rotateDeg(roll[1], lowerArm.getYAxis());
	lowerArm.boom(llen / 2);
	//ofSetColor(0, 255, 0);
	//ofSetColor(75, 0, 130);
	//ofSetColor(255, 0, 255);
	ofSetColor(80);
	if (drawBool == 1) {
		lowerArm.draw();
		ofSetColor(0);
		lowerArm.drawWireframe();
	}
	lowerArm.boom(llen / 2);
	wristpos = lowerArm.getGlobalPosition();

	handDraw();
	jointsDraw();
	upperArm.resetTransform();
	lowerArm.resetTransform();
	hand.resetTransform();
}

void Arm::handDraw() {
	hand.setGlobalPosition(wristpos);
	hand.rotateDeg(yaw[2], hand.getZAxis());
	hand.rotateDeg(pitch[2], hand.getXAxis());
	hand.rotateDeg(roll[2], hand.getYAxis());
	hand.boom(hlen / 2);
	//ofSetColor(0, 255, 0);
	//ofSetColor(75, 0, 130);
	//ofSetColor(255, 0, 255);
	ofSetColor(80);
	if (drawBool == 1) {
		hand.draw();
		ofSetColor(0);
		hand.drawWireframe();
	}
	hand.boom(hlen / 2);
	handpos = hand.getGlobalPosition();
	handquat = hand.getGlobalOrientation();
	handpoint[curLine] = hand.getGlobalPosition();
	curLine++;
	if (curLine == maxLines) {
		curLine = 0;
	}
}

void Arm::handtilt() {
	ofBoxPrimitive h;
	h.set(100);
	ofSetColor(0, 255, 0);
	ofPushMatrix();
	ofRotateZDeg(yaw[2]);
	ofRotateXDeg(pitch[2]);
	ofRotateYDeg(roll[2]);
	h.draw();
	ofSetColor(0);
	h.drawWireframe();
	ofPopMatrix();
}

void Arm::jointsDraw() {
	ofSetColor(255, 255, 0);
	ofDrawSphere(shoulderpos, 22);
	ofSetColor(180, 180, 0);
	ofDrawSphere(elbowpos, 18);
	ofSetColor(90, 90, 0);
	ofDrawSphere(wristpos, 12);
	ofSetColor(50, 50, 0);
	ofDrawSphere(handpos, 10);
	ofSetColor(0, 255, 0);
	//ofSetColor(0, 0, 255);
	ofDrawLine(shoulderpos, elbowpos);
	ofDrawLine(elbowpos, wristpos);
	ofDrawLine(wristpos, handpos);

}

void Arm::start() {
	startThread();
}

void Arm::stop() {
	while (updated == 0) {
	}
	std::unique_lock<std::mutex> lck(mutex);
	stopThread();
	condition.notify_all();
	port.close();

}

void Arm::threadedFunction() {
	while (isThreadRunning()) {
		//std::unique_lock<std::mutex> lock(mutex);
		armUpdate();
		//condition.wait(lock);
	}
}

void Arm::testimus() {

	//for (int i = 0; i < 8; i++) {
	//	ofBoxPrimitive h;
	//	h.set(100);
	//	ofSetColor(0, 255, 0);
	//	ofPushMatrix();
	//	ofTranslate(0, (1 + i) * ofGetHeight() / 9, 0);
	//	ofRotateXDeg(-90);
	//	ofRotateZDeg(yaw[i]);
	//	ofRotateXDeg(pitch[i]);
	//	ofRotateYDeg(roll[i]);
	//	//ofSetColor(0, 255, 0);
	//	//ofNoFill();
	//	//ofDrawBox(50);
	//	h.draw();
	//	ofSetColor(0);
	//	h.drawWireframe();
	//	ofPopMatrix();
	//	ofLog() << "i: " << i << "   yaw: " << yaw[i] << "    pitch: " << pitch[i] << "     roll:" << roll[i] << endl;
	//}
	for (int i = 0; i < 8; i++) {
		ofBoxPrimitive h;
		h.set(100);
		ofSetColor(0, 255, 0);
		ofPushMatrix();
		ofTranslate(0, (1 + i) * ofGetHeight() / 9, 0);
		//ofRotateXDeg(-90);
		//h.rotateDeg(yaw[i], h.getZAxis());
		h.rotateDeg(pitch[i], h.getXAxis());
		//h.rotateDeg(roll[i], h.getYAxis());
		//ofSetColor(0, 255, 0);
		//ofNoFill();
		//ofDrawBox(50);
		h.draw();
		ofSetColor(0);
		h.drawWireframe();
		ofPopMatrix();
		ofLog() << "i: " << i << "   yaw: " << yaw[i] << "    pitch: " << pitch[i] << "     roll:" << roll[i] << endl;
	}
}
	
void Arm::armUpdate() {
	updated = 0;
	if (firstContact == false) {
		char myByte = port.readByte();
		if (myByte == OF_SERIAL_NO_DATA)
			ofLog() << "no data was read";
		else if (myByte == OF_SERIAL_ERROR)
			ofLog() << "an error occurred";
		else {
			port.flush();
			firstContact = true;
			ofLog() << coms << " - TRYING TO WRITE" << endl;
			port.writeByte('A');
		}
	}
	else {
		if (port.available() > 121) {
			int res = port.readBytes(inBuffer, 122);
			port.writeByte('A');
			for (int i = 0; i < 3; i++) {
				// ACCEL
				magno[i][0] = (int16_t)((inBuffer[0 + (i * 18)] << 8 | inBuffer[1 + (i * 18)] & 0xff));
				magno[i][1] = (int16_t)((inBuffer[2 + (i * 18)] << 8 | inBuffer[3 + (i * 18)] & 0xff));
				magno[i][2] = (int16_t)((inBuffer[4 + (i * 18)] << 8 | inBuffer[5 + (i * 18)] & 0xff));
				// GYRO
				magno[i][3] = (int16_t)((inBuffer[6 + (i * 18)] << 8 | inBuffer[7 + (i * 18)] & 0xff));
				magno[i][4] = (int16_t)((inBuffer[8 + (i * 18)] << 8 | inBuffer[9 + (i * 18)] & 0xff));
				magno[i][5] = (int16_t)((inBuffer[10 + (i * 18)] << 8 | inBuffer[11 + (i * 18)] & 0xff));
				// MAGNO
				magno[i][6] = (int16_t)((inBuffer[12 + (i * 18)] << 8 | inBuffer[13 + (i * 18)] & 0xff));
				magno[i][7] = (int16_t)((inBuffer[14 + (i * 18)] << 8 | inBuffer[15 + (i * 18)] & 0xff));
				magno[i][8] = (int16_t)((inBuffer[16 + (i * 18)] << 8 | inBuffer[17 + (i * 18)] & 0xff));
				for (int j = 0; j < 9; j++) {
					if (magno[i][j] > negcheck) { //magno[i][j] = -(magno[i][j] - negcheck);//??????????????????????
					}
					if (j < 3) magno[i][j] = magno[i][j] * ascale;
					else if (j > 2 && j < 6) magno[i][j] = magno[i][j] * gscale *(PI / 180);
					else {
						//magno[i][j-6] = magno[i][j-6];
						if (magno[i][j] > magmax[i][j - 6]) { magmax[i][j - 6] = magno[i][j]; }
						if (magno[i][j] < magmin[i][j - 6]) { magmin[i][j - 6] = magno[i][j]; }
						magbias[i][j - 6] = (magmax[i][j - 6] + magmin[i][j - 6]) / 2;
						magscale[i][j - 6] = (magmax[i][j - 6] - magmin[i][j - 6]) / 2;
						avgrad[i] = (magscale[i][0] + magscale[i][1] + magscale[i][2]) / 3;
						if (magscale[i][j - 6] != 0) { scalebias[i][j - 6] = avgrad[i] / magscale[i][j - 6]; }
						magno[i][j] -= magbias[i][j - 6];
						magno[i][j] *= scalebias[i][j - 6];
					}
					//magno[i][j]=(float)(magno[i][j]*(((magadj[(-6+j)]-128) *.5)/128)+1);
					// APPEND TO HISTORIES
					for (int j = 0; j < 9; j++) {
						nmag[i][j][off] = magno[i][j];
						mhist[i][j][hcount] = magno[i][j];
					}
				}
			}
			// 5x imu6
			for (int i = 0; i < 5; i++) {
				// ACCEL
				imu[i][0] = (int16_t)((inBuffer[54 + (i * 12)] << 8) | (inBuffer[55 + (i * 12)] & 0xff));
				imu[i][1] = (int16_t)((inBuffer[56 + (i * 12)] << 8) | (inBuffer[57 + (i * 12)] & 0xff));
				imu[i][2] = (int16_t)((inBuffer[58 + (i * 12)] << 8) | (inBuffer[59 + (i * 12)] & 0xff));
				// GYRO
				imu[i][3] = (int16_t)((inBuffer[60 + (i * 12)] << 8) | (inBuffer[61 + (i * 12)] & 0xff));
				imu[i][4] = (int16_t)((inBuffer[62 + (i * 12)] << 8) | (inBuffer[63 + (i * 12)] & 0xff));
				imu[i][5] = (int16_t)((inBuffer[64 + (i * 12)] << 8) | (inBuffer[65 + (i * 12)] & 0xff));

				for (int j = 0; j < 6; j++) {
					if (imu[i][j] > negcheck) {//imu[i][j] = -(imu[i][j] - negcheck);
						if (j < 3) { imu[i][j] = imu[i][j] * ascale; }
						else { imu[i][j] = imu[i][j] * gscale *(PI / 180); }
					}
					// APPEND TO HISTORIES
					for (int j = 0; j < 6; j++) {
						nimu[i][j][off] = imu[i][j];
						ihist[i][j][hcount] = imu[i][j];
					}
				}
			}
			// touch stuff
			{
				thumbPressure = ((inBuffer[114] << 8) | (inBuffer[115] & 0xff));
				if (thumbPressure > maxpress) { maxpress = thumbPressure; }
				if (thumbPressure < minpress) { minpress = thumbPressure; }
				rangepress = maxpress - minpress;
				normpress = (1.0f * thumbPressure - minpress) / rangepress;
				switches[0] = 1 - inBuffer[116];
				switches[1] = 1 - inBuffer[117];
				switches[2] = 1 - inBuffer[118];
				switches[3] = 1 - inBuffer[119];
				switchbinary = (switches[3] << 3 | switches[2] << 2 | switches[1] << 1 | switches[0]);
				ttime = ((inBuffer[120] << 8) | (inBuffer[121] & 0xff));
			}
			off = off + 1;
			if (off == nframes) { off = 0; }
			deltat = ttime / 1000000.0f;
			hcount++;
			if (hcount == histlength) { hcount = 0; }
			sensorfusion();
		}
	}
	updated = 1;
	//ofLog() << coms << "ttime : "<<ttime << endl;
	//ofLog() << coms << "deltat: " << deltat << endl;
}

void Arm::Madgwick6(int i, float ax, float ay, float az, float gx, float gy, float gz) {
	float q1 = q[i][0], q2 = q[i][1], q3 = q[i][2], q4 = q[i][3]; // short name local variable for readability
	float norm; // vector norm
	float f1, f2, f3; // objetive funcyion elements
	float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33; // objective function Jacobian elements
	float qDot1, qDot2, qDot3, qDot4;
	float hatDot1, hatDot2, hatDot3, hatDot4;
	float gerrx, gerry, gerrz;
	//float gbiasx, gbiasy, gbiasz; // gyro bias error

	// Auxiliary variables to avoid repeated arithmetic
	float _halfq1 = 0.5 * q1;
	float _halfq2 = 0.5 * q2;
	float _halfq3 = 0.5 * q3;
	float _halfq4 = 0.5 * q4;
	float _2q1 = 2.0 * q1;
	float _2q2 = 2.0 * q2;
	float _2q3 = 2.0 * q3;
	float _2q4 = 2.0 * q4;
	float _2q1q3 = 2.0 * q1 * q3;
	float _2q3q4 = 2.0 * q3 * q4;

	// Normalise accelerometer measurement
	norm = sqrt(ax * ax + ay * ay + az * az);
	if (norm == 0.)
		return; // handle NaN
	norm = 1. / norm;
	ax *= norm;
	ay *= norm;
	az *= norm;

	// Compute the objective function and Jacobian
	f1 = _2q2 * q4 - _2q1 * q3 - ax;
	f2 = _2q1 * q2 + _2q3 * q4 - ay;
	f3 = 1. - _2q2 * q2 - _2q3 * q3 - az;
	J_11or24 = _2q3;
	J_12or23 = _2q4;
	J_13or22 = _2q1;
	J_14or21 = _2q2;
	J_32 = 2. * J_14or21;
	J_33 = 2. * J_11or24;

	// Compute the gradient (matrix multiplication)
	hatDot1 = J_14or21 * f2 - J_11or24 * f1;
	hatDot2 = J_12or23 * f1 + J_13or22 * f2 - J_32 * f3;
	hatDot3 = J_12or23 * f2 - J_33 * f3 - J_13or22 * f1;
	hatDot4 = J_14or21 * f1 + J_11or24 * f2;

	// Normalize the gradient
	norm = sqrt(hatDot1 * hatDot1 + hatDot2 * hatDot2 + hatDot3 * hatDot3 + hatDot4 * hatDot4);
	hatDot1 /= norm;
	hatDot2 /= norm;
	hatDot3 /= norm;
	hatDot4 /= norm;

	// Compute estimated gyroscope biases
	gerrx = _2q1 * hatDot2 - _2q2 * hatDot1 - _2q3 * hatDot4 + _2q4 * hatDot3;
	gerry = _2q1 * hatDot3 + _2q2 * hatDot4 - _2q3 * hatDot1 - _2q4 * hatDot2;
	gerrz = _2q1 * hatDot4 - _2q2 * hatDot3 + _2q3 * hatDot2 - _2q4 * hatDot1;

	// Compute and remove gyroscope biases
//        gbiasx += gerrx * deltat * zeta;
//        gbiasy += gerry * deltat * zeta;
//        gbiasz += gerrz * deltat * zeta;
//        gx -= gbiasx;
//        gy -= gbiasy;
//        gz -= gbiasz;

		// Compute the quaternion derivative
	qDot1 = -_halfq2 * gx - _halfq3 * gy - _halfq4 * gz;
	qDot2 = _halfq1 * gx + _halfq3 * gz - _halfq4 * gy;
	qDot3 = _halfq1 * gy - _halfq2 * gz + _halfq4 * gx;
	qDot4 = _halfq1 * gz + _halfq2 * gy - _halfq3 * gx;

	// Compute then integrate estimated quaternion derivative
	q1 += (qDot1 - (beta * hatDot1)) * deltat;
	q2 += (qDot2 - (beta * hatDot2)) * deltat;
	q3 += (qDot3 - (beta * hatDot3)) * deltat;
	q4 += (qDot4 - (beta * hatDot4)) * deltat;

	// Normalize the quaternion
	norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4); // normalise quaternion
	norm = 1. / norm;
	q[i][0] = q1 * norm;
	q[i][1] = q2 * norm;
	q[i][2] = q3 * norm;
	q[i][3] = q4 * norm;
}

void Arm::Madgwick9(int i, float ax, float ay, float az, float gx, float gy, float gz, float mx, float my,
	float mz) {
	// https://github.com/kriswiner/MPU9250/blob/master/quaternionFilters.ino
	float q1 = q[i][0], q2 = q[i][1], q3 = q[i][2], q4 = q[i][3]; // short name local variable for readability
	float norm;
	float hx, hy, _2bx, _2bz;
	float s1, s2, s3, s4;
	float qDot1, qDot2, qDot3, qDot4;

	// Auxiliary variables to avoid repeated arithmetic
	float _2q1mx;
	float _2q1my;
	float _2q1mz;
	float _2q2mx;
	float _4bx;
	float _4bz;
	float _2q1 = 2. * q1;
	float _2q2 = 2. * q2;
	float _2q3 = 2. * q3;
	float _2q4 = 2. * q4;
	float _2q1q3 = 2. * q1 * q3;
	float _2q3q4 = 2. * q3 * q4;
	float q1q1 = q1 * q1;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q1q4 = q1 * q4;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q2q4 = q2 * q4;
	float q3q3 = q3 * q3;
	float q3q4 = q3 * q4;
	float q4q4 = q4 * q4;
	//		float beta = ;
			// Normalise accelerometer measurement
	norm = sqrt(ax * ax + ay * ay + az * az);
	if (norm == 0.)
		return; // handle NaN
	norm = 1. / norm;
	ax *= norm;
	ay *= norm;
	az *= norm;

	// Normalise magnetometer measurement
	norm = sqrt(mx * mx + my * my + mz * mz);
	if (norm == 0.)
		return; // handle NaN
	norm = 1. / norm;
	mx *= norm;
	my *= norm;
	mz *= norm;

	// Reference direction of Earth's magnetic field
	_2q1mx = 2. * q1 * mx;
	_2q1my = 2. * q1 * my;
	_2q1mz = 2. * q1 * mz;
	_2q2mx = 2. * q2 * mx;
	hx = mx * q1q1 - _2q1my * q4 + _2q1mz * q3 + mx * q2q2 + _2q2 * my * q3 + _2q2 * mz * q4 - mx * q3q3
		- mx * q4q4;
	hy = _2q1mx * q4 + my * q1q1 - _2q1mz * q2 + _2q2mx * q3 - my * q2q2 + my * q3q3 + _2q3 * mz * q4 - my * q4q4;
	_2bx = sqrt(hx * hx + hy * hy);
	_2bz = -_2q1mx * q3 + _2q1my * q2 + mz * q1q1 + _2q2mx * q4 - mz * q2q2 + _2q3 * my * q4 - mz * q3q3
		+ mz * q4q4;
	_4bx = 2. * _2bx;
	_4bz = 2. * _2bz;

	// Gradient decent algorithm corrective step
	s1 = -_2q3 * (2. * q2q4 - _2q1q3 - ax) + _2q2 * (2. * q1q2 + _2q3q4 - ay)
		- _2bz * q3 * (_2bx * (0.5 - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
		+ (-_2bx * q4 + _2bz * q2) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
		+ _2bx * q3 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5 - q2q2 - q3q3) - mz);
	s2 = _2q4 * (2. * q2q4 - _2q1q3 - ax) + _2q1 * (2. * q1q2 + _2q3q4 - ay)
		- 4. * q2 * (1. - 2. * q2q2 - 2. * q3q3 - az)
		+ _2bz * q4 * (_2bx * (0.5 - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
		+ (_2bx * q3 + _2bz * q1) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
		+ (_2bx * q4 - _4bz * q2) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5 - q2q2 - q3q3) - mz);
	s3 = -_2q1 * (2. * q2q4 - _2q1q3 - ax) + _2q4 * (2. * q1q2 + _2q3q4 - ay)
		- 4. * q3 * (1. - 2. * q2q2 - 2. * q3q3 - az)
		+ (-_4bx * q3 - _2bz * q1) * (_2bx * (0.5 - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
		+ (_2bx * q2 + _2bz * q4) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
		+ (_2bx * q1 - _4bz * q3) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5 - q2q2 - q3q3) - mz);
	s4 = _2q2 * (2. * q2q4 - _2q1q3 - ax) + _2q3 * (2. * q1q2 + _2q3q4 - ay)
		+ (-_4bx * q4 + _2bz * q2) * (_2bx * (0.5 - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
		+ (-_2bx * q1 + _2bz * q3) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
		+ _2bx * q2 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5 - q2q2 - q3q3) - mz);
	norm = sqrt(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4); // normalise step magnitude
	norm = 1. / norm;
	s1 *= norm;
	s2 *= norm;
	s3 *= norm;
	s4 *= norm;

	// Compute rate of change of quaternion
	qDot1 = 0.5 * (-q2 * gx - q3 * gy - q4 * gz) - beta * s1;
	qDot2 = 0.5 * (q1 * gx + q3 * gz - q4 * gy) - beta * s2;
	qDot3 = 0.5 * (q1 * gy - q2 * gz + q4 * gx) - beta * s3;
	qDot4 = 0.5 * (q1 * gz + q2 * gy - q3 * gx) - beta * s4;

	// Integrate to yield quaternion
	q1 += qDot1 * deltat;
	q2 += qDot2 * deltat;
	q3 += qDot3 * deltat;
	q4 += qDot4 * deltat;
	norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4); // normalise quaternion
	norm = 1. / norm;
	q[i][0] = q1 * norm;
	q[i][1] = q2 * norm;
	q[i][2] = q3 * norm;
	q[i][3] = q4 * norm;
}

void Arm::sensorfusion() {
	for (int i = 0; i < 3; i++) {
		Madgwick9(i, magno[i][0], magno[i][1], magno[i][2], magno[i][3] * PI / 180., magno[i][4] * PI / 180.,
			magno[i][5] * PI / 180., magno[i][7], magno[i][6], magno[i][8]);
		yaw[i] = atan2(2. * (q[i][1] * q[i][2] + q[i][0] * q[i][3]),
			q[i][0] * q[i][0] + q[i][1] * q[i][1] - q[i][2] * q[i][2] - q[i][3] * q[i][3]);
		pitch[i] = -asin(2. * (q[i][1] * q[i][3] - q[i][0] * q[i][2]));
		roll[i] = -atan2(2. * (q[i][0] * q[i][1] + q[i][2] * q[i][3]),
			q[i][0] * q[i][0] - q[i][1] * q[i][1] - q[i][2] * q[i][2] + q[i][3] * q[i][3]);
		pitch[i] *= 180. / PI;
		yaw[i] *= 180. / PI;
		yaw[i] -= 8; // Declination at Berkley, Colorado.
		roll[i] *= 180.0 / PI;
	}

	for (int i = 3; i < 8; i++) {
		Madgwick6(i, imu[i - 3][0], imu[i - 3][1], imu[i - 3][2], imu[i - 3][3] * PI / 180.,
			imu[i - 3][4] * PI / 180., imu[i - 3][5] * PI / 180.);
		yaw[i] = atan2(2.0 * (q[i][1] * q[i][2] + q[i][0] * q[i][3]),
			q[i][0] * q[i][0] + q[i][1] * q[i][1] - q[i][2] * q[i][2] - q[i][3] * q[i][3]);
		pitch[i] = -asin(2.0 * (q[i][1] * q[i][3] - q[i][0] * q[i][2]));
		roll[i] = -atan2(2.0 * (q[i][0] * q[i][1] + q[i][2] * q[i][3]),
			q[i][0] * q[i][0] - q[i][1] * q[i][1] - q[i][2] * q[i][2] + q[i][3] * q[i][3]);
		pitch[i] *= 180.0 / PI;
		yaw[i] *= 180.0 / PI;
		//yaw[i] *= 0;
		roll[i] *= 180.0 / PI;
	}
	//for (int i = 0; i < 8; i++) {
	//	ofLog() << i << ": " << " Yaw: " << yaw[i] << " Pitch: " << pitch[i] << "Roll: " << -roll[i] << endl;
	//}

}

void Arm::plotimu(vec3 pos, float x, float y, int c) {
	float xoff = x / histlength;
	float yoff = y / 60000;
	ofNoFill();
	ofPushMatrix();
	ofTranslate(0, y / 2, 0);
	//ofSetColor(255, 255, 255);
	ofLine(vec3(0, 0, 0), vec3(x, 0, 0));
	for (int j = 0; j < 5; j++) {
		for (int k = 0; k < 6; k++) {
			//if (k < 3) {
			//	yoff = y / 640;
			//}
			//if (k > 2) {
			//	yoff = y / 700;
			//}
			if (k == 0 || k == 3) {
				ofSetColor(255, 0, 0);
			}
			else if (k == 1 || k == 4) {
				ofSetColor(0, 255, 0);
			}
			else {
				ofSetColor(0, 0, 255);
			}
			ofPushMatrix();
			ofBeginShape();
			for (int i = hcount; i < histlength; i++) {
				ofVertex(x - ((i - hcount) * xoff), ihist[j][k][i] * yoff);
			}
			ofVertex((hcount)* xoff, ihist[j][k][histlength - 1] * yoff);
			for (int i = 0; i < hcount; i++) {
				ofVertex((hcount - i)*xoff, ihist[j][k][i] * yoff);
			}
			ofEndShape(false);
			ofPopMatrix();
		}
	}
	ofPopMatrix();

}

Arm::Arm()
{
	for (int i = 0; i < 8; i++) {
		q[i][0] = 1.0;
		q[i][1] = 0.;
		q[i][2] = 0.;
		q[i][3] = 0.;
	}
}

Arm::~Arm()
{
	stop();
	waitForThread(true);
}
