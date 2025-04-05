#ifndef __MDGWHQ_H
#pragma pack(push, 1)

/* 单笔委托明细 */
typedef struct {
    int64_t OrderQty;  // 委托量 (N15(2), Big-Endian)
} OrderEntry;

/* 行情条目（动态包含委托明细） */
typedef struct {
    char     MDEntryType[2];  // 条目类型
    int64_t  MDEntryPx;       // 价格 (N18(6))
    int64_t  MDEntrySize;     // 数量 (N15(2))
    uint16_t MDPriceLevel;    // 档位
    int64_t  NumberOfOrders;  // 总委托笔数
    uint16_t NoOrders;        // 实际揭示笔数
    OrderEntry Orders[];      // 柔性数组（C99 Flexible Array Member）
} MDEntry;

/* 扩展字段头 */
typedef struct {
    uint16_t NoMDEntries;     // 行情条目个数
    MDEntry  FirstEntry[];      // 首个条目（后续条目动态计算偏移）
} SnapshotExt300111;

typedef struct {
    /* 标准消息头 (8字节) */
    uint32_t BodyLength;      // 消息体长度 (Big-Endian)
    uint32_t MsgType;         // 消息类型 (300111, Big-Endian)

    /* 公共行情字段 (参考规范第4.5.4节) */
    uint64_t OrigTime;        // 行情生成时间 (YYYYMMDDHHMMSSsss, Big-Endian)
    uint16_t ChannelNo;       // 频道代码 (Big-Endian)
    char     MDStreamID[3];   // 行情类别 ("010"=集中竞价)
    char     SecurityID[8];   // 证券代码 (如"000001")
    char     SecurityIDSource[4]; // 代码源 ("102"=深交所)
    char     TradingPhaseCode[8]; // 交易阶段代码 (如"T"=连续竞价)
    int64_t  PrevClosePx;     // 昨收价 (N18(4), Big-Endian)
    int64_t  NumTrades;       // 成交笔数 (Big-Endian)
    int64_t  TotalVolumeTrade;// 成交总量 (N15(2), Big-Endian)
    int64_t  TotalValueTrade; // 成交总金额 (N18(4), Big-Endian)
    uint16_t NoMDEntries;     // 行情条目个数
    MDEntry  FirstEntry[];      // 首个条目（后续条目动态计算偏移）
} SnapshotBase;
#pragma pack(pop)
#endif  //__MDGWHQ_H
