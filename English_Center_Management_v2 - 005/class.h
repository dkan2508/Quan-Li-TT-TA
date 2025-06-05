#pragma once

#include "linked_list.h"
#include "student.h"
#include "teacher.h"
#include "./lib/cJSON/cJSON.h"

#define MAX_STUDENT 100

typedef struct LopHoc LopHoc;

int compareClassID(const int* real_id, const int* expected_id); // DK: hai id giong nhau
int compareClassCourseID(const char* real_course_id, const char* expected_course_id); // DK: real_name la mot substring cua expected_name
int compareClassTeacherID(const int* real_teacher_id, const int* expected_teacher_id); // DK: real_age == expected_age

// Hàm thêm học viên vào danh sách liên kết cho học viên

// Hàm so sánh hai đối tượng học viên. Trả về 1 nếu điều kiện so sánh thỏa mãn, 0 nếu không thỏa mãn
int compareClass(const LopHoc* cls1, const LopHoc* cls2); // Dieu kien ss: hai hoc sinh co thong tin trung nhau

// Hàm in thông tin học viên
void printClass(const LopHoc* cls);
void printAllClass(const LinkedList* list);

LopHoc* createClass();

int freeClass(LopHoc* cls);

// Hàm tìm kiếm / lọc học viên dựa trên thông tin do người dùng nhập vào
LopHoc** findClass(const LinkedList* list, int* resultLength, int useSearchOption, int maxResultLength);

// Hàm sửa thông tin học viên. Trả về 1 nếu thành công, 0 nếu thất bại
int enterClassInfo(LopHoc* cls, int useSearchOption);

int countStudent(const LopHoc* cls);

int assignStudentToClass(LopHoc* cls, const HocVien* hv);

int removeStudentFromClass(LopHoc* cls, const HocVien* hv);

int assignTeacherToClass(LopHoc* cls, const GiangVien* gv);

int removeTeacherFromClass(LopHoc* cls);

int isClassAvailable(const LopHoc* cls);

int isClassFull(const LopHoc* cls);

int isClassEmpty(const LopHoc* cls);

int isStudentInClass(const LopHoc* cls, const HocVien* hv);

int isTeacherInClass(const LopHoc* cls, const GiangVien* gv);

int isClassHasTeacher(const LopHoc* cls);

cJSON* lopHocToJSON(const LopHoc* lop);

LopHoc* parseLopHocFromObject(cJSON* obj);
