#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/irq.h>
#include <linux/interrupt.h>

#include <linux/irqreturn.h>

MODULE_LICENSE("GPL");

#define DPRINT(x)	printk("<%s> " #x " = 0x%x\n", __func__, (int)x);

static struct work_struct mywork;

static void work_handler(struct work_struct *data)
{
	volatile int i = 0;

	printk("work on\n");	
	DPRINT(&i);	

	DPRINT(current);
	DPRINT(current->pid);
	printk("current->comm = %s\n", current->comm);

	for (i = 0; i < 800000000; i++)
		;

	printk("work off\n");	
	
	return;
}

struct tasklet_struct mytasklet;

void beep(unsigned long data)
{
	volatile int i = 0;

	printk("beep on\n");	
	DPRINT(&i);	

	DPRINT(current);
	DPRINT(current->pid);
	printk("current->comm = %s\n", current->comm);

	for (i = 0; i < 800000000; i++)
		;

	printk("beep off\n");	

	return;
}

irqreturn_t myhandler(int irq, void * p)
{
	int tmp;
//	printk("irqmyhandler called!\n");
	DPRINT(irq);
	DPRINT(&tmp);

	DPRINT(current);
	DPRINT(current->pid);
	printk("<myhandler> current->comm = %s\n", current->comm);

//	beep();
	tasklet_schedule(&mytasklet);
	schedule_work(&mywork);

	return IRQ_HANDLED;	
}

static __init int interrupt_init(void)
{
	int ri;
	int btn_irq;

	DPRINT(&btn_irq);	
	DPRINT(interrupt_init);
	DPRINT(current);
	DPRINT(current->pid);
	printk("current->comm = %s\n", current->comm);
	
	tasklet_init(&mytasklet, beep, 0);

	INIT_WORK(&mywork, work_handler);

	btn_irq = 160;

//	ri = request_irq(17, myhandler, 0, "my test interrupt", NULL);
//	ri = request_irq(17, 1, 0, "my test interrupt", NULL);
	ri = request_irq(btn_irq, myhandler, IRQF_TRIGGER_FALLING, "btn K1 interrupt", NULL);
	ri = request_irq(btn_irq+1, myhandler, IRQF_TRIGGER_FALLING, "btn K2 interrupt", NULL);

	DPRINT(ri);

	return 0;
}

static __exit void interrupt_exit(void)
{
	DPRINT(interrupt_exit);

	free_irq(160, NULL);
	free_irq(161, NULL);

	return;
}

module_init(interrupt_init);
module_exit(interrupt_exit);
