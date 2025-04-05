#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "indexhq.h"
#include "endian_convert.c"
#include "index_snap.c"

int main() {
    // 创建并初始化
    IndexSnapshot *szIndex = IndexSnapshot_Create(6);
    InitSZComponentIndex(szIndex);

    // 读取数据
    IndexEntry *closeEntry = IndexSnapshot_GetEntry(szIndex, 5);
    if (closeEntry) {
        double closePrice = toBigEndian64(closeEntry->MDEntryPx) / (double)INDEX_SCALE_FACTOR;
        printf("深证成指收盘: %.2f\n", closePrice);
    }

    IndexSnapshot_Free(szIndex);
    return 0;
}