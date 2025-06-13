#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "class.h"
#include "linked_list.h"
#include "general.h"
#include "./lib/cJSON/cJSON.h"

#define RELEVANT_FIELD_AMOUNT 3

typedef struct ClassMetadata {
    int studentCount;
} ClassMetadata;

typedef struct LopHoc {
    int id;
    int courseID;
    int teacherID;
    int studentIDList[MAX_STUDENT];

    ClassMetadata metadata;
} LopHoc;

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
    printf("ID giang vien: %d\n", cls->teacherID);
    printf("So hoc sinh trong lop: %d\n", cls->metadata.studentCount);
}

void printStudentList(const LopHoc* cls) {
    if(!cls) return;

    printf("\nDanh sach hoc sinh cua lop %d:\n", cls->id);
    for(int i = 1; i <= cls->metadata.studentCount; i++) {
        printf("%d. Hoc vien: %d\n", i, cls->studentIDList[i - 1]);
    }
    printf("Tong so luong: %d\n\n", cls->metadata.studentCount);
}

LopHoc* createClass() {
    LopHoc* cls = (LopHoc*) calloc(1, sizeof(LopHoc));
    if(!cls) return NULL;
    return cls;
}

LopHoc* copyClass(LopHoc* destination, const LopHoc* source) {
    if(!destination || !source) return NULL;

    destination->courseID = source->courseID;
    destination->id = source->id;
    destination->metadata = source->metadata;
    memcpy(destination->studentIDList, source->studentIDList, sizeof(int) * MAX_STUDENT);
    destination->teacherID = source->teacherID;

    return destination;
}

typedef struct compareIDwithClassPacket {
    int id;
    int* result;
} compareIDwithClassPacket;

int compareIDwithClassCallback(const LopHoc* bl, compareIDwithClassPacket* packet) {
    if(!bl || !packet) return STOP;

    if(bl->id == packet->id) {
        *(packet->result) = 1;
        return STOP;
    }
    return CONTINUE;
}

int isClassExist(int classID, const LinkedList* courseList) {
    int listState = isListEmpty(courseList);
    if (listState || listState == -1) return 0;

    int result = 0;
    compareIDwithClassPacket packet = {.id = classID, .result = &result};
    iterList(courseList, (listCallback) compareIDwithClassCallback, (void*) &packet);

    return result;
}

int isClassSubjectExist(const LopHoc* cls, const ClassSubjectValidationContext ctx) {
    if(!cls) return 0;

    if(!ctx.courseContext.checker(cls->courseID, ctx.courseContext.list)) {
        printf("Khong ton tai khoa hoc voi id: %d\n", cls->courseID);
        return 0;
    }
    if(!ctx.teacherContext.checker(cls->teacherID, ctx.teacherContext.list)) {
        printf("Khong ton tai giang vien voi id: %d\n", cls->teacherID);
        return 0;
    }

    return 1;
}

int freeClass(LopHoc* cls) {
    if(!cls) return 0;
    free(cls);
    cls = NULL;
    return 1;
}

int freeClassCallback(LopHoc* cls, void* placeholder) {
    if(!cls) return CONTINUE;
    free(cls);
    cls = NULL;
    return CONTINUE;
}

int printAllClassCallback(const LopHoc* cls) {
    printClass(cls);
    puts("");
    return CONTINUE;
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

int getClassID(const LopHoc* cls) {
    if(!cls) return 0;
    return cls->id;
}

cJSON* lopHocToJSON(const LopHoc* lop) {
    if (!lop) return NULL;

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "id", lop->id);
    cJSON_AddNumberToObject(root, "courseID", lop->courseID);
    cJSON_AddNumberToObject(root, "teacherID", lop->teacherID);

    cJSON* studentArray = cJSON_CreateIntArray(lop->studentIDList, lop->metadata.studentCount);
    cJSON_AddItemToObject(root, "studentIDList", studentArray);

    cJSON* meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "studentCount", lop->metadata.studentCount);
    cJSON_AddItemToObject(root, "metadata", meta);

    return root;
}

LopHoc* parseLopHocFromObject(cJSON* obj) {
    if (!obj) return NULL;

    LopHoc* lop = malloc(sizeof(LopHoc));
    if (!lop) {
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

    if (cJSON_IsArray(studentArray)) {
        int count = cJSON_GetArraySize(studentArray);
        for (int i = 0; i < count && i < MAX_STUDENT; i++) {
            cJSON* item = cJSON_GetArrayItem(studentArray, i);
            if (cJSON_IsNumber(item)) {
                lop->studentIDList[i] = item->valueint;
            }
        }
        lop->metadata.studentCount = count;
    }

    if (meta) {
        cJSON* studentCount = cJSON_GetObjectItem(meta, "studentCount");
        if (cJSON_IsNumber(studentCount)) lop->metadata.studentCount = studentCount->valueint;
    }

    return lop;
}
