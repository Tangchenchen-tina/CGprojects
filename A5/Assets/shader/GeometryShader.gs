#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec4 crntPos_v[];
in vec2 TexCoords_v[];
in vec4 fragPosLight_v[];
in vec4 ShadowCoord_v[];
in vec2 textCoords[];

struct LightSource {
    vec3 position;
    vec3 rgbIntensity;
};

in VsOutFsIn {
	vec3 position_ES; // Eye-space position
	vec3 normal_ES;   // Eye-space normal
	LightSource light;
} fs_in[];


out vec4 crntPos;
out vec2 TexCoords;
out vec4 fragPosLight;
out vec4 ShadowCoord;

out VsOutFsIn {
	vec3 position_ES; // Eye-space position
	vec3 normal_ES;   // Eye-space normal
	LightSource light;
} vs_out;


out vec2 TextCoords;



void main()
{
	if(textCoords[0].x == 0 && textCoords[1].x > 0.9f && textCoords[2].x > 0.9f){
		TextCoords.x = 1;
		TextCoords.y = textCoords[0].y;
    crntPos = crntPos_v[0];
    TexCoords = TexCoords_v[0];
    fragPosLight = fragPosLight_v[0];
    ShadowCoord = ShadowCoord_v[0];
    vs_out = fs_in[0];

		EmitVertex();
		
		TextCoords = textCoords[1];
        crntPos = crntPos_v[1];
    TexCoords = TexCoords_v[1];
    fragPosLight = fragPosLight_v[1];
    ShadowCoord = ShadowCoord_v[1];
    vs_out = fs_in[1];
		EmitVertex();
		
		TextCoords = textCoords[2];
        crntPos = crntPos_v[2];
    TexCoords = TexCoords_v[2];
    fragPosLight = fragPosLight_v[2];
    ShadowCoord = ShadowCoord_v[2];
    vs_out = fs_in[2];
		EmitVertex();		
		
		EndPrimitive();
	}
	else if(textCoords[1].x == 0 && textCoords[0].x > 0.9f && textCoords[2].x > 0.9f){
		TextCoords = textCoords[0];
        crntPos = crntPos_v[0];
    TexCoords = TexCoords_v[0];
    fragPosLight = fragPosLight_v[0];
    ShadowCoord = ShadowCoord_v[0];
    vs_out = fs_in[0];
		EmitVertex();
		
		TextCoords.x = 1;
		TextCoords.y = textCoords[1].y;
        crntPos = crntPos_v[1];
    TexCoords = TexCoords_v[1];
    fragPosLight = fragPosLight_v[1];
    ShadowCoord = ShadowCoord_v[1];
    vs_out = fs_in[1];
		EmitVertex();
		
		TextCoords = textCoords[2];
        crntPos = crntPos_v[2];
    TexCoords = TexCoords_v[2];
    fragPosLight = fragPosLight_v[2];
    ShadowCoord = ShadowCoord_v[2];
    vs_out = fs_in[2];
    EmitVertex();
		
		EndPrimitive();
	}
	else if(textCoords[2].x == 0 && textCoords[0].x > 0.9f && textCoords[1].x > 0.9f){
		TextCoords = textCoords[0];
        crntPos = crntPos_v[0];
    TexCoords = TexCoords_v[0];
    fragPosLight = fragPosLight_v[0];
    ShadowCoord = ShadowCoord_v[0];
    vs_out = fs_in[0];
		EmitVertex();
		
		TextCoords = textCoords[1];
        crntPos = crntPos_v[1];
    TexCoords = TexCoords_v[1];
    fragPosLight = fragPosLight_v[1];
    ShadowCoord = ShadowCoord_v[1];
    vs_out = fs_in[1];
		EmitVertex();
		
		TextCoords.x = 1;
		TextCoords.y = textCoords[2].y;
        crntPos = crntPos_v[2];
    TexCoords = TexCoords_v[2];
    fragPosLight = fragPosLight_v[2];
    ShadowCoord = ShadowCoord_v[2];
    vs_out = fs_in[2];
		EmitVertex();
		
		EndPrimitive();
	}
	else if(textCoords[0].x == 0 && textCoords[1].x == 0 && textCoords[2].x > 0.9f){
		TextCoords.x = 1;
		TextCoords.y = textCoords[0].y;
        crntPos = crntPos_v[0];
    TexCoords = TexCoords_v[0];
    fragPosLight = fragPosLight_v[0];
    ShadowCoord = ShadowCoord_v[0];
    vs_out = fs_in[0];
		EmitVertex();
		
		TextCoords.x = 1;
		TextCoords.y = textCoords[1].y;
        crntPos = crntPos_v[1];
    TexCoords = TexCoords_v[1];
    fragPosLight = fragPosLight_v[1];
    ShadowCoord = ShadowCoord_v[1];
    vs_out = fs_in[1];
		EmitVertex();
		

    crntPos = crntPos_v[2];
    TexCoords = TexCoords_v[2];
    fragPosLight = fragPosLight_v[2];
    ShadowCoord = ShadowCoord_v[2];
    vs_out = fs_in[2];
		EmitVertex();
		
		EndPrimitive();
	}
	else if(textCoords[0].x == 0 && textCoords[2].x == 0 && textCoords[1].x > 0.9f){
		TextCoords.x = 1;
		TextCoords.y = textCoords[0].y;
        crntPos = crntPos_v[0];
    TexCoords = TexCoords_v[0];
    fragPosLight = fragPosLight_v[0];
    ShadowCoord = ShadowCoord_v[0];
    vs_out = fs_in[0];
		EmitVertex();
		
		TextCoords = textCoords[1];
        crntPos = crntPos_v[1];
    TexCoords = TexCoords_v[1];
    fragPosLight = fragPosLight_v[1];
    ShadowCoord = ShadowCoord_v[1];
    vs_out = fs_in[1];
		EmitVertex();
		
		TextCoords.x = 1;
		TextCoords.y = textCoords[2].y;
        crntPos = crntPos_v[2];
    TexCoords = TexCoords_v[2];
    fragPosLight = fragPosLight_v[2];
    ShadowCoord = ShadowCoord_v[2];
    vs_out = fs_in[2];
		EmitVertex();
		
		EndPrimitive();
	}
	else if(textCoords[1].x == 0 && textCoords[2].x == 0 && textCoords[0].x > 0.9f){
		TextCoords = textCoords[0];
    crntPos = crntPos_v[0];
    TexCoords = TexCoords_v[0];
    fragPosLight = fragPosLight_v[0];
    ShadowCoord = ShadowCoord_v[0];
    vs_out = fs_in[0];
		EmitVertex();
		
		TextCoords.x = 1;
		TextCoords.y = textCoords[1].y;
        crntPos = crntPos_v[1];
    TexCoords = TexCoords_v[1];
    fragPosLight = fragPosLight_v[1];
    ShadowCoord = ShadowCoord_v[1];
    vs_out = fs_in[1];
		EmitVertex();
		
		TextCoords.x = 1;
		TextCoords.y = textCoords[2].y;
        crntPos = crntPos_v[2];
    TexCoords = TexCoords_v[2];
    fragPosLight = fragPosLight_v[2];
    ShadowCoord = ShadowCoord_v[2];
    vs_out = fs_in[2];
		EmitVertex();
		
		EndPrimitive();
	}
	else{
		int i;
		for(i = 0; i < gl_in.length(); i++)
		{			
			TextCoords = textCoords[i];
          crntPos = crntPos_v[i];
    TexCoords = TexCoords_v[i];
    fragPosLight = fragPosLight_v[i];
    ShadowCoord = ShadowCoord_v[i];
    vs_out = fs_in[i];
			EmitVertex();
		}
		EndPrimitive();	
	}
}  