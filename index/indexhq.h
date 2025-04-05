#ifndef __INDEXHQ_H
#define __INDEXHQ_H
#pragma pack(push, 1)

#include "index_snapshot_defs.h"
#include <stdint.h>


//单条指数行情条目
typedef struct {
    char     MDEntryType[2];               // 类型 (e.g. MDENTRY_CURRENT)
    int64_t  MDEntryPx;                    // 价格 (需乘以INDEX_PRICE_SCALE)
} IndexEntry;

//指数行情扩展字段 (柔性数组实现)
typedef struct {
    uint16_t NoMDEntries;                  // 实际条目数 (≤MAX_INDEX_ENTRIES)
    IndexEntry Entries[];                  // 动态条目数组
} IndexSnapshotExt;


//完整的指数行情消息结构
//结构体长度，不含柔性数组长度
//4 + 4+ 8 +2 + 3+8+4+8 = 41
//最后的部分，IndexSnapshotExt结构体，有`uint16_t NoMDEntries;`
//字段
typedef struct {
    /* ----- 标准消息头 ----- */
    uint32_t BodyLength;                   // 消息体长度 (需转Big-Endian)
    uint32_t MsgType;                      // 固定为MSG_TYPE_INDEX_SNAPSHOT

    /* ----- 公共行情字段 ----- */
    uint64_t OrigTime;                     // 时间戳 (YYYYMMDDHHMMSSsss)
    uint16_t ChannelNo;                    // 使用CHANNEL_*系列宏
    char     MDStreamID[MDSTREAMID_LEN];   // 行情类别 (e.g. MDSTREAMID_INDEX)
    char     SecurityID[SECURITY_ID_LEN];  // 指数代码 (e.g. "399001")
    char     SecurityIDSource[4];          // 代码源 (e.g. SECURITY_SOURCE_SZSE)
    char     TradingPhaseCode[TRADING_PHASE_LEN]; // 交易阶段 (e.g. TRADING_PHASE_CONTINUOUS)

    /* ----- 扩展字段 ----- */
    IndexSnapshotExt Ext;                  // 柔性数组存储实际数据
} IndexSnapshot;

#pragma pack(pop)

#endif 