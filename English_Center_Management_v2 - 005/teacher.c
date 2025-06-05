#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "teacher.h"
#include "linked_list.h"
#include "general.h"
#include "./lib/cJSON/cJSON.h"

#define RELEVANT_FIELD_AMOUNT 3

/*
Problems:
    - Delete all teachers with eduLvl KL: functional. However, deleting all teachers with name 'a' after that deletes all teachers except Nguyen Duc Minh? V
    - Delete everyone with name 'a' gives a crash. The same goes for multi-deleting with id. The same does not go for deleting with edu lvl, however.
    Deleting with more than 1 bound works fine

    - The crashing issue can now be described as: after multi-deletion in accordance to a single condition, trying the same thing again will lead to crash.

    - Step to reproduce: delete all teachers with KL eduLvl and then delete all with the name 'a'. Suprisingly, using strcmp instead of strstr in the teacher name comparision 
    will solve this issue. 
    */

typedef struct GiangVien  {
    int id;
    char ten[MAX_NAME_LENGTH_TEACHER + 1];
    char trinhDoHocVan[MAX_EDUCATION_LEVEL_LENGTH + 1];
} GiangVien;

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
        printf("Su dung: Trinh do hoc van? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    return 1;
}

int compareTeacherID(const int* real_id, const int* expected_id) {
    return *real_id == *expected_id;
}

int compareTeacherName(const char* real_name, const char* expected_name) {
    return !strcmp(real_name, expected_name);
    // return strstr(real_name, expected_name) ? 1 : 0;
}

int compareTeacherEduLevel(const char* real_level, const char* expected_level) {
    return !strcmp(real_level, expected_level); // kiem tra xem 2 string co hoan toan nhu nhau khong. khi do, strcmp() -> 0
}

int compareTeacher(const GiangVien* gv1, const GiangVien* gv2) {
    if(!gv1 || !gv2) return 0;
    return gv1->id == gv2->id && !strcmp(gv1->ten, gv2->ten) && !strcmp(gv1->trinhDoHocVan, gv2->trinhDoHocVan);
}

void printTeacher(const GiangVien* gv) {
    if(!gv) {
        return;
    }
    printf("Ten giang vien: %s\n", gv->ten);
    printf("ID: %d\n", gv->id);
    printf("Trinh do hoc van: %s\n", gv->trinhDoHocVan);
}

GiangVien* createTeacher() {
    GiangVien* gv = (GiangVien*) calloc(1, sizeof(GiangVien));
    if(!gv) return NULL;
    return gv;
}

int freeTeacher(GiangVien* gv) {
    if(!gv) return 0;
    free(gv);
    gv = NULL;
    return 1;
}

void printAllTeacherCallback(const GiangVien* gv) {
    printTeacher(gv);
    puts("");
}

void printAllTeacher(const LinkedList* list) {
    int listState = isListEmpty(list);
    if(listState || listState == -1) return;

    puts("===== DANH SACH GIANG VIEN =====\n");
    iterList(list, (listCallback) printAllTeacherCallback, NULL);
}

GiangVien** findTeacher(const LinkedList* list, int* resultLength, int useSearchOption, int maxResultLength) {
    int listState = isListEmpty(list);
    if (listState || listState == -1) {
        printf("Failed pre-cond: %d - %d\n", listState == 0, listState == -1);
        return NULL;
    }

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
        
        char name[MAX_NAME_LENGTH_TEACHER + 1];
        int id_adr;
        char eduLvl[MAX_EDUCATION_LEVEL_LENGTH + 1];

        int cond_check = -1;
        int cond_count = 0;
        if (mod_options[++cond_check]) {
            printf("Nhap ten: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';

            conds[cond_count].filterCond = (filterCondition) compareTeacherName;
            conds[cond_count].offset = offsetof(GiangVien, ten);
            conds[cond_count].expected_value = name;

            cond_count++;
        }
        if (mod_options[++cond_check]) {
            printf("Nhap id: ");
            saferScanf("%d", &id_adr);

            conds[cond_count].filterCond = (filterCondition) compareTeacherID;
            conds[cond_count].offset = offsetof(GiangVien, id);
            conds[cond_count].expected_value = &id_adr;

            cond_count++;
        }
        if(mod_options[++cond_check]) {
            printf("Nhap trinh do hoc van: ");
            fgets(eduLvl, sizeof(eduLvl), stdin);
            eduLvl[strcspn(eduLvl, "\n")] = '\0';

            conds[cond_count].filterCond = (filterCondition) compareTeacherEduLevel;
            conds[cond_count].offset = offsetof(GiangVien, trinhDoHocVan);
            conds[cond_count].expected_value = eduLvl;
            cond_count++;
        }

        return (GiangVien**) filterList(list, conds, cond_count, resultLength, maxResultLength);
    }
    return NULL;
}

int enterTeacherInfo(GiangVien* gv, int useSearchOption) {
    if(!gv) return 0;

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
            char name[MAX_NAME_LENGTH_TEACHER + 1];
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            strcpy(gv->ten, name);
        }
        if (mod_options[++cond_count]) {
            printf("Nhap id moi: ");
            saferScanf("%d", &(gv->id));
        }
        if(mod_options[++cond_count]) {
            printf("Nhap trinh do hoc van moi: ");
            char eduLvl[MAX_EDUCATION_LEVEL_LENGTH + 1];
            fgets(eduLvl, sizeof(eduLvl), stdin);
            eduLvl[strcspn(eduLvl, "\n")] = '\0';
            strcpy(gv->trinhDoHocVan, eduLvl);
        }

        return 1;
    }
    return 0;
}

int getTeacherID(const GiangVien* gv) {
    if(!gv) return 0;
    return gv->id;
}

cJSON* giangVienToJSON(const GiangVien* gv) {
    if (!gv) return NULL;

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "id", gv->id);
    cJSON_AddStringToObject(root, "ten", gv->ten);
    cJSON_AddStringToObject(root, "trinhDoHocVan", gv->trinhDoHocVan);

    return root;
}

GiangVien* parseGiangVienFromObject(cJSON* obj) {
    if (!obj) return NULL;

    GiangVien* gv = malloc(sizeof(GiangVien));
    if (!gv) {
        perror("malloc failed");
        return NULL;
    }
    memset(gv, 0, sizeof(GiangVien));

    cJSON* id = cJSON_GetObjectItem(obj, "id");
    cJSON* ten = cJSON_GetObjectItem(obj, "ten");
    cJSON* trinhDoHocVan = cJSON_GetObjectItem(obj, "trinhDoHocVan");

    if (cJSON_IsNumber(id)) gv->id = id->valueint;
    if (cJSON_IsString(ten)) strncpy(gv->ten, ten->valuestring, MAX_NAME_LENGTH_TEACHER);
    if (cJSON_IsString(trinhDoHocVan)) strncpy(gv->trinhDoHocVan, trinhDoHocVan->valuestring, MAX_EDUCATION_LEVEL_LENGTH);

    // Ensure null-termination
    gv->ten[MAX_NAME_LENGTH_TEACHER] = '\0';
    gv->trinhDoHocVan[MAX_EDUCATION_LEVEL_LENGTH] = '\0';

    return gv;
}
