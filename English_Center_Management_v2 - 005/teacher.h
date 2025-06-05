#pragma once

#include "linked_list.h"
#include "./lib/cJSON/cJSON.h"
#define MAX_NAME_LENGTH_TEACHER 100
#define MAX_EDUCATION_LEVEL_LENGTH 100

typedef struct GiangVien GiangVien;

int compareTeacherID(const int* real_id, const int* expected_id); // DK: hai id giong nhau
int compareTeacherName(const char* real_name, const char* expected_name); // DK: real_name la mot substring cua expected_name
int compareTeacherEduLevel(const char* real_level, const char* expected_level); // DK: real_level == expected_level
int compareTeacher(const GiangVien* gv1, const GiangVien* gv2); // Dieu kien ss: hai giang vien co thong tin trung nhau

void printTeacher(const GiangVien* hv);
void printAllTeacher(const LinkedList* list);

GiangVien* createTeacher();

int freeTeacher(GiangVien* hv);

// Hàm tìm kiếm / lọc học viên dựa trên thông tin do người dùng nhập vào
GiangVien** findTeacher(const LinkedList* list, int* resultLength, int useSearchOption, int maxResultLength);

// Hàm sửa thông tin học viên. Trả về 1 nếu thành công, 0 nếu thất bại
int enterTeacherInfo(GiangVien* gv, int useSearchOption);

int getTeacherID(const GiangVien* gv);

cJSON* giangVienToJSON(const GiangVien* gv);

GiangVien* parseGiangVienFromObject(cJSON* obj);
