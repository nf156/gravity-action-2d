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
    o.pos = float4(v.pos, 1.0f);
    o.col = v.col;
    return o;
}

float4 PSMain(VS_OUT p) : SV_TARGET
{
    return p.col;
}