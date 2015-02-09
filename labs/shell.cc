#include "labs/shell.h"
#include "util/debug.h"
#include "labs/vgatext.h"
#include <string.h>





//
// initialize shellstate
//
void shell_init(shellstate_t& state){
  for(int i=0;i<200;i++)
	 state.inp[i][0]='\0';
  state.index=0;
  state.inp_ind=state.index;
  state.char_ind=0;
  state.change=0;
  state.left=1;
  state.leftarea=0;
  state.done=true;
  state.init_coroutine=false;
  state.coroutine_out=0;
  state.init_fiber=false;
  state.fiber_out=0;
  state.fiber_done=true;
  state.scheduler_assign=-1;
  state.scheduler_run=-1;
  for(int i=0;i<10;i++)
    state.schd_slots[i]=false;
}
int fib(int n);
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

   // hoh_debug("Got: "<<unsigned(scankey));
    stateinout.keys++;
    stateinout.output=0;
    char keymap[]=    {'?','~','1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',  'q','w','e','r','t','y','u','i','o','p','[',']','?','?','a','s',  'd','f','g','h','j','k','l',';','?','?','?','?','z','x','c','v',  'b','n','m',',','.','/','?'};
    //hoh_debug(int(stateinout.inp_ind)<<"\n");
    //hoh_debug("index key "<<int(stateinout.index)<<"\n");
    if((scankey>=2 && scankey<12) || (scankey>=16 && scankey<26) || (scankey>=30 && scankey<39) || (scankey>=44 && scankey<51))
    {
      stateinout.inp[stateinout.inp_ind][stateinout.char_ind]=keymap[scankey];
      stateinout.inp[stateinout.inp_ind][stateinout.char_ind+1]='\0';
      stateinout.change=1-stateinout.change;
      stateinout.char_ind+=1;
      if(stateinout.index!=stateinout.inp_ind)
	  		stateinout.index=stateinout.inp_ind;
	
    }
    else if(scankey==57)
    {
      stateinout.inp[stateinout.inp_ind][stateinout.char_ind]=' ';
      stateinout.inp[stateinout.inp_ind][stateinout.char_ind+1]='\0';
      stateinout.change=1-stateinout.change;
      stateinout.char_ind+=1;
      if(stateinout.index!=stateinout.inp_ind)
        stateinout.index=stateinout.inp_ind;
    }
    else if(scankey==14)
    {
      if(stateinout.char_ind-1>=0)
        stateinout.char_ind-=1;
      stateinout.inp[stateinout.inp_ind][stateinout.char_ind]='\0';
      stateinout.change=1-stateinout.change;
    }
    else if(scankey==72)
    {
        if(stateinout.left==0)
        {
        	stateinout.change=1-stateinout.change;
        	stateinout.index-=2;
        	if(stateinout.index<0)
        		stateinout.index+=200;
          int char_i=0;
        	for(int i=0;i<35;i++)
        	{
            if(stateinout.inp[stateinout.index][i]=='\0')
              break;
        		stateinout.inp[stateinout.inp_ind][i]=stateinout.inp[stateinout.index][i];
            char_i++;
        	}
          stateinout.char_ind=char_i;
          stateinout.inp[stateinout.inp_ind][char_i]='\0';
        }
        else
        {
        	stateinout.leftarea-=1;
        	if(stateinout.leftarea<0)
          		stateinout.leftarea+=4;
        }
    }
    else if(scankey==80)
    {
        if(stateinout.left==0)
        {
        		if(stateinout.index!=stateinout.inp_ind)
        		{
        			stateinout.change=1-stateinout.change;
        			stateinout.index+=2;
        			stateinout.index%=200;
        			int char_i=0;
          for(int i=0;i<35;i++)
          {
            if(stateinout.inp[stateinout.index][i]=='\0')
              break;
            stateinout.inp[stateinout.inp_ind][i]=stateinout.inp[stateinout.index][i];
            char_i++;
          }
          stateinout.char_ind=char_i;
          stateinout.inp[stateinout.inp_ind][char_i]='\0';
        				
        		}
        		

        }
        else
        {        
        	stateinout.leftarea+=1;
        	stateinout.leftarea%=4;
        }
    }
    else if(scankey==75)
    {
      stateinout.left-=1;
      if(stateinout.left<0)
      	stateinout.left+=2;
    }
    else if(scankey==77)
    {
    	stateinout.left+=1;
    	stateinout.left%=2;
    }
    else if(scankey==28)
    {
      stateinout.output=1;
      
    }

    // increment the
}


//
// do computation
//
void shell_step(shellstate_t& stateinout){
		
char digi[]={'0','1','2','3','4','5','6','7','8','9'};
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
  	stateinout.output=0;
      //hoh_debug("inp_deb"<<int(stateinout.inp_ind));
      stateinout.inp_ind+=2;
      stateinout.index=stateinout.inp_ind;
      stateinout.inp[stateinout.inp_ind][0]='\0';
      stateinout.inp[stateinout.inp_ind+1][0]='\0';
      stateinout.inp_ind%=200;
      stateinout.char_ind=0;
      stateinout.change=1-stateinout.change;
     // hoh_debug("inp_ind "<<stateinout.inp_ind);
      if(stateinout.inp[stateinout.inp_ind-2][0]=='f' && stateinout.inp[stateinout.inp_ind-2][1]=='i' && stateinout.inp[stateinout.inp_ind-2][2]=='b' && stateinout.inp[stateinout.inp_ind-2][3]==' ')
      {
       // hoh_debug("in fib");
        int i=4,arg=0;bool ill=false;;
        while(stateinout.inp[stateinout.inp_ind-2][i]!='\0')
        {
          
          if(int(stateinout.inp[stateinout.inp_ind-2][i])>=48 && int(stateinout.inp[stateinout.inp_ind-2][i])<=57)
            arg=arg*10+int(stateinout.inp[stateinout.inp_ind-2][i])-48;
          else
            {ill=true;break;}
          i++;
        }
        if(!ill)
        {
            int ans=fib(arg);
            int po=0;
            int p=ans;
            while((p/10)!=0)
            {
             po++;
             p=p/10;
            }
            po++;
            stateinout.inp[stateinout.inp_ind-1][po]='\0';
            for(int i=0;i<po;i++)
            {
             stateinout.inp[stateinout.inp_ind-1][po-1-i]=digi[ans%10];
             ans=ans/10;
            }
        }
        else
        {
          char tmp[17]="Illegal Argument";
          tmp[16]='\0';
          for(int g=0;g<17;g++)
            stateinout.inp[stateinout.inp_ind-1][g]=tmp[g];
        }
      }
      else if(stateinout.inp[stateinout.inp_ind-2][0]=='e' && stateinout.inp[stateinout.inp_ind-2][1]=='c' && stateinout.inp[stateinout.inp_ind-2][2]=='h' && stateinout.inp[stateinout.inp_ind-2][3]=='o' && stateinout.inp[stateinout.inp_ind-2][4]==' ')
      {
      // hoh_debug("in echo");
        int i=5;
        while(stateinout.inp[stateinout.inp_ind-2][i]!='\0')
        {
          stateinout.inp[stateinout.inp_ind-1][i-5]=stateinout.inp[stateinout.inp_ind-2][i];
          i++;
        }
        stateinout.inp[stateinout.inp_ind-1][i+1]='\0';

      }
      else if(stateinout.inp[stateinout.inp_ind-2][0]=='f' && stateinout.inp[stateinout.inp_ind-2][1]=='i' && stateinout.inp[stateinout.inp_ind-2][2]=='c' && stateinout.inp[stateinout.inp_ind-2][3]==' ')
      {
        int i=4,arg=0;bool ill=false;;
        while(stateinout.inp[stateinout.inp_ind-2][i]!='\0')
        {
          
          if(int(stateinout.inp[stateinout.inp_ind-2][i])>=48 && int(stateinout.inp[stateinout.inp_ind-2][i])<=57)
            arg=arg*10+int(stateinout.inp[stateinout.inp_ind-2][i])-48;
          else
            {ill=true;break;}
          i++;
        }
        if(!ill)
        {
            stateinout.coroutine_arg=arg;
            stateinout.init_coroutine=true;
            stateinout.coroutine_out=stateinout.inp_ind-1;
        }
        else
        {
          char tmp[17]="Illegal Argument";
          tmp[16]='\0';
          for(int g=0;g<17;g++)
            stateinout.inp[stateinout.inp_ind-1][g]=tmp[g];
        } 
      }
      else if(stateinout.inp[stateinout.inp_ind-2][0]=='f' && stateinout.inp[stateinout.inp_ind-2][1]=='i' && stateinout.inp[stateinout.inp_ind-2][2]=='f' && stateinout.inp[stateinout.inp_ind-2][3]==' ')
      {
        int i=4,arg=0;bool ill=false;;
        while(stateinout.inp[stateinout.inp_ind-2][i]!='\0')
        {
          
          if(int(stateinout.inp[stateinout.inp_ind-2][i])>=48 && int(stateinout.inp[stateinout.inp_ind-2][i])<=57)
            arg=arg*10+int(stateinout.inp[stateinout.inp_ind-2][i])-48;
          else
            {ill=true;break;}
          i++;
        }
        if(!ill)
        {
            stateinout.fiber_arg=arg;
            stateinout.init_fiber=true;
            stateinout.fiber_out=stateinout.inp_ind-1;
        }
        else
        {
          char tmp[17]="Illegal Argument";
          tmp[16]='\0';
          for(int g=0;g<17;g++)
            stateinout.inp[stateinout.inp_ind-1][g]=tmp[g];
        } 
      }
      else if(stateinout.inp[stateinout.inp_ind-2][0]=='s' && stateinout.inp[stateinout.inp_ind-2][1]=='c' && stateinout.inp[stateinout.inp_ind-2][2]=='h' && stateinout.inp[stateinout.inp_ind-2][3]=='d' && stateinout.inp[stateinout.inp_ind-2][4]==' ')
      {
        int i,arg=0;bool ill=false;
        if(stateinout.inp[stateinout.inp_ind-2][5]=='1' && stateinout.inp[stateinout.inp_ind-2][6]==' ')
        {
          i=7;
           while(stateinout.inp[stateinout.inp_ind-2][i]!='\0')
            {
              
              if(int(stateinout.inp[stateinout.inp_ind-2][i])>=48 && int(stateinout.inp[stateinout.inp_ind-2][i])<=57)
                arg=arg*10+int(stateinout.inp[stateinout.inp_ind-2][i])-48;
              else
                {ill=true;break;}
              i++;
            }
            if(!ill)
            {
                stateinout.scheduler_arg=arg;
                stateinout.scheduler_out=stateinout.inp_ind-1;
                stateinout.scheduler_assign=1;

            }
            else
            {
              char tmp[17]="Illegal Argument";
              tmp[16]='\0';
              for(int g=0;g<17;g++)
                stateinout.inp[stateinout.inp_ind-1][g]=tmp[g];
            }
        }
        else if(stateinout.inp[stateinout.inp_ind-2][5]=='2' && stateinout.inp[stateinout.inp_ind-2][6]==' ')
        {
              i=7;
              while(stateinout.inp[stateinout.inp_ind-2][i]!='\0')
              {
              if(int(stateinout.inp[stateinout.inp_ind-2][i])>=48 && int(stateinout.inp[stateinout.inp_ind-2][i])<=57)
                arg=arg*10+int(stateinout.inp[stateinout.inp_ind-2][i])-48;
              else
                {ill=true;break;}
              i++;
            }
            if(!ill)
            {
                stateinout.scheduler_arg=arg;
                stateinout.scheduler_out=stateinout.inp_ind-1;
                stateinout.scheduler_assign=2;

            }
            else
            {
              char tmp[17]="Illegal Argument";
              tmp[16]='\0';
              for(int g=0;g<17;g++)
                stateinout.inp[stateinout.inp_ind-1][g]=tmp[g];
            }          
        }
        else
        {
          char tmp[35]="Undefined Function";
          tmp[18]='\0';
          for(int g=0;g<35;g++)
            stateinout.inp[stateinout.inp_ind-1][g]=tmp[g];

        }
        
      }
      else
      {
        char s[16]="Illegal Command";
        s[15]='\0';
        for(int g=0;g<16;g++)
          stateinout.inp[stateinout.inp_ind-1][g]=s[g];
      }
  
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
  render.left=shell.left;
  
  render.iter=shell.iter;
  for(int i=0;i<200;i++)
    for(int j=0;j<35;j++)
      render.inp[i][j]=shell.inp[i][j];
  // for(int j=0;j<35;j++)
  //   render.coroutine_out[j]=shell.coroutine_out[j];
  // for(int j=0;j<35;j++)
  // 	render.fiber_out[j]=shell.fiber_out[j];
  render.inp_ind=shell.inp_ind;
  render.done=shell.done;
  render.fiber_done=shell.fiber_done;
  render.change=shell.change;
}


//
// compare a and b
//
bool render_eq(const renderstate_t& a, const renderstate_t& b){
  //hoh_debug(""<<int(a.keys)<< "  "<<int(b.keys)<<" "<< int(a.iter));
  if(a.keys!=b.keys)
    return false;
  else if((b.iter/10000)%2!=((b.iter+1)/10000)%2)
    return false;
  else if(b.change!=a.change || b.done!=a.done || b.fiber_done!=a.fiber_done)
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
  fillrect(0,0,w,h,0,6,      w,h,vgatext_base);
  drawrect(0,0,w/4-1,h,0,7,  w,h,vgatext_base);

  fillrect(w/4-1,0,w,h,0,6,  w,h,vgatext_base);
  drawrect(w/4-1,0,w,h,0,7,  w,h,vgatext_base);
 
  for(int i=0;i<20;i+=2)
  { 
    int tmp=state.inp_ind-i;
    int tmp1=state.inp_ind-i+1;
    if(state.inp_ind-i<0)
      tmp+=200;
    if(tmp1<0)
      tmp1+=200;
    drawtext(w/4,20-i,"$>", 2, 0,7,w,h,vgatext_base);
    drawtext(w/4+2,20-i,state.inp[tmp],35,0,7,w,h,vgatext_base);
    drawtext(w/4+2,20-i+1,state.inp[tmp1],35,0,7,w,h,vgatext_base);
  }
  if(state.left){
  if(state.leftarea==0)
    drawtext(2, 2,"1. Item1", w-25, 7,0,w,h,vgatext_base);
  else
    drawtext(2, 2,"1. Item1", w-25, 0,7,w,h,vgatext_base);
  if(state.leftarea==1)
    drawtext(2, 3,"2. Item2", w-25, 7,0,w,h,vgatext_base);
  else
    drawtext(2, 3,"2. Item2", w-25, 0,7,w,h,vgatext_base);
  if(state.leftarea==2)
    drawtext(2, 4,"3. Item3", w-25, 7,0,w,h,vgatext_base);
  else
    drawtext(2, 4,"3. Item3", w-25, 0,7,w,h,vgatext_base);
  if(state.leftarea==3)
    drawtext(2,5,"4. Item4", w-25, 7,0,w,h,vgatext_base);
  else
    drawtext(2,5,"4. Item4", w-25, 0,7,w,h,vgatext_base);}
  else{

  drawtext(2, 2,"1. Item1", w-25, 0,7,w,h,vgatext_base);  
  drawtext(2, 3,"2. Item2", w-25, 0,7,w,h,vgatext_base);
  drawtext(2, 4,"3. Item3", w-25, 0,7,w,h,vgatext_base);
  drawtext(2,5,"4. Item4", w-25, 0,7,w,h,vgatext_base);
  }
  // if((state.iter/10000)%2==0)
  //   	drawtext(w/4+6,2,"_", 1, 3,5,w,h,vgatext_base);
  drawtext(1,7,"Keys Presses:", 15, 0,7,w,h,vgatext_base);
  drawnumberinhex(2,8,state.keys, w-25, 0,7,w,h,vgatext_base);
  // drawtext(1,9,"Coroutine Output", 16, 1,7,w,h,vgatext_base);
  // if(state.done)
  //     drawtext(2,10,state.coroutine_out, 15, 1,7,w,h,vgatext_base);
  // drawtext(1,11,"Fiber Output",16,1,7,w,h,vgatext_base);
  // if(state.fiber_done)
  // 		drawtext(2,12,state.fiber_out,15,1,7,w,h,vgatext_base);
        
      
  

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

