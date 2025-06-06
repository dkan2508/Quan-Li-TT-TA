#pragma once

#include "./lib/cJSON/cJSON.h"
#include "linked_list.h"

#define MAX_FILE_PATH_LENGTH 100

typedef enum SUBJECT_FILE_TYPE {
    STUDENT, TEACHER, COURSE, CLASS, BILL
} SUBJECT_FILE_TYPE;

int resetManagerFile();
int docFileQuanLi(SUBJECT_FILE_TYPE option, char path[]);
int capNhatFileQuanLi(SUBJECT_FILE_TYPE option, char newPath[]);

int deleteDataFile(SUBJECT_FILE_TYPE option);
int createDataFile(SUBJECT_FILE_TYPE option, char newPath[]);
int moveFile(SUBJECT_FILE_TYPE option, char newPath[]);
int showCurrentPath(SUBJECT_FILE_TYPE option);
int chooseFile(SUBJECT_FILE_TYPE option, char path[]);

// Cac ham ho tro lam viec voi thu vien cJSON
typedef cJSON* (*jsonSerializer)(const void* obj);
typedef void* (*jsonParser)(const cJSON* obj);

int writeJSONFile(cJSON* jsonArr, char path[]);
cJSON* arrToJSONArr(const void** arr, int size, jsonSerializer serializer);
cJSON* linkedListToJSONArr(const LinkedList* list, jsonSerializer serializer);

cJSON* parseJSONArrToObj(char path[], int* size);
void** cJSONObjToArr(cJSON* root, int size, jsonParser parser);
LinkedList* cJSONObjToLinkedList(cJSON* root, int size, jsonParser parser);

// Cac ham lam viec voi chuc nang tu dong luu file, tu dong load file cua chuong trinh
int isAutoLoaderOn();
int onOffAutoLoader(int option);

int isAutoSaverOn();
int onOffAutoSaver(int option);

// Ham khoi tao mot linked list
LinkedList* loadLinkedList(SUBJECT_FILE_TYPE sbj_type, jsonParser parser, int useFileLoader);

int saveLinkedListToJSON(SUBJECT_FILE_TYPE sbj_type, const LinkedList* list, jsonSerializer serializer);

int resetAutoFile();

int isManagerFileExist();

int isAutoFileExist();