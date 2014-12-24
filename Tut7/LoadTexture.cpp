#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "LoadTexture.h"

using namespace std;

GLuint loadBMPTexture(const char *imagePath) {
	cout << "Reading image" << imagePath << endl;

	//data read from the header of the BMP file
	unsigned char header[54];	//each BMP file begins by a 54-bytes header
	unsigned int dataPos;		//position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;		// = width*height*3
	// Actual RGB data
	unsigned char *data;

	//read file as a binary file, hence "rb" as mode
	FILE *file;
	errno_t err;
	err = fopen_s(&file, imagePath, "rb");
	if (err == 0) {
		cout << "Image was opened" << endl;
	}
	else {
		cout << "Image could not be opened" << endl;
		return 0;
	}
		
	//read the header, i.e the first 54 bytes
	//if less than 54 bytes are read, there is a problem
	if (fread(header, 1, 54, file) != 54) {
		cout << "Not a correct .BMP file" << endl;
		return 0;
	}

	//a .BMP file always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		cout << "Not a correct .BMP file" << endl;
		return 0;
	}

	//read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	//some .BMP files are misformatted, guess missing information
	if (imageSize == 0)
		imageSize = width * height * 3;		//3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)
		dataPos = 54;						//the .BMP header is done that way

	//create a buffer
	data = new unsigned char[imageSize];

	//read the actual data from file to buffer
	fread(data, 1, imageSize, file);

	//must always close the file at the end
	fclose(file);

	//ceate one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	//"bind" the newly created texture
	//all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	//give the image to OpenGL
	//.BMP files store colour as Blue, Green, Red, hence GL_BGR
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return textureID;
}