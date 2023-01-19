    void enqueue(struct proc *rp	/* this process is now runnable */)
 {
 /* Add 'rp' to one of the queues of runnable processes.  This function is
  * responsible for inserting a process into one of the scheduling queues.
  * The mechanism is implemented here.   The actual scheduling policy is
  * defined in sched() and pick_proc().
  *
  * This function can be used x-cpu as it always uses the queues of the cpu the
  * process is assigned to.
  */
//   int q = rp->p_priority;	 		/* scheduling queue to use */
//   struct proc **rdy_head, **rdy_tail;
   
//   assert(proc_is_runnable(rp));
    
//   assert(q >= 0);
    
//   rdy_head = get_cpu_var(rp->p_cpu, run_q_head);
//   rdy_tail = get_cpu_var(rp->p_cpu, run_q_tail);
   
   if(rp->tickets > 0){
        total_tckts = total_tckts + (rp->tickets);
    }
/********** Your enqueue modification should be below ************/
     /* Now add the process to the queue. */
  if (!rdy_head[q]) {		/* add to empty queue */
      rdy_head[q] = rdy_tail[q] = rp; 		/* create a new queue */
      rp->p_nextready = NULL;		/* mark new end */
  } 
  else {					/* add to tail of queue */
      struct proc *start = rdy_head[q];
	  //case that needs to be added to the head
	  if((start->tickets) <= (rp->tickets)){//If Head Smaller than Next Item
		//Next Item becomes new head, original head becomes tail
		//start swap, starting at the head
		rdy_head[q] = rp;
		rp->p_nextready = start; //Complete swap

	  }else{//Find Position To Insert
		//Complete the implementation

		//I Will complete the implementation by iterating through the array until I find a valid spot for RP
		struct proc *Left;
		struct proc *Right;
		//Idea is that the correct placement for rp will be in between Left and Right or it reaches the tail
		//If in between I can place rp in between accordingly, else I place it on the tail.
		Left = start;
		Right = Left->p_nextready;
		while(1){
			if(Right != NULL){//Right isn't at the endpoint of queue
			if((Left->tickets) >= (rp->tickets) && (rp->tickets) >= (Right->tickets) ){//Found correct position
				Left->p_nextready = rp;
				rp->p_nextready = Right;
				break;//Exit Loop
			}else{//Was not the correct position, continue iterating
				Left = Left->p_nextready;
				Right = Left->p_nextready;
			}
		}else{ //We're at the end of the queue
			Left->p_nextready = rp;
			rdy_tail[q] = rp;
			rp->p_nextready = NULL;
			break;
		}
		}
		
	  }
  }
 /********* Your enqueue modification should be above ************/
//   if (cpuid == rp->p_cpu) {
//  	  /*
//  	   * enqueueing a process with a higher priority than the current one,
//  	   * it gets preempted. The current process must be preemptible. Testing
//  	   * the priority also makes sure that a process does not preempt itself
//  	   */
//  	  struct proc * p;
//  	  p = get_cpulocal_var(proc_ptr);
//  	  assert(p);
//  	  if((p->p_priority > rp->p_priority) &&
//  			  (priv(p)->s_flags & PREEMPTIBLE))
//  		  RTS_SET(p, RTS_PREEMPTED); /* calls dequeue() */
//   }
//  #ifdef CONFIG_SMP
//   /*
//     * if the process was enqueued on a different cpu and the cpu is idle, i.e.
//     * the time is off, we need to wake up that cpu and let it schedule this new
//     * process
//     */
//   else if (get_cpu_var(rp->p_cpu, cpu_is_idle)) {
//  	  smp_schedule(rp->p_cpu);
//   }
//  #endif
 
//   /* Make note of when this process was added to queue */
//   read_tsc_64(&(get_cpulocal_var(proc_ptr)->p_accounting.enter_queue));
 
 
//  #if DEBUG_SANITYCHECKS
//   assert(runqueues_ok_local());
//  #endif
 }
