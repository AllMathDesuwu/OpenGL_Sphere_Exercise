#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader {
public:
	//Reference ID for the Shader Program
	GLuint ID;
	//Constructor for the Shader Program that takes two paths to shader files
	Shader(const char* vertexFile, const char* fragmentFile);

	//Activate the Shader Program
	void Activate();
	//Delete the Shader Program
	void Delete();
private:
	void compileErrors(unsigned int shader, const char* type);
};

#endif