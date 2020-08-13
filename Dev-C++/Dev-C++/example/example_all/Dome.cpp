/* Dome.hpp  (C) CopyRight SIyuan Deng 2020 -forever,
SeaUI is a free software that can be edited as needed.
The program is based on Win32, so it can only run in a window system.
There must be some errors in my program that I don’t know about,
Therefore, please use it with caution. */

#include <iostream>
#include "SeaUI.hpp"


HWND hwnd;
LRESULT CALLBACK MyWndProc( HWND, UINT, WPARAM, LPARAM ) ;
//LRESULT CALLBACK *******( HWND, UINT, WPARAM, LPARAM ) ;

_SEAUIWINDOW*Textbox_example=Initialization(SUS_TEXTBOX);//初始化为文本框 
_SEAUIWINDOW*MaskWindow_example=Initialization(SUS_MASK);//初始化为遮罩窗口 
_SEAUIWINDOW*LayeredWindow_example=Initialization(SUS_LAYER);//初始化为分层窗口 
_SEAUIWINDOW*NormalWindow_example=Initialization(SUS_NORMAL);//初始化为标准子窗口 
_SEAUIWINDOW*ProgressBar_example=Initialization(SUS_PROGRESSBAR);//初始化为标准子窗口 
         
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow )
{
	RegWindow("测试窗口",MyWndProc,hwnd, hInstance); 
 	MSG          msg ;
    while( GetMessage(&msg, NULL, 0, 0) )
    {
        TranslateMessage( &msg ) ;
        DispatchMessage( &msg ) ;
    } 
    return msg.wParam ;
}


static void OnButtonUp(){
	printf("点击了mask Window(遮罩窗口)\n");
	MessageBox(NULL,"点击了mask Window(遮罩窗口)","Example",MB_OK); 
	return;
}


static void OnButtonUp1(){
	printf("点击了LAYERED Window(分层窗口)\n");
	MessageBox(NULL,"点击了LAYERED Window(分层窗口)","Example",MB_OK); 
	return;
}

static void OnButtonUp2(){
	printf("点击了Normal Window(标准窗口)\n");
	MessageBox(NULL,"点击了Normal Window(标准窗口)","Example",MB_OK); 
	return;
}

static void OnButtonUp3(){
	printf("点击了Progress Bar\n");
	ProgressBar_example->SetPercentage(ProgressBar_example->GetClickPosition());//根据点击位置设置百分比 
	//float num=ProgressBar_example->GetPercentage();//获取当前百分比 
	return;
}



LRESULT CALLBACK MyWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) 
{
	PAINTSTRUCT ps;
	
	
    switch(message)
    {
    case WM_CREATE:{        //在接收到 WM_CREATE 消息时创建一个子窗口
    
    
    	AllocConsole();//显示控制台窗口 
 		freopen("CONOUT$", "w", stdout);//显示控制台窗口 
    	
    	/***************TextBox编辑框******************/
    	//实现原理就是把编辑框再封装，所以适用于win32编辑框的api此处都适用 
		Textbox_example->Create(hWnd,"textbox",0,40,200,25,(HMENU)0);//创建TextBox(编辑框) 
		Textbox_example->Roundrect(25,1);//设置圆角 
		Textbox_example->SetTextSize(10,18);//设置显示字体大小 
		
		//strcpy(str,Textbox_example->GetText());获得输入内容 
		//Textbox_example->SetTextBoxLong( 此处填写Windows编辑框的样式标识符，比如 ES_NUMBER，只能输入数字的样式 )
		//Textbox_example->Password(true);将为密码输入框，用*代替文字 
		//此外还可以通过标准API :SetWindowText来设置文本框内容 
		
		 /*关于Create函数，形参1 HWND为父窗口的句柄（标识符），形参2位自定义名字，形参3和4为*分别为相对于父窗口
		 的X和Y坐标，如果形参1HWND的值为零，那么形参3,4将相对于屏幕左上角坐标创建，
		 形参5,6为创建的窗口的长宽，形参7为HMENU，如果没有特殊要求，为零/ 
		 
		 
		/***************Mask Window遮罩窗口******************/
		MaskWindow_example->Create(hWnd,"MaskWindow",0,90,200,200,(HMENU)0);//创建遮罩窗口 
		MaskWindow_example->SetAlphaValue(0);//设置透明度，此处的透明值是相对于前景图片的 
		MaskWindow_example->Roundrect(25,1);//设置圆角 
		MaskWindow_example->SetBackgroundImage("res/Pavarotti.bmp",200,200,0,0);//设置背景图片，前两个参数是长宽，后两个为xy坐标 ,注意格式为BMP 
		MaskWindow_example->SetForegroundBitmap("res/black.bmp",200,200);//设置前景图片,注意格式为BMP 
		//接下来就是点击和悬停动画的设置 
		MaskWindow_example->ButtonAnimationInitialize(0,70);//设置透明变化程度，参数1为初始透明度，参数2为增长透明度，如果没有初始化，接下来的动画将是无效的 
		MaskWindow_example->ButtonHitAnimation(SEA_UI_ON);//开启点击动画 
		MaskWindow_example->MouseHoverAnimation(SEA_UI_ON);//开启悬停动画 
		MaskWindow_example->SetCallback(WM_LBUTTONUP,OnButtonUp);
		/*设置回调，WM_LBUTTONUP为鼠标在窗口内抬起的标识符，OnButtonUp为回调函数名，注意回调函数的形参为空且为静态的static，
		格式为 static void XXXX(){  }
		还可以设置回调的是  WM_PAINT,WM_LBUTTONDOWN,WM_MOUSEHOVER,WM_MOUSELEAVE. 
		*/ 
		
		/***************Layer Window分层窗口******************/
		//注意，分层窗口创建为所有者窗口 
		LayeredWindow_example->Create(hWnd,"Layer Window",0,310,100,100,(HMENU)0);
		LayeredWindow_example->SetImage("res/karajan.bmp",0,0);//注意格式为BMP 
		LayeredWindow_example->SetAlphaValue(200);//设置透明度 
		LayeredWindow_example->SetFrameInfo(2,0,1,0);
		/*设置边框，参数一为边框的粗细（像素），参数二为边框的颜色COLOREF，用RGB(,,)来表示，
		参数三为bool值，为1时候就自动识别适宜颜色来绘制边框，设置此值将忽略参数二，参数四为是否为3D样式的边框 
		*/ 
	     //接下来就是点击和悬停动画的设置 
		LayeredWindow_example->ButtonAnimationInitialize(200,100);//同上 
		LayeredWindow_example->ButtonHitAnimation(SEA_UI_ON);
		LayeredWindow_example->MouseHoverAnimation(SEA_UI_ON);
		LayeredWindow_example->SetCallback(WM_LBUTTONUP,OnButtonUp1);//设置回调 
		
		
		/***************Layer Window分层窗口******************/
		NormalWindow_example->Create(hWnd,"Normal Window",0,430,100,100,(HMENU)0);
		NormalWindow_example->SetImage("res/black.bmp",0,0);//设置图片，注意为bmp 
		NormalWindow_example->Transform(RGB(0,0,0),RGB(100,100,100));//这是一个抠图小动画，参数一为抠图选区的颜色，参数2为要抠成的颜色，具体显示在下面一行 
		NormalWindow_example->ButtonHitAnimation(SEA_UI_ON);//开启动画 
		NormalWindow_example->SetCallback(WM_LBUTTONUP,OnButtonUp2);//设置回调 
		//此处省略了一些功能，详情请见上面几个控件 
		
		/***************Progress Bar进度条******************/
		ProgressBar_example->Create(hWnd,"Progress Bar",0,0,150,10,(HMENU)0);
		ProgressBar_example->SetBackgroundImage("res/black.bmp",0,0,0,0);//设置背景图片，前两个参数是长宽，后两个为xy坐标 ,注意格式为BMP 
		ProgressBar_example->SetForegroundBitmap("res/progress.bmp",0,0);//设置前景图片,注意格式为BMP 
		ProgressBar_example->SetPercentage(0.7f);//设置百分比
		ProgressBar_example->SetProgressBarDirection(SUD_RIGHT);//设置方向，如果未设置,则默认进度条方向朝右 
		ProgressBar_example->SetCallback(WM_LBUTTONDOWN,OnButtonUp3);//设置回调 
		//此处省略了一些功能，详情请见上面几个控件 
		
		return 0 ;}
	case WM_COMMAND:{
	
			break;}
					
			case WM_PAINT:{
				HDC hdc = BeginPaint(hWnd, &ps);
				DrawPicture(hdc,"res/a.bmp",500,500);
				// TODO: 在此添加主窗口任意绘图代码...
				EndPaint(hWnd, &ps);
				break;
						  }
		
    case WM_DESTROY:
        PostQuitMessage( 0 ) ;
        RemoveWindow(Textbox_example);
        RemoveWindow(MaskWindow_example);
        RemoveWindow(LayeredWindow_example);
        RemoveWindow(NormalWindow_example);
        RemoveWindow(ProgressBar_example);
        return 0 ;
    }
    return DefWindowProc( hWnd, message, wParam, lParam ) ;
}

