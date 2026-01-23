#include "../tomb4/pch.h"
#include "LoadSave.h"
#include "function_table.h"
#include "3dmath.h"
#include "function_stubs.h"
#include "gamemain.h"
#include "output.h"
#include "../game/camera.h"
#include "../game/lara.h"
#include "../game/gameflow.h"
#include "winmain.h"
#include "drawroom.h"
#include "polyinsert.h"
#include "../tomb4/tomb4.h"
#include "texture.h"

static float loadbar_pos;
static long loadbar_maxpos;

static GouraudBarColourSet healthBarColourSet =
{
	{ 64, 96, 128, 96, 64 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 128, 192, 255, 192, 128 },
	{ 0, 0, 0, 0, 0 }
};

static GouraudBarColourSet poisonBarColourSet =
{
	{ 64, 96, 128, 96, 64 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 64, 96, 128, 96, 64 },
	{ 0, 0, 0, 0, 0 },
	{ 128, 192, 255, 192, 128 }
};

static GouraudBarColourSet airBarColourSet =
{
	{ 0, 0, 0, 0, 0 },
	{ 113, 146, 113, 93, 74 },
	{ 123, 154, 123, 107, 91 },
	{ 0, 0, 0, 0, 0 },
	{ 113, 146, 113, 93, 74 },
	{ 0, 0, 0, 0, 0 }
};

static GouraudBarColourSet dashBarColourSet =
{
	{ 144, 192, 240, 192, 144 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 144, 192, 240, 192, 144 },
	{ 144, 192, 240, 192, 144 },
	{ 0, 0, 0, 0, 0 }
};

static GouraudBarColourSet loadBarColourSet =
{
	{ 48, 96, 127, 80, 32 },
	{ 0, 0, 0, 0, 0 },
	{ 48, 96, 127, 80, 32 },
	{ 0, 0, 0, 0, 0 },
	{ 48, 96, 127, 80, 32 },
	{ 48, 96, 127, 80, 32 }
};

static GouraudBarColourSet enemyBarColourSet =
{
	{ 128, 192, 255, 192, 128 },
	{ 64, 96, 128, 96, 64 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 123, 154, 123, 107, 91 },
	{ 0, 0, 0, 0, 0 }
};

static void DrawColoredRect(float x0, float y0, float x1, float y1, float z, ulong c0, ulong c1, ulong c2, ulong c3, TEXTURESTRUCT* tex)
{
	D3DTLVERTEX* v;

	v = MyVertexBuffer;

	v[0].sx = x0;
	v[0].sy = y0;
	v[0].color = RGBA_SETALPHA(c0, 0xFF);

	v[1].sx = x1;
	v[1].sy = y0;
	v[1].color = RGBA_SETALPHA(c1, 0xFF);

	v[2].sx = x1;
	v[2].sy = y1;
	v[2].color = RGBA_SETALPHA(c2, 0xFF);

	v[3].sx = x0;
	v[3].sy = y1;
	v[3].color = RGBA_SETALPHA(c3, 0xFF);

	for (int i = 0; i < 4; i++)
	{
		v[i].sz = z;
		v[i].rhw = f_mpersp / z * f_moneopersp;
		v[i].specular = 0xFF000000;
	}

	AddQuadSorted(v, 0, 1, 2, 3, tex, 0);
}



static void DoBar(long x, long y, long width, long height, long pos, long c1, long c2, bool scaled)
{
	TEXTURESTRUCT tex;
	long p, xw, y2, bar;

	nPolyType = 6;
	clipflags[0] = 0;
	clipflags[1] = 0;
	clipflags[2] = 0;
	clipflags[3] = 0;
	tex.drawtype = 0;
	tex.tpage = 0;

	if (scaled)
		p = GetRenderScale(1);
	else
		p = GetFixedScale(1);

	xw = x + width;
	y2 = y + height;
	bar = width * pos / 100;

	DrawColoredRect((float)x, (float)y, float(x + bar), float(y2), f_mznear, c1, c1, c2, c2, &tex);
	DrawColoredRect((float)x, (float)y2, float(x + bar), float(y2 + height), f_mznear, c2, c2, c1, c1, &tex);

	DrawColoredRect((float)x, (float)y, (float)xw, float(y2 + height), f_mznear + 1, 0, 0, 0, 0, &tex);
	DrawColoredRect(float(x - p), float(y - p), float(xw + p), float(y2 + height + p), f_mznear + 2, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, &tex);
}

static void S_DrawHealthBar2(long pos)
{
	long x, y, w, h;

	w = GetFixedScale(150);
	h = GetFixedScale(6);
	x = phd_centerx - GetFixedScale(75);
	y = GetFixedScale(100);

		DoBar(x, y, w, h, pos, 0xFF000000, lara.poisoned ? 0xFFFFFF00 : 0xFFFF0000, 0);
}

static void S_DrawEnemyBar2(long pos)
{
	long x, y, w, h;

	w = GetFixedScale(150);
	h = GetFixedScale(6);
	x = phd_centerx - GetFixedScale(75);
	y = GetFixedScale(117);

		DoBar(x, y, w, h, pos, 0xFF000000, 0xFFFFA000, 0);
}

void S_DrawHealthBar(long pos)
{
	long x, y, w, h;

	if (!gfCurrentLevel)
		return;

	if (BinocularRange)
	{
		S_DrawHealthBar2(pos);
		return;
	}

	w = GetRenderScale(150);
	h = GetRenderScale(6);

	if (tomb4.bars_pos == 1 || tomb4.bars_pos == 2)//original or improved
	{
		x = GetRenderScale(8);
		y = GetRenderScale(8);
	}
	else
	{
		x = GetRenderScale(36);
		x = phd_winwidth - w - x;
		y = GetRenderScale(18);
	}

		DoBar(x, y, w, h, pos, 0xFF000000, lara.poisoned ? 0xFFFFFF00 : 0xFFFF0000, 1);
}

void S_DrawAirBar(long pos)
{
	long x, y, w, h;

	if (!gfCurrentLevel)
		return;

	w = GetRenderScale(150);
	h = GetRenderScale(6);

	if (tomb4.bars_pos == 1)//original
	{
		x = phd_winwidth - w - GetRenderScale(8);
		y = GetRenderScale(25);
	}
	else if (tomb4.bars_pos == 2)//improved
	{
		x = phd_winwidth - w - GetRenderScale(8);
		y = GetRenderScale(8);
	}
	else
	{
		x = GetRenderScale(36);
		x = phd_winwidth - w - x;
		y = GetRenderScale(43);
	}

		DoBar(x, y, w, h, pos, 0xFF000000, 0xFF0000FF, 1);
}

void S_DrawDashBar(long pos)
{
	long x, y, w, h;

	if (!gfCurrentLevel)
		return;

	w = GetRenderScale(150);
	h = GetRenderScale(6);

	if (tomb4.bars_pos == 1)//original
	{
		x = phd_winwidth - w - GetRenderScale(8);
		y = GetRenderScale(8);
	}
	else if (tomb4.bars_pos == 2)//improved
	{
		x = phd_winwidth - w - GetRenderScale(8);
		y = GetRenderScale(25);
	}
	else
	{
		x = GetRenderScale(36);
		x = phd_winwidth - w - x;
		y = GetRenderScale(68);
	}

		DoBar(x, y, w, h, pos, 0xFF000000, 0xFF00FF00, 1);
}

void S_DrawEnemyBar(long pos)
{
	long x, y, w, h;

	if (BinocularRange)
	{
		S_DrawEnemyBar2(pos);
		return;
	}

	w = GetRenderScale(150);
	h = GetRenderScale(6);

	if (tomb4.bars_pos == 1 || tomb4.bars_pos == 2)//original or improved
	{
		x = GetRenderScale(8);
		y = GetRenderScale(25);
	}
	else
	{
		x = GetRenderScale(36);
		x = phd_winwidth - w - x;
		y = GetRenderScale(93);
	}

		DoBar(x, y, w, h, pos, 0xFF000000, 0xFFFFA000, 1);
}

void DoSlider(long x, long y, long width, long height, long pos, long c1, long c2, long c3)
{
	TEXTURESTRUCT tex;
	float sx, sy, w, h;
	static float V;

	nPolyType = 4;
	V += 0.01F;

	if (V > 0.99F)
		V = 0;

	clipflags[0] = 0;
	clipflags[1] = 0;
	clipflags[2] = 0;
	clipflags[3] = 0;

	sx = (float)x * (float)phd_winxmax / 640.0F;
	sy = (float)y;
	w = (float)GetFixedScale(width);
	h = (float)GetFixedScale(height >> 1);

	tex.tpage = ushort(nTextures - 1);
	tex.drawtype = 0;
	tex.flag = 0;
	tex.u1 = 0;
	tex.v1 = V;
	tex.u2 = 1;
	tex.v2 = V;
	tex.u3 = 1;
	tex.v3 = V + 0.01F;
	tex.u4 = 0;
	tex.v4 = V + 0.01F;
	DrawColoredRect(sx, sy, sx + w, sy + h, f_mznear + 2, c1, c1, c2, c2, &tex);
	DrawColoredRect(sx, sy + h, sx + w, sy + (h * 2), f_mznear + 2, c2, c2, c1, c1, &tex);

	tex.tpage = 0;
	DrawColoredRect(sx - 1, sy - 1, sx + w + 1, sy + (h * 2) + 1, f_mznear + 4, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, &tex);

	w = pos * w / 100;
	tex.drawtype = 2;
	DrawColoredRect(sx, sy, sx + w + 1, sy + (h * 2), f_mznear + 1, c3, c3, c3, c3, &tex);
}

void S_InitLoadBar(long maxpos)
{
	loadbar_pos = 0;
	loadbar_maxpos = maxpos;
}

void S_LoadBar()
{
	long x, y, w, h;

	if (gfCurrentLevel || App.dx.Flags & DXF_HWR)
	{
		_BeginScene();
		InitBuckets();
		InitialiseSortList();
		App.dx.lpD3DDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		App.dx.lpD3DDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		App.dx.lpD3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
		loadbar_pos += 100 / loadbar_maxpos;

		if (tomb4.tr5_loadbar)
		{
			x = GetFixedScale(170);
			w = phd_winwidth - (x << 1);
			h = GetFixedScale(5);
			y = phd_winheight - h - GetFixedScale(20);

				DoBar(x, y, w, h, (long)loadbar_pos, 0xFF000000, 0xFF9F1F80, 0);
		}
		else
		{
			x = GetFixedScale(20);
			w = phd_winwidth - (x << 1);
			h = GetFixedScale(7);
			y = phd_winheight - h - GetFixedScale(20);

				DoBar(x, y, w, h, (long)loadbar_pos, 0xFF000000, 0xFF9F1F80, 0);
		}

		SortPolyList(SortCount, SortList);
		DrawSortList();
		S_DumpScreen();
	}
}
