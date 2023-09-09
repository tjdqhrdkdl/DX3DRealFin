#include "UserInterface.hlsli"


float4 main(VSOut In) : SV_Target
{
    float4 color = (float) 0.0f;
    
	
	for (int i = 0; i < UISBufferSize; ++i)
	{
		float2 UVEnd = UISBuffer[i].UVBeginInCanvas + UISBuffer[i].UVSizeInCanvas;
		
		//현재 UV값이 그리려는 UI 안에 있는 UV인지 확인
		if (UISBuffer[i].UVBeginInCanvas.x <= In.UV.x 
            && UISBuffer[i].UVBeginInCanvas.y <= In.UV.y
			&& UVEnd.x >= In.UV.x
			&& UVEnd.y >= In.UV.y
			)
		{
			//UI가 그려질 영역의 UV값을 계산
			float2 SampleUV = 
			UISBuffer[i].UVBeginInTexture
			+ UISBuffer[i].UVSizeInTexture * ((In.UV - UISBuffer[i].UVBeginInCanvas) / UISBuffer[i].UVSizeInCanvas);
			
			color = albedoTexture.Sample(anisotropicSampler, SampleUV);
		}
	}

    
    if(0.f == color.a)
		discard;
    
    return color;
}