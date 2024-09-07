* 用户态的sysinfo()，首先系统会从user/user.h里的声明找到，用户空间的系统调用函数（如 sysinfo()）是通过链接到 usys.S 中的汇编代码来实现的。usys.S 文件定义了所有系统调用的入口点，这些入口点使用 .global 指令使函数名在链接时可见。当用户程序中调用 sysinfo() 时，链接器会将调用解析为 usys.S 中的 sysinfo 函数。
```
.global sysinfo
sysinfo:
 li a7, SYS_sysinfo
 ecall
 ret
```
* 如果有人使用sysinfo函数，就会把SYS_sysinfo这个系统调用号（define SYS_sysinfo 23）压入寄存器a7，然后调用ecall，使用ecall就会使用内核态运行SYS_sysinfo。

* 随后，在kernel/syscall.c中，syscall函数会读取a7，获得系统调用号（define SYS_sysinfo 23)，然后调用`p->trapframe->a0 = syscalls[num]();`，通过syscalls表获取对应的函数[SYS_sysinfo] sys_sysinfo，将运行结果存入a0。其中,sys_sysinfo由kernel/syscall.c中`extern uint64 sys_sysinfo(void);`调用。这个sys_sysinfo函数最后写在相关的文件里，比如sysproc.c里面。课程没说他应该写在哪，实际应该只是分类的区别。

int
copyout(pagetable_t pagetable, uint64 dstva, char *src, uint64 len)