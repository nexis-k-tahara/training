#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

// グローバル変数の初期化
Student *students = NULL;
int studentCount = 0;
int studentCapacity = 0;

// 学生データを初期化
void initStudents() {
    studentCapacity = INITIAL_CAPACITY;
    students = malloc(studentCapacity * sizeof(Student));
    if (!students) {
        fprintf(stderr, "エラー: メモリの確保に失敗しました。\n");
        exit(1);
    }
}

// 学生データを解放
void cleanupStudents() {
    free(students);
}

// 必要ならメモリ容量を拡張
void ensureCapacity() {
    if (studentCount >= studentCapacity) {
        studentCapacity *= 2;
        Student *newStudents = realloc(students, studentCapacity * sizeof(Student));
        if (!newStudents) {
            fprintf(stderr, "エラー: メモリの再確保に失敗しました。\n");
            exit(1);
        }
        students = newStudents;
    }
}

// 整数入力をバリデーション付きで取得
void getValidInt(const char *prompt, int *value) {
    int result;
    char buffer[256];

    while (1) {
        printf("%s", prompt);
        // 入力を文字列として取得し、チェックする
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // 文字列から整数への変換を試みる
            result = sscanf(buffer, "%d", value);
            if (result == 1) {
                break; // 正常に整数が入力された場合
            } else {
                printf("無効な入力です。数字を入力してください。\n");
            }
        } else {
            printf("入力エラーが発生しました。\n");
        }
    }
}

// 学生を追加
void addStudent() {
    ensureCapacity(); // 必要なら容量を拡張

    Student *newStudent = &students[studentCount];
    printf("学生名を入力してください: ");
    scanf("%s", newStudent->name);

    getValidInt("学生の年齢を入力してください: ", &newStudent->age);
    newStudent->id = studentCount + 1;
    studentCount++;

    printf("学生が追加されました: ID: %d, 名前: %s, 年齢: %d\n",
           newStudent->id, newStudent->name, newStudent->age);
}

// 学生一覧を表示
void listStudents() {
    if (studentCount == 0) {
        printf("登録されている学生はいません。\n");
        return;
    }

    printf("登録されている学生の一覧:\n");
    for (int i = 0; i < studentCount; i++) {
        printf("ID: %d, 名前: %s, 年齢: %d\n",
               students[i].id, students[i].name, students[i].age);
    }
}

// IDで学生を検索
void findStudentById(int id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            printf("学生が見つかりました: ID: %d, 名前: %s, 年齢: %d\n",
                   students[i].id, students[i].name, students[i].age);
            return;
        }
    }
    printf("指定されたIDの学生は見つかりませんでした。\n");
}

// 学生データをファイルに保存
int saveStudentsToFile(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "エラー: ファイルを開けませんでした。\n");
        return 0;
    }
    fwrite(&studentCount, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), studentCount, file);
    fclose(file);
    return 1;
}

// ファイルから学生データを読み込み
int loadStudentsFromFile(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "エラー: ファイルを開けませんでした。\n");
        return 0;
    }
    fread(&studentCount, sizeof(int), 1, file);
    ensureCapacity();
    fread(students, sizeof(Student), studentCount, file);
    fclose(file);
    return 1;
}
