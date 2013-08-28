#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util.h"
#include "inputs.h"

static glm::mat4 camera;
static glm::mat4 projection;

static float horizontalAngle, verticalAngle;
static float fov = 90.0f;

static float speed = 3.0f;
float mouseSpeed = 0.005;

static const float PI = 3.1415926;
static const float PIO2 = PI/2;

static glm::vec3 cameraPosition(0, 0, -5);

void computeMatrixFromInput()
{
	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	
	float deltaTime = (float)(currentTime - lastTime);

	int windowWidth, windowHeight;
	glfwGetWindowSize(&windowWidth, &windowHeight);

	//P("window width=%d, height=%d\n", windowWidth, windowHeight);

	int mouseX, mouseY;
	glfwGetMousePos(&mouseX, &mouseY);

	//P("mouseX=%d, mouseY=%d\n", mouseX, mouseY);
	horizontalAngle = mouseSpeed*(mouseX - windowWidth/2);
	verticalAngle = mouseSpeed*(windowHeight/2-mouseY);


	glm::vec3 direction(cos(verticalAngle)*sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle)*cos(horizontalAngle));

	fov = 90 - 5*glfwGetMouseWheel();


	glm::vec3 right = glm::vec3(sin(horizontalAngle+PIO2), 0, cos(horizontalAngle+PIO2));

	glm::vec3 up = glm::cross(right, direction);
	camera = glm::lookAt(cameraPosition, cameraPosition+direction, up);
	projection = glm::perspective(fov, 4.0f/3.0f, 0.1f, 100.0f);
}

glm::mat4 getCameraMatrix()
{
	return camera;
}

glm::mat4 getProjectionMatrix()
{
	return projection;
}
