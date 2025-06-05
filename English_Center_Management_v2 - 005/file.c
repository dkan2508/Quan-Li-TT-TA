#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "file.h"
#include "./lib/cJSON/cJSON.h"
#include "linked_list.h"

#define MANAGING_FILE "./file/manage.txt"
#define MANAGING_DIR "./file"
#define AUTO_FILE "./file/auto_option.txt"
#define SUBJECT_AMOUNT 5
#define AUTO_FIELD 2

int resetManagerFile() {
    mkdir(MANAGING_DIR);
    FILE* fptr = fopen(MANAGING_FILE, "w");
    if(fptr) {
        for(int i = 0; i < SUBJECT_AMOUNT; i++) fprintf(fptr, "<placeholder>\n");
        fclose(fptr);
        return 1;
    }
    return 0;
}

int docFileQuanLi(SUBJECT_FILE_TYPE option, char path[]) {
    if (!(option >= STUDENT && option <= BILL)) {
        return 0;
    }
    FILE* fptr = fopen(MANAGING_FILE, "r");
    if (!fptr) {
        return 0;
    }
    for(int i = 0; i <= option; i++) {
        if(!fgets(path, MAX_FILE_PATH_LENGTH, fptr)) {
            fclose(fptr);
            return 0;
        }
    }
    path[strcspn(path, "\n")] = '\0';
    fclose(fptr);
    return 1;
}

int capNhatFileQuanLi(SUBJECT_FILE_TYPE option, char newPath[]) {
    if (!(option >= STUDENT && option <= BILL)) {
        return 0;
    }

    if(strlen(newPath) != 0) {
        newPath[strcspn(newPath, "\n")] = '\0';     
    }

    char noiDungFile[SUBJECT_AMOUNT][MAX_FILE_PATH_LENGTH] = {""};
    FILE* fptr1 = fopen(MANAGING_FILE, "r");
    if (!fptr1) {
        return 0;
    }

    // Luu noi dung file ra mang tam thoi
    for (int i = 0; i < SUBJECT_AMOUNT; i++) {
        if (!fgets(noiDungFile[i], MAX_FILE_PATH_LENGTH, fptr1)) {
                fclose(fptr1);
                return 0;
        } else {
            // Xoa bo ky tu newline cua tat ca duong dan cu
            noiDungFile[i][strcspn(noiDungFile[i], "\n")] = '\0';   //MARK: STR_PRO
        }
    }
    fclose(fptr1);

    // Update noi dung cua duong dan moi vao mang tam thoi
    // Su dung MAX_FILE_PATH_LENGTH - 1 de dam bao cho cho ky tu '\0'
    strncpy(noiDungFile[option], newPath, MAX_FILE_PATH_LENGTH - 1); // Dieu nay chi thuong co anh huong khi duong dan moi dai hon MAX_FILE_PATH_LENGTH
    noiDungFile[option][MAX_FILE_PATH_LENGTH - 1] = '\0';      //MARK: STR_PRO

    // Mo file de viet lai noi dung moi
    FILE* fptr2 = fopen(MANAGING_FILE, "w");
    if (!fptr2) {
        return 0;
    }
    // Viet noi dung moi, them '\n' vao cuoi moi duong dan de dung dinh dang
    for (int i = 0; i < SUBJECT_AMOUNT; i++) {
        fprintf(fptr2, "%s\n", noiDungFile[i]);
    }
    fclose(fptr2);
    return 1;
}

int deleteDataFile(SUBJECT_FILE_TYPE option) {
    if (!(option >= STUDENT && option <= BILL)) {
        return 0;
    }

    char filePath[MAX_FILE_PATH_LENGTH];
    docFileQuanLi(option, filePath);
    int ketQuaXoa = remove(filePath);
    if(ketQuaXoa != 0) {
        return 0;
    }
    capNhatFileQuanLi(option, "");
    return 1;
}

int createDataFile(SUBJECT_FILE_TYPE option, char newPath[]) {
    if (!(option >= STUDENT && option <= BILL)) {
        return 0;
    }
    newPath[strcspn(newPath, "\n")] = '\0';

    FILE* fptr = fopen(newPath, "w");
    if(!fptr) {
        return 0;
    }
    
    fclose(fptr);
    capNhatFileQuanLi(option, newPath);
    return 1;
}

int copyFile(char srcPath[], char destPath[]) {
    FILE* srcFile = fopen(srcPath, "r");
    if (!srcFile) {
        return 0;
    }

    FILE* destFile = fopen(destPath, "w");
    if (!destFile) {
        fclose(srcFile);
        return 0;
    }

    int ch;
    while ((ch = fgetc(srcFile)) != EOF) {
        fputc(ch, destFile);
    }

    fclose(srcFile);
    fclose(destFile);
    return 1;
}

int moveFile(SUBJECT_FILE_TYPE option, char newPath[]) {
    if (!(option >= STUDENT && option <= BILL)) {
        return 0;
    }
    newPath[strcspn(newPath, "\n")] = '\0';

    char path[MAX_FILE_PATH_LENGTH];
    docFileQuanLi(option, path);

    int result = copyFile(path, newPath);
    return result;
}

int showCurrentPath(SUBJECT_FILE_TYPE option) {
    if (!(option >= STUDENT && option <= BILL)) {
        return 0;
    }
    char path[MAX_FILE_PATH_LENGTH];
    docFileQuanLi(option, path);
    printf("Duong dan hien tai: %s\n", path);
    return 1;
}

int chooseFile(SUBJECT_FILE_TYPE option, char path[]) {
    if (!(option >= STUDENT && option <= BILL)) {
        return 0;
    }
    path[strcspn(path, "\n")] = '\0';

    // Kiem tra xem dia chi co ton tai khong
    FILE* fptr = fopen(path, "r");
    if(!fptr) {
        return 0;
    }
    fclose(fptr);

    return capNhatFileQuanLi(option, path);
}

int writeJSONFile(cJSON* jsonArr, char path[]) {
    if(!jsonArr || !path) {
        return 0;
    }

    char* jsonString = cJSON_Print(jsonArr);

    int status = 1;
    FILE* f = fopen(path, "w");
    if (f) {
        fputs(jsonString, f);
        fclose(f);
    } else {
        status = 0;
    }

    free(jsonString);
    return status;
}

cJSON* arrToJSONArr(const void** arr, int size, jsonSerializer serializer) {
    if(!arr) return NULL;

    cJSON* array = cJSON_CreateArray();

    for (int i = 0; i < size; i++) {
        cJSON* obj = serializer(arr[i]);
        cJSON_AddItemToArray(array, obj);
    }

    return array;
}

typedef struct linkedListSerializationPacket {
    cJSON* array;
    jsonSerializer serializer;
} linkedListSerializationPacket;

void linkedListSerializationCallback(const void* data, linkedListSerializationPacket* packet) {
    if(!packet) return;
    cJSON* obj = packet->serializer(data);
    cJSON_AddItemToArray(packet->array, obj);
}

cJSON* linkedListToJSONArr(const LinkedList* list, jsonSerializer serializer) {
    if(!list) return NULL;

    cJSON* array = cJSON_CreateArray();
    linkedListSerializationPacket packet = {.array = array, .serializer = serializer};
    iterList(list, (listCallback) linkedListSerializationCallback, &packet);

    return array;
}

cJSON* parseJSONArrToObj(char path[], int* size) {
    FILE* f = fopen(path, "r");
    if (!f) {
        return NULL;
    }

    // Read file contents
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* jsonString = malloc(fsize + 1);
    if (!jsonString) {
        fclose(f);
        return NULL;
    }
    fread(jsonString, 1, fsize, f);
    jsonString[fsize] = '\0';
    fclose(f);

    // Parse JSON array
    cJSON* root = cJSON_Parse(jsonString);
    free(jsonString);

    if (!root || !cJSON_IsArray(root)) {
        fprintf(stderr, "Invalid JSON format!\n");
        cJSON_Delete(root);
        return NULL;
    }

    int count = cJSON_GetArraySize(root);
    if (size) *size = count;

    return root;
}

void** cJSONObjToArr(cJSON* root, int size, jsonParser parser) {
    if(!root || !parser || size <= 0) return NULL;

    void** result = calloc(size, sizeof(void*));
    if(!result) return NULL;

    for(int i = 0; i < size; i++) {
        cJSON* item = cJSON_GetArrayItem(root, i);
        result[i] = parser(item);
    }

    return result;
}

LinkedList* cJSONObjToLinkedList(cJSON* root, int size, jsonParser parser) {
    if(!root || !parser || size < 0) return NULL;

    LinkedList* result = createList();
    for(int i = 0; i < size; i++) {
        cJSON* item = cJSON_GetArrayItem(root, i);
        appendLinkedList(result, parser(item));
    }

    return result;
}

/*
Auto file format: 
1   // Auto-loader mode
1   // Auto-saver mode
*/

int resetAutoFile() {
    mkdir(MANAGING_DIR);
    FILE* fptr = fopen(AUTO_FILE, "w");
    if(!fptr) return 0;

    for(int i = 0; i < AUTO_FIELD; i++) {
        fprintf(fptr, "1\n");
    }
    fclose(fptr);
    return 1;
}

int readAutoModeLine(int line) {
    FILE* fptr = fopen(AUTO_FILE, "r");
    if (!fptr) return -1;

    int c, currentLine = 0, result = -1;

    while ((c = fgetc(fptr)) != EOF) {
        if (currentLine == line) {
            if (c == '0' || c == '1') result = c - '0';
            break;
        }
        if (c == '\n') currentLine++;
    }

    fclose(fptr);
    return result;
}

int isAutoLoaderOn() {
    return readAutoModeLine(0);
}

int isAutoSaverOn() {
    return readAutoModeLine(1);
}

/*
// Set auto mode on a given line (0-indexed)
int setAutoModeLine(int line, int option) {
    if (!(option == 1 || option == 0)) return 0;

    FILE* fptr = fopen(AUTO_FILE, "r");
    if (!fptr) return 0;

    fseek(fptr, 0, SEEK_END);
    long fsize = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    if (fsize <= 0) {
        fclose(fptr);
        return 0;
    }

    char* fcontent = (char*) malloc(fsize);
    if (!fcontent) {
        fclose(fptr);
        return 0;
    }

    fread(fcontent, 1, fsize, fptr);
    fclose(fptr);

    // Find the start index of the target line
    int currentLine = 0;
    long idx = 0;

    while (idx < fsize && currentLine < line) {
        if (fcontent[idx] == '\n') currentLine++;
        idx++;
    }
    printf("After line locating: %d - %d\n", idx, currentLine);

    // Replace the first non-newline character on the target line
    while (idx < fsize && (fcontent[idx] == '\n' || fcontent[idx] == '\r')) idx++;
    printf("After moving idx: %d - %c\n", idx, fcontent[idx]);
    if (idx < fsize) fcontent[idx] = option + '0';
    printf("%c\n", fcontent[idx]);

    fptr = fopen(AUTO_FILE, "w");
    if (!fptr) {
        free(fcontent);
        return 0;
    }

    fwrite(fcontent, 1, fsize, fptr);
    //fprintf(fptr, fcontent);
    fclose(fptr);
    free(fcontent);

    return 1;
}
*/
int setAutoModeLine(int line, int option) {
    if (!(option == 1 || option == 0)) return 0;

    // Load file content into memory
    FILE* fptr = fopen(AUTO_FILE, "r");
    char** lines = NULL;
    int lineCount = 0;

    if (fptr) {
        char buf[256];
        while (fgets(buf, sizeof(buf), fptr)) {
            // Remove trailing newlines
            size_t len = strlen(buf);
            while (len > 0 && (buf[len-1] == '\n' || buf[len-1] == '\r')) {
                buf[--len] = '\0';
            }

            // Save line
            lines = realloc(lines, sizeof(char*) * (lineCount + 1));
            lines[lineCount] = strdup(buf);
            lineCount++;
        }
        fclose(fptr);
    }

    // Ensure enough line
    while (lineCount <= line) {
        lines = realloc(lines, sizeof(char*) * (lineCount + 1));
        lines[lineCount] = strdup("0");
        lineCount++;
    }

    // Update line
    free(lines[line]);
    lines[line] = malloc(2);
    lines[line][0] = option + '0';
    lines[line][1] = '\0';

    // Write all lines back
    fptr = fopen(AUTO_FILE, "w");
    if (!fptr) {
        for (int i = 0; i < lineCount; i++) free(lines[i]);
        free(lines);
        return 0;
    }

    for (int i = 0; i < lineCount; i++) {
        fprintf(fptr, "%s\n", lines[i]);
        free(lines[i]);
    }
    free(lines);
    fclose(fptr);

    return 1;
}


int onOffAutoLoader(int option) {
    return setAutoModeLine(0, option);
}

int onOffAutoSaver(int option) {
    return setAutoModeLine(1, option);
}

int isManagerFileExist() {
    FILE* fptr = fopen(MANAGING_FILE, "r");
    if(!fptr) return 0;
    free(fptr);
    return 1;
}

int isAutoFileExist() {
    FILE* fptr = fopen(AUTO_FILE, "r");
    if(!fptr) return 0;
    free(fptr);
    return 1;
}

int isAutoFileExist();

LinkedList* loadLinkedList(SUBJECT_FILE_TYPE sbj_type, jsonParser parser, int useFileLoader) {
    if (!(sbj_type >= STUDENT && sbj_type <= BILL) || !parser) {
        return NULL;
    }
    if(!useFileLoader) return createList();

    char path[MAX_FILE_PATH_LENGTH];
    if(!docFileQuanLi(sbj_type, path)) return createList();

    int size = 0;
    cJSON* root = parseJSONArrToObj(path, &size);
    if(!root) return createList();

    LinkedList* list = cJSONObjToLinkedList(root, size, parser);
    if(!list) {
        list = createList();
    }

    cJSON_Delete(root);
    return list;
}

int saveLinkedListToJSON(SUBJECT_FILE_TYPE sbj_type, const LinkedList* list, jsonSerializer serializer) {
    if(!(sbj_type >= STUDENT && sbj_type <= BILL) || !list || !serializer) return 0;
    
    cJSON* jsonArr = linkedListToJSONArr(list, serializer);
    if(!jsonArr) {
        return 0;
    }

    char path[MAX_FILE_PATH_LENGTH];
    if(!docFileQuanLi(sbj_type, path)) {
        cJSON_Delete(jsonArr);
        return 0;
    }
    FILE* fptr = fopen(path, "w"); // Don sach file cu
    if(!fptr) {
        cJSON_Delete(jsonArr);
        return 0;
    }
    fclose(fptr);

    int result = writeJSONFile(jsonArr, path);
    cJSON_Delete(jsonArr);

    return result;
}
