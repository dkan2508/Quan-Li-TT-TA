#pragma once

#include <time.h>

#define SKIPPABLE_SCANF_BUFFER 200

// Kiểm tra đầu vào, nếu không hợp lệ sẽ ép người dùng nhập lại
void saferScanf(char formatString[], void* addr);

void printDate(time_t time);
void printTime(time_t time);