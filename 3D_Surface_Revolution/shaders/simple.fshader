#version 330
in vec3 vColor;
out vec4 fragColor;
void main(void)
{
	fragColor = vec4(vColor.x, vColor.y, vColor.z, 1);
}