#pragma once

#include "ofMain.h"
#include "ofxScreenGrab.h"
#include "ofxAudioUnit.h"


#define HEIGHT 512


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void mousePressed(int x, int y, int button);

		void windowResized(int w, int h);
	
		ofxScreenGrab grabber;
	
		ofPixels grabberPix;
	
		ofxAudioUnitOutput output;
		ofxAudioUnit firstAU;
		ofxAudioUnitTap tap;
	
		int columnNum;
	
	
};


static int column[HEIGHT];
static bool invert;
static int speed;

static OSStatus renderSineChord(void * inRefCon,
																AudioUnitRenderActionFlags * ioActionFlags,
																const AudioTimeStamp * inTimeStamp,
																UInt32 inBusNumber,
																UInt32 inNumberFrames,
																AudioBufferList * ioData);