#pragma once

#include "ofMain.h"
#include "ofxJson.h"
#include "ofUtils.h"
#include "ofPath.h"
#include "ofxParagraph.h"

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

		vector<ofxParagraph*> paragraphs;
		
	    int pWidth = 340;
	    int pFontSize = 14;
		float pPadding = pWidth*.30;
		int pLeading = 5;
		// int pSpacing = 10;

		int tot_num_tweets = 10;
		int posX = 10;
		int posYinit = 10;
		int red = 255;
		int blue = 255;
		int green = 255;
};