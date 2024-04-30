#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stdexcept>

#include <vector>

struct Model {};
struct Texture {};


struct Player
{
private:
	glm::vec3 m_movement;
	glm::vec3 m_rotation;

	Model* model;
	Texture* tex;

	bool m_dirty;

public:
	glm::vec3 getMovement() { return m_movement; };
	glm::vec3 getRotation() { return m_rotation; };

	Model& getModel();
	Texture& getTexture();

};