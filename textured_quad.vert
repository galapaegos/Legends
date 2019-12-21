#version 450 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_uv;

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

out V2F
{
	vec3 position;
	vec3 normal;
    vec2 uv;
} vs_out;


out vec4 fragPosLightSpace;


out vec3 lightPos;


out vec3 VSPosition;
out vec3 VSNormal;
out vec3 vLightPosition;

out vec3 WSPosition;
out vec3 WSNormal;
out vec3 EyePosition;

void main() 
{
	vec3 LightPosition = vec3(0.01, 0.01, -2.0);

	vec4 a_normal = vec4(vertex_normal, 0.0) * 2.0 - 1.0;

	vec4 wsPosition = M_Matrix * vec4(vertex_position, 1.0);
	vec4 vsPosition = V_Matrix * wsPosition;
	vec4 vsNormal = N_Matrix * vec4(vertex_normal, 0.0);
	VSPosition = vsPosition.xyz;
	
	WSPosition = wsPosition.xyz;
	WSNormal = normalize(vec3(M_Matrix* vec4(vertex_normal,0)));//normalize(camera.mInvView * vsNormal).xyz);
	VSNormal = normalize(vec3(V_Matrix * vec4(WSNormal,0)));
	EyePosition	= normalize(vec3(Camera_Position) - vec3(wsPosition));

	lightPos = LightPosition;

	mat3 normalMatrix = transpose(inverse(mat3(M_Matrix)));
    vec3 N = normalize(normalMatrix * vertex_normal);

	vs_out.position = vertex_position;
	vs_out.normal = N;
	vs_out.uv = vertex_uv;
	
	fragPosLightSpace = M_Matrix * vec4(vertex_position, 1.0);
	

	vLightPosition = (V_Matrix * M_Matrix * vec4(LightPosition, 1.0)).xyz;

    //project the vertex, the rest is handled by WebGL
    gl_Position = MVP_Matrix * vec4(vertex_position, 1);
}
