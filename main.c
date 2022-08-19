#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#define _CRT_SECURE_NO_WARNINGS

/*
프로그램구조

1.파일을열어 데이터를 배열에저장
2.배열을 사용자의 요청대로 편집
3.종료시 배열의 내용을 원본파일에 덮어쓰기
*/

/*
커맨드종류
clear 숫자
	숫자에 해당하는방 초기화
	0이면 전체초기화
remove 숫자1 숫자2
	숫자1에 해당하는방의 숫자2번자리 초기화
replace 숫자1 숫자2 숫자3 숫자4
	숫자1번방 숫자2번의자와 숫자3번방 숫자4번의자 교체
inserte 숫자1 숫자2 이름
	숫자1번방 숫자2번의자에 이름삽입
rollback
	변경사항 초기화
backup 파일이름
	현재데이터를 복사해저장
open 파일이름
	현재데이터를 삭제하고 읽은 파일내용으로 덮어쓰기
save
	저장및 종료
*/

//문자열은 그냥반환되지 않아서 상수로선언후 포인터로 반환
#define BLANK "비었음"

//getTime함수에 쓰이는 상수들
#define YEAR 0
#define MONTH 1
#define DAY 2
#define HOUR 3
#define MINUITE 4
#define SECOND 5
//방색깔 상수들
#define ROOMBACK1 6
#define ROOMNAME1 15
#define ROOMBACK2 6
#define ROOMNAME2 15
#define ROOMBACK3 6
#define ROOMNAME3 15

//커서좌표저장용 2D벡터 구조체
typedef struct pos2D
{
	int x;
	int y;
}Vector2;

//Vector2 생성함수
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

//커서숨기기
void CursorView()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //커서 굵기 (1 ~ 100)
	cursorInfo.bVisible = FALSE; //커서 Visible TRUE(보임) FALSE(숨김)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

//알고싶은 시간을 입력받아 시간반환
int getTime(int type) {
	//구조체로 시간을 받아와 구조체포인터로 추출
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

//커서좌표
Vector2 getCursor() {
	CONSOLE_SCREEN_BUFFER_INFO presentCur;        	// 콘솔 출력창의 정보를 담기 위해서 정의한 구조체
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);  	//현재 커서의 위치 정보를 저장하는 함수
	return newVector2(presentCur.dwCursorPosition.X, presentCur.dwCursorPosition.Y);  	//구조체의 저장한 값 출력
}

//이름데이터를 입력받아 비었으면 "비었음" 반환 아니면 그대로반환
int* chkname(char arr[]) {
	//"비었음"은 포인터로반환
	arr[6] = 0;
	return arr[0] == '0' ? BLANK : arr;
}

//이름이 있는지에따른 색설정
int chkcolor(char arr[],int nameColor,int defaultColor) {
	return arr[0] == '0' ? defaultColor : nameColor;
}

//글자색 설정
void textcolor(int colorNum) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

//메뉴,시간출력
void showUI(int index) {
	//인덱스에맞춰 삼각형출력
	textcolor(14);
	printf("%s         정보          %s\n", index == 0 ? "▷" : "  ", index == 0 ? "◁" : "  ");
	printf("%s         1 실          %s\t\t", index == 1 ? "▷" : "  ", index == 1 ? "◁" : "  ");                         printf("      %s\t\t\t\t\t\t",index!=0&&index!=4?"[앞]":"   ");                                  printf("%d일\n", getTime(DAY));
	printf("%s         2 실          %s\t\t\t\t\t\t\t\t\t", index == 2 ? "▷" : "  ", index == 2 ? "◁" : "  ");                                                                              printf("%d시\n", getTime(HOUR));
	printf("%s         3 실          %s\t\t\t\t\t\t\t\t\t", index == 3 ? "▷" : "  ", index == 3 ? "◁" : "  ");                                                                              printf("%d분\n", getTime(MINUITE));
	printf("%s         종료          %s\t\t\t\t\t\t\t\t\t", index == 4 ? "▷" : "  ", index == 4 ? "◁" : "  ");                                                                              printf("\n");//printf("%d초\n\n\n",getTime(SECOND));
	textcolor(15);
}

//1번방출력
void showroom1(char arr[][10]) {


	textcolor(ROOMBACK1);
	printf("■■■■■■■■\t"); printf("  ■■■■■■■■\t");     printf("    ■■■■■■■■\t"); printf("      ■■■■■■■■\t"); printf("        ■■■■■■■■\n");
	printf("■            ■\t"); printf("  ■            ■\t");     printf("    ■            ■\t"); printf("      ■            ■\t"); printf("        ■            ■\n");
	printf("■"); textcolor(chkcolor(arr[0],ROOMNAME1,ROOMBACK1)); printf("1번(% 6s)", chkname(arr[0])); textcolor(ROOMBACK1); printf(" ■\t  ■");  textcolor(chkcolor(arr[1], ROOMNAME1, ROOMBACK1)); printf("2번(% 6s)", chkname(arr[1])); textcolor(ROOMBACK1);    printf(" ■\t    ■"); textcolor(chkcolor(arr[2], ROOMNAME1, ROOMBACK1)); printf("3번(% 6s)", chkname(arr[2])); textcolor(ROOMBACK1); printf(" ■\t      ■"); textcolor(chkcolor(arr[3], ROOMNAME1, ROOMBACK1)); printf("4번(% 6s)", chkname(arr[3])); textcolor(ROOMBACK1); printf(" ■\t        ■"); textcolor(chkcolor(arr[4], ROOMNAME1, ROOMBACK1)); printf("5번(% 6s)", chkname(arr[4])); textcolor(ROOMBACK1); printf(" ■\n");
	printf("■            ■\t"); printf("  ■            ■\t");     printf("    ■            ■\t"); printf("      ■            ■\t"); printf("        ■            ■\n");
	printf("■■■■■■■■\t"); printf("  ■■■■■■■■\t");     printf("    ■■■■■■■■\t"); printf("      ■■■■■■■■\t"); printf("        ■■■■■■■■\n\n\n\n");



	printf("■■■■■■■■\t"); printf("  ■■■■■■■■\t"); printf("    ■■■■■■■■\t");     printf("      ■■■■■■■■\t"); printf("        ■■■■■■■■\n");
	printf("■            ■\t"); printf("  ■            ■\t"); printf("    ■            ■\t");     printf("      ■            ■\t"); printf("        ■            ■\n");
	printf("■"); textcolor(chkcolor(arr[5], ROOMNAME1, ROOMBACK1)); printf("6번(% 6s)", chkname(arr[5])); textcolor(ROOMBACK1); printf(" ■\t  ■");  textcolor(chkcolor(arr[6], ROOMNAME1, ROOMBACK1)); printf("7번(% 6s)", chkname(arr[6])); textcolor(ROOMBACK1);    printf(" ■\t    ■"); textcolor(chkcolor(arr[7], ROOMNAME1, ROOMBACK1)); printf("8번(% 6s)", chkname(arr[7])); textcolor(ROOMBACK1); printf(" ■\t      ■"); textcolor(chkcolor(arr[8], ROOMNAME1, ROOMBACK1)); printf("9번(% 6s)", chkname(arr[8])); textcolor(ROOMBACK1); printf(" ■\t        ■"); textcolor(chkcolor(arr[9], ROOMNAME1, ROOMBACK1)); printf("10번(% 6s)", chkname(arr[9])); textcolor(ROOMBACK1); printf("■\n");
	//printf("■6번(%6s) ■\t", chkname(arr[5])); printf("  ■7번(%6s) ■\t", chkname(arr[6])); printf("    ■8번(%6s) ■\t", chkname(arr[7]));     printf("      ■9번(%6s) ■\t", chkname(arr[8])); printf("        ■10번(%6s)■\n", chkname(arr[9]));
	printf("■            ■\t"); printf("  ■            ■\t"); printf("    ■            ■\t");     printf("      ■            ■\t"); printf("        ■            ■\n");
	printf("■■■■■■■■\t"); printf("  ■■■■■■■■\t"); printf("    ■■■■■■■■\t");     printf("      ■■■■■■■■\t"); printf("        ■■■■■■■■\n\n\n\n");


	printf("■■■■■■■■          "); printf("■■■■■■■■          "); printf("■■■■■■■■          ");     printf("■■■■■■■■          "); printf("■■■■■■■■\n");
	printf("■            ■          "); printf("■            ■          "); printf("■            ■          ");     printf("■            ■          "); printf("■            ■\n");
	printf("■"); textcolor(chkcolor(arr[10], ROOMNAME1, ROOMBACK1)); printf("11번(% 6s)", chkname(arr[10])); textcolor(ROOMBACK1); printf("■\t  ■");  textcolor(chkcolor(arr[11], ROOMNAME1, ROOMBACK1)); printf("12번(% 6s)", chkname(arr[11])); textcolor(ROOMBACK1);    printf("■\t    ■"); textcolor(chkcolor(arr[12], ROOMNAME1, ROOMBACK1)); printf("13번(% 6s)", chkname(arr[12])); textcolor(ROOMBACK1); printf("■\t      ■"); textcolor(chkcolor(arr[13], ROOMNAME1, ROOMBACK1)); printf("14번(% 6s)", chkname(arr[13])); textcolor(ROOMBACK1); printf("■\t        ■"); textcolor(chkcolor(arr[14], ROOMNAME1, ROOMBACK1)); printf("15번(% 6s)", chkname(arr[14])); textcolor(ROOMBACK1); printf("■\n");
	//printf("■11번(%6s)■          ", chkname(arr[10])); printf("■12번(%6s)■          ", chkname(arr[11])); printf("■13번(%6s)■          ", chkname(arr[12]));     printf("■14번(%6s)■          ", chkname(arr[13])); printf("■15번(%6s)■\n", chkname(arr[14]));
	printf("■            ■          "); printf("■            ■          "); printf("■            ■          ");     printf("■            ■          "); printf("■            ■\n");
	printf("■■■■■■■■          "); printf("■■■■■■■■          "); printf("■■■■■■■■          ");     printf("■■■■■■■■          "); printf("■■■■■■■■\n\n\n\n");


	printf("■■■■■■■■          "); printf("■■■■■■■■          "); printf("■■■■■■■■          ");     printf("■■■■■■■■          "); printf("■■■■■■■■\n");
	printf("■            ■          "); printf("■            ■          "); printf("■            ■          ");     printf("■            ■          "); printf("■            ■\n");
	printf("■"); textcolor(chkcolor(arr[15], ROOMNAME1, ROOMBACK1)); printf("16번(% 6s)", chkname(arr[15])); textcolor(ROOMBACK1); printf("■\t  ■");  textcolor(chkcolor(arr[16], ROOMNAME1, ROOMBACK1)); printf("17번(% 6s)", chkname(arr[16])); textcolor(ROOMBACK1);    printf("■\t    ■"); textcolor(chkcolor(arr[17], ROOMNAME1, ROOMBACK1)); printf("18번(% 6s)", chkname(arr[17])); textcolor(ROOMBACK1); printf("■\t      ■"); textcolor(chkcolor(arr[18], ROOMNAME1, ROOMBACK1)); printf("19번(% 6s)", chkname(arr[18])); textcolor(ROOMBACK1); printf("■\t        ■"); textcolor(chkcolor(arr[19], ROOMNAME1, ROOMBACK1)); printf("20번(% 6s)", chkname(arr[19])); textcolor(ROOMBACK1); printf("■\n");
	//printf("■16번(%6s)■          ", chkname(arr[15])); printf("■17번(%6s)■          ", chkname(arr[16])); printf("■18번(%6s)■          ", chkname(arr[17]));     printf("■19번(%6s)■          ", chkname(arr[18])); printf("■20번(%6s)■\n", chkname(arr[19]));
	printf("■            ■          "); printf("■            ■          "); printf("■            ■          ");     printf("■            ■          "); printf("■            ■\n");
	printf("■■■■■■■■          "); printf("■■■■■■■■          "); printf("■■■■■■■■          ");     printf("■■■■■■■■          "); printf("■■■■■■■■\n\n\n");


	textcolor(15);
	printf("\t\t\t\t\t\t [뒤]\n");
}

//2번방출력
void showroom2(char arr[][10]) {
	textcolor(ROOMBACK2);
	printf("    ■■■■■■■■ ■■■■■■■■■                          ■■■■■■■■■ ■■■■■■■■\n");
	printf("    ■            ■ ■              ■                          ■              ■ ■            ■\n");
	printf("    ■ "); textcolor(chkcolor(arr[0], ROOMNAME2, ROOMBACK2)); printf("1번(% 6s)", chkname(arr[0])); textcolor(ROOMBACK2); printf("■ ■              ■                          ■              ■ ■"); textcolor(chkcolor(arr[18], ROOMNAME2, ROOMBACK2)); printf("19번(% 6s)", chkname(arr[18])); textcolor(ROOMBACK2);
	printf("■\n    ■            ■ ■              ■                          ■              ■ ■            ■\n");
	printf("    ■■■■■■■■ ■              ■                          ■              ■ ■■■■■■■■\n");
	printf("                     ■              ■                          ■              ■                 \n");
	printf("    ■■■■■■■■ ■              ■                          ■              ■ ■■■■■■■■\n");
	printf("    ■            ■ ■              ■                          ■              ■ ■            ■\n");
	printf("    ■ "); textcolor(chkcolor(arr[1], ROOMNAME2, ROOMBACK2)); printf("2번(% 6s)", chkname(arr[1])); textcolor(ROOMBACK2); printf("■ ■              ■                          ■              ■ ■"); textcolor(chkcolor(arr[17], ROOMNAME2, ROOMBACK2)); printf("18번(% 6s)", chkname(arr[17])); textcolor(ROOMBACK2);
	//printf("    ■ 2번(%6s)■ ■              ■                          ■              ■ ■18번(%6s)■\n", chkname(arr[1]), chkname(arr[17]));
	printf("■\n    ■            ■ ■              ■                          ■              ■ ■            ■\n");
	printf("    ■■■■■■■■ ■              ■                          ■              ■ ■■■■■■■■\n");
	printf("                     ■              ■                          ■              ■           \n");
	printf("    ■■■■■■■■ ■              ■                          ■              ■ ■■■■■■■■\n");
	printf("    ■            ■ ■              ■                          ■              ■ ■            ■\n");
	printf("    ■ "); textcolor(chkcolor(arr[2], ROOMNAME2, ROOMBACK2)); printf("3번(% 6s)", chkname(arr[2])); textcolor(ROOMBACK2); printf("■ ■              ■                          ■              ■ ■"); textcolor(chkcolor(arr[16], ROOMNAME2, ROOMBACK2)); printf("17번(% 6s)", chkname(arr[16])); textcolor(ROOMBACK2);
	//printf("    ■ 3번(%6s)■ ■              ■                          ■              ■ ■17번(%6s)■\n", chkname(arr[2]), chkname(arr[16]));
	printf("■\n    ■            ■ ■              ■                          ■              ■ ■            ■\n");
	printf("    ■■■■■■■■ ■              ■                          ■              ■ ■■■■■■■■\n");
	printf("                     ■              ■                          ■              ■           \n");
	printf("    ■■■■■■■■ ■              ■                          ■              ■ ■■■■■■■■\n");
	printf("    ■            ■ ■              ■                          ■              ■ ■            ■\n");
	printf("    ■ "); textcolor(chkcolor(arr[3], ROOMNAME2, ROOMBACK2)); printf("4번(% 6s)", chkname(arr[3])); textcolor(ROOMBACK2); printf("■ ■              ■                          ■              ■ ■"); textcolor(chkcolor(arr[15], ROOMNAME2, ROOMBACK2)); printf("16번(% 6s)", chkname(arr[15])); textcolor(ROOMBACK2);
	//printf("    ■ 4번(%6s)■ ■              ■                          ■              ■ ■16번(%6s)■\n", chkname(arr[3]), chkname(arr[15]));
	printf("■\n    ■            ■ ■              ■                          ■              ■ ■            ■\n");
	printf("    ■■■■■■■■ ■              ■                          ■              ■ ■■■■■■■■\n");
	printf("                     ■              ■                          ■              ■           \n");
	printf("    ■■■■■■■■ ■              ■                          ■              ■ ■■■■■■■■\n");
	printf("    ■            ■ ■              ■                          ■              ■ ■            ■\n");
	printf("    ■ "); textcolor(chkcolor(arr[4], ROOMNAME2, ROOMBACK2)); printf("5번(% 6s)", chkname(arr[4])); textcolor(ROOMBACK2); printf("■ ■              ■                          ■              ■ ■"); textcolor(chkcolor(arr[14], ROOMNAME2, ROOMBACK2)); printf("15번(% 6s)", chkname(arr[14])); textcolor(ROOMBACK2);
	//printf("    ■ 5번(%6s)■ ■              ■                          ■              ■ ■15번(%6s)■\n", chkname(arr[4]), chkname(arr[14]));
	printf("■\n    ■            ■ ■              ■                          ■              ■ ■            ■\n");
	printf("    ■■■■■■■■ ■              ■                          ■              ■ ■■■■■■■■\n");
	printf("                     ■              ■                          ■              ■                 \n");
	printf("    ■■■■■■■■ ■              ■                          ■              ■ ■■■■■■■■\n");
	printf("    ■            ■ ■              ■                          ■              ■ ■            ■\n");
	printf("    ■ "); textcolor(chkcolor(arr[5], ROOMNAME2, ROOMBACK2)); printf("6번(% 6s)", chkname(arr[5])); textcolor(ROOMBACK2); printf("■ ■              ■                          ■              ■ ■"); textcolor(chkcolor(arr[13], ROOMNAME2, ROOMBACK2)); printf("14번(% 6s)", chkname(arr[13])); textcolor(ROOMBACK2);
	//printf("    ■ 6번(%6s)■ ■              ■                          ■              ■ ■14번(%6s)■\n", chkname(arr[5]), chkname(arr[13]));
	printf("■\n    ■            ■ ■              ■                          ■              ■ ■            ■\n");
	printf("    ■■■■■■■■ ■               ■                        ■               ■ ■■■■■■■■\n");
	printf("                     ■                ■                      ■                ■           \n");
	printf("    ■■■■■■■■ ■                  ■                  ■                  ■ ■■■■■■■■\n");
	printf("    ■            ■ ■                   ■               ■                    ■ ■            ■\n");
	printf("    ■ "); textcolor(chkcolor(arr[6], ROOMNAME2, ROOMBACK2)); printf("7번(% 6s)", chkname(arr[6])); textcolor(ROOMBACK2); printf("■ ■                     ■■■■■■■                       ■ ■"); textcolor(chkcolor(arr[12], ROOMNAME2, ROOMBACK2)); printf("13번(% 6s)", chkname(arr[12])); textcolor(ROOMBACK2);
	//printf("    ■ 7번(%6s)■ ■                     ■■■■■■■                       ■ ■13번(%6s)■\n", chkname(arr[6]), chkname(arr[12]));
	printf("■\n    ■            ■ ■                                                          ■ ■            ■\n");
	printf("    ■■■■■■■■  ■                                                        ■  ■■■■■■■■\n");
	printf("                       ■                                                      ■\n");
	printf("                         ■                                                  ■\n");
	printf("                          ■                                                ■\n");
	printf("         ■■■■■■■■   ■                                             ■  ■■■■■■■■\n");
	printf("         ■            ■    ■                                          ■    ■            ■\n         ■ ");
	textcolor(chkcolor(arr[7], ROOMNAME2, ROOMBACK2)); printf("8번(%6s)", chkname(arr[7])); textcolor(ROOMBACK2); printf("■      ■                                      ■      ■"); textcolor(chkcolor(arr[11], ROOMNAME2, ROOMBACK2)); printf("12번(% 6s)", chkname(arr[11])); textcolor(ROOMBACK2);
	//printf("         ■ 8번(%6s)■      ■                                      ■      ■12번(%6s)■\n", chkname(arr[7]), chkname(arr[11]));
	printf("■\n         ■            ■        ■                                  ■        ■            ■\n");
	printf("         ■■■■■■■■          ■■■■■■■■■■■■■■■■■          ■■■■■■■■\n");
	printf("		                                                                                          \n");
	printf("                  ■■■■■■■■         ■■■■■■■■         ■■■■■■■■\n");
	printf("                  ■            ■         ■            ■         ■            ■\n                  ■ ");
	textcolor(chkcolor(arr[8], ROOMNAME2, ROOMBACK2)); printf("9번(%6s)", chkname(arr[8])); textcolor(ROOMBACK2); printf("■         ■"); textcolor(chkcolor(arr[9], ROOMNAME2, ROOMBACK2)); printf("10번(% 6s)", chkname(arr[9])); textcolor(ROOMBACK2); printf("■         ■");  textcolor(chkcolor(arr[10], ROOMNAME2, ROOMBACK2)); printf("11번(% 6s)", chkname(arr[10])); textcolor(ROOMBACK2);
	//printf("                  ■ 9번(%6s)■         ■10번(%6s)■         ■11번(%6s)■\n", chkname(arr[8]), chkname(arr[9]), chkname(arr[10]));
	printf("■\n                  ■            ■         ■            ■         ■            ■\n");
	printf("                  ■■■■■■■■         ■■■■■■■■         ■■■■■■■■\n\n");
	textcolor(15);
	printf("                                       [뒤]       \n");
	//커서를 맨위로올려서 화면이동
	gotoxy(0, 0);
	printf(" ");

}

//3번방출력
void showroom3(char arr[][10]) {
	textcolor(ROOMBACK3);
	printf("         ■■■■■■■■■   ■■■■■■■■■\t");                                    printf("■■■■■■■■■  ■■■■■■■■■  \t");                                       printf("■■■■■■■■■  ■■■■■■■■■\n");
	printf("         ■              ■   ■              ■\t");                                    printf("■              ■  ■              ■  \t");                                       printf("■              ■  ■              ■\n         ■ ");
	textcolor(chkcolor(arr[0], ROOMNAME3, ROOMBACK3)); printf("1번(%6s)", chkname(arr[0])); textcolor(ROOMBACK3);	printf("  ■   ■ "); textcolor(chkcolor(arr[1], ROOMNAME3, ROOMBACK3)); printf("2번(% 6s)", chkname(arr[1])); textcolor(ROOMBACK3);     printf("  ■\t■ "); textcolor(chkcolor(arr[2], ROOMNAME3, ROOMBACK3)); printf(" 3번(% 6s)", chkname(arr[2])); textcolor(ROOMBACK3); printf(" ■  ■ "); textcolor(chkcolor(arr[3], ROOMNAME3, ROOMBACK3)); printf(" 4번(% 6s)", chkname(arr[3])); textcolor(ROOMBACK3);              printf(" ■  \t■ "); textcolor(chkcolor(arr[4], ROOMNAME3, ROOMBACK3)); printf("5번(% 6s)", chkname(arr[4])); textcolor(ROOMBACK3);  printf("  ■  ■  "); textcolor(chkcolor(arr[5], ROOMNAME3, ROOMBACK3)); printf("6번(% 6s)", chkname(arr[5])); textcolor(ROOMBACK3);
	//printf("         ■ 1번(%6s)  ■   ■ 2번(%6s)  ■\t", chkname(arr[0]), chkname(arr[1]));     printf("■ 3번(%6s)  ■  ■ 4번(%6s)  ■  \t", chkname(arr[2]), chkname(arr[3]));              printf("■ 5번(%6s)  ■  ■  6번(%6s) ■\n", chkname(arr[4]), chkname(arr[5]));
	printf(" ■\n         ■              ■   ■              ■\t");                                    printf("■              ■  ■              ■  \t");                                       printf("■              ■  ■              ■\n");
	printf("         ■■■■■■■■■   ■■■■■■■■■\t");                                    printf("■■■■■■■■■  ■■■■■■■■■  \t");                                       printf("■■■■■■■■■  ■■■■■■■■■\n\n");

	printf("         ■■■■■■■■■   ■■■■■■■■■\t");                                    printf("■■■■■■■■■  ■■■■■■■■■  \t");                                       printf("■■■■■■■■■  ■■■■■■■■■\n");
	printf("         ■              ■   ■              ■\t");                                    printf("■              ■  ■              ■  \t");                                       printf("■              ■  ■              ■\n         ■ ");
	textcolor(chkcolor(arr[6], ROOMNAME3, ROOMBACK3)); printf("7번(%6s)", chkname(arr[6])); textcolor(ROOMBACK3);	printf("  ■   ■ "); textcolor(chkcolor(arr[7], ROOMNAME3, ROOMBACK3)); printf("8번(% 6s)", chkname(arr[7])); textcolor(ROOMBACK3);     printf("  ■\t■ "); textcolor(chkcolor(arr[8], ROOMNAME3, ROOMBACK3)); printf(" 9번(% 6s)", chkname(arr[8])); textcolor(ROOMBACK3); printf(" ■  ■ "); textcolor(chkcolor(arr[9], ROOMNAME3, ROOMBACK3)); printf("10번(% 6s)", chkname(arr[9])); textcolor(ROOMBACK3);              printf(" ■  \t■ "); textcolor(chkcolor(arr[10], ROOMNAME3, ROOMBACK3)); printf("11번(% 6s)", chkname(arr[10])); textcolor(ROOMBACK3); printf(" ■  ■ "); textcolor(chkcolor(arr[11], ROOMNAME3, ROOMBACK3)); printf("12번(% 6s)", chkname(arr[11])); textcolor(ROOMBACK3);
	//printf("         ■ 7번(%6s)  ■   ■ 8번(%6s)  ■\t", chkname(arr[6]), chkname(arr[7]));     printf("■ 9번(%6s)  ■  ■10번(%6s)  ■  \t", chkname(arr[8]), chkname(arr[9]));              printf("■11번(%6s)  ■  ■ 12번(%6s) ■\n", chkname(arr[10]), chkname(arr[11]));
	printf(" ■\n         ■              ■   ■              ■\t");                                    printf("■              ■  ■              ■  \t");                                       printf("■              ■  ■              ■\n");
	printf("         ■■■■■■■■■   ■■■■■■■■■\t");                                    printf("■■■■■■■■■  ■■■■■■■■■  \t");                                       printf("■■■■■■■■■  ■■■■■■■■■\n\n");

	printf("         ■■■■■■■■■   ■■■■■■■■■\t");                                    printf("■■■■■■■■■  ■■■■■■■■■  \t");                                       printf("■■■■■■■■■  ■■■■■■■■■\n");
	printf("         ■              ■   ■              ■\t");                                    printf("■              ■  ■              ■  \t");                                       printf("■              ■  ■              ■\n         ■ ");
	textcolor(chkcolor(arr[12], ROOMNAME3, ROOMBACK3)); printf("13번(%6s)", chkname(arr[12])); textcolor(ROOMBACK3);	printf(" ■   ■ "); textcolor(chkcolor(arr[13], ROOMNAME3, ROOMBACK3)); printf("14번(% 6s)", chkname(arr[13])); textcolor(ROOMBACK3);     printf(" ■\t■ "); textcolor(chkcolor(arr[14], ROOMNAME3, ROOMBACK3)); printf("15번(% 6s)", chkname(arr[14])); textcolor(ROOMBACK3); printf(" ■  ■ "); textcolor(chkcolor(arr[15], ROOMNAME3, ROOMBACK3)); printf("16번(% 6s)", chkname(arr[15])); textcolor(ROOMBACK3);              printf(" ■  \t■ "); textcolor(chkcolor(arr[16], ROOMNAME3, ROOMBACK3)); printf("17번(% 6s)", chkname(arr[16])); textcolor(ROOMBACK3); printf(" ■  ■ "); textcolor(chkcolor(arr[17], ROOMNAME3, ROOMBACK3)); printf("18번(% 6s)", chkname(arr[17])); textcolor(ROOMBACK3);
	//printf("         ■13번(%6s)  ■   ■ 14번(%6s) ■\t", chkname(arr[12]), chkname(arr[13]));     printf("■15번(%6s)  ■  ■ 16번(%6s) ■  \t", chkname(arr[14]), chkname(arr[15]));              printf("■17번(%6s)  ■  ■ 18번(%6s) ■\n", chkname(arr[16]), chkname(arr[17]));
	printf(" ■\n         ■              ■   ■              ■\t");                                    printf("■              ■  ■              ■  \t");                                       printf("■              ■  ■              ■\n");
	printf("         ■■■■■■■■■   ■■■■■■■■■\t");                                    printf("■■■■■■■■■  ■■■■■■■■■  \t");                                       printf("■■■■■■■■■  ■■■■■■■■■\n\n\n");
	textcolor(15);
	printf("\t\t\t\t\t\t [뒤]\n");
}

//디자인없는 자리데이터전체 출력
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

//문자열복사
void copy(char target[], char arr[]) {
	//이름의 길이인 7칸만복사
	for (int i = 0; i < 7; i++) {
		target[i] = arr[i];
	}
}

//파일 저장상황 기록용
void showlog(int a, char massage[]) {
	system("cls");
	printf("%d : %s\n", a, massage);
}

//자리데이터에 이름이있는지 확인
int check(char name[], char nameList[][10]) {
	//있으면 0반환,없으면1반환
	int r = 1;
	for (int i = 0; i < 60; i++) {
		if (compare(name, nameList[i])) {
			r = 0;
			break;
		}
	}
	return r;
}

//문자열비교
int compare(char name1[], char name2[]) {
	//같으면 1, 다르면 0반환
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

//문자열초기화
void clear(char arr[]) {
	//null문자를 만날때까지 null로 초기화
	int i = 0;
	while (arr[i] != 0) {
		arr[i] = 0;
		i++;
	}
}

//자리데이터에서 특정이름의 좌표반환
int findpos(char name[], char nameList[][10]) {
	//찾을때까지 탐지하고 인덱스반환, 없으면 -1반환
	for (int i = 0; i < 60; i++) {
		if (compare(name, nameList[i])) {
			return i;
		}
	}
	return -1;
}

//파일저장
void save(FILE* fp, char nameList[][10]) {
	//한자리씩 저장하며 로그출력
	for (int i = 0; i < 60; i++) {
		if (nameList[i][0] == '0') {
			showlog(i, "비어있는 자리저장중...");
			fputs("0 ", fp);
		}
		else {
			int j = 0;
			showlog(i, "예약된 자리저장중...");
			while (nameList[i][j] != 0) {
				fputc(nameList[i][j], fp);
				j++;
			}
			fputc(' ', fp);
		}
	}
	showlog(60, "EOF 입력중...");
	fputs("~~~~~", fp);
	showlog(61, "파일 닫는중...");
	fclose(fp);
	showlog(62, "저장완료");
}

//현재입력된키와 keycode비교
int getkey(char keycode) {
	if (_kbhit()) {
		if (_getch() == keycode) {
			return 1;
		}
	}
	return 0;
}

//입력받은 값에 특정값을 더해서 반환
int input(char string[], int add) {
	//파이썬input함수에 가중치추가
	int i;
	printf("%s", string);
	scanf("%d", &i);
	return i + add;
}

//메뉴인덱스 입력
int indexinput(int index) {
	//메뉴밖으로 나가지않도록 제한, d를누르면 -2반환
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

//정보출력
void printHelp() {
	printf("\n기숙사 자습 자리관리 프로그램\n\n");
	printf("by 강지운,김하온\n\n\n");
	printf("사용법\n\n");
	printf("위쪽화살표 : 위로 이동\n");
	printf("아래쪽화살표 : 아래로 이동\n");
	printf("엔터 : 선택\n\n");
	printf("이름을 작성하면,\n");
	printf("이미 있을때 : 자리이동 또는 취소\n");
	printf("새로운 이름일때 : 자리선택\n\n");
	printf("종료할때 X버튼을 눌러 종료하면 ");
	textcolor(4);
	printf("저장");
	textcolor(15);
	printf("이 안됩니다!!");
}

//종료창출력
void printExit(char nameList[][10], char originNameList[][10]) {
	printf("\n현재 자리정보를 저장하고 종료합니다.\n\n\n");
	printf("변경된자리\n");
	show(nameList);
	printf("\n\n저장된자리\n");
	show(originNameList);
}

//관리자모드 명령어실행
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
			//읽은 데이터 정리(분할)
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
			//읽은 데이터 정리(분할)
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
	//화면 크기,제목,커서설정
	system("mode con:cols=170 lines=80 | title SelfStudy");
	CursorView();
	//---------------파일읽고 데이터 정리----------------
	char filetxt[100000] = "", nameList[1000][10] = { 0 }, originNameList[1000][10] = { 0 };
	int i = 0, j = 0, k = 0;
	//파일 열고 데이터 읽기
	FILE* fp;
	fp = fopen("data.txt", "r");
	//파일 없으면 새로생성
	if (fp == NULL) {
		fp = fopen("data.txt", "w");
		fputs("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ~~~~~", fp);
		fclose(fp);
		fp = fopen("data.txt", "r");
	}
	fread(filetxt, 1, 100000, fp);
	fclose(fp);
	//읽은 데이터 정리(분할)
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
	//---------------------실행문------------------------
	char name[10] = { 0 }, command[100] = "";
	int roomnumber = 0, chairnumber = 0, isrunning, enterkey = 0, manuIndex = 0, keycode = 0;
	//초기화면
	showUI(manuIndex);
	printHelp();
	//초기입력
	while (keycode != -2) {
		//메뉴이동입력
		keycode = indexinput(manuIndex);
		if (keycode != -2) {
			manuIndex += keycode;
		}
		//메뉴출력
		system("cls");
		showUI(manuIndex);
		//인덱스에따라 내용출력
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
	//관리자모드선택 또는 종료선택시 실행안함
	isrunning = manuIndex != 4 && manuIndex != 0;
	//종료를 선택할때까지 반복
	while (isrunning == 1) {
		//현재 메뉴에서 선택된방으로 방설정
		roomnumber = manuIndex - 1;
		//이름비우고 입력받기
		clear(name);
		name[0] = '0';
		//2실일경우 커서를 아래쪽으로이동
		if (roomnumber == 1) {
			gotoxy(0, 75);
			gotoxy(0, 62);
		}
		printf("이름 : ");
		scanf("%s", name);
		//이름이 없었으면
		if (check(name, nameList)) {
			//비어있는자리를 고를때까지 계속물어보기
			do {
				chairnumber = input("좌석 번호 : ", -1);
			} while (nameList[roomnumber * 20 + chairnumber][0] != '0');
			//자리에 이름복사
			copy(nameList[roomnumber * 20 + chairnumber], name);
		}
		//이름이 이미있으면
		else {
			int mode, pos;
			//올바른 모드 선택할때까지 계속물어보기
			do {
				mode = input("1 : 취소\n2 : 이동\n선택 : ", 0);
			} while (mode != 1 && mode != 2);
			//방 데이터에서 입력한 이름좌표찾기
			pos = findpos(name, nameList);
			//취소선택시
			if (mode == 1) {
				//원래자리 지우기
				clear(nameList[pos]);
				nameList[pos][0] = '0';
			}
			//이동선택시
			else if (mode == 2) {
				do {
					chairnumber = input("좌석 번호 : ", -1);
					//이동할자리가 비어있을때까지 물어보기
				} while (nameList[roomnumber * 20 + chairnumber][0] != '0');
				//이동할자리에 이름채우기
				copy(nameList[roomnumber * 20 + chairnumber], name);
				//원래자리에 이름지우기
				clear(nameList[pos]);
				nameList[pos][0] = '0';
			}
		}
		//정보창으로 돌아가기
		manuIndex = 0;
		system("cls");
		showUI(manuIndex);
		printHelp();
		keycode = 0;
		//다시물어보기
		while (keycode != -2 || manuIndex == 0) {
			//메뉴이동입력
			keycode = indexinput(manuIndex);
			if (keycode != -2) {
				manuIndex += keycode;
			}
			//메뉴출력
			system("cls");
			showUI(manuIndex);
			//인덱스에따라 내용출력
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
		//관리자모드선택 또는 종료선택시 반복안함
		isrunning = manuIndex != 4 && manuIndex != 0;
	}
	//관리자모드
	if (manuIndex == 0) {
		commandRun(nameList);
	}
	//프로그램 종료시 파일저장
	fp = fopen("data.txt", "w");
	save(fp, nameList);
	fclose(fp);
	char savingName[50]="";
	//로그 파일이름 문자열 생성
	sprintf(savingName, "log %d,%d,%d %d,%d,%d.txt\0", getTime(YEAR), getTime(MONTH), getTime(DAY), getTime(HOUR), getTime(MINUITE), getTime(SECOND));
	//로그저장
	fp = fopen(savingName, "w");
	save(fp, nameList);
	fclose(fp);
	printf("%s\n",savingName);
	return 0;
}