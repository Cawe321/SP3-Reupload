
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SceneBasic.h"
#include "SceneIntro.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
double Application::mouse_last_x = 0.0, Application::mouse_last_y = 0.0,
Application::mouse_current_x = 0.0, Application::mouse_current_y = 0.0,
Application::mouse_diff_x = 0.0, Application::mouse_diff_y = 0.0;
double Application::camera_yaw = 0.0, Application::camera_pitch = 0.0;

int m_window_width = 800;
int m_window_height = 600;

Application::SCENETYPE Application::eSceneType = INTRO;
Application::SCENETYPE Application::eOldSceneType = INTRO;
bool Application::bIntroDisplayRendered = false;
Application::PASSVALUE Application::sPassValue = {"Nameless", false};

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	m_window_width = w;
	m_window_height = h;
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool Application::GetMouseUpdate()
{
	glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);

	// Calculate the difference in positions
	mouse_diff_x = mouse_current_x - mouse_last_x;
	mouse_diff_y = mouse_current_y - mouse_last_y;

	//Calculate the yaw and pitch

	if (Application::IsMousePressed(1))
	{
		camera_yaw = (float)mouse_diff_x * 0.0174555555555556f;// * 3.142f / 180.0f;
		camera_pitch = mouse_diff_y * 0.0174555555555556f;// 3.142f / 180.0f );
	}
	else
	{
		camera_yaw = 0;
		camera_pitch = 0;
	}


	//// Do a wraparound if the mouse cursor has gone out of the deadzone
	//if ((mouse_current_x < m_window_deadzone) || (mouse_current_x > m_window_width-m_window_deadzone))
	//{
	//	mouse_current_x = m_window_width >> 1;
	//	glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	//}
	//if ((mouse_current_y < m_window_deadzone) || (mouse_current_y > m_window_height-m_window_deadzone))
	//{
	//	mouse_current_y = m_window_height >> 1;
	//	glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	//}

	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;

	return false;
}

bool Application::IsMousePressed(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
	return glfwGetMouseButton(m_window, key) != 0;
}
void Application::GetCursorPos(double* xpos, double* ypos)
{
	glfwGetCursorPos(m_window, xpos, ypos);
}
GLFWwindow* Application::GetWindow()
{
	return m_window;
}
int Application::GetWindowWidth()
{
	return m_window_width;
}
int Application::GetWindowHeight()
{
	return m_window_height;
}

void Application::SetSceneType(SCENETYPE eSceneType)
{
	Application::eSceneType = eSceneType;
}

void Application::SetIntroDisplayRendered(bool bIntroDisplayRendered)
{
	Application::bIntroDisplayRendered = bIntroDisplayRendered;
}

bool Application::GetIntroDisplayRendered()
{
	return bIntroDisplayRendered;
}

const void Application::SetMouseSeen(bool bSeen)
{
	if (bSeen)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, "Patient Zero", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
	
	// Hide the cursor
	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Application::Run()
{
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame

	Scene* cTempStore = NULL;
	while (eSceneType != EXIT)
	{
		Scene* scene;
		//If this is the first scene being inited
		if (cTempStore == NULL) 
		{

			if (eSceneType == INTRO)
			{
				eOldSceneType = INTRO;
				std::cout << "*****Loading SceneIntro*****\n";
				scene = new SceneIntro();
				scene->Init();
			}
			else if (eSceneType == GAME)
			{
				eOldSceneType = GAME;
				std::cout << "*****Loading SceneBasic*****\n";
				scene = new SceneBasic();
				scene->Init();
			}
			else
			{
				break; // Code should never run here, but for safety precautions
			}
		}
		else //Already inited the scene
		{
			//Transfer scene from storage to scene pointer variable
			scene = cTempStore;
			cTempStore = NULL;
		}

		//Main gameplay loop
		while (eSceneType == eOldSceneType)
		{
			if (IsKeyPressed(VK_ESCAPE) || glfwWindowShouldClose(m_window))
			{
				eSceneType = EXIT;
			}

			GetMouseUpdate();
			scene->Update(m_timer.getElapsedTime());
			scene->Render();
			//Swap buffers
			glfwSwapBuffers(m_window);
			//Get and organize events, like keyboard and mouse input, window resizing, etc...
			glfwPollEvents();
			//m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

		} //Check if the ESC key had been pressed or if the window had been closed

		//Init new scene
		if (eSceneType != EXIT)
		{
			bool bFinishInit = false;
			scene->DeleteInstances(); //So singletons wont get deleted later when inited in the new scene
			if (eSceneType == INTRO)
			{
				eOldSceneType = INTRO;
				std::cout << "*****Loading SceneIntro*****\n";
				cTempStore = new SceneIntro();

			}
			else if (eSceneType == GAME)
			{
				eOldSceneType = GAME;
				std::cout << "*****Loading SceneBasic*****\n";
				cTempStore = new SceneBasic();
			}

			double dCurrentVal = 0;
			double dMaxVal = 100;
			string sMessage;
			while (bFinishInit == false)
			{
				bFinishInit = cTempStore->InitSections(dCurrentVal, dMaxVal, sMessage);
				scene->RenderNextProgressBar(m_timer.getElapsedTime(), dCurrentVal, dMaxVal, sMessage);
				glfwSwapBuffers(m_window);
				//Get and organize events, like keyboard and mouse input, window resizing, etc...
				glfwPollEvents();
			}
		}
		else
		{
			scene->DeleteInstances();
		}

		scene->Exit(); //Close old scene
		delete scene;
	}
	
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();

	_CrtDumpMemoryLeaks();
}
