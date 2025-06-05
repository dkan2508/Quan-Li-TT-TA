#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "course.h"
#include "linked_list.h"
#include "general.h"
#include "./lib/cJSON/cJSON.h"

#define FIELD_AMOUNT 3

typedef struct KhoaHoc  {
    int id;
    char ten[MAX_NAME_LENGTH_COURSE + 1];
    float hocPhi;
} KhoaHoc;

int getModOptions(int* optionArr) {
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
        printf("Su dung: Hoc phi? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    return 1;
}

int compareCourseID(const int* real_id, const int* expected_id) {
    return *real_id == *expected_id;
}

int compareCourseName(const char* real_name, const char* expected_name) {
    return !strcmp(real_name, expected_name);
}

int compareCourseTuition(const float* real_tuition, const float* expected_tuition) {
    return *real_tuition == *expected_tuition;
}

int compareCourse(const KhoaHoc* cr1, const KhoaHoc* cr2) {
    if(!cr1 || !cr2) return 0;
    return cr1->id == cr2->id && !strcmp(cr1->ten, cr2->ten) && cr1->hocPhi == cr2->hocPhi;
}

void printCourse(const KhoaHoc* cr) {
    if(!cr) {
        return;
    }
    printf("Ten khoa hoc: %s\n", cr->ten);
    printf("ID: %d\n", cr->id);
    printf("Hoc phi: %.2f\n", cr->hocPhi);
}

KhoaHoc* createCourse() {
    KhoaHoc* cr = (KhoaHoc*) calloc(1, sizeof(KhoaHoc));
    if(!cr) return NULL;
    return cr;
}

int freeCourse(KhoaHoc* cr) {
    if(!cr) return 0;
    free(cr);
    cr = NULL;
    return 1;
}

void printAllCourseCallback(const KhoaHoc* kh) {
    printCourse(kh);
    puts("");
}

void printAllCourse(const LinkedList* list) {
    int listState = isListEmpty(list);
    if(listState || listState == -1) return;

    puts("===== DANH SACH KHOA HOC =====\n");
    iterList(list, (listCallback) printAllCourseCallback, NULL);
}

KhoaHoc** findCourse(const LinkedList* list, int* resultLength, int useSearchOption, int maxResultLength) {
    int listState = isListEmpty(list);
    if (listState || listState == -1) return NULL;

    int mod_options[FIELD_AMOUNT];
    FieldCondition conds[FIELD_AMOUNT]; 
    int i = 1;
    if(useSearchOption) i = getModOptions(mod_options);
    else {
        for(int j = 0; j < FIELD_AMOUNT; j++) {
            mod_options[j] = 1;
        }
    }

    if (i) {
        int cond_check = -1;
        int cond_count = 0;
        
        char name[MAX_NAME_LENGTH_COURSE + 1];
        int id_adr;
        float tuition;
        if (mod_options[++cond_check]) {
            printf("Nhap ten: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';

            conds[cond_count].filterCond = (filterCondition) compareCourseName;
            conds[cond_count].offset = offsetof(KhoaHoc, ten);
            conds[cond_count].expected_value = name;

            cond_count++;
        }
        if (mod_options[++cond_check]) {
            printf("Nhap id: ");
            saferScanf("%d", &id_adr);

            conds[cond_count].filterCond = (filterCondition) compareCourseID;
            conds[cond_count].offset = offsetof(KhoaHoc, id);
            conds[cond_count].expected_value = &id_adr;

            cond_count++;
        }
        if(mod_options[++cond_check]) {
            printf("Nhap hoc phi: ");
            saferScanf("%f", &tuition);

            conds[cond_count].filterCond = (filterCondition) compareCourseTuition;
            conds[cond_count].offset = offsetof(KhoaHoc, hocPhi);
            conds[cond_count].expected_value = &tuition;
            cond_count++;
        }

        return (KhoaHoc**) filterList(list, conds, cond_count, resultLength, maxResultLength);
    }
    return NULL;
}

int enterCourseInfo(KhoaHoc* cr, int useSearchOption) {
    if(!cr) return 0;

    int mod_options[FIELD_AMOUNT];
    int i = 1;
    if(useSearchOption) i = getModOptions(mod_options);
    else {
        for(int j = 0; j < FIELD_AMOUNT; j++) {
            mod_options[j] = 1;
        }
    }
 
    if (i) {
        int cond_count = -1;
        if (mod_options[++cond_count]) {
            printf("Nhap ten moi: ");
            char name[MAX_NAME_LENGTH_COURSE + 1];
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            strcpy(cr->ten, name);
        }
        if (mod_options[++cond_count]) {
            printf("Nhap id moi: ");
            saferScanf("%d", &(cr->id));
        }
        if(mod_options[++cond_count]) {
            printf("Nhap hoc phi moi: ");
            saferScanf("%f", &(cr->hocPhi));
        }

        return 1;
    }
    return 0;
}

cJSON* khoaHocToJSON(const KhoaHoc* kh) {
    if (!kh) return NULL;

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "id", kh->id);
    cJSON_AddStringToObject(root, "ten", kh->ten);
    cJSON_AddNumberToObject(root, "hocPhi", kh->hocPhi);

    return root;
}

KhoaHoc* parseKhoaHocFromObject(cJSON* obj) {
    if (!obj) return NULL;

    KhoaHoc* kh = malloc(sizeof(KhoaHoc));
    if (!kh) {
        perror("malloc failed");
        return NULL;
    }
    memset(kh, 0, sizeof(KhoaHoc));

    cJSON* id = cJSON_GetObjectItem(obj, "id");
    cJSON* ten = cJSON_GetObjectItem(obj, "ten");
    cJSON* hocPhi = cJSON_GetObjectItem(obj, "hocPhi");

    if (cJSON_IsNumber(id)) kh->id = id->valueint;
    if (cJSON_IsString(ten)) strncpy(kh->ten, ten->valuestring, MAX_NAME_LENGTH_COURSE);
    if (cJSON_IsNumber(hocPhi)) kh->hocPhi = (float) hocPhi->valuedouble;

    // Ensure null-termination
    kh->ten[MAX_NAME_LENGTH_COURSE] = '\0';

    return kh;
}
