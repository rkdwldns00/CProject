#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#define _CRT_SECURE_NO_WARNINGS

/*
���α׷�����

1.���������� �����͸� �迭������
2.�迭�� ������� ��û��� ����
3.����� �迭�� ������ �������Ͽ� �����
*/

/*
Ŀ�ǵ�����
clear ����
	���ڿ� �ش��ϴ¹� �ʱ�ȭ
	0�̸� ��ü�ʱ�ȭ
remove ����1 ����2
	����1�� �ش��ϴ¹��� ����2���ڸ� �ʱ�ȭ
replace ����1 ����2 ����3 ����4
	����1���� ����2�����ڿ� ����3���� ����4������ ��ü
inserte ����1 ����2 �̸�
	����1���� ����2�����ڿ� �̸�����
rollback
	������� �ʱ�ȭ
backup �����̸�
	���絥���͸� ����������
open �����̸�
	���絥���͸� �����ϰ� ���� ���ϳ������� �����
save
	����� ����
*/

//���ڿ��� �׳ɹ�ȯ���� �ʾƼ� ����μ����� �����ͷ� ��ȯ
#define BLANK "�����"

//getTime�Լ��� ���̴� �����
#define YEAR 0
#define MONTH 1
#define DAY 2
#define HOUR 3
#define MINUITE 4
#define SECOND 5
//����� �����
#define ROOMBACK1 6
#define ROOMNAME1 15
#define ROOMBACK2 6
#define ROOMNAME2 15
#define ROOMBACK3 6
#define ROOMNAME3 15

//Ŀ����ǥ����� 2D���� ����ü
typedef struct pos2D
{
	int x;
	int y;
}Vector2;

//Vector2 �����Լ�
Vector2 newVector2(int x, int y) {
	Vector2 re;
	re.x = x;
	re.y = y;
	return re;
}

//gotoxy
void gotoxy(int x, int y)

{

	COORD pos = { x,y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

}

//Ŀ�������
void CursorView()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //Ŀ�� ���� (1 ~ 100)
	cursorInfo.bVisible = FALSE; //Ŀ�� Visible TRUE(����) FALSE(����)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

//�˰���� �ð��� �Է¹޾� �ð���ȯ
int getTime(int type) {
	//����ü�� �ð��� �޾ƿ� ����ü�����ͷ� ����
	time_t timer;
	struct tm* t;
	timer = time(NULL);
	t = localtime(&timer);
	switch (type) {
	case YEAR:
		return t->tm_year + 1900;
		break;
	case MONTH:
		return t->tm_mon + 1;
		break;
	case DAY:
		return t->tm_mday;
		break;
	case HOUR:
		return t->tm_hour;
		break;
	case MINUITE:
		return t->tm_min;
		break;
	case SECOND:
		return t->tm_sec;
		break;
	default:
		return -1;
	}
}

//Ŀ����ǥ
Vector2 getCursor() {
	CONSOLE_SCREEN_BUFFER_INFO presentCur;        	// �ܼ� ���â�� ������ ��� ���ؼ� ������ ����ü
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);  	//���� Ŀ���� ��ġ ������ �����ϴ� �Լ�
	return newVector2(presentCur.dwCursorPosition.X, presentCur.dwCursorPosition.Y);  	//����ü�� ������ �� ���
}

//�̸������͸� �Է¹޾� ������� "�����" ��ȯ �ƴϸ� �״�ι�ȯ
int* chkname(char arr[]) {
	//"�����"�� �����ͷι�ȯ
	arr[6] = 0;
	return arr[0] == '0' ? BLANK : arr;
}

//�̸��� �ִ��������� ������
int chkcolor(char arr[],int nameColor,int defaultColor) {
	return arr[0] == '0' ? defaultColor : nameColor;
}

//���ڻ� ����
void textcolor(int colorNum) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

//�޴�,�ð����
void showUI(int index) {
	//�ε��������� �ﰢ�����
	textcolor(14);
	printf("%s         ����          %s\n", index == 0 ? "��" : "  ", index == 0 ? "��" : "  ");
	printf("%s         1 ��          %s\t\t", index == 1 ? "��" : "  ", index == 1 ? "��" : "  ");                         printf("      %s\t\t\t\t\t\t",index!=0&&index!=4?"[��]":"   ");                                  printf("%d��\n", getTime(DAY));
	printf("%s         2 ��          %s\t\t\t\t\t\t\t\t\t", index == 2 ? "��" : "  ", index == 2 ? "��" : "  ");                                                                              printf("%d��\n", getTime(HOUR));
	printf("%s         3 ��          %s\t\t\t\t\t\t\t\t\t", index == 3 ? "��" : "  ", index == 3 ? "��" : "  ");                                                                              printf("%d��\n", getTime(MINUITE));
	printf("%s         ����          %s\t\t\t\t\t\t\t\t\t", index == 4 ? "��" : "  ", index == 4 ? "��" : "  ");                                                                              printf("\n");//printf("%d��\n\n\n",getTime(SECOND));
	textcolor(15);
}

//1�������
void showroom1(char arr[][10]) {


	textcolor(ROOMBACK1);
	printf("���������\t"); printf("  ���������\t");     printf("    ���������\t"); printf("      ���������\t"); printf("        ���������\n");
	printf("��            ��\t"); printf("  ��            ��\t");     printf("    ��            ��\t"); printf("      ��            ��\t"); printf("        ��            ��\n");
	printf("��"); textcolor(chkcolor(arr[0],ROOMNAME1,ROOMBACK1)); printf("1��(% 6s)", chkname(arr[0])); textcolor(ROOMBACK1); printf(" ��\t  ��");  textcolor(chkcolor(arr[1], ROOMNAME1, ROOMBACK1)); printf("2��(% 6s)", chkname(arr[1])); textcolor(ROOMBACK1);    printf(" ��\t    ��"); textcolor(chkcolor(arr[2], ROOMNAME1, ROOMBACK1)); printf("3��(% 6s)", chkname(arr[2])); textcolor(ROOMBACK1); printf(" ��\t      ��"); textcolor(chkcolor(arr[3], ROOMNAME1, ROOMBACK1)); printf("4��(% 6s)", chkname(arr[3])); textcolor(ROOMBACK1); printf(" ��\t        ��"); textcolor(chkcolor(arr[4], ROOMNAME1, ROOMBACK1)); printf("5��(% 6s)", chkname(arr[4])); textcolor(ROOMBACK1); printf(" ��\n");
	printf("��            ��\t"); printf("  ��            ��\t");     printf("    ��            ��\t"); printf("      ��            ��\t"); printf("        ��            ��\n");
	printf("���������\t"); printf("  ���������\t");     printf("    ���������\t"); printf("      ���������\t"); printf("        ���������\n\n\n\n");



	printf("���������\t"); printf("  ���������\t"); printf("    ���������\t");     printf("      ���������\t"); printf("        ���������\n");
	printf("��            ��\t"); printf("  ��            ��\t"); printf("    ��            ��\t");     printf("      ��            ��\t"); printf("        ��            ��\n");
	printf("��"); textcolor(chkcolor(arr[5], ROOMNAME1, ROOMBACK1)); printf("6��(% 6s)", chkname(arr[5])); textcolor(ROOMBACK1); printf(" ��\t  ��");  textcolor(chkcolor(arr[6], ROOMNAME1, ROOMBACK1)); printf("7��(% 6s)", chkname(arr[6])); textcolor(ROOMBACK1);    printf(" ��\t    ��"); textcolor(chkcolor(arr[7], ROOMNAME1, ROOMBACK1)); printf("8��(% 6s)", chkname(arr[7])); textcolor(ROOMBACK1); printf(" ��\t      ��"); textcolor(chkcolor(arr[8], ROOMNAME1, ROOMBACK1)); printf("9��(% 6s)", chkname(arr[8])); textcolor(ROOMBACK1); printf(" ��\t        ��"); textcolor(chkcolor(arr[9], ROOMNAME1, ROOMBACK1)); printf("10��(% 6s)", chkname(arr[9])); textcolor(ROOMBACK1); printf("��\n");
	//printf("��6��(%6s) ��\t", chkname(arr[5])); printf("  ��7��(%6s) ��\t", chkname(arr[6])); printf("    ��8��(%6s) ��\t", chkname(arr[7]));     printf("      ��9��(%6s) ��\t", chkname(arr[8])); printf("        ��10��(%6s)��\n", chkname(arr[9]));
	printf("��            ��\t"); printf("  ��            ��\t"); printf("    ��            ��\t");     printf("      ��            ��\t"); printf("        ��            ��\n");
	printf("���������\t"); printf("  ���������\t"); printf("    ���������\t");     printf("      ���������\t"); printf("        ���������\n\n\n\n");


	printf("���������          "); printf("���������          "); printf("���������          ");     printf("���������          "); printf("���������\n");
	printf("��            ��          "); printf("��            ��          "); printf("��            ��          ");     printf("��            ��          "); printf("��            ��\n");
	printf("��"); textcolor(chkcolor(arr[10], ROOMNAME1, ROOMBACK1)); printf("11��(% 6s)", chkname(arr[10])); textcolor(ROOMBACK1); printf("��\t  ��");  textcolor(chkcolor(arr[11], ROOMNAME1, ROOMBACK1)); printf("12��(% 6s)", chkname(arr[11])); textcolor(ROOMBACK1);    printf("��\t    ��"); textcolor(chkcolor(arr[12], ROOMNAME1, ROOMBACK1)); printf("13��(% 6s)", chkname(arr[12])); textcolor(ROOMBACK1); printf("��\t      ��"); textcolor(chkcolor(arr[13], ROOMNAME1, ROOMBACK1)); printf("14��(% 6s)", chkname(arr[13])); textcolor(ROOMBACK1); printf("��\t        ��"); textcolor(chkcolor(arr[14], ROOMNAME1, ROOMBACK1)); printf("15��(% 6s)", chkname(arr[14])); textcolor(ROOMBACK1); printf("��\n");
	//printf("��11��(%6s)��          ", chkname(arr[10])); printf("��12��(%6s)��          ", chkname(arr[11])); printf("��13��(%6s)��          ", chkname(arr[12]));     printf("��14��(%6s)��          ", chkname(arr[13])); printf("��15��(%6s)��\n", chkname(arr[14]));
	printf("��            ��          "); printf("��            ��          "); printf("��            ��          ");     printf("��            ��          "); printf("��            ��\n");
	printf("���������          "); printf("���������          "); printf("���������          ");     printf("���������          "); printf("���������\n\n\n\n");


	printf("���������          "); printf("���������          "); printf("���������          ");     printf("���������          "); printf("���������\n");
	printf("��            ��          "); printf("��            ��          "); printf("��            ��          ");     printf("��            ��          "); printf("��            ��\n");
	printf("��"); textcolor(chkcolor(arr[15], ROOMNAME1, ROOMBACK1)); printf("16��(% 6s)", chkname(arr[15])); textcolor(ROOMBACK1); printf("��\t  ��");  textcolor(chkcolor(arr[16], ROOMNAME1, ROOMBACK1)); printf("17��(% 6s)", chkname(arr[16])); textcolor(ROOMBACK1);    printf("��\t    ��"); textcolor(chkcolor(arr[17], ROOMNAME1, ROOMBACK1)); printf("18��(% 6s)", chkname(arr[17])); textcolor(ROOMBACK1); printf("��\t      ��"); textcolor(chkcolor(arr[18], ROOMNAME1, ROOMBACK1)); printf("19��(% 6s)", chkname(arr[18])); textcolor(ROOMBACK1); printf("��\t        ��"); textcolor(chkcolor(arr[19], ROOMNAME1, ROOMBACK1)); printf("20��(% 6s)", chkname(arr[19])); textcolor(ROOMBACK1); printf("��\n");
	//printf("��16��(%6s)��          ", chkname(arr[15])); printf("��17��(%6s)��          ", chkname(arr[16])); printf("��18��(%6s)��          ", chkname(arr[17]));     printf("��19��(%6s)��          ", chkname(arr[18])); printf("��20��(%6s)��\n", chkname(arr[19]));
	printf("��            ��          "); printf("��            ��          "); printf("��            ��          ");     printf("��            ��          "); printf("��            ��\n");
	printf("���������          "); printf("���������          "); printf("���������          ");     printf("���������          "); printf("���������\n\n\n");


	textcolor(15);
	printf("\t\t\t\t\t\t [��]\n");
}

//2�������
void showroom2(char arr[][10]) {
	textcolor(ROOMBACK2);
	printf("    ��������� ����������                          ���������� ���������\n");
	printf("    ��            �� ��              ��                          ��              �� ��            ��\n");
	printf("    �� "); textcolor(chkcolor(arr[0], ROOMNAME2, ROOMBACK2)); printf("1��(% 6s)", chkname(arr[0])); textcolor(ROOMBACK2); printf("�� ��              ��                          ��              �� ��"); textcolor(chkcolor(arr[18], ROOMNAME2, ROOMBACK2)); printf("19��(% 6s)", chkname(arr[18])); textcolor(ROOMBACK2);
	printf("��\n    ��            �� ��              ��                          ��              �� ��            ��\n");
	printf("    ��������� ��              ��                          ��              �� ���������\n");
	printf("                     ��              ��                          ��              ��                 \n");
	printf("    ��������� ��              ��                          ��              �� ���������\n");
	printf("    ��            �� ��              ��                          ��              �� ��            ��\n");
	printf("    �� "); textcolor(chkcolor(arr[1], ROOMNAME2, ROOMBACK2)); printf("2��(% 6s)", chkname(arr[1])); textcolor(ROOMBACK2); printf("�� ��              ��                          ��              �� ��"); textcolor(chkcolor(arr[17], ROOMNAME2, ROOMBACK2)); printf("18��(% 6s)", chkname(arr[17])); textcolor(ROOMBACK2);
	//printf("    �� 2��(%6s)�� ��              ��                          ��              �� ��18��(%6s)��\n", chkname(arr[1]), chkname(arr[17]));
	printf("��\n    ��            �� ��              ��                          ��              �� ��            ��\n");
	printf("    ��������� ��              ��                          ��              �� ���������\n");
	printf("                     ��              ��                          ��              ��           \n");
	printf("    ��������� ��              ��                          ��              �� ���������\n");
	printf("    ��            �� ��              ��                          ��              �� ��            ��\n");
	printf("    �� "); textcolor(chkcolor(arr[2], ROOMNAME2, ROOMBACK2)); printf("3��(% 6s)", chkname(arr[2])); textcolor(ROOMBACK2); printf("�� ��              ��                          ��              �� ��"); textcolor(chkcolor(arr[16], ROOMNAME2, ROOMBACK2)); printf("17��(% 6s)", chkname(arr[16])); textcolor(ROOMBACK2);
	//printf("    �� 3��(%6s)�� ��              ��                          ��              �� ��17��(%6s)��\n", chkname(arr[2]), chkname(arr[16]));
	printf("��\n    ��            �� ��              ��                          ��              �� ��            ��\n");
	printf("    ��������� ��              ��                          ��              �� ���������\n");
	printf("                     ��              ��                          ��              ��           \n");
	printf("    ��������� ��              ��                          ��              �� ���������\n");
	printf("    ��            �� ��              ��                          ��              �� ��            ��\n");
	printf("    �� "); textcolor(chkcolor(arr[3], ROOMNAME2, ROOMBACK2)); printf("4��(% 6s)", chkname(arr[3])); textcolor(ROOMBACK2); printf("�� ��              ��                          ��              �� ��"); textcolor(chkcolor(arr[15], ROOMNAME2, ROOMBACK2)); printf("16��(% 6s)", chkname(arr[15])); textcolor(ROOMBACK2);
	//printf("    �� 4��(%6s)�� ��              ��                          ��              �� ��16��(%6s)��\n", chkname(arr[3]), chkname(arr[15]));
	printf("��\n    ��            �� ��              ��                          ��              �� ��            ��\n");
	printf("    ��������� ��              ��                          ��              �� ���������\n");
	printf("                     ��              ��                          ��              ��           \n");
	printf("    ��������� ��              ��                          ��              �� ���������\n");
	printf("    ��            �� ��              ��                          ��              �� ��            ��\n");
	printf("    �� "); textcolor(chkcolor(arr[4], ROOMNAME2, ROOMBACK2)); printf("5��(% 6s)", chkname(arr[4])); textcolor(ROOMBACK2); printf("�� ��              ��                          ��              �� ��"); textcolor(chkcolor(arr[14], ROOMNAME2, ROOMBACK2)); printf("15��(% 6s)", chkname(arr[14])); textcolor(ROOMBACK2);
	//printf("    �� 5��(%6s)�� ��              ��                          ��              �� ��15��(%6s)��\n", chkname(arr[4]), chkname(arr[14]));
	printf("��\n    ��            �� ��              ��                          ��              �� ��            ��\n");
	printf("    ��������� ��              ��                          ��              �� ���������\n");
	printf("                     ��              ��                          ��              ��                 \n");
	printf("    ��������� ��              ��                          ��              �� ���������\n");
	printf("    ��            �� ��              ��                          ��              �� ��            ��\n");
	printf("    �� "); textcolor(chkcolor(arr[5], ROOMNAME2, ROOMBACK2)); printf("6��(% 6s)", chkname(arr[5])); textcolor(ROOMBACK2); printf("�� ��              ��                          ��              �� ��"); textcolor(chkcolor(arr[13], ROOMNAME2, ROOMBACK2)); printf("14��(% 6s)", chkname(arr[13])); textcolor(ROOMBACK2);
	//printf("    �� 6��(%6s)�� ��              ��                          ��              �� ��14��(%6s)��\n", chkname(arr[5]), chkname(arr[13]));
	printf("��\n    ��            �� ��              ��                          ��              �� ��            ��\n");
	printf("    ��������� ��               ��                        ��               �� ���������\n");
	printf("                     ��                ��                      ��                ��           \n");
	printf("    ��������� ��                  ��                  ��                  �� ���������\n");
	printf("    ��            �� ��                   ��               ��                    �� ��            ��\n");
	printf("    �� "); textcolor(chkcolor(arr[6], ROOMNAME2, ROOMBACK2)); printf("7��(% 6s)", chkname(arr[6])); textcolor(ROOMBACK2); printf("�� ��                     ��������                       �� ��"); textcolor(chkcolor(arr[12], ROOMNAME2, ROOMBACK2)); printf("13��(% 6s)", chkname(arr[12])); textcolor(ROOMBACK2);
	//printf("    �� 7��(%6s)�� ��                     ��������                       �� ��13��(%6s)��\n", chkname(arr[6]), chkname(arr[12]));
	printf("��\n    ��            �� ��                                                          �� ��            ��\n");
	printf("    ���������  ��                                                        ��  ���������\n");
	printf("                       ��                                                      ��\n");
	printf("                         ��                                                  ��\n");
	printf("                          ��                                                ��\n");
	printf("         ���������   ��                                             ��  ���������\n");
	printf("         ��            ��    ��                                          ��    ��            ��\n         �� ");
	textcolor(chkcolor(arr[7], ROOMNAME2, ROOMBACK2)); printf("8��(%6s)", chkname(arr[7])); textcolor(ROOMBACK2); printf("��      ��                                      ��      ��"); textcolor(chkcolor(arr[11], ROOMNAME2, ROOMBACK2)); printf("12��(% 6s)", chkname(arr[11])); textcolor(ROOMBACK2);
	//printf("         �� 8��(%6s)��      ��                                      ��      ��12��(%6s)��\n", chkname(arr[7]), chkname(arr[11]));
	printf("��\n         ��            ��        ��                                  ��        ��            ��\n");
	printf("         ���������          ������������������          ���������\n");
	printf("		                                                                                          \n");
	printf("                  ���������         ���������         ���������\n");
	printf("                  ��            ��         ��            ��         ��            ��\n                  �� ");
	textcolor(chkcolor(arr[8], ROOMNAME2, ROOMBACK2)); printf("9��(%6s)", chkname(arr[8])); textcolor(ROOMBACK2); printf("��         ��"); textcolor(chkcolor(arr[9], ROOMNAME2, ROOMBACK2)); printf("10��(% 6s)", chkname(arr[9])); textcolor(ROOMBACK2); printf("��         ��");  textcolor(chkcolor(arr[10], ROOMNAME2, ROOMBACK2)); printf("11��(% 6s)", chkname(arr[10])); textcolor(ROOMBACK2);
	//printf("                  �� 9��(%6s)��         ��10��(%6s)��         ��11��(%6s)��\n", chkname(arr[8]), chkname(arr[9]), chkname(arr[10]));
	printf("��\n                  ��            ��         ��            ��         ��            ��\n");
	printf("                  ���������         ���������         ���������\n\n");
	textcolor(15);
	printf("                                       [��]       \n");
	//Ŀ���� �����ο÷��� ȭ���̵�
	gotoxy(0, 0);
	printf(" ");

}

//3�������
void showroom3(char arr[][10]) {
	textcolor(ROOMBACK3);
	printf("         ����������   ����������\t");                                    printf("����������  ����������  \t");                                       printf("����������  ����������\n");
	printf("         ��              ��   ��              ��\t");                                    printf("��              ��  ��              ��  \t");                                       printf("��              ��  ��              ��\n         �� ");
	textcolor(chkcolor(arr[0], ROOMNAME3, ROOMBACK3)); printf("1��(%6s)", chkname(arr[0])); textcolor(ROOMBACK3);	printf("  ��   �� "); textcolor(chkcolor(arr[1], ROOMNAME3, ROOMBACK3)); printf("2��(% 6s)", chkname(arr[1])); textcolor(ROOMBACK3);     printf("  ��\t�� "); textcolor(chkcolor(arr[2], ROOMNAME3, ROOMBACK3)); printf(" 3��(% 6s)", chkname(arr[2])); textcolor(ROOMBACK3); printf(" ��  �� "); textcolor(chkcolor(arr[3], ROOMNAME3, ROOMBACK3)); printf(" 4��(% 6s)", chkname(arr[3])); textcolor(ROOMBACK3);              printf(" ��  \t�� "); textcolor(chkcolor(arr[4], ROOMNAME3, ROOMBACK3)); printf("5��(% 6s)", chkname(arr[4])); textcolor(ROOMBACK3);  printf("  ��  ��  "); textcolor(chkcolor(arr[5], ROOMNAME3, ROOMBACK3)); printf("6��(% 6s)", chkname(arr[5])); textcolor(ROOMBACK3);
	//printf("         �� 1��(%6s)  ��   �� 2��(%6s)  ��\t", chkname(arr[0]), chkname(arr[1]));     printf("�� 3��(%6s)  ��  �� 4��(%6s)  ��  \t", chkname(arr[2]), chkname(arr[3]));              printf("�� 5��(%6s)  ��  ��  6��(%6s) ��\n", chkname(arr[4]), chkname(arr[5]));
	printf(" ��\n         ��              ��   ��              ��\t");                                    printf("��              ��  ��              ��  \t");                                       printf("��              ��  ��              ��\n");
	printf("         ����������   ����������\t");                                    printf("����������  ����������  \t");                                       printf("����������  ����������\n\n");

	printf("         ����������   ����������\t");                                    printf("����������  ����������  \t");                                       printf("����������  ����������\n");
	printf("         ��              ��   ��              ��\t");                                    printf("��              ��  ��              ��  \t");                                       printf("��              ��  ��              ��\n         �� ");
	textcolor(chkcolor(arr[6], ROOMNAME3, ROOMBACK3)); printf("7��(%6s)", chkname(arr[6])); textcolor(ROOMBACK3);	printf("  ��   �� "); textcolor(chkcolor(arr[7], ROOMNAME3, ROOMBACK3)); printf("8��(% 6s)", chkname(arr[7])); textcolor(ROOMBACK3);     printf("  ��\t�� "); textcolor(chkcolor(arr[8], ROOMNAME3, ROOMBACK3)); printf(" 9��(% 6s)", chkname(arr[8])); textcolor(ROOMBACK3); printf(" ��  �� "); textcolor(chkcolor(arr[9], ROOMNAME3, ROOMBACK3)); printf("10��(% 6s)", chkname(arr[9])); textcolor(ROOMBACK3);              printf(" ��  \t�� "); textcolor(chkcolor(arr[10], ROOMNAME3, ROOMBACK3)); printf("11��(% 6s)", chkname(arr[10])); textcolor(ROOMBACK3); printf(" ��  �� "); textcolor(chkcolor(arr[11], ROOMNAME3, ROOMBACK3)); printf("12��(% 6s)", chkname(arr[11])); textcolor(ROOMBACK3);
	//printf("         �� 7��(%6s)  ��   �� 8��(%6s)  ��\t", chkname(arr[6]), chkname(arr[7]));     printf("�� 9��(%6s)  ��  ��10��(%6s)  ��  \t", chkname(arr[8]), chkname(arr[9]));              printf("��11��(%6s)  ��  �� 12��(%6s) ��\n", chkname(arr[10]), chkname(arr[11]));
	printf(" ��\n         ��              ��   ��              ��\t");                                    printf("��              ��  ��              ��  \t");                                       printf("��              ��  ��              ��\n");
	printf("         ����������   ����������\t");                                    printf("����������  ����������  \t");                                       printf("����������  ����������\n\n");

	printf("         ����������   ����������\t");                                    printf("����������  ����������  \t");                                       printf("����������  ����������\n");
	printf("         ��              ��   ��              ��\t");                                    printf("��              ��  ��              ��  \t");                                       printf("��              ��  ��              ��\n         �� ");
	textcolor(chkcolor(arr[12], ROOMNAME3, ROOMBACK3)); printf("13��(%6s)", chkname(arr[12])); textcolor(ROOMBACK3);	printf(" ��   �� "); textcolor(chkcolor(arr[13], ROOMNAME3, ROOMBACK3)); printf("14��(% 6s)", chkname(arr[13])); textcolor(ROOMBACK3);     printf(" ��\t�� "); textcolor(chkcolor(arr[14], ROOMNAME3, ROOMBACK3)); printf("15��(% 6s)", chkname(arr[14])); textcolor(ROOMBACK3); printf(" ��  �� "); textcolor(chkcolor(arr[15], ROOMNAME3, ROOMBACK3)); printf("16��(% 6s)", chkname(arr[15])); textcolor(ROOMBACK3);              printf(" ��  \t�� "); textcolor(chkcolor(arr[16], ROOMNAME3, ROOMBACK3)); printf("17��(% 6s)", chkname(arr[16])); textcolor(ROOMBACK3); printf(" ��  �� "); textcolor(chkcolor(arr[17], ROOMNAME3, ROOMBACK3)); printf("18��(% 6s)", chkname(arr[17])); textcolor(ROOMBACK3);
	//printf("         ��13��(%6s)  ��   �� 14��(%6s) ��\t", chkname(arr[12]), chkname(arr[13]));     printf("��15��(%6s)  ��  �� 16��(%6s) ��  \t", chkname(arr[14]), chkname(arr[15]));              printf("��17��(%6s)  ��  �� 18��(%6s) ��\n", chkname(arr[16]), chkname(arr[17]));
	printf(" ��\n         ��              ��   ��              ��\t");                                    printf("��              ��  ��              ��  \t");                                       printf("��              ��  ��              ��\n");
	printf("         ����������   ����������\t");                                    printf("����������  ����������  \t");                                       printf("����������  ����������\n\n\n");
	textcolor(15);
	printf("\t\t\t\t\t\t [��]\n");
}

//�����ξ��� �ڸ���������ü ���
void show(char nameList[][10]) {
	for (int i = 0; i < 3; i++) {
		int j = 0;
		for (j = 0; j < 10; j++) {
			printf("%8s ", nameList[j + i * 20]);
		}
		printf("\n");
		for (; j < 20; j++) {
			printf("%8s ", nameList[j + i * 20]);
		}
		printf("\n\n");
	}
}

//���ڿ�����
void copy(char target[], char arr[]) {
	//�̸��� ������ 7ĭ������
	for (int i = 0; i < 7; i++) {
		target[i] = arr[i];
	}
}

//���� �����Ȳ ��Ͽ�
void showlog(int a, char massage[]) {
	system("cls");
	printf("%d : %s\n", a, massage);
}

//�ڸ������Ϳ� �̸����ִ��� Ȯ��
int check(char name[], char nameList[][10]) {
	//������ 0��ȯ,������1��ȯ
	int r = 1;
	for (int i = 0; i < 60; i++) {
		if (compare(name, nameList[i])) {
			r = 0;
			break;
		}
	}
	return r;
}

//���ڿ���
int compare(char name1[], char name2[]) {
	//������ 1, �ٸ��� 0��ȯ
	int i = 0, r = 1;
	while (name1[i] != 0 && name2[i] != 0) {
		if (name1[i] != name2[i]) {
			r = 0;
			break;
		}
		i++;
	}
	return r;
}

//���ڿ��ʱ�ȭ
void clear(char arr[]) {
	//null���ڸ� ���������� null�� �ʱ�ȭ
	int i = 0;
	while (arr[i] != 0) {
		arr[i] = 0;
		i++;
	}
}

//�ڸ������Ϳ��� Ư���̸��� ��ǥ��ȯ
int findpos(char name[], char nameList[][10]) {
	//ã�������� Ž���ϰ� �ε�����ȯ, ������ -1��ȯ
	for (int i = 0; i < 60; i++) {
		if (compare(name, nameList[i])) {
			return i;
		}
	}
	return -1;
}

//��������
void save(FILE* fp, char nameList[][10]) {
	//���ڸ��� �����ϸ� �α����
	for (int i = 0; i < 60; i++) {
		if (nameList[i][0] == '0') {
			showlog(i, "����ִ� �ڸ�������...");
			fputs("0 ", fp);
		}
		else {
			int j = 0;
			showlog(i, "����� �ڸ�������...");
			while (nameList[i][j] != 0) {
				fputc(nameList[i][j], fp);
				j++;
			}
			fputc(' ', fp);
		}
	}
	showlog(60, "EOF �Է���...");
	fputs("~~~~~", fp);
	showlog(61, "���� �ݴ���...");
	fclose(fp);
	showlog(62, "����Ϸ�");
}

//�����Էµ�Ű�� keycode��
int getkey(char keycode) {
	if (_kbhit()) {
		if (_getch() == keycode) {
			return 1;
		}
	}
	return 0;
}

//�Է¹��� ���� Ư������ ���ؼ� ��ȯ
int input(char string[], int add) {
	//���̽�input�Լ��� ����ġ�߰�
	int i;
	printf("%s", string);
	scanf("%d", &i);
	return i + add;
}

//�޴��ε��� �Է�
int indexinput(int index) {
	//�޴������� �������ʵ��� ����, d�������� -2��ȯ
	int r = 0;
	char key;
	key = _getch();
	if ((key == 80) && index < 4) {
		r = 1;
	}
	else if (key == 72 && index > 0) {
		r = -1;
	}
	else if (key == 13) {
		r = -2;
	}
	return r;
}

//�������
void printHelp() {
	printf("\n����� �ڽ� �ڸ����� ���α׷�\n\n");
	printf("by ������,���Ͽ�\n\n\n");
	printf("����\n\n");
	printf("����ȭ��ǥ : ���� �̵�\n");
	printf("�Ʒ���ȭ��ǥ : �Ʒ��� �̵�\n");
	printf("���� : ����\n\n");
	printf("�̸��� �ۼ��ϸ�,\n");
	printf("�̹� ������ : �ڸ��̵� �Ǵ� ���\n");
	printf("���ο� �̸��϶� : �ڸ�����\n\n");
	printf("�����Ҷ� X��ư�� ���� �����ϸ� ");
	textcolor(4);
	printf("����");
	textcolor(15);
	printf("�� �ȵ˴ϴ�!!");
}

//����â���
void printExit(char nameList[][10], char originNameList[][10]) {
	printf("\n���� �ڸ������� �����ϰ� �����մϴ�.\n\n\n");
	printf("������ڸ�\n");
	show(nameList);
	printf("\n\n������ڸ�\n");
	show(originNameList);
}

//�����ڸ�� ��ɾ����
void commandRun(char nameList[][10])
{
	char command[100];
	do {
		int val1, val2, val3, val4;
		system("cls");
		show(nameList);
		printf("command : ");
		scanf("%s", command);
		if (compare("clear", command)) {
			scanf("%d", &val1);
			switch (val1) {
			case 0:
				for (int i = 0; i < 60; i++) {
					nameList[i][0] = '0';
					nameList[i][1] = 0;
				}
				break;
			case 1:
				for (int i = 0; i < 20; i++) {
					nameList[i][0] = '0';
					nameList[i][1] = 0;
				}
				break;
			case 2:
				for (int i = 20; i < 40; i++) {
					nameList[i][0] = '0';
					nameList[i][1] = 0;
				}
				break;
			case 3:
				for (int i = 40; i < 60; i++) {
					nameList[i][0] = '0';
					nameList[i][1] = 0;
				}
				break;
			}
		}
		else if (compare("remove", command)) {
			scanf("%d %d", &val1, &val2);
			val1--;
			val2--;
			nameList[val1 * 20 + val2][0] = '0';
			nameList[val1 * 20 + val2][1] = 0;
		}
		else if (compare("replace", command)) {
			char clip[10];
			scanf("%d %d %d %d", &val1, &val2, &val3, &val4);
			val1--;
			val2--;
			val3--;
			val4--;
			copy(clip, nameList[val1 * 20 + val2]);
			copy(nameList[val1 * 20 + val2], nameList[val3 * 20 + val4]);
			copy(nameList[val3 * 20 + val4], clip);
		}
		else if (compare("insert", command)) {
			char val3[10];
			scanf("%d %d %s", &val1, &val2, &val3);
			val1--;
			val2--;
			copy(nameList[val1 * 20 + val2], val3);
		}
		else if (compare("rollback", command)) {
			FILE* fp;
			int i = 0, j = 0, k = 0;
			char filetxt[100000]="";
			fp = fopen("data.txt", "r");
			fread(filetxt, 1, 100000, fp);
			fclose(fp);
			//���� ������ ����(����)
			while (filetxt[i] != '~') {
				k = 0;
				clear(nameList[j]);
				while (filetxt[i] != ' ' && filetxt[i] != '~') {
					nameList[j][k] = filetxt[i];
					k++;
					i++;
				}
				i++;
				j++;
			}
			fclose(fp);
		}
		else if (compare("open", command)) {
			FILE* fp;
			int i = 0, j = 0, k = 0;
			char filetxt[100000] = "",filename[100000]="";
			scanf("%s", filename);
			fp = fopen(filename, "r");
			fread(filetxt, 1, 100000, fp);
			fclose(fp);
			//���� ������ ����(����)
			while (filetxt[i] != '~') {
				k = 0;
				clear(nameList[j]);
				while (filetxt[i] != ' ' && filetxt[i] != '~') {
					nameList[j][k] = filetxt[i];
					k++;
					i++;
				}
				i++;
				j++;
			}
			fclose(fp);
		}
		else if (compare("backup", command)) {
			FILE* fp;
			char filename[100000] = "";
			scanf("%s", filename);
			fp = fopen(filename, "w");
			save(fp, nameList);
		}
	} while (!compare("save", command));
}

int main() {
	//ȭ�� ũ��,����,Ŀ������
	system("mode con:cols=170 lines=80 | title SelfStudy");
	CursorView();
	//---------------�����а� ������ ����----------------
	char filetxt[100000] = "", nameList[1000][10] = { 0 }, originNameList[1000][10] = { 0 };
	int i = 0, j = 0, k = 0;
	//���� ���� ������ �б�
	FILE* fp;
	fp = fopen("data.txt", "r");
	//���� ������ ���λ���
	if (fp == NULL) {
		fp = fopen("data.txt", "w");
		fputs("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ~~~~~", fp);
		fclose(fp);
		fp = fopen("data.txt", "r");
	}
	fread(filetxt, 1, 100000, fp);
	fclose(fp);
	//���� ������ ����(����)
	while (filetxt[i] != '~') {
		k = 0;
		while (filetxt[i] != ' ' && filetxt[i] != '~') {
			nameList[j][k] = filetxt[i];
			originNameList[j][k] = filetxt[i];
			k++;
			i++;
		}
		i++;
		j++;
	}
	//---------------------���๮------------------------
	char name[10] = { 0 }, command[100] = "";
	int roomnumber = 0, chairnumber = 0, isrunning, enterkey = 0, manuIndex = 0, keycode = 0;
	//�ʱ�ȭ��
	showUI(manuIndex);
	printHelp();
	//�ʱ��Է�
	while (keycode != -2) {
		//�޴��̵��Է�
		keycode = indexinput(manuIndex);
		if (keycode != -2) {
			manuIndex += keycode;
		}
		//�޴����
		system("cls");
		showUI(manuIndex);
		//�ε��������� �������
		if (manuIndex == 0) {
			printHelp();
		}
		if (manuIndex == 1) {
			showroom1(nameList);
		}
		else if (manuIndex == 2) {
			showroom2(nameList + 20);
		}
		else if (manuIndex == 3) {
			showroom3(nameList + 40);
		}
		else if (manuIndex == 4) {
			printExit(nameList, originNameList);
		}
	}
	//�����ڸ�弱�� �Ǵ� ���ἱ�ý� �������
	isrunning = manuIndex != 4 && manuIndex != 0;
	//���Ḧ �����Ҷ����� �ݺ�
	while (isrunning == 1) {
		//���� �޴����� ���õȹ����� �漳��
		roomnumber = manuIndex - 1;
		//�̸����� �Է¹ޱ�
		clear(name);
		name[0] = '0';
		//2���ϰ�� Ŀ���� �Ʒ��������̵�
		if (roomnumber == 1) {
			gotoxy(0, 75);
			gotoxy(0, 62);
		}
		printf("�̸� : ");
		scanf("%s", name);
		//�̸��� ��������
		if (check(name, nameList)) {
			//����ִ��ڸ��� �������� ��ӹ����
			do {
				chairnumber = input("�¼� ��ȣ : ", -1);
			} while (nameList[roomnumber * 20 + chairnumber][0] != '0');
			//�ڸ��� �̸�����
			copy(nameList[roomnumber * 20 + chairnumber], name);
		}
		//�̸��� �̹�������
		else {
			int mode, pos;
			//�ùٸ� ��� �����Ҷ����� ��ӹ����
			do {
				mode = input("1 : ���\n2 : �̵�\n���� : ", 0);
			} while (mode != 1 && mode != 2);
			//�� �����Ϳ��� �Է��� �̸���ǥã��
			pos = findpos(name, nameList);
			//��Ҽ��ý�
			if (mode == 1) {
				//�����ڸ� �����
				clear(nameList[pos]);
				nameList[pos][0] = '0';
			}
			//�̵����ý�
			else if (mode == 2) {
				do {
					chairnumber = input("�¼� ��ȣ : ", -1);
					//�̵����ڸ��� ������������� �����
				} while (nameList[roomnumber * 20 + chairnumber][0] != '0');
				//�̵����ڸ��� �̸�ä���
				copy(nameList[roomnumber * 20 + chairnumber], name);
				//�����ڸ��� �̸������
				clear(nameList[pos]);
				nameList[pos][0] = '0';
			}
		}
		//����â���� ���ư���
		manuIndex = 0;
		system("cls");
		showUI(manuIndex);
		printHelp();
		keycode = 0;
		//�ٽù����
		while (keycode != -2 || manuIndex == 0) {
			//�޴��̵��Է�
			keycode = indexinput(manuIndex);
			if (keycode != -2) {
				manuIndex += keycode;
			}
			//�޴����
			system("cls");
			showUI(manuIndex);
			//�ε��������� �������
			if (manuIndex == 0) {
				printHelp();
			}
			if (manuIndex == 1) {
				showroom1(nameList);
			}
			else if (manuIndex == 2) {
				showroom2(nameList + 20);
			}
			else if (manuIndex == 3) {
				showroom3(nameList + 40);
			}
			else if (manuIndex == 4) {
				printExit(nameList, originNameList);
			}
		}
		//�����ڸ�弱�� �Ǵ� ���ἱ�ý� �ݺ�����
		isrunning = manuIndex != 4 && manuIndex != 0;
	}
	//�����ڸ��
	if (manuIndex == 0) {
		commandRun(nameList);
	}
	//���α׷� ����� ��������
	fp = fopen("data.txt", "w");
	save(fp, nameList);
	fclose(fp);
	char savingName[50]="";
	//�α� �����̸� ���ڿ� ����
	sprintf(savingName, "log %d,%d,%d %d,%d,%d.txt\0", getTime(YEAR), getTime(MONTH), getTime(DAY), getTime(HOUR), getTime(MINUITE), getTime(SECOND));
	//�α�����
	fp = fopen(savingName, "w");
	save(fp, nameList);
	fclose(fp);
	printf("%s\n",savingName);
	return 0;
}