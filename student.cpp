//关闭C6031警告，防止scanf的返回值被忽略
#pragma warning(disable:6031)
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<windows.h>
#include<string.h>
#include<errno.h>//在load函数中要用到errno宏定义，来判断文件是否不存在
#include"student.h"

Student stu[MAX_STUDENTS_NUM];
int data_dirty = 0;//用来判断是否更新文件数据
int stu_count = 0;

//用于处理在输入整数时，输入其他非法数（如小数，字符）的情况，用来清理输入的缓冲区，避免程序出现死循环
int read_int()
{
	int value;
	int ch;
	int valid;

	while (1)
	{
		// 尝试读取整数
		int result = scanf("%d", &value);

		// 检测到输入结束，退出程序
		if (result == EOF)
		{
			printf("\n检测到输入结束，系统即将退出！\n");
			exit(EXIT_SUCCESS);
		}

		// 整数读取失败，清理当前输入行
		if (result != 1)
		{
			while ((ch = getchar()) != '\n' && ch != EOF)
			{
			}

			printf("输入无效，请输入合法整数！\n");
			continue;
		}

		// 暂时认为输入合法
		valid = 1;

		// 检查整数后面是否还有非法字符
		while ((ch = getchar()) != '\n' && ch != EOF)
		{
			if (!isspace((unsigned char)ch))//需加入ctype头文件
			{
				valid = 0;
			}
		}

		// 整行合法，返回整数
		if (valid == 1)
		{
			return value;
		}

		printf("输入无效，请输入合法整数！\n");
	}
}

//读取合法的性别，只接受"男"或"女"
static void read_gender(char* buf, const char* prompt)
{
	char value[16];
	int ch;

	while (1)
	{
		printf("%s", prompt);

		//读取失败（通常是输入流结束），直接退出
		if (scanf("%15s", value) != 1)
		{
			printf("\n检测到输入结束，系统即将退出！\n");
			exit(EXIT_SUCCESS);
		}

		//清掉本行剩余字符，避免残留影响后面的班级、成绩输入
		while ((ch = getchar()) != '\n' && ch != EOF)
		{
		}

		//必须是完整的"男"或"女"，"男生"、"男abc"都会被拒绝
		if (strcmp(value, "男") == 0 || strcmp(value, "女") == 0)//需加入string头文件
		{
			strcpy_s(buf, 10, value);
			return;
		}

		printf("性别只能是「男」或「女」，请重新输入！\n");
	}
}

// 读取合法成绩，成绩范围为 0~100
static double read_score(const char* prompt)
{
	double value;
	int ch;
	int result;
	int valid;

	while (1)
	{
		printf("%s", prompt);

		// 尝试读取浮点数
		result = scanf("%lf", &value);

		// 输入结束，退出程序
		if (result == EOF)
		{
			printf("\n检测到输入结束，系统即将退出！\n");
			exit(EXIT_SUCCESS);
		}

		// 读取失败，清理当前输入行
		if (result != 1)
		{
			while ((ch = getchar()) != '\n' && ch != EOF)
			{
			}

			if (ch == EOF)
			{
				exit(EXIT_SUCCESS);
			}

			printf("输入无效，请输入合法成绩！\n");
			continue;
		}

		// 暂时认为输入合法
		valid = 1;

		// 检查数字后面是否存在非法字符
		while ((ch = getchar()) != '\n' && ch != EOF)
		{
			if (!isspace((unsigned char)ch))
			{
				valid = 0;
			}
		}

		// 检查成绩范围
		if (valid == 1 && value >= 0.0 && value <= 100.0)
		{
			return value;
		}

		printf("成绩输入无效，请输入0~100之间的合法数字！\n");
	}
}

//查找函数，输入学号，查找成功返回下标/查找失败返回-1
int Find_student(int num)
{
	for (int i = 0; i < stu_count; i++)
	{
		if (num == stu[i].num)
		{
			return i;
		}
	}
	return -1;
}

//读取一个"已存在的学号"并返回其下标；用户输入 0 表示取消，返回 -1
static int read_existing_index(const char* prompt)
{
	printf("%s", prompt);
	while (1)
	{
		int num = read_int();

		if (num == 0)                 // 用户主动取消，立刻返回
		{
			return -1;
		}

		int idx = Find_student(num);
		if (idx != -1)                // 找到了，返回下标
		{
			return idx;
		}

		// 没找到：提示后回到循环开头重新读
		printf("不存在该学号的学生，请重新输入（输入 0 取消）：\n");
	}
}

//添加学生
void add_student()
{
	if (stu_count >= MAX_STUDENTS_NUM)
	{
		printf("学生信息已满，无法继续添加！\n");
		return;
	}
	int continue_add = 1;
	do {
		if (stu_count >= MAX_STUDENTS_NUM)
		{
			printf("学生信息已满，无法继续添加！\n");
			return;
		}
		printf("请输入学生学号：");
		int new_student_num = read_int();
		if (Find_student(new_student_num) != -1)
		{
			printf("该学号学生已存在，请检查输入的学号！\n");
			continue;
		}
		stu[stu_count].num = new_student_num;
		printf("请输入学生姓名：");
		scanf("%49s", stu[stu_count].name);
		read_gender(stu[stu_count].gender, "请输入学生性别（男/女）：");
		printf("请输入学生班级：");
		scanf("%19s", stu[stu_count].classes);
		stu[stu_count].math_score =
			read_score("请输入学生的高数成绩（0~100）：");
		stu[stu_count].c_score =
			read_score("请输入学生的C语言成绩（0~100）：");
		stu[stu_count].ds_score =
			read_score("请输入学生的数据结构成绩（0~100）：");
		stu_count++;
		data_dirty = 1;
		printf("该学生添加成功！\n");
		printf("是否继续添加学生1(是)/0(否)：");
		continue_add = read_int();
	} while (continue_add);
}

//显示所有的学生
void show_all_student()
{
	if (stu_count == 0)
	{
		printf("该系统中暂无学生信息，无法展示！\n");
		return;
	}
	else
	{
		for (int i = 0; i < stu_count; i++)
		{
			printf("学号：%d 姓名：%s 性别：%s 班级：%s ", stu[i].num, stu[i].name, stu[i].gender, stu[i].classes);
			printf("高数成绩：%.1f C语言成绩：%.1f 数据结构成绩：%.1f\n", stu[i].math_score, stu[i].c_score, stu[i].ds_score);
		}
	}
	printf("-----------------------\n");
}

//查找学生
void search_student()
{
	if (stu_count == 0)
	{
		printf("系统中暂无学生信息，无法进行查找\n");
		return ;
	}
	int i = read_existing_index("请输入要查找学生的学号（输入 0 取消）：");
	if (i == -1)
	{
		printf("已取消查找\n");
		return;
	}

	printf("学号：%d 姓名：%s 性别：%s 班级：%s ", stu[i].num, stu[i].name, stu[i].gender, stu[i].classes);
	printf("高数成绩：%.1f C语言成绩：%.1f 数据结构成绩：%.1f\n", stu[i].math_score, stu[i].c_score, stu[i].ds_score);
}

//删除学生
void delete_student()
{
	if (stu_count == 0)
	{
		printf("系统中暂无学生信息，无法进行删除\n");
		return;
	}
	int a = read_existing_index("请输入要删除学生的学号（输入 0 取消）：");
	if (a == -1)
	{
		printf("已取消删除\n");
		return;
	}

	printf("确认删除%s(学号:%d)的学生信息吗？（是：1/否：0）",stu[a].name,stu[a].num);
	int if_delete;
	while (1)
	{
		if_delete = read_int();
		switch (if_delete)
		{
		case 1:
			for (int i = a; i < stu_count - 1; i++)
			{
				stu[i] = stu[i + 1];
			}
			stu_count--;
			printf("该学生删除成功！\n");
			data_dirty = 1;
			return;
		case 0:
			printf("已取消删除\n");
			return;
		default:printf("请输入正确的数字编号（是：1/否：0）\n");
		}
	}
}

//修改学生信息
void modify_student()
{
	if (stu_count == 0)
	{
		printf("系统中暂无学生信息，无法进行修改\n");
		return;
	}
	int i = read_existing_index("请输入要修改学生的学号（输入 0 取消）：");
	if (i == -1)
	{
		printf("已取消修改\n");
		return;
	}

	while (1)
	{
		printf("修改学生菜单\n");
		printf("1.姓名\n");
		printf("2.性别\n");
		printf("3.班级\n");
		printf("4.高数成绩\n");
		printf("5.C语言成绩\n");
		printf("6.数据结构成绩\n");
		printf("7.返回主菜单\n");
		printf("请输入选择实现的功能：");
		int a = read_int();
		switch (a)
		{
		case 1:
			printf("请输入修改后的姓名：");
			scanf("%49s", stu[i].name);
			printf("修改成功！\n");
			data_dirty = 1;
			break;
		case 2:
			read_gender(stu[i].gender, "请输入修改后的性别（男/女）：");
			printf("修改成功！\n");
			data_dirty = 1;
			break;
		case 3:
			printf("请输入修改后的班级：");
			scanf("%19s", stu[i].classes);
			printf("修改成功！\n");
			data_dirty = 1;
			break;
		case 4:
			stu[i].math_score =
				read_score("请输入修改后的高数成绩（0~100）：");
			printf("修改成功！\n");
			data_dirty = 1;
			break;

		case 5:
			stu[i].c_score =
				read_score("请输入修改后的C语言成绩（0~100）：");
			printf("修改成功！\n");
			data_dirty = 1;
			break;

		case 6:
			stu[i].ds_score =
				read_score("请输入修改后的数据结构成绩（0~100）：");
			printf("修改成功！\n");
			data_dirty = 1;
			break;
		case 7:
			return;
		default:
			printf("输入的数字没有任何功能\n");
		}
	}
}

//根据科目返回对应的科目成绩
double get_score(const Student& s, int subject)
{
	switch (subject)
	{
	case 1:
		return s.math_score;
	case 2:
		return s.c_score;
	case 3:
		return s.ds_score;
	case 4:
		return s.math_score + s.c_score + s.ds_score;
	default:
		return -1;
	}
}

//按成绩排序
void sort_student()
{
	if (stu_count == 0)
	{
		printf("暂无学生信息，无法进行排序\n");
		return;
	}
	printf("请输入想查看的学科编号:1.高数 2.C语言 3.数据结构 4.各科总分\n");
	int subject = read_int();
	if (subject != 1 && subject != 2 && subject != 3&&subject!=4)
	{
		printf("输入的学科代号为无效数字\n");
		return;
	}
	Student flag_stu[MAX_STUDENTS_NUM];
	for (int i = 0; i < stu_count; i++)
	{
		flag_stu[i] = stu[i];
	}
	for (int i = 0; i < stu_count - 1; i++)//排序算法采用冒泡排序
	{
		for (int j = 0; j < stu_count - i - 1; j++)
		{
			if (get_score(flag_stu[j],subject)< get_score(flag_stu[j+1], subject))
			{
				Student flag = flag_stu[j];
				flag_stu[j] = flag_stu[j + 1];
				flag_stu[j + 1] = flag;
			}
		}
	}
	//计算名次：同分同名次，避免分数相同名次却不同
	int ranks[MAX_STUDENTS_NUM];
	for (int i = 0; i < stu_count; i++)
	{
		if (i > 0 && get_score(flag_stu[i], subject) == get_score(flag_stu[i - 1], subject))
		{
			ranks[i] = ranks[i - 1];
		}
		else
		{
			ranks[i] = i + 1;
		}
	}

	switch (subject)
	{
	case 1:
		for (int i = 0; i < stu_count; i++)
		{
			printf("排名：%d 学号：%d 姓名：%s 高数成绩：%.1f\n", ranks[i], flag_stu[i].num, flag_stu[i].name, flag_stu[i].math_score);
		}
		break;
	case 2:
		for (int i = 0; i < stu_count; i++)
		{
			printf("排名：%d 学号：%d 姓名：%s C语言成绩：%.1f\n", ranks[i], flag_stu[i].num, flag_stu[i].name, flag_stu[i].c_score);
		}
		break;
	case 3:
		for (int i = 0; i < stu_count; i++)
		{
			printf("排名：%d 学号：%d 姓名：%s 数据结构成绩：%.1f\n", ranks[i], flag_stu[i].num, flag_stu[i].name, flag_stu[i].ds_score);
		}
		break;
	case 4:
		for (int i = 0; i < stu_count; i++)
		{
			printf("排名：%d 学号：%d 姓名：%s 总分：%.1f\n", ranks[i], flag_stu[i].num, flag_stu[i].name, get_score(flag_stu[i], subject));
		}
		break;
	default:printf("输入的学科代号为无效数字\n");
	}
}

//展示单科成绩平均分
void average_score()
{
	if (stu_count == 0)
	{
		printf("系统中暂无学生信息，无法获取平均分\n");
		return;
	}
	printf("请输入想查看的学科编号:1.高数 2.C语言 3.数据结构 4.各科总分\n");
	int subject = read_int();
	if (subject != 1 && subject != 2 && subject != 3&& subject != 4)
	{
		printf("输入的学科代号为无效数字\n");
		return;
	}
	double sum = 0;
	for (int i = 0; i < stu_count; i++)
	{
		sum = sum + get_score(stu[i], subject);
	}
	double average = sum / stu_count;
	switch (subject)
	{
	case 1:
		printf("高数平均分为：%.3f\n", average);
		break;
	case 2:
		printf("C语言平均分为：%.3f\n", average);
		break;
	case 3:
		printf("数据结构平均分为：%.3f\n", average);
		break;
	case 4:
		printf("总分平均分为：%.3f\n", average);
		break;
	default:printf("输入的学科代号为无效数字\n");
	}
}

//显示及格人数和及格率
void get_pass()
{
	if (stu_count == 0)
	{
		printf("系统中暂无学生信息，无法获取及格情况\n");
		return;
	}
	printf("请输入想查看的学科编号:1.高数 2.C语言 3.数据结构 4.总分\n");
	int subject = read_int();
	if (subject != 1 && subject != 2 && subject != 3 && subject!=4)
	{
		printf("输入的学科代号为无效数字\n");
		return;
	}
	int pass_stu = 0;
	if (subject == 4)
	{
		for (int i = 0; i < stu_count; i++)
		{
			if (get_score(stu[i], subject) >= 180)
			{
				pass_stu++;
			}
		}
	}
	else
	{
		for (int i = 0; i < stu_count; i++)
		{
			if (get_score(stu[i], subject) >= 60)
			{
				pass_stu++;
			}
		}
	}
	double pass_rate = 1.0 * pass_stu / stu_count;
	printf("该科成绩及格人数为：%d,及格率为：%.3f%%\n", pass_stu, pass_rate*100);
}

// 保存学生信息
void save_student()
{
	// 第一步：打开临时文件
	FILE* fp = fopen("student.tmp", "wb");

	if (fp == NULL)
	{
		printf("临时文件打开失败，学生信息保存失败！\n");
		return;
	}

	int save_ok = 1;

	// 第二步：将所有学生信息写入临时文件
	for (int i = 0; i < stu_count; i++)
	{
		if (fwrite(&stu[i], sizeof(Student), 1, fp) != 1)
		{
			save_ok = 0;
			printf("学生信息写入失败！\n");
			break;
		}
	}

	// 第三步：检查文件是否成功关闭
	if (fclose(fp) != 0)
	{
		save_ok = 0;
		printf("临时文件关闭失败！\n");
	}

	// 第四步：只有写入和关闭都成功，才替换原文件
	if (save_ok == 1)
	{
		if (MoveFileExA(
			"student.tmp",
			"student.dat",
			MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH
		))//该功能需加入windows头文件，将临时文件里的学生信息替换到原文件
		{
			printf("学生信息保存成功！\n");
		}
		else
		{
			printf("学生信息替换失败，原有数据文件未主动删除！\n");
		}
	}
	else
	{
		printf("学生信息保存失败，原有数据文件未主动删除！\n");
	}
}

// 在文件中读取学生信息
void load_student()
{
	FILE* fp = fopen("student.dat", "rb");

	if (fp == NULL)
	{
		if (errno == ENOENT)//需加入errno头文件，来判断文件是否不存在
		{
			stu_count = 0;
			printf("暂无学生数据文件，将以空学生列表启动\n");
			return;
		}

		printf("文件打开失败\n");
		return;
	}

	// 将文件指针移动到末尾
	if (fseek(fp, 0, SEEK_END) != 0)
	{
		fclose(fp);
		printf("无法移动到文件末尾\n");
		return;
	}

	// 获取文件大小
	long file_size = ftell(fp);

	if (file_size < 0)
	{
		fclose(fp);
		printf("获取文件大小失败\n");
		return;
	}

	// 检查文件大小是否完整
	if (file_size % (long)sizeof(Student) != 0)
	{
		fclose(fp);
		printf("文件中的学生数据不完整，无法正常读取\n");
		return;
	}

	// 计算文件中的学生数量
	long record_count = file_size / (long)sizeof(Student);

	// 检查学生数量是否超过数组容量
	if (record_count > MAX_STUDENTS_NUM)
	{
		fclose(fp);
		printf("文件中的学生数量超过上限\n");
		return;
	}

	// 将文件指针移动到开头
	if (fseek(fp, 0, SEEK_SET) != 0)
	{
		fclose(fp);
		printf("无法移动到文件开头\n");
		return;
	}

	// 创建临时数组
	Student temp_stu[MAX_STUDENTS_NUM];

	int load_ok = 1;

	// 读取所有学生数据到临时数组
	for (int i = 0; i < (int)record_count; i++)
	{
		if (fread(&temp_stu[i], sizeof(Student), 1, fp) != 1)
		{
			load_ok = 0;
			printf("学生信息读取失败！\n");
			break;
		}
	}

	// 检查文件关闭是否成功
	if (fclose(fp) != 0)
	{
		load_ok = 0;
	}

	// 只有全部读取成功，才更新正式数组
	if (load_ok == 1)
	{
		for (int i = 0; i < (int)record_count; i++)
		{
			stu[i] = temp_stu[i];
		}

		stu_count = (int)record_count;

		printf("学生信息读取成功，共读取%d名学生\n", stu_count);
	}
	else
	{
		printf("学生信息读取失败，未更新学生列表！\n");
	}
}