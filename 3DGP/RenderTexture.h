#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <exception>

struct RenderTexture
{
private:
	GLuint m_fboId;
	GLuint m_texId;
	GLuint m_rboId;
	int m_width;
	int m_height;
	RenderTexture(int _width, int _height);
	~RenderTexture();

public:
	void bind();
	void unbind();
	GLuint getTexture();

};

