#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/string.h>

	static int PID = -99;
	module_param(PID, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
	//MODULE_PARM_DESC( PID, "PID :))");
	
	static int Priority = -99;
	module_param(Priority, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
	//MODULE_PARM_DESC( Priority, "Priority :))");
	
	static unsigned Policy = -99;
	module_param(Policy, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
	//MODULE_PARM_DESC( ID, "Policy :))");
	
	void  info_the_task(struct task_struct *task)
{
	struct task_struct *child;
	struct list_head *head;

	for_each_process(task){
		if(task->pid == PID || task->prio == Priority || task->policy == Policy){
			printk(KERN_INFO "Executable name: %s \n", task->comm);
			printk(KERN_INFO "Process ID: %d \n", task->pid);
			printk(KERN_INFO "Priority: %d \n", task->prio);
			printk(KERN_INFO "Static priority: %d \n", task->static_prio);
			printk(KERN_INFO "Parent PID: %d \n", task->parent->pid);
			printk(KERN_INFO "Time slice: %s \n", task->rt.time_slice);
			printk(KERN_INFO "Policy: %d \n", task->policy);
			
			INIT_LIST_HEAD(&(child->children));
			
			list_for_each(head, &(task->children)){
				child=list_entry(head, struct task_struct, sibling);
				
				printk(KERN_INFO "Child executable name: %s \n", child->comm);
				printk(KERN_INFO "Child process ID: %d \n", child->pid);
				
			}
		}else if(PID<0 || Priority<0|| Policy<0){
			printk(KERN_INFO "Incorrect PID or Priority or Policy");
		
	}
}
}
	
	
	int myfileinfo_init(void)
	{    
	printk(KERN_INFO "Loading Schedule Info Module...\n");
	
    info_the_task(&init_task);

    return 0;
	}
	
	void myfileinfo_exit(void) {
	
		printk(KERN_INFO "Removing Module\n");

	}
	
	module_init(myfileinfo_init);
	module_exit(myfileinfo_exit);
	
	MODULE_LICENSE( "GPL");
	MODULE_DESCRIPTION( "Project1 - COMP304");
	MODULE_AUTHOR("Cem Uyuk");
	
	
