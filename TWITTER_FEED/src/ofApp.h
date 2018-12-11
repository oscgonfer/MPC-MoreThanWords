#pragma once

#include "ofMain.h"
#include "ofxJson.h"
#include "ofUtils.h"
#include "ofPath.h"

#define json_file_path "/Users/macoscar/Documents/04_Projects/03_ArtWork/of_v0.10.0_osx_release/apps/myApps/MTW_PY_LIVE/tweets_list.json"

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