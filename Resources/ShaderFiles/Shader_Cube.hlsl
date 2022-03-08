
float4x4	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture		g_Texture;
float		g_ColorStack;
float4      g_Color;
uint		g_Alpha; // 0 == ���� , 1 == ���� �׽�Ʈ , 2 == ���� ����
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
	/*���������� �޾ƿͼ� ���� ���� ����, ��, ��������� ���Ѵ�. */
	VS_OUT			Out = (VS_OUT)0;

	vector			vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);

	Out.vPosition = mul(vPosition, g_ProjMatrix);
	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix).xyz;

	/* ����. */
	return Out;
}

/* W������ X, Y, Z, W�� ��� ������ .*/
/* ����Ʈ �����̽��� ������ ��ġ�� ��ȭ�Ѵ�. */

/* ���������� ���̸�(TRIANGLELIST) �����Ͷ���� �����Ѵ�. */

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


//� ��ũ��Ŀ�� �׸���. 
//
//� ���̴��� �׸���. 
//
//� �н��� �׸���.(1)
//
//���۸� �������Ѵ�. 
//
//�н��� ������. 
// 
//���̴��׸��⸦ ������. 


technique Default_Technique
{
	/* ��� + �׸��� + ���̶���Ʈ + ������Ʈ */
	/* �� ��ü�� �׸����۾��� ������. */

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