#version 330 core

in vec3 fcolor;

out vec4 FragColor;
uniform vec3 colGreen;
uniform bool invisible;
void main() {
        if(invisible) FragColor = vec4(colGreen, 1);
	else FragColor = vec4(fcolor*colGreen, 1);
}

