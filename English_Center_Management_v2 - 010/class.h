#pragma once

#include "linked_list.h"
#include "student.h"
#include "teacher.h"
#include "./lib/cJSON/cJSON.h"

#define MAX_STUDENT 100

typedef struct LopHoc LopHoc;

typedef int(*classSubjectChecker)(int, const LinkedList*);
typedef struct ClassValidationContext {
    const LinkedList* list;
    classSubjectChecker checker;
} ClassValidationContext;

typedef struct ClassSubjectValidationContext {
    const ClassValidationContext courseContext;
    const ClassValidationContext teacherContext;
} ClassSubjectValidationContext;

int compareClassID(const int* real_id, const int* expected_id);
int compareClassCourseID(const char* real_course_id, const char* expected_course_id); 
int compareClassTeacherID(const int* real_teacher_id, const int* expected_teacher_id);

// Hàm so sánh hai đối tượng học viên. Trả về 1 nếu điều kiện so sánh thỏa mãn, 0 nếu không thỏa mãn
int compareClass(const LopHoc* cls1, const LopHoc* cls2); // Dieu kien ss: hai hoc sinh co thong tin trung nhau

void printClass(const LopHoc* cls);
void printAllClass(const LinkedList* list);
void printStudentList(const LopHoc* cls);

LopHoc* createClass();

LopHoc* copyClass(LopHoc* destination, const LopHoc* source);

int isClassExist(int classID, const LinkedList* courseList);

int isClassSubjectExist(const LopHoc* cls, const ClassSubjectValidationContext ctx);

int freeClass(LopHoc* cls);

int freeClassCallback(LopHoc* cls, void* placeholder);

// Hàm tìm kiếm / lọc học viên dựa trên thông tin do người dùng nhập vào
LopHoc** findClass(const LinkedList* list, int* resultLength, int useSearchOption, int maxResultLength);

// Hàm sửa thông tin học viên. Trả về 1 nếu thành công, 0 nếu thất bại
int enterClassInfo(LopHoc* cls, int useSearchOption);

int countStudent(const LopHoc* cls);

int assignStudentToClass(LopHoc* cls, const HocVien* hv);

int removeStudentFromClass(LopHoc* cls, const HocVien* hv);

int isClassAvailable(const LopHoc* cls);

int isClassFull(const LopHoc* cls);

int isClassEmpty(const LopHoc* cls);

int isStudentInClass(const LopHoc* cls, const HocVien* hv);

int getClassID(const LopHoc* cls);

cJSON* lopHocToJSON(const LopHoc* lop);

LopHoc* parseLopHocFromObject(cJSON* obj);
