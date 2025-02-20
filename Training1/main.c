#include <stdio.h>
#include <stdlib.h>
#include "student.h"

int main() {
    initStudents(); // 初期化
    if (!loadStudentsFromFile(DATA_FILE)) {
        printf("既存のデータはありませんでした。\n");
    }
    int choice;
    int id;
    do {
    printf("1. 学生を追加\n");
    printf("2. 学生一覧を表示\n");
    printf("3. 学生をIDで検索\n");
    printf("4. 学生を削除\n");
    printf("5. 保存して終了\n");
    getValidInt("選択してください (1-5): ", &choice);

    switch (choice) {
        case 1:
            addStudent();
            break;
        case 2:
            listStudents();
            break;
        case 3:
            findStudentById(id);
            break;
	case 4:
	    deleteStudentById(id);
	    break;
        case 5:
            if (saveStudentsToFile(DATA_FILE)) {
                printf("データを保存しました。\n");
            } else {
                printf("データの保存に失敗しました。\n");
            }
            printf("プログラムを終了します。\n");
            break;
        default:
            printf("無効な選択です。もう一度入力してください。\n");
        }
    } while (choice != 5);

    cleanupStudents(); // メモリ解放
    return 0;
}
