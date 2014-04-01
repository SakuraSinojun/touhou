

#include "dice.h"
#include <stdlib.h>
#include <time.h>

namespace sadv {

static bool g_inited = false;

void dice_init(void)
{
    srand((unsigned int)time(NULL));
}

int dice(int faces, int times)
{
    if (!g_inited) {
        dice_init();
        g_inited = true;
    }
    if (faces == 0)
        return 0;
    int i;
    int d = 0;
    for (i=0; i<times; i++) {
        int t = rand() % faces + 1; 
        d += t;
    }
    return d;
}

static int dice5d6k2_proc(const void * p1, const void * p2)
{
    int n1 = *(const int *)p1;
    int n2 = *(const int *)p2;
    return n1 < n2;
}

int dice5d6k2()
{
    int a[5];
    int i;
    for (i=0; i<5; i++) {
        a[i] = d6();
    }
    qsort(a, 5, sizeof(a[0]), dice5d6k2_proc);
    return a[0] + a[1] + a[2];
}

} // namespace sadv

