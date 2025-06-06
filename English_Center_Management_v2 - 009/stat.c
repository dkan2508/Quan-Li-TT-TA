#include <stdio.h>
#include <time.h>

#include "linked_list.h"
#include "general.h"
#include "stat.h"
#include "bill.h"

typedef enum DateCompareMode {
    COMPARE_YEAR,
    COMPARE_YEAR_MONTH
} DateCompareMode;

typedef struct RevenueListIterPacket {
    time_t constraint;
    float revenue;
    float expense;
    DateCompareMode mode;
} RevenueListIterPacket;

int revenueListIterCallback(const Bill* bl, RevenueListIterPacket* packet) {
    if (!bl || !packet) {
        return CONTINUE;
    }

    time_t billTime = getBillTime(bl);

    struct tm *billTm = localtime(&billTime);
    struct tm *constraintTm = localtime(&(packet->constraint));

    if (!billTm || !constraintTm) {
        return CONTINUE;
    }

    int match = 0;
    switch (packet->mode) {
        case COMPARE_YEAR:
            match = (billTm->tm_year == constraintTm->tm_year);
            break;
        case COMPARE_YEAR_MONTH:
            match = (billTm->tm_year == constraintTm->tm_year) &&
                    (billTm->tm_mon == constraintTm->tm_mon);
            break;
        default:
            match = 0;
            break;
    }
    if (!match) return CONTINUE;

    if (isRevenue(bl)) {
        packet->revenue += getBillPrice(bl);
    } else if (isExpense(bl)) {
        packet->expense += getBillPrice(bl);
    }
    return CONTINUE;
}

int revenueStatYearly(const LinkedList* billList) {
    if(!billList) return 0;

    struct tm t = {0};
    printf("Nhap nam thong ke: ");
    saferScanf("%d", &(t.tm_year));
    t.tm_year -= 1900;
    RevenueListIterPacket packet = {
        .constraint = mktime(&t),
        .expense = 0,
        .revenue = 0,
        .mode = COMPARE_YEAR,
    };
    iterList(billList, (listCallback) revenueListIterCallback, &packet);

    puts("===== KET QUA THONG KE =====");
    printf("Tong loi nhuan: %.2f\n", packet.revenue);
    printf("Tong doanh thu: %.2f\n", packet.expense);
    printf("Lai (khong tinh thue): %.2f\n", packet.revenue - packet.expense);

    return 1;
}

int revenueStatMonthly(const LinkedList* billList) {
    if(!billList) return 0;

    struct tm t = {0};
    printf("Nhap nam thong ke: ");
    saferScanf("%d", &(t.tm_year));
    t.tm_year -= 1900;
    printf("Nhap thang thong ke: ");
    saferScanf("%d", &(t.tm_mon));
    t.tm_mon -= 1;

    RevenueListIterPacket packet = {
        .constraint = mktime(&t),
        .expense = 0,
        .revenue = 0,
        .mode = COMPARE_YEAR_MONTH,
    };
    iterList(billList, (listCallback) revenueListIterCallback, &packet);

    puts("===== KET QUA THONG KE =====");
    printf("Tong loi nhuan: %.2f\n", packet.revenue);
    printf("Tong doanh thu: %.2f\n", packet.expense);
    printf("Lai (khong tinh thue): %.2f\n", packet.revenue - packet.expense);

    return 1;
}
