#version 330
uniform mat4 transformations;
in vec3 aVertex;
in vec3 aColor;
out vec3 vColor;
void main()
{
	gl_Position = transformations * vec4(aVertex.x, aVertex.y, aVertex.z, 1);
	vColor = aColor;
}
