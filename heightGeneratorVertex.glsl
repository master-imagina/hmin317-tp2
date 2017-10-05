#version 330 core
in vec3 a_position;
in vec2 a_texcoord;

out vec2 textureCoord;

void main()
{
    textureCoord = a_texcoord;
    gl_Position = vec4(a_position, 1.0);
}
