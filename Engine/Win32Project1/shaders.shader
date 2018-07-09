cbuffer CameraBuffer : register( cb0 ) {
    float dTime;
	float time;
	float2 pad;
	float4x4 projectionMatrix;
};

Texture2D shaderTexture : register(t0);
SamplerState MeshTextureSampler : register(s0);

struct VOut
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

VOut VShader(float4 position : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD0)
{
    VOut output;
	
    output.position = mul(float4(position.xyz, 1), transpose(projectionMatrix));
	//output.position = position;
    output.normal = normal;
    //Position is in clip space.
	output.uv = uv;//(position.xy + 1) / 2;

    return output;
}


float3 PShader(VOut input) : SV_TARGET
{
	float3 lightPos = float3(-100, 10000, -100);
	float3 lightdir = normalize(input.position.xyz - lightPos);
	return max(0, dot((input.normal), lightdir)) * float3(1, 0, 0);
}
