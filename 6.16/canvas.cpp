#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>	//随机数种子用

#include "mmsystem.h"//导入声音头文件
#pragma comment(lib,"winmm.lib")//导入声音头文件库


struct BULLET                                                                //子弹结构
{
	int x,y;
	bool exist;
};
struct MONSTER                                                               //僵尸结构 
{
	int x,y;
	int blood;
	bool exist;
};
struct STONE
{
	int x,y;
	bool exist;
};
HINSTANCE hInst;															//各种变量初始化
HBITMAP dra[6],bg[2],pl[5],bullet,k[2],over,win,life[5],stone;
HDC		hdc,mdc,bufdc,buffdc;
HWND	hWnd;
DWORD	tPre,tNow;
int	    x,y;
int     num,bcount=300;
int     count=0;
BULLET  b[300];
STONE	s;
MONSTER m[6];
int     q[5]={-50,20,110,200,300},qi=rand()%5,wi=rand()%5,ei=rand()%5,ri=rand()%5,ti=rand()%5;

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void				MyPaint(HDC hdc);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;

	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

    while( msg.message!=WM_QUIT )
    {
        if( PeekMessage( &msg, NULL, 0,0 ,PM_REMOVE) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		else
		{
			tNow = GetTickCount();

			if(tNow-tPre >= 100)
				MyPaint(hdc);
		}
    }

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "canvas";
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	char filename[20] = "";
	HBITMAP bmp;
	hInst = hInstance;

	hWnd = CreateWindow("canvas", "菜园保卫战" , WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	MoveWindow(hWnd,10,10,640,480,true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hdc = GetDC(hWnd);
	mdc = CreateCompatibleDC(hdc);
	bufdc = CreateCompatibleDC(hdc);
	buffdc = CreateCompatibleDC(hdc);
	bmp = CreateCompatibleBitmap(hdc,640,480);

	SelectObject(mdc,bmp);
	
	k[0]=(HBITMAP)LoadImage(NULL,"k0.bmp",IMAGE_BITMAP,700,150,LR_LOADFROMFILE);
	k[1]=(HBITMAP)LoadImage(NULL,"k1.bmp",IMAGE_BITMAP,700,150,LR_LOADFROMFILE);
	life[0]=(HBITMAP)LoadImage(NULL,"life1.bmp",IMAGE_BITMAP,310,60,LR_LOADFROMFILE);
	life[1]=(HBITMAP)LoadImage(NULL,"life2.bmp",IMAGE_BITMAP,310,60,LR_LOADFROMFILE);
	life[2]=(HBITMAP)LoadImage(NULL,"life3.bmp",IMAGE_BITMAP,310,60,LR_LOADFROMFILE);
	life[3]=(HBITMAP)LoadImage(NULL,"life4.bmp",IMAGE_BITMAP,310,60,LR_LOADFROMFILE);
	life[4]=(HBITMAP)LoadImage(NULL,"life5.bmp",IMAGE_BITMAP,310,60,LR_LOADFROMFILE);
	dra[0] = (HBITMAP)LoadImage(NULL,"dra1.bmp",IMAGE_BITMAP,800,300,LR_LOADFROMFILE);
	dra[1] = (HBITMAP)LoadImage(NULL,"dra2.bmp",IMAGE_BITMAP,800,300,LR_LOADFROMFILE);
	dra[2] = (HBITMAP)LoadImage(NULL,"dra3.bmp",IMAGE_BITMAP,800,300,LR_LOADFROMFILE);
	dra[3] = (HBITMAP)LoadImage(NULL,"dra4.bmp",IMAGE_BITMAP,800,300,LR_LOADFROMFILE);
	dra[4] = (HBITMAP)LoadImage(NULL,"dra5.bmp",IMAGE_BITMAP,800,300,LR_LOADFROMFILE);
	dra[5] = (HBITMAP)LoadImage(NULL,"dra6.bmp",IMAGE_BITMAP,1200,300,LR_LOADFROMFILE);
	bg[0] = (HBITMAP)LoadImage(NULL,"bg.bmp",IMAGE_BITMAP,640,480,LR_LOADFROMFILE);
	bg[1] = (HBITMAP)LoadImage(NULL,"bg1.bmp",IMAGE_BITMAP,640,480,LR_LOADFROMFILE);
	pl[0] = (HBITMAP)LoadImage(NULL,"pl1.bmp",IMAGE_BITMAP,150,100,LR_LOADFROMFILE);
	pl[1] = (HBITMAP)LoadImage(NULL,"pl2.bmp",IMAGE_BITMAP,150,100,LR_LOADFROMFILE);
	pl[2] = (HBITMAP)LoadImage(NULL,"pl3.bmp",IMAGE_BITMAP,150,100,LR_LOADFROMFILE);
	pl[3] = (HBITMAP)LoadImage(NULL,"pl4.bmp",IMAGE_BITMAP,150,100,LR_LOADFROMFILE);
	pl[4] = (HBITMAP)LoadImage(NULL,"pl5.bmp",IMAGE_BITMAP,150,100,LR_LOADFROMFILE);
	bullet = (HBITMAP)LoadImage(NULL,"bullet.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	stone=(HBITMAP)LoadImage(NULL,"stone.bmp",IMAGE_BITMAP,150,100,LR_LOADFROMFILE);
	over = (HBITMAP)LoadImage(NULL,"over.bmp",IMAGE_BITMAP,640,159,LR_LOADFROMFILE);
	win = (HBITMAP)LoadImage(NULL,"win.bmp",IMAGE_BITMAP,640,160,LR_LOADFROMFILE);

	m[0].x=m[1].x=m[2].x=m[3].x=m[4].x=m[5].x=640;									//怪兽体征值初始化
	m[5].y=110;
	m[3].blood=2;
	m[4].blood=2;
	m[5].blood=20;
	
	m[0].exist=m[1].exist=m[2].exist=m[3].exist=m[4].exist=true;
	m[5].exist=false;
	s.exist=false;
	MyPaint(hdc);

	return TRUE;
}



void MyPaint(HDC hdc)
{
	int i = 0;
	if(num == 8)
	num = 0;
	char str[50]="";	
	int score;
	PlaySound("foot.wav", NULL, SND_FILENAME | SND_ASYNC);
    
	srand((unsigned int)time(0));													//随机数种子
	m[4].y=q[qi];
	m[3].y=q[wi];
	m[2].y=q[ei];
	m[1].y=q[ri];
	m[0].y=q[ti];


	SelectObject(bufdc,bg[0]);														//贴各种图
	BitBlt(mdc,0,0,640,480,bufdc,0,0,SRCCOPY);

    SelectObject(bufdc,pl[0]);
	BitBlt(mdc,30,0,75,100,bufdc,75,0,SRCAND);
	BitBlt(mdc,30,0,75,100,bufdc,0,0,SRCPAINT);

	SelectObject(bufdc,pl[1]);
	BitBlt(mdc,30,100,75,100,bufdc,75,0,SRCAND);
	BitBlt(mdc,30,100,75,100,bufdc,0,0,SRCPAINT);

	SelectObject(bufdc,pl[2]);
	BitBlt(mdc,30,200,75,100,bufdc,75,0,SRCAND);
	BitBlt(mdc,30,200,75,100,bufdc,0,0,SRCPAINT);

	SelectObject(bufdc,pl[3]);
	BitBlt(mdc,30,280,75,100,bufdc,75,0,SRCAND);
	BitBlt(mdc,30,280,75,100,bufdc,0,0,SRCPAINT);
	
	SelectObject(bufdc,pl[4]);
	BitBlt(mdc,30,360,75,100,bufdc,75,0,SRCAND);
	BitBlt(mdc,30,360,75,100,bufdc,0,0,SRCPAINT);


  
	SelectObject(bufdc,dra[3]);
	BitBlt(mdc,m[3].x,m[3].y,100,150,bufdc,num*100,150,SRCAND);
	BitBlt(mdc,m[3].x,m[3].y,100,150,bufdc,num*100,0,SRCPAINT);

	SelectObject(bufdc,dra[2]);
	BitBlt(mdc,m[2].x,m[2].y,100,150,bufdc,num*100,150,SRCAND);
	BitBlt(mdc,m[2].x,m[2].y,100,150,bufdc,num*100,0,SRCPAINT);

	SelectObject(bufdc,dra[1]);
	BitBlt(mdc,m[1].x,m[1].y,100,150,bufdc,num*100,150,SRCAND);
	BitBlt(mdc,m[1].x,m[1].y,100,150,bufdc,num*100,0,SRCPAINT);
	
	SelectObject(bufdc,dra[0]);
	BitBlt(mdc,m[0].x,m[0].y,100,150,bufdc,num*100,150,SRCAND);
	BitBlt(mdc,m[0].x,m[0].y,100,150,bufdc,num*100,0,SRCPAINT);

	SelectObject(bufdc,dra[4]);
	BitBlt(mdc,m[4].x,m[4].y,98,150,bufdc,num*98,150,SRCAND);
	BitBlt(mdc,m[4].x,m[4].y,98,150,bufdc,num*98,0,SRCPAINT);

	SelectObject(bufdc,dra[5]);
	BitBlt(mdc,m[5].x,m[5].y,150,150,bufdc,num*150,150,SRCAND);
    BitBlt(mdc,m[5].x,m[5].y,150,150,bufdc,num*150,0,SRCPAINT);

	SelectObject(buffdc,bg[1]);
	BitBlt(mdc,0,0,640,480,buffdc,0,0,SRCCOPY);

	SelectObject(buffdc,k[0]);
	BitBlt(mdc,150,30,350,150,buffdc,350,0,SRCAND);
	BitBlt(mdc,150,30,350,150,buffdc,0,0,SRCPAINT);

	if(m[0].x>30)																			//移动速度设定
		m[0].x-=12;					  
	else{
		m[0].x=30;
		SelectObject(bufdc,over);
		BitBlt(mdc,0,90,640,159,bufdc,0,0,SRCCOPY);
		DeleteDC(bufdc);
		for(i=0;i<100;i++)
		{
			b[i].exist=false;
		}
		}

	if(m[1].x>30)
		m[1].x-=6.6;					  
	else{
		m[1].x=30;
		SelectObject(bufdc,over);
		BitBlt(mdc,0,90,640,159,bufdc,0,0,SRCCOPY); 
			DeleteDC(bufdc);

		for(i=0;i<100;i++)
		{
			b[i].exist=false;
		}
	}
				  
	if(m[2].x>30)
		m[2].x-=5.5;			  
	else{
		m[2].x=30;
		SelectObject(bufdc,over);
		BitBlt(mdc,0,90,640,159,bufdc,0,0,SRCCOPY);
			DeleteDC(bufdc);

		for(i=0;i<100;i++)
		{
			b[i].exist=false;
		}
	}

				  
	
	if(m[3].x>30)
		m[3].x-=6;		  
	else{
		m[3].x=30;
		SelectObject(bufdc,over);
		BitBlt(mdc,0,90,640,159,bufdc,0,0,SRCCOPY);
			DeleteDC(bufdc);
		for(i=0;i<100;i++)
		{
			b[i].exist=false;
		}
	}

					  

	if(m[4].x>30)
		m[4].x-=8;			  
	else{
		m[4].x=30;
		SelectObject(bufdc,over);
		BitBlt(mdc,0,90,640,159,bufdc,0,0,SRCCOPY);
			DeleteDC(bufdc);
        for(i=0;i<100;i++)
		{
			b[i].exist=false;
		}
	}

	if(m[5].x>40 &&m[5].exist==true)											//boss
		m[5].x-=4;	
	
	if(m[5].x==40 && m[5].exist==true)
	{
		SelectObject(bufdc,over);
		BitBlt(mdc,0,90,640,159,bufdc,0,0,SRCCOPY);
		DeleteDC(bufdc);
        for(i=0;i<100;i++)
		{
			b[i].exist=false;
		}
	}
	

	SelectObject(bufdc,stone);																	//加石头
	if(s.exist==true)
	{			
				BitBlt(mdc,s.x,s.y,77,100,bufdc,77,0,SRCAND);
				BitBlt(mdc,s.x,s.y,77,100,bufdc,0,0,SRCPAINT);	
					if(s.x>m[4].x -5 &&  s.y==m[4].y+40)            
					{
						
						m[4].exist=false;
						m[4].x=640;
						s.exist = false;
						qi=rand()%5;
						score+=20;
					
				}
					else	if(s.x>m[3].x -5 &&  s.y==m[3].y+40)            
					{
						
						m[3].exist=false;
						m[3].x=640;
						s.exist = false;
						qi=rand()%5;
						score+=20;

					
				}
					else 	if(s.x>m[2].x -5 &&  s.y==m[2].y+40)            
					{
						
						m[2].exist=false;
						m[2].x=640;
						s.exist = false;
						qi=rand()%5;
						score+=20;
					
				}
					else 	if(s.x>m[1].x -5 &&  s.y==m[1].y+40)            
					{
						
						m[1].exist=false;
						m[1].x=640;
						s.exist = false;
						qi=rand()%5;
						score+=20;
					
				}
					else 	if(s.x>m[0].x -5 &&  s.y==m[0].y+40)            
					{
						
						m[0].exist=false;
						m[0].x=640;
						s.exist = false;
						qi=rand()%5;
						score+=20;
					
				}
					
					else if(s.x>m[5].x -5 &&  s.y==m[5].y+40 && m[5].exist==true)      //boss关底
					{
						
						m[5].blood-=3;
						s.exist = false;
						score+=20;
					}
			}
			
				
	


	SelectObject(bufdc,bullet);																	//子弹消灭怪兽设定及贴图
	if(bcount!=0)
		for(i=0;i<300;i++)
			if(b[i].exist)
			{
				BitBlt(mdc,b[i].x,b[i].y,30,15,bufdc,0,15,SRCAND);
				BitBlt(mdc,b[i].x,b[i].y,30,15,bufdc,0,0,SRCPAINT);

				b[i].x += 10;
				if(b[i].x > 640)
				{
					bcount--;
					b[i].exist = false;
				}
			
					if(b[i].x>m[4].x +5 &&  (b[i].y==m[4].y+90 || b[i].y==m[4].y+110))            //+5 decrease err
					{
						
						m[4].exist=false;
						m[4].x=640;
						b[i].exist = false;
						count++;
						qi=rand()%5;
					
				}
					else if(b[i].x>m[3].x +5 &&  (b[i].y==m[3].y+90 || b[i].y==m[3].y+110))
					{
						m[3].blood--;
						if(m[3].blood==0)
						{
						m[3].exist=false;
						m[3].x=640;
						m[3].blood=2;
						wi=rand()%5;
						}
						b[i].exist = false;
						count++;
						
						
				}
					else if(b[i].x>m[2].x +5 &&   (b[i].y==m[2].y+90 || b[i].y==m[2].y+110))
					{
						
						m[2].exist=false;
						m[2].x=640;
						b[i].exist = false;
						count++;
						ei=rand()%5;
					
				}
					else if(b[i].x>m[1].x +5 &&   (b[i].y==m[1].y+90 || b[i].y==m[1].y+110))
					{
						
						m[1].exist=false;
						m[1].x=640;
						b[i].exist = false;
						count++;
						ri=rand()%5;
						
				}
					else if(b[i].x>m[0].x +5 &&   (b[i].y==m[0].y+90 || b[i].y==m[0].y+110))
					{
						
						m[0].exist=false;
						m[0].x=640;
						b[i].exist = false;
						count++;
						ti=rand()%5;
					}
					
					else if(b[i].x>m[5].x +5 &&  b[i].y==m[5].y+110 && m[5].exist==true)      //boss关底
					{
						
						m[5].blood--;
						b[i].exist = false;				
					}
			}
			
			if (count>=50 && m[5].exist==false)													//boss existence
			{
				m[5].exist=true;
			}
			
			if(m[5].blood>20 &&m[5].blood<=25)													//血条控制
			{
				SelectObject(bufdc,life[0]);
	            BitBlt(mdc,m[5].x,m[5].y-50,155,60,bufdc,155,0,SRCAND);
				BitBlt(mdc,m[5].x,m[5].y-50,155,60,bufdc,0,0,SRCPAINT);
			}

			if(m[5].blood>15 &&m[5].blood<=20)
			{
				SelectObject(bufdc,life[1]);
	            BitBlt(mdc,m[5].x,m[5].y-50,155,60,bufdc,155,0,SRCAND);
				BitBlt(mdc,m[5].x,m[5].y-50,155,60,bufdc,0,0,SRCPAINT);
			}

			if(m[5].blood>8 &&m[5].blood<=15)
			{
				SelectObject(bufdc,life[2]);
	            BitBlt(mdc,m[5].x,m[5].y-50,155,60,bufdc,155,0,SRCAND);
				BitBlt(mdc,m[5].x,m[5].y-50,155,60,bufdc,0,0,SRCPAINT);
			}

			if(m[5].blood>5 &&m[5].blood<=8)
			{
				SelectObject(bufdc,life[3]);
	            BitBlt(mdc,m[5].x,m[5].y-50,155,60,bufdc,155,0,SRCAND);
				BitBlt(mdc,m[5].x,m[5].y-50,155,60,bufdc,0,0,SRCPAINT);
			}

			if(m[5].blood>0 &&m[5].blood<=5)
			{
				SelectObject(bufdc,life[4]);
	            BitBlt(mdc,m[5].x,m[5].y-50,155,60,bufdc,155,0,SRCAND);
				BitBlt(mdc,m[5].x,m[5].y-50,155,60,bufdc,0,0,SRCPAINT);
			}



			if(m[5].blood<=0)
					{
						    m[5].exist=false;
							SelectObject(bufdc,win);
	                     	BitBlt(mdc,0,90,640,160,bufdc,0,0,SRCCOPY); 
		                	DeleteDC(bufdc);
		            for(i=0;i<100;i++)
					{
						b[i].exist=false;
					}

				}

		
			score=count*100-(bcount-300)*20;															//屏显信息
			sprintf(str,"你已消灭怪物数: %d 得分：%d",count,score);										//character instruction
			TextOut (mdc,0,0,str,strlen(str));
			sprintf(str,"还有%d个怪物即将抵达战场",50-count);
			TextOut (mdc,0,20,str,strlen(str));
			
    
	tPre = GetTickCount();    
	num++;
	BitBlt(hdc,0,0,640,480,mdc,0,0,SRCCOPY);

}
	

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)               //callback
{
	int i;
	switch (message)
	{


	case WM_LBUTTONDOWN:																	//前景图设定
		if(x>150 && x<500 && y>30 && y<180)
		   DeleteDC(buffdc);
			
		
		
			for(i=0;i<300;i++)																//子弹消失
			{
				if(!b[i].exist && x<100 && y<100)
				{
					b[i].x = 90;	
					b[i].y = 40; 
					b[i].exist = true;
					bcount++;
					break;
				}
				else if(!b[i].exist && x<100 && y<192 && y>96)
				{
					b[i].x = 90;	
					b[i].y = 130; 
					b[i].exist = true;
					bcount++;		
					break;
				}
				else if(!b[i].exist && x<100 && y<288 && y>192)
				{
					b[i].x = 90;	
					b[i].y = 220; 
					b[i].exist = true;
					bcount++;		
					break;
				}
				else if(!b[i].exist && x<100 && y<380 && y>288)
				{
					b[i].x = 90;	
					b[i].y = 310; 
					b[i].exist = true;
					bcount++;		
					break;
				}
					else if(!b[i].exist && x<100 && y<480 && y>380)
				{
					b[i].x = 90;	
					b[i].y = 390; 
					b[i].exist = true;
					bcount++;
					break;
				}
			}
			break;
	case WM_RBUTTONDOWN:
		if(s.exist==false)
		{
			s.exist=true;
		s.x=x;
		if(y<100)
			s.y=-10;
		if( y<192 && y>96)
			s.y=60;
		if( y<288 && y>192)
			s.y=150;
		if(y<380 && y>288)
			s.y=240;
		if(y<480 && y>380)
			s.y=340;
		
		}
		break;

	case WM_DESTROY:
			DeleteDC(mdc);
			DeleteDC(bufdc);
 	        DeleteObject(pl);
			DeleteObject(bg);
			DeleteObject(dra);
			DeleteObject(bullet);
			ReleaseDC(hWnd,hdc);
			PostQuitMessage(0);
			break;
		case WM_MOUSEMOVE:
			x=LOWORD(lParam);
			y=HIWORD(lParam);
			break;
		default:						
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}
