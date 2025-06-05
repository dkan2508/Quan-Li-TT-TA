#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "class.h"
#include "linked_list.h"
#include "general.h"
#include "./lib/cJSON/cJSON.h"

/*
Maybe somehow make teacher not a required fill for classroom creation
*/

#define RELEVANT_FIELD_AMOUNT 3

typedef struct ClassMetadata {
    int hasTeacher;
    int hasCourse;
    int studentCount;
} ClassMetadata;

typedef struct LopHoc {
    int id;
    int courseID;
    int teacherID;
    int studentIDList[MAX_STUDENT];

    ClassMetadata metadata;
} LopHoc;

// Cac ham khong xuat hien trong student.h
static int getModOptions(int* optionArr) {
    if(!optionArr) return 0;

    int result;
    int idx = 0;
    puts("===== Lua chon su dung truong thong tin (nhap 1 neu chon, 0 neu khong) ======");
    do {
        printf("Su dung: ID lop hoc? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    do {
        printf("Su dung: ID khoa hoc? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    do {
        printf("Su dung: ID giang vien? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    return 1;
}

int compareClassID(const int* real_id, const int* expected_id) {
    return *real_id == *expected_id;
}

int compareClassCourseID(const char* real_course_id, const char* expected_course_id) {
    return *real_course_id == *expected_course_id;
}

int compareClassTeacherID(const int* real_teacher_id, const int* expected_teacher_id) {
    return *real_teacher_id == *expected_teacher_id;
}

int compareClass(const LopHoc* cls1, const LopHoc* cls2) {
    if(!cls1 || !cls2) return 0;
    return cls1->id == cls2->id && cls1->courseID == cls2->courseID && cls1->teacherID == cls2->teacherID;
}

void printClass(const LopHoc* cls) {
    if(!cls) {
        return;
    }
    printf("ID: %d\n", cls->id);
    printf("ID khoa hoc: %d\n", cls->courseID);
    if(cls->metadata.hasTeacher) printf("ID giang vien: %d\n", cls->teacherID);
    else puts("ID giang vien: Lop hoc chua co giang vien!");
}

LopHoc* createClass() {
    LopHoc* cls = (LopHoc*) calloc(1, sizeof(LopHoc));
    if(!cls) return NULL;
    return cls;
}

int freeClass(LopHoc* cls) {
    if(!cls) return 0;
    free(cls);
    cls = NULL;
    return 1;
}

void printAllClassCallback(const LopHoc* cls) {
    printClass(cls);
    puts("");
}

void printAllClass(const LinkedList* list) {
    int listState = isListEmpty(list);
    if(listState || listState == -1) return;

    puts("===== DANH SACH LOP HOC =====\n");
    iterList(list, (listCallback) printAllClassCallback, NULL);
}

LopHoc** findClass(const LinkedList* list, int* resultLength, int useSearchOption, int maxResultLength) {
    int listState = isListEmpty(list);
    if (listState || listState == -1) return NULL;

    int mod_options[RELEVANT_FIELD_AMOUNT];
    FieldCondition conds[RELEVANT_FIELD_AMOUNT]; 
    int i = 1;
    if(useSearchOption) i = getModOptions(mod_options);
    else {
        for(int j = 0; j < RELEVANT_FIELD_AMOUNT; j++) {
            mod_options[j] = 1;
        }
    }

    if (i) {
        int cond_check = -1;
        int cond_count = 0;
        
        int id_adr;
        int id_kh;
        int id_gv;
        
        if (mod_options[++cond_check]) {
            printf("Nhap ID: ");
            saferScanf("%d", &id_adr);

            conds[cond_count].filterCond = (filterCondition) compareClassID;
            conds[cond_count].offset = offsetof(LopHoc, id);
            conds[cond_count].expected_value = &id_adr;

            cond_count++;
        }
        if (mod_options[++cond_check]) {
            printf("Nhap ID khoa hoc: ");
            saferScanf("%d", &id_kh);

            conds[cond_count].filterCond = (filterCondition) compareClassCourseID;
            conds[cond_count].offset = offsetof(LopHoc, courseID);
            conds[cond_count].expected_value = &id_kh;

            cond_count++;
        }
        if(mod_options[++cond_check]) {
            printf("Nhap ID giang vien: ");
            saferScanf("%d", &id_gv);

            conds[cond_count].filterCond = (filterCondition) compareClassTeacherID;
            conds[cond_count].offset = offsetof(LopHoc, teacherID);
            conds[cond_count].expected_value = &id_gv;
            cond_count++;
        }

        return (LopHoc**) filterList(list, conds, cond_count, resultLength, maxResultLength);
    }
    return NULL;
}

int enterClassInfo(LopHoc* cls, int useSearchOption) {
    if(!cls) return 0;

    int mod_options[RELEVANT_FIELD_AMOUNT];
    int i = 1;
    if(useSearchOption) i = getModOptions(mod_options);
    else {
        for(int j = 0; j < RELEVANT_FIELD_AMOUNT; j++) {
            mod_options[j] = 1;
        }
    }

    if (i) {
        int cond_count = -1;
        if (mod_options[++cond_count]) {
            printf("Nhap id moi: ");
            saferScanf("%d", &(cls->id));
        }
        if (mod_options[++cond_count]) {
            printf("Nhap id khoa hoc moi: ");
            saferScanf("%d", &(cls->courseID));
        }
        if(mod_options[++cond_count]) {
            printf("Nhap id giang vien moi: ");
            saferScanf("%d", &(cls->teacherID));
        }

        return 1;
    }
    return 0;
}

int countStudent(const LopHoc* cls) {
    if(!cls) return 0;

    return cls->metadata.studentCount;
}

int isClassFull(const LopHoc* cls) {
    if(!cls) return 0;
    return cls->metadata.studentCount == MAX_STUDENT;
}

int isClassEmpty(const LopHoc* cls) {
    if(!cls) return 0;
    return cls->metadata.studentCount == 0;
}

int isClassAvailable(const LopHoc* cls) {
    if(!cls) return 0;
    return cls->metadata.studentCount < MAX_STUDENT;
}

int isStudentInClass(const LopHoc* cls, const HocVien* hv) {
    if(!cls || !hv) {
        return 0;
    }
    for(int i = 0; i < cls->metadata.studentCount; i++) {
        if(cls->studentIDList[i] == getStudentID(hv)) return 1;
    }
    return 0;
}

int assignStudentToClass(LopHoc* cls, const HocVien* hv) {
    if(!cls || !hv) return 0;
    else if(isClassFull(cls)) return 0;

    cls->studentIDList[cls->metadata.studentCount++] = getStudentID(hv);
    return 1;
}

int removeStudentFromClass(LopHoc* cls, const HocVien* hv) {
    if(!cls || !hv) return 0;
    else if(isClassEmpty(cls)) return 0;

    int idIdx = -1;
    for(int i = 0; i < cls->metadata.studentCount; i++) {
        if(cls->studentIDList[i] == getStudentID(hv)) {
            idIdx = i;
            break;
        }
    }
    if(idIdx == -1) return 0;

    for(int i = idIdx; i < cls->metadata.studentCount - 1; i++) {
        cls->studentIDList[i] = cls->studentIDList[i + 1];
    }

    cls->metadata.studentCount--;

    return 1;
}

int removeTeacherFromClass(LopHoc* cls) {
    if(!cls) return 0;

    cls->metadata.hasTeacher = 0;
    return 1;
}

int isTeacherInClass(const LopHoc* cls, const GiangVien* gv) {
    if(!cls || !gv) return 0;
    return cls->metadata.hasTeacher && cls->teacherID == getTeacherID(gv);
}

int isClassHasTeacher(const LopHoc* cls) {
    if(!cls) return 0;
    return cls->metadata.hasTeacher;
}

int assignTeacherToClass(LopHoc* cls, const GiangVien* gv) {
    if(!cls || !gv) return 0;

    cls->teacherID = getTeacherID(gv);
    cls->metadata.hasTeacher = 1;
    return 1;
}

cJSON* lopHocToJSON(const LopHoc* lop) {
    if (!lop) return NULL;

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "id", lop->id);
    cJSON_AddNumberToObject(root, "courseID", lop->courseID);
    cJSON_AddNumberToObject(root, "teacherID", lop->teacherID);

    // studentIDList as JSON array
    cJSON* studentArray = cJSON_CreateIntArray(lop->studentIDList, lop->metadata.studentCount);
    cJSON_AddItemToObject(root, "studentIDList", studentArray);

    // metadata as JSON object
    cJSON* meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "hasTeacher", lop->metadata.hasTeacher);
    cJSON_AddNumberToObject(meta, "hasCourse", lop->metadata.hasCourse);
    cJSON_AddNumberToObject(meta, "studentCount", lop->metadata.studentCount);
    cJSON_AddItemToObject(root, "metadata", meta);

    return root;
}

LopHoc* parseLopHocFromObject(cJSON* obj) {
    if (!obj) return NULL;

    LopHoc* lop = malloc(sizeof(LopHoc));
    if (!lop) {
        perror("malloc failed");
        return NULL;
    }
    memset(lop, 0, sizeof(LopHoc));

    cJSON* id = cJSON_GetObjectItem(obj, "id");
    cJSON* courseID = cJSON_GetObjectItem(obj, "courseID");
    cJSON* teacherID = cJSON_GetObjectItem(obj, "teacherID");
    cJSON* studentArray = cJSON_GetObjectItem(obj, "studentIDList");
    cJSON* meta = cJSON_GetObjectItem(obj, "metadata");

    if (cJSON_IsNumber(id)) lop->id = id->valueint;
    if (cJSON_IsNumber(courseID)) lop->courseID = courseID->valueint;
    if (cJSON_IsNumber(teacherID)) lop->teacherID = teacherID->valueint;

    // studentIDList
    if (cJSON_IsArray(studentArray)) {
        int count = cJSON_GetArraySize(studentArray);
        for (int i = 0; i < count && i < MAX_STUDENT; i++) {
            cJSON* item = cJSON_GetArrayItem(studentArray, i);
            if (cJSON_IsNumber(item)) {
                lop->studentIDList[i] = item->valueint;
            }
        }
        lop->metadata.studentCount = count; // update from array
    }

    // metadata
    if (meta) {
        cJSON* hasTeacher = cJSON_GetObjectItem(meta, "hasTeacher");
        cJSON* hasCourse = cJSON_GetObjectItem(meta, "hasCourse");
        cJSON* studentCount = cJSON_GetObjectItem(meta, "studentCount");
        if (cJSON_IsNumber(hasTeacher)) lop->metadata.hasTeacher = hasTeacher->valueint;
        if (cJSON_IsNumber(hasCourse)) lop->metadata.hasCourse = hasCourse->valueint;
        if (cJSON_IsNumber(studentCount)) lop->metadata.studentCount = studentCount->valueint;
    }

    return lop;
}
