struct Output {
    float4 Color : COLOR;
    float4 Position : SV_Position;
};

struct Params {
    matrix mvp;
};

ConstantBuffer<Params> params : register(b0);

Output main(float3 pos : POSITION, float3 color : COLOR) {
    Output output;

    output.Position = mul(float4(pos, 1.0f), params.mvp);
    output.Color = float4(color, 1.0f);

    return output;
}
