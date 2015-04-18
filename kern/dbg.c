#include <dbg.h> 
#include <type.h>
#include <vga.h>
#include <printk.h>

/* print current runlevel and register */
void print_cur_status(){
    static int round = 0;
    uint16_t reg1, reg2, reg3, reg4;
    __asm__ __volatile__( 	"mov %%cs, %0;"
			"mov %%ds, %1;"
			"mov %%es, %2;"
			"mov %%ss, %3;"
			: "=m"(reg1), "=m"(reg2), "=m"(reg3), "=m"(reg4));

    printk("%d: @ring%d\n", round, reg1 & 0x3);
	printk("%d: cs = %x\n", round, reg1);
	printk("%d: ds = %x\n", round, reg2);
	printk("%d: es = %x\n", round, reg3);
	printk("%d: ss = %x\n", round, reg4);
    // round?
	++round;
}

void print_stack_trace(){
    uint32_t *ebp, *eip;
    __asm__ __volatile__("mov %%ebp, %0":"=r"(ebp));
    while (ebp){
        eip = ebp + 1;
        printk("[0x%x]\n", *eip);
        ebp = (uint32_t *) *ebp;
    }
}

void panic(const char *msg){
    setcolor(COL_L_RED, COL_BLACK);
    printk("*** Kernel panic %s ***\n", msg);
    printk("Current CPU status:\n");
    print_cur_status();

    //i can't get the name of funciton form call stack, so don't use it
    //printk("Call stack:\n");
    //print_stack_trace();
    for (;;);
}

