cbuffer CB0 : register(b0)
{
    float2 offset; // x,y移動
    float2 pad; // 16byteアライン用
};

struct VS_IN
{
    float3 pos : POSITION;
    float4 col : COLOR;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};

VS_OUT VSMain(VS_IN v)
{
    VS_OUT o;
    float3 p = v.pos;
    p.xy += offset; // CPUから渡したオフセットで移動
    o.pos = float4(p, 1.0f);
    o.col = v.col;
    return o;
}

float4 PSMain(VS_OUT p) : SV_TARGET
{
    return p.col;
}