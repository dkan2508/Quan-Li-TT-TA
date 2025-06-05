#pragma once

#include "linked_list.h"
#include "./lib/cJSON/cJSON.h"
#define MAX_NAME_LENGTH_STUDENT 100


typedef struct HocVien HocVien;

// Các hàm so sánh các field của struct HocVien
int compareStudentID(const int* real_id, const int* expected_id); // DK: hai id giong nhau
int compareStudentName(const char* real_name, const char* expected_name); // DK: real_name la mot substring cua expected_name
int compareStudentAge(const int* real_age, const int* expected_age); // DK: real_age == expected_age

// Hàm thêm học viên vào danh sách liên kết cho học viên

// Hàm so sánh hai đối tượng học viên. Trả về 1 nếu điều kiện so sánh thỏa mãn, 0 nếu không thỏa mãn
int compareStudent(const HocVien* hv1, const HocVien* hv2); // Dieu kien ss: hai hoc sinh co thong tin trung nhau

// Hàm in thông tin học viên
void printStudent(const HocVien* hv);
void printAllStudent(const LinkedList* list);

HocVien* createStudent();

int freeStudent(HocVien* hv);

// Hàm tìm kiếm / lọc học viên dựa trên thông tin do người dùng nhập vào
HocVien** findStudent(const LinkedList* list, int* resultLength, int useSearchOption, int maxResultLength);

// Hàm sửa thông tin học viên. Trả về 1 nếu thành công, 0 nếu thất bại
int enterStudentInfo(HocVien* hv, int useSearchOption);

int getStudentID(const HocVien* hv);

cJSON* hocVienToJSON(const HocVien* hv);
HocVien* parseHocVienFromObject(cJSON* obj);
