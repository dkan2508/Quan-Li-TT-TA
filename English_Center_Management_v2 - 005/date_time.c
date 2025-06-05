#include <stdio.h>
#include <stdlib.h>

#define MAX_STUDENT 100

typedef struct ClassMetadata {
    int hasTeacher;
    int hasCourse;
    int studentCount;
} ClassMetadata;

typedef struct LopHoc {
    int id;
    int courseID;
    int teacherID;
    int* studentIDList[MAX_STUDENT];

    ClassMetadata metadata;
} LopHoc;

int main() {
    LopHoc* cls = calloc(1, sizeof(LopHoc));
    printf("%d %d %d %d %d %d\n", cls->id, cls->courseID, cls->teacherID, cls->metadata.hasCourse, cls->metadata.hasTeacher, cls->metadata.studentCount);


    return 0;
}
