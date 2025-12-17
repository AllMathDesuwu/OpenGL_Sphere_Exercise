#include <iostream>
#include "cmath"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_img.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

#define WIDTH 800
#define HEIGHT 800

void generateUVSphere(int numSectors, int numStacks, float radius, GLfloat* vertOut, GLuint* indxOut);
void generateCircle(int numSectors, float radius, GLfloat* vertOut, GLuint* indxOut);

int main() {
	//Initialize GLFW
	glfwInit();

	//Tell GLFW what version of OpenGL we are using
	//using version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Using CORE profile (not legacy)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//vertex coordinates
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3,		0.0f,	0.8f, 0.3f,		0.02f,	0.0f, 0.0f,	//lower left
		 0.5f, -0.5f * float(sqrt(3)) / 3,		0.0f,	0.8f, 0.3f,		0.02f,	0.0f, 0.0f,	//lower right
		 0.0f,  0.5f * float(sqrt(3)) * 2 / 3,	0.0f,	1.0f, 0.6f,		0.32f,	0.0f, 0.0f,	//top
		-0.25f, 0.5f * float(sqrt(3)) / 6,		0.0f,	0.9f, 0.45f,	0.17f,	0.0f, 0.0f,	//inner left
		 0.25f, 0.5f * float(sqrt(3)) / 6,		0.0f,	0.9f, 0.45f,	0.17f,	0.0f, 0.0f,//inner right
		 0.0f, -0.5f * float(sqrt(3)) / 3,		0.0f,	0.8f, 0.3f,		0.02f,	0.0f, 0.0f//inner bottom
	};

	GLuint indices[] =
	{
		0, 3, 5,	//lower left triangle
		3, 2, 4,	//lower right triangle
		5, 4, 1		//upper triangle
	};

	//Create a GLFWwindow object of 800x800 pixels naming it "Tutorial"
	GLFWwindow* window = glfwCreateWindow(800, 800, "Tutorial", NULL, NULL);
	//Null check
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Introduce window into current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	//Specify the viewport of OpenGL in the Window
	//In this case viewport goes from x = 0, y = 0 to x = 800, y = 800
	glViewport(0, 0, WIDTH, HEIGHT);

	//Creates Shader object using shader files
	Shader shaderProgram("default.vert", "default.frag");

	//Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	//Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	//Gnerates Vertex Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	//Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//Unbind all to avoid accidentally changing them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	Shader shaderProgram2("pointCloud.vert", "pointCloud.frag");

	VAO VAO2;
	VAO2.Bind();

	const int NUM_STACKS = 24;
	const int NUM_SECTORS = 24;
	const int numVertices = (NUM_STACKS - 1) * NUM_SECTORS + 2;
	const int numTris = 2 * (NUM_SECTORS) + (NUM_STACKS - 2) * (2 * NUM_SECTORS);	//first term is for the Northern and Southern-most sectors-- aka the ones that can be reprsented by a single triangle
	//the second term represents the rest of the circles-- the ones that are squares and therefore require two triangles to represent them

	GLfloat sphereVertexArray[numVertices * 3];
	GLuint sphereIndexArray[numTris * 3];
	generateUVSphere(NUM_SECTORS, NUM_STACKS, 1.0f, sphereVertexArray, sphereIndexArray);
	VBO VBO2(sphereVertexArray, sizeof(sphereVertexArray));
	EBO EBO2(sphereIndexArray, sizeof(sphereIndexArray));

	/*for (int i = 0; i < numTris; i++) {
		if ((i == NUM_SECTORS || (i - NUM_SECTORS) % (2 * NUM_SECTORS) == 0) && i != 0) {
			std::cout << std::endl;
		}
		for (int j = 0; j < 3; j++) {
			std::cout << sphereIndexArray[3 * i + j] << " ";
		}
		std::cout << std::endl;
	}*/
	std::cout << std::endl;
	/*GLfloat circleVertices[NUM_SECTORS * 3];
	generateCircle(NUM_SECTORS, 5.0f, circleVertices, NULL);
	VBO VBO2(circleVertices, sizeof(circleVertices));*/

	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 3 * sizeof(GLfloat), (void*)0);
	VAO2.Unbind();
	VBO2.Unbind();	
	EBO2.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

	//main while loop
	while (!glfwWindowShouldClose(window)) {
		//specify background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//Clean the back buffer and assign the new color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//clear front buffer
		//Tell OpenGL which Shader Program to use
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);

		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
		//Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		//Draw a triangle using the GL_TRIANGLES primitive
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		VAO1.Unbind();

		shaderProgram2.Activate();
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram2, "camMatrix");
		VAO2.Bind();
		//glDrawArrays(GL_POINTS, 0, numVertices);
		glDrawElements(GL_TRIANGLES, numTris * 4, GL_UNSIGNED_INT, 0);
		VAO2.Unbind();

		//swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		//Take care of all GLFW events
		glfwPollEvents();


	}

	//Delete the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	VAO2.Delete();
	VBO2.Delete();
	shaderProgram2.Delete();

	//Delete window before ending program
	glfwDestroyWindow(window);
	//Terminate GLFW before ending program
	glfwTerminate();
	return 0;
}

void generateUVSphere(int numSectors, int numStacks, float radius, GLfloat* vertOut, GLuint* indxOut) {
	const double pi = 2 * acos(0.0);

	int  index = 0;	//index counts position in array (divide by three gives vertex), subindex gives x, y, or z
	for (int i = 0; i <= numStacks; i++) {	//number of vertical units
		float phi = (90 - 180 * (i / (float)numStacks)) * (pi / 180);
		float y = radius * sinf(phi);
		float adjustedRadius = radius * cosf(phi);
		for (int j = 0; j < numSectors; j++) {
			float theta = (360 * (j / (float)numSectors)) * (pi / 180);
			float x = adjustedRadius * cosf(theta);
			float z = adjustedRadius * sinf(theta);

			vertOut[index] = x;
			vertOut[index + 1] = y;
			vertOut[index + 2] = z;
			index += 3;

			if (i == 0 || i == numStacks) break;
		}
	}

	index = 0;	//represents the index of the current float in the array
	int vertexIdx = 0;	//represents the index of the current vertex
	const int NEXT_STACK = numSectors;	//only works in general case--not when cur layer or next layer doesn't have proper number of vertices
	for (int i = 0; i < numStacks; i++) {
		int firstIdxInStack = index;
		int offset = 0;
		int vertexOffset = 0;
		if (!(i == 0 || i == numStacks - 1)) {
			for (int j = 0; j < numSectors; j++) {
				//upper triangle
				int tempIdx = index + offset;
				indxOut[tempIdx] = vertexIdx + vertexOffset;
				indxOut[tempIdx + 1] = vertexIdx + NEXT_STACK + vertexOffset;
				indxOut[tempIdx + 2] = vertexIdx + ((vertexOffset + 1) % numSectors); //modulo in case it wraps around

				//lower triangle
				indxOut[tempIdx + 3] = vertexIdx + ((vertexOffset + 1) % numSectors);
				indxOut[tempIdx + 4] = vertexIdx + NEXT_STACK + vertexOffset;
				indxOut[tempIdx + 5] = vertexIdx + NEXT_STACK + ((vertexOffset + 1) % numSectors);

				offset += 6;
				vertexOffset++;
			}
			index = firstIdxInStack + offset;
			vertexIdx += vertexOffset;
		}
		else if (i == 0) {
			for (int j = 0; j < numSectors; j++) {
				int tempIdx = index + offset;
				indxOut[tempIdx] = vertexIdx;
				indxOut[tempIdx + 1] = vertexIdx + 1 + (vertexOffset % numSectors);
				indxOut[tempIdx + 2] = vertexIdx + 1 + ((vertexOffset + 1) % numSectors);

				offset += 3;
				vertexOffset++;
			}
			index = firstIdxInStack + offset;
			vertexIdx++;
		}
		else {
			for (int j = 0; j < numSectors; j++) {
				int tempIdx = index + offset;
				indxOut[tempIdx] = vertexIdx + vertexOffset;
				indxOut[tempIdx + 1] = vertexIdx + NEXT_STACK;	//DON'T ADD VERTEX OFFSET because all of these triangles will share this vertex-- the "Southern"-most vertex regardless of offset from the first index of the stack
				indxOut[tempIdx + 2] = vertexIdx + ((vertexOffset + 1) % numSectors);

				offset += 3;
				vertexOffset++;
				//TODO: FIX THIS!!! Remember, you want the index of the VERTEX, not the index of an individual element fo a vertex.
			}
			//keeping track of these at this point probably doesn't matter much
			index = firstIdxInStack + offset;
			vertexIdx += vertexOffset;
		}
	}
}

//testing my understanding of math to debug the above...
//turns out I did attribute linking to the wrong VBO... oops
void generateCircle(int numSectors, float radius, GLfloat* vertOut, GLuint* indxOut) {
	const double pi = 2 * acos(0.0);

	int  index = 0;	//index counts position in array (divide by three gives vertex), subindex gives x, y, or z
	for (int j = 0; j < numSectors; j++) {
		float theta = (360 * (j / (float)numSectors)) * (pi / 180);
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);

		vertOut[index] = x;
		std::cout << x << " ";
		vertOut[index + 1] = 0;
		std::cout << 0 << " ";
		vertOut[index + 2] = y;
		std::cout << y << " ";

		std::cout << std::endl;
		index += 3;
	}
	std::cout << std::endl;
}