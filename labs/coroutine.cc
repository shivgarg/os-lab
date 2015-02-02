#include "labs/coroutine.h"

char digi1[]={'0','1','2','3','4','5','6','7','8','9'};

void g(coroutine_t* pf_coro, f_t* pf_locals, char* ret, bool* done,int arg)
{
	coroutine_t& f_coro=*pf_coro;
	char* retvalue=ret;
	bool& cdone=*done;
	int& i = pf_locals->i;
	int& j=pf_locals->j;
	int& k=pf_locals->k;
	h_begin(f_coro);
	
	for(i=0;i<arg;i++)
		for(j=0;j<arg;j++)
			for(k=1;k<arg+1;k++)
			{
				int ans=i*j/k;
            	int po=0;
            	int p=ans;
            	while((p/10)!=0)
	            {
    		         po++;
            		 p=p/10;
            	}
            	po++;
            	retvalue[po]='\0';
            	for(int i=0;i<po;i++)
            	{
             		retvalue[po-1-i]=digi1[ans%10];
             		ans=ans/10;
            	}
            	cdone=false;
            	h_yield(f_coro);

			}
	int ans=i*j/k;
	int po=0;
	int p=ans;
	while((p/10)!=0)
    {
         po++;
		 p=p/10;
	}
	po++;
	retvalue[po]='\0';
	for(int i=0;i<po;i++)
	{
 		retvalue[po-1-i]=digi1[ans%10];
 		ans=ans/10;
	}
	cdone=true;
	h_end(f_coro);
}





void shell_step_coroutine(shellstate_t& shellstate, coroutine_t& f_coro, f_t& f_locals){
	if(shellstate.init_coroutine)
	{
		coroutine_reset(f_coro);
		f_locals.i=0;f_locals.j=0;f_locals.k=0;
		shellstate.done=false;
		shellstate.init_coroutine=false;
	}
	else if(!shellstate.done)
	{
		g(&f_coro,&f_locals,shellstate.coroutine_out,&shellstate.done,shellstate.coroutine_arg);
	}



}

