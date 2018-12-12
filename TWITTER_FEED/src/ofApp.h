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
		int posX = 20;
		int posY = 20;
		int red = 255;
		int blue = 255;
		int green = 255;
};