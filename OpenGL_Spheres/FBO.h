#ifndef FBO_CLASS_H
#define FBO_CLASS_H

#include <glad/glad.h>
#include "global_params.h"

#include <vector>

class FBO {
public:
	GLuint ID;
	GLuint rbo;
	int scrWidth, scrHeight;
	std::vector<GLuint> texIDs;
	FBO();

	void Bind(GLenum mode = GL_FRAMEBUFFER);
	void Unbind();
	void Delete();

	void AttachTexture(GLuint texSlot, GLenum internalFormat, GLenum texFormat, GLenum pixelDataType, char* data, GLenum bindMode = GL_FRAMEBUFFER);
	//void AttachTexture(GLuint* texID, GLuint texSlot, GLenum internalFormat, GLenum texFormat, char* data, GLenum bindMode = GL_FRAMEBUFFER);
	void AttachRenderbuffer(GLenum format, GLenum attachTarget, GLenum bindMode = GL_FRAMEBUFFER);
	bool VerifyFramebuffer();
};

#endif