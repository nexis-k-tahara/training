#ifndef STUDENT_H
#define STUDENT_H

#define INITIAL_CAPACITY 10 // 初期容量
#define DATA_FILE "students.dat" // 保存ファイル名

typedef struct {
    int id;
    char name[50];
    int age;
} Student;

// グローバル変数
extern Student *students; // 学生データへのポインタ
extern int studentCount;  // 現在の学生数
extern int studentCapacity; // 学生の最大登録可能数

// 関数プロトタイプ
void initStudents();
void cleanupStudents();
void ensureCapacity();
void addStudent();
void listStudents();
void findStudentById(int id);
int saveStudentsToFile(const char *filename);
int loadStudentsFromFile(const char *filename);
void getValidInt(const char *prompt, int *value);

#endif
