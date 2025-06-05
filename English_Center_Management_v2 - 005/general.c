#include <stdio.h>
#include <string.h>
#include "general.h"

void saferScanf(char formatString[], void* addr) {
    /*
    Ep nguoi dung nhap dung dinh dang input de dam bao tinh dung dan du lieu.
    Chi co the nhan du lieu cho mot bien mot luc, khong nhu scanf
    */
    // Chu y tranh su dung format string voi whitespace phia sau, vd: " %d  ". Dieu do se khien cho input khong bao gio duoc chap nhan
    int result;
    char extraChr;
    char chr;

    while (1) {
        result = scanf(formatString, addr);
        if(result == 0 || result == EOF) {
            printf("Dau vao khong hop le\n");
            printf("Dau vao: ");
            while((chr = getchar()) != '\n' && chr != EOF); // Don sach input khong hop le cu trong buffer
        } else {
            if(scanf("%c", &extraChr) == 1 && extraChr != '\n') { // Do mot ly do nao do, input nhu "3ad" voi format %d vx duoc nhan. Dieu kien nay o day de ngan dieu do
                printf("Dau vao khong hop le\n");
                printf("Dau vao: ");
                while((chr = getchar()) != '\n' && chr != EOF); // Don sach input khong hop le cu trong buffer
            } else {
                break;
            }
        }
    }
}

int skippableScanf(const char* format, void* result, int allow_skip) {
    char buffer[SKIPPABLE_SCANF_BUFFER];

    while (1) {
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            return -1;
        }

        if (buffer[0] == '\n') {
            if(allow_skip) {
                return 0;
            } else {
                printf("Khong the bo qua truong nay!\n");
                printf("Dau vao: ");
                continue;
            }
        }

        char *newline = strchr(buffer, '\n');
        if (newline) *newline = '\0';

        int scanned = sscanf(buffer, format, result);
        if (scanned == 1) {
            return 1;
        } else {
            printf("Dau vao khong hop le!\n");
            printf("Dau vao: ");
        }
    }
}


void printDate(time_t time) {
    struct tm* tm_info = localtime(&time);
    if (tm_info) {
        printf("%02d/%02d/%04d", 
               tm_info->tm_mday, 
               tm_info->tm_mon + 1,   
               tm_info->tm_year + 1900
        );
    }
}


void printTime(time_t time) {
    struct tm* tm_info = localtime(&time);
    if (tm_info) {
        printf("%02d:%02d:%02d", 
               tm_info->tm_hour, 
               tm_info->tm_min, 
               tm_info->tm_sec
        );
    }
}

