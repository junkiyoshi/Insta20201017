#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);

	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}

	this->radius = 12;
	auto x_span = this->radius * sqrt(3);
	auto flg = true;
	for (float y = 0; y < ofGetHeight() + this->radius; y += this->radius * 1.5) {

		for (float x = 0; x < ofGetWidth() + this->radius; x += x_span) {

			glm::vec2 location;
			if (flg) {

				location = glm::vec2(x, y);
			}
			else {

				location = glm::vec2(x + (this->radius * sqrt(3) / 2), y);
			}

			this->hexagon_list.push_back(location);
			this->noise_seed_list.push_back(glm::vec2(ofRandom(1000), ofRandom(1000)));
			this->alpha_list.push_back(0);
		}
		flg = !flg;
	}

	this->font.loadFont("fonts/Kazesawa-bold.ttf", 260, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	ofFbo fbo;
	ofPixels pixels;
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
	ofClear(0);

	string word = "HEX";
	this->font.drawString(word, ofGetWidth() * 0.5 - this->font.stringWidth(word) * 0.5 - 5, ofGetHeight() * 0.5 + this->font.stringHeight(word) * 0.5);

	fbo.end();
	fbo.readToPixels(pixels);

	ofColor pix_color;
	int span = 10;

	for (int i = 0; i < this->hexagon_list.size(); i++) {
		
		pix_color = pixels.getColor((int)this->hexagon_list[i].x, (int)this->hexagon_list[i].y);
		if (pix_color != ofColor(0, 0)) {

			this->alpha_list[i] = this->alpha_list[i] < 255 ? this->alpha_list[i] + 50 : 255;
		}
		else {

			this->alpha_list[i] = this->alpha_list[i] > 0 ? this->alpha_list[i] - 2 : 0;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofColor color;
	for (int i = 0; i < this->hexagon_list.size(); i++) {

		vector<glm::vec2> vertices;
		for (auto deg = 90; deg < 450; deg += 60) {

			vertices.push_back(this->hexagon_list[i] + glm::vec2(this->radius * cos(deg * DEG_TO_RAD), this->radius * sin(deg * DEG_TO_RAD)));
		}

		color = this->base_color_list[(int)ofMap(ofNoise(glm::vec3(this->hexagon_list[i] * 0.005, ofGetFrameNum() * 0.02)), 0, 1, 0, this->base_color_list.size())];

		ofFill();
		ofSetColor(color, this->alpha_list[i]);
		ofBeginShape();
		ofVertices(vertices);
		ofEndShape(true);

		ofNoFill();
		ofSetColor(239);
		ofBeginShape();
		ofVertices(vertices);
		ofEndShape(true);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}