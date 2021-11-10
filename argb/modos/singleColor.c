#include "singleColor.h"
#include "../stripControl.h"

void prepSingleColor()
{
    setStripBrightness(200);
    setStripColor_L(getColor(PRIMARY));
}
