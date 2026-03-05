#version 460 core

layout(location = 0) in vec3 aPosition;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 vFragPos;

void main()
{
    vFragPos = vec3(u_Model * vec4(aPosition, 1.0));
    
    gl_Position = u_Projection * u_View * u_Model * vec4(aPosition, 1.0);
}
