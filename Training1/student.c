#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

//必要ならメモリ容量を縮小
void reducedCapacity() {
    // 現在のメモリの1/4以下の学生登録数かつ、現在のメモリが初期メモリより多い場合
    if (studentCount <= studentCapacity / 4 && studentCapacity > INITIAL_CAPACITY) {
        // 学生登録数を1/2にする(不要なメモリを縮小)
	studentCapacity /= 2;
	Student* newStudents = realloc(students, studentCapacity * sizeof(Student));
	if (!newStudents){
	    fprintf(stderr, "エラー：メモリの再確保に失敗しました。\n");
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
            if (result == 1 && *value >= 0) {
                break; // 正常に整数が入力された場合
            } else {
                printf("無効な入力です。数字を入力してください。\n");
            }
        } else {
            printf("入力エラーが発生しました。\n");
        }
    }
}

//文字列入力をバリデーション付きで取得
void getValidStr(const char *prompt, char *str){
    int i;
    int isString;
    char buffer[MAX_NAMELENGTH];

    while(1){
	isString = 1;
        printf("%s",prompt);
	//入力を文字列として取得し、チェックする
	if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
	    //改行(\n)を除去する(\0に置き換える)
	    buffer[strcspn(buffer,"\n")] = '\0'; //strcspn 一致する文字を含まずに先頭からの文字数
	    //未入力の場合
	    if (buffer[0] == '\0') {
		isString = 0;
	    } else{
	        //取得した文字列の中身を1文字ずつチェックする
                for (i = 0; i < sizeof(buffer); i++){
                    //数字orスペースが含まれる場合
		    if (isdigit((unsigned char)buffer[i]) || isspace((unsigned char)buffer[i])) {
		        isString = 0;
		        break;
		    }
                }
	    }
	    if (isString) {
		strcpy(str,buffer);
		break; //正常に整数が入力された場合
	    } else{
		//入力が整数、空白を含むあるいは未入力の場合
		printf("無効な入力です。文字を入力してください。\n");
	    }
        } else{
	    printf("入力エラーが発生しました。");
	}
    } 
}

// 学生を追加
void addStudent() {
    ensureCapacity(); // 必要なら容量を拡張
    int addIndex = studentCount; // 登録する要素

    // 削除されているIDがないか確認
    for (int i = 0; i < studentCount; i++) {
	if (students[i].id != i + 1) {
	    // 削除IDのある要素の位置を保持
	    addIndex = i;
	    // 削除IDの要素の位置は詰められているため構造体配列を後ろにずらす
	    for (int j = studentCount; j < i; j--) {
		students[j] = students[j - 1];
	    }
	    break;
	}
    }
    // 登録
    Student *newStudent = &students[addIndex];
    getValidStr("学生名を入力してください：", newStudent->name);

    getValidInt("学生の年齢を入力してください: ", &newStudent->age);
    newStudent->id = addIndex + 1;
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
    getValidInt("検索する学生のIDを入力してください：",&id);
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            printf("学生が見つかりました: ID: %d, 名前: %s, 年齢: %d\n",
                   students[i].id, students[i].name, students[i].age);
            break;
        }
	if (i == studentCount - 1) {
	    printf("指定されたIDの学生は見つかりませんでした。\n");
	}
    }
}

// 学生データの削除
void deleteStudentById(int id){
    // 最終要素数を取得
    int endIndex = studentCount - 1;

    getValidInt("削除する学生のIDを入力してください：",&id);
    // 入力したIDが登録されているか確認
    for (int i = 0; i < studentCount; i++){
	if (students[i].id == id) {
	    // 削除学生表示用
	    Student deleteStudent = students[i];
	    // 合致したIDと合致したIDを疑似的に削除する
	    for (int j = i;j < studentCount - 1; j++) {
		// 合致したIDの後ろの要素から配列を前に詰める
	        students[j] = students[j + 1];
	    }
	    // 削除後、一番後ろの要素を初期化する
	    memset(&students[endIndex], -1, sizeof(Student));
	    // 学生登録数を1減らす
	    studentCount--;

	    printf("学生を削除しました：ID: %d, 名前: %s, 年齢: %d\n",
	           deleteStudent.id, deleteStudent.name, deleteStudent.age);
	    reducedCapacity();
	    break;
	}
	if (i == studentCount - 1) {
	    printf("指定されたIDの学生は見つかりませんでした。\n");
	}
    }
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
