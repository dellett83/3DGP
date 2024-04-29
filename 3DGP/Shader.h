#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/ext.hpp>


#include <string>

struct Mesh;

struct Shader
{
private:
    GLuint m_id;
    GLuint v_id;
    GLuint f_id;
    
    std::string m_vertsrc;
    std::string m_fragsrc;

    bool m_dirty;
public:
    Shader(const std::string& _fragpath, const std::string& _vertpath);
    GLuint id();
    void uniform(const std::string& _name, const glm::mat4& value);
    void draw(const Mesh& mesh);
};
