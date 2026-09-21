#include "FBO.h"
#include <iostream>

FBO::FBO() {
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	scrWidth = WIDTH;
	scrHeight = HEIGHT;

	while (texIDs.size() <= 32) {
		texIDs.emplace_back(0);	//add some dummy ints
	}

	////testing
	//texIDs[31] = 42;
	//bool success = texIDs.at(31) == 42 && *(&texIDs[0] + 31) == 42;
	//std::cout << "Sanity check passed?: " << success << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Bind(GLenum mode) {
	glBindFramebuffer(mode, ID);
}

void FBO::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Delete() {
	glDeleteFramebuffers(1, &ID);
}

void FBO::AttachTexture(GLuint texSlot, GLenum internalFormat, GLenum texFormat, GLenum pixelDataType, char* data, GLenum bindMode) {
	glBindFramebuffer(bindMode, ID);

	if (texSlot < 0) return;
	int cap = std::min((int)texSlot, 31);
	std::cout << "current tex number: " << cap << std::endl;
	while (texIDs.size() <= cap) {
		texIDs.emplace_back(0);	//add some dummy ints
	}

	//GLuint texID = texIDs.at(std::min((int)texSlot, 31));
	//GLuint* texIDAddr = &(texIDs.at(std::min((int)texSlot, 31)));
	GLuint* texIDAddr = &texIDs[0] + std::min((int)texSlot, 31);
	glGenTextures(1, texIDAddr);
	glBindTexture(GL_TEXTURE_2D, *texIDAddr);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, scrWidth, scrHeight, 0, texFormat, pixelDataType, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(bindMode, GL_COLOR_ATTACHMENT0 + cap, GL_TEXTURE_2D, *texIDAddr, 0);
	//std::cout << "Errors: " << glGetError() << std::endl;

	std::cout << "tex ID address: " << std::hex << "0x" << texIDAddr << std::dec << std::endl;
	std::cout << "tex ID: " << *texIDAddr << std::dec << std::endl;

	/*GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	std::cout << "Error Status: " << std::hex << status << std::dec << std::endl;*/

	glBindFramebuffer(bindMode, 0);
}

//void FBO::AttachTexture(GLuint* texID, GLuint texSlot, GLenum internalFormat, GLenum texFormat, char* data, GLenum bindMode = GL_FRAMEBUFFER) {
//	glBindFramebuffer(bindMode, ID);
//
//	glGenTextures(1, texID);
//	glBindTexture(GL_TEXTURE_2D, *texID);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scrWidth, scrHeight, 0, GL_RGBA, GL_FLOAT, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + texSlot, GL_TEXTURE_2D, *texID, 0);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}

void FBO::AttachRenderbuffer(GLenum format, GLenum attachTarget, GLenum bindMode) {
	glBindFramebuffer(bindMode, ID);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, format, scrWidth, scrHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachTarget, GL_RENDERBUFFER, rbo);

	glBindFramebuffer(bindMode, 0);
}

bool FBO::VerifyFramebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	bool check = status == GL_FRAMEBUFFER_COMPLETE;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (!check) {
		std::cout << "Error Status: " << std::hex << status << std::dec << std::endl;
	}
	else {
		std::cout << "Complete Framebuffer!" << std::endl;
	}
	return check;
}