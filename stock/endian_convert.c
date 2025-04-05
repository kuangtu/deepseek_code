

// 将 32 位整数转换为 Big-Endian 格式
uint32_t toBigEndian32(uint32_t value) {
    return ((value & 0xFF000000) >> 24) |
           ((value & 0x00FF0000) >> 8)  |
           ((value & 0x0000FF00) << 8)  |
           ((value & 0x000000FF) << 24);
}

// 将 16 位整数转换为 Big-Endian 格式
uint16_t toBigEndian16(uint16_t value) {
    return ((value & 0xFF00) >> 8) | ((value & 0x00FF) << 8);
}

/* 方法1：基于位操作的通用实现（不依赖平台特性） */
uint64_t toBigEndian64(uint64_t value) {
    return ((value & 0x00000000000000FFULL) << 56) |  // 字节0 -> 字节7
           ((value & 0x000000000000FF00ULL) << 40) |  // 字节1 -> 字节6
           ((value & 0x0000000000FF0000ULL) << 24) |  // 字节2 -> 字节5
           ((value & 0x00000000FF000000ULL) << 8)  |  // 字节3 -> 字节4
           ((value & 0x000000FF00000000ULL) >> 8)  |  // 字节4 -> 字节3
           ((value & 0x0000FF0000000000ULL) >> 24) |  // 字节5 -> 字节2
           ((value & 0x00FF000000000000ULL) >> 40) |  // 字节6 -> 字节1
           ((value & 0xFF00000000000000ULL) >> 56);   // 字节7 -> 字节0
}



