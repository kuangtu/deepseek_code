#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "mdgwhq.h"
#include "endian_convert.c"

/* 计算单个MDEntry及其Orders所需内存 */
size_t calcMDEntrySize(uint16_t noOrders) {
    return sizeof(MDEntry) + noOrders * sizeof(OrderEntry);
}


/* 安全获取第index个MDEntry (返回NULL表示越界) */
MDEntry* getMDEntry(SnapshotBase *snap, uint16_t index) {
    if (index >= toBigEndian16(snap->NoMDEntries)) return NULL;

    uint8_t *ptr = (uint8_t*)&snap->FirstEntry;
    for (uint16_t i = 0; i < index; i++) {
        MDEntry *entry = (MDEntry*)ptr;
        ptr += calcMDEntrySize(toBigEndian16(entry->NoOrders));
    }
    return (MDEntry*)ptr;
}


/* 释放整个快照结构 */
void freeSnapshot(SnapshotBase *snap) {
    free(snap); // 一次性释放所有内存（包含柔性数组）
}


/* 获取快照实际占用的内存大小 */
size_t getSnapshotSize(SnapshotBase *snap) {
    size_t size = sizeof(SnapshotBase);
    MDEntry *entry = snap->FirstEntry;

    for (uint16_t i = 0; i < toBigEndian16(snap->NoMDEntries); i++) {
        size += calcMDEntrySize(toBigEndian16(entry->NoOrders));
        entry = (MDEntry*)((uint8_t*)entry + calcMDEntrySize(toBigEndian16(entry->NoOrders)));
    }
    return size;
}
/* 初始化一个包含3个行情条目的快照 */
SnapshotBase* createExampleSnapshot() {
    // 计算总内存大小 (基础部分 + 3个MDEntry + 委托明细)
    size_t total_size = sizeof(SnapshotBase) +
                       calcMDEntrySize(2) +  // 买1档(2笔委托)
                       calcMDEntrySize(1) +  // 卖1档(1笔委托)
                       calcMDEntrySize(0);   // 最新价(无委托)

    // 分配内存
    SnapshotBase *snap = malloc(total_size);
    memset(snap, 0, total_size);

    /* 填充基础字段 */
    snap->BodyLength = toBigEndian32(total_size - sizeof(uint32_t)*2); // 扣除MsgType和BodyLength
    snap->MsgType = toBigEndian32(300111);
    snap->OrigTime = toBigEndian64(20240520145930000); // 2024-05-20 14:59:30.000
    snap->ChannelNo = toBigEndian16(1010); // 股票频道
    strcpy(snap->MDStreamID, "010");
    strcpy(snap->SecurityID, "000001");
    strcpy(snap->SecurityIDSource, "102");
    strcpy(snap->TradingPhaseCode, "T"); // 连续竞价
    snap->PrevClosePx = toBigEndian64(185000000); // 昨收18.50元
    snap->NoMDEntries = toBigEndian16(3); // 共3个条目

    /* 填充买1档 (2笔委托) */
    MDEntry *bid = getMDEntry(snap, 0);
    strcpy(bid->MDEntryType, "0");
    bid->MDEntryPx = toBigEndian64(186400000); // 18.64元
    bid->MDEntrySize = toBigEndian64(35000);    // 总数量
    bid->MDPriceLevel = toBigEndian16(1);
    bid->NumberOfOrders = toBigEndian64(2);     //	总委托笔数
    bid->NoOrders = toBigEndian16(2);           // 揭示2笔
    bid->Orders[0].OrderQty = toBigEndian64(20000); // 第一笔
    bid->Orders[1].OrderQty = toBigEndian64(15000); // 第二笔

    /* 填充卖1档 (1笔委托) */
    MDEntry *ask = getMDEntry(snap, 1);
    strcpy(ask->MDEntryType, "1");
    ask->MDEntryPx = toBigEndian64(186500000); // 18.65元
    ask->MDEntrySize = toBigEndian64(28000);
    ask->MDPriceLevel = toBigEndian16(1);
    ask->NumberOfOrders = toBigEndian64(1);
    ask->NoOrders = toBigEndian16(1);
    ask->Orders[0].OrderQty = toBigEndian64(28000);

    /* 填充最新价 (无委托明细) */
    MDEntry *last = getMDEntry(snap, 2);
    strcpy(last->MDEntryType, "2");
    last->MDEntryPx = toBigEndian64(186400000); // 最新成交价18.64
    last->MDEntrySize = toBigEndian64(100);     // 最新成交量
    last->NumberOfOrders = toBigEndian64(1);    // 无意义字段
    last->NoOrders = toBigEndian16(0);          // 无委托明细

    return snap;
}

void printSnapshot(SnapshotBase *snap) {
    printf("证券代码: %s\n", snap->SecurityID);
    printf("最新价: %.2f\n", toBigEndian64(getMDEntry(snap, 2)->MDEntryPx) / 1e6);

    // 打印买一档明细
    MDEntry *bid = getMDEntry(snap, 0);
    printf("买一档: %.2f 总委量:%lld\n", 
           toBigEndian64(bid->MDEntryPx) / 1e6,
           toBigEndian64(bid->MDEntrySize));
    
    for (uint16_t i = 0; i < toBigEndian16(bid->NoOrders); i++) {
        printf("  委托%d: %lld股\n", i+1, toBigEndian64(bid->Orders[i].OrderQty));
    }
}

int
main()
{
  printf("the sizeof int is:%zu\n", sizeof(int));
  printf("Size of SnapshotBase:%zu\n", sizeof(SnapshotBase));
  SnapshotBase *snap = createExampleSnapshot();
  printSnapshot(snap);
  freeSnapshot(snap);
  return 0;
}
