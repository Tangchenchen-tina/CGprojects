#version 330

// Model-Space coordinates
in vec3 position;
in vec3 normal;

uniform mat4 ModelView;
uniform mat4 Perspective;
uniform float outlining;



void main() {
	gl_Position = Perspective * ModelView * vec4(position+normal*outlining, 1.0);
}