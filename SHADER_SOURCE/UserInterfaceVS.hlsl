#include "UserInterface.hlsli"
//Mesh : RectMesh
VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;
    
    //float4 worldPosition = mul(In.Pos, world);
    //float4 viewPosition = mul(worldPosition, view);
    //float4 ProjPosition = mul(viewPosition, projection);
    //Out.Pos = ProjPosition;
    
	Out.Pos = In.Pos * 2.f;
	Out.Pos.w = 1.f;
    Out.Color = In.Color;
    Out.UV = In.UV;
    
    return Out;
}

