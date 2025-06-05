#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

typedef struct ListNode {
    void* data;
    struct ListNode* next;
} ListNode;

typedef struct LinkedList {
    int size;
    ListNode* root;
} LinkedList;

ListNode* createNode() {
    ListNode* newNode = (ListNode*) malloc(sizeof(ListNode));
    if(!newNode) {
        return NULL;
    }
    newNode->data = NULL;
    newNode->next = NULL;
    return newNode;
}

LinkedList* createList() {
    LinkedList* newList = (LinkedList*) malloc(sizeof(LinkedList));
    if(!newList) {
        return NULL;
    }
    newList->root = NULL;
    newList->size = 0;
    return newList;
}

int isListEmpty(const LinkedList* list) {
    if(!list) return -1;
    else if(!list->root) return 1;
    return 0;
}

void destroyList(LinkedList* list) {
    int listState = isListEmpty(list);
    if(listState == -1 || listState) {
        return;
    }
    ListNode* temp = list->root;
    ListNode* nodeToDelete;
    while(temp) {
        nodeToDelete = temp;
        temp = temp->next;
        free(nodeToDelete);
        nodeToDelete = NULL;
    }
    list->root = NULL;
    list->size = 0;
}

int countMember(const LinkedList* list) {
    int listState = isListEmpty(list);
    if(listState == -1 || listState) {
        return 0;
    }
    return list->size;
}

int appendLinkedList(LinkedList* list, void* obj) {
    int listState = isListEmpty(list);
    if(listState == -1) {
        return 0;
    }

    ListNode* newNode = createNode();
    if(!newNode) {
        return 0;
    }
    newNode->data = obj;
    newNode->next = list->root;
    list->root = newNode;
    list->size++;
    return 1;
    /*
    if(listState) {
        list->root = newNode;
        return 1;
    }

    ListNode* temp = list->root;
    while(temp && temp->next) {
        temp = temp->next;
    }
    temp->next = newNode;
    return 1;
    */
}

int removeElementLinkedList(LinkedList* list, const void* obj, objComparator comparator) {
    // Hàm comparator có nhiệm vụ so sánh đối tượng tại từng node với mục tiêu obj.
    // Hàm trả về 1 nếu hai đối tượng như nhau và 0 nếu chúng khác nhau (theo quy định riêng với từng đối tượng)
    int listState = isListEmpty(list);
    if(listState == -1 || listState || !obj || !comparator) {
        return 0;
    }

    ListNode* temp = list->root;
    if(comparator(temp->data, obj)) {
        list->root = temp->next;
        free(temp);
        temp = NULL;
        list->size--;
        return 1;
    }
    while(temp && temp->next && !comparator(temp->next->data, obj)) {
        temp = temp->next;
    }
    if(temp && !temp->next && comparator(temp->data, obj)) {
        free(temp);
        temp == NULL;
        list->size--;
        return 1;
    } else if(temp && temp->next && comparator(temp->next->data, obj)) {
        ListNode* nodeToDelete = temp->next;
        temp->next = temp->next->next;
        free(nodeToDelete);
        list->size--;
        return 1;
    }
    return 0;
}

void** filterList(const LinkedList* list, const FieldCondition* conds, int condAmount, int* resultSize, int maxResultLength) {
    int listState = isListEmpty(list);
    if(listState == -1 || listState || !conds) {
        return NULL;
    }

    int currentMemberCount = countMember(list);
    int init_size = currentMemberCount > maxResultLength && maxResultLength > 0 ? maxResultLength : currentMemberCount;
    void** result = malloc(sizeof(void*) * init_size);
    if(!result) {
        return NULL;
    }
    *resultSize = 0;

    ListNode* temp = list->root;
    int pass;
    while(temp && *resultSize <= init_size) {
        pass = 1;
        for(int i = 0; i < condAmount; i++) {
            const void* field_ptr = ((char*) temp->data) + conds[i].offset; // cast temp->data ve char* de dam bao se dich dung offset byte. sizeof(char) luon = 1
            if(!(conds[i].filterCond(field_ptr, conds[i].expected_value))) {
                pass = 0;
                break;
            }
        }
        if(pass) {
            result[(*resultSize)++] = temp->data;
        }

        temp = temp->next;
    }

    result = realloc(result, sizeof(void*) * (*resultSize));
    return result;
}

void iterList(const LinkedList* list, listCallback callback, void* extra) {
    int listState = isListEmpty(list);
    if(listState == -1 || listState || !callback) {
        return;
    }

    ListNode* temp = list->root;
    while(temp) {
        callback(temp->data, extra);
        temp = temp->next;
    }
}

typedef struct listToArrPacket {
    void** arr;
    int currentPos;
    int size;
} listToArrPacket;

void listToArrCallback(void* data, listToArrPacket* packet) {
    if(!packet) return;

    if(packet->currentPos < packet->size) packet->arr[packet->currentPos++] = data;
}

void** linkedListToArr(const LinkedList* list, int* arrSize) {
    int listState = isListEmpty(list);
    if(listState == -1 || listState) {
        return NULL;
    }

    int size = countMember(list);

    void** result = calloc(size, sizeof(void*));
    if(!result) return NULL;
    if(arrSize) *arrSize = size;
    listToArrPacket packet = {.arr = result, .currentPos = 0, .size = size};
    iterList(list, (listCallback) listToArrCallback, &packet);

    return result;
}

int isDataExist()
