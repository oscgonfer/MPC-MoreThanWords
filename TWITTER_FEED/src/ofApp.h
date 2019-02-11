#pragma once

#include "ofMain.h"
#include "ofxJson.h"
#include "ofUtils.h"
#include "ofPath.h"
#include "ofxParagraph.h"
#include "ofxSyphon.h"

#define json_file_name "tweets_list.json"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void draw();

		void keyPressed(int key);

		ofxJSONElement json;
		ofxParagraph paragraph;
		shared_ptr<ofxSmartFont> pFont;
		shared_ptr<ofxSmartFont> authorFont;
		shared_ptr<ofxSmartFont> hFont;		

		// ofTrueTypeFont pFont;
		vector<ofxParagraph*> paragraphs;
		
	    int pWidth = 380;
	    int pFontSize = 6;
		float pPadding = pWidth*.30;
		int pLeading = 3;
		ofColor pColor = ofColor::fromHex(0xFFFFFF); // twitterBlue = 0x1da1f2
		ofColor hColor = ofColor::fromHex(0xccd6dd); // twitterBlue = 0x1da1f2
		ofColor backgroundParagraph = ofColor::fromHex(0x1f2429);
		ofColor backgroundGeneral = ofColor::fromHex(0x1f2429);

		int tot_num_tweets = 10;
		int posX = 30;
		int posYinit = 30;

		ofxSyphonServer mainOutputSyphonServer;
};
