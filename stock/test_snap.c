#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "mdgwhq.h"
#include "endian_convert.c"

/* 测试calcMDEntrySize函数 */
static void test_calcMDEntrySize(void **state) {
    (void)state; // 未使用
    
    // 测试0笔委托
    assert_int_equal(calcMDEntrySize(0), sizeof(MDEntry));
    
    // 测试1笔委托
    assert_int_equal(calcMDEntrySize(1), sizeof(MDEntry) + sizeof(OrderEntry));
    
    // 测试多笔委托
    assert_int_equal(calcMDEntrySize(5), sizeof(MDEntry) + 5 * sizeof(OrderEntry));
}

/* 测试getMDEntry函数 */
static void test_getMDEntry(void **state) {
    SnapshotBase *snap = createExampleSnapshot();
    
    // 测试获取有效条目
    MDEntry *entry = getMDEntry(snap, 0);
    assert_non_null(entry);
    assert_string_equal(entry->MDEntryType, "0");
    
    entry = getMDEntry(snap, 1);
    assert_non_null(entry);
    assert_string_equal(entry->MDEntryType, "1");
    
    entry = getMDEntry(snap, 2);
    assert_non_null(entry);
    assert_string_equal(entry->MDEntryType, "2");
    
    // 测试越界访问
    assert_null(getMDEntry(snap, 3));
    assert_null(getMDEntry(snap, 100));
    
    freeSnapshot(snap);
}

/* 测试getSnapshotSize函数 */
static void test_getSnapshotSize(void **state) {
    SnapshotBase *snap = createExampleSnapshot();
    
    // 计算预期大小
    size_t expected_size = sizeof(SnapshotBase) +
                          calcMDEntrySize(2) +  // 买1档
                          calcMDEntrySize(1) +  // 卖1档
                          calcMDEntrySize(0);   // 最新价
    
    assert_int_equal(getSnapshotSize(snap), expected_size);
    
    freeSnapshot(snap);
}

/* 测试createExampleSnapshot创建的数据 */
static void test_exampleSnapshot_content(void **state) {
    SnapshotBase *snap = createExampleSnapshot();
    
    // 验证基础字段
    assert_int_equal(toBigEndian32(snap->MsgType), 300111);
    assert_string_equal(snap->SecurityID, "000001");
    assert_string_equal(snap->TradingPhaseCode, "T");
    assert_int_equal(toBigEndian16(snap->NoMDEntries), 3);
    
    // 验证买1档数据
    MDEntry *bid = getMDEntry(snap, 0);
    assert_string_equal(bid->MDEntryType, "0");
    assert_int_equal(toBigEndian64(bid->MDEntryPx), 186400000);
    assert_int_equal(toBigEndian16(bid->NoOrders), 2);
    assert_int_equal(toBigEndian64(bid->Orders[0].OrderQty), 20000);
    assert_int_equal(toBigEndian64(bid->Orders[1].OrderQty), 15000);
    
    // 验证卖1档数据
    MDEntry *ask = getMDEntry(snap, 1);
    assert_string_equal(ask->MDEntryType, "1");
    assert_int_equal(toBigEndian64(ask->MDEntryPx), 186500000);
    assert_int_equal(toBigEndian16(ask->NoOrders), 1);
    assert_int_equal(toBigEndian64(ask->Orders[0].OrderQty), 28000);
    
    // 验证最新价数据
    MDEntry *last = getMDEntry(snap, 2);
    assert_string_equal(last->MDEntryType, "2");
    assert_int_equal(toBigEndian64(last->MDEntryPx), 186400000);
    assert_int_equal(toBigEndian16(last->NoOrders), 0);
    
    freeSnapshot(snap);
}

/* 主测试函数 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_calcMDEntrySize),
        cmocka_unit_test(test_getMDEntry),
        cmocka_unit_test(test_getSnapshotSize),
        cmocka_unit_test(test_exampleSnapshot_content),
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}