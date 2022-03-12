#version 330
in vec2 aVertex;
in vec3 aColor;
out vec3 vColor;
void main()
{
	gl_Position = vec4(aVertex.x, aVertex.y, 0, 1);
	vColor = aColor;
}