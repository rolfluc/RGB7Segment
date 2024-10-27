#include "SegmentInterface.h"
#include "AddressableLED.h"
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

void SetDisplay(SegmentVal v0, Color c0, SegmentVal v1, Color c1)
{
	// TODO how do I generalize this? The whole point of the two different ones is to natively manage the pin changes as memory changes
	// I can make this gross but I don't want to.
	switch (v0)
	{
	case Display_O:
	case Display_0:
		{
			ColorToPadded(c0, &displayBuffer.seg0.A);
			ColorToPadded(c0, &displayBuffer.seg0.B);
			ColorToPadded(c0, &displayBuffer.seg0.C);
			ColorToPadded(c0, &displayBuffer.seg0.D);
			ColorToPadded(c0, &displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ClearPadded(&displayBuffer.seg0.G);
			break;
		}
	case Display_1:
		{
			ClearPadded(&displayBuffer.seg0.A);
			ColorToPadded(c0, &displayBuffer.seg0.B);
			ColorToPadded(c0, &displayBuffer.seg0.C);
			ClearPadded(&displayBuffer.seg0.D);
			ClearPadded(&displayBuffer.seg0.E);
			ClearPadded(&displayBuffer.seg0.F);
			ClearPadded(&displayBuffer.seg0.G);
			break;
		}
	case Display_2:
		{
			ColorToPadded(c0, &displayBuffer.seg0.A);
			ColorToPadded(c0, &displayBuffer.seg0.B);
			ClearPadded(&displayBuffer.seg0.C);
			ColorToPadded(c0, &displayBuffer.seg0.D);
			ColorToPadded(c0, &displayBuffer.seg0.E);
			ClearPadded(&displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;                                 
		}
	case Display_3:
		{
			ColorToPadded(c0, &displayBuffer.seg0.A);
			ColorToPadded(c0, &displayBuffer.seg0.B);
			ColorToPadded(c0, &displayBuffer.seg0.C);
			ColorToPadded(c0, &displayBuffer.seg0.D);
			ClearPadded(&displayBuffer.seg0.E);
			ClearPadded(&displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;
		}
	case Display_4:
		{
			ClearPadded(&displayBuffer.seg0.A);
			ColorToPadded(c0, &displayBuffer.seg0.B);
			ColorToPadded(c0, &displayBuffer.seg0.C);
			ClearPadded(&displayBuffer.seg0.D);
			ClearPadded(&displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;
		}
	case Display_S:
	case Display_5:
		{
			ColorToPadded(c0, &displayBuffer.seg0.A);
			ClearPadded(&displayBuffer.seg0.B);
			ColorToPadded(c0, &displayBuffer.seg0.C);
			ColorToPadded(c0, &displayBuffer.seg0.D);
			ClearPadded(&displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;
		}
	case Display_6:
		{
			ColorToPadded(c0, &displayBuffer.seg0.A);
			ClearPadded(&displayBuffer.seg0.B);
			ColorToPadded(c0, &displayBuffer.seg0.C);
			ColorToPadded(c0, &displayBuffer.seg0.D);
			ColorToPadded(c0, &displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;
		}
	case Display_7:
		{
			ColorToPadded(c0, &displayBuffer.seg0.A);
			ColorToPadded(c0, &displayBuffer.seg0.B);
			ColorToPadded(c0, &displayBuffer.seg0.C);
			ClearPadded(&displayBuffer.seg0.D);
			ClearPadded(&displayBuffer.seg0.E);
			ClearPadded(&displayBuffer.seg0.F);
			ClearPadded(&displayBuffer.seg0.G);
			break;
		}
	case Display_8:
		{
			ColorToPadded(c0, &displayBuffer.seg0.A);
			ColorToPadded(c0, &displayBuffer.seg0.B);
			ColorToPadded(c0, &displayBuffer.seg0.C);
			ColorToPadded(c0, &displayBuffer.seg0.D);
			ColorToPadded(c0, &displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;
		}
	case Display_9:
		{
			ColorToPadded(c0, &displayBuffer.seg0.A);
			ColorToPadded(c0, &displayBuffer.seg0.B);
			ColorToPadded(c0, &displayBuffer.seg0.C);
			ClearPadded(&displayBuffer.seg0.D);
			ClearPadded(&displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;
		}
	case Display_A:
		{
			ColorToPadded(c0, &displayBuffer.seg0.A);
			ColorToPadded(c0, &displayBuffer.seg0.B);
			ColorToPadded(c0, &displayBuffer.seg0.C);
			ClearPadded(&displayBuffer.seg0.D);
			ColorToPadded(c0, &displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;
		}
	case Display_b:
		{
			ClearPadded(&displayBuffer.seg0.A);
			ClearPadded(&displayBuffer.seg0.B);
			ColorToPadded(c0, &displayBuffer.seg0.C);
			ColorToPadded(c0, &displayBuffer.seg0.D);
			ColorToPadded(c0, &displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;
		}
	case Display_C:
		{
			ColorToPadded(c0, &displayBuffer.seg0.A);
			ClearPadded(&displayBuffer.seg0.B);
			ClearPadded(&displayBuffer.seg0.C);
			ColorToPadded(c0, &displayBuffer.seg0.D);
			ColorToPadded(c0, &displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ClearPadded(&displayBuffer.seg0.G);
			break;
		}
	case Display_d:
		{
			ClearPadded(&displayBuffer.seg0.A);
			ColorToPadded(c0, &displayBuffer.seg0.B);
			ColorToPadded(c0, &displayBuffer.seg0.C);
			ColorToPadded(c0, &displayBuffer.seg0.D);
			ColorToPadded(c0, &displayBuffer.seg0.E);
			ClearPadded(&displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;
		}
	case Display_E:
		{
			ColorToPadded(c0, &displayBuffer.seg0.A);
			ClearPadded(&displayBuffer.seg0.B);
			ClearPadded(&displayBuffer.seg0.C);
			ColorToPadded(c0, &displayBuffer.seg0.D);
			ColorToPadded(c0, &displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;
		}
	case Display_F:
		{
			ColorToPadded(c0, &displayBuffer.seg0.A);
			ClearPadded(&displayBuffer.seg0.B);
			ClearPadded(&displayBuffer.seg0.C);
			ClearPadded(&displayBuffer.seg0.D);
			ColorToPadded(c0, &displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;
		}
	case Display_H:
		{
			ClearPadded(&displayBuffer.seg0.A);
			ColorToPadded(c0, &displayBuffer.seg0.B);
			ColorToPadded(c0, &displayBuffer.seg0.C);
			ClearPadded(&displayBuffer.seg0.D);
			ColorToPadded(c0, &displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;
		}
	case Display_L:
		{
			ClearPadded(&displayBuffer.seg0.A);
			ClearPadded(&displayBuffer.seg0.B);
			ClearPadded(&displayBuffer.seg0.C);
			ColorToPadded(c0, &displayBuffer.seg0.D);
			ColorToPadded(c0, &displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ClearPadded(&displayBuffer.seg0.G);
			break;
		}
	case Display_P:
		{
			ColorToPadded(c0, &displayBuffer.seg0.A);
			ColorToPadded(c0, &displayBuffer.seg0.B);
			ClearPadded(&displayBuffer.seg0.C);
			ClearPadded(&displayBuffer.seg0.D);
			ColorToPadded(c0, &displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;
		}
	case Display_Y:
		{
			ClearPadded(&displayBuffer.seg0.A);
			ColorToPadded(c0, &displayBuffer.seg0.B);
			ColorToPadded(c0, &displayBuffer.seg0.C);
			ColorToPadded(c0, &displayBuffer.seg0.D);
			ClearPadded(&displayBuffer.seg0.E);
			ColorToPadded(c0, &displayBuffer.seg0.F);
			ColorToPadded(c0, &displayBuffer.seg0.G);
			break;
		}
	case Display_Nothing:
		{
			ClearPadded(&displayBuffer.seg0.A);
			ClearPadded(&displayBuffer.seg0.B);
			ClearPadded(&displayBuffer.seg0.C);
			ClearPadded(&displayBuffer.seg0.D);
			ClearPadded(&displayBuffer.seg0.E);
			ClearPadded(&displayBuffer.seg0.F);
			ClearPadded(&displayBuffer.seg0.G);
			break;
		}
	}
}