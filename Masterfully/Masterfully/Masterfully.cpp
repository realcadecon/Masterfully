// Masterfully.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Masterfully.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <time.h>
#include <unordered_map>


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Shape.h"
#include "Component.h"
#include "JointNode.h"
#include "Line.h"
#include <inc/Kinect.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include "Text.h"
#include "Texture.h"

#include <pqxx/pqxx>

using namespace std;

GLFWwindow* window; // Main application window
GLFWwindow* kinectVideoOut; //RGB kinect video output
string RES_DIR = "./resources/"; // Where data files live
//string RES_DIR = "C:/Users/cadea/OneDrive/Documents/Masterfully/Masterfully/Masterfully/resources";
shared_ptr<Program> prog;
shared_ptr<Program> BPprog;
shared_ptr<Program> FTprog;
shared_ptr<Program> TEXprog;
shared_ptr<Shape> shape;
shared_ptr<Shape> sphere;
shared_ptr<Shape> quad;
JointNode* studentRoot;
JointNode* teacherRoot;
unordered_map<JointType, JointNode*> studentJointMap;
unordered_map<JointType, JointNode*> teacherJointMap;

const int FRAMERATE = 30;
GLuint textureId;			 // ID of the texture to contain Kinect RGB Data
GLubyte textureData[128*128 * 4]; // BGRA array containing the texture data
int indCountQ = 0;
map<string, GLuint> qBufIDs;
shared_ptr<Texture> texture0;

IKinectSensor* sensor = nullptr;
IBodyFrameReader* bodyFrameReader = nullptr;
IColorFrameReader* colorFramerReader = nullptr;
HRESULT hr;

const int FONT_HEIGHT = 48;
std::map<char, Character> characters;
unsigned int VAO, VBO;

struct Pose {
	string name, src, srcPic;

	Pose(string _name, string _src, string _srcPic) {
		name = _name;
		src = _src;
		srcPic = _srcPic;
	}
};

vector<Pose> poses;
int currPose = 5;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

template<class Interface>
static inline void safeRelease(Interface*& interfaceToRelease)
{
	if (interfaceToRelease != nullptr) {
		interfaceToRelease->Release();
		interfaceToRelease = nullptr;
	}
}

static void error_callback(int error, const char* description)
{
	cerr << description << endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	if (codepoint == 46) {;}
}

static void initHierarchy(JointNode *root, unordered_map<JointType, JointNode*> &jointMap) {
	// Spine Nodes
	//root = new JointNode(JointType_SpineBase);
	jointMap[JointType_SpineBase] = root;

	JointNode* spineMid = new JointNode(JointType_SpineMid, glm::vec3(1,0,0)); //red
	jointMap[JointType_SpineMid] = spineMid;

	JointNode* spineShoulder = new JointNode(JointType_SpineShoulder, glm::vec3(51, 0, 0)); //dark red
	jointMap[JointType_SpineShoulder] = spineShoulder;

	JointNode* neck = new JointNode(JointType_Neck, glm::vec3(255, 102, 102)); //light red
	jointMap[JointType_Neck] = neck;

	JointNode* head = new JointNode(JointType_Head, glm::vec3(255, 204, 204)); //very light red
	jointMap[JointType_Head] = head;

	// Right Side
	JointNode* shoulderRight = new JointNode(JointType_ShoulderRight, glm::vec3(153, 76, 0)); //orange
	jointMap[JointType_ShoulderRight] = shoulderRight;

	JointNode* elbowRight = new JointNode(JointType_ElbowRight, glm::vec3(102, 51, 0)); //dark orange
	jointMap[JointType_ElbowRight] = elbowRight;

	JointNode* wristRight = new JointNode(JointType_WristRight, glm::vec3(255, 153, 51)); //light orange
	jointMap[JointType_WristRight] = wristRight;

	JointNode* handRight = new JointNode(JointType_HandRight, glm::vec3(255, 204, 153)); //tan
	jointMap[JointType_HandRight] = handRight;

	JointNode* handTipRight = new JointNode(JointType_HandTipRight, glm::vec3(204, 204, 0)); //yellow
	jointMap[JointType_HandTipRight] = handTipRight;

	JointNode* thumbRight = new JointNode(JointType_ThumbRight, glm::vec3(204, 204, 0)); //yellow
	jointMap[JointType_ThumbRight] = thumbRight;

	JointNode* hipRight = new JointNode(JointType_HipRight, glm::vec3(0, 1, 0)); //green
	jointMap[JointType_HipRight] = hipRight;

	JointNode* kneeRight = new JointNode(JointType_KneeRight, glm::vec3(25, 51, 0)); //dark green
	jointMap[JointType_KneeRight] = kneeRight;

	JointNode* ankleRight = new JointNode(JointType_AnkleRight, glm::vec3(102, 255, 102)); //light green
	jointMap[JointType_AnkleRight] = ankleRight;

	JointNode* footRight = new JointNode(JointType_FootRight, glm::vec3(204, 255, 204)); //pale green
	jointMap[JointType_FootRight] = footRight;

	// Left Side
	JointNode* shoulderLeft = new JointNode(JointType_ShoulderLeft, glm::vec3(0, 0, 1)); //blue
	jointMap[JointType_ShoulderLeft] = shoulderLeft;

	JointNode* elbowLeft = new JointNode(JointType_ElbowLeft, glm::vec3(0, 0, 51)); //dark blue
	jointMap[JointType_ElbowLeft] = elbowLeft;

	JointNode* wristLeft = new JointNode(JointType_WristLeft, glm::vec3(0, 204, 204)); //light blue
	jointMap[JointType_WristLeft] = wristLeft;

	JointNode* handLeft = new JointNode(JointType_HandLeft, glm::vec3(0, 102, 102)); //dark cyan
	jointMap[JointType_HandLeft] = handLeft;

	JointNode* handTipLeft = new JointNode(JointType_HandTipLeft, glm::vec3(153, 204, 255)); //pale blue
	jointMap[JointType_HandTipLeft] = handTipLeft;

	JointNode* thumbLeft = new JointNode(JointType_ThumbLeft, glm::vec3(153, 204, 255)); //pale blue
	jointMap[JointType_ThumbLeft] = thumbLeft;

	JointNode* hipLeft = new JointNode(JointType_HipLeft, glm::vec3(76, 0, 153)); //purple
	jointMap[JointType_HipLeft] = hipLeft;

	JointNode* kneeLeft = new JointNode(JointType_KneeLeft, glm::vec3(25, 0, 51)); // dark purple
	jointMap[JointType_KneeLeft] = kneeLeft;

	JointNode* ankleLeft = new JointNode(JointType_AnkleLeft, glm::vec3(178, 102, 255)); //light purple
	jointMap[JointType_AnkleLeft] = ankleLeft;

	JointNode* footLeft = new JointNode(JointType_FootLeft, glm::vec3(204, 0, 102)); //pink
	jointMap[JointType_FootLeft] = footLeft;

	// Make Hierarchy
	// --- Spine Connections
	root->limbs.push_back(new JointNode::Limb( glm::vec3(0.f, 0.f, 0.f), 0.f, spineMid ));
	root->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, hipRight ));
	root->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, hipLeft ));

	spineMid->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, spineShoulder ));

	spineShoulder->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, neck ));
	spineShoulder->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, shoulderRight ));
	spineShoulder->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, shoulderLeft ));

	neck->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, head ));

	// --- Right Side Connections
	// --- --- Right Arm
	shoulderRight->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, elbowRight ));
	elbowRight->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, wristRight ));
	wristRight->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, handRight ));
	handRight->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, handTipRight ));
	handRight->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, thumbRight ));

	// --- --- Right Leg
	hipRight->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, kneeRight ));
	kneeRight->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, ankleRight ));
	ankleRight->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, footRight ));

	// Left Side Connections
	// --- --- Left Arm
	shoulderLeft->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, elbowLeft ));
	elbowLeft->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, wristLeft ));
	wristLeft->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, handLeft ));
	handLeft->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, handTipLeft ));
	handLeft->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, thumbLeft ));

	// --- --- Left Leg
	hipLeft->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, kneeLeft ));
	kneeLeft->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, ankleLeft ));
	ankleLeft->limbs.push_back(new JointNode::Limb( glm::vec3(0, 0, 0), 0.f, footLeft ));
}

void loadTeacherNorms(string filename) {
	ifstream ifs(filename);
	JointNode* root = teacherRoot;
	stack<JointNode*> s;
	float x, y, z;
	if (ifs.is_open()) {
		s.push(root);
		while (!s.empty()) {
			JointNode* cur = s.top();
			s.pop();
			for (auto* l : cur->limbs) {
				ifs >> x >> y >> z;
				l->norm = glm::vec3(x, y, z);
	
				s.push(l->node);
			}
		}
	}
}

void TEST_loadStudentData(string filename) {
	ifstream ifs(filename);
	JointNode* root = studentRoot;
	stack<JointNode*> s;
	float x, y, z;
	if (ifs.is_open()) {
		s.push(root);
		while (!s.empty()) {
			JointNode* cur = s.top();
			s.pop();
			ifs >> x >> y >> z;
			cur->pos = 3.f * glm::vec3(x, y, z);
			for (auto* l : cur->limbs) {
				s.push(l->node);
			}
		}

		s.push(root);
		while (!s.empty()) {
			JointNode* cur = s.top();
			s.pop();
			for (auto* x : cur->limbs) {
				x->length = glm::distance(x->node->pos, cur->pos);
				x->norm = glm::normalize(x->node->pos - cur->pos);
				cout << x->norm.x << " " << x->norm.y << " " << x->norm.z << "\n";
				s.push(x->node);
			}
		}
	}
}

static void init()
{
	GLSL::checkVersion();

	// Set background color.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);

	// Initialize mesh.
	shape = make_shared<Shape>();
	shape->loadMesh(RES_DIR + "cube.obj");
	shape->init();

	sphere = make_shared<Shape>();
	sphere->loadMesh(RES_DIR + "sphere.obj");
	sphere->init();

	quad = make_shared<Shape>();
	quad->loadMesh(RES_DIR + "ground.obj");
	quad->init();

	// Initialize the GLSL program.
	prog = make_shared<Program>();
	prog->setVerbose(true);
	prog->setShaderNames(RES_DIR + "simple_vert.glsl", RES_DIR + "simple_frag.glsl");
	prog->init();
	prog->addUniform("P");
	prog->addUniform("MV");
	prog->addUniform("col");
	prog->addUniform("alpha");
	prog->addAttribute("aPos");
	prog->addAttribute("aNor");
	prog->setVerbose(false);

	// BP Shader
	BPprog = make_shared<Program>();
	BPprog->setShaderNames(RES_DIR + "blinn-phong" + "_vert.glsl", RES_DIR + "blinn-phong" + "_frag.glsl");
	BPprog->setVerbose(true);
	BPprog->init();
	BPprog->addAttribute("aPos");
	BPprog->addAttribute("aNor");
	BPprog->addUniform("MV");
	BPprog->addUniform("P");
	BPprog->addUniform("VM");
	BPprog->addUniform("col");
	BPprog->setVerbose(false);

	// FreeType Shader
	FTprog = make_shared<Program>();
	FTprog->setShaderNames(RES_DIR + "FT" + "_vert.glsl", RES_DIR + "FT" + "_frag.glsl");
	FTprog->setVerbose(true);
	FTprog->init();
	FTprog->addUniform("text");
	FTprog->addUniform("textColor");
	FTprog->addUniform("projection");
	FTprog->setVerbose(true);

	studentRoot = new JointNode(JointType_SpineBase, glm::vec3(0,0,0));
	teacherRoot = new JointNode(JointType_SpineBase, glm::vec3(0, 0, 0));
	initHierarchy(studentRoot, studentJointMap);
	initHierarchy(teacherRoot, teacherJointMap);

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return ;
	}

	string fontFile = "./resources/fonts/arial.ttf";
	FT_Face face;
	if (FT_New_Face(ft, fontFile.c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, FONT_HEIGHT);

	Text::loadCharacters(face, characters);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//loadTeacherNorms("./resources/warrior2.txt");
	loadTeacherNorms(poses[currPose].src);

	//texture Shader
	TEXprog = make_shared<Program>();
	TEXprog->setShaderNames(RES_DIR + "texture_vert.glsl", RES_DIR + "texture_frag.glsl");
	TEXprog->setVerbose(true);
	TEXprog->init();
	TEXprog->addAttribute("aPos");
	TEXprog->addAttribute("aNor");
	TEXprog->addAttribute("aTex");
	TEXprog->addUniform("P");
	TEXprog->addUniform("MV");
	TEXprog->addUniform("texture0");

	texture0 = make_shared<Texture>();
	texture0->setFilename(poses[currPose].srcPic);
	//texture0->setFilename("./resources/grass2.png");
	texture0->init();
	texture0->setUnit(0);
	texture0->setWrapModes(GL_REPEAT, GL_REPEAT);

	// If there were any OpenGL errors, this will print something.
	// You can intersperse this line in your code to find the exact location
	// of your OpenGL error.
	GLSL::checkError(GET_FILE_LINE);
}

glm::vec3 CSPtovec3(CameraSpacePoint &p) {
	return glm::vec3(p.X, p.Y, p.Z);
}

void loadJointsIntoHierarchy(Joint* joint, JointNode* root, unordered_map<JointType, JointNode*> &jointMap) {
	for (int i = 0; i < JointType_Count; ++i) {
		jointMap[JointType(i)]->pos = CSPtovec3(joint[i].Position);
	}
	
	stack<JointNode*> s;

	s.push(root);
	while (!s.empty()) {
		JointNode* cur = s.top();
		s.pop();
		for (auto* x : cur->limbs) {
			x->length = glm::distance(x->node->pos, cur->pos);
			x->norm = glm::normalize(x->node->pos - cur->pos);
			cout <<x->norm.x << " " << x->norm.y << " " << x->norm.z << "\n";
			s.push(x->node);
		}
	}
}

static bool initKinect() {
	if (FAILED(GetDefaultKinectSensor(&sensor))) {
		return false;
	}
	if (sensor) {
		sensor->Open();

		IColorFrameSource* framesource = NULL;
		sensor->get_ColorFrameSource(&framesource);
		framesource->OpenReader(&colorFramerReader);
		if (framesource) {
			framesource->Release();
			framesource = NULL;
		}
		return true;
	}
	else {
		return false;
	}
}

static void renderVideo() {
	IColorFrame* frame = NULL;
	if (SUCCEEDED(colorFramerReader->AcquireLatestFrame(&frame))) {
		frame->CopyConvertedFrameDataToArray(640 * 480 * 4, textureData, ColorImageFormat_Bgra);
	}
	if (frame) frame->Release();

	// Initialize textures
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 640, 480,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)textureData);
	glBindTexture(GL_TEXTURE_2D, 0);

	// OpenGL setup
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glEnable(GL_TEXTURE_2D);

	// Camera setup
	glViewport(0, 0, 640, 480);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 640, 480, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//draw frames
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 640, 480, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)textureData);

	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(640, 0, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(640, 480, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 480, 0.0f);
	glEnd();

}

static void getJointData(Joint* bodyJoints) {
	IBodyFrame* bodyFrame = nullptr;
	hr = bodyFrameReader->AcquireLatestFrame(&bodyFrame);

	if (SUCCEEDED(hr)) {
		IBody* bodies[BODY_COUNT] = { 0 };
		hr = bodyFrame->GetAndRefreshBodyData(_countof(bodies), bodies);

		if (SUCCEEDED(hr)) {
			//processBodies(BODY_COUNT, bodies);
			for (unsigned int bodyIndex = 0; bodyIndex < BODY_COUNT; bodyIndex++) {
				IBody* body = bodies[bodyIndex];

				//Get the tracking status for the body, if it's not tracked we'll skip it
				BOOLEAN isTracked = false;
				HRESULT hr = body->get_IsTracked(&isTracked);
				if (FAILED(hr) || isTracked == false) {
					continue;
				}

				//If we're here the body is tracked so lets get the joint properties for this skeleton
				Joint joints[JointType_Count];
				hr = body->GetJoints(_countof(joints), joints);
				if (SUCCEEDED(hr)) {
					for (int i = 0; i < 25; i++) {
						bodyJoints[i] = joints[i];
					}
					const CameraSpacePoint& leftHip = joints[JointType_HipLeft].Position;
					const CameraSpacePoint& leftKnee = joints[JointType_KneeLeft].Position;
					//Let's print the head's position
					const CameraSpacePoint& headPos = joints[JointType_Head].Position;
					const CameraSpacePoint& leftHandPos = joints[JointType_HandLeft].Position;

					if (leftHandPos.Y > headPos.Y) {
						std::cout << "DATA START\n";
					}

					HandState leftHandState;
					hr = body->get_HandLeftState(&leftHandState);
					if (SUCCEEDED(hr)) {
						if (leftHandState == HandState_Closed) {
							std::cout << "CLOSED HAND\n";
						}
						else if (leftHandState == HandState_Open) {
							std::cout << "OPEN HAND\n";
						}
						else if (leftHandState == HandState_Lasso) {
							std::cout << "PEW PEW HANDS\n";
						}
						else if (leftHandState == HandState_NotTracked) {
							std::cout << "HAND IS NOT TRACKED\n";
						}
						else if (leftHandState == HandState_Unknown) {
							std::cout << "HANDS STATE IS UNKNOWN\n";
						}
					}
				}
			}
			//After body processing is done, we're done with our bodies so release them.
			for (unsigned int bodyIndex = 0; bodyIndex < _countof(bodies); bodyIndex++) {
				safeRelease(bodies[bodyIndex]);
			}

			safeRelease(bodyFrame);
		}
	}
	else if (sensor) {
		BOOLEAN isSensorAvailable = false;
		hr = sensor->get_IsAvailable(&isSensorAvailable);
		if (SUCCEEDED(hr) && isSensorAvailable == false) {
			std::cerr << "No available sensor is found.\n";
		}
	}
	else {
		std::cerr << "Trouble reading the body frame.\n";
	}
}

glm::vec3 getColor(glm::vec3 sNorm, glm::vec3 tNorm) {
	// temporary logic
	glm::vec3 col(0, 255, 0);
	float ang = glm::acos(glm::dot(sNorm, tNorm));
	int val = min(ang/(M_PI/2), 1.0) * 510.0;
	if (val > 255) {
		col.r += 255;
		col.g -= (val - 255);
	}
	else {
		col.r += val;
	}
	return col;
}

float getGrade(glm::vec3 sNorm, glm::vec3 tNorm) {
	float ang = abs(glm::acos(glm::dot(sNorm, tNorm)));
	float val = max(1.0 - (ang / (M_PI)), 0.0);
	
	return val;
}

void computeTeacherData() {
	stack<JointNode*> s;
	stack<JointNode*> s_t;
	teacherRoot->pos = studentRoot->pos;
	s.push(studentRoot);
	s_t.push(teacherRoot);
	while (!s.empty()) {
		JointNode* cur = s.top();
		s.pop();
		JointNode* cur_t = s_t.top();
		s_t.pop();
		for (int i = 0; i < cur_t->limbs.size(); ++i) {
			cur_t->limbs[i]->length = cur->limbs[i]->length;
			cur_t->limbs[i]->node->pos = cur_t->pos + (cur_t->limbs[i]->length * cur_t->limbs[i]->norm);
			s.push(cur->limbs[i]->node);
			s_t.push(cur_t->limbs[i]->node);
		}
	}
}

string getJointName(JointType joint) {
	switch (joint) {
		case 0: 
			return "Spine Base";
		case 1:
			return "Spine Mid";
		case 2:
			return "Neck";
		case 3:
			return "Head";
		case 4:
			return "Left Shoulder";
		case 5:
			return "Left Elbow";
		case 6:
			return "Left Wrist";
		case 7:
			return "Left Hand";
		case 8:
			return "Right Shoulder";
		case 9:
			return "Right Elbow";
		case 10:
			return "Right Wrist";
		case 11:
			return "Right Hand";
		case 12:
			return "Left Hip";
		case 13:
			return "Left Knee";
		case 14:
			return "Left Ankle";
		case 15:
			return "Left Foot";
		case 16:
			return "Right Hip";
		case 17:
			return "Right Knee";
		case 18:
			return "Right Ankle";
		case 19:
			return "Right Foot";
		case 20:
			return "Spine Shoulder";
		case 21:
			return "Tip of Left Hand";
		case 22:
			return "Left Thumb";
		case 23:
			return "Tip of Right Hand";
		case 24:
			return "Right Thumb";
	}
}

glm::vec3 getGradeCol(float grade) {
	float col_grad = min(1.f, (grade - 70.f) / 25.f);
	glm::vec3 grade_col(1.f, 0.f, 0.f);
	float val = col_grad * 2.f;
	if (val > 1.f) {
		grade_col.g = 1.f;
		grade_col.r -= (val - 1.f);
	}
	else {
		grade_col.g += val;
	}
	return grade_col;
}

static void render()
{
	// Get Joint data from Kinect
	Joint bodyJoints[25];
	getJointData(bodyJoints);
	// Load Joint data into hierarchy
	if (bodyJoints[0].Position.X > -1000) {
		loadJointsIntoHierarchy(bodyJoints, studentRoot, studentJointMap);
		computeTeacherData();
	}
	else {
		// FOR TESTING
		//TEST_loadStudentData("./resources/test.txt");
		computeTeacherData();
		cerr << "No Body Detected\n";
	}

	// Grade variable
	float grade_sum = 0;
	float grade_min = 1.f;
	JointType grade_min_type = JointType_SpineBase;

	// Get current frame buffer size.
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	float aspect = width / (float)height;
	glViewport(0, 0, width, height);

	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create matrix stacks.
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	// Apply projection.
	P->pushMatrix();
	P->multMatrix(glm::perspective((float)(45.0 * M_PI / 180.0), aspect, 0.01f, 100.0f));
	// Apply camera transform.
	MV->pushMatrix();
	MV->translate(glm::vec3(0, 0, -7));

	// Draw mesh using GLSL.

	MV->pushMatrix();
	stack<JointNode*> s;
	stack<JointNode*> s_t;
	// FOR TESTING:
    
	if(true) {
	//if (bodyJoints != nullptr) {
		s.push(studentRoot);
		s_t.push(teacherRoot);
		while (!s.empty()) {
			BPprog->bind();
			glUniformMatrix4fv(BPprog->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);
			JointNode* cur = s.top(); // Current Student Joint
			JointNode* cur_t = s_t.top(); // Current Teacher Joint
			s.pop();
			s_t.pop();
			// Draw student joint
			MV->pushMatrix();
				MV->translate(cur->pos);
				MV->scale(.15, .15, .15);
				glUniformMatrix4fv(BPprog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
				glm::mat4 VM = glm::transpose(glm::inverse(MV->topMatrix()));
				glUniformMatrix4fv(BPprog->getUniform("VM"), 1, GL_FALSE, &VM[0][0]);
				glUniform3f(BPprog->getUniform("col"), cur->color.r, cur->color.g, cur->color.b);
				sphere->draw(BPprog);
			MV->popMatrix();
			BPprog->unbind();
			// Draw teacher joint
			prog->bind();
			glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);
			MV->pushMatrix();
				MV->translate(cur_t->pos);
				MV->translate(0, 0, -.5);
				MV->scale(.15, .15, .15);
				glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
				glUniform3f(prog->getUniform("col"), 200.f, 200.f, 200.f);
				glUniform1f(prog->getUniform("alpha"), 1.f);
				sphere->draw(prog);
			MV->popMatrix();
			prog->unbind();
			// Draw limbs between joints
			for (int i = 0; i < cur_t->limbs.size(); ++i) {
				// Draw student limb
				BPprog->bind();
				glUniformMatrix4fv(BPprog->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);
				auto* x = cur->limbs[i];
				auto* x_t = cur_t->limbs[i];
				s.push(x->node);
				s_t.push(x_t->node);
				x->node->color = getColor(x->norm, x_t->norm);
				float tmp_grade = getGrade(x->norm, x_t->norm);
				
				grade_sum += tmp_grade;
				if (tmp_grade < grade_min) {
					if (!(x->node->type == JointType_ThumbLeft || x->node->type == JointType_ThumbRight)) {
						grade_min = tmp_grade;
						grade_min_type = x->node->type;
						cout << "tmp_grade: " << tmp_grade << " type: " << grade_min_type << "\n";
					}
				}
				
				MV->pushMatrix();
					MV->translate(cur->pos);
					MV->rotate(glm::acos(glm::dot(glm::vec3(0, 1, 0), x->norm)), glm::cross(glm::vec3(0, 1, 0), x->norm));
					MV->translate(glm::vec3(0, x->length / 2.f, 0));
					MV->scale(glm::vec3(0.08, x->length, 0.08));
					glUniformMatrix4fv(BPprog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
					glm::mat4 VM = glm::transpose(glm::inverse(MV->topMatrix()));
					glUniformMatrix4fv(BPprog->getUniform("VM"), 1, GL_FALSE, &VM[0][0]);
					glUniform3f(BPprog->getUniform("col"), x->node->color.r, x->node->color.g, x->node->color.b);
					shape->draw(BPprog);
				MV->popMatrix();
				BPprog->unbind();
				// Draw teacher limb
				prog->bind();
				glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);
				MV->pushMatrix();
					MV->translate(0, 0, -.5);
					MV->translate(cur_t->pos);
					MV->rotate(glm::acos(glm::dot(glm::vec3(0, 1, 0), x_t->norm)), glm::cross(glm::vec3(0, 1, 0), x_t->norm));
					MV->translate(glm::vec3(0, x_t->length / 2.f, 0));
					MV->scale(glm::vec3(0.08, x_t->length, 0.08));
					glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
					glUniform3f(prog->getUniform("col"), 200.f, 200.f, 200.f);
					glUniform1f(prog->getUniform("alpha"), 1.f);
					shape->draw(prog);
				MV->popMatrix();
				prog->unbind();
			}
		}
		prog->bind();
		MV->pushMatrix();
		MV->translate(studentJointMap[grade_min_type]->pos);
		MV->scale(0.4f);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
		glUniform3f(prog->getUniform("col"), 200.f, 0.f, 0.f);
		glUniform1f(prog->getUniform("alpha"), 0.5f);
		sphere->draw(prog);
		MV->popMatrix();
		prog->unbind();
		MV->popMatrix();
		//textured Quad
		TEXprog->bind();
		texture0->bind(TEXprog->getUniform("texture0"));
		P->popMatrix();
		P->pushMatrix();
		P->multMatrix(glm::perspective((float)(45.0 * M_PI / 180.0), aspect, 0.01f, 100.0f));
		MV->pushMatrix();
		glUniformMatrix4fv(TEXprog->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);
		/*glEnableVertexAttribArray(TEXprog->getAttribute("aPos"));
		GLSL::checkError(GET_FILE_LINE);
		glEnableVertexAttribArray(TEXprog->getAttribute("aTex"));
		glBindBuffer(GL_ARRAY_BUFFER, qBufIDs["bPos"]);
		glVertexAttribPointer(TEXprog->getAttribute("aPos"), 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, qBufIDs["bTex"]);
		glVertexAttribPointer(TEXprog->getAttribute("aTex"), 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, qBufIDs["bInd"]);*/
		
		MV->pushMatrix();
			
			MV->translate(-2, -2, -2);
			MV->scale(5);
			
			glUniformMatrix4fv(TEXprog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			//glDrawElements(GL_TRIANGLES, indCountQ, GL_UNSIGNED_INT, (void*)0);
			quad->draw(TEXprog);
		MV->popMatrix();

		/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(TEXprog->getAttribute("aTex"));
		glDisableVertexAttribArray(TEXprog->getAttribute("aPos"));*/

		MV->popMatrix();

		TEXprog->unbind();


	}
	//Render title
	Text::renderText(FTprog, poses[currPose].name, 200, height - FONT_HEIGHT - 5, 1, glm::vec3(0, 0, 0), VAO, VBO, characters, window);


	// Render the Grade
	float grade = 100.0 * grade_sum / 25.0;
	string grade_out = to_string(grade);
	grade_out = grade_out.substr(0, 4) + "%";
	
	glm::vec3 grade_col = getGradeCol(grade);
	
	Text::renderText(FTprog, grade_out, 0 + 5, height - FONT_HEIGHT - 5, 1, grade_col, VAO, VBO, characters, window);
	// Render advice
	glm::vec3 min_grade_col = getGradeCol(grade_min);
	Text::renderText(FTprog, "Work on your:", width - 125, height/2 + 0.3*FONT_HEIGHT, 0.3, glm::vec3(0, 0, 0), VAO, VBO, characters, window);
	Text::renderText(FTprog, getJointName(grade_min_type), width - 125, height / 2 , 0.3, glm::vec3(0, 0, 0), VAO, VBO, characters, window);
	Text::renderText(FTprog, string(to_string(100.f*grade_min).substr(0, 4) + "% Accurate"), width - 125, height / 2 - 0.3*FONT_HEIGHT, 0.3, min_grade_col, VAO, VBO, characters, window);
	
	//MV->popMatrix();
	prog->unbind();
	

	// Pop matrix stacks.
	MV->popMatrix();
	P->popMatrix();

	GLSL::checkError(GET_FILE_LINE);
}

int test()
{
	try {
		pqxx::connection C("postgres://fwdufwcq:nSMG96HTO2RxWz2E-Iu-WNwkfoFMKnKW@heffalump.db.elephantsql.com/fwdufwcq");
		if (C.is_open()) {
			const char* sql;
			cout << "Opened database succesfully: " << C.dbname() << endl;
			sql = "SELECT COUNT (*) from poses";
			pqxx::nontransaction N(C);
			pqxx::result R(N.exec(sql));
			N.commit();
			int numPoses = stoi(R[0][0].c_str());
			for (int i = 0; i < numPoses; i++) {
				pqxx::nontransaction N2(C);
				string angles;
				string query = "SELECT name, angles, image from poses WHERE id = " + to_string(i);
				sql = query.c_str();
				pqxx::result R(N2.exec(sql));
				N2.commit();
				Pose temp(R[0][0].c_str(), R[0][1].c_str(), R[0][2].c_str());
				poses.push_back(temp);
			}
			currPose = 2;
		}
	}
	catch(const exception& e){
		cerr << e.what() << endl;
		return 1;
	}
	//load poses
	//Pose war1("Warrior I", "./resources/warrior1.txt", "./resources/war1.JPG");
	//poses.push_back(war1);
	//Pose war2("Warrior II", "./resources/warrior2.txt", "./resources/war2.JPG");
	//poses.push_back(war2);
	//Pose tri("Extended Triangle", "./resources/tri.txt", "./resources/tri.JPG"); //still needs picture
	//poses.push_back(tri);
	//Pose lotus("Lotus Pose", "./resources/lotus.txt", "./resources/lotus.JPG");
	//poses.push_back(lotus);
	//Pose upDog("Upward-Facing Dog", "./resources/upwardDog.txt", "./resources/upDog.JPG");
	//poses.push_back(upDog);	
	//Pose lordDance("Lord of the Dance", "./resources/lordOfDance.txt", "./resources/lordDance.JPG");
	//poses.push_back(lordDance);
	//Pose sideStretch("Intense Side Stretch", "./resources/sideStretch.txt", "./resources/sideStretch.JPG");
	//poses.push_back(sideStretch);	
	//Pose downDog("Downward Facing Dog", "./resources/downDog.txt", "./resources/downDog.JPG");
	//poses.push_back(downDog);
	//currPose = 0;


	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if (!glfwInit()) {
		return -1;
	}
	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(800, 480, "Animation", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	/*kinectVideoOut = glfwCreateWindow(640, 480, "Kinect Video", NULL, NULL);
	if (!kinectVideoOut) {
		glfwTerminate();
		return -1;
	} */
	// Make the window's context current.
	glfwMakeContextCurrent(window);
	// Initialize GLEW.
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}
	glGetError(); // A bug in glewInit() causes an error that we can safely ignore.
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	// Set vsync.
	glfwSwapInterval(1);
	// Set keyboard callback.
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, character_callback);
	// Initialize scene.
	init();
	//initKinect();
	// Loop until the user closes the window.
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		//animation
		glfwMakeContextCurrent(window);
		render();
		glfwSwapBuffers(window);

		//kinect video
		//glfwMakeContextCurrent(kinectVideoOut);
		//renderVideo();
		//glfwSwapBuffers(kinectVideoOut);
		
		glfwPollEvents();
		while (glfwGetTime() < lastTime + 1.0/ FRAMERATE) { //frame limiter
			
		}
		lastTime += 1.0 / FRAMERATE;
	}
	// Quit program.
	glfwDestroyWindow(window);
	//glfwDestroyWindow(kinectVideoOut);
	glfwTerminate();

	// Garbage Cleanup of Hierarchy

	return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

	//Get the default Kinect sensor
	hr = GetDefaultKinectSensor(&sensor);

	BOOLEAN isSensorAvailable = false;
	hr = sensor->get_IsAvailable(&isSensorAvailable);
	if (SUCCEEDED(hr) && isSensorAvailable == false) {
		std::cerr << "No available sensor is found.\n";
	}

	cerr << sensor << "\n";
	//If the function succeeds, open the sensor
	if (SUCCEEDED(hr)) {
		hr = sensor->Open();
		
		if (SUCCEEDED(hr)) {
			//Get a body frame source from which we can get our body frame reader
			IBodyFrameSource* bodyFrameSource = nullptr;
			hr = sensor->get_BodyFrameSource(&bodyFrameSource);

			if (SUCCEEDED(hr)) {
				hr = bodyFrameSource->OpenReader(&bodyFrameReader);
			}

			//We're done with bodyFrameSource, so we'll release it
			safeRelease(bodyFrameSource);
		}
	}
	else {
		cout << "sensor not detected\n";
	}

	if (sensor == nullptr || FAILED(hr)) {
		std::cerr << "Cannot find any sensors.\n";
		return E_FAIL;
	}

	cout << "test" << endl;
	cerr << "test" << endl;

    test();

    // Initialize global strings
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadStringW(hInstance, IDC_MASTERFULLY, szWindowClass, MAX_LOADSTRING);
    //MyRegisterClass(hInstance);

    //// Perform application initialization:
    //if (!InitInstance (hInstance, nCmdShow))
    //{
    //    return FALSE;
    //}

    //HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MASTERFULLY));

    //MSG msg;

    //// Main message loop:
    //while (GetMessage(&msg, nullptr, 0, 0))
    //{
    //    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    //    {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //    }
    //}

    //return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MASTERFULLY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MASTERFULLY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
