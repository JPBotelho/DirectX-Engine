cbuffer CameraBuffer : register( cb0 ) {
    float dTime;
	float time;
	float2 pad;
	float4x4 projectionMatrix;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;
	
    output.position = mul(float4(position.xyz, 1), transpose(projectionMatrix));
	//output.position = position;
    output.color = color;
    //Position is in clip space.
    output.uv = (position.xy + 1) / 2;

    return output;
}


float3 PShader(VOut input) : SV_TARGET
{
    return 1;
}
