#pragma once

#include "linked_list.h"
#include "./lib/cJSON/cJSON.h"
#define MAX_NAME_LENGTH_TEACHER 100
#define MAX_EDUCATION_LEVEL_LENGTH 100

typedef struct GiangVien GiangVien;

int compareTeacherID(const int* real_id, const int* expected_id);
int compareTeacherName(const char* real_name, const char* expected_name);
int compareTeacherEduLevel(const char* real_level, const char* expected_level);
int compareTeacher(const GiangVien* gv1, const GiangVien* gv2);

void printTeacher(const GiangVien* hv);
void printAllTeacher(const LinkedList* list);

GiangVien* createTeacher();

GiangVien* copyTeacher(GiangVien* destination, const GiangVien* source);

int freeTeacher(GiangVien* hv);

int freeTeacherCallback(GiangVien* gv, void* placeholder);

// Hàm tìm kiếm / lọc học viên dựa trên thông tin do người dùng nhập vào
GiangVien** findTeacher(const LinkedList* list, int* resultLength, int useSearchOption, int maxResultLength);

// Hàm sửa thông tin học viên. Trả về 1 nếu thành công, 0 nếu thất bại
int enterTeacherInfo(GiangVien* gv, int useSearchOption);

int getTeacherID(const GiangVien* gv);

int isTeacherExist(int teacherID, const LinkedList* teacherList);

cJSON* giangVienToJSON(const GiangVien* gv);

GiangVien* parseGiangVienFromObject(cJSON* obj);
