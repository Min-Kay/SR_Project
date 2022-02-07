#pragma once

static D3DXVECTOR3		GetMouse(void)
{
	POINT	Pt{};

	GetCursorPos(&Pt);
	ScreenToClient(g_hWnd, &Pt);

	return D3DXVECTOR3(float(Pt.x), float(Pt.y), 0.f);
}