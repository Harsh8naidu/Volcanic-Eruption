#version 330

layout(location = 0) in vec3 position;

uniform mat4 gVP;

void main()
{
	gl_Position = gVP * vec4(position, 1.0);
}
