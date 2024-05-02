attribute vec3 a_Position;     
attribute vec2 a_TexCoord;
attribute vec3 a_Normal;     
uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
varying vec2 v_TexCoord;
varying vec3 v_Normal;
varying vec3 v_FragPos;


void main()
{
gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
v_TexCoord = a_TexCoord;                              
v_Normal = mat3(u_Model) * a_Normal;
v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
};
