#version 130

in vec4 vPosition;
in vec4 vColor;

out vec4 pass_color;
uniform mat4 rot;

void main()
{
    pass_color = vColor;
    gl_Position = rot * vPosition;
}
