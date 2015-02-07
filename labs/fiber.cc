#include "labs/fiber.h"

void g(addr_t* pmain_stack, addr_t* pf_stack, char* pret, bool* pdone,int arg){
	char digi1[]={'0','1','2','3','4','5','6','7','8','9'};
	addr_t& main_stack=*pmain_stack;
	addr_t& f_stack=*pf_stack;
	char* retvalue=pret;
	bool& done = *pdone;
	int i,j,k;
	int ans;
	
	for(i=1;i<arg;i++)
		for(j=1;j<arg;j++)
			for(k=1;k<arg;k++)
				{ans=i*j/k;
					//hoh_debug(""<<ans<<"\n");
					stack_saverestore(f_stack,main_stack);}
	ans=i*j/k;
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
	done=true;	
	stack_saverestore(f_stack,main_stack);


}

void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, addr_t& f_stack, addr_t f_array, uint32_t f_arraysize){
			
 		if(shellstate.init_fiber)
 		{
 			shellstate.fiber_done=false;;
 			
 			shellstate.init_fiber=false;
 			
 			stack_init5(f_stack,f_array,f_arraysize,&g,&main_stack,&f_stack,shellstate.inp[shellstate.fiber_out],&shellstate.fiber_done,shellstate.fiber_arg);
 		}
 		else if(!shellstate.fiber_done)
 		{
			stack_saverestore(main_stack,f_stack);
			
 		}

}

