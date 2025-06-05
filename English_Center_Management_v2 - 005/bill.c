#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <string.h>

#include "bill.h"
#include "general.h"
#include "linked_list.h"
#include "./lib/cJSON/cJSON.h"

/*
Problems:
    - The file saver fails to delete old data and instead just append new data  V
    - For some reason, the bill searching method is not work correctly. For example, the user enter a valid id but nothing but a msg shows up.  V
    This seems to be a local problem, as other areas searching method work fine.    V
    - Delete fails to delete all bills that match certain conditions. Only the first one is deleted. This is a global problem   V
    - Crash occurs when delete bills using the status condition, even for individual deletion   V
    - Suggestion:
        - Add a function to check for valid subjects (receiver and sender)
        - CompareBill as the comparator for deleting might delete the wrong obj with the same id. fix it
        - Add breaks between different bills when print them out the screen     V
        - Try to make the modification of an object to not require re-typing all the other info. In other word, allow the user to choose which to change    V
        - ID cho trung tam??    V
*/

#define RELEVANT_FIELD_AMOUNT 7

typedef enum SUBJECT_TYPE {
    STUDENT, TEACHER, CENTER
} SUBJECT_TYPE;

typedef enum TRANSACTION_TYPE {
    REVENUE, EXPENSE
} TRANSACTION_TYPE;

typedef enum BILL_STATUS {
    PENDING, COMPLETED, CANCELLED, FAILED
} BILL_STATUS;

typedef struct Subject {
    int id;
    SUBJECT_TYPE sbj_type;
} Subject;

typedef struct BillMetaData {
    TRANSACTION_TYPE type;
} BillMetaData;

typedef struct Bill {
    Subject sender;
    Subject receiver;

    int id;
    float soTien;
    time_t transactionTime;
    BILL_STATUS status;

    BillMetaData metadata;
} Bill;

int compareBill(const Bill* bl1, const Bill* bl2) {
    if(!bl1 || !bl2) return 0;
    return bl1->id == bl2->id;
}

int compareBillID(const int* real_id, const int* expected_id) {
    return *real_id == *expected_id;
}

int compareBillMoney(const float* real_money, const float* expected_money) {
    return *real_money == *expected_money;
}

int compareBillDate(const time_t* real_date, const time_t* expected_date) {
    if (!real_date || !expected_date) return 0;

    struct tm real_tm = *localtime(real_date);
    struct tm expected_tm = *localtime(expected_date);

    if (real_tm.tm_mday == expected_tm.tm_mday &&
        real_tm.tm_mon == expected_tm.tm_mon &&
        real_tm.tm_year == expected_tm.tm_year) {
        return 1;
    }

    return 0;
}

int compareBillSubject(const Subject* real_sbj, const Subject* expected_sbj) {
    if(!real_sbj || !expected_sbj) return 0;
    return real_sbj->id == expected_sbj->id && real_sbj->sbj_type == expected_sbj->sbj_type;
}

int compareBillStatus(const BILL_STATUS* real_sts, const BILL_STATUS* expected_sts) {
    return *real_sts == *expected_sts;
}

int compareBillTransactionType(const TRANSACTION_TYPE* real_tpy, const TRANSACTION_TYPE* expected_tpy) {
    return *real_tpy == *expected_tpy;
}

const char* subjectTypeToString(SUBJECT_TYPE type) {
    switch (type) {
        case STUDENT: return "Hoc vien";
        case TEACHER: return "Giao vien";
        case CENTER: return "Trung tam";
        default: return "Khong xac dinh";
    }
}

const char* lifecycleStatusToStr(BILL_STATUS status) {
    switch (status) {
        case PENDING: return "Dang xu ly";
        case COMPLETED: return "Hoan thanh";
        case CANCELLED: return "Huy bo";
        case FAILED: return "That bai";
        default: return "Khong xac dinh";
    }
}

const char* transactionTypeToStr(TRANSACTION_TYPE type) {
    switch(type) {
        case REVENUE: return "Thu nhap";
        case EXPENSE: return "Chi phi";
        default: return "Khong xac dinh";
    }
}

void printBill(const Bill* bl) {
    if(!bl) return;

    printf("Id hoa don: %d\n", bl->id);
    printf("So tien giao dich: %.2f\n", bl->soTien);
    if(bl->sender.sbj_type == CENTER) printf("Nguoi gui: %s\n", subjectTypeToString(bl->sender.sbj_type));
    else printf("Nguoi gui: %s - %d\n", subjectTypeToString(bl->sender.sbj_type), bl->sender.id);

    if(bl->receiver.sbj_type == CENTER) printf("Nguoi nhan: %s\n", subjectTypeToString(bl->receiver.sbj_type));
    else printf("Nguoi nhan: %s - %d\n", subjectTypeToString(bl->receiver.sbj_type), bl->receiver.id);

    printf("Ngay giao dich: ");
    printDate(bl->transactionTime);
    puts("");
    printf("Gio giao dich: ");
    printTime(bl->transactionTime);
    puts("");
    printf("Trang thai giao dich: %s\n", lifecycleStatusToStr(bl->status));
    printf("Phan loai giao dich: %s\n", transactionTypeToStr(bl->metadata.type));
}

void printAllBillCallback(const Bill* bl) {
    printBill(bl);
    puts("");
}

void printAllBill(const LinkedList* list) {
    int listState = isListEmpty(list);
    if(listState || listState == -1) return;

    puts("===== DANH SACH HOA DON =====\n");
    iterList(list, (listCallback) printAllBillCallback, NULL);
}

Bill* createBill() {
    Bill* newBill = (Bill*) calloc(1, sizeof(Bill));
    if(!newBill) return NULL;
    return newBill;
}

int freeBill(Bill* bl) {
    if(!bl) return 0;
    free(bl);
    bl = NULL;
    return 1;
}

static int getModOptions(int* optionArr) {
    if(!optionArr) return 0;

    int result;
    int idx = 0;
    puts("===== Lua chon su dung truong thong tin (nhap 1 neu chon, 0 neu khong) ======");
    do {
        printf("Su dung: ID? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    do {
        printf("Su dung: So tien? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    do {
        printf("Su dung: Ngay giao dich? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    do {
        printf("Su dung: Nguoi gui? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    do {
        printf("Su dung: Nguoi nhan? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    do {
        printf("Su dung: Trang thai giao dich? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    do { 
        printf("Su dung: Loai giao dich? ");
        saferScanf("%d", &result);
    } while(result != 1 && result != 0);
    optionArr[idx++] = result;

    return 1;
}

Bill** findBill(const LinkedList* list, int* resultLength, int useSearchOption, int maxResultLength) {
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
        float money;
        struct tm t = {0};
        time_t tm_t;
        Subject sdr;
        Subject rcv;
        BILL_STATUS sts;
        TRANSACTION_TYPE tpy;

        if (mod_options[++cond_check]) {
            printf("Nhap id hoa don: ");
            saferScanf("%d", &id_adr);

            conds[cond_count].filterCond = (filterCondition) compareBillID;
            conds[cond_count].offset = offsetof(Bill, id);
            conds[cond_count].expected_value = &id_adr;

            cond_count++;
        }

        if(mod_options[++cond_check]) {
            printf("Nhap so tien giao dich: ");
            saferScanf("%f", &money);

            conds[cond_count].filterCond = (filterCondition) compareBillMoney;
            conds[cond_count].offset = offsetof(Bill, soTien);
            conds[cond_count].expected_value = &money;
            cond_count++;
        }

        if(mod_options[++cond_check]) {
            puts("Nhap ngay giao dich: ");

            printf("Nhap ngay: ");
            saferScanf("%d", &(t.tm_mday));

            printf("Nhap thang: ");
            saferScanf("%d", &(t.tm_mon));
            t.tm_mon -= 1;

            printf("Nhap nam: ");
            saferScanf("%d", &(t.tm_year));
            t.tm_year -= 1900;

            tm_t = mktime(&t);

            conds[cond_count].filterCond = (filterCondition) compareBillDate;
            conds[cond_count].offset = offsetof(Bill, transactionTime);
            conds[cond_count].expected_value = &tm_t;

            cond_count++;
        }

        if(mod_options[++cond_check]) {
            puts("Nhap thong tin nguoi gui: ");

            puts("Chon doi tuong gui: ");
            puts("1. Hoc sinh");
            puts("2. Giao vien");
            puts("3. Trung tam");
            do {
                printf("Nhap doi tuong gui: ");
                saferScanf("%d", &(sdr.sbj_type));
                if(!(sdr.sbj_type >= 1 && sdr.sbj_type <= 3)) puts("Doi tuong khong hop le!");
            } while(!(sdr.sbj_type >= 1 && sdr.sbj_type <= 3));
            sdr.sbj_type--;

            if(sdr.sbj_type != CENTER) {    // Tranh nhap id cua trung tam do trung tam ko co id
                printf("Nhap ID nguoi gui: ");
                saferScanf("%d", &(sdr.id));
            } else sdr.id = 0;

            conds[cond_count].filterCond = (filterCondition) compareBillSubject;
            conds[cond_count].offset = offsetof(Bill, sender);
            conds[cond_count].expected_value = &sdr;

            cond_count++;
        }

        if(mod_options[++cond_check]) {
            puts("Nhap thong tin nguoi nhan: ");

            puts("Chon doi tuong nhan: ");
            puts("1. Hoc sinh");
            puts("2. Giao vien");
            puts("3. Trung tam");
            do {
                printf("Nhap doi tuong nhan: ");
                saferScanf("%d", &(rcv.sbj_type));
                if(!(rcv.sbj_type >= 1 && rcv.sbj_type <= 3)) puts("Doi tuong khong hop le!");
            } while(!(rcv.sbj_type >= 1 && rcv.sbj_type <= 3));
            rcv.sbj_type--;

            if(rcv.sbj_type != CENTER) {
                printf("Nhap ID nguoi nhan: ");
                saferScanf("%d", &(rcv.id));
            } else rcv.id = 0;

            conds[cond_count].filterCond = (filterCondition) compareBillSubject;
            conds[cond_count].offset = offsetof(Bill, receiver);
            conds[cond_count].expected_value = &rcv;

            cond_count++;
        }

        if(mod_options[++cond_check]) {
            puts("Lua chon trang thai hoa don");
            puts("1. Dang xu ly");
            puts("2. Hoan thanh");
            puts("3. Huy bo");
            puts("4. That bai");
            do {
                printf("Nhap trang thai hoa don: ");
                saferScanf("%d", &sts);
                if(!(sts >= 1 && sts <= 4)) puts("Trang thai khong hop le!");
            } while(!(sts >= 1 && sts <= 4));
            sts--;

            conds[cond_count].filterCond = (filterCondition) compareBillStatus;
            conds[cond_count].offset = offsetof(Bill, status);
            conds[cond_count].expected_value = &sts;

            cond_count++;
        }

        if (mod_options[++cond_check]) {
            puts("Chon loai giao dich:");
            puts("1. Thu nhap");
            puts("2. Chi phi");
            do {
                printf("Nhap loai giao dich: ");
                saferScanf("%d", &tpy);
                if(!(tpy >= 1 && tpy <= 2)) puts("Loai giao dich khong hop le!");
            } while(!(tpy >= 1 && tpy <= 2));
            tpy--;

            conds[cond_count].filterCond = (filterCondition) compareBillTransactionType;
            conds[cond_count].offset = offsetof(Bill, metadata.type);
            conds[cond_count].expected_value = &tpy;

            cond_count++;
        }

        return (Bill**) filterList(list, conds, cond_count, resultLength, maxResultLength);
    }
    return NULL;
}

int enterBillInfo(Bill* bl, int useSearchOption) {
    if(!bl) return 0;

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
            printf("Nhap id hoa don: ");
            saferScanf("%d", &(bl->id));
        }

        if(mod_options[++cond_count]) {
            printf("Nhap so tien giao dich: ");
            saferScanf("%f", &(bl->soTien));
        }

        if(mod_options[++cond_count]) {
            int option = 1;
            do {
                printf("Su dung thoi gian hien tai (1) hay tu nhap (0)? ");
                saferScanf("%d", &option);
                if(!(option == 1 || option == 0)) puts("Lua chon khong hop le!");
            } while(!(option == 1 || option == 0));
            if(option) bl->transactionTime = time(NULL);
            else {
                struct tm t = {0};
                puts("Nhap ngay giao dich: ");

                printf("Nhap ngay: ");
                saferScanf("%d", &(t.tm_mday));

                printf("Nhap thang: ");
                saferScanf("%d", &(t.tm_mon));
                t.tm_mon -= 1;

                printf("Nhap nam: ");
                saferScanf("%d", &(t.tm_year));
                t.tm_year -= 1900;

                printf("Nhap gio: ");
                saferScanf("%d", &(t.tm_hour));

                printf("Nhap phut: ");
                saferScanf("%d", &(t.tm_min));

                printf("Nhap giay: ");
                saferScanf("%d", &(t.tm_sec));

                bl->transactionTime = mktime(&t);
            }
        }

        if(mod_options[++cond_count]) {
            puts("Nhap thong tin nguoi gui: ");
            Subject sdr;

            puts("Chon doi tuong gui: ");
            puts("1. Hoc sinh");
            puts("2. Giao vien");
            puts("3. Trung tam");
            do {
                printf("Nhap doi tuong gui: ");
                saferScanf("%d", &(sdr.sbj_type));
                if(!(sdr.sbj_type >= 1 && sdr.sbj_type <= 3)) puts("Doi tuong khong hop le!");
            } while(!(sdr.sbj_type >= 1 && sdr.sbj_type <= 3));
            sdr.sbj_type--;

            if(sdr.sbj_type != CENTER) {    // Tranh nhap id cua trung tam do trung tam ko co id
                printf("Nhap ID nguoi gui: ");
                saferScanf("%d", &(sdr.id));
            } else sdr.id = 0;

            bl->sender = sdr;
        }

        if(mod_options[++cond_count]) {
            puts("Nhap thong tin nguoi nhan: ");
            Subject rcv;

            puts("Chon doi tuong nhan: ");
            puts("1. Hoc sinh");
            puts("2. Giao vien");
            puts("3. Trung tam");
            do {
                printf("Nhap doi tuong nhan: ");
                saferScanf("%d", &(rcv.sbj_type));
                if(!(rcv.sbj_type >= 1 && rcv.sbj_type <= 3)) puts("Doi tuong khong hop le!");
            } while(!(rcv.sbj_type >= 1 && rcv.sbj_type <= 3));
            rcv.sbj_type--;

            if(rcv.sbj_type != CENTER) {    // Tranh nhap id cua trung tam do trung tam ko co id
                printf("Nhap ID nguoi nhan: ");
                saferScanf("%d", &(rcv.id));
            } else rcv.id = 0;

            bl->receiver = rcv;
        }

        if(mod_options[++cond_count]) {
            puts("Lua chon trang thai hoa don");
            puts("1. Dang xu ly");
            puts("2. Hoan thanh");
            puts("3. Huy bo");
            puts("4. That bai");
            BILL_STATUS sts;
            do {
                printf("Nhap trang thai hoa don: ");
                saferScanf("%d", &sts);
                if(!(sts >= 1 && sts <= 4)) puts("Trang thai khong hop le khong hop le!");
            } while(!(sts >= 1 && sts <= 4));
            sts--;

            bl->status = sts;
        }

        if (mod_options[++cond_count]) {
            puts("Chon loai giao dich:");
            puts("1. Thu nhap");
            puts("2. Chi phi");
            TRANSACTION_TYPE tpy;
            do {
                printf("Nhap loai giao dich: ");
                saferScanf("%d", &tpy);
                if(!(tpy >= 1 && tpy <= 2)) puts("Loai giao dich khong hop le!");
            } while(!(tpy >= 1 && tpy <= 2));
            tpy--;
            bl->metadata.type = tpy;
        }

        return 1;
    }
    return 0;
}

int getBillID(const Bill* bl) {
    if(!bl) return 0;
    return bl->id;
}

time_t getBillTime(const Bill* bl) {
    if(!bl) return 0;
    return bl->transactionTime;
}

int isRevenue(const Bill* bl) {
    if(!bl) return 0;
    return bl->metadata.type == REVENUE;
}

int isExpense(const Bill* bl) {
    if(!bl) return 0;
    return bl->metadata.type == EXPENSE;
}

float getBillPrice(const Bill* bl) {
    if(!bl) return 0;
    return bl->soTien;
}

cJSON* billToJSON(const Bill* bill) {
    if (!bill) return NULL;

    // Create the root JSON object
    cJSON* root = cJSON_CreateObject();

    // Basic fields
    cJSON_AddNumberToObject(root, "id", bill->id);
    cJSON_AddNumberToObject(root, "soTien", bill->soTien);
    cJSON_AddNumberToObject(root, "transactionTime", (double) bill->transactionTime);
    cJSON_AddNumberToObject(root, "status", bill->status);

    // Nested sender
    cJSON* sender = cJSON_CreateObject();
    cJSON_AddNumberToObject(sender, "id", bill->sender.id);
    cJSON_AddNumberToObject(sender, "sbj_type", bill->sender.sbj_type);
    cJSON_AddItemToObject(root, "sender", sender);

    // Nested receiver
    cJSON* receiver = cJSON_CreateObject();
    cJSON_AddNumberToObject(receiver, "id", bill->receiver.id);
    cJSON_AddNumberToObject(receiver, "sbj_type", bill->receiver.sbj_type);
    cJSON_AddItemToObject(root, "receiver", receiver);

    // Metadata
    cJSON* meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "type", bill->metadata.type);
    cJSON_AddItemToObject(root, "metadata", meta);

    return root;
}

Bill* parseBillFromObject(cJSON* obj) {
    
    if (!obj) return NULL;

    Bill* bill = malloc(sizeof(Bill));
    if (!bill) {
        return NULL;
    }
    
    memset(bill, 0, sizeof(Bill));
    

    // Basic fields
    cJSON* id = cJSON_GetObjectItem(obj, "id");
    cJSON* soTien = cJSON_GetObjectItem(obj, "soTien");
    cJSON* transactionTime = cJSON_GetObjectItem(obj, "transactionTime");
    cJSON* status = cJSON_GetObjectItem(obj, "status");
    


    cJSON* sender = cJSON_GetObjectItem(obj, "sender");
    cJSON* receiver = cJSON_GetObjectItem(obj, "receiver");
    cJSON* metadata = cJSON_GetObjectItem(obj, "metadata");
    
    if (cJSON_IsNumber(id)) bill->id = id->valueint;
    if (cJSON_IsNumber(soTien)) bill->soTien = (float) soTien->valuedouble;
    if (cJSON_IsNumber(transactionTime)) bill->transactionTime = (time_t) transactionTime->valuedouble;
    if (cJSON_IsNumber(status)) {
        bill->status = status->valueint;
    }
    

    if (sender) {
        cJSON* s_id = cJSON_GetObjectItem(sender, "id");
        cJSON* s_type = cJSON_GetObjectItem(sender, "sbj_type");
        if (cJSON_IsNumber(s_id)) bill->sender.id = s_id->valueint;
        if (cJSON_IsNumber(s_type)) bill->sender.sbj_type = s_type->valueint;
    }

    if (receiver) {
        cJSON* r_id = cJSON_GetObjectItem(receiver, "id");
        cJSON* r_type = cJSON_GetObjectItem(receiver, "sbj_type");
        if (cJSON_IsNumber(r_id)) bill->receiver.id = r_id->valueint;
        if (cJSON_IsNumber(r_type)) bill->receiver.sbj_type = r_type->valueint;
    }

    if (metadata) {
        cJSON* m_type = cJSON_GetObjectItem(metadata, "type");
        if (cJSON_IsNumber(m_type)) bill->metadata.type = m_type->valueint;
    }

    return bill;
}
