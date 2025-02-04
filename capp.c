#include "capp.h"
#include "base.h"
#include "graphics.h"
#include "fnt.h"
#include "component/Button.h"

static bool isDown = false;
int point_x=0, point_y=0;
_FNT *fnt = NULL;
int timer = 0;
int number = 0;

Button *btn_ok,*btn_2;

void logoc(int data){
    cls(20,20,20);
    char text[300];
    sprintf(text,"定时器 运行第%d次\ntest", ++number);
    drawText(text, 100,300,  0xff30ff30,0,1);
    // fnt_drawline(fnt,text,10,10);
    ref(0,0,300,300);
}


void click_ok(int id)
{
 printf("%s：%d\n","点击",id);
 toast("按钮点击",0);
 //printf("点\n");
// return 0;
 }

//绘制画面
void draw_main()
{
    //绘制按钮
    btn_draw(btn_ok);
    btn_draw(btn_2);
    //刷新屏幕
}

int init()
{
    setscrsize(480,720);
    cls(60, 60, 60);
    // drawCircle(0, 0, 30, 0xff30f0f0);
    // int bmp = readBitmapFromAssets("ic_launcher.png");
    // int new_bmp = clipBitmap(bmp,0,0,50,140);
    
    // drawBitmapFlip(bmp, 30, 30,50,140,0,0);
// drawLine(point_x, point_y, 30, 30, 0xff202020);
    btn_ok = btn_create("测试1",20,20,720-40,120-40);
    btn_2 = btn_create("测试2",20,120+20,720-40,120-40);
    int time = getuptime();
    fnt = fnt_create("font.png", "font.fnt");
    printf(" fnt = %d\n",(int)fnt);
    // fnt_draw(fnt,33,30,30);
    // fnt_draw(fnt,(int)'a',50,30);
    draw_main();
    fnt_drawline(fnt,"影子论坛：\nhttp://bbs.yzjlb.net\n\n影子俱乐部网站：\nwww.yzjlb.net\n\n手机C交流QQ群：\n370468001\n╮（╯＿╰）╭\n~~~～",10,10);
    // drawText("测试", 30,30, 1, 0xff30ff30);
    // drawBitmap(fnt->bitmap,0,0);
    printf("耗时：%dms\n",getuptime()-time);
    ref(0,0,SCRW,SCRH);
    // 
    // fnt_free(fnt);
    timer = timercreate();
    // rectst rect = {0,30,300,300};
    // drawRect(rect.x, rect.y, rect.w,rect.h, 0xff2060a0);
    // drawTextEx("测试 test test test test test \n测试\n", 0, 290, &rect, 0xfff0f0f0, 0, 1);
    // rectst rect2 = {30,80,300,300};
    // drawTextEx("测试 test test test test test \n测试\n", 30, 300, &rect2, 0xfff0f0f0, 0, 1);
    //timerstart(timer, 1, 1, logoc, 1);
    // drawBitmap(bmp,0,0);
    // for(int i=0;i<5;i++)
    // drawBitmap(new_bmp, i*50, 0);
    // bitmapFree(new_bmp);
    // bitmapFree(bmp);

    ref(0,0,SCRW,SCRH);

}

int event(int type, int p1, int p2)
{

    if (type == MS_DOWN)
    {
        point_x = p1;
        point_y = p2;
        isDown = true;
    }
    else if (type == MS_MOVE)
    {
        if (isDown)
        {
            drawLine(point_x, point_y, p1, p2, 0xffffffff);
            ref(0, 0, 300, 300);

            point_x = p1;
            point_y = p2;
        }
    }
    else if (type == MS_UP)
    {
        isDown = false;
    }
    return 0;
}
int pause(void)
{
    return 0;
}
int resume(void)
{
    return 0;
}

int exitApp(void){
    fnt_free(fnt);
    timerstop(timer);
    timerdel(timer);
}
