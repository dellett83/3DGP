#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stdexcept>

#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texcoord;
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
	GLuint m_vaoid;
	GLuint m_vboid;
	bool m_dirty;

	std::vector<Face> m_faces;

public:

	Mesh();

	void addFace(const Face& _face);
	GLuint vao_id();
	GLsizei vertex_count() const;


};

