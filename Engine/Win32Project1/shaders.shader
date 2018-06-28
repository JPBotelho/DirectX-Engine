cbuffer CameraBuffer : register( b0 ) {
    float dTime;
	float time;
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
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
	position.w = 1;
	//output.position = mul(position, worldMatrix);
    output.position = mul(position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
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
