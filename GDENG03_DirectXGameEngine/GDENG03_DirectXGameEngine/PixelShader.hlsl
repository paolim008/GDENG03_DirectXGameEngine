struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float3 color1 : COLOR1;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;

    float m_angle;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
    return float4(lerp(input.color, input.color1, (sin(m_angle) + 1.0f) / 2.0f), 1.0f);
}