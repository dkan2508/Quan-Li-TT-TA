#pragma once

#include "linked_list.h"
#include "./lib/cJSON/cJSON.h"

#define MAX_BILL_MSG_LENGTH 100

typedef struct Bill Bill;

int compareBill(const Bill* bl1, const Bill* bl2);

void printBill(const Bill* bl);
void printAllBill(const LinkedList* list);

Bill* createBill();

int freeBill(Bill* bl);

Bill** findBill(const LinkedList* list, int* resultLength, int useSearchOption, int maxResultLength);

int enterBillInfo(Bill* bl, int useSearchOption);

int getBillID(const Bill* bl);

time_t getBillTime(const Bill* bl);

int isRevenue(const Bill* bl);

int isExpense(const Bill* bl);

float getBillPrice(const Bill* bl);

cJSON* billToJSON(const Bill* bill);

Bill* parseBillFromObject(cJSON* obj);
