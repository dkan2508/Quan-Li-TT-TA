#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "general.h"
#include "linked_list.h"

#include "student.h"
#include "teacher.h"
#include "course.h"
#include "class.h"
#include "bill.h"
#include "stat.h"
#include "file.h"

/*
Problem:
    - Consider creating an interface to avoid giving higher functionalities (stat analysis, ...) access to lower features like the linked list
    - The search system's inherent problem: rigid, hardcode in all fields to be choosen ==> The fields that doesn't show up during creation also can't be modified. 
    - A system to prevent repeated ID is missing.
*/

int main() {
    if(!isManagerFileExist()) {
        if(!resetManagerFile()) puts("Khoi tao file quan li duong dan that bai!");
    }
    if(!isAutoFileExist()) {
        if(!resetAutoFile()) puts("Khoi tao file quan li chuc nang tu dong load va luu tru file that bai!");
    }

    int useFileLoader = isAutoLoaderOn();
    
    LinkedList* studentList = loadLinkedList(STUDENT, (jsonParser) parseHocVienFromObject, useFileLoader);
    LinkedList* teacherList = loadLinkedList(TEACHER, (jsonParser) parseGiangVienFromObject, useFileLoader);
    LinkedList* courseList = loadLinkedList(COURSE, (jsonParser) parseKhoaHocFromObject, useFileLoader);
    LinkedList* classList = loadLinkedList(CLASS, (jsonParser) parseLopHocFromObject, useFileLoader);
    LinkedList* billList = loadLinkedList(BILL, (jsonParser) parseBillFromObject, useFileLoader);
    
    int luaChon;
    do {
        printf("\n===== MENU CHINH =====\n");
        printf("1. Quan ly hoc sinh\n");
        printf("2. Quan ly giang vien\n");
        printf("3. Quan ly khoa hoc\n");
        printf("4. Quan ly lop hoc\n");
        printf("5. Quan ly hoa don\n");
        printf("6. Thong ke\n");
        printf("7. Quan li file\n");
        printf("8. Thoat chuong trinh\n");
        printf("Nhap lua chon: ");
        saferScanf("%d", &luaChon);
        
        switch (luaChon) {
            int luaChonCon;
            case 1: {
                do {
                    printf("\n--- MENU ---\n");
                    printf("1. Xem danh sach hoc sinh\n");
                    printf("2. Them hoc sinh\n");
                    printf("3. Xoa hoc sinh\n");
                    printf("4. Sua thong tin hoc sinh\n");
                    printf("5. Tim kiem hoc sinh\n");
                    printf("6. Dang ky lop hoc\n");
                    printf("7. Roi lop hoc\n");
                    printf("8. Quay lai\n");
                    printf("Nhap lua chon: ");
                    saferScanf("%d", &luaChonCon);
                    
                    switch (luaChonCon) {
                        case 1: { 
                            int studentNum = countMember(studentList);
                            if(studentNum > 0) {
                                puts("Danh sach cac hoc vien:");
                                printAllStudent(studentList);
                            } else {
                                puts("Chua co hoc vien!");
                            }
                            break;
                        }
                        case 2: {
                            HocVien* newStudent = createStudent();
                            if(!newStudent) {
                                puts("Khong the tao hoc vien moi!");
                                break;
                            }
                            int infoResult = enterStudentInfo(newStudent, 0);
                            if(!infoResult) {
                                puts("Nhap thong tin khong hoan thanh");
                                free(newStudent);
                                break;
                            }
                            int appendResult = appendLinkedList(studentList, (void*) newStudent);
                            if(appendResult) puts("Them hoc vien thanh cong!");
                            else {
                                puts("Them hoc vien that bai!");
                                free(newStudent);
                            }
                            break;
                        }
                        case 3: {
                            int deleteListLength;
                            HocVien** deleteList = findStudent(studentList, &deleteListLength, 1, FIND_ALL_FILTER_ELEMENT);
                            if(deleteListLength) {
                                for(int i = 0; i < deleteListLength; i++) {
                                    int result = removeElementLinkedList(studentList, deleteList[i], (objComparator) compareStudent);
                                    free(deleteList[i]);
                                    if(!result) puts("Xoa hoc vien that bai\n");
                                }
                                free(deleteList);
                                puts("Da xoa xong cac hoc vien!");
                            } else puts("Khong tim thay hoc vien!");

                            break;
                        }
                        case 4: {
                            // Tim hoc sinh muon sua thong tin
                            int filterResultLength = 0;
                            HocVien** filteredList = findStudent(studentList, &filterResultLength, 1, 1);

                            if(filteredList) {
                                puts("\nChon thong tin can dieu chinh:");
                                int modResult = enterStudentInfo(filteredList[0], 1); // Luon chi xet hoc sinh dau
                                if(modResult) {
                                    puts("Thay doi thong tin thanh cong");
                                } else puts("Thay doi thong tin that bai");
                                
                                free(filteredList);
                                break;
                            }
                            puts("Tim hoc vien that bai!");

                            break;
                        }
                        case 5: {
                            int filterResultLength = 0;
                            HocVien** filteredList = findStudent(studentList, &filterResultLength, 1, FIND_ALL_FILTER_ELEMENT);
                            
                            puts("\nDanh sach ket qua:");
                            if(filteredList) {
                                for(int i = 0; i < filterResultLength; i++) {
                                    printStudent(filteredList[i]);
                                    puts("");
                                }
                                free(filteredList);
                                break;
                            }
                            puts("Tim kiem hoc vien that bai");

                            break;
                        }
                        case 6: {
                            int resultLengthStudent = 0;
                            puts("\nNhap thong tin hoc sinh:");
                            HocVien** searchResultStudent = findStudent(studentList, &resultLengthStudent, 1, 1);
                            if(!searchResultStudent) {
                                puts("Tim kiem that bai!");
                                break;
                            }

                            int resultLengthClass = 0;
                            puts("\nNhap thong tin lop hoc:");
                            LopHoc** searchResultClass = findClass(classList, &resultLengthClass, 1, FIND_ALL_FILTER_ELEMENT);
                            if(!searchResultClass) {
                                puts("Tim kiem that bai!");
                                free(searchResultStudent);
                                break;
                            }

                            HocVien* student = searchResultStudent[0];
                            LopHoc* class = NULL;
                            for(int i = 0; i < resultLengthClass; i++) {
                                if(isClassAvailable(searchResultClass[i])) { // Tim class dau tien chua day hoc sinh
                                    class = searchResultClass[i];
                                    break;
                                }
                            }
                            if(!class) {
                                puts("Khong con lop trong!");
                                free(searchResultStudent);
                                free(searchResultClass);
                                break;
                            }

                            puts("\nThem hoc sinh:");
                            printStudent(student);
                            puts("\nVao lop:");
                            printClass(class);
                            int result = assignStudentToClass(class, student);
                            if(result) puts("Dang ky hoc thanh cong!");
                            else puts("Dang ky hoc that bai!");

                            free(searchResultStudent);
                            free(searchResultClass);

                            break;
                        }
                        case 7: {
                            int resultLengthStudent = 0;
                            puts("\nNhap thong tin hoc sinh:");
                            HocVien** searchResultStudent = findStudent(studentList, &resultLengthStudent, 1, 1);
                            if(!searchResultStudent) break;

                            int resultLengthClass = 0;
                            puts("\nNhap thong tin lop hoc:");
                            LopHoc** searchResultClass = findClass(classList, &resultLengthClass, 1, FIND_ALL_FILTER_ELEMENT);
                            if(!searchResultClass) {
                                free(searchResultStudent);
                                break;
                            }

                            HocVien* student = searchResultStudent[0];
                            LopHoc* class = NULL;
                            for(int i = 0; i < resultLengthClass; i++) {
                                if(!isClassEmpty(searchResultClass[i]) && isStudentInClass(searchResultClass[i], student)) {
                                    class = searchResultClass[i];
                                    break;
                                }
                            }
                            if(!class) {
                                puts("Khong tim thay hoc sinh!");
                                free(searchResultStudent);
                                free(searchResultClass);
                                break;
                            }

                            puts("\nXoa hoc sinh:");
                            printStudent(student);
                            puts("\nKhoi lop:");
                            printClass(class);

                            int result = removeStudentFromClass(class, student);
                            if(result) puts("Xoa hoc sinh thanh cong!");
                            else puts("Xoa hoc sinh that bai!");

                            free(searchResultStudent);
                            free(searchResultClass);

                            break;
                        }
                        case 8: {
                            break;
                        }
                        default:
                            printf("Lua chon khong hop le. Vui long chon lai.\n");
                            
                    }
                } while (luaChonCon != 8);
                break;
            }
            case 2: {
                do {
                    printf("\n--- MENU ---\n");
                    printf("1. Xem danh sach giang vien\n");
                    printf("2. Them giang vien\n");
                    printf("3. Xoa giang vien\n");
                    printf("4. Sua thong tin giang vien\n");
                    printf("5. Tim kiem giang vien\n");
                    printf("6. Dang ky lop day\n");
                    printf("7. Huy day\n");
                    printf("8. Quay lai\n");
                    printf("Nhap lua chon: ");
                    saferScanf("%d", &luaChonCon);

                    switch (luaChonCon) {
                        case 1: { 
                            int teacherNum = countMember(teacherList);
                            if(teacherNum > 0) {
                                puts("Danh sach cac giang vien:");
                                printAllTeacher(teacherList);
                            } else {
                                puts("Chua co giang vien!");
                            }
                            break;
                        }
                        case 2: {
                            GiangVien* newTeacher = createTeacher();
                            if(!newTeacher) {
                                puts("Khong the tao giang vien moi!");
                                break;
                            }
                            int infoResult = enterTeacherInfo(newTeacher, 0);
                            if(!infoResult) {
                                puts("Nhap thong tin khong hoan thanh");
                                free(newTeacher);
                                break;
                            }
                            int appendResult = appendLinkedList(teacherList, (void*) newTeacher);
                            if(appendResult) puts("Them giang vien thanh cong!");
                            else {
                                puts("Them giang vien that bai!");
                                free(newTeacher);
                            }
                            break;
                        }
                        case 3: {
                            int deleteListLength;
                            GiangVien** deleteList = findTeacher(teacherList, &deleteListLength, 1, FIND_ALL_FILTER_ELEMENT);
                            if(deleteListLength) {
                                for(int i = 0; i < deleteListLength; i++) {
                                    int result = removeElementLinkedList(teacherList, deleteList[i], (objComparator) compareTeacher);
                                    free(deleteList[i]);
                                    if(!result) puts("Xoa giang vien that bai\n");
                                }
                                free(deleteList);
                                puts("Da xoa xong cac giang vien!");
                            } else puts("Khong tim thay giang vien!");

                            break;
                        }
                        case 4: {
                            int filterResultLength = 0;
                            GiangVien** filteredList = findTeacher(teacherList, &filterResultLength, 1, 1);

                            if(filteredList) {
                                puts("\nChon thong tin can dieu chinh:");
                                int modResult = enterTeacherInfo(filteredList[0], 1);
                                if(modResult) {
                                    puts("Thay doi thong tin thanh cong");
                                } else puts("Thay doi thong tin that bai");
                                
                                free(filteredList);
                                break;
                            }
                            puts("Tim giang vien that bai!");

                            break;
                        }
                        case 5: {
                            int filterResultLength = 0;
                            GiangVien** filteredList = findTeacher(teacherList, &filterResultLength, 1, FIND_ALL_FILTER_ELEMENT);

                            puts("\nDanh sach ket qua:");
                            if(filteredList) {
                                for(int i = 0; i < filterResultLength; i++) {
                                    printTeacher(filteredList[i]);
                                    puts("");
                                }
                                free(filteredList);
                                break;
                            }
                            puts("Tim kiem giang vien that bai");

                            break;
                        }
                        case 6: {
                            int resultLengthTeacher = 0;
                            puts("\nNhap thong tin giao vien:");
                            GiangVien** searchResultTeacher = findTeacher(teacherList, &resultLengthTeacher, 1, 1);
                            if(!searchResultTeacher) {
                                puts("Tim kiem that bai!");
                                break;
                            }

                            int resultLengthClass = 0;
                            puts("\nNhap thong tin lop hoc:");
                            LopHoc** searchResultClass = findClass(classList, &resultLengthClass, 1, 1);
                            if(!searchResultClass) {
                                puts("Tim kiem that bai!");
                                free(searchResultTeacher);
                                break;
                            }

                            GiangVien* teacher = searchResultTeacher[0];
                            LopHoc* class = searchResultClass[0];

                            puts("Them giao vien:");
                            printTeacher(teacher);
                            puts("Vao lop:");
                            printClass(class);

                            int result = assignTeacherToClass(class, teacher);
                            if(result) puts("Dang ky day thanh cong!");
                            else puts("Dang ky day that bai!");

                            free(searchResultTeacher);
                            free(searchResultClass);

                            break;
                        }
                        case 7: {
                            int resultLengthClass = 0;

                            puts("\nNhap thong tin lop hoc:");
                            LopHoc** searchResultClass = findClass(classList, &resultLengthClass, 1, FIND_ALL_FILTER_ELEMENT);
                            if(!searchResultClass) break;

                            LopHoc* class = NULL;
                            for(int i = 0; i < resultLengthClass; i++) {
                                if(isClassHasTeacher(class)) {
                                    class = searchResultClass[i];
                                    break;
                                }
                            }
                            if(!class) {
                                puts("Khong tim thay lop co giao vien!");
                                free(searchResultClass);
                                break;
                            }

                            puts("Xoa giao vien khoi lop:");
                            printClass(class);

                            int result = removeTeacherFromClass(class);
                            if(result) puts("Xoa giao vien thanh cong thanh cong!");
                            else puts("Xoa giao vien that bai!");

                            free(searchResultClass);
                            break;
                        }
                        case 8: {
                            break;
                        }
                        default:
                            printf("Lua chon khong hop le. Vui long chon lai.\n");
                            
                    }
                } while (luaChonCon != 8);
                break;
            }
            case 3: {
                do {
                    printf("\n--- MENU ---\n");
                    printf("1. Xem danh sach khoa hoc\n");
                    printf("2. Them khoa hoc\n");
                    printf("3. Xoa khoa hoc\n");
                    printf("4. Sua thong tin khoa hoc\n");
                    printf("5. Tim kiem khoa hoc\n");
                    printf("6. Quay lai\n");
                    printf("Nhap lua chon: ");
                    saferScanf("%d", &luaChonCon);

                    switch (luaChonCon) {
                        case 1: { 
                            int courseNum = countMember(courseList);
                            if(courseNum > 0) {
                                puts("Danh sach cac khoa hoc:");
                                printAllCourse(courseList);
                            } else {
                                puts("Chua co khoa hoc!");
                            }
                            break;
                        }
                        case 2: {
                            KhoaHoc* newCourse = createCourse();
                            if(!newCourse) {
                                puts("Khong the tao khoa hoc moi!");
                                break;
                            }
                            int infoResult = enterCourseInfo(newCourse, 0);
                            if(!infoResult) {
                                puts("Nhap thong tin khong hoan thanh");
                                free(newCourse);
                                break;
                            }
                            int appendResult = appendLinkedList(courseList, (void*) newCourse);
                            if(appendResult) puts("Them khoa hoc thanh cong!");
                            else {
                                puts("Them khoa hoc that bai!");
                                free(newCourse);
                            }
                            break;
                        }
                        case 3: {
                            int deleteListLength;
                            KhoaHoc** deleteList = findCourse(courseList, &deleteListLength, 1, FIND_ALL_FILTER_ELEMENT);
                            if(deleteListLength) {
                                for(int i = 0; i < deleteListLength; i++) {
                                    int result = removeElementLinkedList(courseList, deleteList[i], (objComparator) compareCourse);
                                    free(deleteList[i]);
                                    if(!result) puts("Xoa khoa hoc that bai\n");
                                }
                                puts("Da xoa xong cac khoa hoc!");
                            } else puts("Khong tim thay khoa hoc!");

                            break;
                        }
                        case 4: {
                            int filterResultLength = 0;
                            KhoaHoc** filteredList = findCourse(courseList, &filterResultLength, 1, 1);

                            if(filteredList) {
                                puts("\nChon thong tin can dieu chinh:");
                                int modResult = enterCourseInfo(filteredList[0], 1);
                                if(modResult) {
                                    puts("Thay doi thong tin thanh cong");
                                } else puts("Thay doi thong tin that bai");
                                
                                free(filteredList);
                                break;
                            }
                            puts("Tim khoa hoc that bai!");

                            break;
                        }
                        case 5: {
                            int filterResultLength = 0;
                            KhoaHoc** filteredList = findCourse(courseList, &filterResultLength, 1, FIND_ALL_FILTER_ELEMENT);

                            puts("\nDanh sach ket qua:");
                            if(filteredList) {
                                for(int i = 0; i < filterResultLength; i++) {
                                    printCourse(filteredList[i]);
                                    puts("");
                                }
                                free(filteredList);
                                break;
                            }
                            puts("Tim kiem khoa hoc that bai");

                            break;
                        }
                        case 6: {
                            break;
                        }
                        default:
                            printf("Lua chon khong hop le. Vui long chon lai.\n");
                            
                    }
                } while (luaChonCon != 6);
                break;
            }
            case 4: {
                do {
                    printf("\n--- MENU ---\n");
                    printf("1. Xem danh sach lop hoc\n");
                    printf("2. Them lop hoc\n");
                    printf("3. Xoa lop hoc\n");
                    printf("4. Sua thong tin lop hoc\n");
                    printf("5. Tim kiem lop hoc\n");
                    printf("6. Quay lai\n");
                    printf("Nhap lua chon: ");
                    saferScanf("%d", &luaChonCon);

                    switch (luaChonCon) {
                        case 1: { 
                            int classNum = countMember(classList);
                            if(classNum > 0) {
                                puts("Danh sach cac lop hoc:");
                                printAllClass(classList);
                            } else {
                                puts("Chua co lop hoc!");
                            }
                            break;
                        }
                        case 2: {
                            LopHoc* newClass = createClass();
                            if(!newClass) {
                                puts("Khong the tao lop hoc moi!");
                                break;
                            }
                            int infoResult = enterClassInfo(newClass, 0);
                            if(!infoResult) {
                                puts("Nhap thong tin khong hoan thanh");
                                free(newClass);
                                break;
                            }
                            int appendResult = appendLinkedList(classList, (void*) newClass);
                            if(appendResult) puts("Them lop hoc thanh cong!");
                            else {
                                puts("Them lop hoc that bai!");
                                free(newClass);
                            }
                            break;
                        }
                        case 3: {
                            int deleteListLength;
                            LopHoc** deleteList = findClass(classList, &deleteListLength, 1, FIND_ALL_FILTER_ELEMENT);
                            if(deleteListLength) {
                                for(int i = 0; i < deleteListLength; i++) {
                                    int result = removeElementLinkedList(classList, deleteList[i], (objComparator) compareClass);
                                    free(deleteList[i]);
                                    if(!result) puts("Xoa lop hoc that bai\n");
                                }
                                puts("Da xoa xong cac lop hoc!");
                            } else puts("Khong tim thay lop hoc!");

                            break;
                        }
                        case 4: {
                            int filterResultLength = 0;
                            LopHoc** filteredList = findClass(classList, &filterResultLength, 1, 1);

                            if(filteredList) {
                                puts("\nChon thong tin can dieu chinh:");
                                int modResult = enterClassInfo(filteredList[0], 1);
                                if(modResult) {
                                    puts("Thay doi thong tin thanh cong");
                                } else puts("Thay doi thong tin that bai");
                                
                                free(filteredList);
                                break;
                            }
                            puts("Tim lop hoc that bai!");

                            break;
                        }
                        case 5: {
                            int filterResultLength = 0;
                            LopHoc** filteredList = findClass(classList, &filterResultLength, 1, FIND_ALL_FILTER_ELEMENT);

                            puts("\nDanh sach ket qua:");
                            if(filteredList) {
                                for(int i = 0; i < filterResultLength; i++) {
                                    printClass(filteredList[i]);
                                    puts("");
                                }
                                free(filteredList);
                                break;
                            }
                            puts("Tim kiem lop hoc that bai");

                            break;
                        }
                        case 6: {
                            break;
                        }
                        default:
                            printf("Lua chon khong hop le. Vui long chon lai.\n");
                            
                    }
                } while (luaChonCon != 6);
                break;
            }
            case 5: {
                do {
                    printf("\n--- MENU ---\n");
                    printf("1. Xem danh sach hoa don\n");
                    printf("2. Them hoa don\n");
                    printf("3. Xoa hoa don\n");
                    printf("4. Sua thong tin hoa don\n");
                    printf("5. Tim kiem hoa don\n");
                    printf("6. Quay lai\n");
                    printf("Nhap lua chon: ");
                    saferScanf("%d", &luaChonCon);

                    switch (luaChonCon) {
                        case 1: { 
                            int billNum = countMember(billList);
                            if(billNum > 0) {
                                puts("Danh sach cac hoa don:");
                                printAllBill(billList);
                            } else {
                                puts("Chua co hoa don!");
                            }
                            break;
                        }
                        case 2: {
                            Bill* newBill = createBill();
                            if(!newBill) {
                                puts("Khong the tao hoa don moi!");
                                break;
                            }
                            int infoResult = enterBillInfo(newBill, 0);
                            if(!infoResult) {
                                puts("Nhap thong tin khong hoan thanh");
                                free(newBill);
                                break;
                            }
                            int appendResult = appendLinkedList(billList, (void*) newBill);
                            if(appendResult) puts("Them hoa don thanh cong!");
                            else {
                                puts("Them hoa don that bai!");
                                free(newBill);
                            }
                            break;
                        }
                        case 3: {
                            int deleteListLength;
                            Bill** deleteList = findBill(billList, &deleteListLength, 1, FIND_ALL_FILTER_ELEMENT);
                            if(deleteListLength) {
                                for(int i = 0; i < deleteListLength; i++) {
                                    int result = removeElementLinkedList(billList, deleteList[i], (objComparator) compareBill);
                                    free(deleteList[i]);
                                    if(!result) puts("Xoa hoa don that bai\n");
                                }
                                free(deleteList);
                                puts("Da xoa xong cac hoa don!");
                            } else puts("Khong tim thay hoa don!");

                            break;
                        }
                        case 4: {
                            int filterResultLength = 0;
                            Bill** filteredList = findBill(billList, &filterResultLength, 1, 1);

                            if(filteredList) {
                                puts("\nChon thong tin can dieu chinh:");
                                int modResult = enterBillInfo(filteredList[0], 1);
                                if(modResult) {
                                    puts("Thay doi thong tin thanh cong");
                                } else puts("Thay doi thong tin that bai");
                                
                                free(filteredList);
                                break;
                            }
                            puts("Tim hoa don that bai!");

                            break;
                        }
                        case 5: {
                            int filterResultLength = 0;
                            Bill** filteredList = findBill(billList, &filterResultLength, 1, FIND_ALL_FILTER_ELEMENT);

                            puts("\nDanh sach ket qua:");
                            if(filteredList) {
                                for(int i = 0; i < filterResultLength; i++) {
                                    printBill(filteredList[i]);
                                    puts("");
                                }
                                free(filteredList);
                                break;
                            }
                            puts("Tim kiem hoa don that bai");

                            break;
                        }
                        case 6:
                            break;
                        default:
                            printf("Lua chon khong hop le. Vui long chon lai.\n");      
                    }
                } while (luaChonCon != 6);
                break;
            }
            case 6: {
                do {
                    printf("\n--- Thong ke ---\n");
                    printf("1. Thong ke hoc sinh\n");
                    printf("2. Thong ke giang vien\n");
                    printf("3. Thong ke khoa hoc\n");
                    printf("4. Thong ke doanh thu\n");
                    printf("5. Quay lai\n");
                    printf("Nhap lua chon: ");
                    saferScanf("%d", &luaChonCon);

                    switch (luaChonCon){
                        case 1: {
                            break;
                        }
                        case 2: {
                            break;
                        }
                        case 3: {
                            break;
                        }
                        case 4: {
                            int luaChonCon2;
                            do {
                                printf("\n--- Thong ke doanh thu ---\n");
                                printf("1. Thong ke theo nam\n");
                                printf("2. Thong ke theo thang\n");
                                printf("3. Quay lai\n");
                                printf("Nhap lua chon: ");
                                saferScanf("%d", &luaChonCon2);

                                switch (luaChonCon2) {
                                    case 1: {
                                        int result = revenueStatYearly(billList);
                                        if(!result) puts("Thong ke that bai");
                                        break;
                                    }
                                    case 2: {
                                        int result = revenueStatMonthly(billList);
                                        if(!result) puts("Thong ke that bai");
                                        break;
                                    }
                                    case 3: {
                                        break;
                                    }
                                    default:
                                        printf("Lua chon khong hop le, vui long chon lai\n");
                                        break;
                                    }
                            } while(luaChonCon2 != 3);

                            break;
                        }
                        case 5: {
                            break;
                        }
                        default:
                            printf("Lua chon khong hop le, vui long chon lai\n");
                    }
                } while(luaChonCon != 5);
                break;
            }
            case 7: {
                do {
                    printf("\n--- FILE ---\n");
                    printf("1. File hoc sinh\n");
                    printf("2. File giang vien\n");
                    printf("3. File khoa hoc\n");
                    printf("4. File lop hoc\n");
                    printf("5. File hoa don\n");
                    printf("6. Reset file quan li\n");
                    printf("7. Quan li auto file\n");
                    printf("8. Quay lai\n");
                    printf("Nhap lua chon: ");
                    saferScanf("%d", &luaChonCon);

                    if(luaChonCon == 6) {
                        int result = resetManagerFile();
                        if(result) puts("Reset file quan ly thanh cong");
                        else puts("Reset file quan ly that bai");
                    } else if(luaChonCon == 7) {
                        int luaChonCon2;
                        do {
                            printf("\n--- Hanh dong ---\n");
                            printf("1. Auto loader\n");
                            printf("2. Auto saver\n");
                            printf("3. Reset auto file\n");
                            printf("4. Quay lai\n");
                            printf("Nhap lua chon: ");
                            saferScanf("%d", &luaChonCon2);
                    
                            switch (luaChonCon2) {
                                case 1: {
                                    int option;
                                    do {
                                        printf("\n--- Hanh dong ---\n");
                                        printf("1. Su dung auto loader\n");
                                        printf("2. Tat auto loader\n");
                                        printf("Nhap lua chon: ");
                                        saferScanf("%d", &option);
                                        if(!(option == 1 || option == 2)) puts("Lua chon khong hop le");
                                    } while(!(option == 1 || option == 2));
                                    int mode = option == 1 ? 1 : 0;
                                    onOffAutoLoader(mode);
                                    break;
                                }
                                case 2: {
                                    int option;
                                    do {
                                        printf("\n--- Hanh dong ---\n");
                                        printf("1. Su dung auto saver\n");
                                        printf("2. Tat auto saver\n");
                                        printf("Nhap lua chon: ");
                                        saferScanf("%d", &option);
                                        if(!(option == 1 || option == 2)) puts("Lua chon khong hop le");
                                    } while(!(option == 1 || option == 2));
                                    int mode = option == 1 ? 1 : 0;
                                    onOffAutoSaver(mode);
                                    break;
                                }
                                case 3: {
                                    if(resetAutoFile()) puts("Reset thanh cong");
                                    else puts("Reset that bai");
                                    break;
                                }
                                case 4: {
                                    break;
                                }
                                default:
                                    printf("Lua chon khong hop le. Vui long chon lai.\n");
                            }             
                        } while (luaChonCon2 != 4);
                    } else if(luaChonCon == 1 || luaChonCon == 2 || luaChonCon == 3 || luaChonCon == 4 || luaChonCon == 5) {
                        int luaChonCon2;
                        do {
                            printf("\n--- Hanh dong ---\n");
                            printf("1. Xoa file\n");
                            printf("2. Tao file\n");
                            printf("3. Di chuyen file\n");
                            printf("4. Xem duong dan hien tai\n");
                            printf("5. Chon file\n");
                            printf("6. Quay lai\n");
                            printf("Nhap lua chon: ");
                            saferScanf("%d", &luaChonCon2);
                    
                            switch (luaChonCon2) {
                                case 1: {
                                    int result = deleteDataFile(luaChonCon - 1);
                                    if(result) puts("Xoa file du lieu thanh cong");
                                    else puts("Xoa file du lieu that bai");
                                    break;
                                }
                                case 2: {
                                    char path[MAX_FILE_PATH_LENGTH];
                                    printf("Nhap duong dan cho file: ");
                                    fgets(path, MAX_FILE_PATH_LENGTH - 1, stdin);
                                    path[strcspn(path, "\n")] = '\0';
                                    path[MAX_FILE_PATH_LENGTH - 1] = '\0';
                                    int result = createDataFile(luaChonCon - 1, path);
                                    if(result) puts("Tao file moi thanh cong");
                                    else puts("Tao file moi that bai");
                                    break;
                                }
                                case 3: {
                                    char path[MAX_FILE_PATH_LENGTH];
                                    printf("Nhap dia chi moi: ");
                                    fgets(path, MAX_FILE_PATH_LENGTH - 1, stdin);
                                    path[strcspn(path, "\n")] = '\0';
                                    path[MAX_FILE_PATH_LENGTH - 1] = '\0';

                                    int result = moveFile(luaChonCon - 1, path);
                                    if(result) puts("Di chuyen file thanh cong");
                                    else puts("Di chuyen file that bai");
                                    break;
                                }
                                case 4: {
                                    int result = showCurrentPath(luaChonCon - 1);
                                    if(!result) puts("Xem duong dan that bai");
                                    break;
                                }
                                case 5: {
                                    // Chuong trinh chi co the lam viec voi du lieu cua mot file cho moi loai doi tuong tai mot thoi diem
                                    // Lay duong dan cua mot file du lieu
                                    printf("CANH BAO: HAY DAM BAO FILE DUOC HUONG TOI LA FILE DU LIEU DUNG LOAI VA DUNG CU PHAP!!!!\n");
                                    char duongDanMoi[MAX_FILE_PATH_LENGTH];
                                    printf("Nhap dia chi moi: ");
                                    fgets(duongDanMoi, MAX_FILE_PATH_LENGTH - 1, stdin);
                                    duongDanMoi[strcspn(duongDanMoi, "\n")] = '\0';
                                    duongDanMoi[MAX_FILE_PATH_LENGTH - 1] = '\0';

                                    int result = chooseFile(luaChonCon - 1, duongDanMoi);
                                    if(result) puts("Chon file thanh cong");
                                    else puts("Chon file that bai");
                                    break;
                                }
                                case 6:
                                    break;
                                default:
                                    printf("Lua chon khong hop le. Vui long chon lai.\n");
                            }             
                        } while (luaChonCon2 != 6);
                    } else if(luaChonCon != 8) {
                        printf("Lua chon khong hop le. Vui long chon lai!\n");
                    }
                } while(luaChonCon != 8);
                break;
            }   
            case 8:
                printf("Thoat chuong trinh.\n");
                break;
            default:
                printf("Lua chon khong hop le. Vui long chon lai.\n");
        }
    } while (luaChon != 8);

    int useFileSaver = isAutoSaverOn();
    if(useFileSaver) {
        if(!saveLinkedListToJSON(STUDENT, studentList, (jsonSerializer) hocVienToJSON)) puts("Luu file hoc sinh that bai!");
        if(!saveLinkedListToJSON(TEACHER, teacherList, (jsonSerializer) giangVienToJSON)) puts("Luu file giang vien that bai!");
        if(!saveLinkedListToJSON(COURSE, courseList, (jsonSerializer) khoaHocToJSON)) puts("Luu file khoa hoc that bai!");
        if(!saveLinkedListToJSON(CLASS, classList, (jsonSerializer) lopHocToJSON)) puts("Luu file lop hoc that bai!");
        if(!saveLinkedListToJSON(BILL, billList, (jsonSerializer) billToJSON)) puts("Luu file hoa don that bai!");
    }

    iterList(studentList, (listCallback) freeStudent, NULL);
    destroyList(studentList);

    iterList(teacherList, (listCallback) freeTeacher, NULL);
    destroyList(teacherList);

    iterList(courseList, (listCallback) freeCourse, NULL);
    destroyList(courseList);

    iterList(classList, (listCallback) freeClass, NULL);
    destroyList(classList);

    iterList(billList, (listCallback) freeBill, NULL);
    destroyList(billList);

    return 0;
}
