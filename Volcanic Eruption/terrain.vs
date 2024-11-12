#version 330

layout(location = 0) in vec3 position;

uniform mat4 gVP;

out vec4 color;

void main()
{
	gl_Position = gVP * vec4(position, 1.0);

	color = vec4(position.y / 200.0);
}
