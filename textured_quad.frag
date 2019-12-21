#version 450 core

out vec4 fragColor;

in vec4 fragPosLightSpace;

in vec3 lightPos;

in vec3 VSPosition;
in vec3 VSNormal;

in vec3 WSPosition;
in vec3 WSNormal;
in vec3 EyePosition;

in V2F
{
	vec3 position;
	vec3 normal;
	vec2 uv;
} fs_in;

layout(std140, binding = 0) uniform Matrices
{
	mat4 P_Matrix; // Projection Matrix
	mat4 V_Matrix; // View Matrix
	mat4 I_V_Matrix; // Inverse View Matrix
	mat4 VP_Matrix; // View Projection Matrix
	mat4 M_Matrix; // Model Matrix
	mat4 I_M_Matrix; // Inverse Model Matrix
	mat4 MVP_Matrix; // Model View Projection Matrix
	mat4 T_MV_Matrix; // Transpose of the Model View
	mat4 N_Matrix;  // Inverse Transpose of the Model View
	mat4 P_MVP_Matrix;  // Previous MVP
	vec3 Camera_Position; //camera position in world space
};

uniform sampler2D Elevation;
uniform sampler2D Atmosphere;
uniform sampler2D Heatmap;
uniform sampler2D Biome;

uniform int display_elevation;
uniform int display_atmosphere;
uniform int display_heatmap;
uniform int display_biome;

const float opt255 = 1.0/255.0;

vec4 identify_heatmap(float temperature) {
	//float v = temperature *0.5 + 0.5;
	float v = temperature/36.f;
	if(v > 0.)
		return vec4(v, 1. - v, 0., 1.);
	else {
		 v = -v;
		 return vec4(v, 0., 1. - v, 1.);
	}
}

vec4 identify_biome(float biome) {
	vec4 color = vec4(0);

	const float opt = 1./255;

	if(biome >= 1*opt && biome <= 6*opt)
		color = vec4(0., 0., 1., 1.);
	else if(biome >= 7*opt && biome <= 10*opt)
		color = vec4(1., 1., 0., 1.);
	else if(biome >= 11*opt && biome <= 13*opt)
		color = vec4(0., 1., 0., 1.);
	else if(biome >= 14*opt && biome <= 16*opt)
		color = vec4(0.5, 1., 0., 1.);
	else if(biome >= 17*opt && biome <= 18*opt)
		color = vec4(0.5, 1., 1., 1.);
	else
		color = vec4(0.5, 0., 1., 1.);

	/*
	//Biome_Unknown = -1,
	if(biome == -1)
		color = vec4(0.75, 0.1, 1., 1);

	//Aquatic_Coral,
	if(biome == 0)
		color = vec4(0.1, 0.1, 1., 1.);
	
	//Aquatic_Estuaries, 
	if(biome == 1)
		color = vec4(0.1, 0.25, 1., 1.);

	//Aquatic_Ocean,
	if(biome == 2)
		color = vec4(0., 0., 1., 1.);

	//Aquatic_Lakes,
	if(biome == 3)
		color = vec4(0.2, 0.3, 1., 1.);

	//Aquatic_Ponds, 
	if(biome == 4)
		color = vec4(0.2, 0.3, 1., 1.);

	//Aquatic_Wetlands,
	if(biome == 5)
		color = vec4(0., 0.5, 0.9, 1.);

	//Deserts_Hot_and_Dry,
	if(biome == 6)
		color = vec4(0.2, 0.3, 1., 1.);

	//Deserts_Semiarid,
	if(biome == 7)
		color = vec4(0.2, 0.3, 1., 1.);

	//Deserts_Coastal,
	if(biome == 8)
		color = vec4(0.2, 0.3, 1., 1.);

	//Deserts_Cold,
	if(biome == 9)
		color = vec4(0.2, 0.3, 1., 1.);

	//Forests_Tropical,
	if(biome == 10)
		color = vec4(0.1, 1., 0., 1.);

	//Forests_Temperate,
	if(biome == 11)
		color = vec4(0., 1., 0.25, 1.);

	//Forests_Boreal,
	if(biome == 12)
		color = vec4(0.3, 1., 0.3, 1.);

	//Grasslands_Savanna, 
	if(biome == 13)
		color = vec4(0.2, 0.3, 1., 1.);

	//Grasslands_Temperate,
	if(biome == 14)
		color = vec4(0.2, 0.3, 1., 1.);

	//Grasslands_Steppes,
	if(biome == 15)
		color = vec4(0.2, 0.3, 1., 1.);

	//Tundra_Artic,
	if(biome == 16)
		color = vec4(0.2, 0.3, 1., 1.);

	//Tundra_Alpine,
	if(biome == 17)
		color = vec4(0.2, 0.3, 1., 1.);
	*/

	return color;
}

void main ()
{
	// Elevation
	vec4 elevation = texture2D (Elevation, fs_in.uv).rrra;
	vec4 atmosphere = texture2D (Atmosphere, fs_in.uv).rrra;
	vec4 heatmap = texture2D (Heatmap, fs_in.uv).rrra;
	float biome = texture2D(Biome, fs_in.uv).r;

	vec4 color = vec4(0);

	if(display_elevation > 0) {
		color = elevation / 255.f;

		if (display_heatmap > 0) {
			color = identify_heatmap(heatmap.r);
		} else if(display_biome > 0) {
			color = identify_biome(biome);
		}
	}
	else
		color = vec4(1);
		
	fragColor = color;
}