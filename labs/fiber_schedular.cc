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



void one(addr_t* pmain_stack, addr_t* pf_stack, pair_int * pl,preempt_t *pre,int* change){
	char digi1[]={'0','1','2','3','4','5','6','7','8','9'};
	hoh_debug("running\n");
	addr_t& main_stack=*pmain_stack;
	addr_t& f_stack=*pf_stack;
	char* retvalue=pl->ret;
	bool& done = *(pl->done);
	int i=1,j=1,k=1;
	int ans;
	int* shell_run=(pl->run_inst);
    //hoh_debug("inst cnt"<<*(pl->run_inst));
    int count=0;
    int arg=pl->arg;
    hoh_debug(arg);
	for(i=1;i<arg;i++){
       // hoh_debug("here");
		for(j=1;j<arg;j++){
			for(k=1;k<arg;k++)
				{
					count++;
                   
                    ans=i*j/k;
					
                           // for(int jk=0;jk<5;jk++)
                    
				}
               
        }
         pre->saved_stack=0;
       //  hoh_debug("yield\n");
        // hoh_debug("yield\n");
        stack_saverestore(f_stack,main_stack);
    }
    count++;
	ans=i*j/k;
    hoh_debug(ans);
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
    if(ans<0)
        ans+=1000000000;
	for(i=0;i<po;i++)
	{
 		retvalue[po-1-i]=digi1[ans%10];
 		ans=ans/10;
    }
    pre->saved_stack=0;
    (*shell_run)--;
    done=false; 
    hoh_debug("before final");
    hoh_debug(*shell_run);
    *change=1-*change;
    //hoh_debug("before final"<<(*run));
          // for(i=0;i<5;i++)
    //hoh_debug("yield\n");
	stack_saverestore(f_stack,main_stack);


}

void two(addr_t* pmain_stack, addr_t* pf_stack, pair_int * pl,preempt_t *pre,int* change){
	char digi1[]={'0','1','2','3','4','5','6','7','8','9'};
	hoh_debug("running\n");
	addr_t& main_stack=*pmain_stack;
	addr_t& f_stack=*pf_stack;
	char* retvalue=pl->ret;
	bool& done = *(pl->done);
	int i=2;
	int ans;
	bool t=false;
	int& shell_run=*(pl->run_inst);
    int arg=pl->arg;
	for(i=2;i<arg;i++)
	{
		//hoh_debug("in two\n");
        if(arg%i==0)
		{
			t=true;
		
		}
       // pre->saved_stack=0;
        // for(int ik=0;ik<5;ik++)
       // hoh_debug("yield\n");
		//stack_saverestore(f_stack,main_stack);
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
    pre->saved_stack=0;
    shell_run--;
    done=false; 
    hoh_debug(shell_run);
    *change=1-*change;
//  hoh_debug("before final");
      // for(int i=0;i<5;i++)
    //hoh_debug("yield\n");

    stack_saverestore(f_stack,main_stack);


}





void shell_step_fiber_schedular(shellstate_t& shellstate, addr_t& main_stack,preempt_t& preempt,addr_t stackptrs[], size_t stackptrs_size, addr_t arrays, size_t arrays_size,dev_lapic_t& lapic){

    //insert your code here
    
    int ind=1;
    while(ind<6 && (!shellstate.schd_slots[(shellstate.scheduler_run+ind)%5]))
    {
    	ind++;
        //hoh_debug("ind "<< ind);
    }
   // hoh_debug(ind);
    if(ind!=6)
    {
    	//hoh_debug("ind run "<<(shellstate.scheduler_run+ind)%5<<" "<< ind);
        if(shellstate.first[(shellstate.scheduler_run+ind)%5])
        {
            shellstate.first[(shellstate.scheduler_run+ind)%5]=false;
            uint32_t stk_off=((shellstate.scheduler_run+ind)%5+1)*((uint32_t)arrays_size)/10;
            //(*(shellstate.fibs[(shellstate.scheduler_run+ind)%5].run_inst))++;
            if(shellstate.fibs[(shellstate.scheduler_run+ind)%5].fun==1){
                stack_init5(stackptrs[(shellstate.scheduler_run+ind)%5],arrays,stk_off,&one,&main_stack,&stackptrs[(shellstate.scheduler_run+ind)%5],&shellstate.fibs[(shellstate.scheduler_run+ind)%5],&preempt,&shellstate.change);            
            }
            else
            {
                stack_init5(stackptrs[(shellstate.scheduler_run+ind)%5],arrays,stk_off,&two,&main_stack,&stackptrs[(shellstate.scheduler_run+ind)%5],&shellstate.fibs[(shellstate.scheduler_run+ind)%5],&preempt,&shellstate.change);            
            }


        }
        else{
            //hoh_debug("asasa");
        preempt.saved_stack=&stackptrs[(shellstate.scheduler_run+ind)%5];
        lapic.reset_timer_count(100);
       // hoh_debug("going to "<< (shellstate.scheduler_run+ind)%5);
    	stack_saverestore(main_stack,stackptrs[(shellstate.scheduler_run+ind)%5]);
       // hoh_debug("context switched out\n");
        lapic.reset_timer_count(0);
        //__asm("sti");
      
        __asm("sti\n\t");
        
        }
        shellstate.scheduler_run+=ind;
        shellstate.scheduler_run%=5;
    }


}
