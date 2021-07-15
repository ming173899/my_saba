#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 2) uniform PSMaterialUBO
{
	vec4 EdgeColor;
} matUBO;

layout (location = 0) out vec4 outFragColor;

void main()
{
	outFragColor = matUBO.EdgeColor;
}
