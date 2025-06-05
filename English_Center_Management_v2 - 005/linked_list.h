#pragma once
#define FIND_ALL_FILTER_ELEMENT -1

typedef struct LinkedList LinkedList;

// Dieu kien loc cho mot truong cu the cua mot struct
typedef int (*filterCondition)(const void* check_val_ptr, const void* expected_val_ptr);
typedef struct FieldCondition {
    filterCondition filterCond; // Ham so sanh gia tri nhan duoc voi gia tri expected_value.
                                                      // Tra ve 1 neu dieu kien loc thoa man, 0 neu khong thoa man
    int offset; // So byte tinh tu field dau cua struct den field ung dieu kien loc
    const void* expected_value; // Gia tri goc de loc
} FieldCondition;

// Mot so dang tham so ham
typedef int (*objComparator)(const void *, const void *);
typedef void (*listCallback)(const void* data, void* extra);


// Tạo và khởi tạo một danh sách liên kết
// Trả về một con trỏ tới danh sách liên kết rỗng nếu thành công, trả về NULL nếu thất bại
LinkedList* createList();

// Thêm một phần tử vào cuối linked list. Trả về 1 nếu thành công, 0 nếu thất bại
int appendLinkedList(LinkedList* list, void* obj);

// Xóa một phần tử nào đó trong linked list. Trả về 1 nếu thành công, 0 nếu thất bại
// Nếu linked list có sự xuất hiện nhiều lần của một phần tử, chỉ phần tử đầu tiên sẽ bị xóa.
int removeElementLinkedList(LinkedList* list, const void* obj, objComparator);

void** filterList(const LinkedList* list, const FieldCondition* conds, int condAmount, int* resultSize, int maxResultLength);

// Gọi free lên mọi node của cây
// Trả về 1 nếu thành công, 0 nếu thất bại
void destroyList(LinkedList* list);

// Kiểm tra xem một linked list có rỗng không
// Trả về 1 nếu có, 0 nếu không. Neu list la NULL, tra ve -1
int isListEmpty(const LinkedList* list);

// Đếm số phần tử của linked list. Trả về số phần tử cua linked list
int countMember(const LinkedList* list);

// Thực hiện loop qua danh sách, gọi callback cho từng đối tượng
void iterList(const LinkedList* list, listCallback callback, void* extra);

void** linkedListToArr(const LinkedList* list, int* arrSize);
