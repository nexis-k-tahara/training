#ifndef STUDENT_H
#define STUDENT_H

#define INITIAL_CAPACITY 10 // 初期容量
#define DATA_FILE "students.dat" // 保存ファイル名
#define MAX_NAMELENGTH 50 // 学生名の最大サイズ

typedef struct {
    int id;
    char name[MAX_NAMELENGTH];
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
void reducedCapacity();
void addStudent();
void listStudents();
void findStudentById(int id);
void deleteStudentById(int id);
int saveStudentsToFile(const char *filename);
int loadStudentsFromFile(const char *filename);
void getValidInt(const char *prompt, int *value);
void getValidStr(const char *prompt, char *str);

#endif
