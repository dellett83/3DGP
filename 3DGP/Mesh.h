#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

struct Vertex
{
	glm::vec3 positions;
	glm::vec2 texcoords;
	glm::vec3 normal;
};

struct Face
{
	Vertex a;
	Vertex b;
	Vertex c;
};


struct Mesh
{
private:
	GLuint m_VaoId;
	GLuint m_VboIds[3];
	bool m_Dirty;

	std::vector<Mesh> m_Faces;

public:

	Mesh();

	void addFace(const Face& _face);
	GLuint Id();



};

