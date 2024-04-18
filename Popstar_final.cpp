#include <graphics.h>
#include<Windows.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <conio.h>
//#include "tools.h"
#include <mmsystem.h>	//播放背景音乐和音效的头文件
#pragma comment(lib,"winmm.lib")	//播放音乐需要的库文件
#include <tchar.h> 
#pragma comment(lib,"WinMM.lib")
#pragma warning(disable:4996)
#define N 40//方块边长

#define WIN_WIDHT 600
#define WIN_HEIGHT 1000
#define ROWS 10
#define COLS 10
#define BLOCK_TYPE_COUNT 7
#define difficulty 6

IMAGE imaBg;
IMAGE imgBlocks[BLOCK_TYPE_COUNT];

struct block
{
	int type;//表示方块
	int x, y;
	int row, col;
	int match;
	int tmd;//透明度:0-255,0完全透明
	int exist;

};
struct block map[ROWS + 2][COLS + 2];
const int off_x = 75;
const int off_y = 340;
const int blocks_size = 45;
const int gap_size = 0;

int TargetScore;
int GameLevel;

int Click1, Click2, Click3;//技能：锤子，重排，交换
int posX1, posY1;
int changeX1, changeY1, changeX2, changeY2;
int Flag1;
bool propsing;
bool isMoving;
bool swap;
bool proper;
int score;
int userover;
int starLeave;
int click1 = 0;//锤子
int click2 = 0;//刷新
int click3 = 0;//交换
int flag1 = 0;//工具使用

//库内定义
IMAGE background_image;//游戏页面背景图片
IMAGE RegbgImage;//注册页面背景图片
IMAGE nowImage;  //当前
MOUSEMSG m;    //鼠标 
//MOUSEMSG m;
COLORREF colorArr[6] = { RGB(200, 0, 0), RGB(0, 200, 0),RGB(0,0,200),RGB(0, 200, 200), RGB(200, 0, 200),RGB(200,200,0) };//颜色池（红，绿，蓝，青，紫，黄）

//结构体定义
typedef struct position//位置结构体
{
	int x;
	int y;
}pos;     //位置信息

//全局变量
pos mou;    //鼠标位置结构体
pos sameArr[100];   //同色方块坐标
int index = 0;    //同色方块个数
int mark = 0;//积分
int level = 1;//关卡数
int target = 0;//目标分
int levelTemp = 1;//关卡数储存
int GamePhase;	//处于哪一个界面

//函数声明
void Game(char c);//光明模式
void loadData();//读取数据
void recordData();//保存数据
void game(void);    //初始化游戏界面
void playgame(void);   //游戏玩法
int GetSameColor(pos, COLORREF);//获取同颜色的方块
int IsTheSame(pos, COLORREF);   //判断方块附近颜色是否一样
void falling(void);         //方块下落移动
void Checking(void);//检查残局并结束游戏
void StartDraw();//开始界面
void MainMenuOp();//主菜单操作
void StartGameOp();//开始游戏操作
void KeepGameOp();//继续游戏操作
void HelpOp();//帮助操作
void OverOp();//结束操作
void Start();//开始游戏界面
void Keep();//继续游戏界面
void help();//帮助界面
void mouse();	//鼠标操作
void Game1();//游戏-无尽模式
void popstar();//游戏主体
void InitMap();//初始化map
void init(char c);//初始化方块


//主程序
int main()
{

	initgraph(640, 500);//创建窗体
	srand((unsigned)time(NULL));//随机种子
	rectangle(10, 10, 630, 470);//创建矩形

	// 获取窗口句柄
	HWND hwnd = GetHWnd();

	// 设置窗口标题文字
	SetWindowText(hwnd, "Popstar! 消灭星星!");

	//初始化菜单界面
	StartDraw();

	//界面操控标识
	GamePhase = 0;

	//游戏循环
	while (1)
	{

		m = GetMouseMsg();//获取鼠标操作
		mouse();

	}

	getch();
	closegraph();

}

//开始界面绘制
void StartDraw()
{

	//标题
	settextstyle(52, 30, _T("幼圆"));//字的长度（52） 和 宽度（30）
	settextcolor(RGB(255, 255, 0));
	outtextxy(40, 40, _T("Popstar！消灭星星☆"));

	//作者
	settextstyle(20, 10, _T("幼圆"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(300, 103, _T("小组成员：周毅康、易梦哲、高祥"));

	//开始游戏
	rectangle(240, 150, 365, 185);
	settextstyle(22, 12, _T("微软雅黑"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(250, 155, _T("开始游戏"));

	//继续游戏
	rectangle(240, 230, 365, 265);
	settextstyle(22, 12, _T("微软雅黑"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(250, 235, _T("继续游戏"));

	//帮助
	rectangle(240, 310, 365, 345);
	settextstyle(22, 12, _T("微软雅黑"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(250, 315, _T("游戏帮助"));

	//退出游戏
	rectangle(240, 390, 365, 425);
	settextstyle(22, 12, _T("微软雅黑"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(250, 395, _T("退出游戏"));

	//环境
	settextcolor(RGB(255, 255, 255));
	outtextxy(90, 460, _T("Programing by Visual Studio && EasyX"));

}

//鼠标操作
void mouse()
{

	//音乐
	mciSendString("open res/menuBg.mp3 alias menuBg", 0, 0, 0);
	mciSendString("play menuBg repeat", 0, 0, 0);
	mciSendString("setaudio menuBg volume to 100", 0, 0, 0);

	//主菜单界面
	if (GamePhase == 0)
	{

		//主菜单操作
		MainMenuOp();
		
	}

	//开始游戏
	if (GamePhase == 1)
	{

		//开始游戏操作
		StartGameOp();

	}

	//继续游戏
	if (GamePhase == 2)
	{

		//继续游戏操作
		KeepGameOp();

	}

	//帮助界面
	if (GamePhase == 3)
	{

		//帮助操作
		HelpOp();

	}

	//结束界面
	if (GamePhase == 4)
	{

		//结束操作
		OverOp();

	}

	//清除鼠标消息队列
	FlushMouseMsgBuffer();

}

//开始游戏界面
void Start()
{

	//光明模式
	rectangle(250, 190, 365, 225);
	settextstyle(22, 12, _T("微软雅黑"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(260, 195, _T("光明模式"));

	//无尽模式
	rectangle(250, 270, 365, 305);
	settextstyle(22, 12, _T("微软雅黑"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(260, 275, _T("无尽模式"));

	//返回
	rectangle(250, 350, 325, 385);
	settextstyle(22, 12, _T("微软雅黑"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(260, 355, _T("返回"));

}

//继续游戏界面
void Keep()
{

	//光明模式
	rectangle(250, 190, 365, 225);
	settextstyle(22, 12, _T("微软雅黑"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(260, 195, _T("光明模式"));

	//无尽模式
	rectangle(250, 270, 365, 305);
	settextstyle(22, 12, _T("微软雅黑"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(260, 275, _T("无尽模式"));

	//返回
	rectangle(250, 350, 325, 385);
	settextstyle(22, 12, _T("微软雅黑"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(260, 355, _T("返回"));

}

//帮助界面
void help()
{

	//提示语
	settextstyle(30, 0, _T("微软雅黑"));
	outtextxy(210, 150, _T("没有帮助，自己摸索去吧！"));

	//返回
	rectangle(250, 360, 325, 395);
	settextstyle(22, 12, _T("微软雅黑"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(260, 365, _T("返回"));

}

//游戏模式-无尽模式
void Game1()
{

	//初始化窗口
	initgraph(700, 700);

	//游戏初始界面
	game();

	//游戏运行
	playgame();

	//界面暂停，不退出
	system("pause");

}


//设置起全文字体的函数
void setFont(const char* fontName, int fontWidth, int fontHeight)
{

	LOGFONT f;
	gettextstyle(&f);

	f.lfHeight = fontHeight;                      // 设置字体高度为 48
	f.lfWidth = fontWidth;

	strcpy(f.lfFaceName, fontName);    // 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	setbkmode(TRANSPARENT);
	settextstyle(&f);

}


void init(char c)
{
	initgraph(WIN_WIDHT, WIN_HEIGHT);

	loadimage(&imaBg, "res/gamebg.png");

	char name[64];

	for (int i = 0; i < BLOCK_TYPE_COUNT; i++)
	{

		sprintf_s(name, sizeof(name), "res/%d.png", i + 1);
		loadimage(&imgBlocks[i], name, blocks_size, blocks_size, true);

	}

	srand(time(NULL));

	if (c == 0)
	{

		InitMap();

	}

	isMoving = false;
	propsing = false;
	Click1 = 0;
	Click2 = 0;
	Click3 = 0;
	swap = false;
	proper = false;
	Flag1 = 1;

	if (c == 0)
	{

		score = 0;
		starLeave = 50;

	}

	userover = 0;
	setFont("Segoe UI Black", 20, 40);  //设置全文字体

	//播放背景音乐


	mciSendString("open res/bg.mp3 alias bgm1", 0, 0, 0);
	mciSendString("play bgm1 repeat", 0, 0, 0);
	mciSendString("setaudio bgm1 volume to 60", 0, 0, 0);
	mciSendString("open res/start.mp3 alias startbgm", 0, 0, 0);
	mciSendString("play startbgm", 0, 0, 0);
	mciSendString("setaudio startbgm volume to 60", 0, 0, 0);




}


void InitMap() 
{

	for (int i = 1; i <= ROWS; i++)
	{

		for (int j = 1; j <= COLS; j++)
		{

			map[i][j].type = rand() % difficulty + 1;
			map[i][j].row = i;
			map[i][j].col = j;
			map[i][j].x = off_x + (j - 1) * (blocks_size + gap_size);
			map[i][j].y = off_y + (i - 1) * (blocks_size + gap_size);
			map[i][j].match = 0;
			map[i][j].tmd = 255;
			map[i][j].exist = 1;

		}

	}

}


void putimageTMD(int  picture_x, int picture_y, IMAGE* picture, int tmd) //x为载入图片的X坐标，y为Y坐标
{

	if (picture_y < 0)return;

	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针

	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{

		for (int ix = 0; ix < picture_width; ix++)
		{

			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度

			if (tmd < 255)
			{

				if (sa > 200) sa = tmd;
				else continue;

			}

			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B

			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{

				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标

				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;

				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)
					| (sb * sa / 255 + db * (255 - sa) / 255);

			}

		}

	}

}


void recordData()
{
	FILE* inFile = fopen("gamedata.dat", "wb");

	fwrite(map, sizeof(struct block), (COLS + 2) * (ROWS + 2), inFile);
	fwrite(&score, sizeof(int), 1, inFile);
	fwrite(&starLeave, sizeof(int), 1, inFile);
	fwrite(&GameLevel, sizeof(int), 1, inFile);
	fclose(inFile);

}


void loadData()
{
	FILE* tmpFile = fopen("gamedata.dat", "rb");

	fread(map, sizeof(struct block), (COLS + 2) * (ROWS + 2), tmpFile);
	fread(&score, sizeof(int), 1, tmpFile);
	fread(&starLeave, sizeof(int), 1, tmpFile);
	fread(&GameLevel, sizeof(int), 1, tmpFile);
	fclose(tmpFile);

}


void updateWindow()
{

	BeginBatchDraw();
	putimage(0, 0, &imaBg);

	for (int i = 1; i <= ROWS; i++)
	{

		for (int j = 1; j <= COLS; j++)
		{

			if (map[i][j].type && map[i][j].exist)
			{

				IMAGE* img = &imgBlocks[map[i][j].type - 1];
				putimageTMD(map[i][j].x, map[i][j].y, img, map[i][j].tmd);//使图像拥有透明度

			}

		}

	}


	char levelStr[16];
	sprintf_s(levelStr, sizeof(levelStr), "%d", GameLevel);
	int x1 = 100;
	outtextxy(x1, 48, levelStr);


	char targerStr[16];
	sprintf_s(targerStr, sizeof(targerStr), "%d", TargetScore);
	int x2 = 257 + (110 - strlen(targerStr) * 20) / 2;
	outtextxy(x2, 44, targerStr);


	char scoreStr[16];
	sprintf_s(scoreStr, sizeof(scoreStr), "%d", score);
	int x = 204 + (144 - strlen(scoreStr) * 20) / 2;
	outtextxy(x, 128, scoreStr);


	char starStr[16];
	sprintf_s(starStr, sizeof(starStr), "%d", starLeave);
	int x3 = 476 + (62 - strlen(starStr) * 20) / 2;
	outtextxy(x3, 129, starStr);

	EndBatchDraw();

}


void exchange(int row1, int col1, int row2, int col2)
{

	struct block temp = map[row1][col1];

	map[row1][col1] = map[row2][col2];
	map[row2][col2] = temp;

	map[row1][col1].row = row1;
	map[row1][col1].col = col1;
	map[row2][col2].row = row2;
	map[row2][col2].col = col2;

}


bool isInRect(ExMessage* msg, int x, int y, int w, int h)
{

	if (msg->x > x && msg->x<x + w && msg->y>y && msg->y < y + h)
	{

		return true;

	}

	return false;

}


void userClick()
{

	ExMessage msg;

	if (peekmessage(&msg))
	{

		if (msg.message == WM_RBUTTONDOWN)
		{

			Click1 = Click2 = Click3 = 0;

		}

		if (msg.message == WM_LBUTTONDOWN)
		{

			if (isInRect(&msg, 28, 133, 43, 39))
			{

				userover = 1;
				recordData();

			}
			else if (isInRect(&msg, 543, 137, 37, 37))
			{
				system("start https://www.bilibili.com/video/BV1J4411v7g6/?vd_source=4a9f5fae5a1a17a41db46af9fb8d7a5c");
			}
			else if (isInRect(&msg, 305, 226, 75, 75) && starLeave >= 10)
			{
				Click1 = 1;
				Click2 = Click3 = 0;

			}
			else if (isInRect(&msg, 405, 226, 75, 75) && starLeave >= 25)
			{

				Click2 = 1;
				Click1 = Click3 = 0;

			}
			else if (isInRect(&msg, 505, 226, 75, 75) && starLeave >= 10)
			{

				Click3 = 1;
				Click1 = Click2 = 0;

			}
			else
			{

				if (msg.x < off_x || msg.y < off_y) return;

				int col = (msg.x - off_x) / (blocks_size + gap_size) + 1;
				int row = (msg.y - off_y) / (blocks_size + gap_size) + 1;

				if (col > COLS || row > ROWS || map[row][col].exist == 0) return;

				if (Click1 == 1 && map[row][col].exist)
				{
					PlaySound("res/clear.wav", 0, SND_FILENAME | SND_ASYNC);
					starLeave = starLeave - 10;
					map[row][col].match = 1;
					map[row][col].tmd = 0;
					score = score + 10;
					Click1 = 0;
					propsing = false;
					return;

				}
				else if (Click3 >= 1)
				{

					if (map[row][col].exist)  Click3++;

					if (Click3 == 2)
					{

						changeX1 = row;
						changeY1 = col;
						return;

					}
					else if (Click3 == 3)
					{
						PlaySound("res/blockChange.wav", 0, SND_FILENAME | SND_ASYNC);



						starLeave = starLeave - 10;
						changeX2 = row;
						changeY2 = col;

						int temp;
						temp = map[changeX1][changeY1].type;

						map[changeX1][changeY1].type = map[changeX2][changeY2].type;
						map[changeX2][changeY2].type = temp;

						Click3 = 0;
						propsing = false;



						return;

					}

				}
				else
				{

					swap = true;

					posX1 = row;
					posY1 = col;

					PlaySound("res/pao.wav", 0, SND_FILENAME | SND_ASYNC);

					return;

				}

			}

		}

	}

	if (Click2)
	{


		PlaySound("res/colorChange.wav", 0, SND_FILENAME | SND_ASYNC);

		starLeave = starLeave - 25;

		for (int i = ROWS; i >= 1; i--)
		{

			for (int j = 1; j <= COLS; j++)
			{

				if (map[i][j].exist)
				{

					map[i][j].type = rand() % difficulty + 1;

				}

			}

		}

		Click2 = 0;
		propsing = false;

	}

}


void move()
{

	isMoving = false;

	for (int i = ROWS; i >= 1; i--)
	{

		for (int j = 1; j <= COLS; j++)
		{

			struct block* p = &map[i][j];
			int dx, dy;

			for (int k = 0; k < 4; k++)
			{

				int x = off_x + (p->col - 1) * (blocks_size + gap_size);
				int y = off_y + (p->row - 1) * (blocks_size + gap_size);

				dx = p->x - x;
				dy = p->y - y;

				if (dx) p->x -= dx / abs(dx);
				if (dy) p->y -= dy / abs(dy);

			}

			if (dx || dy) isMoving = true;
		}

	}

}


void check(int posX1, int posY1)
{

	int i;

	for (i = 0; i < 4; i++)
	{

		switch (i)
		{

		case 0:
			if ((map[posX1][posY1].type == map[posX1 - 1][posY1].type) && (map[posX1 - 1][posY1].exist == 1) && (map[posX1 - 1][posY1].match == 0))
			{

				map[posX1 - 1][posY1].match = 1;
				score = score + 10;
				check(posX1 - 1, posY1);

			}
			break;

		case 1:
			if ((map[posX1][posY1].type == map[posX1 + 1][posY1].type) && (map[posX1 + 1][posY1].exist == 1) && (map[posX1 + 1][posY1].match == 0))
			{

				map[posX1 + 1][posY1].match = 1;
				score = score + 10;
				check(posX1 + 1, posY1);

			}
			break;

		case 2:
			if ((map[posX1][posY1].type == map[posX1][posY1 - 1].type) && (map[posX1][posY1 - 1].exist == 1) && (map[posX1][posY1 - 1].match == 0))
			{

				map[posX1][posY1 - 1].match = 1;
				score = score + 10;
				check(posX1, posY1 - 1);

			}
			break;

		case 3:
			if ((map[posX1][posY1].type == map[posX1][posY1 + 1].type) && (map[posX1][posY1 + 1].exist == 1) && (map[posX1][posY1 + 1].match == 0))
			{

				map[posX1][posY1 + 1].match = 1;
				score = score + 10;
				check(posX1, posY1 + 1);

			}
			break;

		}

	}

	swap = false;

}


void xiaochu()
{
	bool flag = false;

	for (int i = 1; i <= ROWS; i++)
	{

		for (int j = 1; j <= COLS; j++)
		{

			if (map[i][j].match && map[i][j].tmd > 10 && map[i][j].exist)
			{

				swap = false;
				isMoving = true;

				if (map[i][j].tmd == 255)
				{

					flag = true;

				}

				map[i][j].tmd -= 25;

			}

			if (map[i][j].tmd <= 10)
			{

				map[i][j].tmd = 0;
				map[i][j].exist = 0;

			}

		}

	}

	if (flag)
	{

		PlaySound("res/clear.wav", 0, SND_FILENAME | SND_ASYNC);

	}

}


void updateGame1()
{

	int i, j;

	for (i = ROWS; i >= 1; i--)
	{

		for (j = 1; j <= COLS; j++)
		{

			if (map[i][j].match)
			{

				for (int k = i - 1; k >= 1; k--)
				{

					if (map[k][j].match == 0)
					{

						isMoving = true;
						exchange(k, j, i, j);
						break;

					}

				}

			}

		}

	}

	if (i == 1 && j == COLS)
	{

		isMoving = false;

	}

}


void updateGame2(int i)
{

	int k;

	for (k = i + 1; k <= COLS; k++)
	{

		if (map[ROWS][k].exist)
		{

			break;

		}

	}

	for (int j = ROWS; j >= 1; j--)
	{

		if (map[j][k].exist)
		{

			isMoving = true;
			exchange(j, i, j, k);

		}

	}

}


void merge()
{

	int i, j;

	for (i = 1; i <= COLS; i++)
	{

		int counter = 0;

		for (j = ROWS; j >= 1; j--)
		{

			if (map[j][i].exist) break;
			counter++;

		}

		if (counter == ROWS)
		{

			updateGame2(i);

		}

	}

	if (i == COLS && j == 1)
	{

		isMoving = false;

	}

}


void EndDetection()//残局检测
{

	for (int i = ROWS; i >= 1; i--)
	{

		for (int j = 1; j <= COLS; j++)
		{

			if (map[i][j].exist)
			{

				if (map[i - 1][j].type == map[i][j].type && map[i - 1][j].exist)
				{

					goto end;

				}

				if (map[i + 1][j].type == map[i][j].type && map[i + 1][j].exist)
				{

					goto end;

				}

				if (map[i][j - 1].type == map[i][j].type && map[i][j - 1].exist)
				{

					goto end;

				}

				if (map[i][j + 1].type == map[i][j].type && map[i][j + 1].exist)
				{

					goto end;

				}

			}

		}

	}

	Flag1 = 0;
	Sleep(2000);

end:return;

}


void Game(char c)
{

	if (c == 0) GameLevel = 1;

	while (GameLevel <= 11 || !userover)
	{

		TargetScore = 500 + (GameLevel - 1) * 50;

		init(c);

		while (Flag1 == 1)
		{

			userClick();  //用户点击（包括道具）

			if (userover == 1)   //如果用户游戏中退出，则存档
			{

				setfillcolor(RGB(0, 0, 0));
				fillrectangle(0, 0, 600, 1066);//黑色背景
				settextstyle(40, 0, _T(""));
				settextcolor(RGB(255, 0, 0));
				outtextxy(60, 350, _T("About to exit and archive\n"));
				Sleep(2000);
				mciSendString("stop bgm1", 0, 0, 0);
				initgraph(640, 500);
				StartDraw();
				GamePhase = 0;

				goto end;

			}

			if (swap) check(posX1, posY1); //如果是正常点击（非道具）则递归检查

			xiaochu();  //消除
			updateWindow();	//刷新屏幕
			updateGame1();  //下降
			move();   //移动

			if (!isMoving) merge();	//左移

			if (!isMoving) EndDetection();  //残局检测

			if (isMoving) Sleep(5);

		}

		if (score >= TargetScore)
		{

			c = 0;
			GameLevel++;

			setfillcolor(RGB(0, 0, 0));
			fillrectangle(0, 0, 600, 1066);//黑色背景
			settextstyle(40, 0, _T(""));
			settextcolor(RGB(255, 0, 0));
			outtextxy(140, 350, _T("Congratuations!\n"));

			Sleep(2000);

		}
		else
		{

			c = 0;

			setfillcolor(RGB(0, 0, 0));
			fillrectangle(0, 0, 600, 1066);//黑色背景
			settextstyle(40, 0, _T(""));
			settextcolor(RGB(255, 0, 0));
			outtextxy(150, 350, _T("Gameover! Loser!\n"));
			InitMap();
			score = 0;
			starLeave = 50;
			recordData();

			Sleep(2000);
			mciSendString("stop bgm1", 0, 0, 0);

			setfillcolor(RGB(0, 0, 0));
			fillrectangle(0, 0, 700, 1066);//黑色背景
			settextstyle(40, 0, _T(""));
			settextcolor(RGB(255, 0, 0));
			outtextxy(60, 350, _T("About to exit and archive\n"));

			Sleep(2000);

			initgraph(640, 500);
			StartDraw();
			GamePhase = 0;

			goto end;

		}

	}

end:return;

}

//游戏全局
void popstar()
{
	//启动窗口
	initgraph(640, 500);

	//设置随机种子
	srand((unsigned)time(NULL));

	//绘制矩形
	rectangle(10, 10, 630, 470);

	// 获取窗口句柄
	HWND hwnd = GetHWnd();

	// 设置窗口标题文字
	SetWindowText(hwnd, "Popstar! 消灭星星!");

	//画主菜单界面
	StartDraw();

	//主菜单模式
	GamePhase = 0;

	//游戏循环
	while (1)
	{

		//获取鼠标状态
		m = GetMouseMsg();

		//执行鼠标操作
		mouse();

	}

	getch();
	closegraph();

}

//初始化游戏界面
void game(void)
{

	//积分设置
	target = 300 + 100 * level;

	loadimage(&background_image, "E:\BK1.png", 700, 700);//取背景图
	putimage(0, 0, &background_image);//放置背景图

	setfillcolor(RGB(0, 0, 0));//填充颜色-黑色
	fillrectangle(100, 0, 600, 700);//黑色背景
	settextstyle(20, 0, _T(""));//字体长度-宽度
	settextcolor(RGB(255, 255, 255));//文本颜色-白色
	outtextxy(200, 5, _T("Hello,welcome to our popstar!"));//输出游戏提示语

	settextstyle(15, 0, _T(""));//字体长度-宽度
	settextcolor(RGB(255, 255, 255));//文本颜色-白色
	outtextxy(200, 30, _T("关卡数："));//输出游戏提示语
	outtextxy(200, 50, _T("目标得分："));//输出游戏提示语
	outtextxy(200, 70, _T("当前得分："));//输出游戏提示语

	//关卡数
	setlinecolor(RGB(0, 0, 0));//设置画笔颜色
	setfillcolor(RGB(0, 0, 0));//填充颜色-黑色
	fillrectangle(260, 30, 500, 50);//黑色背景
	settextstyle(15, 0, _T(""));//字体长度-宽度
	settextcolor(RGB(255, 255, 255));//文本颜色-白色

	level = levelTemp;
	char levelstr[16];
	sprintf_s(levelstr, sizeof(levelstr), "%d", level);
	outtextxy(260, 30, levelstr);//输出游戏提示语

	//目标分
	setlinecolor(RGB(0, 0, 0));//设置画笔颜色
	setfillcolor(RGB(0, 0, 0));//填充颜色-黑色
	fillrectangle(280, 50, 500, 70);//黑色背景
	settextstyle(15, 0, _T(""));//字体长度-宽度
	settextcolor(RGB(255, 255, 255));//文本颜色-白色

	char targetstr[16];
	sprintf_s(targetstr, sizeof(targetstr), "%d", target);
	outtextxy(280, 50, targetstr);//输出游戏提示语

	//积分
	setlinecolor(RGB(0, 0, 0));//设置画笔颜色
	setfillcolor(RGB(0, 0, 0));//填充颜色-黑色
	fillrectangle(280, 70, 590, 90);//黑色背景
	settextstyle(15, 0, _T(""));//字体长度-宽度
	settextcolor(RGB(255, 255, 255));//文本颜色-白色

	char markstr[16];
	sprintf_s(markstr, sizeof(markstr), "%d", mark);
	outtextxy(280, 70, markstr);//输出游戏提示语

	//退出按钮
	setlinecolor(RGB(255, 255, 255));//设置画笔颜色
	rectangle(10, 8, 60, 30);
	settextstyle(18, 12, _T("微软雅黑"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(12, 10, _T("退出"));

	//随机种子
	srand((unsigned)time(NULL));

	//生成方块
	for (int i = 0; i < 10; i++)//行-y方向
	{

		for (int j = 0; j < 10; j++)//列-x方向
		{

			COLORREF cl = colorArr[rand() % 6];     //产生0~6六个随机数在随即颜色池中取颜色

			setfillcolor(cl);//填充颜色
			setlinecolor(RGB(0, 0, 0));//画笔颜色-黑
			fillroundrect(150 + j * N, 150 + i * N, 150 + N + j * N, 150 + (i + 1) * N, 10, 10);//画矩形

			settextstyle(30, 0, _T(""));
			setbkmode(TRANSPARENT);//设置透明背景
			settextcolor(RGB(255, 255, 255));//设置文字颜色
			outtextxy(155 + j * N, 155 + i * N, _T("☆"));//在方块中央画星星

		}

	}

	//锤子按钮
	COLORREF cx = RGB(255, 0, 0);     //产生0~6六个随机数在随即颜色池中取颜色                      
	setfillcolor(cx);//填充颜色

	setlinecolor(RGB(0, 0, 0));//画笔颜色-黑
	fillroundrect(150 + 2 * N, 150 + 11 * N, 150 + 3 * N, 150 + 12 * N, 10, 10);//画矩形
	setlinecolor(RGB(255, 255, 255));//设置画笔颜色

	//rectangle(10, 8, 60, 30);
	settextstyle(20, 8, _T("微软雅黑"));
	settextcolor(RGB(0, 0, 0));
	outtextxy(150 + 2 * N + 5, 150 + 11 * N + 10, _T("锤子"));

	//重排按钮
	COLORREF cy = RGB(150, 255, 0);     //产生0~6六个随机数在随即颜色池中取颜色                      
	setfillcolor(cy);//填充颜色

	setlinecolor(RGB(0, 0, 0));//画笔颜色-黑
	fillroundrect(150 + 4 * N, 150 + 11 * N, 150 + 5 * N, 150 + 12 * N, 10, 10);//画矩形
	setlinecolor(RGB(255, 255, 255));//设置画笔颜色

	//rectangle(10, 8, 60, 30);
	settextstyle(20, 8, _T("微软雅黑"));
	settextcolor(RGB(0, 0, 0));
	outtextxy(150 + 4 * N + 5, 150 + 11 * N + 10, _T("重排"));

	//交换按钮
	COLORREF cz = RGB(0, 150, 255);     //产生0~6六个随机数在随即颜色池中取颜色
	setfillcolor(cz);//填充颜色

	setlinecolor(RGB(0, 0, 0));//画笔颜色-黑
	fillroundrect(150 + 6 * N, 150 + 11 * N, 150 + 7 * N, 150 + 12 * N, 10, 10);//画矩形
	setlinecolor(RGB(255, 255, 255));//设置画笔颜色

	//rectangle(10, 8, 60, 30);
	settextstyle(20, 8, _T("微软雅黑"));
	settextcolor(RGB(0, 0, 0));
	outtextxy(150 + 6 * N + 5, 150 + 11 * N + 10, _T("交换"));

}

//游戏运行
void playgame(void)
{
	mciSendString("open res/happy.mp3 alias bgm2", 0, 0, 0);
	mciSendString("play bgm2 repeat", 0, 0, 0);
	mciSendString("setaudio bgm2 volume to 100", 0, 0, 0);
	COLORREF c2 = RGB(0, 0, 0);//定义黑色

	int change1 = 0;
	int change2 = 0;

	COLORREF block1;
	COLORREF block2;

	pos temp1;
	pos temp2;

	while (1)//循环获取游戏每时刻的状态
	{

		//获取鼠标状态
		m = GetMouseMsg();

		switch (m.uMsg)//多case；保留后续增加游戏功能
		{

			//左键单击按下
		case WM_LBUTTONDOWN:

			//获取鼠标点击位置
			mou.x = m.x;
			mou.y = m.y;

			//标准化
			mou.x = (mou.x - 150) / N * N + 150 + 20;
			mou.y = (mou.y - 150) / N * N + 150 + 20;

			//使用道具
			if (flag1)
			{

				//使用锤子
				if (click1)
				{

					//提示音
					PlaySound("res/clear.wav", 0, SND_FILENAME | SND_ASYNC);

					int test1 = 0;

					COLORREF  cl = getpixel(mou.x, mou.y); //获取当前位置的颜色

					if (cl != RGB(0, 0, 0)) //如果颜色是白色
					{

						cl = RGB(0, 0, 0); //将颜色改为黑色

					}

					for (int r = 0; r < 6; r++)
					{

						if (getpixel(mou.x, mou.y) == colorArr[r])  //如果某方块本身在颜色池内
						{

							test1 = 1;//测试成功

						}

					}

					if (mou.x <= 550 && mou.y <= 550 && test1)
					{

						setfillcolor(cl); //设置填充颜色
						setcolor(RGB(0, 0, 0)); //设置画笔颜色
						fillroundrect((mou.x - 150) / N * N + 150, (mou.y - 150) / N * N + 150, (mou.x - 150) / N * N + N + 150, (mou.y - 150) / N * N + N + 150, 10, 10); //绘制矩形

						sameArr[index].x = (mou.x);
						sameArr[index].y = (mou.y);
						index++;//移动索引扩容

						//下落和左移
						falling();

						index = 0;
						mark += 10;

						setfillcolor(RGB(0, 0, 0));//填充颜色-黑色
						fillrectangle(280, 70, 590, 90);//黑色背景
						settextstyle(15, 0, _T(""));//字体长度-宽度
						settextcolor(RGB(255, 255, 255));//文本颜色-白色

						//改变积分
						char markstr[16];
						sprintf_s(markstr, sizeof(markstr), "%d", mark);
						outtextxy(280, 70, markstr);//输出游戏提示语

						flag1 = 0;
						click1 = 0;

						Checking();//检验残局并结束游戏

						break;
						
					}

				}

				//使用重排
				else if (click2)
				{

					//提示音
					PlaySound("res/colorChange.wav", 0, SND_FILENAME | SND_ASYNC);

					pos each;//判断色块的位置
					pos temp;//临时存储pos型

					int i = 0;
					int j = 0;

					for (i = 170; i < 550; i += N) //遍历每一列
					{

						for (j = 170; j < 550; j += N)
						{

							each.x = i;
							each.y = j;

							for (int r = 0; r < 6; r++)
							{

								if (getpixel(each.x, each.y) == colorArr[r])  //如果某方块本身在颜色池内
								{

									COLORREF cl = colorArr[rand() % 6];     //产生0~6六个随机数在随即颜色池中取颜色                      
									setfillcolor(cl);//填充颜色

									setlinecolor(RGB(0, 0, 0));//画笔颜色-黑
									fillroundrect(i - 20, j - 20, i + 20, j + 20, 10, 10);//画矩形

									settextstyle(30, 0, _T(""));
									setbkmode(TRANSPARENT);//设置透明背景
									settextcolor(RGB(255, 255, 255));//设置文字颜色
									outtextxy(i - 15, j - 15, _T("☆"));//在方块中央画星星

								}

							}

						}

					}

					//检验残局并结束游戏
					Checking();

					//延时
					Sleep(50);

					flag1 = 0;
					click2 = 0;

					break;

				}

				//使用交换
				else if (click3)
				{

					//未选中方块
					if (!change1)
					{

						if (mou.x < 550 && mou.y < 550)
						{

							temp1.x = mou.x;
							temp1.y = mou.y;
							block1 = getpixel(mou.x, mou.y);
							change1 = 1;

						}

					}

					//选中一个方块
					else if (change1)
					{

						if (mou.x < 550 && mou.y < 550)
						{

							//提示音
							PlaySound("res/blockChange.wav", 0, SND_FILENAME | SND_ASYNC);

							temp2.x = mou.x;
							temp2.y = mou.y;
							block2 = getpixel(mou.x, mou.y);

							setfillcolor(block1);//填充颜色
							setlinecolor(RGB(0, 0, 0));//画笔颜色-黑
							fillroundrect(temp2.x - 20, temp2.y - 20, temp2.x + 20, temp2.y + 20, 10, 10);//画矩形

							settextstyle(30, 0, _T(""));
							setbkmode(TRANSPARENT);//设置透明背景
							settextcolor(RGB(255, 255, 255));//设置文字颜色
							outtextxy(temp2.x - 15, temp2.y - 15, _T("☆"));//在方块中央画星星

							setfillcolor(block2);//填充颜色
							setlinecolor(RGB(0, 0, 0));//画笔颜色-黑
							fillroundrect(temp1.x - 20, temp1.y - 20, temp1.x + 20, temp1.y + 20, 10, 10);//画矩形

							settextstyle(30, 0, _T(""));
							setbkmode(TRANSPARENT);//设置透明背景
							settextcolor(RGB(255, 255, 255));//设置文字颜色
							outtextxy(temp1.x - 15, temp1.y - 15, _T("☆"));//在方块中央画星星

							Checking();//检验残局并结束游戏
							Sleep(50);//延时

							change1 = 0;
							flag1 = 0;
							click3 = 0;
						}

					}

					break;

				}

			}

			else
			{

				if (m.x >= 10 && m.x <= 60 && m.y >= 8 && m.y <= 30)	//退出
				{

					//提示音
					mciSendString("stop bgm2", 0, 0, 0);

					//储存关卡数
					levelTemp = level;

					//closegraph();
					//initgraph(640, 500);

					setfillcolor(RGB(0, 0, 0));
					fillrectangle(0, 0, 700, 1066);//黑色背景

					settextstyle(40, 0, _T(""));
					settextcolor(RGB(255, 0, 0));
					outtextxy(80, 350, _T("About to exit and archive\n"));

					//延时
					Sleep(2000);

					//重新作图
					initgraph(640, 500);

					//初始菜单界面
					StartDraw();

					//主菜单模式
					GamePhase = 0;

					//重开游戏
					popstar();

					break;

				}

				//执行锤子
				else if (mou.x == 150 + 2 * N + 20 && mou.y == 150 + 11 * N + 20)
				{

					click1 = 1;
					flag1 = 1;

					break;

				}

				//执行重排
				else if (mou.x == 150 + 4 * N + 20 && mou.y == 150 + 11 * N + 20)
				{

					click2 = 1;
					flag1 = 1;

					break;

				}

				//执行交换
				else if (mou.x == 150 + 6 * N + 20 && mou.y == 150 + 11 * N + 20)
				{

					click3 = 1;
					flag1 = 1;

					break;

				}

				//正常游戏
				else
				{

					int a = 0;//初始化颜色污染测试标志

					for (int i = 0; i < 6; i++)
					{

						if (getpixel(mou.x, mou.y) == colorArr[i])//如果鼠标位置颜色在颜色池内，检验颜色范围，防止黑色污染
						{

							a = 1;//颜色未污染

						}

					}

					//颜色未污染
					if (a == 1)
					{

						//提示音
						PlaySound("res/pao.wav", 0, SND_FILENAME | SND_ASYNC);

						//获取周围同色方块
						GetSameColor(mou, getpixel(mou.x, mou.y));

						//存在同色方块
						if (index > 1)
						{

							int j = 0;//初始化索引

							for (j = 0; j < index - 1; j++)
							{

								setlinecolor(RGB(0, 0, 0));//设置画笔颜色
								setfillcolor(RGB(0, 0, 0));//设置填充颜色
								fillroundrect(sameArr[j].x - 20, sameArr[j].y - 20, sameArr[j].x + 20, sameArr[j].y + 20, 10, 10);

							}

							//提示音
							PlaySound("res/clear.wav", 0, SND_FILENAME | SND_ASYNC);

							//消除方块并下落左移
							falling();

							//积分
							mark += 10 * index;

							setfillcolor(RGB(0, 0, 0));//填充颜色-黑色
							fillrectangle(280, 70, 590, 90);//黑色背景
							settextstyle(15, 0, _T(""));//字体长度-宽度
							settextcolor(RGB(255, 255, 255));//文本颜色-白色

							char markstr[16];
							sprintf_s(markstr, sizeof(markstr), "%d", mark);
							outtextxy(280, 70, markstr);//输出游戏提示语

							//检验残局并结束游戏
							Checking();

							//延时
							Sleep(150);

						}

						index = 0;//重置同色块数量索引

						break;

					}

				}

			}

			//鼠标移动
		case WM_MOUSEMOVE:
			if (m.x >= 10 && m.x <= 60 && m.y >= 8 && m.y <= 30)	//退出
			{

				//退出
				settextstyle(18, 12, _T("微软雅黑"));
				settextcolor(RGB(255, 255, 255));
				outtextxy(12, 10, _T("退出"));

				//锤子
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(0, 0, 0));
				outtextxy(150 + 2 * N + 5, 150 + 11 * N + 10, _T("锤子"));

				//重排
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(0, 0, 0));
				outtextxy(150 + 4 * N + 5, 150 + 11 * N + 10, _T("重排"));

				//交换
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(0, 0, 0));
				outtextxy(150 + 6 * N + 5, 150 + 11 * N + 10, _T("交换"));

				break;

			}

			//锤子
			else if (m.x >= 150 + 2 * N && m.x <= 150 + 3 * N && m.y >= 150 + 11 * N && m.y <= 150 * 12 * N)
			{

				//退出
				setlinecolor(RGB(255, 255, 255));//设置画笔颜色
				rectangle(10, 8, 60, 30);
				settextstyle(18, 12, _T("微软雅黑"));
				settextcolor(RGB(255, 51, 68));
				outtextxy(12, 10, _T("退出"));

				//锤子
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(255, 255, 255));
				outtextxy(150 + 2 * N + 5, 150 + 11 * N + 10, _T("锤子"));

				//重排
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(0, 0, 0));
				outtextxy(150 + 4 * N + 5, 150 + 11 * N + 10, _T("重排"));

				//交换
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(0, 0, 0));
				outtextxy(150 + 6 * N + 5, 150 + 11 * N + 10, _T("交换"));

				break;

			}

			//重排
			else if (m.x >= 150 + 4 * N && m.x <= 150 + 5 * N && m.y >= 150 + 11 * N && m.y <= 150 * 12 * N)
			{

				//退出
				settextstyle(18, 12, _T("微软雅黑"));
				settextcolor(RGB(255, 51, 68));
				outtextxy(12, 10, _T("退出"));

				//锤子
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(0, 0, 0));
				outtextxy(150 + 2 * N + 5, 150 + 11 * N + 10, _T("锤子"));

				//重排
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(255, 255, 255));
				outtextxy(150 + 4 * N + 5, 150 + 11 * N + 10, _T("重排"));

				//交换
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(0, 0, 0));
				outtextxy(150 + 6 * N + 5, 150 + 11 * N + 10, _T("交换"));

				break;

			}

			//交换
			else if (m.x >= 150 + 6 * N && m.x <= 150 + 7 * N && m.y >= 150 + 11 * N && m.y <= 150 * 12 * N)
			{

				//退出
				settextstyle(18, 12, _T("微软雅黑"));
				settextcolor(RGB(255, 51, 68));
				outtextxy(12, 10, _T("退出"));

				//锤子
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(0, 0, 0));
				outtextxy(150 + 2 * N + 5, 150 + 11 * N + 10, _T("锤子"));

				//重排
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(0, 0, 0));
				outtextxy(150 + 4 * N + 5, 150 + 11 * N + 10, _T("重排"));

				//交换
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(255, 255, 255));
				outtextxy(150 + 6 * N + 5, 150 + 11 * N + 10, _T("交换"));

				break;

			}

			//取消选中
			else
			{

				//退出
				settextstyle(18, 12, _T("微软雅黑"));
				settextcolor(RGB(255, 51, 68));
				outtextxy(12, 10, _T("退出"));

				//锤子
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(0, 0, 0));
				outtextxy(150 + 2 * N + 5, 150 + 11 * N + 10, _T("锤子"));

				//重排
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(0, 0, 0));
				outtextxy(150 + 4 * N + 5, 150 + 11 * N + 10, _T("重排"));

				//交换
				settextstyle(20, 8, _T("微软雅黑"));
				settextcolor(RGB(0, 0, 0));
				outtextxy(150 + 6 * N + 5, 150 + 11 * N + 10, _T("交换"));

			}

			break;

		}

	}

}

//获取同颜色的方块
int GetSameColor(pos mou, COLORREF cl)//传入：鼠标位置、颜色
{

	int k = 0;//初始化索引

	pos temp;//pos型常量的临时存储

	//规整化鼠标点击位置（便于构造方块）
	mou.x = (mou.x - 150) / N * N + 150 + 20;
	mou.y = (mou.y - 150) / N * N + 150 + 20;

	//扩充存储同色数组
	sameArr[index].x = (mou.x);
	sameArr[index].y = (mou.y);

	//移动索引扩容
	index++;

	for (k = 0; k < 4; k++) //向不同方向搜索
	{

		switch (k)
		{

		case 0:temp.x = mou.x; temp.y = mou.y - N; break; //上
		case 1:temp.x = mou.x; temp.y = mou.y + N; break; //下
		case 2:temp.x = mou.x - N; temp.y = mou.y; break; //左
		case 3:temp.x = mou.x + N; temp.y = mou.y; break; //右

		}

		if (IsTheSame(temp, cl) == 1) //附近色块同色判断
		{

			GetSameColor(temp, cl);//移动判断色块位置，类似递归

		}

	}

	return index - 1;//返回相邻同色块数量

}

//判断方块附近颜色是否一样
int IsTheSame(pos mou, COLORREF cl)
{

	if (getpixel(mou.x, mou.y) != cl)
	{

		return 0; //不一样

	}

	//颜色一样
	else
	{

		int i = 0;

		for (i = 0; i < index; i++)
		{

			if (mou.x == sameArr[i].x && mou.y == sameArr[i].y)
			{

				return 0; //不一样

			}

		}

		return 1;  //一样

	}

}

//方块下落移动和左移
void falling()//消除和移动
{

	//索引
	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;
	int p = 0;
	int r = 0;
	int q = 0;

	pos temp;//定义一个pos类型的临时存储变量temp

	//色块数据规整化
	for (i = 0; i < index - 1; i++)                                  //使小坐标排在数组前面
		for (j = 0; j < index - 1 - i; j++)                          //冒泡法
		{

			if (sameArr[j].x > sameArr[j + 1].x)
			{

				temp = sameArr[j];//交换位置信息
				sameArr[j] = sameArr[j + 1];
				sameArr[j + 1] = temp;

			}

			if (sameArr[j].y > sameArr[j + 1].y)
			{

				temp = sameArr[j];//交换位置信息
				sameArr[j] = sameArr[j + 1];
				sameArr[j + 1] = temp;

			}

		}

	for (i = 0; i < index; i++)       //下落
	{

		for (k = sameArr[i].y; k > 50; k -= N)//从当前位置开始，每次向下移动N个单位
		{

			COLORREF  cl = getpixel(sameArr[i].x, k - N); //获取当前位置的颜色

			//如果颜色是白色
			if (cl == RGB(255, 255, 255))
			{

				//将颜色改为黑色
				cl = RGB(0, 0, 0);

			}

			setfillcolor(cl); //设置填充颜色
			setcolor(RGB(0, 0, 0)); //设置画笔颜色

			//绘制矩形
			fillroundrect((sameArr[i].x - 150) / N * N + 150, (k - 150) / N * N + 150, (sameArr[i].x - 150) / N * N + N + 150, (k - 150) / N * N + N + 150, 10, 10);

			settextstyle(30, 0, _T("")); //设置文本样式
			setbkmode(TRANSPARENT); //设置背景模式

			//如果颜色不是黑色
			if (cl != RGB(0, 0, 0))
			{

				//将文本颜色改为白色
				settextcolor(RGB(255, 255, 255));

				//在矩形中心绘制星星
				outtextxy((sameArr[i].x - 150) / N * N + 155, (k - 150) / N * N + 155, _T("☆"));

			}

		}

	}

	//遍历所有列
	for (q = 0; q < 10; q++)
	{

		for (i = 170; i < 510; i += N) //遍历每一列
		{

			for (j = 170; j < 550; j += N) //遍历每一行
			{

				if (getpixel(i, j) == RGB(0, 0, 0))  //如果当前位置是黑色
				{

					m = m + 1; //黑色计数器加1

				}

			}

			if (m == 10)//如果某一列全是黑色
			{

				for (i; i < 510; i += N)  //将这一列的所有方块左移
				{

					for (p = 170; p < 550; p += N)
					{

						COLORREF c3 = getpixel(i + N, p); //获取下一个方块的颜色
						setfillcolor(c3); //设置填充颜色

						settextcolor(RGB(255, 255, 255)); //设置文本颜色

						//绘制矩形
						fillroundrect(i - 20, p - 20, i + 20, p + 20, 10, 10);

						settextstyle(30, 0, _T("")); //设置文本样式
						setbkmode(TRANSPARENT); //设置背景模式

						//不为黑色
						if (c3 != RGB(0, 0, 0))
						{

							outtextxy(i - 20 + 5, p - 20 + 5, _T("☆")); //在矩形中心绘制星星

						}

					}

				}

				for (r = 170; r < 550; r += N) //最后一列变黑
				{

					setfillcolor(RGB(0, 0, 0)); //设置填充颜色为黑色

					//绘制矩形
					fillroundrect(510, r - 20, 510 + N, r + N, 10, 10);

				}

			}

			m = 0; //重置计数器

		}

	}

}

//残局检测和结束游戏
void Checking(void)
{

	pos each;//判断色块的位置
	pos temp;//临时存储pos型

	//初始化索引
	int q = 0;
	int i = 0;
	int j = 0;
	int k = 0;

	int test = 0;//初始设置测试成功

	for (q = 0; q < 10; q++)//遍历所有列
	{

		for (i = 170; i < 550; i += N) //遍历每一列
		{

			for (j = 170; j < 550; j += N)
			{

				each.x = i;
				each.y = j;

				for (k = 0; k < 4; k++) //向不同方向搜索
				{

					switch (k)
					{

					case 0:temp.x = each.x; temp.y = each.y - N; break; //上
					case 1:temp.x = each.x; temp.y = each.y + N; break; //下
					case 2:temp.x = each.x - N; temp.y = each.y; break; //左
					case 3:temp.x = each.x + N; temp.y = each.y; break; //右

					}

					for (int r = 0; r < 6; r++)
					{

						if (getpixel(each.x, each.y) == colorArr[r] && getpixel(temp.x, temp.y) == getpixel(each.x, each.y))  //如果某方块本身在颜色池内，且与四面方块都同色
						{

							test = 1;//测试失败

						}

					}

				}

			}

		}

	}

	//检测不通过
	if (!test)
	{

		//彩蛋
		if (mark >= 1000)
		{

			setfillcolor(RGB(0, 0, 0));
			fillrectangle(100, 0, 600, 700);//黑色背景
			settextstyle(40, 0, _T(""));
			settextcolor(RGB(255, 0, 0));
			outtextxy(80, 300, _T("Congratulations! Easter Egg!\n"));
			Sleep(2000);

			//链接
			system("start https://www.bilibili.com/video/BV16M411R7Tz/?spm_id_from=333.337.search-card.all.click&vd_source=c8acd38a861a3a3898e5cf5e3549a8c5");

		}

		//通关
		else if (mark >= target)
		{

			level++;
			levelTemp++;
			mark = 0;

			Game1();

		}

		//失败
		else
		{

			//音乐
			mciSendString("stop bgm2", 0, 0, 0);

			setfillcolor(RGB(0, 0, 0));
			fillrectangle(100, 0, 600, 700);//黑色背景

			settextstyle(40, 0, _T(""));
			settextcolor(RGB(255, 0, 0));
			outtextxy(180, 300, _T("Gameover! Loser!\n"));

			//延时
			Sleep(2000);


		}

	}

}


//主菜单处理
void MainMenuOp()
{

	if (m.uMsg == WM_MOUSEMOVE)//鼠标移动
	{

		if (m.x >= 240 && m.x <= 365 && m.y >= 150 && m.y <= 185)	//开始游戏
		{

			//标题
			settextstyle(52, 30, _T("幼圆"));//字的长度（52） 和 宽度（30）
			settextcolor(RGB(255, 255, 0));
			outtextxy(40, 40, _T("Popstar！消灭星星☆"));

			//作者
			settextstyle(20, 10, _T("幼圆"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(300, 103, _T("小组成员：周毅康、易梦哲、高祥"));

			//开始游戏
			settextstyle(22, 12, _T("微软雅黑"));	//选中
			settextcolor(RGB(255, 255, 255));
			outtextxy(250, 155, _T("开始游戏"));

			//恢复其他选中

			//继续游戏
			rectangle(240, 230, 365, 265);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 235, _T("继续游戏"));

			//帮助
			rectangle(240, 310, 365, 345);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 315, _T("游戏帮助"));
			settextcolor(BLACK);
			outtextxy(120, 320, _T("你在逗我？"));
			outtextxy(370, 320, _T("这么简单还要帮助？"));

			//退出
			rectangle(240, 390, 365, 425);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 395, _T("退出游戏"));

		}

		else if (m.x >= 240 && m.x <= 365 && m.y >= 230 && m.y <= 265)	//继续游戏
		{

			//标题
			settextstyle(52, 30, _T("幼圆"));//字的长度（52） 和 宽度（30）
			settextcolor(RGB(255, 255, 0));
			outtextxy(40, 40, _T("Popstar！消灭星星☆"));

			//作者
			settextstyle(20, 10, _T("幼圆"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(300, 103, _T("小组成员：周毅康、易梦哲、高祥"));

			//继续游戏
			settextstyle(22, 12, _T("微软雅黑"));	//选中
			settextcolor(RGB(255, 255, 255));
			outtextxy(250, 235, _T("继续游戏"));

			//恢复其他选中

			//开始游戏
			rectangle(240, 150, 365, 185);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 155, _T("开始游戏"));

			//帮助
			rectangle(240, 310, 365, 345);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 315, _T("游戏帮助"));
			settextcolor(BLACK);
			outtextxy(120, 320, _T("你在逗我？"));
			outtextxy(370, 320, _T("这么简单还要帮助？"));

			//退出
			rectangle(240, 390, 365, 425);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 395, _T("退出游戏"));

		}

		else if (m.x >= 240 && m.x <= 365 && m.y >= 310 && m.y <= 345)	//帮助界面
		{

			//标题
			settextstyle(52, 30, _T("幼圆"));//字的长度（52） 和 宽度（30）
			settextcolor(RGB(255, 255, 0));
			outtextxy(40, 40, _T("Popstar！消灭星星☆"));

			//作者
			settextstyle(20, 10, _T("幼圆"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(300, 103, _T("小组成员：周毅康、易梦哲、高祥"));

			//帮助
			settextstyle(22, 12, _T("微软雅黑"));	//选中
			settextcolor(RGB(255, 255, 255));
			outtextxy(250, 315, _T("游戏帮助"));

			outtextxy(120, 320, _T("你在逗我？"));
			outtextxy(370, 320, _T("这么简单还要帮助？"));

			//恢复其他选中

			//开始游戏
			rectangle(240, 150, 365, 185);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 155, _T("开始游戏"));

			//进入继续游戏
			rectangle(240, 230, 365, 265);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 235, _T("继续游戏"));

			//退出
			rectangle(240, 390, 365, 425);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 395, _T("退出游戏"));

		}

		else if (m.x >= 240 && m.x <= 365 && m.y >= 390 && m.y <= 425)	//退出
		{

			//标题
			settextstyle(52, 30, _T("幼圆"));//字的长度（52） 和 宽度（30）
			settextcolor(RGB(255, 255, 0));
			outtextxy(40, 40, _T("Popstar！消灭星星☆"));

			//作者
			settextstyle(20, 10, _T("幼圆"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(300, 103, _T("小组成员：周毅康、易梦哲、高祥"));

			//退出游戏
			settextstyle(22, 12, _T("微软雅黑"));	//选中
			settextcolor(RGB(255, 255, 255));
			outtextxy(250, 395, _T("退出游戏"));

			//恢复其他选中

			//开始游戏
			rectangle(240, 150, 365, 185);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 155, _T("开始游戏"));

			//继续游戏
			rectangle(240, 230, 365, 265);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 235, _T("继续游戏"));

			//帮助
			rectangle(240, 310, 365, 345);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 315, _T("游戏帮助"));

			settextcolor(BLACK);
			outtextxy(120, 320, _T("你在逗我？"));
			outtextxy(370, 320, _T("这么简单还要帮助？"));

		}

		else	//都没有选中
		{

			//标题
			settextstyle(52, 30, _T("幼圆"));//字的长度（52） 和 宽度（30）
			settextcolor(RGB(255, 255, 0));
			outtextxy(40, 40, _T("Popstar！消灭星星☆"));

			//作者
			settextstyle(20, 10, _T("幼圆"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(300, 103, _T("小组成员：周毅康、易梦哲、高祥"));

			//开始游戏
			rectangle(240, 150, 365, 185);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 155, _T("开始游戏"));

			//继续游戏
			rectangle(240, 230, 365, 265);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 235, _T("继续游戏"));

			//帮助
			rectangle(240, 310, 365, 345);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 315, _T("游戏帮助"));

			settextcolor(BLACK);
			outtextxy(120, 320, _T("你在逗我？"));
			outtextxy(370, 320, _T("这么简单还要帮助？"));

			//退出
			rectangle(240, 390, 365, 425);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 395, _T("退出游戏"));

		}

	}
	else if (m.uMsg == WM_LBUTTONDOWN)//鼠标左键点击
	{

		cleardevice();

		if (m.x >= 240 && m.x <= 365 && m.y >= 150 && m.y <= 185)	//进入游戏界面
		{

			GamePhase = 1;//改变界面标识
			Start();//画开始游戏界面
			//Sleep(1000);

		}

		else if (m.x >= 240 && m.x <= 365 && m.y >= 230 && m.y <= 265)	//进入继续游戏界面
		{

			GamePhase = 2;;//改变界面标识
			Keep();//画继续游戏界面
			//Sleep(1000);

		}

		else if (m.x >= 240 && m.x <= 365 && m.y >= 310 && m.y <= 345)	//进入帮助界面
		{

			GamePhase = 3;;//改变界面标识
			help();//画帮助界面

		}

		else if (m.x >= 240 && m.x <= 365 && m.y >= 390 && m.y <= 425)	//退出游戏
		{

			closegraph();

		}




	}

}


//开始游戏操作
void StartGameOp()
{
	if (m.uMsg == WM_MOUSEMOVE)
	{

		if (m.x >= 250 && m.x <= 365 && m.y >= 270 && m.y <= 305)//无尽模式
		{

			//光明模式
			rectangle(250, 190, 365, 225);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 195, _T("光明模式"));

			//无尽模式
			rectangle(250, 270, 365, 305);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 255, 255));
			outtextxy(260, 275, _T("无尽模式"));

			//返回
			rectangle(250, 350, 325, 385);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 355, _T("返回"));

		}

		else if (m.x >= 250 && m.x <= 365 && m.y >= 190 && m.y <= 225)//光明模式
		{

			//光明模式
			rectangle(250, 190, 365, 225);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 255, 255));
			outtextxy(260, 195, _T("光明模式"));

			//无尽模式
			rectangle(250, 270, 365, 305);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 275, _T("无尽模式"));

			//返回
			rectangle(250, 350, 325, 385);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 355, _T("返回"));

		}

		else if (m.x >= 250 && m.x <= 325 && m.y >= 350 && m.y <= 385)	//返回
		{

			//光明模式
			rectangle(250, 190, 365, 225);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 195, _T("光明模式"));

			//无尽模式
			rectangle(250, 270, 365, 305);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 275, _T("无尽模式"));

			//返回
			rectangle(250, 350, 325, 385);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 255, 255));
			outtextxy(260, 355, _T("返回"));

		}

		else //取消选中
		{

			//光明模式
			rectangle(250, 190, 365, 225);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 195, _T("光明模式"));

			//无尽模式
			rectangle(250, 270, 365, 305);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 275, _T("无尽模式"));

			//返回
			rectangle(250, 350, 325, 385);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 355, _T("返回"));

		}

	}

	else if (m.uMsg == WM_LBUTTONDOWN)//鼠标左键点击
	{

		if (m.x >= 250 && m.x <= 325 && m.y >= 350 && m.y <= 385)//返回
		{

			//清空窗口
			cleardevice();

			//画主菜单界面
			StartDraw();

			//改变标识符状态
			GamePhase = 0;

		}
		else if (m.x >= 250 && m.x <= 365 && m.y >= 190 && m.y <= 225)//光明模式
		{

			//音乐
			mciSendString("stop menuBg", 0, 0, 0);

			//游戏
			Game(0);

		}
		else if (m.x >= 250 && m.x <= 365 && m.y >= 270 && m.y <= 305)//无尽模式
		{

			//修改临时关卡储存值
			levelTemp = 1;

			//音乐
			mciSendString("stop menuBg", 0, 0, 0);

			//游戏
			Game1();

		}

	}

}


//继续游戏操作
void KeepGameOp()
{

	if (m.uMsg == WM_MOUSEMOVE)
	{

		if (m.x >= 250 && m.x <= 365 && m.y >= 270 && m.y <= 305)//无尽模式
		{

			//光明模式
			rectangle(250, 190, 365, 225);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 195, _T("光明模式"));

			//无尽模式
			rectangle(250, 270, 365, 305);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 255, 255));
			outtextxy(260, 275, _T("无尽模式"));

			//返回
			rectangle(250, 350, 325, 385);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 355, _T("返回"));

		}
		else if (m.x >= 250 && m.x <= 365 && m.y >= 190 && m.y <= 225)//光明模式
		{

			//光明模式
			rectangle(250, 190, 365, 225);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 255, 255));
			outtextxy(260, 195, _T("光明模式"));

			//无尽模式
			rectangle(250, 270, 365, 305);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 275, _T("无尽模式"));

			//返回
			rectangle(250, 350, 325, 385);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 355, _T("返回"));

		}
		else if (m.x >= 250 && m.x <= 325 && m.y >= 350 && m.y <= 385)	//返回
		{

			//光明模式
			rectangle(250, 190, 365, 225);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 195, _T("光明模式"));

			//无尽模式
			rectangle(250, 270, 365, 305);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 275, _T("无尽模式"));

			//返回
			rectangle(250, 350, 325, 385);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 255, 255));
			outtextxy(260, 355, _T("返回"));

		}
		else //取消选中
		{

			//光明模式
			rectangle(250, 190, 365, 225);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 195, _T("光明模式"));

			//无尽模式
			rectangle(250, 270, 365, 305);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 275, _T("无尽模式"));

			//返回
			rectangle(250, 350, 325, 385);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 355, _T("返回"));

		}

	}

	if (m.uMsg == WM_LBUTTONDOWN)
	{

		//cleardevice();

		if (m.x >= 250 && m.x <= 325 && m.y >= 350 && m.y <= 385)//返回
		{

			//清空窗口
			cleardevice();

			//画主菜单界面
			StartDraw();

			//改变界面标识
			GamePhase = 0;

		}
		else if (m.x >= 250 && m.x <= 365 && m.y >= 190 && m.y <= 225)//光明模式
		{

			//音乐
			mciSendString("stop menuBg", 0, 0, 0);

			//读取
			loadData();

			//游戏
			Game(1);

		}
		else if (m.x >= 250 && m.x <= 365 && m.y >= 270 && m.y <= 305)//无尽模式
		{

			//音乐
			mciSendString("stop menuBg", 0, 0, 0);

			//读取
			Game1();

		}

	}

}

//帮助操作
void HelpOp()
{

	if (m.uMsg == WM_MOUSEMOVE)
	{

		if (m.x >= 250 && m.x <= 325 && m.y >= 360 && m.y <= 395)	//选中返回
		{

			rectangle(250, 360, 325, 395);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 255, 255));
			outtextxy(260, 365, _T("返回"));

		}

		else	//取消选中
		{
			rectangle(250, 360, 325, 395);
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(260, 365, _T("返回"));
		}

	}

	if (m.uMsg == WM_LBUTTONDOWN)//左键点击
	{

		if (m.x >= 250 && m.x <= 325 && m.y >= 360 && m.y <= 395)
		{

			//清空窗口
			cleardevice();

			//画主菜单界面
			StartDraw();

			//改变界面标识
			GamePhase = 0;

		}

	}

}


//结束操作
void OverOp()
{

	if (m.uMsg == WM_MOUSEMOVE)
	{

		//重来一局
		if (m.x >= 240 && m.x <= 365 && m.y >= 220 && m.y <= 255)
		{

			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 255, 255));
			outtextxy(250, 225, _T("重来一局"));

			//取消选择	
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 275, _T("游戏结束"));

		}

		else if (m.x >= 240 && m.x <= 365 && m.y >= 270 && m.y <= 305)	//选中游戏结束
		{

			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 255, 255));
			outtextxy(250, 275, _T("游戏结束"));

			//取消选择
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 225, _T("重来一局"));
		}

		else	//都不选
		{
			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 275, _T("游戏结束"));

			settextstyle(22, 12, _T("微软雅黑"));
			settextcolor(RGB(255, 51, 68));
			outtextxy(250, 225, _T("重来一局"));

		}

	}

}