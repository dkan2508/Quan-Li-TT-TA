#pragma once

#include <time.h>

#define SKIPPABLE_SCANF_BUFFER 200

// Kiểm tra đầu vào, nếu không hợp lệ sẽ ép người dùng nhập lại
void saferScanf(char formatString[], void* addr);
int skippableScanf(const char* format, void* result, int allow_skip);

void printDate(time_t time);
void printTime(time_t time);