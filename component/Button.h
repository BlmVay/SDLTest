#ifndef _XGUI_BUTTON_
#define _XGUI_BUTTON_
#include "View.h"
#include "../base.h"
#include "../graphics.h"

/*
XGUI
  按钮控件 v1.1
　by：风的影子

  更新记录：
  细节修改
  修复点击事件event错误
  
  
影子俱乐部网站：www.yzjlb.net
手机C论坛：bbs.yzjlb.net
手机C交流QQ群：370468001

最后修改于：2019-11-10
*/

//按钮结构体

typedef struct
{
  char *name;          //控件名字
  int id;              //控件id
  int isshow;          //是否显示 不显示时将不会执行点击事件
  int isdown;          //是否按下
  int x;               //位置x
  int y;               //位置y
  int w;               //宽度
  int h;               //高度
  int backgroundColor; //背景色
  int draw_mode;       //绘制模式 0自动绘制 1手动绘制
  int timer;           //定时器句柄
  void *onclick;       //点击事件回调函数
  void *tag;           //标签

  char *text;           //按钮文字
  int textx;            //文字x坐标
  int texty;            //文字y坐标
  int rect_color;       //按钮边框颜色
  int light_color;      //点击高亮颜色
  int light_rect_color; //高亮边框颜色

} Button;

#ifdef TCC
extern int btn_impact(Button *btn, int x, int y);
#endif

//新增按钮
//参数分别是 按钮文字 按钮x坐标 y坐标 宽度 高度
void *btn_create(char *text, int x, int y, int w, int h)
{
  int fw, fh;
  Button *btn;
  btn = (Button *)malloc(sizeof(Button));
  memset(btn, 0, sizeof(Button));
  btn->name = "Button";
  btn->text = text;
  btn->x = x;
  btn->y = y;
  btn->w = w;
  btn->h = h;
  btn->id = 0;
  textwh(text, 0, 1, &fw, &fh);
  btn->textx = x + (w - fw) / 2;
  btn->texty = y + (h - fh) / 2;
  btn->backgroundColor = 0xfff0f5f5;
  btn->rect_color = 0xffa0d5f0;
  btn->light_color = 0xffe0e0f5;
  btn->light_rect_color = 0xffa0e0f5;
  btn->timer = timercreate();
  btn->isdown = 0;
  btn->isshow = 1;
  return btn;
}

//设置按钮id
void btn_setid(Button *btn, int id)
{
  btn->id = id;
}

//获取按钮id
int btn_getid(Button *btn)
{
  return btn->id;
}

//显示按钮 参数：按钮结构体指针
void btn_draw(Button *btn)
{
  if (btn->isshow)
  {
    if (btn->isdown)
    {

      //绘制一个矩形
      drawRect(btn->x, btn->y, btn->w, btn->h, btn->light_rect_color);
      drawRect(btn->x + 4, btn->y + 4, btn->w - 8, btn->h - 8, btn->light_color);

      //绘制文字
      dtext(btn->text, btn->textx, btn->texty, 50, 50, 50, 0, 1);
    }
    else
    {
      //绘制一个矩形

      drawRect(btn->x, btn->y, btn->w, btn->h, btn->rect_color);
      drawRect(btn->x + 4, btn->y + 4, btn->w - 8, btn->h - 8, btn->backgroundColor);

      //绘制文字
      dtext(btn->text, btn->textx, btn->texty, 50, 50, 50, 0, 1);
    }
  }
}

//设置按钮文字 参数：按钮结构体指针 新的按钮文字
void btn_settext(Button *btn, char *text)
{
  int fw, fh;
  btn->text = text;
  textwh(text, 0, 1, &fw, &fh);
  btn->textx = btn->x + (btn->w - fw) / 2;
  btn->texty = btn->y + (btn->h - fh) / 2;
  btn_draw(btn);
  ref(btn->x, btn->y, btn->w, btn->h);
}

//设置定时器刷新模式 0非定时器刷新 1定时器刷新
void btn_setref(Button *btn, int mode)
{
  btn->draw_mode = mode;
}

//显示按钮
int btn_show(Button *btn)
{
  btn->isshow = 1;
  btn_draw(btn);
  return 0;
}

//隐藏按钮
int btn_hide(Button *btn)
{
  btn->isshow = 0;
  btn->isdown = 0;

  return 0;
}

//按钮是否显示
int btn_isshow(Button *btn)
{
  return btn->isshow;
}

//设置点击回调函数 由于手机C不支持函数指针，所以采用char*，函数原型为定时器的回调函数void click_ok(int id)
void btn_setonclick(Button *btn, void *click)
{
  btn->onclick = click;
}

//按钮event事件
void btn_event(Button *btn, int type, int p1, int p2)
{

  if (type == MS_DOWN)
  {
    if (btn_impact(btn, p1, p2))
    {
      btn->isdown = 1;
      if (btn->draw_mode == 0)
      {
        btn_draw(btn);
        ref(btn->x, btn->y, btn->w, btn->h);
      }
      //printf("down");
    }
  }
  if (type == MS_UP)
  {
    //if(btn_impact(btn,p1,p2))
    {
      if (btn->isdown)
      {
        btn->isdown = 0;
        if (btn->draw_mode == 0)
        {
          btn_draw(btn);
          ref(btn->x, btn->y, btn->w, btn->h);
        }
        if (btn->onclick != NULL)
        {
          // printf("click");

          timerstart(btn->timer, 10, btn->id, btn->onclick, 0);
        }
      }
    }
  }
}

//判断按钮点击事件 参数：按钮指针 触屏x坐标 触屏y坐标
//返回值：TRUE(1)成功 FALSE(0)失败
int btn_impact(Button *btn, int x, int y)
{
  if (btn->isshow)
  {
    if (x > btn->x && x < btn->x + btn->w && y > btn->y && y < btn->y + btn->h)
      return 1;
  }

  return 0;
}

//释放按钮内存
void btn_free(Button *btn)
{
  timerdel(btn->timer);
  free(btn);
}

#endif
