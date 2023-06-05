#include <stdio.h>

enum {
    LV_PART_MAIN         = 0x000000,   /**< A background like rectangle*/
    LV_PART_SCROLLBAR    = 0x010000,   /**< The scrollbar(s)*/
    LV_PART_INDICATOR    = 0x020000,   /**< Indicator, e.g. for slider, bar, switch, or the tick box of the checkbox*/
    LV_PART_KNOB         = 0x030000,   /**< Like handle to grab to adjust the value*/
    LV_PART_SELECTED     = 0x040000,   /**< Indicate the currently selected option or section*/
    LV_PART_ITEMS        = 0x050000,   /**< Used if the widget has multiple similar elements (e.g. table cells)*/
    LV_PART_TICKS        = 0x060000,   /**< Ticks on scale e.g. for a chart or meter*/
    LV_PART_CURSOR       = 0x070000,   /**< Mark a specific place e.g. for text area's cursor or on a chart*/
    LV_PART_CUSTOM_FIRST = 0x080000,    /**< Extension point for custom widgets*/
    LV_PART_ANY          = 0x0F0000,    /**< Special value can be used in some functions to target all parts*/
};

void f(unsigned char v){
	char c = v;
	unsigned char uc = v;
	unsigned int a = c, b = uc;
	int i = c, j = uc;
	printf("----------------\n");
	printf("%%c: %c, %c\n", c, uc);
	printf("%%X: %X, %X\n", c, uc);
	printf("%%u: %u, %u\n", a, b);
	printf("%%d: %d, %d\n", i, j);
	
	unsigned char uInt = LV_PART_MAIN;
	unsigned char uInt1 = LV_PART_SCROLLBAR;
	unsigned char uInt2 = LV_PART_INDICATOR;
	printf("uInt: %d, uInt1: %d, uInt2: %d\n", uInt, uInt1, uInt2);
	
	int iInt = LV_PART_MAIN;
	int iInt1 = LV_PART_SCROLLBAR;
	int iInt2 = LV_PART_INDICATOR;
	printf("iInt: %d, iInt1: %d, iInt2: %d\n", iInt, iInt1, iInt2);
}

int main(int argc, char *argv[])
{
    f(0x80);
    f(0x7F);
    return 0;
}