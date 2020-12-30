
#ifndef _VIEWPAGER_H_
#define _VIEWPAGER_H_

#include "View.h"
#include "Gesture.h"
#include "ImageButton.h"

/*
XGUI 模拟安卓ViewPager
滑动界面
自带定时器

*/

typedef struct VIEW_PAGE{
 View *view; //当前页面
 struct VIEW_PAGE *next; //下一个页面
} VIEW_PAGE;

//创建ViewPager
typedef struct
{
  char *name; //控件名字
int id; //控件id
int isshow; //是否显示 不显示时将不会执行点击事件
int isdown; //是否按下
int x; //位置x
int y; //位置y
int w; //宽度
int h; //高度
int backgroundColor; //背景色
int draw_mode; //刷新模式
int timer; //定时器句柄
char *onclick; //点击事件回调函数

VIEW_PAGE *pages; //页面
int page_x;
int page_y;
int index;//当前页面
int page_load; //当前页面的显示进度
int speed; //加速度
int upx;
int upy;
GESTURE *ges;
} ViewPager;
#ifdef TCC
int pager_size(ViewPager *pager);
 void pager_draw(ViewPager *pager);
 View *pager_get(ViewPager *pager,int index);
 void pager_onleft(int data);
 void pager_onright(int data);
 View *pager_current(ViewPager *pager);
 void pager_run();
 void pager_move(ViewPager *pager,int move);
 int pager_isout(ViewPager *pager);
 void pager_goto(ViewPager *pager,int index);
#endif

//创建ViewPager
void *pager_create(int x,int y,int w,int h){
 ViewPager *pager = malloc(sizeof(ViewPager));
 memset(pager,0,sizeof(ViewPager));
 pager->name = "ViewPager";
 pager->id = 0;
 pager->isshow = 1;
 pager->x = x;
 pager->y = y;
 pager->w =w;
 pager->h=h;
 pager->backgroundColor = 0xff000000;
 pager->draw_mode = 0;
 pager->timer = timercreate();
 pager->ges = ges_create(x,y,w,h);
 ges_settag(pager->ges,pager);
 #ifndef TCC
 ges_setonleft(pager->ges, "pager_onleft");
 ges_setonright(pager->ges,"pager_onright");
 #else
 ges_setonleft(pager->ges, pager_onleft);
 ges_setonright(pager->ges,pager_onright);
 #endif
 return pager;
 }
 

void pager_onright(int data){
 printf("--> onleft %d\n",data);
 GESTURE *ges = (GESTURE*)data;
 ViewPager *pager = ges->tag;
 
 
 if(pager->index>0){
  pager->index--;
  }
 int move_x = (-pager->index)*pager->w- pager->page_x;
 if(move_x>0){
  pager->speed = SCRW/20;
 }
 pager_move(pager,pager->speed);
  
 if(pager->draw_mode==0){
   pager_draw(pager);
   ref(0,0,SCRW,SCRH);
   }
  //启用定时器
  #ifdef TCC
  timerstart(pager->timer, 33,(int)pager, pager_run,1);
  #else
  timerstart(pager->timer, 33,(int)pager, "pager_run",1);
  #endif
 }

void pager_onleft(int data){
 printf("--> onright %d\n",data);
 GESTURE *ges = (GESTURE*)data;
 ViewPager *pager = ges->tag;
 
 if(pager->index<pager_size(pager)-1){
  pager->index ++;
  }
  int move_x = (-pager->index)*pager->w- pager->page_x;
  if(move_x<0){
   pager->speed = -SCRW/20;
  }
  pager_move(pager,pager->speed);
  
  
  
  if(pager->draw_mode==0){
   pager_draw(pager);
   ref(0,0,SCRW,SCRH);
   }
  //启用定时器
  #ifdef TCC
  timerstart(pager->timer, 33,(int)pager, pager_run,1);
  #else
  timerstart(pager->timer, 33,(int)pager, "pager_run",1);
  #endif
 }

void pager_run(int data){
 if(data>=0 && data<100){
  printf("pager run error %d\n",data);
  return;
  }
  
 ViewPager *pager = (void*)data;
 int _move = (-pager->index)*pager->w;
 if(pager_isout(pager)){
  pager_goto(pager,pager->index);
  timerstop(pager->timer);
 }
   
  
 pager_move(pager,pager->speed);
 if(pager->speed<0 && pager->page_x<_move){
  pager->speed=0;
  pager_move(pager, _move-pager->page_x);
  timerstop(pager->timer);
  }
 if(pager->speed>0 && pager->page_x>_move){
  pager->speed=0;
  pager_move(pager,_move-pager->page_x);
  timerstop(pager->timer);
  }
  if(pager->draw_mode==0){
  pager_draw(pager);
  ref(0,0,SCRW,SCRH);
  }
 }

void pager_move(ViewPager *pager,int move){
 pager->page_x+=move;
 for(int i=0;i<pager_size(pager);i++){
  View *view = pager_get(pager,i);
 //将控件居中
 view->x += move;
 view->y = pager->y + (pager->h-view->h)/2;
 }
 }
 
//跳转到指定位置
void pager_goto(ViewPager *pager,int index){
 int _move = (-pager->index)*pager->w;
 pager_move(pager,_move-pager->page_x);
}

//向ViewPager中添加控件
void pager_add(ViewPager *pager, void *view){
 VIEW_PAGE *page = malloc(sizeof(VIEW_PAGE));
 View *temp=view;
 memset(page,0,sizeof(VIEW_PAGE));
 VIEW_PAGE *start = pager->pages;
 page->view = view;
 page->next = NULL;
 if(start==NULL){
  pager->pages = page;
 }
 else{
  while(start->next){
  start = start->next;
  }
 start->next = page;
 }
 
 int size = pager_size(pager);
 page->view->x = pager->x+(pager->w-temp->w)/2+(size-1)*pager->w;
 }

//使用手动刷新模式(当界面刷新采用全局定时器的时候，使用此模式)
void pager_setMode(ViewPager *pager,int mode){
 pager->draw_mode = mode;
 }
 
//设置页面
void pager_setindex(ViewPager *pager, int index){
 pager->index = index;
 }

//获取页面数量
int pager_size(ViewPager *pager){
 int size=0;
 VIEW_PAGE *page = pager->pages;
 while(page){
  page = page->next;
  size++;
  }
 return size;
}

View *pager_get(ViewPager *pager,int index){
 if(pager_size(pager)==0)return NULL;
 VIEW_PAGE *page = pager->pages;
 int i = 0;
 //printf("pager_current\n");
 while(page!=NULL){
 // printf("while\n");
  if(index ==i){
  // printf("parger_current return\n");
   return page->view;
   }
  page = page->next;
  i++;
  }
  
  return page->view;
 }

//获取当前页面
View *pager_current(ViewPager *pager){
 if(pager_size(pager)==0)return NULL;
 VIEW_PAGE *page = pager->pages;
 int i = 0;
 //printf("pager_current\n");
 while(page!=NULL){
 // printf("while\n");
  if(pager->index ==i){
  // printf("parger_current return\n");
   return page->view;
   }
  page = page->next;
  i++;
  }
  
  return page->view;
 }

//绘制
void pager_draw(ViewPager *pager){
 view_draw((View*)pager);
 //绘制内部页面
 for(int i=0;i<pager_size(pager);i++){
  View *page = pager_get(pager,i);
 if(page!=NULL){
 //printf("draw %s\n",page->name);
  if(strcmp (page->name,"ImageButton")==0){
   img_draw((void*)page);
   printf("img");
  }
  else
  view_draw(page);
 }
 
 }
 
 char *temp[309];
 sprintf(temp,"index=%d %d %d",pager->index,pager->page_x ,pager->page_y);
 dtext(temp,0,0,240,240,240,0,1);
}

//判断viewpager是否超出边界
int pager_isout(ViewPager *pager){
 if(pager->page_x>0) return 1;
 if(pager->page_x<(pager_size(pager)-1)*pager->w) return 1;
 return 0;
}

//恢复状态 如果滑动的屏幕过半，则index改变
void pager_up(ViewPager *pager){
 int mx = -pager->index*pager->w;
 if(pager->page_x - mx < -pager->w/2){
  pager->index++;
  if(pager->index >= pager_size(pager)){
   pager->index=pager_size(pager);
   }
  
 }
 else if(pager->page_x - mx > pager->w/2){
  pager->index--;
  if(pager->index<0)pager->index=0;
 }
 pager_goto(pager,pager->index);
}

//event事件
void pager_event(ViewPager *pager,int type,int p1,int p2){
 ges_event(pager->ges,type,p1,p2);
 //printf("pager get\n");
 
// printf("pager event %d\n",pager->index);
 if(type==MS_DOWN){
  pager->upx=p1;
  pager->upy = p2;
  }
 else if(type==MS_MOVE){
  pager_move(pager, p1-pager->upx);
  
  
  pager->upx=p1;
  pager->upy = p2;
  }
 else if(type==MS_UP){
  
  if(!ges_isleft(pager->ges) && !ges_isright(pager->ges)){
   //printf("~~~left%d",ges_isleft(pager->ges));
   //if(pager_isout(pager))
   pager_up(pager);
   
   
  }
  
  //view->x+= p1-pager->upx;
  //view->y+= p2-pager->upy;
  }
 if(pager->draw_mode==0)
 pager_draw(pager);
 
// printf("pager up\n");
 
}

//释放内存 你懂的
void pager_free(ViewPager *pager){
 //遍历释放控件的结构体
 VIEW_PAGE *pages = pager->pages;
 while(pages){
  VIEW_PAGE *temp_page = pages->next;
  free(pages);
  pages = temp_page;
  }
  timerdel(pager->timer);
  ges_free(pager->ges);
 //释放
 free(pager);
 }




#endif
