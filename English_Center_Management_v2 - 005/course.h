#pragma once

#include "linked_list.h"
#include "./lib/cJSON/cJSON.h"

#define MAX_NAME_LENGTH_COURSE 100

typedef struct KhoaHoc KhoaHoc;

int getModOptions(int* optionArr);

int compareCourseID(const int* real_id, const int* expected_id); // DK: hai id giong nhau
int compareCourseName(const char* real_name, const char* expected_name); // DK: real_name la mot substring cua expected_name
int compareCourseTuition(const float* real_tuition, const float* expected_tuition); // DK: real_tuition = expected_tuition
int compareCourse(const KhoaHoc* cr1, const KhoaHoc* cr2); // Dieu kien ss: hai giang vien co thong tin trung nhau

void printCourse(const KhoaHoc* cr);
void printAllCourse(const LinkedList* list);

KhoaHoc* createCourse();

int freeCourse(KhoaHoc* cr);

// Hàm tìm kiếm / lọc học viên dựa trên thông tin do người dùng nhập vào
KhoaHoc** findCourse(const LinkedList* list, int* resultLength, int useSearchOption, int maxResultLength);

// Hàm sửa thông tin học viên. Trả về 1 nếu thành công, 0 nếu thất bại
int enterCourseInfo(KhoaHoc* cr, int useSearchOption);

cJSON* khoaHocToJSON(const KhoaHoc* kh);

KhoaHoc* parseKhoaHocFromObject(cJSON* obj);

