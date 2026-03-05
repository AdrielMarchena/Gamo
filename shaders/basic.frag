#version 460 core

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

in vec3 v_FragPos;
in vec3 v_Normal;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}