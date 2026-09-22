#include "global_params.h"
#include <iostream>
#include "cmath"
#include "Mesh.h"
#include "Sphere.h"
#include "FBO.h"
#include "Quad.h"

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

	std::vector<Texture> earthTexs;
	Texture earthTex("earth_tex.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);
	earthTexs.push_back(earthTex);
	std::list<SimObject*> world;
	Sphere earth(96, 96, 6371000.0f, earthTexs, (float)(5.972 * std::pow(10, 24)), world);
	earth.Position = glm::vec3(0.0f, 0.0f, -10000000.0f);

	std::vector<Texture> moonTexs;
	Texture moonTex("moon_tex.png", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE);
	moonTexs.push_back(moonTex);
	Sphere moon(96, 96, 1737000.4f, moonTexs, (float)(7.346 * std::pow(10, 22)), world);
	moon.Position = glm::vec3(384784000.0f, 0.0f, -10000000.0f);
	moon.Velocity = glm::vec3(0.0f, 0.0f, 1017.8f);

	std::vector<Texture> testTexs;
	Texture testTex("test_tex.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);
	testTexs.push_back(testTex);
	Sphere testLight(96, 96, 100000000.0f, testTexs, 0.0f, world, true);
	testLight.Position = glm::vec3(384784000.0f, 100000.0f, 10000000.0f);
	testLight.Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	testLight.SetLightParams(0.0f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	std::cout << "Num Lights: " << SimObject::lights.size() << std::endl;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	/*glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);*/

	Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 100.0f));

	Shader shaderGeometryPass("gBuffer.vert", "gBuffer.frag");
	Shader shaderDeferredPass("deferredShading.vert", "deferredShading.frag");
	Shader shaderDeferredLights("deferredLightSource.vert", "deferredLightSource.frag");
	Shader hdrRender("hdrRender.vert", "hdrRender.frag");
	Shader basicGShader("basicGShader.vert", "basicGShader.frag");
	Shader testRender("testQuad.vert", "testQuad.frag");

	//std::cout << glGetError() << std::endl << std::endl;
	FBO gBuff;
	gBuff.AttachTexture(0, GL_RGB16F, GL_RGB, GL_FLOAT, NULL);	//Position
	gBuff.AttachTexture(1, GL_RGB16F, GL_RGB, GL_FLOAT, NULL);	//Normal
	gBuff.AttachTexture(2, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, NULL);		//Color + Specular
	//gBuff.Bind();
	gBuff.AttachRenderbuffer(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 , GL_COLOR_ATTACHMENT2};
	gBuff.Bind();
	glDrawBuffers(3, attachments);
	gBuff.Unbind();
	/*int code = glGetError();
	if (code != 0) {
		std::cout << "Error: " << code << std::endl;
	}
	else {
		std::cout << "No errors making Framebuffer!" << std::endl;
	}*/
	gBuff.VerifyFramebuffer();

	FBO hdrBuff;
	hdrBuff.AttachTexture(0, GL_RGB16F, GL_RGB, GL_FLOAT, NULL);
	hdrBuff.AttachRenderbuffer(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	hdrBuff.Bind();
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	hdrBuff.Unbind();
	hdrBuff.VerifyFramebuffer();
	
	//FBO ppBuffs[2];	//pp is for ping-pong...
	//for (int i = 0; i < 2; i++) {
	//	ppBuffs[i].AttachTexture(0, GL_RGBA16F, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//}

	//shaderDeferredPass.Activate();
	/*glUniform1i(glGetUniformLocation(shaderDeferredPass.ID, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(shaderDeferredPass.ID, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(shaderDeferredPass.ID, "gAlbedoSpec"), 2);*/
	/*GLuint* gPosition = &(gBuff.texIDs[0]);
	GLuint* gNormal = &(gBuff.texIDs[1]);
	GLuint* gAlbedoSpec = &(gBuff.texIDs[2]);
	shaderDeferredPass.Activate();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, *gAlbedoSpec);*/

	Quad screenQuad;

	float prevTime = (float) glfwGetTime();
	float curTime;
	float secCounter = 0;
	//main while loop
	while (!glfwWindowShouldClose(window)) {
		curTime = glfwGetTime();
		float deltaTime = curTime - prevTime;
		secCounter += deltaTime;

		//specify background color
		//glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		//Clean the back buffer and assign the new color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//clear front buffer

		//beginning of geometry pass
		gBuff.Bind();	//comment/uncomment this line to get diagnostic stuffs to appear...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 1000.0f);
		
		earth.update(deltaTime);
		earth.Draw(shaderGeometryPass, camera);
		if (secCounter >= 1.0f) {
			glm::vec3 diff = moon.Position - earth.Position;
			std::cout << "Distance: " << std::sqrtf(glm::dot(diff, diff)) << std::endl;
			std::cout << "Speed: " << std::sqrtf(glm::dot(moon.Velocity, moon.Velocity)) << std::endl;
			/*std::cout << moon.Position.x << " " << moon.Position.y << " " << moon.Position.z << std::endl;
			std::cout << moon.Velocity.x << " " << moon.Velocity.y << " " << moon.Velocity.z << std::endl;
			std::cout << moon.Acceleration.x << " " << moon.Acceleration.y << " " << moon.Acceleration.z << std::endl;*/
			//std::cout << std::sqrt(glm::dot(moon.SumOfForces, moon.SumOfForces)) << std::endl;
			std::cout << std::endl;

			secCounter -= 1.0f;
		}
		moon.update(deltaTime);
		moon.Draw(shaderGeometryPass, camera);

		testLight.Draw(shaderGeometryPass, camera);
		gBuff.Unbind();

		//goto end_of_render_cycle;

		//lighting pass
		hdrBuff.Bind(); //begin collecting raw color values into HDR buff
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderDeferredPass.Activate();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBuff.texIDs.at(0));	//Position
		glUniform1i(glGetUniformLocation(shaderDeferredPass.ID, "gPosition"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gBuff.texIDs.at(1));	//Normal
		glUniform1i(glGetUniformLocation(shaderDeferredPass.ID, "gNormal"), 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBuff.texIDs.at(2));	//Albedo + Spec
		glUniform1i(glGetUniformLocation(shaderDeferredPass.ID, "gAlbedoSpec"), 2);
		//std::cout << gBuff.texIDs.at(2) << std::endl;
		//glActiveTexture(GL_TEXTURE0);
		// << std::endl;

		//to appease the linter...
		//const int MAX_LIGHTS = 1;
		//int outerLim = (int)ceil(SimObject::lights.size() / ((float)MAX_LIGHTS));

		////sanity check?
		//goto end_of_render_cycle;	//this also has to be present for stuffs to appear...

		//ppBuffs[0].Bind();
		
		auto lightIter = SimObject::lights.begin();
		for (int j = 0; j < SimObject::lights.size(); j++) {
			SimObject* curLight = *lightIter;
			if (lightIter == SimObject::lights.end() || !curLight) break;
			glUniform1i(glGetUniformLocation(shaderDeferredPass.ID, "numLights"), 1);

			std::string prefix = "lights[" + std::to_string(j);

			std::string pos = prefix + "].Position";
			glUniform3f(glGetUniformLocation(shaderDeferredPass.ID, pos.c_str()), curLight->Position.x / 10000000.0f, curLight->Position.y / 10000000.0f, curLight->Position.z / 10000000.0f);
			std::string color = prefix + "].Color";
			glUniform3f(glGetUniformLocation(shaderDeferredPass.ID, color.c_str()), curLight->light.Color.x, curLight->light.Color.y, curLight->light.Color.z);
			std::string linear = prefix + "].Linear";
			glUniform1f(glGetUniformLocation(shaderDeferredPass.ID, linear.c_str()), curLight->light.Linear);
			std::string quadratic = prefix + "].Quadratic";
			glUniform1f(glGetUniformLocation(shaderDeferredPass.ID, quadratic.c_str()), curLight->light.Quadratic);

			std::advance(lightIter, 1);
		}
		screenQuad.Draw();
		//risky stuff
		shaderDeferredLights.Activate();
		glBindTexture(GL_TEXTURE_2D, testTexs[0].ID);	//Position
		glm::vec3 lightColor = testLight.light.Color;
		glUniform3f(glGetUniformLocation(shaderDeferredLights.ID, "lightColor"), lightColor.r, lightColor.g, lightColor.b);
		glActiveTexture(GL_TEXTURE0);
		testLight.Draw(shaderDeferredLights, camera);	//draw the light source on top
		//end risky stuff

		/*for (int i = 0; i < outerLim; i++) {
			int j;
			for (j = 0; j < MAX_LIGHTS; j++) {
				SimObject* curLight = *lightIter;
				if (lightIter == SimObject::lights.end() || !curLight) break;
				std::string prefix = "lights[" + std::to_string(j);

				std::string pos = prefix + "].Position";
				glUniform3f(glGetUniformLocation(shaderDeferredPass.ID, pos.c_str()), curLight->Position.x / 10000000.0f, curLight->Position.y / 10000000.0f, curLight->Position.z / 10000000.0f);
				std::string color = prefix + "].Color";
				glUniform3f(glGetUniformLocation(shaderDeferredPass.ID, color.c_str()), curLight->light.Color.x, curLight->light.Color.y, curLight->light.Color.z);
				std::string linear = prefix + "].Linear";
				glUniform1f(glGetUniformLocation(shaderDeferredPass.ID, linear.c_str()), curLight->light.Linear);
				std::string quadratic = prefix + "].Quadratic";
				glUniform1f(glGetUniformLocation(shaderDeferredPass.ID, quadratic.c_str()), curLight->light.Quadratic);

				std::advance(lightIter, 1);
			}
			glUniform3f(glGetUniformLocation(shaderDeferredPass.ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
			glUniform1i(glGetUniformLocation(shaderDeferredPass.ID, "numLights"), j + 1);
			screenQuad.Draw();
		}*/
		hdrBuff.Unbind();

		//do post-processing for HDR
		hdrBuff.Unbind();
		//std::cout << glGetError() << std::endl;
		glClear(GL_COLOR_BUFFER_BIT);
		hdrRender.Activate(); 
		glUniform1i(glGetUniformLocation(hdrRender.ID, "hdrBuffer"), 0);	//texture stored in 0th color attachment
		glUniform1f(glGetUniformLocation(hdrRender.ID, "exposure"), 2.0f);
		glActiveTexture(GL_TEXTURE0);	//probably works because textures attached to GL_TEXTURE0 by default???
		glBindTexture(GL_TEXTURE_2D, hdrBuff.texIDs.at(0));

		screenQuad.Draw();
		hdrBuff.Unbind();
		//ppBuffs[0].Unbind();

		//Draw in your lights
		//gBuff.Bind(GL_READ_FRAMEBUFFER);


		//At this point you should start to ping pong to achieve bloom

		//And then apply some kind of tonemapping for HDR so things can be reasonably rendered to screen

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		////hdrRender.Activate();
		//testRender.Activate();
		//glUniform1i(glGetUniformLocation(testRender.ID, "buff"), 0);
		//glActiveTexture(GL_TEXTURE0);
		////glBindTexture(GL_TEXTURE_2D, ppBuffs[0].texIDs[0]);
		//glBindTexture(GL_TEXTURE_2D, gBuff.texIDs.at(0));
		////glUniform1i(glGetUniformLocation(hdrRender.ID, "hdrBuffer"), 0);
		////glUniform1f(glGetUniformLocation(hdrRender.ID, "exposure"), 1.0);
		//screenQuad.Draw();

		end_of_render_cycle:
		//swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		//Take care of all GLFW events
		glfwPollEvents();

		prevTime = curTime;
	}

	//Delete the objects we've created
	shaderProgram.Delete();
	shaderGeometryPass.Delete();
	shaderDeferredPass.Delete();
	shaderDeferredLights.Delete();
	hdrRender.Delete();
	testRender.Delete();
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