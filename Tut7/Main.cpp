#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "LoaderShader.h"
#include "LoadTexture.h"

using namespace std;

//function prototypes
void renderScene();
void createCubeVertexBuffer();
void createCubeTextureCoordBuffer();
void initialiseGlutCallback();

GLuint cubeVertexBuffer;
GLuint cubeTextureCoordBuffer;
GLuint programID;
GLuint matrixID;
GLuint textureID;
GLuint texture;
glm::mat4 projection;
glm::mat4 view;
glm::mat4 cubeModel;
glm::mat4 cubeMVP;

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(500, 300);
	glutCreateWindow("Tutorial 07 - Adding Texture to Cube");

	initialiseGlutCallback();

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s\n", glewGetErrorString(res));

		return 1;
	}

	//need to be after glewInit(), otherwise got error
	programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

	//get a handle for our "MVP" uniform
	matrixID = glGetUniformLocation(programID, "MVP");

	//projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	//or, for an ortho camera :
	/*projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // In world coordinates*/

	//camera matrix
	view = glm::lookAt(
		glm::vec3(4, 3, -3), //camera is at (4, 3, -3), in World Space
		glm::vec3(0, 0, 0), //and looks at the origin
		glm::vec3(0, 1, 0)  //head is up (set to 0,-1,0 to look upside-down)
		);

	//model matrix for cube : an identity matrix (model will be at the origin)
	cubeModel = glm::mat4(1.0f);

	//our ModelViewProjection : multiplication of our 3 matrices
	//remember, matrix multiplication is the other way around
	cubeMVP = projection * view * cubeModel;

	//load texture
	texture = loadBMPTexture("texture.bmp");

	//get a handle for our "myTextureSampler" uniform
	textureID = glGetUniformLocation(programID, "myTextureSampler");

	//black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//enable depth test
	glEnable(GL_DEPTH_TEST);
	//accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	createCubeVertexBuffer();
	createCubeTextureCoordBuffer();

	glutMainLoop();

	return 0;
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//use the shader
	glUseProgram(programID);

	// Send our transformation to the currently bound shader, 
	// in the "cubeMVP" uniform
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &cubeMVP[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, cubeTextureCoordBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//draw cube
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glutSwapBuffers();
}

void createCubeVertexBuffer() {
	//3 consecutive vertices give a triangle.
	//a cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, 
	//and 12*3 vertices
	GLfloat vertices[] = {
		-1.0f, -1.0f, -1.0f,		// triangle 1 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,			// triangle 1 : end
		1.0f, 1.0f, -1.0f,			// triangle 2 : begin
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,			// triangle 2 : end
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	glGenBuffers(1, &cubeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void createCubeTextureCoordBuffer() {
	GLfloat textureCoords[] = {
		0.000059f, 1.0f - 0.000004f,
		0.000103f, 1.0f - 0.336048f,
		0.335973f, 1.0f - 0.335903f,
		1.000023f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.336024f, 1.0f - 0.671877f,
		0.667969f, 1.0f - 0.671889f,
		1.000023f, 1.0f - 0.000013f,
		0.668104f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.000059f, 1.0f - 0.000004f,
		0.335973f, 1.0f - 0.335903f,
		0.336098f, 1.0f - 0.000071f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.336024f, 1.0f - 0.671877f,
		1.000004f, 1.0f - 0.671847f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.668104f, 1.0f - 0.000013f,
		0.335973f, 1.0f - 0.335903f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.668104f, 1.0f - 0.000013f,
		0.336098f, 1.0f - 0.000071f,
		0.000103f, 1.0f - 0.336048f,
		0.000004f, 1.0f - 0.671870f,
		0.336024f, 1.0f - 0.671877f,
		0.000103f, 1.0f - 0.336048f,
		0.336024f, 1.0f - 0.671877f,
		0.335973f, 1.0f - 0.335903f,
		0.667969f, 1.0f - 0.671889f,
		1.000004f, 1.0f - 0.671847f,
		0.667979f, 1.0f - 0.335851f
	};

	glGenBuffers(1, &cubeTextureCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeTextureCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
}

void initialiseGlutCallback() {

	glutDisplayFunc(renderScene);
}
