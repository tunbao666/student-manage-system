//关闭C6031警告，防止scanf的返回值被忽略
#pragma warning(disable:6031)
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include"student.h"

//主函数
int main()
{
	load_student();
	printf("欢迎使用学生管理系统\n");
	int use_flag = 1;
	while (use_flag == 1)
	{
		printf("========== 学生管理系统 ==========\n");

		printf("请选择你要使用的功能：\n");
		printf("1.添加学生信息\n");
		printf("2.删除学生信息\n");
		printf("3.查找学生信息\n");
		printf("4.修改学生信息\n");
		printf("5.显示所有学生信息\n");
		printf("6.查看单科成绩排名\n");
		printf("7.查看单科成绩平均分\n");
		printf("8.查看及格人数与及格率\n");
		printf("0.退出系统\n");
		int flag = read_int();
		switch (flag)
		{
		case 1: add_student();    break;
		case 2: delete_student(); break;
		case 3: search_student(); break;
		case 4: modify_student(); break;
		case 5: show_all_student(); break;
		case 6: sort_student();   break;
		case 7: average_score();  break;
		case 8: get_pass();       break;
		case 0: use_flag = 0;     break;                 // 菜单直接支持退出
		default: printf("请输入 0~8 之间的功能编号\n"); break;
		}
		if (data_dirty)
		{
			save_student();//只有更新数据后才保存数据
			data_dirty=0;
		}
		
		if (use_flag == 0)
		{
			printf("感谢使用学生信息管理系统\n");
			break;
		}
		else
		{
			printf("\n按回车键返回主菜单...");
			while (getchar() != '\n')
			{
			}
			system("cls");//将屏幕刷新成干净菜单
		}
	}
	return 0;
}