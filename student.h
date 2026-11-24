#pragma once

#define MAX_STUDENTS_NUM 50

typedef struct {
	int num;
	char name[50];
	char gender[10];
	char classes[20];
	double math_score;
	double c_score;
	double ds_score;
}Student;//结构体，每一个学生所包含的各种信息

extern Student stu[MAX_STUDENTS_NUM];
extern int data_dirty;
extern int stu_count;

//用于处理在输入整数时，输入其他非法数（如小数，字符）的情况，用来清理输入的缓冲区，避免程序出现死循环
int read_int();

//查找函数，输入学号，查找成功返回下标/查找失败返回-1
int Find_student(int num);

//1添加学生
void add_student();

//5显示所有学生
void show_all_student();

//3查找学生
void search_student();

//2删除学生
void delete_student();

//4修改学生信息
void modify_student();

//根据科目返回对应的科目成绩
double get_score(const Student& s, int subject);

//6按成绩排序
void sort_student();

//7展示单科成绩平均分
void average_score();

//8显示及格人数和及格率
void get_pass();

//在文件中保存学生信息
void save_student();

//在文件中读取学生信息
void load_student();