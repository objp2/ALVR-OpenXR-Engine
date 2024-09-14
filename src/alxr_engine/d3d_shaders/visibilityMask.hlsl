struct PSVertex {
    float4 Pos : SV_POSITION;
};

struct Vertex {
    float2 Pos : POSITION;
};

cbuffer ViewProjectionConstantBuffer : register(b0) {
    float4x4 Projection;
};

PSVertex MainVS(Vertex input) {
    PSVertex output;
    output.Pos = mul(Projection, float4(input.Pos, -1, 1));
    return output;
}

float4 MainPS() : SV_TARGET{
    return float4(0, 0, 0, 0);
}
