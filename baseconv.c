// Author: 8891689
#include "baseconv.h"
#include <stdio.h>
#include <string.h>

/* 進制轉換：將無符號整數轉為對應進制字符串（先倒序再反轉） */
void convert_base(unsigned long long value, const char* digits, char* out, size_t out_size)
{
    int radix = (int)strlen(digits);
    char buf[65]; /* 64 位二進制最大長度+結束符 */
    int i = 0;
    
    if (value == 0) {
        buf[i++] = digits[0];
    } else {
        while (value > 0 && i < (int)(sizeof(buf) - 1)) {
            buf[i++] = digits[value % radix];
            value /= radix;
        }
    }
    buf[i] = '\0';
    
    /* 檢查輸出緩衝區大小 */
    if ((size_t)(i + 1) > out_size) {
        if (out_size > 0)
            out[0] = '\0';
        return;
    }
    
    /* 反轉 buf 存入 out */
    for (int j = 0; j < i; j++) {
        out[j] = buf[i - 1 - j];
    }
    out[i] = '\0';
}

/* 反向轉換：將進制字符串轉回無符號整數 */
unsigned long long convert_from_base(const char* str, const char* digits)
{
    int radix = (int)strlen(digits);
    unsigned long long result = 0;
    const char* p = str;
    while (*p) {
        /* 查找當前字元在字元表中的位置 */
        const char* pos = strchr(digits, *p);
        if (pos == NULL) {
            /* 若找不到，則輸入字符串包含無效字元 */
            return 0;  /* 或者根據需要進行錯誤處理 */
        }
        int digit_val = (int)(pos - digits);
        result = result * radix + digit_val;
        p++;
    }
    return result;
}


