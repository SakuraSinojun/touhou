

#pragma once


namespace sadv {

int dice(int faces, int times);

int dice5d6k2();

#define dice4(x)    dice(4, x)
#define dice6(x)    dice(6, x)
#define dice20(x)   dice(20, x)
#define d20()       dice(20, 1)
#define d6()        dice(6, 1)

} // namespace sadv

