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
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

VOut VShader(float3 position : POSITION0, float3 normal : NORMAL0, float2 uv : TEXCOORD0)
{
    VOut output;
	
    output.position.xyz = mul(float4(position.xyz, 1), transpose(projectionMatrix)).xyz;
	//output.position = position;
    output.normal = normal;
    //Position is in clip space.
    output.uv = (position.xy + 1) / 2;

    return output;
}


float3 PShader(VOut input) : SV_TARGET
{
	Texture2D test = shaderTexture;
	return 1;//shaderTexture.Sample(MeshTextureSampler, input.color.xy*2).xyz;//diffuseTexture.Sample(float2(.5, .5));
}
