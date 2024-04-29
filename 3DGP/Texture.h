#include <GL/glew.h>

#include <vector>
#include <string>



struct Texture
{
private:
	int m_Width; 
	int m_Height;
	bool m_Dirty;

	GLuint m_Id;

	std::vector<unsigned char> m_Data;

public:
	Texture();
	Texture(const std::string& _path);

	GLuint getId();


};