#include "SegmentInterface.h"
#include "AddressableLED.h"
#include "SPI.h"
#include <memory.h>
// Total Data size, 2 x 7 segments, 3 colors per segment, 4 bits of data per bit of color
// 24 bits of color, 24 * 4 (bits of SPI data) / 8 = 12 bytes
#define NUMBER_SEGMENTS 7 * 2
#define BYTES_PER_SEGMENT 12

// 7 Segment Data ordering is:
// D1->E1->F1->A1->B1->G1->C1->E2->F2->A2->B2->C2->G2->D2

typedef struct 
{
	uint32_t R;
	uint32_t G;
	uint32_t B;
}PaddedColor;

typedef struct 
{
	PaddedColor D;
	PaddedColor E;
	PaddedColor F;
	PaddedColor A;
	PaddedColor B;
	PaddedColor G;
	PaddedColor C;
}Segment0;

typedef struct 
{
	PaddedColor E;
	PaddedColor F;
	PaddedColor A;
	PaddedColor B;
	PaddedColor C;
	PaddedColor G;
	PaddedColor D;
}Segment1;

typedef struct 
{
	Segment0 seg0;
	Segment1 seg1;
}Display;

Display displayBuffer;

static inline void ClearPadded(PaddedColor* c)
{
	memset((uint8_t*)c, 0, sizeof(PaddedColor));
}

static inline void ColorToPadded(Color c, PaddedColor* pc)
{
	FillColor((uint8_t*)pc, c);
}

static inline void FillBuffer(SegmentVal v, Color col, PaddedColor* a, PaddedColor* b, PaddedColor* c, PaddedColor* d, PaddedColor* e, PaddedColor* f, PaddedColor* g)
{
	switch (v)
	{
	case Display_O:
	case Display_0:
		{
			ColorToPadded(col, a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			ColorToPadded(col, d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ClearPadded(g);
			break;
		}
	case Display_1:
		{
			ClearPadded(a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			ClearPadded(d);
			ClearPadded(e);
			ClearPadded(f);
			ClearPadded(g);
			break;
		}
	case Display_2:
		{
			ColorToPadded(col, a);
			ColorToPadded(col, b);
			ClearPadded(c);
			ColorToPadded(col, d);
			ColorToPadded(col, e);
			ClearPadded(f);
			ColorToPadded(col, g);
			break;                                 
		}
	case Display_3:
		{
			ColorToPadded(col, a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			ColorToPadded(col, d);
			ClearPadded(e);
			ClearPadded(f);
			ColorToPadded(col, g);
			break;
		}
	case Display_4:
		{
			ClearPadded(a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			ClearPadded(d);
			ClearPadded(e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_S:
	case Display_5:
		{
			ColorToPadded(col, a);
			ClearPadded(b);
			ColorToPadded(col, c);
			ColorToPadded(col, d);
			ClearPadded(e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_6:
		{
			ColorToPadded(col, a);
			ClearPadded(b);
			ColorToPadded(col, c);
			ColorToPadded(col, d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_7:
		{
			ColorToPadded(col, a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			ClearPadded(d);
			ClearPadded(e);
			ClearPadded(f);
			ClearPadded(g);
			break;
		}
	case Display_8:
		{
			ColorToPadded(col, a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			ColorToPadded(col, d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_9:
		{
			ColorToPadded(col, a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			ClearPadded(d);
			ClearPadded(e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_A:
		{
			ColorToPadded(col, a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			ClearPadded(d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_b:
		{
			ClearPadded(a);
			ClearPadded(b);
			ColorToPadded(col, c);
			ColorToPadded(col, d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_C:
		{
			ColorToPadded(col, a);
			ClearPadded(b);
			ClearPadded(c);
			ColorToPadded(col, d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ClearPadded(g);
			break;
		}
	case Display_d:
		{
			ClearPadded(a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			ColorToPadded(col, d);
			ColorToPadded(col, e);
			ClearPadded(f);
			ColorToPadded(col, g);
			break;
		}
	case Display_E:
		{
			ColorToPadded(col, a);
			ClearPadded(b);
			ClearPadded(c);
			ColorToPadded(col, d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_F:
		{
			ColorToPadded(col, a);
			ClearPadded(b);
			ClearPadded(c);
			ClearPadded(d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_H:
		{
			ClearPadded(a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			ClearPadded(d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_L:
		{
			ClearPadded(a);
			ClearPadded(b);
			ClearPadded(c);
			ColorToPadded(col, d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ClearPadded(g);
			break;
		}
	case Display_P:
		{
			ColorToPadded(col, a);
			ColorToPadded(col, b);
			ClearPadded(c);
			ClearPadded(d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_Y:
		{
			ClearPadded(a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			ColorToPadded(col, d);
			ClearPadded(e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_Nothing:
		{
			ClearPadded(a);
			ClearPadded(b);
			ClearPadded(c);
			ClearPadded(d);
			ClearPadded(e);
			ClearPadded(f);
			ClearPadded(g);
			break;
		}
	}
}

void SetDisplay(SegmentVal v0, Color c0, SegmentVal v1, Color c1)
{
	// Kind of gross way to do this. But hey. It works.
	FillBuffer(v0, c0, &displayBuffer.seg0.A, &displayBuffer.seg0.B, &displayBuffer.seg0.C, &displayBuffer.seg0.D, &displayBuffer.seg0.E, &displayBuffer.seg0.F, &displayBuffer.seg0.G);
	FillBuffer(v1, c1, &displayBuffer.seg1.A, &displayBuffer.seg1.B, &displayBuffer.seg1.C, &displayBuffer.seg1.D, &displayBuffer.seg1.E, &displayBuffer.seg1.F, &displayBuffer.seg1.G);
	
	SendSPIDMA((uint8_t*)&displayBuffer, sizeof(displayBuffer));
}

SegmentVal GetSegmentForInt(uint8_t singleDecimal)
{
	if (singleDecimal > 9)
	{
		return Display_9;
	}
	return (SegmentVal)singleDecimal;
}