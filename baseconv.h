// Author: 8891689
#ifndef BASECONV_H
#define BASECONV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * 將無符號整數 value 按照給定的字元表 digits 轉換成對應進制的字符串。
 * 轉換結果保存在 out 中，out_size 指定 out 緩衝區大小。
 * 若 out_size 不足，則會存入空字符串。
 */
void convert_base(unsigned long long value, const char* digits, char* out, size_t out_size);

/**
 * 將以給定 digits 表示的進制字符串 str 轉換回對應的無符號整數。
 * 若遇到無效字元，返回 0 並可根據需要擴展錯誤處理。
 */
unsigned long long convert_from_base(const char* str, const char* digits);

#ifdef __cplusplus
}
#endif

#endif // BASECONV_H

