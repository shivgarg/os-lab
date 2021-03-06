#include "labs/shell.h"
#include "util/debug.h"
#include "labs/vgatext.h"
#include <string.h>

//
// initialize shellstate
//
void shell_init(shellstate_t& state){
  state.inp[0]='\0';
  state.ind=0;

}

//
// handle keyboard event.
// key is in scancode format.
// For ex:
// scancode for following keys are:
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | esc |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |  0 |  - |  = |back|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 01  | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 0a | 0b | 0c | 0d | 0e |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | tab |  q |  w |  e |  r |  t |  y |  u |  i |  o |  p |  [ |  ] |entr|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 0f  | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 1a | 1b | 1c |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     |ctrl |  a |  s |  d |  f |  g |  h |  j |  k |  l |  ; |  ' |    |shft|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 1d  | 1e | 1f | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 2a |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
// so and so..
//
// - restrict yourself to: 0-9, a-z, esc, enter, arrows
// - ignore other keys like shift, control keys
// - only handle the keys which you're interested in
// - for example, you may want to handle up(0x48),down(0x50) arrow keys for menu.
//
void shell_update(uint8_t scankey, shellstate_t& stateinout){

    hoh_debug("Got: "<<unsigned(scankey));
    stateinout.keys++;
    stateinout.output=0;
    stateinout.out2[0]='\0';
    
    if((scankey>=2 && scankey<12) || (scankey>=16 && scankey<26) || (scankey>=30 && scankey<39) || (scankey>=44 && scankey<51) || scankey==57)
    {
      char c[2];c[0]='a';c[1]='\0';
      //strncat(stateinout.inp,c,10);
    }
    else if(scankey==72)
    {
        stateinout.leftarea-=1;
        if(stateinout.leftarea<0)
          stateinout.leftarea+=4;
    }
    else if(scankey==80)
    {
        stateinout.leftarea+=1;
        stateinout.leftarea%=4;
    }
    else if(scankey==75)
    {
      
    }
    else if(scankey==77)
    {

    }
    else if(scankey==28)
    {
      stateinout.output=1;
    }

    // increment the
}

int fib(int n);
//
// do computation
//
void shell_step(shellstate_t& stateinout){

  //
  //one way:
  // if a function is enabled in stateinout
  //   call that function( with arguments stored in stateinout) ;
  //
  stateinout.iter++;
    if(stateinout.iter<0)
      stateinout.iter=0;
  if(stateinout.output)
  {
      stateinout.out1=fib(stateinout.leftarea*20);
      stateinout.out2[0]='h';
      stateinout.out2[1]='i';
      stateinout.out2[2]='e';
      stateinout.out2[3]='\0';
  
  }

}


//
// shellstate -. renderstate
//
void shell_render(const shellstate_t& shell, renderstate_t& render){

  //
  // renderstate. number of keys pressed = shellstate. number of keys pressed
  //
  // renderstate. menu highlighted = shellstate. menu highlighted
  //
  // renderstate. function result = shellstate. output argument
  //
  // etc.
  //

  render.keys=shell.keys;
  
  render.leftarea=shell.leftarea;
  render.inp[0]='i';
  render.inp[1]='\0';
  //strncat(render.inp,shell.inp,100);
  render.output=shell.output;
  render.out2[0]='t';
  render.out2[1]='\0';
  //strncat(render.out2,shell.out2,100);
  render.out1=shell.out1;
  render.iter=shell.iter;
}


//
// compare a and b
//
bool render_eq(const renderstate_t& a, const renderstate_t& b){
  hoh_debug(""<<int(a.keys)<< "  "<<int(b.keys)<<" "<< int(a.iter));
  if(a.keys!=b.keys)
    return false;
  else if((b.iter/10000)%2!=((b.iter+1)/10000)%2)
    return false;
  else
    return true;
}

int fib(int n)
{
	if(n==0)
		return 1;
	else if(n==1)
		return 1;
	else
		return fib(n-1)+fib(n-2);
}
static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawtext(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);

//
// Given a render state, we need to write it into vgatext buffer
//
void render(const renderstate_t& state, int w, int h, addr_t vgatext_base){


  // this is just an example:
  //
  // Please create your own user interface
  //
  // You may also have simple command line user interface
  // or menu based interface or a combination of both.
  //
  fillrect(0,0,w,h,3,6,      w,h,vgatext_base);
  drawrect(0,0,w/4,h,1,7,  w,h,vgatext_base);

  fillrect(w/4,0,w,h,3,6,  w,h,vgatext_base);
  drawrect(w/4,0,w,h,1,7,  w,h,vgatext_base);
  drawtext(w/4+2,2,"$:> ", 4, 3,5,w,h,vgatext_base);

  if(state.leftarea==0)
    drawtext(2, 7,"1. Item1", w-25, 7,1,w,h,vgatext_base);
  else
    drawtext(2, 7,"1. Item1", w-25, 1,7,w,h,vgatext_base);
  if(state.leftarea==1)
    drawtext(2, 8,"2. Item2", w-25, 7,1,w,h,vgatext_base);
  else
    drawtext(2, 8,"2. Item2", w-25, 1,7,w,h,vgatext_base);
  if(state.leftarea==2)
    drawtext(2, 9,"3. Item3", w-25, 7,1,w,h,vgatext_base);
  else
    drawtext(2, 9,"3. Item3", w-25, 1,7,w,h,vgatext_base);
  if(state.leftarea==3)
    drawtext(2,10,"4. Item4", w-25, 7,1,w,h,vgatext_base);
  else
    drawtext(2,10,"4. Item4", w-25, 1,7,w,h,vgatext_base);
  if((state.iter/10000)%2==0)
    	drawtext(w/4+6,2,"_", 1, 3,5,w,h,vgatext_base);
  drawtext(0,13,"No of keys pressed:", w-25, 1,7,w,h,vgatext_base);
  drawnumberinhex(2,14,state.keys, w-25, 1,7,w,h,vgatext_base);

  
  
  if(state.output)
  { 
    drawtext(w/4+2,10,"Output: ",8,3,5,w,h,vgatext_base);
    drawtext(w/4+10,10,state.out2,w-25,3,5,w,h,vgatext_base);
    drawnumberinhex(w/4+10,11,state.out1,w-25,3,5,w,h,vgatext_base);
  }

  //drawtext(40,15,"Hello! ", w-25, 1,7,w,h,vgatext_base);

}


//
//
// helper functions
//
//

static void writecharxy(int x, int y, uint8_t c, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  vgatext::writechar(y*w+x,c,bg,fg,vgatext_base);
}

static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int y=y0;y<y1;y++){
    for(int x=x0;x<x1;x++){
      writecharxy(x,y,0,bg,fg,w,h,vgatext_base);
    }
  }
}

static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){

  writecharxy(x0,  y0,  0xc9, bg,fg, w,h,vgatext_base);
  writecharxy(x1-1,y0,  0xbb, bg,fg, w,h,vgatext_base);
  writecharxy(x0,  y1-1,0xc8, bg,fg, w,h,vgatext_base);
  writecharxy(x1-1,y1-1,0xbc, bg,fg, w,h,vgatext_base);

  for(int x=x0+1; x+1 < x1; x++){
    writecharxy(x,y0, 0xcd, bg,fg, w,h,vgatext_base);
  }

  for(int x=x0+1; x+1 < x1; x++){
    writecharxy(x,y1-1, 0xcd, bg,fg, w,h,vgatext_base);
  }

  for(int y=y0+1; y+1 < y1; y++){
    writecharxy(x0,y, 0xba, bg,fg, w,h,vgatext_base);
  }

  for(int y=y0+1; y+1 < y1; y++){
    writecharxy(x1-1,y, 0xba, bg,fg, w,h,vgatext_base);
  }
}

static void drawtext(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int i=0;i<maxw;i++){
    writecharxy(x+i,y,str[i],bg,fg,w,h,vgatext_base);
    if(!str[i]){
      break;
    }
  }
}

static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  enum {max=sizeof(uint32_t)*2+1};
  char a[max];
  for(int i=0;i<max-1;i++){
    a[max-1-i-1]=hex2char(number%16);
    number=number/16;
  }
  a[max-1]='\0';

  drawtext(x,y,a,maxw,bg,fg,w,h,vgatext_base);
}

