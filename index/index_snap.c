#include "index_snapshot.h"
#include <stdlib.h>
#include <string.h>


    
#define SET_ENTRY(idx, type, value) \
    entry = IndexSnapshot_GetEntry(snap, idx); \
    strcpy(entry->MDEntryType, type); \
    entry->MDEntryPx = toBigEndian64((int64_t)(value * INDEX_SCALE_FACTOR))
		
		
/* 计算实际需要的内存大小 */
static size_t IndexSnapshot_CalcSize(uint16_t numEntries) {
    return sizeof(IndexSnapshot) + 
           (numEntries - 1) * sizeof(IndexEntry); // 柔性数组已包含1个条目
}

/* 创建指数快照 (需指定条目数) */
IndexSnapshot* IndexSnapshot_Create(uint16_t numEntries) {
    if (numEntries > MAX_INDEX_ENTRIES) return NULL;

    IndexSnapshot *snap = malloc(IndexSnapshot_CalcSize(numEntries));
    if (snap) {
        memset(snap, 0, IndexSnapshot_CalcSize(numEntries));
        snap->Ext.NoMDEntries = toBigEndian16(numEntries);
        snap->MsgType = toBigEndian32(MSG_TYPE_INDEX_SNAPSHOT);
    }
    return snap;
}

/* 安全获取条目指针 */
IndexEntry* IndexSnapshot_GetEntry(IndexSnapshot *snap, uint16_t index) {
    if (!snap || index >= fromBigEndian16(snap->Ext.NoMDEntries)) return NULL;
    return &snap->Ext.Entries[index];
}

/* 释放内存 */
void IndexSnapshot_Free(IndexSnapshot *snap) {
    free(snap);
}


void InitSZComponentIndex(IndexSnapshot *snap) {
    // 基础字段
    snap->BodyLength = toBigEndian32(IndexSnapshot_CalcSize(6) - 8);
    snap->OrigTime = toBigEndian64(20240522150000000);
    snap->ChannelNo = toBigEndian16(CHANNEL_MAIN_INDEX);
    strcpy(snap->MDStreamID, "900");
    strcpy(snap->SecurityID, "399001");
    strcpy(snap->SecurityIDSource, "102");
    strcpy(snap->TradingPhaseCode, "E"); // 已闭市

    // 设置条目数据
    IndexEntry *entry;


    SET_ENTRY(0, ENTRY_TYPE_CURRENT, 3923.45);
    SET_ENTRY(1, ENTRY_TYPE_PREV_CLOSE, 3902.18);
    SET_ENTRY(2, ENTRY_TYPE_OPEN, 3910.25);
    SET_ENTRY(3, ENTRY_TYPE_HIGH, 3935.67);
    SET_ENTRY(4, ENTRY_TYPE_LOW, 3918.03);
    SET_ENTRY(5, ENTRY_TYPE_CLOSE, 3923.45);

    #undef SET_ENTRY
}