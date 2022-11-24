#version 330

// Model-Space coordinates
in vec3 position;
in vec3 normal;

struct LightSource {
    vec3 position;
    vec3 rgbIntensity;
};
uniform LightSource light;

uniform mat4 ModelView;
uniform mat4 Perspective;
uniform mat4 Model;
uniform mat4 lightProjection;

// Remember, this is transpose(inverse(ModelView)).  Normals should be
// transformed using this matrix instead of the ModelView matrix.
uniform mat3 NormalMatrix;

out VsOutFsIn {
	vec3 position_ES; // Eye-space position
	vec3 normal_ES;   // Eye-space normal
	LightSource light;
} vs_out;

out vec4 crntPos;
out vec2 TexCoords;
out vec4 fragPosLight;
out vec4 ShadowCoord;

layout (location=2) in vec2 aTexCoords;



void main() {
	vec4 pos4 = vec4(position, 1.0);

	//-- Convert position and normal to Eye-Space:
	vs_out.position_ES = (ModelView * pos4).xyz;
	vs_out.normal_ES = normalize(NormalMatrix * normal);

	vs_out.light = light;

	crntPos = Model * vec4(position, 1.0);
	fragPosLight = lightProjection * crntPos;

	TexCoords = mat2(0.0, -1.0, 1.0, 0.0)*aTexCoords;

	fragPosLight /= fragPosLight.w;

	mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,0.0, 0.5, 0.0, 0.0,0.0, 0.0, 0.5, 0.0,0.5, 0.5, 0.5, 1.0);

	ShadowCoord = biasMatrix *  fragPosLight;
	gl_Position = Perspective * ModelView * vec4(position, 1.0);
}
