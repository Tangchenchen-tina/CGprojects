#version 330

uniform bool picking;
uniform int shadow;

struct LightSource {
    vec3 position;
    vec3 rgbIntensity;
};

in VsOutFsIn {
	vec3 position_ES; // Eye-space position
	vec3 normal_ES;   // Eye-space normal
	LightSource light;
} fs_in;

out vec4 fragColour;
in vec4 crntPos;
in vec4 fragPosLight;
in vec4 ShadowCoord;

struct Material {
    vec3 kd;
    vec3 ks;
    float shininess;
};
uniform Material material;

// Ambient light intensity for each RGB component.
uniform vec3 ambientIntensity;
uniform samplerCube depthMap;
uniform sampler2D shadowMap;

// float shadowCalc(vec3 fragPos, LightSource light){
//    vec3 lightToFrag = fragPos - light.position;
//    float depth = texture(depthMap, lightToFrag).r;
//    depth *= far_plane;
//    float bias = 0.05;
//    return (depth+bias) < length(lightToFrag)? 0.0f: 1.0f;
// }


// float shadowCalc(){
//    vec3 pos = fragPosLight.xyz * 0.5+0.5;
//    float depth = texture(shadowMap, pos.xy).r;
//    return depth < pos.z ? 0.0:1.0;
// }


vec3 phongModel(vec3 fragPosition, vec3 fragNormal) {
	LightSource light = fs_in.light;

    // Direction from fragment to light source.
    vec3 l = normalize(light.position - fragPosition);

    // Direction from fragment to viewer (origin - fragPosition).
    vec3 v = normalize(-fragPosition.xyz);

    float n_dot_l = max(dot(fragNormal, l), 0.0);

    vec3 color;
    //   if (n_dot_l > 0.95)
    //    color = vec3(1.0, 1.0, 1.0); 
    //   else if (n_dot_l > 0.5)
    //    color = vec3(0.7, 0.7 ,0.7);
    //   else if (n_dot_l > 0.25)
    //    color = vec3(0.5, 0.5, 0.5);
    //   else 
    //    color = vec3(0.3, 0.3, 0.3);

    if (n_dot_l > 0.95)
       color = vec3(1.0, 1.0, 1.0); 
      else if (n_dot_l > 0.9)
       color = vec3(0.9, 0.9 ,0.9);
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
       color = vec3(0.2, 0.2, 0.2);
      else 
       color = vec3(0.1, 0.1, 0.1);

   // float shadow = shadowCalc();
   // shadow = 1.0;

   // ShadowCoord.z: 0.3-0.4
   // texture(shadowMap, ShadowCoord.xy).z < 0.1
   float visibility = 1.0;
   if(shadow == 1){
      float bias = 0.01;
      //vec3 ShadowC = ShadowCoord.xyz/ShadowCoord.w;
      vec4 ShadowC = ShadowCoord;
      if(texture(shadowMap, ShadowC.xy).r < ShadowC.z-bias){
         visibility = 0;
      }
   }


    vec3 result = ambientIntensity+ (visibility) * material.kd* color *light.rgbIntensity;
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
		fragColour = vec4(phongModel(fs_in.position_ES, fs_in.normal_ES), 1.0);
	}
}
