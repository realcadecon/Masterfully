// Masterfully.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Masterfully.h"

#define _USE_MATH_DEFINES
#define PI 3.14159265
#include <cmath>
#include <iostream>
#include <vector>
#include <stack>


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
#include <inc/Kinect.h>

using namespace std;

GLFWwindow* window; // Main application window
string RES_DIR = "./resources/"; // Where data files live
shared_ptr<Program> prog;
shared_ptr<Shape> shape;
shared_ptr<Shape> sphere;
shared_ptr<Component> root;
static vector<shared_ptr<Component>> dfsOrder;
static int pos = 0;
static vector<int> spinners = { 2, 5 };

IKinectSensor* sensor = nullptr;
IBodyFrameReader* bodyFrameReader = nullptr;
HRESULT hr;
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
	if (codepoint == 46) { ++pos; pos = pos % dfsOrder.size(); } // .
	if (codepoint == 44) { --pos; if (pos == -1) pos = dfsOrder.size() - 1; pos = pos % dfsOrder.size(); } // ,
	if (codepoint == 120) { dfsOrder[pos]->ang += glm::vec3(0.1, 0, 0); } // x
	if (codepoint == 88) { dfsOrder[pos]->ang += glm::vec3(-0.1, 0, 0); } // X
	if (codepoint == 121) { dfsOrder[pos]->ang += glm::vec3(0, 0.1, 0); } // y
	if (codepoint == 89) { dfsOrder[pos]->ang += glm::vec3(0, -0.1, 0); } // Y
	if (codepoint == 122) { dfsOrder[pos]->ang += glm::vec3(0, 0, 0.1); } // z
	if (codepoint == 90) { dfsOrder[pos]->ang += glm::vec3(0, 0, -0.1); } // Z
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


	// Initialize the GLSL program.
	prog = make_shared<Program>();
	prog->setVerbose(true);
	prog->setShaderNames(RES_DIR + "simple_vert.glsl", RES_DIR + "simple_frag.glsl");
	prog->init();
	prog->addUniform("P");
	prog->addUniform("MV");
	prog->addAttribute("aPos");
	prog->addAttribute("aNor");
	prog->setVerbose(false);

	// initialize root component
	root = make_shared<Component>("root");
	root->scale = glm::vec3(2, 3, 1);

	auto head = make_shared<Component>("head");
	head->tp = glm::vec3(0, 1.5, 0);
	head->tm = glm::vec3(0, 0.6, 0);
	head->scale = glm::vec3(1.2, 1.2, 1.2);
	root->addComponent(head);

	auto larm = make_shared<Component>("left arm");
	larm->tp = glm::vec3(-1, 1.15, 0);
	larm->tm = glm::vec3(0, -1, 0);
	larm->scale = glm::vec3(0.5, 2, 0.5);
	larm->ang = glm::vec3(0, 0, -0.5 * PI);
	auto llarm = make_shared<Component>("lower left arm");
	llarm->tp = glm::vec3(0, -2, 0);
	llarm->tm = glm::vec3(0, -0.75, 0);
	llarm->scale = glm::vec3(0.4, 1.5, 0.4);
	larm->addComponent(llarm);
	root->addComponent(larm);

	auto rarm = make_shared<Component>("right arm");
	rarm->tp = glm::vec3(1, 1.15, 0);
	rarm->tm = glm::vec3(0, -1, 0);
	rarm->scale = glm::vec3(0.5, 2, 0.5);
	rarm->ang = glm::vec3(0, 0, 0.5 * PI);
	auto lrarm = make_shared<Component>("lower right arm");
	lrarm->tp = glm::vec3(0, -2, 0);
	lrarm->tm = glm::vec3(0, -0.75, 0);
	lrarm->scale = glm::vec3(0.4, 1.5, 0.4);
	rarm->addComponent(lrarm);
	root->addComponent(rarm);

	auto lleg = make_shared<Component>("left leg");
	lleg->tp = glm::vec3(-0.5, -1.5, 0);
	lleg->tm = glm::vec3(0, -1.125, 0);
	lleg->scale = glm::vec3(0.9, 2.25, 0.9);
	auto llleg = make_shared<Component>("lower left leg");
	llleg->tp = glm::vec3(0, -2.25, 0);
	llleg->tm = glm::vec3(0, -1, 0);
	llleg->scale = glm::vec3(0.8, 2, 0.8);
	lleg->addComponent(llleg);
	root->addComponent(lleg);

	auto rleg = make_shared<Component>("right leg");
	rleg->tp = glm::vec3(0.5, -1.5, 0);
	rleg->tm = glm::vec3(0, -1.125, 0);
	rleg->scale = glm::vec3(0.9, 2.25, 0.9);
	auto lrleg = make_shared<Component>("lower right leg");
	lrleg->tp = glm::vec3(0, -2.25, 0);
	lrleg->tm = glm::vec3(0, -1, 0);
	lrleg->scale = glm::vec3(0.8, 2, 0.8);
	rleg->addComponent(lrleg);
	root->addComponent(rleg);


	if (root->children.size() > 0) {
		stack<shared_ptr<Component>> frontier;
		frontier.push(root);
		while (!frontier.empty()) {
			shared_ptr<Component> cur = frontier.top();
			frontier.pop();
			dfsOrder.push_back(cur);
			int sz = cur->children.size();
			for (int i = sz - 1; i >= 0; --i) {
				frontier.push(cur->children[i]);
			}
		}
	}

	// If there were any OpenGL errors, this will print something.
	// You can intersperse this line in your code to find the exact location
	// of your OpenGL error.
	GLSL::checkError(GET_FILE_LINE);
}

static void render()
{
	IBodyFrame* bodyFrame = nullptr;
	hr = bodyFrameReader->AcquireLatestFrame(&bodyFrame);
	vector<Joint*> bodyJoints;

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
					bodyJoints.push_back(joints);
					//Let's print the head's position
					const CameraSpacePoint& headPos = joints[JointType_Head].Position;
					const CameraSpacePoint& leftHandPos = joints[JointType_HandLeft].Position;

					//Let's check if the use has his hand up
					if (leftHandPos.Y >= headPos.Y) {
						std::cout << "LEFT HAND UP!!\n";
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
	// Get current frame buffer size.
	int width, height;
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
	MV->translate(glm::vec3(0, 1, -12));

	// Draw mesh using GLSL.
	prog->bind();
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);
	/*double t = glfwGetTime();
	double mod = 1 + (0.05 / 2) + (0.05 / 2) * sin(2 * PI * 2 * t);
	root->draw(MV, prog, shape, sphere, dfsOrder[pos], mod, dfsOrder, spinners);*/
	MV->pushMatrix();
	for (auto x : bodyJoints) {
		for(int i = 0; i < JointType_Count; i++){
			if (x[i].TrackingState == 0)
				continue;
			MV->translate(x[i].Position.X, x[i].Position.Y, x[i].Position.Z);
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			sphere->draw(prog);
		}
	}
	MV->popMatrix();
	prog->unbind();

	// Pop matrix stacks.
	MV->popMatrix();
	P->popMatrix();

	GLSL::checkError(GET_FILE_LINE);
}

int test()
{

	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if (!glfwInit()) {
		return -1;
	}
	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(640, 480, "YOUR NAME", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
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
	// Loop until the user closes the window.
	while (!glfwWindowShouldClose(window)) {
		// Render scene.
		render();
		// Swap front and back buffers.
		glfwSwapBuffers(window);
		// Poll for and process events.
		glfwPollEvents();
	}
	// Quit program.
	glfwDestroyWindow(window);
	glfwTerminate();
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

	if (sensor == nullptr || FAILED(hr)) {
		std::cerr << "Cannot find any sensors.\n";
		return E_FAIL;
	}
	cout << "test" << endl;
    test();

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MASTERFULLY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MASTERFULLY));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
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
