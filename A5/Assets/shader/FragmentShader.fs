#version 330

uniform bool picking;
uniform int shadowflag;
uniform int textureflag;
uniform bool toonrender;

struct LightSource {
    vec3 position;
    vec3 rgbIntensity;
};

in VsOutFsIn {
	vec3 position_ES; // Eye-space position
	vec3 normal_ES;   // Eye-space normal
	LightSource light;
} vs_out;

in vec4 crntPos;
in vec4 fragPosLight;
in vec4 ShadowCoord;
in vec2 textCoords;

out vec4 fragColour;

struct Material {
    vec3 kd;
    vec3 ks;
    float shininess;
};
uniform Material material;

// Ambient light intensity for each RGB component.
uniform int textureID;
uniform vec3 ambientIntensity;
uniform samplerCube depthMap;
uniform sampler2D shadowMap;
uniform sampler2D textureMap;
uniform sampler2D charactertextureMap;


vec3 phongModel(vec3 fragPosition, vec3 fragNormal) {
	LightSource light = vs_out.light;

    // Direction from fragment to light source.
    vec3 l = normalize(light.position - fragPosition);

    // Direction from fragment to viewer (origin - fragPosition).
    vec3 v = normalize(-fragPosition.xyz);

    float n_dot_l = max(dot(fragNormal, l), 0.0);

    vec3 color;
    vec3 result;

   float visibility = 1.0;
   if(shadowflag == 1){
      float bias = 0.01;
      //vec3 ShadowC = ShadowCoord.xyz/ShadowCoord.w;
      vec4 ShadowC = ShadowCoord;
      if(texture(shadowMap, ShadowC.xy).r < ShadowC.z-bias){
         visibility = 0;
      }
   }
   

    //   if (n_dot_l > 0.95)
    //    color = vec3(1.0, 1.0, 1.0); 
    //   else if (n_dot_l > 0.5)
    //    color = vec3(0.7, 0.7 ,0.7);
    //   else if (n_dot_l > 0.25)
    //    color = vec3(0.5, 0.5, 0.5);
    //   else 
    //    color = vec3(0.3, 0.3, 0.3);
if(toonrender){
   if (n_dot_l > 0.9)
       color = vec3(1, 1 ,1);
   else if (n_dot_l > 0.8)
       color = vec3(0.8, 0.8, 0.8);
   else if (n_dot_l > 0.7)
       color = vec3(0.7, 0.7, 0.7);
   else if (n_dot_l > 0.6)
       color = vec3(0.6, 0.6, 0.6);
   else if (n_dot_l > 0.5)
       color = vec3(0.5, 0.5, 0.5);
   else if (n_dot_l > 0.4)
       color = vec3(0.4, 0.4, 0.4);
   else if (n_dot_l > 0.3)
       color = vec3(0.3, 0.3, 0.3);
   else if (n_dot_l > 0.2)
       color = vec3(0.1, 0.1, 0.1);
   else 
       color = vec3(0.05, 0.05, 0.05);

   if(textureflag == 1){
      if(textureID == 0){
         color = texture(textureMap, textCoords).rgb;
      }
      else if(textureID == 1){
         color = texture(charactertextureMap, textCoords).rgb;
      }
   }
   if(textureID != -1){
      result = color;
   }else{
      result = ambientIntensity+ (visibility) * material.kd* color *light.rgbIntensity;
   }

}else{
	vec3 diffuse;
	diffuse = material.kd * n_dot_l;

    vec3 specular = vec3(0.0);

    if (n_dot_l > 0.0) {
		// Halfway vector.
		vec3 h = normalize(v + l);
        float n_dot_h = max(dot(fragNormal, h), 0.0);

        specular = material.ks * pow(n_dot_h, material.shininess);
    }

    result = ambientIntensity + visibility * light.rgbIntensity * (diffuse + specular);
   if(textureflag == 1 && textureID != -1){
      if(textureID == 0){
         result = texture(textureMap, textCoords).rgb;
      }
      else if(textureID == 1){
         result = texture(charactertextureMap, textCoords).rgb;
      }
   }

   }
    return result;


	// vec3 diffuse;
	// diffuse = material.kd * n_dot_l;

   //  vec3 specular = vec3(0.0);

   //  if (n_dot_l > 0.0) {
	// 	// Halfway vector.
	// 	vec3 h = normalize(v + l);
   //      float n_dot_h = max(dot(fragNormal, h), 0.0);

   //      specular = material.ks * pow(n_dot_h, material.shininess);
   //  }

   //  return ambientIntensity + visibility * light.rgbIntensity * (diffuse + specular);
}

void main() {
	if( picking ) {
		fragColour = vec4(material.kd, 1.0);
	} else {
		fragColour = vec4(phongModel(vs_out.position_ES, vs_out.normal_ES), 1.0);
	}
}
