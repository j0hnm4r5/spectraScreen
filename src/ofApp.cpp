#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	
//	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	
	
	grabber.setup(ofGetWidth(), ofGetHeight());
	
	
	firstAU.setup(kAudioUnitType_Effect, kAudioUnitSubType_GraphicEQ);
	
	firstAU.connectTo(tap).connectTo(output);
	
	AURenderCallbackStruct callbackInfo;
	callbackInfo.inputProc = renderSineChord;
	callbackInfo.inputProcRefCon = this;
	firstAU.setRenderCallback(callbackInfo);
	
	output.start();
	
	for (int i = 0; i < HEIGHT; i++) {
		column[i] = 0;
	}
	
	columnNum = 0;
	
	invert = false;
	speed = 5;

}

//--------------------------------------------------------------
OSStatus renderSineChord(void * inRefCon,
												 AudioUnitRenderActionFlags * ioActionFlags,
												 const AudioTimeStamp * inTimeStamp,
												 UInt32 inBusNumber,
												 UInt32 inNumberFrames,
												 AudioBufferList * ioData) {
	
	Float32 * leftSamples  = (Float32 *)ioData->mBuffers[0].mData;
	Float32 * rightSamples = (Float32 *)ioData->mBuffers[1].mData;
	
	
	static double t = 0;
	
	// for each frame in buffer
	for (int frame = 0; frame < inNumberFrames; frame++) {
		
		leftSamples[frame] = sin(0);

		
		for (int pix = 0; pix < HEIGHT; pix++) {
			// y(t) = amplitude * sin(frequency * t + phase)
			
			if (invert) {
				leftSamples[frame] += ((255 - column[pix]) / 255.) * sin(((HEIGHT - pix) / 3) * t);
			} else {
				leftSamples[frame] += (column[pix] / 255.) * sin(((HEIGHT - pix) / 3) * t);
			}
			
			
		}
		
		leftSamples[frame] /= float(HEIGHT);
		
		
		t += 0.01;
	}
	
	
	// copy left channel into right channel
	memcpy(rightSamples, leftSamples, inNumberFrames * sizeof(Float32));

	// everything went smoothly
	return noErr;
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	grabber.grabScreen(ofGetWindowPositionX(), ofGetWindowPositionY());
	
	grabber.getTextureReference().readToPixels(grabberPix);
	grabberPix.setImageType(OF_IMAGE_GRAYSCALE);
	
	for (int y = 0; y < HEIGHT; y++) {
		column[y] = 255 - grabberPix.getColor(columnNum % ofGetWidth(), y).getBrightness();
	}
	
	columnNum += speed;

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofSetColor(200, 200, 200);
//	grabber.draw(0, 0);
	
	ofTexture tex;
	tex.allocate(grabberPix);
	tex.draw(0, 0);
	
	if (invert) {
		ofSetColor(0, 220, 0);
	} else {
		ofSetColor(220, 0, 220);
	}
	
	
	ofSetLineWidth(2);
	ofDrawLine(columnNum % ofGetWidth(), 0, columnNum % ofGetWidth(), ofGetHeight());
	
	// border lines
	ofSetColor(220, 220, 220);
	ofSetLineWidth(50);
	ofDrawLine(0, 0, 0, ofGetHeight());
	ofDrawLine(0, ofGetHeight(), ofGetWidth(), ofGetHeight());
	ofDrawLine(ofGetWidth(), 0, ofGetWidth(), ofGetHeight());

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	if (key == ' ') {
		invert = !invert;
	}
	
	if (key == OF_KEY_LEFT || key == OF_KEY_DOWN) {
		speed--;
	}
	
	if (key == OF_KEY_RIGHT || key == OF_KEY_UP) {
		speed++;
	}

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
	ofSetWindowShape(ofGetWidth(), HEIGHT);
	grabber.setup(ofGetWidth(), ofGetHeight());
	columnNum = 0;

}