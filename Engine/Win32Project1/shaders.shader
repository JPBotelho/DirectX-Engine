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
	float3 lightPos = float3(1, 10, 3);
	Texture2D test = shaderTexture;
	return dot(normalize(input.normal), normalize(input.position.xyz - lightPos));//shaderTexture.Sample(MeshTextureSampler, input.color.xy*2).xyz;//diffuseTexture.Sample(float2(.5, .5));
}
