#include<list>
#include<graphics.h>
#include<conio.h>
#include<cstdio>
#include<time.h>
#include<mmsystem.h>
#include<cmath>
#include <windows.h> 
#include <iostream>
#include <algorithm>

#pragma comment (lib,"winmm.lib")
#undef _UNICODE#undef UNICODE


#define MAPH 11
#define MAPW 15
#define MAX 30
#define MAXFOOD3 30//最大食物数量
#define MAXFOOD2 20//最大食物数量
#define MAXFOOD1 10//最大食物数量
#define MAXFOOD4 40//最大食物数量
#define TRAP1 15//最大陷阱数量
#define TRAP2 20//最大陷阱数量
#define TRAP3 25//最大陷阱数量
#define TRAP4 30//最大陷阱数量
#define SLENGTH 10 //设置方格边长为10
#define DLENGTH 14 //设置方格对角线长度为14
#define BP 10 //设置绘图起点为10
#define PGL 50 //设置绘图区域方格边长为50
#define W 1 //为了便于观察不覆盖方格边界线，设置1个像素的间隔
#define R 15 //绘制的圆半径为15个像素

using namespace std;
int fen = 0;
int NumIndex = 0;
int num1 = 2;//0 下 1 左 2 右 3 上
int bossnumindex = 0;
int bossnum1 = 0;//0 左 上 1 右 下
int x = BP, y = BP;
int bossx=900, bossy=100;
int bossx1=900, bossy1 = 100;
int goal = 1;
int BC = 10; //人类步长
int timeiff = 0;
int waittime = 0;
int waittime1 = 0;
int usetime = -1;
int usetime1 = -1;
int jifen1 = 2000;
int jifen2 = 3000;
int jifen3 = 4000;
int jifen4 = 6000;
int TL = 3;
int HP = 7;
int detime = 0;//设置存储当前时间的变量
int tishi = 0;
clock_t start, timeend;

typedef enum
{
	BLANK, BEGIN, END, WALL, ROAD, FOOD, FASTFOOD, BOSS1, BOSS2, BOX, HANDRAIL,
	TRAP
}TYPE;
typedef struct GRID
{
	int x;
	int y;
	TYPE type;
	int G;
	int F;
	int H;
	struct GRID* prev;
}GRID, * PGRID;
typedef struct MYTIME
{
	int second;
	int hour;
	int minute;
}MYTIME;
typedef struct MYROAD
{
	int r[MAPH * MAPW];
	int length;//r数组的长度
	int count; //需要的总步数
}MYROAD;
//不懂这步
const TCHAR str[10][2] = { _T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"),
_T("6"), _T("7"), _T("8"), _T("9") };
IMAGE bkimg, player1, player2, win, lose, wall1,
wall2, food1, food2, gamebk, boss1, boss2, fastfood1, fastfood2, pchoose,
pend, scenario, langan1, langan2, box1, box2, trap1, trap2,roundold1,roundold2,roundnew1,
roundnew2,flower1,flower2,story1, story2, story3, story4, story5, story6, story7, story8, story9;
void choose(PGRID pmap);
void fail1(PGRID pmap);
void win1(int x, PGRID pmap);
void fail2(PGRID pmap);
void win2(int x, PGRID pmap);
void win3(int x, PGRID pmap);
void fail3(PGRID pmap);
void win4(int x, PGRID pmap);
void fail4(PGRID pmap);
void imgresouse()
{
	loadimage(&bkimg, _T("bkimg.jpg"));
	loadimage(&player1, _T("boy1.jpg"));
	loadimage(&player2, _T("boy2.jpg"));
	loadimage(&win, _T("胜利.jpg"), 1000, 563);
	loadimage(&lose, _T("失败界面.jpg"), 1000, 563);
	loadimage(&food1, _T("果子1.jpg"));
	loadimage(&food2, _T("果子2.jpg"));
	loadimage(&wall1, _T("障碍1.jpg"));
	loadimage(&wall2, _T("障碍2.jpg"));
	loadimage(&gamebk, _T("草地.png"), 750, 550);
	loadimage(&boss1, _T("boss1.jpg"));
	loadimage(&boss2, _T("boss2.jpg"));
	loadimage(&fastfood1, _T("fastfood1.jpg"));
	loadimage(&fastfood2, _T("fastfood2.jpg"));
	loadimage(&pchoose, _T("选择.png"), 1000, 563);
	loadimage(&pend, _T("通关.jpg"), 1000, 563);
	loadimage(&scenario, _T("剧情.jpg"), 1000, 563);
	loadimage(&langan1, _T("荆棘.png"), PGL, PGL);
	loadimage(&langan2, _T("荆棘黑.png"), PGL, PGL);
	loadimage(&box1, _T("宝箱.png"), PGL, PGL);
	loadimage(&box2, _T("宝箱黑.png"), PGL, PGL);
	loadimage(&trap1, _T("陷阱.png"), PGL, PGL);
	loadimage(&trap2, _T("陷阱黑.png"), PGL, PGL);
	loadimage(&roundold1, _T("传送阵原始.png"), PGL, PGL);
	loadimage(&roundold2, _T("传送阵原始 黑.png"), PGL, PGL);
	loadimage(&roundnew1, _T("传送阵新.png"), PGL, PGL);
	loadimage(&roundnew2, _T("传送阵新黑.png"), PGL, PGL);
	loadimage(&flower1, _T("花1.jpg"), PGL, PGL);
	loadimage(&flower2, _T("花2.jpg"), PGL, PGL);
	loadimage(&story1, _T("story1.png"), 1000, 563);
	loadimage(&story2, _T("story2.png"), 1000, 563);
	loadimage(&story3, _T("story3.png"), 1000, 563);
	loadimage(&story4, _T("story4.png"), 1000, 563);
	loadimage(&story5, _T("story5.png"), 1000, 563);
	loadimage(&story6, _T("story6.png"), 1000, 563);
	loadimage(&story7, _T("story7.png"), 1000, 563);
	loadimage(&story8, _T("story8.png"), 1000, 563);
	loadimage(&story9, _T("story9.png"), 1000, 563);
	mciSendString(_T("open ./bottom.mp3 alias bottom"), NULL, 0, NULL);
	mciSendString(_T("open ./begingame.mp3 alias begingame"), NULL, 0, NULL);
	mciSendString(_T("open ./win.mp3 alias win"), NULL, 0, NULL);
	mciSendString(_T("open ./lose.mp3 alias lose"), NULL, 0, NULL);
}
void drawwelcome()
{
	mciSendString(_T("play begingame"), NULL, 0, NULL);
	while (1)
	{
		MOUSEMSG m;  //定义鼠标信息结构体变量m
		TCHAR str[5];
		LOGFONT f;
		gettextstyle(&f);
		settextcolor(BLACK);
		f.lfHeight = 20;
		f.lfWidth = 15;
		settextstyle(&f);
		setfillcolor(RGB(153, 122, 49));
		setlinestyle(PS_SOLID, 2); //定义像素为2 的实线
		m = GetMouseMsg(); //获得鼠标信息
		if (m.x < 700 && m.x >300 && m.y < 495 && m.y > 435)
		{
			setlinecolor(RGB(105, 70, 32));
			if (m.uMsg == WM_LBUTTONDOWN)//如果点击左键
			{
				//显示进度条
				for (int i = 200; i <= 700; i++)
				{
					BeginBatchDraw();
					solidrectangle(i, 340, i + 10, 360);
					_sntprintf_s(str, 60, _T("%d%c"), (i - 200) / 5, '%');
					solidrectangle(700, 340, 760, 360);
					outtextxy(700, 340, str);
					rectangle(200, 340, 760, 360);
					Sleep(5);
					EndBatchDraw();
				}
				Sleep(500);
				break;
			}
		}
		else
		{
			putimage(0, 0, &bkimg);
		}
	}
}
void initmap(PGRID pmap)
{
	int i, j;
	for (i = 0; i < MAPH; i++)
	{
		for (j = 0; j < MAPW; j++)
		{
			pmap[i * MAPW + j].x = j;
			pmap[i * MAPW + j].y = i;
			pmap[i * MAPW + j].type = BLANK;
			pmap[i * MAPW + j].F = 0;
			pmap[i * MAPW + j].G = 0;
			pmap[i * MAPW + j].H = 0;
			pmap[i * MAPW + j].prev = NULL;
		}
	}
}
void player(PGRID pmap)
{
	pmap[0].type = BEGIN;
	pmap[MAPH * MAPW - 1].type = BLANK;
	pmap[MAPW - 1].type = BOSS1;
	pmap[MAPW * (MAPH - 1)].type = BOSS2;
	int k;
	int l;
	for (k = 0; k < MAPH; k++)
	{
		for (l = 0; l < MAPW; l++)
		{
			pmap[k * MAPW + l].H = (abs(pmap[k * MAPW + l].x - pmap[MAPH * MAPW - 1].x) + abs(pmap[k * MAPW + l].y - pmap[MAPH * MAPW - 1].y)) * SLENGTH;
		}
	}
}
int score(int fen)
{
	fen++;
	return fen;
}
void createwall(PGRID pmap)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < MAX; i++)
	{
		int r = rand() % (MAPH * MAPW);
		if (pmap[r].type == END || pmap[r].type == BEGIN || 
			pmap[r].type == WALL || pmap[r].type == BOSS1 || 
			pmap[r].type == BOSS2||pmap[r].type==TRAP|| r == MAPH * MAPW - 1)//防止生成相同位置的障碍
		{
			i--;
			continue;
		}
		pmap[r].type = WALL;//选中的即为障碍格
	}
}
void createfood1(PGRID pmap)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < MAXFOOD1; i++)
	{
		int r = rand() % (MAPH * MAPW);
		if (pmap[r].type == END || pmap[r].type == BEGIN ||
			pmap[r].type == WALL || pmap[r].type == FOOD ||
			pmap[r].type == BOSS1 || pmap[r].type == BOSS2 || 
			pmap[r].type == HANDRAIL||pmap[r].type==TRAP||
			r== MAPH * MAPW-1)//防止生成相同位置的食物
		{
			i--;
			continue;
		}
		pmap[r].type = FOOD;//选中的即为食物格
	}

}
void createfood2(PGRID pmap)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < MAXFOOD2; i++)
	{
		int r = rand() % (MAPH * MAPW);
		if (pmap[r].type == END || pmap[r].type == BEGIN ||
			pmap[r].type == WALL || pmap[r].type == FOOD ||
			pmap[r].type == BOSS1 || pmap[r].type == BOSS2 || pmap[r].type == HANDRAIL
			||r== MAPH * MAPW-1)//防止生成相同位置的食物
		{
			i--;
			continue;
		}
		pmap[r].type = FOOD;//选中的即为食物格
	}

}
void createfood3(PGRID pmap)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < MAXFOOD3; i++)
	{
		int r = rand() % (MAPH * MAPW);
		if (pmap[r].type == END || pmap[r].type == BEGIN ||
			pmap[r].type == WALL || pmap[r].type == FOOD ||
			pmap[r].type == BOSS1 || pmap[r].type == BOSS2 || pmap[r].type == HANDRAIL || r == MAPH * MAPW - 1)//防止生成相同位置的食物
		{
			i--;
			continue;
		}
		pmap[r].type = FOOD;//选中的即为食物格
	}

}
void createfood4(PGRID pmap)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < MAXFOOD4; i++)
	{
		int r = rand() % (MAPH * MAPW);
		if (pmap[r].type == END || pmap[r].type == BEGIN ||
			pmap[r].type == WALL || pmap[r].type == FOOD ||
			pmap[r].type == BOSS1 || pmap[r].type == BOSS2 || pmap[r].type == HANDRAIL || r == MAPH * MAPW - 1)//防止生成相同位置的食物
		{
			i--;
			continue;
		}
		pmap[r].type = FOOD;//选中的即为食物格
	}

}
void createtrap1(PGRID pmap)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < TRAP1; i++)
	{
		int r = rand() % (MAPH * MAPW);
		if (pmap[r].type == END || pmap[r].type == BEGIN ||
			pmap[r].type == WALL || pmap[r].type == FOOD ||
			pmap[r].type == BOSS1 || pmap[r].type == BOSS2 ||
			pmap[r].type == HANDRAIL || pmap[r].type == TRAP || r == MAPH * MAPW - 1)//防止生成相同位置的食物
		{
			i--;
			continue;
		}
		pmap[r].type = TRAP;//选中的即为食物格
	}

}
void createtrap2(PGRID pmap)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < TRAP2; i++)
	{
		int r = rand() % (MAPH * MAPW);
		if (pmap[r].type == END || pmap[r].type == BEGIN ||
			pmap[r].type == WALL || pmap[r].type == FOOD ||
			pmap[r].type == BOSS1 || pmap[r].type == BOSS2 ||
			pmap[r].type == HANDRAIL || pmap[r].type == TRAP || r == MAPH * MAPW - 1)//防止生成相同位置的食物
		{
			i--;
			continue;
		}
		pmap[r].type = TRAP;//选中的即为食物格
	}

}
void createtrap3(PGRID pmap)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < TRAP3; i++)
	{
		int r = rand() % (MAPH * MAPW);
		if (pmap[r].type == END || pmap[r].type == BEGIN ||
			pmap[r].type == WALL || pmap[r].type == FOOD ||
			pmap[r].type == BOSS1 || pmap[r].type == BOSS2 ||
			pmap[r].type == HANDRAIL || pmap[r].type == TRAP || r == MAPH * MAPW - 1)//防止生成相同位置的食物
		{
			i--;
			continue;
		}
		pmap[r].type = TRAP;//选中的即为食物格
	}

}
void createtrap4(PGRID pmap)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < TRAP4;i++)
	{
		int r = rand() % (MAPH * MAPW);
		if (pmap[r].type == END || pmap[r].type == BEGIN ||
			pmap[r].type == WALL || pmap[r].type == FOOD ||
			pmap[r].type == BOSS1 || pmap[r].type == BOSS2 ||
			pmap[r].type == HANDRAIL || pmap[r].type == TRAP || r == MAPH * MAPW - 1)//防止生成相同位置的陷阱
		{
			i--;
			continue;
		}
		pmap[r].type = TRAP;//选中的即为陷阱格
	}

}
int createfastfood(PGRID pmap)
{
	srand((unsigned)time(NULL));
	int r = rand() % (MAPH * MAPW);
	while (pmap[r].type == END || pmap[r].type == BEGIN ||
		pmap[r].type == WALL || pmap[r].type == FOOD ||
		pmap[r].type == BOSS1 || pmap[r].type == BOSS2||
		pmap[r].type==HANDRAIL||pmap[r].type==BOX||pmap[r].type==TRAP || r == MAPH * MAPW - 1)//防止生成相同位置的食物
	{
		r = rand() % (MAPH * MAPW) - 3;
	}
	return r;
}
int createbox(PGRID pmap)
{
	srand((unsigned)time(NULL));
	int r = rand() % (MAPH * MAPW);
	while (pmap[r].type == END || pmap[r].type == BEGIN ||
		pmap[r].type == WALL || pmap[r].type == FOOD ||
		pmap[r].type == BOSS1 || pmap[r].type == BOSS2 || 
		pmap[r].type == FASTFOOD||pmap[r].type==HANDRAIL||pmap[r].type==TRAP || r == MAPH * MAPW - 1)//防止生成相同位置的食物
	{
		r = rand() % (MAPH * MAPW) - 3;
	}
	return r;
}
//A*算法子函数，找到pCur的邻点，记录至pneighbor[]中
void findneighbor(GRID pneighbor[], PGRID pCur)
{
	pneighbor[0].x = pCur->x + 1;
	pneighbor[0].y = pCur->y;
	pneighbor[1].x = pCur->x;
	pneighbor[1].y = pCur->y + 1;
	pneighbor[2].x = pCur->x - 1;
	pneighbor[2].y = pCur->y;
	pneighbor[3].x = pCur->x;
	pneighbor[3].y = pCur->y - 1;
}
//A*算法子函数，获得最小打开表
void getMinOpenlist(list<PGRID>* OpenList)
{
	PGRID p, s;
	int i;
	p = OpenList->front();
	OpenList->pop_front();
	for (i = 0; i < OpenList->size(); i++)
	{
		s = OpenList->front();
		OpenList->pop_front();
		if (s->F < p->F)
		{
			OpenList->push_back(p);
			p = s;
		}
		else
		{
			OpenList->push_back(s);
		}
	}
	OpenList->push_front(p);
}
//A*子函数，判断格子是否在列表中，存在返回1，不存在返回0
int isInList(PGRID pGrid, list<PGRID> List)
{
	list<PGRID>::iterator itr;
	itr = find(List.begin(), List.end(), pGrid);
	if (itr == List.end())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
//A*子函数，判断格子（数组）是否在列表中，存在返回1，不存在返回0
int isNeiInlist(PGRID pneighbor, list<PGRID>  List)
{
	list<PGRID>::iterator itr;
	PGRID p;
	int t = 0;
	for (itr = List.begin(); itr != List.end(); itr++)
	{
		p = *itr;
		if (p->x == pneighbor->x && p->y == pneighbor->y)
		{
			t = 1;
		}
	}
	return t;
}
//A*算法子函数，计算G,F
void getGridf(PGRID p)
{
	int t;
	if (p->prev != NULL)
	{
		t = abs(p->x - p->prev->x) + abs(p->y - p->prev->y);
		if (t == 2)
		{
			p->G = p->prev->G + DLENGTH;
		}
		else
		{
			p->G = p->prev->G + SLENGTH;
		}
	}
	else
	{
		p->G = 0;
	}
	p->F = p->G + p->H;
}
void isAccess(GRID pneighbor[], PGRID pmap)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		//下标越界，则删除
		if (pneighbor[i].x >= MAPW || pneighbor[i].x < 0 || pneighbor[i].y >= MAPH || pneighbor[i].y < 0)
		{
			pneighbor[i].x = -1;
			pneighbor[i].y = -1;
		}
		//判断是否为障碍物
		else
		{
			if (pmap[pneighbor[i].y * MAPH + pneighbor[i].x].type == WALL)
			{
				pneighbor[i].x = -1;
				pneighbor[i].y = -1;
			}
		}
	}
}
//A*算法函数，计算结果保存至pmap中
bool getRoad(PGRID pmap, PGRID pbegin, PGRID pend)
{
	list<PGRID> OpenList;
	list<PGRID> CloseList;
	list<PGRID> RoadList;

	PGRID pCur;
	PGRID p;
	GRID pNeighbor[8];

	int i;
	int G;
	int t;

	//把起点放入openlist
	getGridf(pbegin);//起始点加入，代价赋予
	pbegin->prev = NULL;
	OpenList.push_back(pbegin);
	pCur = OpenList.front();
	while (!(isInList(pend, OpenList) || OpenList.empty()))
	{
		//取出openlist里F值最小的方格，为当前格
		getMinOpenlist(&OpenList);
		pCur = OpenList.front();
		//把它放到关闭列表中
		OpenList.pop_front();
		CloseList.push_back(pCur);
		//找到当前相邻的八格
		findneighbor(pNeighbor, pCur);
		isAccess(pNeighbor, pmap);

		for (i = 0; i < 8; i++)
		{
			//对于相邻的8格中，略过不可通过或者关闭列表中的
			if ((-1 == pNeighbor[i].x && -1 == pNeighbor[i].y) || isNeiInlist(&pNeighbor[i], CloseList))
			{
				;
			}
			else
			{
				p = &pmap[pNeighbor[i].y * MAPW + pNeighbor[i].x];
				//如果不在开启列表中，添加进去
				if (!isNeiInlist(&pNeighbor[i], OpenList))
				{
					p->prev = pCur;
					getGridf(p);
					OpenList.push_back(p);
				}
				//如果在，用G值检查新路径是否更好
				else
				{
					t = abs(p->x - pCur->x) + abs(p->y - pCur->y);
					if (t == 2)
					{
						G = pCur->G + DLENGTH;
					}
					else
					{
						G = pCur->G + SLENGTH;
					}
					if (G < p->G)
					{
						p->prev = pCur;
						getGridf(p);
					}
				}
			}
		}
	}
	if (isInList(pend, OpenList))
	{
		RoadList.push_front(pend);
		pCur = RoadList.front();
		while (pCur->prev != pbegin)
		{
			pCur = pCur->prev;
			pCur->type = ROAD;
			RoadList.push_front(pCur);
		}
		RoadList.push_front(pbegin);
	}
	else
	{
		return false;
	}
	return true;
}
void drawtishi()
{
	settextcolor(WHITE);//设置字体颜色
	setlinecolor(WHITE);
	settextstyle(50, 0, _T("楷体"));//设置字体
	RECT r1;
	r1.left = 800;
	r1.right = 900;
	r1.top = 450;
	r1.bottom = 500;
	rectangle(800, 450, 900, 500);
	drawtext(_T("提示"), &r1, DT_CENTER || DT_VCENTER || DT_SINGLELINE);
}
void DrawRoad(PGRID pmap)
{
	int x1, y1, x2, y2, x, y;
	int i, j;
	for (i = 0; i < MAPH; i++)
	{
		for (j = 0; j < MAPW; j++)
		{
			if (pmap[i * MAPW + j].type == ROAD)
			{
				x1 = BP + j * PGL;
				y1 = BP + i * PGL;
				x2 = x1 + PGL;
				y2 = y1 + PGL;
				setlinecolor(RGB(255, 255, 0));
				rectangle(x1 + W, y1 + W, x2 - W, y2 - W);
				rectangle(x1 + W + W, y1 + W + W, x2 - W - W, y2 - W - W);
				x = (x2 - x1) / 2 + x1;
				y = (y2 - y1) / 2 + y1;
				setlinecolor(RGB(255, 0, 0));
				setfillcolor(RGB(255, 0, 0));
				fillcircle(x, y, R);
			}
		}
	}
}
void drawHPTL()
{
	setlinecolor(WHITE);
	setfillcolor(RED);
	solidrectangle(850, 200, 850 + HP * 10, 230);
	setfillcolor(BLUE);
	solidrectangle(850, 300, 850 + TL * 20, 330);
	rectangle(850, 200, 950, 230);
	rectangle(850, 300, 950, 330);
}
void drawmap(PGRID pmap)
{
	int x1, x2, y1, y2;
	int i, j;

	//画外框
	setlinecolor(BLACK);
	fillrectangle(MAPW * PGL + 2 * BP, 0, MAPW * PGL + 2 * BP - 5, MAPH * PGL + 2 * BP);
	fillrectangle(0, 0, MAPW * PGL + 2 * BP - 5, 5);
	fillrectangle(0, MAPW * PGL + 2 * BP - 5, MAPW * PGL + 2 * BP - 5, MAPH * PGL + 2 * BP);
	fillrectangle(0, 0, 5, MAPH * PGL + 2 * BP);
	//画棋盘
	putimage(BP, BP, &gamebk);
	//画水平垂直线
	x1 = BP;
	x2 = BP;
	srand((unsigned)time(NULL));
	if (pmap[(MAPH-1) * MAPW + MAPW-1].type == BLANK)
	{
		putimage(BP + PGL * (MAPW - 1), BP + PGL * (MAPH - 1), 50, 50, &roundold2, 0, 0, SRCAND);
		putimage(BP + PGL * (MAPW - 1), BP + PGL * (MAPH - 1), 50, 50, &roundold1, 0, 0, SRCPAINT);
	}
	if (pmap[MAPH*MAPW-1].type == END)
	{
		putimage(BP + PGL * (MAPW - 1), BP + PGL * (MAPH - 1), 50, 50, &roundnew2, 0, 0, SRCAND);
		putimage(BP + PGL * (MAPW - 1), BP + PGL * (MAPH - 1), 50, 50, &roundnew1, 0, 0, SRCPAINT);
	}
	for (i = 0; i < MAPH; i++)
	{
		for (j = 0; j < MAPW; j++)
		{
			if (pmap[i * MAPW + j].type == WALL)
			{
				x1 = BP + j * PGL;
				y1 = BP + i * PGL;
				x2 = x1 + PGL;
				y2 = y1 + PGL;
				putimage(x1, y1, 50, 50, &wall2, 0, 0, SRCAND);
				putimage(x1, y1, 50, 50, &wall1, 0, 0, SRCPAINT);
			}
			else if (pmap[i * MAPW + j].type == FOOD)
			{
				x1 = BP + j * PGL;
				y1 = BP + i * PGL;
				putimage(x1, y1, 50, 50, &food2, 0, 0, SRCAND);
				putimage(x1, y1, 50, 50, &food1, 0, 0, SRCPAINT);
			}
			else if (pmap[i * MAPW + j].type == FASTFOOD)
			{
				x1 = BP + j * PGL;
				y1 = BP + i * PGL;
				putimage(x1, y1, 50, 50, &fastfood2, 0, 0, SRCAND);
				putimage(x1, y1, 50, 50, &fastfood1, 0, 0, SRCPAINT);
			}
			else if (pmap[i * MAPW + j].type == HANDRAIL)
			{
				x1 = BP + j * PGL;
				y1 = BP + i * PGL;
				putimage(x1, y1, 50, 50, &langan2, 0, 0, SRCAND);
				putimage(x1, y1, 50, 50, &langan1, 0, 0, SRCPAINT);
			}
			else if (pmap[i * MAPW + j].type == BOX)
			{
				x1 = BP + j * PGL;
				y1 = BP + i * PGL;
				putimage(x1, y1, 50, 50, &box1, 0, 0, SRCAND);
				putimage(x1, y1, 50, 50, &box2, 0, 0, SRCPAINT);
			}
			else if (pmap[i * MAPW + j].type == TRAP)
			{
				x1 = BP + j * PGL;
				y1 = BP + i * PGL;
				putimage(x1, y1, 50, 50, &trap2, 0, 0, SRCAND);
				putimage(x1, y1, 50, 50, &trap1, 0, 0, SRCPAINT);
			}
		}
	}
	//画分值
	drawHPTL();
	TCHAR m[3];
	settextstyle(30, 0, _T("楷体"));
	settextcolor(WHITE);
	outtextxy(770, 10, _T("收集:"));
	_sntprintf_s(m, 10, _T("%d"), fen);
	outtextxy(860, 10, m);
	TCHAR n[3];
	outtextxy(770, 200, _T("血量"));
	_sntprintf_s(n, 10, _T("%d"), HP);
	outtextxy(855, 200, n);
	TCHAR u[3];
	outtextxy(770, 300, _T("体力"));
	_sntprintf_s(u, 10, _T("%d"), TL);
	outtextxy(855, 300, u);
}
void drawmap4(PGRID pmap)
{
	int x1, x2, y1, y2;
	int i, j;

	//画外框
	setlinecolor(BLACK);
	fillrectangle(MAPW * PGL + 2 * BP, 0, MAPW * PGL + 2 * BP - 5, MAPH * PGL + 2 * BP);
	fillrectangle(0, 0, MAPW * PGL + 2 * BP - 5, 5);
	fillrectangle(0, MAPW * PGL + 2 * BP - 5, MAPW * PGL + 2 * BP - 5, MAPH * PGL + 2 * BP);
	fillrectangle(0, 0, 5, MAPH * PGL + 2 * BP);
	//画棋盘
	putimage(BP, BP, &gamebk);
	//画水平垂直线
	x1 = BP;
	x2 = BP;
	srand((unsigned)time(NULL));
	putimage(BP + PGL * (MAPW - 1), BP + PGL * (MAPH-1), 50, 50, &flower2, 0, 0, SRCAND);
	putimage(BP + PGL * (MAPW - 1), BP + PGL * (MAPH - 1), 50, 50, &flower1, 0, 0, SRCPAINT);
	for (i = 0; i < MAPH; i++)
	{
		for (j = 0; j < MAPW; j++)
		{
			if (pmap[i * MAPW + j].type == WALL)
			{
				x1 = BP + j * PGL;
				y1 = BP + i * PGL;
				x2 = x1 + PGL;
				y2 = y1 + PGL;
				putimage(x1, y1, 50, 50, &wall2, 0, 0, SRCAND);
				putimage(x1, y1, 50, 50, &wall1, 0, 0, SRCPAINT);
			}
			else if (pmap[i * MAPW + j].type == FOOD)
			{
				x1 = BP + j * PGL;
				y1 = BP + i * PGL;
				putimage(x1, y1, 50, 50, &food2, 0, 0, SRCAND);
				putimage(x1, y1, 50, 50, &food1, 0, 0, SRCPAINT);
			}
			else if (pmap[i * MAPW + j].type == FASTFOOD)
			{
				x1 = BP + j * PGL;
				y1 = BP + i * PGL;
				putimage(x1, y1, 50, 50, &fastfood2, 0, 0, SRCAND);
				putimage(x1, y1, 50, 50, &fastfood1, 0, 0, SRCPAINT);
			}
			else if (pmap[i * MAPW + j].type == HANDRAIL)
			{
				x1 = BP + j * PGL;
				y1 = BP + i * PGL;
				putimage(x1, y1, 50, 50, &langan2, 0, 0, SRCAND);
				putimage(x1, y1, 50, 50, &langan1, 0, 0, SRCPAINT);
			}
			else if (pmap[i * MAPW + j].type == BOX)
			{
				x1 = BP + j * PGL;
				y1 = BP + i * PGL;
				putimage(x1, y1, 50, 50, &box1, 0, 0, SRCAND);
				putimage(x1, y1, 50, 50, &box2, 0, 0, SRCPAINT);
			}
			else if (pmap[i * MAPW + j].type == TRAP)
			{
				x1 = BP + j * PGL;
				y1 = BP + i * PGL;
				putimage(x1, y1, 50, 50, &trap2, 0, 0, SRCAND);
				putimage(x1, y1, 50, 50, &trap1, 0, 0, SRCPAINT);
			}
		}
	}
	//画分值
	drawHPTL();
	TCHAR m[3];
	settextstyle(30, 0, _T("楷体"));
	settextcolor(WHITE);
	outtextxy(770, 10, _T("收集:"));
	_sntprintf_s(m, 10, _T("%d"), fen);
	outtextxy(860, 10, m);
	TCHAR n[3];
	outtextxy(770, 200, _T("血量"));
	_sntprintf_s(n, 10, _T("%d"), HP);
	outtextxy(855, 200, n);
	TCHAR u[3];
	outtextxy(770, 300, _T("体力"));
	_sntprintf_s(u, 10, _T("%d"), TL);
	outtextxy(855, 300, u);
}
void drawboss1()
{
	putimage(bossx, bossy, 64, 50, &boss2, bossnumindex * 64,
		50 * bossnum1, SRCAND);//掩码图与消除边框
	putimage(bossx, bossy, 64, 50, &boss1, bossnumindex * 64,
		50 * bossnum1, SRCPAINT);//掩码图或操作把人物放入
}
void drawboss2()
{
	putimage(bossx1, bossy1, 64, 50, &boss2, bossnumindex * 64,
		50 * bossnum1, SRCAND);//掩码图与消除边框
	putimage(bossx1, bossy1, 64, 50, &boss1, bossnumindex * 64,
		50 * bossnum1, SRCPAINT);//掩码图或操作把人物放入
}
void bossfast(PGRID pmap)
{
	int num2 = 2;
	if (bossy > y && abs(bossy - y) >= abs(bossx - x)) num2 = 3;
	else if (bossy < y && abs(bossy - y) > abs(bossx - x)) num2 = 0;
	else if (bossx > x && abs(bossy - y) <= abs(bossx - x)) num2 = 1;
	else if (bossx < x && abs(bossy - y) < abs(bossx - x)) num2 = 2;

	switch (num2)
	{
	case 1:
		bossnum1 = 0;
		bossx -= 3;
		for (int i = 0; i < MAPH; i++)
		{
			for (int j = 0; j < MAPW; j++)
			{
				if (bossx< BP || bossx>MAPW * PGL - 5 * BP)
				{
					bossx += 3;
					break;
				}
				else if (pmap[i * MAPW + j].type == WALL &&
					bossx >= PGL * j - 3 * BP &&
					bossx < PGL * (j + 1) && bossy >= i * PGL - 3 * BP &&
					bossy < (i + 1) * PGL - 3 * BP)
				{
					bossx += 3;
					break;
				}
			}
		}
		break;
	case 2:
		bossnum1 = 1;
		bossx += 3;
		for (int i = 0; i < MAPH; i++)
		{
			for (int j = 0; j < MAPW; j++)
			{
				if (bossx< BP || bossx>MAPW * PGL - 5 * BP)
				{
					bossx -= 3;
					break;
				}
				else if (pmap[i * MAPW + j].type == WALL &&
					bossx >= PGL * j - 3 * BP &&
					bossx < PGL * (j + 1) && bossy >= i * PGL - 3 * BP &&
					bossy < (i + 1) * PGL - 3 * BP)
				{
					bossx -= 3;
					break;
				}
			}
		}
		break;
	case 3:
		bossnum1 = 0;
		bossy -= 3;
		for (int i = 0; i < MAPH; i++)
		{
			for (int j = 0; j < MAPW; j++)
			{
				if (bossy< BP || bossy>MAPH * PGL - 3 * BP)
				{
					bossy += 3;
					break;
				}
				else if (pmap[i * MAPW + j].type == WALL
					&& bossx >= PGL * j - 3 * BP
					&& bossx < PGL * (j + 1) && bossy >= i * PGL - 3 * BP
					&& bossy < (i + 1) * PGL - 3 * BP)
				{
					bossy += 3;
					break;
				}
			}
		}
		break;
	case 0:
		bossnum1 = 1;
		bossy += 3;
		for (int i = 0; i < MAPH; i++)
		{
			for (int j = 0; j < MAPW; j++)
			{
				if (bossy< BP || bossy>MAPH * PGL - 3 * BP)
				{
					bossy -= 3;
					break;
				}
				else if (pmap[i * MAPW + j].type == WALL && 
					bossx >= PGL * j - 3 * BP && bossx < PGL * (j + 1)
					&& bossy >= i * PGL - 3 * BP && bossy < (i + 1) * PGL - 3 * BP)
				{
					bossy -= 3;
					break;
				}
			}
		}
		break;
	}
	bossnumindex = (++bossnumindex) % 8;//按循环次数在0-7选图贴图
}
void bossslow(PGRID pmap)
{
	int num2 = 2;
	if (bossy1 > y && abs(bossy1 - y) >= abs(bossx1 - x)) num2 = 3;
	else if (bossy1 < y && abs(bossy1 - y) > abs(bossx1 - x)) num2 = 0;
	else if (bossx1 > x && abs(bossy1 - y) <= abs(bossx1 - x)) num2 = 1;
	else if (bossx1 < x && abs(bossy1 - y) < abs(bossx1 - x)) num2 = 2;

	switch (num2)
	{
	case 1:
		bossnum1 = 0;
		bossx1 -= 1;
		for (int i = 0; i < MAPH; i++)
		{
			for (int j = 0; j < MAPW; j++)
			{
				if (bossx1< BP || bossx1>MAPW * PGL - 5 * BP)
				{
					bossx1 += 1;
					break;
				}
				else if (pmap[i * MAPW + j].type == WALL &&
					bossx1 >= PGL * j - 3 * BP &&
					bossx1 < PGL * (j + 1) && bossy1 >= i * PGL - 3 * BP &&
					bossy1 < (i + 1) * PGL - 3 * BP)
				{
					bossx1 += 1;
					break;
				}
			}
		}
		break;
	case 2:
		bossnum1 = 1;
		bossx1 += 1;
		for (int i = 0; i < MAPH; i++)
		{
			for (int j = 0; j < MAPW; j++)
			{
				if (bossx1< BP || bossx1>MAPW * PGL - 5 * BP)
				{
					bossx1 -= 1;
					break;
				}
				else if (pmap[i * MAPW + j].type == WALL &&
					bossx1 >= PGL * j - 3 * BP &&
					bossx1 < PGL * (j + 1) && bossy1 >= i * PGL - 3 * BP &&
					bossy1 < (i + 1) * PGL - 3 * BP)
				{
					bossx1 -= 1;
					break;
				}
			}
		}
		break;
	case 3:
		bossnum1 = 0;
		bossy1 -= 1;
		for (int i = 0; i < MAPH; i++)
		{
			for (int j = 0; j < MAPW; j++)
			{
				if (bossy1< BP || bossy1>MAPH * PGL - 3 * BP)
				{
					bossy1 += 1;
					break;
				}
				else if (pmap[i * MAPW + j].type == WALL
					&& bossx1 >= PGL * j - 3 * BP
					&& bossx1 < PGL * (j + 1) && bossy1 >= i * PGL - 3 * BP
					&& bossy1 < (i + 1) * PGL - 3 * BP)
				{
					bossy1 += 1;
					break;
				}
			}
		}
		break;
	case 0:
		bossnum1 = 1;
		bossy1 += 1;
		for (int i = 0; i < MAPH; i++)
		{
			for (int j = 0; j < MAPW; j++)
			{
				if (bossy1< BP || bossy1>MAPH * PGL - 3 * BP)
				{
					bossy1 -= 1;
					break;
				}
				else if (pmap[i * MAPW + j].type == WALL && bossx1 >=
					PGL * j - 3 * BP && bossx1 < PGL * (j + 1) &&
					bossy1 >= i * PGL - 3 * BP && bossy1 < (i + 1) * PGL - 3 * BP)
				{
					bossy1 -= 1;
					break;
				}
			}
		}
		break;
	}
	bossnumindex = (++bossnumindex) % 8;//按循环次数在0-7选图贴图
}
void drawplayer()
{
	putimage(x, y, 50, 50, &player2, NumIndex * 50, 50 * num1, SRCAND);
	putimage(x, y, 50, 50, &player1, NumIndex * 50, 50 * num1, SRCPAINT);
}
void Keydown(PGRID pmap)
{
	char step;
	step = _getch();
	switch (step)
	{
	case 'w':
	case 'W':
	case 72:
		num1 = 3;
		y -= BC;
		for (int i = 0; i < MAPH; i++)
		{
			for (int j = 0; j < MAPW; j++)
			{
				if (y< BP || y>MAPH * PGL - 4 * BP)
				{
					y += BC;
					break;
				}
				else if ((pmap[i * MAPW + j].type == WALL || pmap[i * MAPW + j].type == HANDRAIL) && x >= PGL * j - 2 * BP && x < PGL * (j + 1) && y >= i * PGL - 3 * BP && y < (i + 1) * PGL - 3 * BP)
				{
					y += BC;
					break;
				}
				else if (pmap[i * MAPW + j].type == FOOD && x >= PGL * j && x < PGL * j + 4 * BP && y >= PGL * i - 2 * BP && y < PGL * i + 2 * BP)
				{
					fen++;
					pmap[i * MAPW + j].type = BLANK;
				}
				else if (pmap[i * MAPW + j].type == TRAP && x >= PGL * j && x < PGL * j + 4 * BP && y >= PGL * i && y < PGL * i + 2 * BP)
				{
					HP--;
					pmap[i * MAPW + j].type = BLANK;
				}
				else if (pmap[i * MAPW + j].type == FASTFOOD && x >= PGL * j && x < PGL * j + 4 * BP && y >= PGL * i - 2 * BP && y < PGL * i + 2 * BP)
				{
					pmap[i * MAPW + j].type = BLANK;
					usetime = 200;
				}
				else if (pmap[i * MAPW + j].type == BOX &&
					x >= PGL * j && x < PGL * j + 4 * BP &&
					y >= PGL * i - 2 * BP && y < PGL * i + 2 * BP)
				{
					pmap[i * MAPW + j].type = BLANK;
					usetime1 = 200;
					int z = rand() % 4;
					if (z == 0)
					{
						TL++;
					}
					else if (z == 1)
					{
						HP++;
					}
					else if (z == 2 || z == 3)
					{
						HP--;
						HP--;
					}
				}
			}
		}
		break;
	case 's':
	case 'S':
	case 80:
		num1 = 0;
		y += BC;
		for (int i = 0; i < MAPH; i++)
		{
			for (int j = 0; j < MAPW; j++)
			{
				if (y<BP || y>MAPH * PGL - 4 * BP)
				{
					y -= BC;
					break;
				}
				else if ((pmap[i * MAPW + j].type == WALL || pmap[i * MAPW + j].type == HANDRAIL) && x >= PGL * j - 2 * BP && x < PGL * (j + 1) && y >= i * PGL - 3 * BP && y < (i + 1) * PGL - 3 * BP)
				{
					y -= BC;
					break;
				}
				else if (pmap[i * MAPW + j].type == FOOD && x >= PGL * j && x < PGL * j + 4 * BP && y >= PGL * i - 2 * BP && y < PGL * i + 2 * BP)
				{
					fen++;
					pmap[i * MAPW + j].type = BLANK;
				}
				else if (pmap[i * MAPW + j].type == TRAP && x >= PGL * j && x < PGL * j + 4 * BP && y >= PGL * i  && y < PGL * i + 2 * BP)
				{
					HP--;
					pmap[i * MAPW + j].type = BLANK;
				}
				else if (pmap[i * MAPW + j].type == FASTFOOD && x >= PGL * j && x < PGL * j + 4 * BP && y >= PGL * i - 2 * BP && y < PGL * i + 2 * BP)
				{
					pmap[i * MAPW + j].type = BLANK;
					usetime = 200;
				}
				else if (pmap[i * MAPW + j].type == BOX &&
					x >= PGL * j && x < PGL * j + 4 * BP &&
					y >= PGL * i - 2 * BP && y < PGL * i + 2 * BP)
				{
					pmap[i * MAPW + j].type = BLANK;
					usetime1 = 200;
					int z = rand() % 4;
					if (z == 0)
					{
						TL++;
					}
					else if (z == 1)
					{
						HP++;
					}
					else if (z == 2 || z == 3)
					{
						HP--;
						HP--;
					}
				}
			}
		}
		break;
	case 'a':
	case 'A':
	case 75:
		num1 = 1;
		x -= BC;
		for (int i = 0; i < MAPH; i++)
		{
			for (int j = 0; j < MAPW; j++)
			{
				if (x<BP || x>MAPW * PGL - 3 * BP)
				{
					x += BC;
					break;
				}
				else if ((pmap[i * MAPW + j].type == WALL || pmap[i * MAPW + j].type == HANDRAIL) && x >= PGL * j - 2 * BP && x < PGL * (j + 1) && y >= i * PGL - 3 * BP && y < (i + 1) * PGL - 3 * BP)
				{
					x += BC;
					break;
				}
				else if (pmap[i * MAPW + j].type == FOOD && x >= PGL * j && x < PGL * j + 4 * BP && y >= PGL * i - 2 * BP && y < PGL * i + 2 * BP)
				{
					fen++;
					pmap[i * MAPW + j].type = BLANK;
				}
				else if (pmap[i * MAPW + j].type == TRAP && x >= PGL * j && x < PGL * j + 4 * BP && y >= PGL * i  && y < PGL * i + 2 * BP)
				{
					HP--;
					pmap[i * MAPW + j].type = BLANK;
				}
				else if (pmap[i * MAPW + j].type == FASTFOOD && x >= PGL * j && x < PGL * j + 4 * BP && y >= PGL * i - 2 * BP && y < PGL * i + 2 * BP)
				{
					pmap[i * MAPW + j].type = BLANK;
					usetime = 200;
				}
				else if (pmap[i * MAPW + j].type == BOX &&
					x >= PGL * j && x < PGL * j + 4 * BP &&
					y >= PGL * i - 2 * BP && y < PGL * i + 2 * BP)
				{
					pmap[i * MAPW + j].type = BLANK;
					usetime1 = 200;
					int z = rand() % 4;
					if (z == 0)
					{
						TL++;
					}
					else if (z == 1)
					{
						HP++;
					}
					else if (z == 2 || z == 3)
					{
						HP--;
						HP--;
					}
				}
			}
		}
		break;
	case 'd':
	case'D':
	case 77:
		num1 = 2;
		x += BC;
		for (int i = 0; i < MAPH; i++)
		{
			for (int j = 0; j < MAPW; j++)
			{
				if (x< BP || x>MAPW * PGL - 3 * BP)
				{
					x -= BC;
					break;
				}
				else if ((pmap[i * MAPW + j].type == WALL || pmap[i * MAPW + j].type == HANDRAIL) && x >= PGL * j - 2 * BP && x < PGL * (j + 1) && y >= i * PGL - 3 * BP && y < (i + 1) * PGL - 3 * BP)
				{
					x -= BC;
					break;
				}
				else if (pmap[i * MAPW + j].type == FOOD && x >= PGL * j && x < PGL * j + 4 * BP && y >= PGL * i - 2 * BP && y < PGL * i + 2 * BP)
				{
					fen++;
					pmap[i * MAPW + j].type = BLANK;
				}
				else if (pmap[i * MAPW + j].type == TRAP && x >= PGL * j && x < PGL * j + 4 * BP && y >= PGL * i && y < PGL * i + 2 * BP)
				{
					HP--;
					pmap[i * MAPW + j].type = BLANK;
				}
				else if (pmap[i * MAPW + j].type == FASTFOOD && x >= PGL * j && x < PGL * j + 4 * BP && y >= PGL * i - 2 * BP && y < PGL * i + 2 * BP)
				{
					pmap[i * MAPW + j].type = BLANK;
					usetime = 200;
				}
				else if (pmap[i * MAPW + j].type == BOX &&
					x >= PGL * j && x < PGL * j + 4 * BP &&
					y >= PGL * i - 2 * BP && y < PGL * i + 2 * BP)
				{
					pmap[i * MAPW + j].type = BLANK;
					usetime1 = 200;
					int z = rand() % 4;
					if (z == 0)
					{
						TL++;
					}
					else if (z == 1)
					{
						HP++;
					}
					else if (z == 2 || z == 3)
					{
						HP--;
						HP--;
					}
				}
			}
		}
		break;
	case 'j':
	case 'J':
		if (TL > 0)
		{
			for (int r = 0; r < MAPW * MAPH; r++)
			{
				if (x > pmap[r].x * PGL - 2 * BP && x <= (pmap[r].x + 1) *
					PGL - 2 * BP && y > pmap[r].y * PGL - 4 * BP && y <=
					(pmap[r].y + 1) * PGL - 4 * BP)
				{
					int m, n, i, j;
					m = r - 1; n = r + 1; i = r + MAPW; j = r - MAPW;
					if (r == 0)
					{
						if (pmap[n].type == WALL) pmap[n].type = BLANK;
						if (pmap[i].type == WALL) pmap[i].type = BLANK;
					}
					else if (r == MAPW - 1)
					{
						if (pmap[m].type == WALL) pmap[m].type = BLANK;
						if (pmap[i].type == WALL) pmap[i].type = BLANK;
					}
					else if (r == MAPW * (MAPH - 1))
					{
						if (pmap[n].type == WALL) pmap[n].type = BLANK;
						if (pmap[j].type == WALL) pmap[j].type = BLANK;
					}
					else if (r % MAPW == 0)
					{
						if (pmap[n].type == WALL) pmap[n].type = BLANK;
						if (pmap[i].type == WALL) pmap[i].type = BLANK;
						if (pmap[j].type == WALL) pmap[j].type = BLANK;
					}
					else if ((r + 1) % MAPW == 0)
					{
						if (pmap[m].type == WALL) pmap[m].type = BLANK;
						if (pmap[i].type == WALL) pmap[i].type = BLANK;
						if (pmap[j].type == WALL) pmap[j].type = BLANK;
					}
					else if (r < MAPW - 1)
					{
						if (pmap[m].type == WALL) pmap[m].type = BLANK;
						if (pmap[n].type == WALL) pmap[n].type = BLANK;
						if (pmap[i].type == WALL) pmap[i].type = BLANK;
					}
					else if (r > MAPW * (MAPH - 1))
					{
						if (pmap[m].type == WALL) pmap[m].type = BLANK;
						if (pmap[n].type == WALL) pmap[n].type = BLANK;
						if (pmap[j].type == WALL) pmap[j].type = BLANK;
					}
					else
					{
						if (pmap[m].type == WALL) pmap[m].type = BLANK;
						if (pmap[n].type == WALL) pmap[n].type = BLANK;
						if (pmap[i].type == WALL) pmap[i].type = BLANK;
						if (pmap[j].type == WALL) pmap[j].type = BLANK;
					}
				}
			}
			TL--;
		}
		break;
	case 'G':
	case'g':
		DrawRoad(pmap);
		if (tishi == 0) tishi = 1;
		else if (tishi == 1) tishi = 0;
	}
}
void begingame1(PGRID pmap)
{
	putimage(0, 0, &story2);
	Sleep(5000);
	mciSendString(_T("stop begingame"), NULL, 0, NULL);
	mciSendString(_T("open ./gamemusic.mp3 alias mymusic"), NULL, 0, NULL);
	mciSendString(_T("play mymusic repeat"), NULL, 0, NULL);
	start = clock();
	while ((x != MAPW * PGL - 3 * BP || y != MAPH * PGL - 4 * BP) && goal != 0&&HP>0)
	{
		Sleep(30);
		bossslow(pmap);
		BeginBatchDraw();
		cleardevice();
		putimage(0, 0, &bkimg);
		drawmap(pmap);
		if (tishi == 1)
		{
			DrawRoad(pmap);
		}
		drawplayer();
		drawboss2();
		waittime++;
		waittime1++;
		if (waittime == 200)
		{
			int r = createfastfood(pmap);
			if (r != 0)
			{
				pmap[r].type = FASTFOOD;
			}
		}
		if (usetime > 0)
		{
			BC = 20;
			usetime--;
		}
		else if (usetime == 0)
		{
			BC = 10;
			waittime = 0;
			usetime--;
		}
		if (waittime1 == 200)
		{
			int r = createbox(pmap);
			if (r != 0)
			{
				pmap[r].type = BOX;
			}
		}
		if (usetime1 > 0)
		{
			usetime1--;
		}
		else if (usetime1 == 0)
		{
			waittime1 = 0;
			usetime1--;
		}
		if (fen == MAXFOOD1)
		{
			pmap[MAPH * MAPW - 2].type = BLANK;
			pmap[(MAPH - 1) * MAPW - 1].type = BLANK;
			pmap[(MAPH - 1) * MAPW - 2].type = BLANK;
			pmap[MAPH * MAPW - 1].type = END;
			putimage(BP + PGL * (MAPW - 1), BP + PGL * (MAPH - 1), 50, 50, &roundnew2, 0, 0, SRCAND);
			putimage(BP + PGL * (MAPW - 1), BP + PGL * (MAPH - 1), 50, 50, &roundnew1, 0, 0, SRCPAINT);
		}
		if ((x <bossx + 4 * BP && x>bossx - 3 * BP &&
			y<bossy + 4 * BP && y>bossy - 2 * BP) ||(
				x <bossx1 + 4 * BP && x>bossx1 - 3 * BP &&
				y<bossy1 + 4 * BP && y>bossy1 - 2 * BP))
		{
			goal = 0;
		}
		 
		if (_kbhit())
		{
			Keydown(pmap);
			putimage(0, 0, &bkimg);
			drawmap(pmap);
			drawplayer();
			drawboss2();
			NumIndex = (++NumIndex) % 4;
			if (tishi == 1)
			{
				DrawRoad(pmap);
			}
		}
		drawtishi();
		MYTIME mytime;
		TCHAR strtime[9];
		timeend = clock();
		timeiff = (timeend - start) / 1000;
		detime = timeiff;
		mytime.hour = timeiff / 3600;
		mytime.minute = timeiff % 3600 / 60;
		mytime.second = timeiff % 60;
		settextstyle(30, 0, _T("楷体"));
		settextcolor(WHITE);
		outtextxy(770, 60, _T("时间："));
		_sntprintf_s(strtime, sizeof(strtime), _T("%02d:%02d:%02d"), mytime.hour, mytime.minute, mytime.second);
		outtextxy(860, 60, strtime);
		EndBatchDraw();
	}
	if (detime <= 60)
	{
		jifen1 = 2000;
	}
	else if (detime > 60 && detime < 120)
	{
		jifen1 = 2000 - 12 * (detime - 60);
	}
	else if (detime >= 120 && detime <= 180)
	{
		jifen1 = 1280 - 8 * (detime - 120);
	}
	else if (detime > 180 && detime <= 300)
	{
		jifen1 = 800 - 4 * (detime - 180);
	}
	else if (detime > 300)
	{
		jifen1 = 300;
	}
	if (goal == 1&&HP>0)
	{
		mciSendString(_T("close mymusic"), NULL, 0, NULL);
		goal = 1;
		x = BP;
		y = BP;
		fen = 0;
		win1(jifen1+100*HP+200*TL, pmap);
	}
	else if (goal == 0||HP<=0)
	{
		mciSendString(_T("close mymusic"), NULL, 0, NULL);
		goal = 1;
		x = BP;
		y = BP;
		fen = 0;
		fail1(pmap);
	}
}
void begingame2(PGRID pmap)
{
	putimage(0, 0, &story4);
	Sleep(5000);
	mciSendString(_T("open ./gamemusic.mp3 alias mymusic"), NULL, 0, NULL);
	mciSendString(_T("play mymusic repeat"), NULL, 0, NULL);
	start = clock();
	while ((x != MAPW * PGL - 3 * BP || y != MAPH * PGL - 4 * BP) && goal != 0&&HP>0)
	{
		Sleep(30);
		bossfast(pmap);
		BeginBatchDraw();
		cleardevice();
		putimage(0, 0, &bkimg);
		drawmap(pmap);
		if (tishi == 1)
		{
			DrawRoad(pmap);
		}
		drawplayer();
		drawboss1();
		waittime++;
		waittime1++;
		if (waittime == 200)
		{
			int r = createfastfood(pmap);
			if (r != 0)
			{
				pmap[r].type = FASTFOOD;
			}
		}
		if (usetime > 0)
		{
			BC = 20;
			usetime--;
		}
		else if (usetime == 0)
		{
			BC = 10;
			waittime = 0;
			usetime--;
		}
		if (waittime1 == 200)
		{
			int r = createbox(pmap);
			if (r != 0)
			{
				pmap[r].type = BOX;
			}
		}
		if (usetime1 > 0)
		{
			usetime1--;
		}
		else if (usetime1 == 0)
		{
			waittime1 = 0;
			usetime1--;
		}
		if (fen == MAXFOOD2)
		{
			pmap[MAPH * MAPW - 2].type = BLANK;
			pmap[(MAPH - 1) * MAPW - 1].type = BLANK;
			pmap[(MAPH - 1) * MAPW - 2].type = BLANK;
			pmap[MAPH * MAPW - 1].type = END;
			putimage(BP + PGL * (MAPW - 1), BP + PGL * (MAPH - 1), 50, 50, &roundnew2, 0, 0, SRCAND);
			putimage(BP + PGL * (MAPW - 1), BP + PGL * (MAPH - 1), 50, 50, &roundnew1, 0, 0, SRCPAINT);
		}
		if ((x <bossx + 4 * BP && x>bossx - 3 * BP &&
			y<bossy + 4 * BP && y>bossy - 2 * BP) || (
				x <bossx1 + 4 * BP && x>bossx1 - 3 * BP &&
				y<bossy1 + 4 * BP && y>bossy1 - 2 * BP))
		{
			goal = 0;
		}

		if (_kbhit())
		{
			Keydown(pmap);
			putimage(0, 0, &bkimg);
			drawmap(pmap);
			drawplayer();
			drawboss1();
			NumIndex = (++NumIndex) % 4;
			if (tishi == 1)
			{
				DrawRoad(pmap);
			}
		}
		drawtishi();
		MYTIME mytime;
		TCHAR strtime[9];
		timeend = clock();
		timeiff = (timeend - start) / 1000;
		detime = timeiff;
		mytime.hour = timeiff / 3600;
		mytime.minute = timeiff % 3600 / 60;
		mytime.second = timeiff % 60;
		settextstyle(30, 0, _T("楷体"));
		settextcolor(WHITE);
		outtextxy(770, 60, _T("时间："));
		_sntprintf_s(strtime, sizeof(strtime), _T("%02d:%02d:%02d"), mytime.hour, mytime.minute, mytime.second);
		outtextxy(860, 60, strtime);
		EndBatchDraw();
	}
	if (detime <= 60)
	{
		jifen2 = 3000;
	}
	else if (detime > 60 && detime < 120)
	{
		jifen2 = 3000 - 18 * (detime - 60);
	}
	else if (detime >= 120 && detime <= 180)
	{
		jifen1 = 1920 - 12 * (detime - 120);
	}
	else if (detime > 180 && detime <= 300)
	{
		jifen1 = 1200 - 6 * (detime - 180);
	}
	else if (detime > 300)
	{
		jifen2 = 450;
	}
	if (goal == 1&&HP>0)
	{
		mciSendString(_T("close mymusic"), NULL, 0, NULL);
		goal = 1;
		x = BP;
		y = BP;
		fen = 0;
		win2(jifen2+150*HP+300*TL, pmap);
	}
	else if (goal == 0||HP<=0)
	{
		mciSendString(_T("close mymusic"), NULL, 0, NULL);
		goal = 1;
		x = BP;
		y = BP;
		fen = 0;
		fail2(pmap);
	}
}
void begingame3(PGRID pmap)
{
	putimage(0, 0, &story6);
	Sleep(5000);
	mciSendString(_T("open ./gamemusic.mp3 alias mymusic"), NULL, 0, NULL);
	mciSendString(_T("play mymusic"), NULL, 0, NULL);
	start = clock();
	while ((x != MAPW * PGL - 3 * BP || y != MAPH * PGL - 4 * BP) && goal != 0 && HP>0)
	{
		Sleep(30);
		bossfast(pmap);
		bossslow(pmap);
		BeginBatchDraw();
		cleardevice();
		putimage(0, 0, &bkimg);
		drawmap(pmap);
		if (tishi == 1)
		{
			DrawRoad(pmap);
		}
		drawplayer();
		drawboss1();
		drawboss2();
		waittime++;
		waittime1++;
		if (waittime == 200)
		{
			int r = createfastfood(pmap);
			if (r != 0)
			{
				pmap[r].type = FASTFOOD;
			}
		}
		if (usetime > 0)
		{
			BC = 20;
			usetime--;
		}
		else if (usetime == 0)
		{
			BC = 10;
			waittime = 0;
			usetime--;
		}
		if (waittime1 == 200)
		{
			int r = createbox(pmap);
			if (r != 0)
			{
				pmap[r].type = BOX;
			}
		}
		if (usetime1 > 0)
		{
			usetime1--;
		}
		else if (usetime1 == 0)
		{
			waittime1 = 0;
			usetime1--;
		}
		if (fen == MAXFOOD3)
		{
			pmap[MAPH * MAPW - 2].type = BLANK;
			pmap[(MAPH - 1) * MAPW - 1].type = BLANK;
			pmap[(MAPH - 1) * MAPW - 2].type = BLANK;
			pmap[MAPH * MAPW - 1].type = END;
			putimage(BP + PGL * (MAPW - 1), BP + PGL * (MAPH - 1), 50, 50, &roundnew2, 0, 0, SRCAND);
			putimage(BP + PGL * (MAPW - 1), BP + PGL * (MAPH - 1), 50, 50, &roundnew1, 0, 0, SRCPAINT);
		}
		if ((x <bossx + 4 * BP && x>bossx - 3 * BP &&
			y<bossy + 4 * BP && y>bossy - 2 * BP) || (
				x <bossx1 + 4 * BP && x>bossx1 - 3 * BP &&
				y<bossy1 + 4 * BP && y>bossy1 - 2 * BP))
		{
			goal = 0;
		}

		if (_kbhit())
		{
			Keydown(pmap);
			putimage(0, 0, &bkimg);
			drawmap(pmap);
			drawplayer();
			drawboss1();
			drawboss2();
			NumIndex = (++NumIndex) % 4;
			if (tishi == 1)
			{
				DrawRoad(pmap);
			}
		}
		drawtishi();
		MYTIME mytime;
		TCHAR strtime[9];
		timeend = clock();
		timeiff = (timeend - start) / 1000;
		detime = timeiff;
		mytime.hour = timeiff / 3600;
		mytime.minute = timeiff % 3600 / 60;
		mytime.second = timeiff % 60;
		settextstyle(30, 0, _T("楷体"));
		settextcolor(WHITE);
		outtextxy(770, 60, _T("时间："));
		_sntprintf_s(strtime, sizeof(strtime), _T("%02d:%02d:%02d"), mytime.hour, mytime.minute, mytime.second);
		outtextxy(860, 60, strtime);
		EndBatchDraw();
	}
	if (detime <= 60)
	{
		jifen3 = 4000;
	}
	else if (detime > 60 && detime < 120)
	{
		jifen3 = 4000 - 24 * (detime - 60);
	}
	else if (detime >= 120 && detime <= 180)
	{
		jifen3 = 2560 - 16 * (detime - 120);
	}
	else if (detime > 180 && detime <= 300)
	{
		jifen3 = 1600 - 8 * (detime - 180);
	}
	else if (detime > 300)
	{
		jifen3 = 600;
	}
	if (goal == 1&&HP>0)
	{
		mciSendString(_T("close mymusic"), NULL, 0, NULL);
		goal = 1;
		x = BP;
		y = BP;
		fen = 0;
		win3(jifen3+200*HP+400*TL, pmap);
	}
	else if (goal == 0||HP<=0)
	{
		mciSendString(_T("close mymusic"), NULL, 0, NULL);
		goal = 1;
		x = BP;
		y = BP;
		fen = 0;
		fail3(pmap);
	}
}
void begingame4(PGRID pmap)
{
	putimage(0, 0, &story8);
	Sleep(5000);
	mciSendString(_T("open ./gamemusic.mp3 alias mymusic"), NULL, 0, NULL);
	mciSendString(_T("play mymusic repeat"), NULL, 0, NULL);
	start = clock();
	while ((x != MAPW * PGL - 3 * BP || y != MAPH * PGL - 4 * BP) && goal != 0&&HP>0)
	{
		Sleep(30);
		bossslow(pmap);
		bossfast(pmap);
		BeginBatchDraw();
		cleardevice();
		putimage(0, 0, &bkimg);
		drawmap4(pmap);
		if (tishi == 1)
		{
			DrawRoad(pmap);
		}
		drawplayer();
		drawboss1();
		drawboss2();
		waittime++;
		waittime1++;
		if (waittime == 200)
		{
			int r = createfastfood(pmap);
			if (r != 0)
			{
				pmap[r].type = FASTFOOD;
			}
		}
		if (usetime > 0)
		{
			BC = 20;
			usetime--;
		}
		else if (usetime == 0)
		{
			BC = 10;
			waittime = 0;
			usetime--;
		}
		if (waittime1 == 200)
		{
			int r = createbox(pmap);
			if (r != 0)
			{
				pmap[r].type = BOX;
			}
		}
		if (usetime1 > 0)
		{
			usetime1--;
		}
		else if (usetime1 == 0)
		{
			waittime1 = 0;
			usetime1--;
		}
		if (fen == MAXFOOD4)
		{
			pmap[MAPH * MAPW - 2].type = BLANK;
			pmap[(MAPH - 1) * MAPW - 1].type = BLANK;
			pmap[(MAPH - 1) * MAPW - 2].type = BLANK;
			pmap[MAPH * MAPW - 1].type = END;
			putimage(BP + PGL * (MAPW - 1), BP + PGL * (MAPH - 1), 50, 50, &flower2, 0, 0, SRCAND);
			putimage(BP + PGL * (MAPW - 1), BP + PGL * (MAPH - 1), 50, 50, &flower1, 0, 0, SRCPAINT);
		}
		if ((x <bossx + 4 * BP && x>bossx - 3 * BP &&
			y<bossy + 4 * BP && y>bossy - 2 * BP) || (
				x <bossx1 + 4 * BP && x>bossx1 - 3 * BP &&
				y<bossy1 + 4 * BP && y>bossy1 - 2 * BP))
		{
			goal = 0;
		}

		if (_kbhit())
		{
			Keydown(pmap);
			putimage(0, 0, &bkimg);
			drawmap4(pmap);
			drawplayer();
			drawboss1();
			drawboss2();
			NumIndex = (++NumIndex) % 4;
			if (tishi == 1)
			{
				DrawRoad(pmap);
			}
		}
		drawtishi();
		MYTIME mytime;
		TCHAR strtime[9];
		timeend = clock();
		timeiff = (timeend - start) / 1000;
		detime = timeiff;
		mytime.hour = timeiff / 3600;
		mytime.minute = timeiff % 3600 / 60;
		mytime.second = timeiff % 60;
		settextstyle(30, 0, _T("楷体"));
		settextcolor(WHITE);
		outtextxy(770, 60, _T("时间："));
		_sntprintf_s(strtime, sizeof(strtime), _T("%02d:%02d:%02d"), mytime.hour, mytime.minute, mytime.second);
		outtextxy(860, 60, strtime);
		EndBatchDraw();
	}
	if (detime <= 60)
	{
		jifen4 = 6000;
	}
	else if (detime > 60 && detime < 120)
	{
		jifen4 = 6000 - 36 * (detime - 60);
	}
	else if (detime >= 120 && detime <= 180)
	{
		jifen4 = 3840 - 24 * (detime - 120);
	}
	else if (detime > 180)
	{
		goal = 1;
		x = BP;
		y = BP;
		fen = 0;
		fail4(pmap);
	}
	if (goal == 1&&HP>0)
	{
		mciSendString(_T("close mymusic"), NULL, 0, NULL);
		goal = 1;
		x = BP;
		y = BP;
		fen = 0;
		win4(jifen4+300*HP+600*TL, pmap);
	}
	else if (goal == 0||HP<=0)
	{
		mciSendString(_T("close mymusic"), NULL, 0, NULL);
		goal = 1;
		x = BP;
		y = BP;
		fen = 0;
		fail4(pmap);
	}
}
void fail1(PGRID pmap)
{
	mciSendString(_T("open ./lose.mp3 alias lose"), NULL, 0, NULL);
	mciSendString(_T("play lose"), NULL, 0, NULL);
	cleardevice();
	waittime = 0;
	waittime1 = 0;
	putimage(0, 0, &lose);
	while (1)
	{
		if (MouseHit())
		{
			MOUSEMSG m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_LBUTTONDOWN:
				if (m.x >= 60 && m.x <= 280 && m.y >= 470 && m.y <= 540)
				{
					mciSendString(_T("close lose"), NULL, 0, NULL);
					bossx1 = BP;
					bossy1 = (MAPH - 1) * PGL;
					cleardevice();
					initmap(pmap);//初始化地图
					player(pmap);
					createwall(pmap);
					PGRID pbegin = &pmap[0];
					PGRID pend = &pmap[MAPH * MAPW - 1];
					//避免生成无解的地图
					while (getRoad(pmap, pbegin, pend) != true ) //getRoad(pmap,pbegin,pend*/是A*算法入口，将计算过后的路径信息保存在pmap中
					{
						initmap(pmap);
						player(pmap);
						createwall(pmap);
					}
					pmap[MAPH * MAPW - 2].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 1].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 2].type = HANDRAIL;
					drawplayer();
					drawboss2();
					createfood1(pmap);
					createtrap1(pmap);
					HP = 7;
					TL = 3;
					begingame1(pmap);
					_getch();
					break;
					//选择简单模式跳转简单地图，下面同理
				}
				if (m.x >= 390 && m.x <= 600 && m.y >= 470 && m.y <= 540)
				{
					mciSendString(_T("close lose"), NULL, 0, NULL);
					cleardevice();
					choose(pmap);
					_getch();
					break;
				}
				if (m.x >= 720 && m.x <= 940 && m.y >= 470 && m.y <= 540)
				{
					mciSendString(_T("close lose"), NULL, 0, NULL);
					closegraph();
					exit(0);
				}
			}
		}
	}
}
void win1(int x, PGRID pmap)
{
	putimage(0, 0, &story3);
	Sleep(2000);
	_getch();
	cleardevice();
	waittime = 0;
	waittime1 = 0;
	mciSendString(_T("open ./win.mp3 alias win"), NULL, 0, NULL);
	mciSendString(_T("play win"), NULL, 0, NULL);
	putimage(0, 0, &win);
	TCHAR u[9];
	settextstyle(40, 0, _T("楷体"));
	settextcolor(BLACK);
	outtextxy(420, 200, _T("游戏得分"));
	_sntprintf_s(u, 20, _T("%d"), x);
	outtextxy(460, 250, u);
	jifen1 = 2000;
	while (1)
	{
		if (MouseHit())
		{
			MOUSEMSG m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_LBUTTONDOWN:
				if (m.x >= 80 && m.x <= 280 && m.y >= 460 && m.y <= 500)
				{
					mciSendString(_T("close win"), NULL, 0, NULL);
					cleardevice();
					bossx = (MAPW - 1) * PGL;
					bossy = BP;
					bossx1 = 900;
					bossy1 = 100;
					initmap(pmap);//初始化地图
					player(pmap);
					createwall(pmap);
					PGRID pbegin = &pmap[0];
					PGRID pend = &pmap[MAPH * MAPW - 1];
					//避免生成无解的地图
					while (getRoad(pmap, pbegin, pend) != true ) //getRoad(pmap,pbegin,pend*/是A*算法入口，将计算过后的路径信息保存在pmap中
					{
						initmap(pmap);
						player(pmap);
						createwall(pmap);
					}
					pmap[MAPH * MAPW - 2].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 1].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 2].type = HANDRAIL;
					drawplayer();
					drawboss1();
					createfood2(pmap);
					createtrap2(pmap);
					HP = 7;
					TL = 3;
					begingame2(pmap);
					_getch();
					break;
					//选择简单模式跳转简单地图，下面同理
				}
				if (m.x >= 420 && m.x <= 580 && m.y >= 460 && m.y <= 500)
				{
					mciSendString(_T("close win"), NULL, 0, NULL);
					cleardevice();
					choose(pmap);
					_getch();
					break;
				}
				if (m.x >= 720 && m.x <= 920 && m.y >= 460 && m.y <= 500)
				{
					mciSendString(_T("close win"), NULL, 0, NULL);
					closegraph();
					exit(0);
				}
			}
		}
	}
}
void fail2(PGRID pmap)
{
	mciSendString(_T("open ./lose.mp3 alias lose"), NULL, 0, NULL);
	mciSendString(_T("play lose"), NULL, 0, NULL);
	cleardevice();
	waittime = 0;
	waittime1 = 0;
	putimage(0, 0, &lose);
	while (1)
	{
		if (MouseHit())
		{
			MOUSEMSG m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_LBUTTONDOWN:
				if (m.x >= 60 && m.x <= 280 && m.y >= 470 && m.y <= 540)
				{
					mciSendString(_T("close lose"), NULL, 0, NULL);
					cleardevice();
					bossx = (MAPW - 1) * PGL;
					bossy = BP;
					initmap(pmap);//初始化地图
					player(pmap);
					createwall(pmap);
					PGRID pbegin = &pmap[0];
					PGRID pend = &pmap[MAPH * MAPW - 1];
					//避免生成无解的地图
					while (getRoad(pmap, pbegin, pend) != true) //getRoad(pmap,pbegin,pend*/是A*算法入口，将计算过后的路径信息保存在pmap中
					{
						initmap(pmap);
						player(pmap);
						createwall(pmap);
					}
					pmap[MAPH * MAPW - 2].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 1].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 2].type = HANDRAIL;
					drawplayer();
					drawboss1();
					createfood2(pmap);
					createtrap2(pmap);
					HP = 7;
					TL = 3;
					begingame2(pmap);
					_getch();
					break;
					//选择简单模式跳转简单地图，下面同理
				}
				if (m.x >= 390 && m.x <= 600 && m.y >= 470 && m.y <= 540)
				{
					mciSendString(_T("close lose"), NULL, 0, NULL);
					cleardevice();
					choose(pmap);
					_getch();
					break;
				}
				if (m.x >= 720 && m.x <= 940 && m.y >= 470 && m.y <= 540)
				{
					mciSendString(_T("close lose"), NULL, 0, NULL);
					closegraph();
					exit(0);
				}
			}
		}
	}
}
void win2(int x, PGRID pmap)
{
	putimage(0, 0, &story5);
	Sleep(2000);
	_getch();
	cleardevice();
	waittime = 0;
	waittime1 = 0;
	mciSendString(_T("open ./win.mp3 alias win"), NULL, 0, NULL);
	mciSendString(_T("play win"), NULL, 0, NULL);
	putimage(0, 0, &win);
	TCHAR u[9];
	settextstyle(40, 0, _T("楷体"));
	settextcolor(BLACK);
	outtextxy(420, 200, _T("游戏得分"));
	_sntprintf_s(u, 20, _T("%d"), x);
	outtextxy(460, 250, u);
	jifen2 = 3000;
	while (1)
	{
		if (MouseHit())
		{
			MOUSEMSG m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_LBUTTONDOWN:
				if (m.x >= 80 && m.x <= 280 && m.y >= 460 && m.y <= 500)
				{
					mciSendString(_T("close win"), NULL, 0, NULL);
					bossx = (MAPW - 1) * PGL;
					bossy = BP;
					bossx1 = BP;
					bossy1 = (MAPH - 1) * PGL;
					cleardevice();
					initmap(pmap);//初始化地图
					player(pmap);
					createwall(pmap);
					PGRID pbegin = &pmap[0];
					PGRID pend = &pmap[MAPH * MAPW - 1];
					//避免生成无解的地图
					while (getRoad(pmap, pbegin, pend) != true ) //getRoad(pmap,pbegin,pend*/是A*算法入口，将计算过后的路径信息保存在pmap中
					{
						initmap(pmap);
						player(pmap);
						createwall(pmap);
					}
					pmap[MAPH * MAPW - 2].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 1].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 2].type = HANDRAIL;
					drawplayer();
					drawboss1();
					drawboss2();
					createfood3(pmap);
					createtrap3(pmap);
					HP = 7;
					TL = 2;
					begingame3(pmap);
					_getch();
					break;
					//选择简单模式跳转简单地图，下面同理
				}
				if (m.x >= 420 && m.x <= 580 && m.y >= 460 && m.y <= 500)
				{
					mciSendString(_T("close win"), NULL, 0, NULL);
					cleardevice();
					choose(pmap);
					_getch();
					break;
				}
				if (m.x >= 720 && m.x <= 920 && m.y >= 460 && m.y <= 500)
				{
					mciSendString(_T("close win"), NULL, 0, NULL);
					closegraph();
					exit(0);
				}
			}
		}
	}
}
void win3(int x, PGRID pmap)
{
	putimage(0, 0, &story7);
	Sleep(2000);
	_getch();
	cleardevice();
	waittime = 0;
	waittime1 = 0;
	mciSendString(_T("open ./win.mp3 alias win"), NULL, 0, NULL);
	mciSendString(_T("play win"), NULL, 0, NULL);
	putimage(0, 0, &win);
	TCHAR u[9];
	settextstyle(40, 0, _T("楷体"));
	settextcolor(BLACK);
	outtextxy(420, 200, _T("游戏得分"));
	_sntprintf_s(u, 20, _T("%d"), x);
	outtextxy(460, 250, u);
	jifen3 = 4000;
	while (1)
	{
		if (MouseHit())
		{
			MOUSEMSG m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_LBUTTONDOWN:
				bossx = (MAPW - 1) * PGL;
				bossy = BP;
				bossx1 = BP;
				bossy1 = (MAPH - 1) * PGL;
				if (m.x >= 80 && m.x <= 280 && m.y >= 460 && m.y <= 500)
				{
					mciSendString(_T("close win"), NULL, 0, NULL);
					cleardevice();
					initmap(pmap);//初始化地图
					player(pmap);
					createwall(pmap);
					PGRID pbegin = &pmap[0];
					PGRID pend = &pmap[MAPH * MAPW - 1];
					//避免生成无解的地图
					while (getRoad(pmap, pbegin, pend) != true ) //getRoad(pmap,pbegin,pend*/是A*算法入口，将计算过后的路径信息保存在pmap中
					{
						initmap(pmap);
						player(pmap);
						createwall(pmap);
					}
					pmap[MAPH * MAPW - 2].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 1].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 2].type = HANDRAIL;
					drawplayer();
					drawboss1();
					drawboss2();
					createfood4(pmap);
					createtrap4(pmap);
					HP = 7;
					TL = 2;
					begingame4(pmap);
					_getch();
					break;
					//选择简单模式跳转简单地图，下面同理
				}
				if (m.x >= 420 && m.x <= 580 && m.y >= 460 && m.y <= 500)
				{
					mciSendString(_T("close win"), NULL, 0, NULL);
					cleardevice();
					choose(pmap);
					_getch();
					break;
				}
				if (m.x >= 720 && m.x <= 920 && m.y >= 460 && m.y <= 500)
				{
					mciSendString(_T("close win"), NULL, 0, NULL);
					closegraph();
					exit(0);
				}
			}
		}
	}
}
void win4(int x, PGRID pmap)
{
	putimage(0, 0, &story9);
	Sleep(2000);
	_getch();
	cleardevice();
	waittime = 0;
	waittime1 = 0;
	mciSendString(_T("open ./win.mp3 alias win"), NULL, 0, NULL);
	mciSendString(_T("play win"), NULL, 0, NULL);
	putimage(0, 0, &pend);
	TCHAR u[9];
	settextstyle(40, 0, _T("楷体"));
	settextcolor(BLACK);
	outtextxy(420, 200, _T("游戏得分"));
	_sntprintf_s(u, 20, _T("%d"), x);
	outtextxy(460, 250, u);
	jifen4 = 6000;
	while (1)
	{
		if (MouseHit())
		{
			MOUSEMSG m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_LBUTTONDOWN:
				if (m.x >= 80 && m.x <= 280 && m.y >= 460 && m.y <= 500)
				{
					mciSendString(_T("close win"), NULL, 0, NULL);
					cleardevice();
					bossx = (MAPW - 1) * PGL;
					bossy = BP;
					bossx1 = BP;
					bossy1 = (MAPH - 1) * PGL;
					initmap(pmap);//初始化地图
					player(pmap);
					createwall(pmap);
					PGRID pbegin = &pmap[0];
					PGRID pend = &pmap[MAPH * MAPW - 1];
					//避免生成无解的地图
					while (getRoad(pmap, pbegin, pend) != true ) //getRoad(pmap,pbegin,pend*/是A*算法入口，将计算过后的路径信息保存在pmap中
					{
						initmap(pmap);
						player(pmap);
						createwall(pmap);
					}
					pmap[MAPH * MAPW - 2].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 1].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 2].type = HANDRAIL;
					drawplayer();
					drawboss1();
					drawboss2();
					createfood4(pmap);
					createtrap4(pmap);
					HP = 7;
					TL = 2;
					begingame4(pmap);
					_getch();
					break;
					//选择简单模式跳转简单地图，下面同理
				}
				if (m.x >= 420 && m.x <= 580 && m.y >= 460 && m.y <= 500)
				{
					mciSendString(_T("close win"), NULL, 0, NULL);
					cleardevice();
					choose(pmap);
					_getch();
					break;
				}
				if (m.x >= 720 && m.x <= 920 && m.y >= 460 && m.y <= 500)
				{
					mciSendString(_T("close win"), NULL, 0, NULL);
					closegraph();
					exit(0);
				}
			}
		}
	}
}
void fail3(PGRID pmap)
{
	mciSendString(_T("open ./lose.mp3 alias lose"), NULL, 0, NULL);
	mciSendString(_T("play lose"), NULL, 0, NULL);
	cleardevice();
	waittime = 0;
	waittime1 = 0;
	putimage(0, 0, &lose);
	while (1)
	{
		if (MouseHit())
		{
			MOUSEMSG m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_LBUTTONDOWN:
				if (m.x >= 60 && m.x <= 280 && m.y >= 470 && m.y <= 540)
				{
					mciSendString(_T("close lose"), NULL, 0, NULL);
					cleardevice();
					bossx = (MAPW - 1) * PGL;
					bossy = BP;
					bossx1 = BP;
					bossy1 = (MAPH - 1) * PGL;
					initmap(pmap);//初始化地图
					player(pmap);
					createwall(pmap);
					PGRID pbegin = &pmap[0];
					PGRID pend = &pmap[MAPH * MAPW - 1];
					//避免生成无解的地图
					while (getRoad(pmap, pbegin, pend) != true ) //getRoad(pmap,pbegin,pend*/是A*算法入口，将计算过后的路径信息保存在pmap中
					{
						initmap(pmap);
						player(pmap);
						createwall(pmap);
					}
					pmap[MAPH * MAPW - 2].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 1].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 2].type = HANDRAIL;
					drawplayer();
					drawboss2();
					drawboss1();
					createfood3(pmap);
					createtrap3(pmap);
					HP = 7;
					TL = 2;
					begingame3(pmap);
					_getch();
					break;
					//选择简单模式跳转简单地图，下面同理
				}
				if (m.x >= 390 && m.x <= 600 && m.y >= 470 && m.y <= 540)
				{
					mciSendString(_T("close lose"), NULL, 0, NULL);
					cleardevice();
					choose(pmap);
					_getch();
					break;
				}
				if (m.x >= 720 && m.x <= 940 && m.y >= 470 && m.y <= 540)
				{
					mciSendString(_T("close lose"), NULL, 0, NULL);
					closegraph();
					exit(0);
				}
			}
		}
	}
}
void fail4(PGRID pmap)
{
	mciSendString(_T("open ./lose.mp3 alias lose"), NULL, 0, NULL);
	mciSendString(_T("play lose"), NULL, 0, NULL);
	cleardevice();
	waittime = 0;
	waittime1 = 0;
	putimage(0, 0, &lose);
	while (1)
	{
		if (MouseHit())
		{
			MOUSEMSG m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_LBUTTONDOWN:
				if (m.x >= 60 && m.x <= 280 && m.y >= 470 && m.y <= 540)
				{
					mciSendString(_T("close lose"), NULL, 0, NULL);
					cleardevice();
					bossx = (MAPW - 1) * PGL;
					bossy = BP;
					bossx1 = BP;
					bossy1 = (MAPH - 1) * PGL;
					initmap(pmap);//初始化地图
					player(pmap);
					createwall(pmap);
					PGRID pbegin = &pmap[0];
					PGRID pend = &pmap[MAPH * MAPW - 1];
					//避免生成无解的地图
					while (getRoad(pmap, pbegin, pend) != true ) //getRoad(pmap,pbegin,pend*/是A*算法入口，将计算过后的路径信息保存在pmap中
					{
						initmap(pmap);
						player(pmap);
						createwall(pmap);
					}
					pmap[MAPH * MAPW - 2].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 1].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 2].type = HANDRAIL;
					drawplayer();
					drawboss2();
					drawboss1();
					createfood4(pmap);
					createtrap4(pmap);
					HP = 7;
					TL = 2;
					begingame4(pmap);
					_getch();
					break;
					//选择简单模式跳转简单地图，下面同理
				}
				if (m.x >= 390 && m.x <= 600 && m.y >= 470 && m.y <= 540)
				{
					mciSendString(_T("close lose"), NULL, 0, NULL);
					cleardevice();
					choose(pmap);
					_getch();
					break;
				}
				if (m.x >= 720 && m.x <= 940 && m.y >= 470 && m.y <= 540)
				{
					mciSendString(_T("close lose"), NULL, 0, NULL);
					closegraph();
					exit(0);
				}
			}
		}
	}
}
void choose(PGRID pmap)
{
	cleardevice();
	putimage(0, 0, &pchoose);
	while (1)
	{
		if (MouseHit())
		{
			mciSendString(_T("stop begingame"), NULL, 0, NULL);
			setlinecolor(WHITE);
			MOUSEMSG m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_LBUTTONDOWN:
				if (m.x >= 100 && m.x <= 260 && m.y >= 440 && m.y <= 510)
				{
					cleardevice();
					bossx1 = BP; bossy1 = (MAPH - 1) * PGL;
					initmap(pmap);//初始化地图
					player(pmap);
					createwall(pmap);
					PGRID pbegin = &pmap[0];
					PGRID pend = &pmap[MAPH * MAPW - 1];
					//避免生成无解的地图
					while (getRoad(pmap, pbegin, pend) != true ) //getRoad(pmap,pbegin,pend*/是A*算法入口，将计算过后的路径信息保存在pmap中
					{
						initmap(pmap);
						player(pmap);
						createwall(pmap);
					}
					pmap[MAPH * MAPW - 2].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 1].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 2].type = HANDRAIL;
					drawplayer();
					drawboss2();
					createfood1(pmap);
					createtrap1(pmap);
					HP = 7;
					TL = 3;
					begingame1(pmap);
					_getch();
					break;
					//选择简单模式跳转简单地图，下面同理
				}
				if (m.x >= 420 && m.x <= 560 && m.y >= 440 && m.y <= 510)
				{
					cleardevice();
					bossx = (MAPW - 1) * PGL; bossy = BP;
					initmap(pmap);//初始化地图
					player(pmap);
					createwall(pmap);
					PGRID pbegin = &pmap[0];
					PGRID pend = &pmap[MAPH * MAPW - 1];
					//避免生成无解的地图
					
					while (getRoad(pmap, pbegin, pend) != true ) //getRoad(pmap,pbegin,pend*/是A*算法入口，将计算过后的路径信息保存在pmap中
					{
						initmap(pmap);
						player(pmap);
						createwall(pmap);
					}
					pmap[MAPH * MAPW - 2].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 1].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 2].type = HANDRAIL;
					drawplayer();
					drawboss1();
					createfood2(pmap);
					createtrap2(pmap);
					HP = 7;
					TL = 3;
					begingame2(pmap);
					_getch();
					break;
				}
				if (m.x >= 680 && m.x <= 900 && m.y >= 450 && m.y <= 510)
				{
					cleardevice();
					bossx = (MAPW - 1) * PGL; bossy = BP;
					bossx1 = BP; bossy1 = (MAPH - 1) * PGL;
					initmap(pmap);//初始化地图
					player(pmap);
					createwall(pmap);
					PGRID pbegin = &pmap[0];
					PGRID pend = &pmap[MAPH * MAPW - 1];
					//避免生成无解的地图
				
					while (getRoad(pmap, pbegin, pend) != true) //getRoad(pmap,pbegin,pend*/是A*算法入口，将计算过后的路径信息保存在pmap中
					{
						initmap(pmap);
						player(pmap);
						createwall(pmap);

					}
					pmap[MAPH * MAPW - 2].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 1].type = HANDRAIL;
					pmap[(MAPH - 1) * MAPW - 2].type = HANDRAIL;
					drawplayer();
					drawboss1();
					drawboss2();
					createfood3(pmap);
					createtrap3(pmap);
					HP = 7;
					TL = 2;
					begingame3(pmap);
					_getch();
					break;
				}
				if (m.x >= 420 && m.x <= 600 && m.y >= 370 && m.y <= 400)
				{
					closegraph();
					exit(0);
				}
			}
		}
	}
}
int main(void)
{
	PGRID pmap = new GRID[MAPH * MAPW];//开辟内存空间
	//	PGRID pbossmap = pmap;
	//	PGRID pbossmap = new GRID[MAPH*MAPW];
	imgresouse();
	initgraph(1000, 563);
	setbkmode(TRANSPARENT);
	cleardevice();
	putimage(0, 0, &bkimg);
	drawwelcome();
	putimage(0, 0, &story1);
	_getch();
	//	MYROAD mRoad;
	//GRID pCur = *pbegin;//当前人物位置属性
	choose(pmap);
	Sleep(2000);
	_getch();
	closegraph();
	return 0;
}