
float4x4	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture		g_Texture;
float		g_ColorStack;
float4      g_Color;
uint		g_Alpha; // 0 == 안함 , 1 == 알파 테스트 , 2 == 알파 블렌드
float		g_AlphaTestRef;


sampler DefaultSampler = sampler_state
{
	texture = g_Texture;

	MagFilter = linear;
	MipFilter = linear;
	MinFilter = linear;

	AddressU = wrap;
	AddressV = wrap;
};



struct VS_IN
{
	float3 vPosition : POSITION;
	float3 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : POSITION;
	float3 vTexUV : TEXCOORD0;
	float3 vWorldPos : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
	/*로컬정점을 받아와서 내가 직접 월드, 뷰, 투영행렬을 곱한다. */
	VS_OUT			Out = (VS_OUT)0;

	vector			vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);

	Out.vPosition = mul(vPosition, g_ProjMatrix);
	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix).xyz;

	/* 리턴. */
	return Out;
}

/* W값으로 X, Y, Z, W를 모두 나눈다 .*/
/* 뷰포트 스페이스로 정점의 위치를 변화한다. */

/* 정점세개가 모이면(TRIANGLELIST) 래스터라이즈를 수행한다. */

struct PS_IN
{
	float4 vPosition : POSITION;
	float3 vTexUV : TEXCOORD0;
	float3 vWorldPos : TEXCOORD1;
};


struct PS_OUT
{
	vector	vColor : COLOR;
};


PS_OUT PS_MAIN_CUBE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = texCUBE(DefaultSampler, In.vTexUV);

	return Out;
}

PS_OUT PS_SETCOLOR_CUBE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = texCUBE(DefaultSampler, In.vTexUV);

	Out.vColor = Out.vColor + g_Color + float4(float3(1.f, 1.f, 1.f) * g_ColorStack, 0.f);

	saturate(Out.vColor);

	if (g_Alpha == 1)
	{
		if (Out.vColor.a < 0.3f)
			discard;
	}

	return Out;
}

PS_OUT PS_GROWCOLOR_CUBE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = texCUBE(DefaultSampler, In.vTexUV);

	Out.vColor.rgb += g_ColorStack;

	saturate(Out.vColor);

	if (g_Alpha == 1)
	{
		if (Out.vColor.a < 0.3f)
			discard;
	}

	return Out;
}

PS_OUT PS_GRAYCOLOR_CUBE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = texCUBE(DefaultSampler, In.vTexUV);

	Out.vColor.gb = Out.vColor.r;

	if (g_Alpha == 1)
	{
		if (Out.vColor.a < g_AlphaTestRef)
			discard;
	}


	return Out;
}


//어떤 테크니커로 그린다. 
//
//어떤 쉐이더로 그린다. 
//
//어떤 패스로 그린다.(1)
//
//버퍼를 렌더링한다. 
//
//패스를 끝낸다. 
// 
//쉐이더그리기를 끝낸다. 


technique Default_Technique
{
	/* 명암 + 그림자 + 하이라이트 + 림라이트 */
	/* 그 객체의 그리기작업이 끝난다. */

	pass Default_Cube
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_CUBE();
	}

	pass SetColor_Cube
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_SETCOLOR_CUBE();
	}

	pass GrowColor_Cube
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_GROWCOLOR_CUBE();
	}

	pass GrayColor_Cube
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_GRAYCOLOR_CUBE();
	}

	pass Default_Blend_Cube
	{
		AlphaBlendEnable = true;
		BlendOp = add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_CUBE();
	}

	pass SetColor_Blend_Cube
	{
		AlphaBlendEnable = true;
		BlendOp = add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_SETCOLOR_CUBE();
	}

	pass GrowColor_Blend_Cube
	{
		AlphaBlendEnable = true;
		BlendOp = add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_GROWCOLOR_CUBE();
	}

	pass GrayColor_Blend_Cube
	{
		AlphaBlendEnable = true;
		BlendOp = add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_GRAYCOLOR_CUBE();
	}

}