#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <conio.h> //kbhit()ʹ��

int score = 0; //�ܷ�
int rank = 0;  //�ȼ�

#define BLOCK_COUNT    7
#define BLOCK_WIDTH    5
#define BLOCK_HEIGHT	  5
#define UNIT_SIZE          20

#define START_X   130
#define START_Y	30

#define KEY_UP			72
#define	KEY_RIGHT	77
#define KEY_DOWN	80
#define KEY_LEFT		75
#define KEY_SPACE	32

int speed = 500;
int minX = 30;
int minY = 30;

typedef enum {
	BLOCK_UP,
	BLOCK_RIGHT,
	BLOCK_DOWN,
	BLOCK_LEFT
} block_dir_t;

typedef enum {
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
} move_dir_t;

int NextIndex = -1;  //��һ�����������
int BlockIndex = -1; //��ǰ���������

int color[BLOCK_COUNT] = {
	GREEN, CYAN,  MAGENTA, BROWN, YELLOW, BLUE, RED,
};

int visit[30][15]; //��������
int markColor[30][15]; //��ʾ��Ӧλ�õ���ɫ

int block[BLOCK_COUNT*4][BLOCK_HEIGHT][BLOCK_WIDTH] = {
	// |  �ͷ���
	{
		0, 0, 0, 0, 0,
	    0, 0, 1, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 0 },
	{ 
		0,0,0,0,0,
	    0,0,0,0,0,
	    0,1,1,1,0,
	    0,0,0,0,0,
	    0,0,0,0,0 },
	{  0,0,0,0,0,
	    0,0,1,0,0,
     	0,0,1,0,0,
	    0,0,1,0,0,
	    0,0,0,0,0 },

	{ 0,0,0,0,0,
	   0,0,0,0,0,
	   0,1,1,1,0,
	   0,0,0,0,0,
	   0,0,0,0,0 },

	  // L�η���
	{ 0,0,0,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,1,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,0,1,0,
	0,1,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,0,0,0,
	0,1,1,1,0,
	0,1,0,0,0,
	0,0,0,0,0 },

	// j �η���
	{ 0,0,0,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,1,1,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,0,0,0,
	0,1,1,1,0,
	0,0,0,1,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,1,1,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,0,0,0,
	0,1,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0 },


	// �� �η���
	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	// T �η���
	{ 0,0,0,0,0,
	0,1,1,1,0,
	0,0,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,0,1,0,
	0,0,1,1,0,
	0,0,0,1,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,1,0,0,
	0,1,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,0,0,0,
	0,1,1,0,0,
	0,1,0,0,0,
	0,0,0,0,0 },

	// Z �η���
	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,0,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,1,0,0,
	0,1,1,0,0,
	0,1,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,0,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,1,0,0,
	0,1,1,0,0,
	0,1,0,0,0,
	0,0,0,0,0 },

	// S�η���
	{ 0,0,0,0,0,
	0,0,1,1,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,0,0,0,
	0,1,1,0,0,
	0,0,1,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,1,1,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,0,0,0,
	0,1,1,0,0,
	0,0,1,0,0,
	0,0,0,0,0 }
};

// ��ӭ����
void welcome(void) {
	// ��ʼ������
	initgraph(550, 660);

	// ���ô��ڱ���
	HWND window = GetHWnd(); //��ȡ����
	SetWindowText(window, _T("����˹����    �п���� �޽�ΰ")); //���ô��ڱ���

	// �����ı���������ʽ
	setfont(40, 0, _T("΢���ź�"));
	setcolor(WHITE);
	outtextxy(205, 200, _T("����˹����"));

	setfont(22, 0, _T("����"));
	outtextxy(175, 300, _T("��̣� �Ӷ���˹���鿪ʼ!"));

	Sleep(3000); //˯�ߣ���ͣ��3000���룬3����
}

// ��ʼ����Ϸ����
void initGameScene(void) {
	char str[16];

	//�����Ļ
	cleardevice();

	rectangle(27, 27, 336, 635);
	rectangle(29, 29, 334, 633);
	rectangle(370, 50, 515, 195);

	setfont(24, 0, _T("����"));
	setcolor(LIGHTGRAY);
	outtextxy(405, 215, _T("��һ��"));

	setcolor(RED);
	outtextxy(405, 280, _T("����"));
	sprintf(str, "%d", score); 
	outtextxy(415, 310, str);

	outtextxy(405, 375, _T("�ȼ�"));
	sprintf(str, "%d", rank); 
	outtextxy(425, 405, str);

	// ����˵��  ��  �� �� ��
	setcolor(LIGHTBLUE);
	outtextxy(390, 475, "����˵��");
	outtextxy(390, 500, "������ת");
	outtextxy(390, 525, "��: �½�");
	outtextxy(390, 550, "��: ����");
	outtextxy(390, 575, "��: ����");
	outtextxy(390, 600, "�ո���ͣ");
}

void clearBlock(void) {
	setcolor(BLACK);
	setfont(23, 0, "����");

	for (int i=0; i<BLOCK_HEIGHT; i++) {
		for (int j=0; j<BLOCK_WIDTH; j++) {
			//"��"
			int x = 391 + j * UNIT_SIZE;
			int y = 71 + i * UNIT_SIZE;
			outtextxy(x, y, "��");
		}
	}
}




// ���Ʒ���
void drawBlock(int x, int y) {
	setcolor(color[NextIndex]);
	setfont(23, 0, "����");

	for (int i=0; i<BLOCK_HEIGHT; i++) {
		for (int j=0; j<BLOCK_WIDTH; j++) {
			//"��"
			if (block[NextIndex*4][i][j] == 1) {
				int x2 = x + j * UNIT_SIZE;
				int y2 = y + i * UNIT_SIZE;
				outtextxy(x2, y2, "��");
			}
		}
	}
}

// ���Ʒ���:  ��ָ��λ�û���ָ�������ָ������
void drawBlock(int x, int y, int blockIndex, block_dir_t dir) {
	setcolor(color[blockIndex]);
	setfont(23, 0, "����");
	int id = blockIndex * 4 + dir;

	for (int i=0; i<BLOCK_HEIGHT; i++) {
		for (int j=0; j<BLOCK_WIDTH; j++) {
			//"��"
			if (block[id][i][j] == 1) {
				int x2 = x + j * UNIT_SIZE;
				int y2 = y + i * UNIT_SIZE;
				outtextxy(x2, y2, "��");
			}
		}
	}
}



// ���ָ��λ��ָ������ķ���
// ����x: ��������Ͻǵ�x����
// ����y: ��������Ͻ�����Ϸ�����ڵ����꣬������Ϸ���򶥲��ľ���
void clearBlock(int x, int y, block_dir_t dir) {
	setcolor(BLACK);
	int id = BlockIndex * 4 + dir;
	y += START_Y;

	for (int i=0; i<5; i++) {
		for (int j=0; j<5; j++) {
			if (block[id][i][j] == 1) {
				// �����÷���ĵ�i�еĵ�j��
				outtextxy(x+20*j,  y+i*20, "��");
			}
		}
	}
}

void nextblock(void) {
	clearBlock(); // ������Ͻ�����

	// ���ѡ��һ�ַ���
	srand(time(NULL)); //ʹ��ʱ�亯���ķ���ֵ������Ϊ�������
    NextIndex =	rand() % BLOCK_COUNT;

	drawBlock(391, 71);
}

// �����ָ��λ�ÿ�����ָ�������ƶ����ͷ���1�� ����ͷ���0
int moveable(int x0, int y0, move_dir_t moveDir, block_dir_t blockDir) {
	// ���㵱ǰ��������Ͻ���30x15����Ϸ���е�λ�ã��ڶ����У��ڶ����У�
	int x = (y0 - minY) / UNIT_SIZE;
	int y = (x0 - minX) / UNIT_SIZE;
	int id = BlockIndex * 4 + blockDir;
	int ret = 1;

	if (moveDir == MOVE_DOWN) {
		for (int i=0; i<5; i++) {
			for (int j=0; j<5; j++) {
				if (block[id][i][j] == 1 &&
					(x + i + 1 >= 30  ||  visit[x+i+1][y+j] == 1)) {
						 ret = 0;
				}
			}
		}
	} else if (moveDir == MOVE_LEFT) {
		for (int i=0; i<5; i++) {
			for (int j=0; j<5; j++) {
				if (block[id][i][j] == 1 &&
					(y + j == 0 ||  visit[x+i][y+j-1]==1)) {
						ret = 0;
				}
			}
		}

	} else if (moveDir == MOVE_RIGHT) {
		for (int i=0; i<5; i++) {
			for (int j=0; j<5; j++) {
				if (block[id][i][j] == 1 && 
					(y+j+1>=15 || visit[x+i][y+j+1]==1)) {
						 ret = 0;
				}
			}
		}
	}

	return ret;
}

// �����Ϸ�Ƿ����
void failCheck() {
	if (!moveable(START_X, START_Y, MOVE_DOWN, BLOCK_UP)) {
		setcolor(WHITE);
		setfont(45, 0, "����");
		outtextxy(75, 300, "GAME OVER!");
		Sleep(1000);
		system("pause");
		closegraph();
		exit(0);
	}
}



// �жϵ�ǰ�����Ƿ����ת��ָ������
// ע�⣬ ��ʱ��û��ת���÷��򣡣���
int rotatable(int x, int y, block_dir_t dir) {
	int id = BlockIndex * 4 + dir;
	int xIndex = (y - minY) / 20;
	int yIndex = (x - minX) / 20;


	if (!moveable(x, y, MOVE_DOWN, dir)) {
		return 0;
	}

	for (int i=0; i<5; i++) {
		for (int j=0; j<5; j++) {
			 if (block[id][i][j] == 1 &&
				 (yIndex+j<0 || yIndex+j>=15 || visit[xIndex+i][yIndex+j]==1)) {
					 return 0;
			 }
		}
	}

	return 1;
}

void wait(int interval) { 
	int count  = interval / 10;
	for (int i=0; i<count; i++) {
		Sleep(10);
		if (kbhit()) {
			return;
		}
	}
}

void mark(int x, int y, int blockIndex, block_dir_t dir) {
	int id = blockIndex * 4 + dir;
	int x2 = (y - minY) / 20;
	int y2 = (x - minX) / 20;

	for (int i=0; i<5; i++) {
		for (int j=0; j<5; j++) {
			if (block[id][i][j] == 1) {
				visit[x2+i][y2+j] = 1;
				markColor[x2+i][y2+j] = color[blockIndex];
			}
		}
	}
}

void move(void){
	int x = START_X;
	int y = START_Y;
	int k = 0;
	block_dir_t  blockDir = BLOCK_UP;
	int curSpeed = speed;

	// �����Ϸ�Ƿ����
	failCheck();

	// �������½���
	while (1) {
		if (kbhit()) {
			int key = getch();
			if (key == KEY_SPACE) {
				getch();
			}
		}

		// �����ǰ����
		clearBlock(x, k, blockDir);

		if (kbhit()) {
			int key = getch();

			if(key == KEY_UP) {
				block_dir_t nextDir = (block_dir_t)((blockDir + 1) % 4);
				if (rotatable(x, y+k, nextDir)) {
					blockDir = nextDir;
				}
			} else if (key == KEY_DOWN) {
				curSpeed = 50;
			} else if (key == KEY_LEFT) {
				if (moveable(x, y+k+20, MOVE_LEFT, blockDir)) {
					x -= 20;
				}
			} else if (key ==KEY_RIGHT) {
				if (moveable(x, y+k+20, MOVE_RIGHT, blockDir)) {
					x += 20;  //x = x + 20;
				}
			}
		}

		k += 20;

		// ���Ƶ�ǰ����
		drawBlock(x, y+k, BlockIndex, blockDir);

		wait(curSpeed);
		
		//k += 20;

		// ����ġ��̻�������
		if (!moveable(x, y+k, MOVE_DOWN, blockDir)) {
			 mark(x, y+k, BlockIndex, blockDir);
			 break;
		}
	}
}

void newblock() {
	// ȷ������ʹ�õķ�������
	BlockIndex = NextIndex;

	// ���ƸմӶ����½��ķ���
	drawBlock(START_X, START_Y);

	// ���³��ֵķ�����ͣһ�ᣬ���û�ʶ��
	Sleep(100); //0.1��

	// �����Ͻ����򣬻�����һ������
	nextblock();

	// ���齵��
	move();
}

//������x�У�����������ж�����
void down(int x){
	for (int i=x; i>0; i--) {
		// ������i�У���j�еķ�������
		for (int j=0; j<15; j++) {
			if (visit[i-1][j]) {
				visit[i][j] = 1;
				markColor[i][j] = markColor[i-1][j];
				setcolor(markColor[i][j]);
				outtextxy(20*j + minX, 20*i+minY, "��");
			} else {
				visit[i][j] = 0;
				setcolor(BLACK);
				outtextxy(20*j + minX, 20*i+minY, "��");
			}
		}
	}

	// �����ϵ���һ�У������б�Ϊ0����һ�У�
	setcolor(BLACK);
	for (int j=0; j<15; j++) {
		visit[0][j] = 0;
		outtextxy(20*j + minX, minY, "��");
	}
}

// ���·���������lines��ʾ����������
void addScore(int lines) {
	char str[32];

	setcolor(RED);
	score += lines * 10;
	sprintf(str, "%d", score);
	outtextxy(415, 310, str);
}

void updateGrade() {
	// ���µȼ�����ʾ
	// ���裺50��һ��
	rank = score / 50;
	char str[16];
	sprintf(str, "%d", rank);
	outtextxy(425, 405, str);

	// �����ٶ�, �ȼ�Խ�ߣ��ٶ�Խ�죬speedԽС��
	// ������500�� �����100
	speed = 500 - rank*100;
	if (speed <= 100) {
		speed = 100;
	}
}

void check(void) {
	int i, j;
	int clearLines = 0;

	for (i=29; i>=0; i--) {
		// ����i����û����
		for (j=0; j<15 && visit[i][j]; j++) ;

		//ִ�е��˴�ʱ�������������
		// 1. ��i��û����������ʾ�п�λ ��ʱ j<15
		// 2. ��i�������ˣ���ʱ j>=15
		if (j >= 15) {
			// ��ʱ����i���Ѿ����ˣ�����Ҫ������i��
			down(i);  //������i�У�����������ж�����
			i++;  // ��Ϊ������ѭ������ i--, ����������i++, ʹ���´�ѭ��ʱ���ٰ���һ�м��һ��
			clearLines++;
		}
	}

	// ���·���
	addScore(clearLines);

	// ���µȼ�(���µȼ���ʾ�������ٶȣ�
	updateGrade();
}

int main(void) {
	welcome();
	initGameScene();

	// �����·���
	nextblock();
	Sleep(500);

	// ��ʼ����������
	memset(visit, 0, sizeof(visit));

	while (1) {
		newblock();

		// �������У������·������ٶ�
		check();
	}

	system("pause");
	closegraph();
	return 0;
}
