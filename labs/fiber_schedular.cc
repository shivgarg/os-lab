#include "labs/fiber_schedular.h"
#include "util/debug.h"


//
// stackptrs:      Type: addr_t[stackptrs_size].  array of stack pointers (generalizing: main_stack and f_stack)
// stackptrs_size: number of elements in 'stacks'.
//
// arrays:      Type: uint8_t [arrays_size]. array of memory region for stacks (generalizing: f_array)
// arrays_size: size of 'arrays'. equal to stackptrs_size*STACK_SIZE.
//
// Tip: divide arrays into stackptrs_size parts.
// Tip: you may implement a circular buffer using arrays inside shellstate_t
//      if you choose linked lists, create linked linked using arrays in
//      shellstate_t. (use array indexes as next pointers)
// Note: malloc/new is not available at this point.
//



void one(addr_t* pmain_stack, addr_t* pf_stack, char* pret,int arg,pair_int *pp,preempt_t *pre){
	char digi1[]={'0','1','2','3','4','5','6','7','8','9'};
	//hoh_debug("running\n");
	addr_t& main_stack=*pmain_stack;
	addr_t& f_stack=*pf_stack;
	char* retvalue=pret;
	bool& done = *pp->done;
	int i=1,j=1,k=1;
	int ans;
	int& shell_run=*pp->slot;
	for(i=1;i<arg;i++)
		for(j=1;j<arg;j++)
			for(k=1;k<arg;k++)
				{
					hoh_debug("in two\n");
                    ans=i*j/k;
					pre->saved_stack=0;
                           // for(int jk=0;jk<5;jk++)
                    hoh_debug("yield\n");
                    stack_saverestore(f_stack,main_stack);
				}
	ans=i*j/k;
	int po=0;
	int p=ans;
    //hoh_debug("asas\n");
	while((p/10)!=0)
    {
         po++;
		 p=p/10;
	}
	po++;
	retvalue[po]='\0';
	for(i=0;i<po;i++)
	{
 		retvalue[po-1-i]=digi1[ans%10];
 		ans=ans/10;
	}
	done=false;	
	//hoh_debug("before final"<<(*run));
	shell_run--;
	//hoh_debug("before final"<<(*run));
    pre->saved_stack=0;
          // for(i=0;i<5;i++)
    hoh_debug("yield\n");
	stack_saverestore(f_stack,main_stack);


}

void two(addr_t* pmain_stack, addr_t* pf_stack, char* pret,int arg,pair_int *pp,preempt_t *pre){
	char digi1[]={'0','1','2','3','4','5','6','7','8','9'};
	//hoh_debug("running\n");
	addr_t& main_stack=*pmain_stack;
	addr_t& f_stack=*pf_stack;
	char* retvalue=pret;
	bool& done = *pp->done;
	int i=2;
	int ans;
	bool t=false;
	int& shell_run=*pp->slot;
	for(i=2;i<arg;i++)
	{
		hoh_debug("in two\n");
        if(arg%i==0)
		{
			t=true;
		
		}
        pre->saved_stack=0;
        // for(int ik=0;ik<5;ik++)
        hoh_debug("yield\n");
		stack_saverestore(f_stack,main_stack);
	}
	if(t)
		ans=0;
	else
		ans=1;
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
	done=false;	
	shell_run--;
//	hoh_debug("before final");
    pre->saved_stack=0;
      // for(int i=0;i<5;i++)
    hoh_debug("yield\n");

    stack_saverestore(f_stack,main_stack);


}





void shell_step_fiber_schedular(shellstate_t& shellstate, addr_t& main_stack,preempt_t& preempt,addr_t stackptrs[], size_t stackptrs_size, addr_t arrays, size_t arrays_size,dev_lapic_t& lapic){

    //insert your code here
    
   	stackptrs[9]=arrays+arrays_size;
    if(shellstate.scheduler_assign==1)
    {
    	//hoh_debug("in scheduler_assign"<< shellstate.scheduler_assign);
    	int slt=-1;
    	for(int i=0;i<5;i++)
    	{
    		if(!shellstate.schd_slots[i])
    			{	
    				shellstate.schd_slots[i]=true;
    				slt=i;
    				break;
    			}
    	}
    	//hoh_debug("slot "<<slt);
    	if(slt==-1)
    	{
    		char tmp[25]="Low on Resources";
    		tmp[16]='\0';
    		for(int l=0;l<25;l++)
    			shellstate.inp[shellstate.scheduler_out][l]=tmp[l];

    	}
    	else if(shellstate.run_instances[0]==3)
    	{
    		char tmp[25]="3 Instances running";
    		tmp[19]='\0';
    		shellstate.schd_slots[slt]=false;
    		for(int l=0;l<25;l++)
    			shellstate.inp[shellstate.scheduler_out][l]=tmp[l];
    	}	
    	else{
    	//stackptrs[slt]=arrays+(slt+1)*(arrays_size)/10;
    	uint32_t stk_off=(slt+1)*((uint32_t)arrays_size)/10;
    	shellstate.run_instances[0]+=1;
        shellstate.fib1.done=&shellstate.schd_slots[slt];
        shellstate.fib1.slot=&shellstate.run_instances[0];
    	stack_init6(stackptrs[slt],arrays,stk_off,&one,&main_stack,&stackptrs[slt],shellstate.inp[shellstate.scheduler_out],shellstate.scheduler_arg,&shellstate.fib1,&preempt);
    	}
    	shellstate.scheduler_assign=-1;
    }
    else if(shellstate.scheduler_assign==2)
    {
    	//hoh_debug("in scheduler_assign gjty  "<< shellstate.scheduler_assign);
    	int slt=-1;
    	for(int i=0;i<5;i++)
    	{
    		if(!shellstate.schd_slots[i])
    			{	
    				shellstate.schd_slots[i]=true;
    				slt=i;
    				break;
    			}
    	}
    	//hoh_debug("slot "<<slt);
    	if(slt==-1)
    	{
    		char tmp[25]="Low on Resources";
    		tmp[16]='\0';
    		for(int l=0;l<25;l++)
    			shellstate.inp[shellstate.scheduler_out][l]=tmp[l];

    	}
    	else if(shellstate.run_instances[1]==3)
    	{
    		char tmp[35]="3 Instances running";
    		tmp[19]='\0';
    		shellstate.schd_slots[slt]=false;
    		for(int l=0;l<25;l++)
    			shellstate.inp[shellstate.scheduler_out][l]=tmp[l];
    	}
    	else{
    	//stackptrs[slt]=arrays+(slt+1)*(arrays_size)/10;
    	uint32_t stk_off=(slt+1)*((uint32_t)arrays_size)/10;
    	shellstate.run_instances[1]+=1;
        shellstate.fib2.done=&shellstate.schd_slots[slt];
        shellstate.fib2.slot=&shellstate.run_instances[1];
    	stack_init6(stackptrs[slt],arrays,stk_off,&two,&main_stack,&stackptrs[slt],shellstate.inp[shellstate.scheduler_out],shellstate.scheduler_arg,&shellstate.fib2,&preempt);
    	}
    	shellstate.scheduler_assign=-1;
    }
    int ind=1;
    while(ind<6 && (!shellstate.schd_slots[(shellstate.scheduler_run+ind)%5]))
    {
    	ind++;
    }
    
    if(ind!=6)
    {
    	//hoh_debug("ind run "<<(shellstate.scheduler_run+ind)%5<<" "<< ind);
        preempt.saved_stack=(addr_t)&stackptrs[(shellstate.scheduler_run+ind)%5];
        lapic.reset_timer_count(2);
    	stack_saverestore(main_stack,stackptrs[(shellstate.scheduler_run+ind)%5]);
        lapic.reset_timer_count(0);
        hoh_debug("context switched out\n");
        __asm("sti\n\t");
        lapic.reset_timer_count(0);
    	shellstate.scheduler_run+=ind;
    	shellstate.scheduler_run%=5;
    }


}
