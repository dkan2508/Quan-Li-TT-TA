#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "student.h"
#include "linked_list.h"
#include "general.h"
#include "./lib/cJSON/cJSON.h"

#define RELEVANT_FIELD_AMOUNT 3  // So truong cua struct se duoc su dung de hoi/truy van thong tin. Khong phai tong so tat ca cac truong cua struct

typedef struct HocVien {
    int id;
    char ten[MAX_NAME_LENGTH_STUDENT + 1];
    int tuoi;
} HocVien;

/*
Chỉnh sửa lại sao cho các lựa chọn về trường không phải tuân theo một thứ tự nhất định
*/

// Cac ham khong xuat hien trong student.h
static int getModOptions(int* optionArr) {
    if(!optionArr) return 0;

    int result;
    int idx = 0;
    puts("===== Lua chon su dung truong thong tin (nhap 1 neu chon, 0 neu khong) ======");
    do {
        printf("Su dung: Ten? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    do {
        printf("Su dung: ID? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    do {
        printf("Su dung: Tuoi? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    return 1;
}

int compareStudentID(const int* real_id, const int* expected_id) {
    return *real_id == *expected_id;
}

int compareStudentName(const char* real_name, const char* expected_name) {
    return !strcmp(real_name, expected_name);
}

int compareStudentAge(const int* real_age, const int* expected_age) {
    return *real_age == *expected_age;
}

int compareStudent(const HocVien* hv1, const HocVien* hv2) {
    if(!hv1 || !hv2) return 0;
    return hv1->id == hv2->id && !strcmp(hv1->ten, hv2->ten) && hv1->tuoi == hv2->tuoi;
}

void printStudent(const HocVien* hv) {
    if(!hv) {
        return;
    }
    printf("Ten hoc sinh: %s\n", hv->ten);
    printf("ID: %d\n", hv->id);
    printf("Tuoi hoc sinh: %d\n", hv->tuoi);
}

HocVien* createStudent() {
    HocVien* hv = (HocVien*) calloc(1, sizeof(HocVien));
    if(!hv) return NULL;
    return hv;
}

int freeStudent(HocVien* hv) {
    if(!hv) return 0;
    free(hv);
    hv = NULL;
    return 1;
}

void printAllStudentCallback(const HocVien* hv) {
    printStudent(hv);
    puts("");
}

void printAllStudent(const LinkedList* list) {
    int listState = isListEmpty(list);
    if(listState || listState == -1) return;

    puts("===== DANH SACH HOC VIEN =====\n");
    iterList(list, (listCallback) printAllStudentCallback, NULL);
}

HocVien** findStudent(const LinkedList* list, int* resultLength, int useSearchOption, int maxResultLength) {
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
        
        char name[MAX_NAME_LENGTH_STUDENT + 1];
        int id_adr;
        int age;
        
        if (mod_options[++cond_check]) {
            printf("Nhap ten: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';

            conds[cond_count].filterCond = (filterCondition) compareStudentName;
            conds[cond_count].offset = offsetof(HocVien, ten);
            conds[cond_count].expected_value = name;

            cond_count++;
        }
        if (mod_options[++cond_check]) {
            printf("Nhap id: ");
            saferScanf("%d", &id_adr);

            conds[cond_count].filterCond = (filterCondition) compareStudentID;
            conds[cond_count].offset = offsetof(HocVien, id);
            conds[cond_count].expected_value = &id_adr;

            cond_count++;
        }
        if(mod_options[++cond_check]) {
            printf("Nhap tuoi: ");
            saferScanf("%d", &age);

            conds[cond_count].filterCond = (filterCondition) compareStudentAge;
            conds[cond_count].offset = offsetof(HocVien, tuoi);
            conds[cond_count].expected_value = &age;
            cond_count++;
        }

        return (HocVien**) filterList(list, conds, cond_count, resultLength, maxResultLength);
    }
    return NULL;
}

int enterStudentInfo(HocVien* hv, int useSearchOption) {
    if(!hv) return 0;

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
            printf("Nhap ten moi: ");
            char name[MAX_NAME_LENGTH_STUDENT + 1];
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            strcpy(hv->ten, name);
        }
        if (mod_options[++cond_count]) {
            printf("Nhap id moi: ");
            saferScanf("%d", &(hv->id));
        }
        if(mod_options[++cond_count]) {
            printf("Nhap tuoi moi: ");
            saferScanf("%d", &(hv->tuoi));
        }

        return 1;
    }
    return 0;
}

int getStudentID(const HocVien* hv) {
    if(!hv) return 0;
    return hv->id;
}

cJSON* hocVienToJSON(const HocVien* hv) {
    if (!hv) return NULL;

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "id", hv->id);
    cJSON_AddStringToObject(root, "ten", hv->ten);
    cJSON_AddNumberToObject(root, "tuoi", hv->tuoi);

    return root;
}

HocVien* parseHocVienFromObject(cJSON* obj) {
    if (!obj) return NULL;

    HocVien* hv = malloc(sizeof(HocVien));
    if (!hv) {
        perror("malloc failed");
        return NULL;
    }
    memset(hv, 0, sizeof(HocVien));

    cJSON* id = cJSON_GetObjectItem(obj, "id");
    cJSON* ten = cJSON_GetObjectItem(obj, "ten");
    cJSON* tuoi = cJSON_GetObjectItem(obj, "tuoi");

    if (cJSON_IsNumber(id)) hv->id = id->valueint;
    if (cJSON_IsString(ten) && ten->valuestring) strncpy(hv->ten, ten->valuestring, MAX_NAME_LENGTH_STUDENT);
    if (cJSON_IsNumber(tuoi)) hv->tuoi = tuoi->valueint;

    return hv;
}