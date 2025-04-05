#ifndef INDEX_SNAPSHOT_DEFS_H
#define INDEX_SNAPSHOT_DEFS_H

/* ---------- 消息类型 ---------- */
#define MSG_TYPE_INDEX_SNAPSHOT      309011U    // 指数行情消息类型

/* ---------- 频道代码 ---------- */
#define CHANNEL_MAIN_INDEX           10U        // 深市主板指数
#define CHANNEL_GEM_INDEX            11U        // 创业板指数
#define CHANNEL_HK_INDEX             50U        // 港股指数

/* ---------- 行情类别标识 ---------- */
#define MDSTREAMID_INDEX             "900"      // 指数行情标识
#define MDSTREAMID_HK_INDEX          "630"      // 港股指数标识

/* ---------- 证券代码源 ---------- */
#define SECURITY_SOURCE_SZSE         "102"      // 深交所
#define SECURITY_SOURCE_HKEX         "103"      // 港交所

/* ---------- 行情条目类型 ---------- */
#define ENTRY_TYPE_CURRENT       "3"     // 当前指数
#define ENTRY_TYPE_PREV_CLOSE    "xa"    // 昨日收盘
#define ENTRY_TYPE_OPEN          "xb"    // 今日开盘
#define ENTRY_TYPE_HIGH          "xc"    // 最高指数
#define ENTRY_TYPE_LOW           "xd"    // 最低指数
#define ENTRY_TYPE_CLOSE         "xl"    // 收盘指数

/* ---------- 交易阶段代码 ---------- */
#define TRADING_PHASE_CONTINUOUS     "T"        // 连续交易
#define TRADING_PHASE_CLOSED         "E"        // 已闭市
#define TRADING_PHASE_AUCTION        "C"        // 集合竞价

/* ---------- 数值限制 ---------- */
#define MAX_INDEX_ENTRIES            8         // 单消息最大条目数
#define INDEX_PRICE_SCALE            1000000LL  // 指数精度(10^6)
#define SECURITY_ID_LEN              8         // 证券代码长度
#define MDSTREAMID_LEN               3         // 行情类别长度
#define TRADING_PHASE_LEN            8         // 交易阶段代码长度

/* 指数精度 (N18(6)) */
#define INDEX_SCALE_FACTOR       1000000 // 10^6

#endif // INDEX_SNAPSHOT_DEFS_H
