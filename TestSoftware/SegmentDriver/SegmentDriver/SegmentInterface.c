#include "SegmentInterface.h"
#include "AddressableLED.h"
//#include "SPI.h"
#include "Timer.h"
#include <memory.h>
// Total Data size, 2 x 7 segments, 3 colors per segment, 4 bits of data per bit of color
// 24 bits of color, 24 * 4 (bits of SPI data) / 8 = 12 bytes
#define NUMBER_SEGMENTS 7 * 2
#define BYTES_PER_SEGMENT 12

// 7 Segment Data ordering is:
// D1->E1->F1->A1->B1->G1->C1->E2->F2->A2->B2->C2->G2->D2

typedef struct 
{
	colorBuffer R;
	colorBuffer G;
	colorBuffer B;
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

static inline void ColorToPadded(Color c, PaddedColor* pc)
{
	FillColor((uint8_t*)pc, c);
}

static inline void SetDark(PaddedColor* c)
{
	static const Color ColorDark = { 0, 0, 0 };
	ColorToPadded(ColorDark, c);
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
			SetDark(g);
			break;
		}
	case Display_1:
		{
			SetDark(a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			SetDark(d);
			SetDark(e);
			SetDark(f);
			SetDark(g);
			break;
		}
	case Display_2:
		{
			ColorToPadded(col, a);
			ColorToPadded(col, b);
			SetDark(c);
			ColorToPadded(col, d);
			ColorToPadded(col, e);
			SetDark(f);
			ColorToPadded(col, g);
			break;                                 
		}
	case Display_3:
		{
			ColorToPadded(col, a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			ColorToPadded(col, d);
			SetDark(e);
			SetDark(f);
			ColorToPadded(col, g);
			break;
		}
	case Display_4:
		{
			SetDark(a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			SetDark(d);
			SetDark(e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_S:
	case Display_5:
		{
			ColorToPadded(col, a);
			SetDark(b);
			ColorToPadded(col, c);
			ColorToPadded(col, d);
			SetDark(e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_6:
		{
			ColorToPadded(col, a);
			SetDark(b);
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
			SetDark(d);
			SetDark(e);
			SetDark(f);
			SetDark(g);
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
			SetDark(d);
			SetDark(e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_A:
		{
			ColorToPadded(col, a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			SetDark(d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_b:
		{
			SetDark(a);
			SetDark(b);
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
			SetDark(b);
			SetDark(c);
			ColorToPadded(col, d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			SetDark(g);
			break;
		}
	case Display_d:
		{
			SetDark(a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			ColorToPadded(col, d);
			ColorToPadded(col, e);
			SetDark(f);
			ColorToPadded(col, g);
			break;
		}
	case Display_E:
		{
			ColorToPadded(col, a);
			SetDark(b);
			SetDark(c);
			ColorToPadded(col, d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_F:
		{
			ColorToPadded(col, a);
			SetDark(b);
			SetDark(c);
			SetDark(d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_H:
		{
			SetDark(a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			SetDark(d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_L:
		{
			SetDark(a);
			SetDark(b);
			SetDark(c);
			ColorToPadded(col, d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			SetDark(g);
			break;
		}
	case Display_P:
		{
			ColorToPadded(col, a);
			ColorToPadded(col, b);
			SetDark(c);
			SetDark(d);
			ColorToPadded(col, e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_Y:
		{
			SetDark(a);
			ColorToPadded(col, b);
			ColorToPadded(col, c);
			ColorToPadded(col, d);
			SetDark(e);
			ColorToPadded(col, f);
			ColorToPadded(col, g);
			break;
		}
	case Display_Nothing:
		{
			SetDark(a);
			SetDark(b);
			SetDark(c);
			SetDark(d);
			SetDark(e);
			SetDark(f);
			SetDark(g);
			break;
		}
	}
}

void SetDisplay(SegmentVal v0, Color c0, SegmentVal v1, Color c1)
{
	// Kind of gross way to do this. But hey. It works.
	FillBuffer(v0, c0, &displayBuffer.seg0.A, &displayBuffer.seg0.B, &displayBuffer.seg0.C, &displayBuffer.seg0.D, &displayBuffer.seg0.E, &displayBuffer.seg0.F, &displayBuffer.seg0.G);
	FillBuffer(v1, c1, &displayBuffer.seg1.A, &displayBuffer.seg1.B, &displayBuffer.seg1.C, &displayBuffer.seg1.D, &displayBuffer.seg1.E, &displayBuffer.seg1.F, &displayBuffer.seg1.G);
	
	//SendTimerDMA((uint8_t*)&tmpBuffer, 4*sizeof(uint16_t));
}

SegmentVal GetSegmentForInt(uint8_t singleDecimal)
{
	if (singleDecimal > 9)
	{
		return Display_9;
	}
	return (SegmentVal)singleDecimal;
}