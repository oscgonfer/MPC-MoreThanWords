#pragma once

#include "ofMain.h"
#include "ofxJson.h"
#include "ofUtils.h"
#include "ofPath.h"

#define json_file_name "tweets_list.json"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);

		ofxJSONElement json;
		ofTrueTypeFont ttf;
		int tot_num_tweets = 10;
};