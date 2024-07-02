# c 进程 线程

## 进程

`top -p 35605` 

`chrt -p 35605`

观测指定pid进程执行策略,优先级.



### **进程控制块**

进程控制块就是用于保存一个进程信息的结构体，又称之为PCB

> - `process state` 进程状态
> - `process number` 进程由pid唯一标记。
> - `program counter` PC值
> - `registers` 寄存器的值
> - `memory limits` 内存中的管理信息 (起始地址 空间 虚拟内存 ..)
> - `list of open files` 打开的文件

PCB结构体中的部分数据 

**调度数据**

​		进程的状态、标志、优先级、调度策略等。 

**时间数据**

​		创建该进程的时间、在用户态的运行时间、在内核态的运   			行时间等。 

**文件系统数据** 

​		umask掩码、文件描述符表等。 

​		内存数据、进程上下文、进程标识（进程号） 

​		... 



### 进程的上下文

- `text` 代码 (二进制)  只读 **文字常量区、代码区**
- `data` 全局和静态变量数据 **静态全局区**
- `stack 栈`  存放**局部变量，函数返回值**  FIFO
- `heap 堆`    程序运行时的**动态内存分配**
- `PCB 进程控制块` 包含进程信息(控制信息等)

data stack heap : 支撑进程运行的环境

<img src="assets/image-20220221115950820.png" alt="image-20220221115950820" style="zoom:25%;" />



### 进程号

每个进程都由一个进程号来标识，其**类型为pid_t**. 

进程号的范围：0～32767 

进程号总是唯一的，但进程号可以重用。当一个进程终止后，其进程号就可以再次使用了

在ubuntu中查看当前系统中所有的开启的进程

> ps ajx 

PPID：当前进程的**父进程**的进程号 

PID：**当前进程**的进程号 

PGID：当前进程**所在的组**的进程组ID 

COMMAND：当前进程的**名字** 

###### 特殊进程号

进程号为**0**的进程通常是**调度进程**，常被称为**交换进程**(swapper)。 

进程号为**1**的进程通常是**init进程**，init进程是所有进程的祖先.

###### PID PPID PGID

**PID**　当前进程的进程号　标识进程的一个非负整型数。 

**PPID**父进程号　任何进程(除init进程)都是由另一个进程创建，该进程称为被创建进程的父进程，对应的进程号称为父进程号(PPID)。 

**PGID**进程组是一个或多个进程的集合。他们之间相互关联，进程组可以接收同一终端的各种信号，关联的进程有一个进程组号(PGID) 。 

```c
#include <sys/types.h> 
#include <unistd.h> 

功能：获取当前进程的进程号 
	pid_t getpid(void); 
功能：获取当前进程的父进程的进程号 
	pid_t getppid(void);    
功能：获取当前进程所在进程组的id
    pid_t getpgid(pid_t pid); 
```



### 常用函数

##### fork函数 创建

```c
pid_t fork(void); 
```

###### 功能：

在已有的进程基础上有创建一个子进程 

###### 参数： 

> 无

###### 返回值：

>  成功：  >0  返回给父进程，子进程的进程号，
>
>  失败： ‐1 返回给父进程，子进程不会创建

###### 性质

使用fork函数得到的子进程是父进程的一个复制品，它从父进程处继承了整个进程的地址空间.

地址空间: 

> 包括进程上下文、进程堆栈、打开的文件描述符、信号控制设定、进程优先级、进程组号等。
>
> 子进程所独有的**只有**它的**进程号，计时器**等。因此，使用fork函数的代价是很大的。

- 通过fork函数的返回值来区分父子进程的独立的代码区
- 父子进程拥有独立的地址空间

- 子进程会复制父进程fork之前的所有内容
- 子进程继承父进程的部分公有的区域, 如部分内核空间

fork之后，父子进程完全独立，所以不管双方怎么改变（堆区、栈区、数据区等），都不会受对方影响

子进程会继承父进程的一些公有的区域，比如如磁盘空间，内核空间文件描述符的偏移量保存在内核空间中，所以父进程改变偏移量，则子进程获取的偏移量是改变之后的

###### 示例

```c
pid_t pid1,pid2;
static int b = 777;
int c = 888;

pid1 = fork();
pid2 = fork();
if(pid1 < 0)
{
	perror("fail to fork");
    return -1;
}
if(pid1 = 0)  //子进程1的代码区
{
	printf("This is a son process1\n");
	printf("a = %d, b = %d, c = %d\n", a, b, c);
}
if(pid2 = 0)  //子进程2的代码区
{
	printf("This is a son process2\n");
	printf("a = %d, b = %d, c = %d\n", a, b, c);
}
else  //主进程的代码区
{ 
	printf("This is a parent process\n");
	a++;
	b++;
	c++;
	printf("a = %d, b = %d, c = %d\n", a, b, c);
}
```

##### vfork函数 创建

```c
#include <sys/types.h> 
#include <unistd.h>
pid_t vfork(void);
```

###### 性质

功能 参数 返回值 与fork函数 几乎一样

<img src="assets/image-20211214231127229.png" alt="image-20211214231127229" style="zoom:50%;" />

但它们创建的子进程是有区别的

- vfork保证**子进程先运行**，在它调用**exec或exit之后**，父进程才可能被调度运行
- vfork并**不将**父进程的**地址空间完全复制**到子进程中，**子进程和父进程共享同一块空间**,因为子进程会立即调用exec(或exit)，于是也就不访问该地址空间.

###### 示例

```c
pid_t pid = vfork(); 
int num = 1;
if(pid<0) perror("vfork"); 
if(pid==0) 
{ 
    int i = 0; 
    for(i=0;i<3;i++) 
    { 
        num++;
        printf("this is son process\n"); 
    }
    _exit(0); 
}
else 
{ 
    printf("this is father process\n"); 
    printf("num=%d\n", num); 
	sleep(1);    
}

输出为
    this is son process
    this is son process
    this is son process
    this is father process
    num=4
```



##### sleep函数 进程的挂起

```c
#include <unistd.h> 
unsigned int sleep(unsigned int seconds); 
```

###### 功能：

> 进程在一定的时间内没有任何动作，称为进程的挂起（进程处于等待态） 

###### 参数： 

> seconds：指定要挂起的秒数 

###### 返回值： 

> 若进程挂起到sec指定的时间则返回0，若有信号中断则返回剩余秒数 

###### 性质

进程挂起指定的秒数后程序并不会立即执行，系统只是将此进程切换到就绪态 

sleep运行时进程为等待态，时间到达后会==先切换到就绪态==，如果代码继续运行，再切换到运行态

###### 实例:

```c
while(1)
{
	printf("morning");
	sleep(1);
}
```



##### wait函数 挂起等待子进程 

```c
#include <sys/types.h> 
#include <sys/wait.h>
pid_t wait(int *status);
```

###### 功能：

> - 等待**任意一个**子进程终止，如果子进程终止了，此函数会==回收子进程的资源==。
> - 调用wait函数的进程会==挂起==，直到它的一个==子进程退出==或收到一个不能被忽视的信号时才被唤醒. 
> - 若调用进程没有子进程或它的子进程已经结束，该函数立即返回。
> - 接收 void exit(int status)  void _exit(int status) 函数的返回值

###### 参数：

> - status：函数返回时，参数status中保存着子进程退出时的状态信息。 
>
> - 子进程的退出信息在 int *status中包含了多个字段， 用宏定义可以取出其中的每个字段
>
> - 接收 void exit(int status)  void _exit(int status) 函数的返回值
>
>   **取出子进程的退出信息** 
>
>   WIFEXITED(status) 
>
>   ​     如果子进程是正常终止的，取出的字段值非零。 
>
>   WEXITSTATUS(status) 
>
>   ​    返回子进程的退出状态，退出状态保存在 status 变量的 8~16 位。在用此宏前应先用宏 WIFEXITED 判断子进 程是否正常退出，正常退出才可以使用此宏。

###### 返回值：

> - 如果执行成功则返回子进程的进程号。 
> - 出错返回-1，失败原因存于 errno 中。

###### 示例

```c
pid_t pid; 
pid=fork(); 
if(pid<0) 
    perror("fork"); 
if(pid==0) 
{ 
    int i = 0; 
    for(i=0;i<5;i++) 
    { 
        printf("this is son process\n"); 
        sleep(1); 
    }
    _exit(2); 
}
else 
{ 
    int status = 0; 
    wait(&status); 
    if(WIFEXITED(status)!=0) 
    { 
        printf("son process return %d\n", WEXITSTATUS(status)); 
    }
    printf("this is father process\n"); 
}
```

##### waitpid函数 等待子进程终止

```c
#include <sys/types.h> 
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *status,int options) 
```

###### **功能：**

> 等待子进程终止，如果子进程终止了，此函数会回收子进程的资源。 

###### **参数**

> `pid_t waitpid(pid_t pid, int *status,int options) `
>
> pid：指定的进程或者进程组 
>
> status：保存子进程退出时的状态信息 (可以设置NULL)
>
> options ： 选项 
>
> ​	其中 **参数 pid 的值有以下几种类型**
>
> > pid>0：等待进程 ID 等于 ==pid 的子进程==。 
> >
> > pid=0:  等待==同一个进程组==中的==任何一个子进程==，如果子进程已经加入了别的进程组，waitpid 不会等待它。 
> >
> > pid=-1：等待==任意一个子进程==，此时 waitpid 和 wait 作用一样。 
> >
> > pid<-1：等待==指定进程组==中的任何子进程，这个进程组的 ID 等于 pid 的绝对值。
>
> **options 参数能进一步控制 waitpid 的操作：** 
>
> > 0： 同 wait，阻塞父进程，等待子进程退出。 
> >
> > WNOHANG： 没有任何已经结束的子进程，则立即返回。 
> >
> > WUNTRACED: 如果子进程暂停了则此函数马上返回，并且不予以理会子进程的结束状态。（跟踪调试，很少用到） 

###### **返回值:** 

> 成功: 子进程的进程号。 
>
> 出错返回-1，失败原因存于 errno 中。
>
> ​		printf("errno=%d\n",errno);

###### 示例:

```c
pid_t pid; 
pid=fork(); 
if(pid < 0) 
    perror("fork"); 
if(pid == 0) 
{ 
    int i = 0; 
    for(i=0;i<5;i++)
    { 
        printf("this is son process\n"); 
        sleep(1); 
    }
    _exit(2); 
}else 
{ 
    # pid：指定的进程或者进程组
    # status：保存子进程退出时的状态信息
    # 0： 同 wait，阻塞父进程，等待子进程退出
    int *status;
    waitpid(pid, status, 0); 
    printf("this is father process\n"); 
}
```

##### system函数 执行shell命令

`#include <stdlib.h> `

`int system(const char *command);`

```c
int status = system("ls -alh"); 
```

###### 功能：

- system 会调用 fork 函数**产生子进程**，
- 子进程**调用 exec** 启动/bin/sh -c string 来执行**参数 command 字符串**所代表的命令，
- 命令执行完后返回原调用进程。 

###### 参数：

> 要执行的命令的字符串。 

###### 返回值：

> 如果 command 为 NULL，则 system()函数返回非 0，一般为 1。
>
> 如果 system()在调用/bin/sh 时失败则返回 127，其它失败原因返回-1。
>
> system 调用成功后会返回执行 shell 命令后的**返回值**。其返回值可能为 1、127 也可能为-1，故最好应再检查 **errno** 来确认执行成功。

```c
int status; 
status = system("ls -alh"); 
if(WIFEXITED(status)) 
{ 
    printf("the exit status is %d \n", status); 
}else 
{ 
    printf("abnornamal exit\n"); 
}
```

##### exec 函数族 

六个 exec 函数中只有 execve 是真正意义的系统调用(内核提供的接口)，其它函数都是在此基础上经过封装 的库函数.

###### 功能：

> 在一个进程里面执行另一个程序，主要用于执行命令

###### 参数:

> path：命令或者程序的路径 
>
> ==l==：(list)如果是带l的函数，对应的命令或者程序是通过每一个参数进行传递的，最后一个为NULL表示结束
>
> > 例如："ls", "‐l", NULL 
> >
> > execl("/bin/ls", "ls", "-a", "-l", "-h", NULL);
>
> ==v==:   (vector)如果是带v的函数，对应的命令或者程序是通过一个指针数组来传递的， 指针数组的最后一个元素为NULL标识结束
>
> > char *str[] = {"ls", "‐l", NULL}; 
> >
> > execv("/bin/ls", str); 
>
> ==p==：(path)如果是不带p的函数，第一个参数必须传当前命令或者程序的**绝对路径**， 如果是带p的函数，第一个参数既可以是**绝对路径，也可以是相对路径** 
>
> > execlp("ls", "ls", "-a", "-l", "-h", NULL);
>
> ==e==:(environment)存有环境变量字符串地址的指针数组的地址。execle 和 execve 改变的是 exec 启动的程序的环境变量（新的环境变量完全由 environment 指定），其他四个函数启动的程序则使用默认系统环境变量。
>
> > char *env[]={"USER=ME", "GONGSI=QF", NULL}; 
> >
> > execle("./test", "test", NULL, env);

###### 返回值:

> exec 函数族与一般的函数不同，exec 函数族中的函数执行成功后不会返回。
>
> 只有调用失败了，它们才会返回 －1

一个进程调用 exec 后，除了进程 ID，进程还保留了下列特征不变： 

> 父进程号   进程组号   控制终端   根目录 
>
> 当前工作目录   进程信号屏蔽集   未处理信号

### 进程的终止

#### exit _exit 退出当前进程

在 linux 下可以通过以下方式结束正在运行的进程： 

```c
void exit(int value); 
void _exit(int value); 
```

##### exit函数

```c
#include <stdlib.h> 
void exit(int status) 
```

功能：退出当前进程 

参数：

> status：退出状态,由父进程通过wait函数接收这个状态(低 8 位有效)。 
>
> - 一般失败退出设置为非0 
> - 一般成功退出设置为0 

返回值: 无

`exit(0); `

##### _exit  函数

```c
#include <unistd.h> 
void _exit(int value)
```

功能：退出当前进程 

参数：

> status：退出状态,由父进程通过wait函数接收这个状态(低 8 位有效)。 
>
> - 一般失败退出设置为非0 
> - 一般成功退出设置为0 

返回值: 无

`_exit(0); `

##### exit 和 exit的区别

exit 为库函数，而_exit 为系统调用. 

exit库函数 刷新I/O缓冲后, 调用了 _exit 系统调用.

_exit 系统调用 不刷新I/O缓冲

| exit           | _exit          |
| -------------- | -------------- |
| 库函数         | 系统调用       |
| 刷新缓冲区     | 不会刷新缓冲区 |
| 一般会使用exit |                |

<img src="assets/image-20211214224209895.png" alt="image-20211214224209895" style="zoom:33%;" />

####  atexit 进程退出清理

进程在退出前可以用 atexit 函数**注册退出处理函数**。

```c
#include <stdlib.h> 
int atexit(void (*function)(void));
```

###### 功能：

> 注册进程**正常结束前**调用的函数，进程退出执行**注册函数**。 

###### 参数：

> function：进程结束前，调用函数的入口地址。
>
> 一个进程中可以多次调用 atexit 函数注册清理函数，正常结束前调用函数的顺序和注册时的顺序相反。

###### 返回值：

> 成功：0      失败：非0 

###### 性质

> atexit函数在进程结束时才会去执行参数对应的回调函数 
>
> atexit多次调用后，执行顺序与调用顺序相反   LIFO

###### 示例

```c
void clear_fun1(void) 
{ printf("perform clear fun1 \n"); }
void clear_fun2(void) 
{ printf("perform clear fun2 \n"); }
int main(int argc, char *argv[]) {
    atexit(clear_fun1); 
    atexit(clear_fun2);  
    // 执行顺序为clear_fun2 clear_fun1
    printf("process exit 3 sec later!!!\n"); 
    sleep(3); 
    return 0; 
}

```

### 避免僵尸进程

1. 父进程使用wait()或者waitpid()之类的函数等待子进程退出
2. 父进程先产生一个子进程，然后子进程再产生一个孙子进程，子进程在孙子进程之前退出。
3. 使用信号函数sigaction为SIGCHLD设置wait处理函数。 (只有这个使用)

屏蔽子进程退出的信号,避免产生僵尸进程

`signal(SIGCHLD,SIG_IGN);`



## 线程

### 概念



#### 空间

线程间共享属于进程的数据段(全局变量)

```c
int value = 100;

void *func1(void *arg){
    for(int i=0;i<3;i++){
        printf("hello(%d)\n",value++);
        sleep(1);
    }
}
void *func2(void *arg){
    for(int i=0;i<3;i++){
        printf("world(%d)\n",value++);
        sleep(1);
    }
}

int main()
{
    pthread_t tid,tid2;
    pthread_create(&tid,NULL,func1,NULL);
    pthread_create(&tid2,NULL,func1,NULL);
    pthread_join(tid,NULL);
    pthread_join(tid2,NULL);
    
    printf("In main thread(%d)\n",value);
    return 0;
}

```

![image-20220303190319283](assets/image-20220303190319283.png)



#### 线程进程比较

每一个进程创建的时候系统会给其4G虚拟内存，3G用户空间是私有的，所以进程切换 时，用户空间也会切换，所以会增加系统开销，而一个进程中的多个线程共享一个进程的资源，所以线程切换时不用切换这些资源，效率会更高 

线程的调度机制跟进程是一样的，多个线程来回切换运行。



每个**进程**有一个**地址空间**和一个**控制线程**(main函数就是主控线程)

<img src="assets/image-20211222224445551.png" alt="image-20211222224445551" style="zoom:50%;" />

进程的上下文

- `text` 代码 (二进制)  只读 **文字常量区、代码区**
- `data` 全局和静态变量数据 **静态全局区**
- `stack 栈`  存放**局部变量，函数返回值**  FIFO
- `heap 堆`    程序运行时的**动态内存分配**
- `PCB 进程控制块` 包含进程信息(控制信息等)

data stack heap : 支撑进程运行的环境

<img src="assets/image-20220221115950820.png" alt="image-20220221115950820" style="zoom:25%;" />

调度:         **线程**是**CPU调度和分派**的基本单位。

拥有资源: **进程**是系统中**程序执行和资源分配**的基本单位。

> 线程自己一般不拥有资源（除了必不可少的程序计数器，一组寄存器和栈），但它可以去访问其所属进程的资源，如进程代码段，数据段以及系统资源（已打开的文件，I/O设备等）

系统开销: **同一个进程**中的多个**线程**可**共享同一地址空间**，因此它们之间的同步和通信的实现也变得比较容易

> 在进程切换时候，涉及到整个当前进程CPU环境的保存以及新被调度运行的进程的CPU环境的设置；
>
> 线程切换只需要保存和设置少量寄存器的内容，并不涉及存储器管理方面的操作，从而能更有效地使用系统资源和提高系统的吞吐量。 

并发:        不仅**进程**间可以**并发执行**，而且在一个进程中的**多个线程之间**也可以**并发执行**。 

1. 解释一：并行是指两个或者多个事件在同一时刻发生；而并发是指两个或多个事件在同一时间间隔发生。
2. 解释二：并行是在不同实体上的多个事件，并发是在同一实体上的多个事件。
3. 解释三：并行是在多台处理器上同时处理多个任务。如 hadoop 分布式集群，并发是在一台处理器上“同时”处理多个任务。

#### 多线程的用处 

**多任务程序的设计** 

一个程序可能要处理不同应用，要处理多种任务，如果开发不同的进程来处理，系统开销很大，数据共享，程序结构都不方便，这时可使用多线程编程方法。 

**并发程序设计** 

一个任务可能分成不同的步骤去完成，这些不同的步骤之间可能是松散耦合，可能通过线程的互斥，同步并发完成。这样可以为不同的任务步骤建立线程。 

**网络程序设计** 

为提高网络的利用效率，我们可能使用多线程，对==每个连接用一个线程去处理==。 

**数据共享**

==同一个进程中的不同线程共享进程的数据空间==，方便不同线程间的数据共享。 

**在多CPU系统中，实现真正的并行**



### 线程属性

##### 查询

```sh
# 观察多线程tid以及数量
ps -el
ps -elf | more
ps -elf | grep thread

# 观测指定pid进程执行策略,优先级.
top -p 5004
chrt -p 35605

# 强制切换pid对应进程 Real-time Scheduling 进程,并且设置priority value
chrt -f -p priority_value pid
chrt -f -p 11 35605
```



```sh
# 线程内获取线程id
pthread_self();

# 线程竞争CPU的区间 scope
PTHREAD_SCOPE_SYSTEM   # 系统 SCS LINUX默认
PTHREAD_SCOPE_PROCESS  # 进程 PCS

# Inherit scheduler 调度器
PTHREAD_INERIT_SCHED
```



##### Scheduling 

```sh
# Scheduling policy调度策略 优先级
# # PR越低优先级越高

# Real-time Scheduling
# # PR∈[-100,-2] 
PR=-1-priority value
PR < 0 Real-time Scheduling

# Normal Scheduling
PR = -1
# priority value=0是个保留值
priority value=0

# # PR越低优先级越高PR∈[0,39] 
PR>=0 Normal Scheduling 
PR＝20+NI
```



###### Normal Scheduling 

> `SCHED_OTHER`时间片轮转RR  `SCHED_IDLE`  `SCHED_BARCH`   
>
> Priority value = 0 使用友好值NICE判断优先级 NI∈[-20,19]
>
> `PR＝20+NI`PR越高,优先级越低PR∈[0,39] 
>
> PR的取值也可以是rt(对应Real-time Scheduling,Real-time Scheduling中不重视PR值)



###### Real-time Scheduling

> `SCHED_FIFO`  `SCHED_RR` 
>
> Priority value ∈ [1,99]
>
> 1最低优先级 99 最高优先级
>
> Real-time Scheduling中不重视PR值) NI为0
>
> PR的取值为负`PR=-1-priority value` ∈[-100,-2] PR越低优先级越高



Normal Scheduling 优先级永远低于Real-time Scheduling





### 线程  POSIX

线程是CPU调度的基本单位。包含**线程id，程序计数器PC，寄存器，栈**(私有)

同一进程下的线程之间**分享code. data. file.** 等其他操作系统资源 

重量级进程有一个单线程。一个进程有多线程，它允许在同一时刻执行多个任务。

#### 线程库POSIX

Thread Library为程序员提供创建和管理线程的API 

- `POSIX Pthreads`：**linux下用户线程库和内核线程库** 
- `Windows Threads`：内核线程库 
- `Java Threads`：依据所依赖的操作系统而定

`Pthreads`是`POSIX`标准定义的线程创建与同步API。不同的操作系统对该标准的实现不尽相同。 

线程函数的程序在pthread库中，故链接时要加上参数-lpthread。 

> 由于线程库原本不是系统本身的，所以在链接时需要手动链接库文件 `gcc *.c ‐lpthread`





#### pthread_create

```c
pthread_t thread1;
struct Pams {int pam1, int pam2};
Pams pam = {1, 2}
void *pthread_fun1(void *arg) 、
{
    Pams Pam = *(Pams *)arg;
    printf("pam1 = %d\n", Pam->pam1);
}

int res =  pthread_create(&thread1, NULL, pthread_fun1, (void *)&pam);

if(res != 0) {
    perror("fail to pthread_create"); 
}
```

功能

- 创建一个新的子线程 

- pthread_create创建的线程不与父线程在同一点开始运行，

  从指定的函数开始运行，该函数运行完后，该线程也就退出了。 

- 线程依赖进程存在，进程结束后，进程中所有的线程都会强制退出。 

参数

```c
int pthread_create(
    pthread_t *thread,  // 返回给thread 当前创建的线程id , 线程标识符地址。
    const pthread_attr_t *attr, // 设置为NULL表示以默认的属性创建 
    void *(*start_routine)(void *), // 线程处理函数的入口地址，如果当前函数执行完毕，则子线程也执行完毕 
    void *arg  // 给线程处理函数start_routine传参的参数。 
);
```

- thread：  返回给thread 当前创建的线程id , 线程标识符地址。 

- attr：		线程属性结构体地址，设置为NULL表示以默认的属性创建  

- start_routine：线程处理函数的入口地址，如果当前函数执行完毕，则子线程也执行完毕 

- arg：给线程处理函数start_routine传参的参数。 

返回值

- 成功：返回 0 

- 失败：返回非 0 





#### pthread_once

在多线程环境中，有些事仅需要执行一次。

当你写一个库时，就不能像初始化应用程序那样在main函数中初始化了。

- 可以用静态初始化，
- 但使用`pthread_once`  一次初始化会比较容易些。

尽管pthread_once()调用可能会出现在多个线程中，

- init_routine()函数仅执行一次，
- 究竟在哪个线程中执行是不定的，是由内核调度来决定。

```c
int pthread_once(pthread_once_t *once_control, void (*init_routine) (void))；
```

功能：

- 使用once_control变量保证init_routine()函数在本进程执行序列中仅执行一次。

- Linux Threads使用互斥锁和条件变量保证由pthread_once()指定的函数执行且仅执行一次

- once_control变量用于表示是否执行过。

  - 初值

    - 初值为PTHREAD_ONCE_INIT（Linux Threads定义为0）
    - 如果不是PTHREAD_ONCE_INIT，`pthread_once()` 的行为会不正常。

  - 实际"一次性函数"的执行状态有三种：

    - NEVER(0)

    - IN_PROGRESS(1) 

      所有pthread_once()都必须等待其中一个激发"已执行一次"信号，因此所有pthread_once ()都会陷入永久的等待中

    - DONE(2)

      表示该函数已执行过一次，从而所有pthread_once()都会立即返回0。



#### join detach

linux线程执行和windows不同，pthread有两种状态： 

- 可结合态（joinable）

  - 线程默认创建为可结合态**joinable**

  - 需要通过`pthrad_join`函数回收子线程退出的资源

    当 线程函数自己返回退出 或 `pthread_exit` 时**都不会释放**线程所占用堆栈和线程描述符（总计8K多）。只有当调用了`pthread_join`之后这些资源才会被释放。 

- 分离态（detached）

  - 使用`pthread_detach`函数将线程设置为分离态。 

    - 既不用阻塞，

    - 也可以自动回收子线程退出的资源. 

      资源在 线程函数退出 或`pthread_exit`时**自动会被释放**

  - 如果子线程已经设置为分离态，就不需要再使用`pthread_join`了 

##### pthread_join

`#include <pthread.h> `

```cpp
int pthread_join(pthread_t thread, void **retval);
```

```c
void *pthread_fun1(void *arg) 
{
    int ret = *(int *)arg;
  	return (void *)&ret;
}
pthread_create(&thread1, NULL, pthread_fun1, (void *)&imPams); 
int mRet = pthread_join(thread1, (void **)&ret);
if(mRet != 0)
{ 
    perror("fail to pthread_join"); 
    exit(1);
}
printf("ret_val = %d\n", *ret);
```

功能

- 线程**默认创建**为**可结合态joinable**，需要通过`pthrad_join`函数回收子线程退出的资源。
- 阻塞等待一个子线程的退出 
  - 等待子线程结束，类似wait函数 等待子进程结束
  - 并回收子线程资源。
- 可以接收到某一个子线程调用`pthread_exit`时设置的退出状态值 
- 如果子线程已经设置为分离态，就不需要再使用`pthread_join`了 
- 子线程如果要返回退出状态，可以通过返回值或者通过pthread_exit函数 

参数

- thread：指定线程的id, 被阻塞等待的线程号。 
- retval：
  - 保存子线程的退出状态值，用来存储线程退出状态的指针的地址。 
  - 如果不接受则设置为NULL 

返回值

- 成功返回 0，失败返回非 0。



##### pthread_detach

```cpp
int pthread_detach(pthread_t thread);
```



```c
pthread_t thread;
pthread_create(&thread, NULL, thread_fun, NULL);
if(pthread_detach(thread) != 0) 
{
    perror("fail to pthread_detach"); 
    exit(1); 
}
```

功能

- 使调用线程与当前进程分离，使其成为一个独立的线程，(并行) 
- 使用`pthread_detach`函数将线程设置为分离态。 
  - 既不用阻塞，
  - 该子线程终止时，系统将自动回收子线程的资源。

参数

- thread：指定的子线程的id 线程号 

返回值

- 成功：返回 0，失败返回非 0。 



#### 退出/取消

##### pthread_exit



```cpp
void pthread_exit(void *retval);
```

```c
void *pthread_fun1(void *arg) 、
{
    static char buf[] = "This thread has quited";
    pthread_exit(buf);
}
pthread_t thread1;
pthread_create(&thread1, NULL, pthread_fun1, NULL);
char *str;
pthread_join(thread1, (void **)&str); 
printf("str = %s\n", str);
```

功能：

- 退出正在执行的线程  退出调用线程。 
- **线程**中 可在**不终止整个进程**的情况下停止它的控制流 
  - 线程调用 pthread_exit 退出线程，从执行函数中返回。 

  - 线程可以被同一进程中的其它线程取消。 
- 一个进程中的多个线程是共享该进程的数据段，因此，通常线程退出后所占用的资源并不会释放。
- 需要调用 pthread_join释放资源，获得返回值

参数:

- retval：当前线程的退出状态值 存储线程退出状态的指针。
- 这个值可以被调用`pthread_join`函数的线程接收到

返回值

- 无



##### pthread_cancel

```c
int pthread_cancel(pthread_t thread);
```

```c
pthread_t thread; 
pthread_create(&thread, NULL, pthread_fun, NULL);

//通过调用pthread_cancel函数取消另一个线程 
pthread_cancel(thread); 
pthread_join(thread, NULL);
```

功能

- 取消一个正在执行线程的操作。 
- `pthread_cancel`函数的实质是发信号给目标线程thread，使目标线程退出。 
- 发出信号就返回
  - 只是发送终止信号给目标线程，**不会等待**取消目标线程**执行完才返回**。 。
  - 发送成功并不意味着目标线程一定就会终止，线程被取消时，
- **线程的取消属性**会决定线程**能否被取消**以及**何时被取消**。

参数

- thread: 要销毁的线程的id  目标线程 ID。 

返回值

- 成功返回 0，失败返回非0 出错编号。

线程的取消属性

- **线程的取消状态** 即线程能不能被取消 

  - 在 Linux 系统下，线程默认可以被取消。编程时可以在线程函数中通过`pthread_setcancelstate` 函数设置线程是否可以被取消。 

  - ```c
    pthread_setcancelstate(int state,int *old_state); 
    ```

  - state：

    - PTHREAD_CANCEL_DISABLE：不可以被取消 
    - PTHREAD_CANCEL_ENABLE：可以被取消。 

  - old_state： 

    - 保存调用线程原来的可取消状态的内存地址

- **线程取消点**         即线程被取消的地方

  - 编程时可以通过 `pthread_testcancel `函数设置线程的取消点。 

  - ```c
    void pthread_testcancel(void);
    ```

- **线程的取消类型** 

  - 在线程能被取消的状态下，是立马被取消结束还是执行到取消点的时候被取消结束 

  - ```c
    pthread_setcanceltype(int type, int *oldtype); 
    ```

  - type：

    - PTHREAD_CANCEL_ASYNCHRONOUS：立即取消
    - PTHREAD_CANCEL_DEFERRED：不立即被取消 

  - oldtype：

    - 保存调用线程原来的可取消类型的内存地址。



#### 退出清理 

##### pthread_cleanup_push

对清理函数进行注册

```c
void pthread_cleanup_push(void (* routine)(void *), void *arg);
```

功能:

- 线程可以注册它退出时要调用的函数，这样的函数称为线程清理处理程序(thread cleanup handler)。 

  - 线程可以建立**多个清理处理程序**。 
  - **处理程序在栈**中，故它们的执行顺序与它们注册时的顺序相反。 

- 注册清理函数：将清除函数压栈。满足以下情况时，依次执行。

- 当线程执行以下动作时会调用清理函数： 

  - 调用`pthread_exit`退出线程。 

  - 响应其它线程的取消请求。 

  - 用非零execute调用pthread_cleanup_pop

    无论哪种情况` pthread_cleanup_pop `都将删除上一次 `pthread_cleanup_push `调用注册的清理处理函数。

- 注意push与pop必须配对使用，即使pop执行不到。

参数:

- routine：线程清理函数的指针。 

- arg：传给线程清理函数的参数。 



##### pthread_cleanup_pop

```c
void pthread_cleanup_pop(int execute);
```

功能：

- 调用并弹出上一次注册的`pthread_cleanup_push`清理函数

  - 系统自动调用线程清理函数。

  - 不会退出线程。

- 将清除函数弹栈，即删除执行过的清理函数。 

参数：

- execute:线程清理函数执行标志位。 
  - 非 0，弹出清理函数，执行清理函数。 
  - 0，弹出清理函数，不执行清理函数。 





#### 互斥锁

##### 概念

互斥锁 mutex是一种简单的加锁的方法来控制对共享资源的访问，mutex只有两种状态,即 

> 上锁(lock)
>
> 解锁(unlock)。 

在访问该资源前，首先应申请mutex，

- 如果mutex处于**lock状态**，则默认阻塞申请者。 

- 如果mutex处于**unlock状态**，则会申请到mutex并立即lock；

  unlock操作应该由lock者进行 



##### pthread_mutex_init

- **静态分配**

```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
```

- **动态分配**

```c
int pthread_mutex_init(pthread_mutex_t *mutex,const pthread_mutexattr_t *attr);

pthread_mutex_t mutex;

pthread_mutex_init(&mutex, NULL);
```

功能

- 初始化一个互斥锁。

参数：

- mutex：指定的互斥锁 互斥锁地址。 
- attr：互斥锁的属性，NULL 为默认的属性。 

返回值：

- 成功返回 0，失败返回非 0。



##### pthread_mutex_lock

```c
#include <pthread.h> 

int pthread_mutex_lock(pthread_mutex_t *mutex); 
```

功能：

- 对互斥锁上锁，
- 若已经上锁，则调用者一直阻塞到互斥锁解锁

参数：

- mutex：指定的互斥锁 互斥锁地址。 

返回值：

- 成功返回 0，失败返回非 0。 



##### pthread_mutex_trylock

```c
#include <pthread.h> 

int pthread_mutex_trylock(pthread_mutex_t *mutex);
```

**功能：**

- 对互斥锁上锁，
- 若已经上锁，则上锁失败，函数立即返回。 

**参数：**

- mutex：指定的互斥锁 互斥锁地址。 

**返回值：**

- 成功返回 0，失败返回非 0。



##### `pthread_mutex_unlock`

```c
#include <pthread.h>

int pthread_mutex_unlock(pthread_mutex_t * mutex);
```

功能：

- 对指定的互斥锁解锁。 

参数：

- mutex：互斥锁地址。 

返回值：

- 成功返回 0,失败返回非 0。



##### `pthread_mutex_destroy`

在所有使用过此互斥锁的线程都不再需要使用时候，应调用`pthread_mutex_destroy`销毁互斥锁

```c
pthread_mutex_t mymutex;    
pthread_mutex_init(&mymutex, NULL);

// 当互斥锁使用完毕后，要销毁
pthread_mutex_destroy(&mymutex);
```





##### 互斥锁案例

两人公用同一银行账户。

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//通过互斥锁解决线程间互斥问题

int money = 10000;

//第一步：创建互斥锁(由于两个线程操作同一个互斥锁，所以定义在全局更加方便一点)
pthread_mutex_t mymutex;

void *pthread_fun1(void *arg)
{
    int get, rest, actual;
    get = 10000;
    
    //第三步：对共享资源的操作进行上锁
    pthread_mutex_lock(&mymutex);

    printf("张三正在查询余额...\n");
    sleep(1);
    rest = money;

    printf("张三正在取钱...\n");
    sleep(1);
    if(get > rest)
    {
        actual = 0;
    }
    else 
    {
        actual = get;
        rest = rest - get;
        money = rest;
    }

    printf("张三想取%d元，实际取了%d元，余额为%d元\n", get, actual, rest);

    //第四步：当共享资源的操作执行完毕后，对互斥锁执行解锁操作
    pthread_mutex_unlock(&mymutex);

    pthread_exit(NULL);
}

void *pthread_fun2(void *arg)
{
    int get, rest, actual;
    get = 10000;

    //第三步：对共享资源的操作进行上锁
    pthread_mutex_lock(&mymutex);
    
    printf("李四正在查询余额...\n");
    sleep(1);
    rest = money;

    printf("李四正在取钱...\n");
    sleep(1);
    if(get > yu)
    {
        actual = 0;
    }
    else 
    {
        actual = get;
        rest = rest - get;
        money = rest;
    }

    printf("李四想取%d元，实际取了%d元，余额为%d元\n", get, actual, rest);

    //第四步：当共享资源的操作执行完毕后，对互斥锁执行解锁操作
    pthread_mutex_unlock(&mymutex);

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    //第二步：初始化互斥锁
    pthread_mutex_init(&mymutex, NULL);

    pthread_t thread1, thread2;

    if(pthread_create(&thread1, NULL, pthread_fun1, NULL) != 0)
    {
        perror("fail to pthread_create");
        exit(1);
    }

    if(pthread_create(&thread2, NULL, pthread_fun2, NULL) != 0)
    {
        perror("fail to pthread_create");
        exit(1);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    //第五步：当互斥锁使用完毕后，要销毁
    pthread_mutex_destroy(&mymutex);
    
    return 0;
}
```





## 同步与互斥

### 概述

在多任务操作系统中，同时运行的多个任务可能都需要访问/使用同一种资源 

多个任务之间有依赖关系，某个任务的运行依赖于另一个任务。

- 互斥
  - ==一个公共资源同一时刻只能被一个进程或线程使用==，
  - 多个进程或线程**不能同时使用公共资源**。
  - 

- 同步

  - 两个或两个以上的进程或线程在运行过程中==协同步调==，**按预定的先后次序运行**。 

  - 同步就是**在互斥的基础上有顺序**的执行进程或线程。



POSIX标准中进程和线程同步和互斥的方法,主要有两种方式。 

- **信号量**
- **互斥锁**

互斥锁 见 `线程 POSIX` - `互斥锁 pthread_mutex`



### 信号量

详见 `进程间通信` - `POSIX进程见通信` - `信号量`







# Android Framework中的Thread

## java general

我们来看看Framework中常用的Thread是个何种形态。

挑选一个Thread子类进行研究。

- 在源码中搜索 `threadLoop`
- 或者 搜索 `thread`   `: public Thread`



### 继承Thread

这里挑了 Thread的子类 —— `AudioWatchDog`

```sh
frameworks/av/services/audioflinger/AudioWatchdog.h
```

```cpp
#ifndef AUDIO_WATCHDOG_H
#define AUDIO_WATCHDOG_H
#include <time.h>
#include <utils/Thread.h>

namespace android {

......

class AudioWatchdog : public Thread {

public:
    AudioWatchdog(unsigned periodMs = 50) : Thread(false/*canCallJava*/), mPaused(false),
            mPeriodNs(periodMs * 1000000), mMaxCycleNs(mPeriodNs * 2),
            // mOldTs// mLogTs initialized below
            mOldTsValid(false), mUnderruns(0), mLogs(0), mDump(&mDummyDump)
        {
            #define MIN_TIME_BETWEEN_LOGS_SEC 60// force an immediate log on first underrun
            mLogTs.tv_sec = MIN_TIME_BETWEEN_LOGS_SEC;
            mLogTs.tv_nsec = 0;
        }
    virtual         ~AudioWatchdog() { }
    // Do not call Thread::requestExitAndWait() without first calling requestExit().
    // Thread::requestExitAndWait() is not virtual, and the implementation doesn't do enough.virtualvoid        
    requestExit();

    // FIXME merge API and implementation with AudioTrackThreadvoid            
    pause();        // suspend thread from execution at next loop boundaryvoid            
    resume();       // allow thread to execute, if not requested to exit
    // Where to store the dump, or NULL to not updatevoid            
    setDump(AudioWatchdogDump* dump);

private:
    virtual bool threadLoop();

    Mutex           mMyLock;        // Thread::mLock is private
    Condition       mMyCond;        // Thread::mThreadExitedCondition is privatebool            mPaused;        // whether thread is currently paused

    ......
};

}   // namespace android#endif  // AUDIO_WATCHDOG_H
```





##### Thread::run

```sh
Thread被创建
	Thread::run  
		Thread::__threadLoop
			Thread::readyToRun
			Thread::threadLoop 循环调用. 返回false时退出循环
```

Thread被创建，Thread中的run被调用，__threadLoop()被调用，readyToRun()被调用，然后循环调用threadLoop()。并且 在threadLoop()返回false时，可以退出循环。



###### androidCreateThreadEtc

```sh
frameworks/core/core/libutils/Threads.cpp
```

```cpp
status_t Thread::run(const char* name, int32_t priority, size_t stack)
{
    LOG_ALWAYS_FATAL_IF(name == nullptr, "thread name not provided to Thread::run");

    Mutex::Autolock _l(mLock);
```

run()方法中有这么一段

```cpp
    if (mCanCallJava) {
        res = createThreadEtc(_threadLoop,
                this, name, priority, stack, &mThread);
    } else {
        res = androidCreateRawThreadEtc(_threadLoop,
                this, name, priority, stack, &mThread);
    }
```

mCanCallJava的意思是能不能被JNI层调用，

然后根据值去创建Thread，这里有两个分支，最终代码都会走到`androidCreateRawThreadEtc`



```sh
frameworks/core/core/libutils/include/utils/AndroidThreads.h
```

```cpp
// Create thread with lots of parameters
inline bool createThreadEtc(thread_func_t entryFunction,
                            void *userData,
                            const char* threadName = "android:unnamed_thread",
                            int32_t threadPriority = PRIORITY_DEFAULT,
                            size_t threadStackSize = 0,
                            thread_id_t *threadId = nullptr)
{
    return androidCreateThreadEtc(entryFunction, userData, threadName,
        threadPriority, threadStackSize, threadId) ? true : false;
}
```



###### pthread_create

最终代码都会走到`androidCreateRawThreadEtc`

> linux下c语言pthread创建线程的例子，也就 是pthread_create()。
>
> 里面传进来的entryFunction是Thread中的_threadLoop()

```sh
frameworks/core/core/libutils/Threads.cpp
```

```cpp
// Low-level thread creation -- never creates threads that can
// interact with the Java VM.
int androidCreateRawThreadEtc(android_thread_func_t entryFunction,
                              void *userData,
                              const char* threadName,
                              int32_t threadPriority,
                              size_t threadStackSize,
                              android_thread_id_t *threadId)
{

    pthread_t thread;
    int result = pthread_create(&thread, &attr,
                                (android_pthread_entry)entryFunction, userData);
```





##### Thread::_threadLoop

Thread::_threadLoop 从而执行 threadLoop方法

```sh
frameworks/core/core/libutils/Threads.cpp
```

```cpp
int Thread::_threadLoop(void* user)
{
    Thread* const self= static_cast<Thread*>(user);

    sp<Thread> strong(self->mHoldSelf);
    wp<Thread> weak(strong);
    self->mHoldSelf.clear();
	// ...
    bool first =true;

    do {
        bool result;
        if (first) {
            first = false;
            self->mStatus = self->readyToRun();
			// ...
                result = self->threadLoop();
            }
        } else {
            result = self->threadLoop();
        }
        // establish a scope for mLock
        {
        Mutex::Autolock _l(self->mLock);
        if (result == false || self->mExitPending) {
            self->mExitPending = true;
            self->mRunning = false;
            self->mThread = thread_id_t(-1);
            self->mThreadExitedCondition.broadcast();
            break;
        }
        }

        strong.clear();
        strong = weak.promote();
    } while(strong !=0);

    return0;
}
```

_threadLoop()这个方法就是Thread的最大秘密，它是一个while循环。

1、创建线程时，会sp和wp一次线程本身。

2、**如果是第一次执行**

- **运行线程的readyToRun()方法，再执行threadLoop()，**
- **否则，直接运行threadLoop()。**

3、**threadLoop()方法有返回值，**

- **如果threadLoop()返回false的时候，线程会做清理工作，然后退出while循环，结束运行。**

- 解答了：为什么线程Thread中的threadLoop()能够循环处理数据。



##### Thread::threadLoop() 

重写该函数，返回false时，结束循环。

###### 是虚函数

```sh
frameworks/core/core/libutils/include/utils/Thread.h
```

```cpp
namespace android {

// DO NOT USE: please use std::thread

class Thread : virtual public RefBase
{
public:
    // Create a Thread object, but doesn't create or start the associated
    // thread. See the run() method.
    explicit            Thread(bool canCallJava = true);
    virtual             ~Thread();

    // Start the thread in threadLoop() which needs to be implemented.
    // NOLINTNEXTLINE(google-default-arguments)
    virtual status_t    run(    const char* name,
                                int32_t priority = PRIORITY_DEFAULT,
                                size_t stack = 0);
    // ...
private:
    // Derived class must implement threadLoop(). The thread starts its life
    // here. There are two ways of using the Thread object:
    // 1) loop: if threadLoop() returns true, it will be called again if
    //          requestExit() wasn't called.
    // 2) once: if threadLoop() returns false, the thread will exit upon return.
    virtual bool        threadLoop() = 0;

	// ...
};

}  // namespace android

```



###### 重写 threadLoop

核心方法就是threadLoop()，一般需要重写。如果不返回false，会一直执行。

```cpp
bool AudioWatchdog::threadLoop()
{
    // ...
    rc = nanosleep(&req, NULL);
    if (!((rc == 0) || (rc == -1 && errno == EINTR))) {
        pause();
        return false;
    }
    return true;
}
```

但是threadLoop()是怎么启动的呢？又是怎么循环运行的呢？—— 去找run 方法  （后续就是之前介绍的_threadLoop方法了）

- 在AudioFlinger.h中MixerThread

  - MixerThread持有一个AudioWatchdog的sp对象

  - AudioWatchdog对象创建了，并且调用了它的run方法

- run方法的实现，在父类Thread.



### 调用 run()方法

#### 显式调用

...



#### init进程 创建

特殊情况

有的时候Android Framework中Thread的run()方法很难发现在哪里被调用。



##### e.g. Sf

例如SurfaceFlinger它也是一个Thread子类，但却是由 init 进程 根据`.rc`文件创建。

Thread的子类

```cpp
class SurfaceFlinger : public BinderService<SurfaceFlinger>,
                       public BnSurfaceComposer,
                       private IBinder::DeathRecipient,
                       private Thread,
                       private HWComposer::EventHandler
{
```

##### instantiate()方法

统一在`main_xxxserver.cpp中的 main()函数中`调用了`xxx::instantiate();`函数

例如 `SurfaceFlinger::instantiate();`

但sf本身并没有实现instantiate()方法，那找它的父类 BinderService。 SERVICE在这里是一个模板类。在这里SERVICE自然对应SurfaceFlinger。`instantiate()方法`会调用`publish()方法`。

```sh
BinderService.instantiate
	BinderService.publish
```

publish()会向ServiceManager添加一个Service，这个Service就是Surfaceflinger。

```sh
frameworks/native/libs/binder/include/binder/BinderService.h
```

```cpp
template<typename SERVICE>
class BinderService
{
public:
    static void instantiate() { publish(); }
    
    static status_t publish(bool allowIsolated = false,
                            int dumpFlags = IServiceManager::DUMP_FLAG_PRIORITY_DEFAULT) {
        sp<IServiceManager> sm(defaultServiceManager());
        return sm->addService(String16(SERVICE::getServiceName()), new SERVICE(), allowIsolated,
                              dumpFlags);
    }


```

然后我们看SurfaceFlinger的构造函数。



#### onFirstRef 调用 run()

**调用sp时会调用SurfaceFlinger的onFirstRef()方法。**

```sh
SurfaceFlinger::onFirstRef
	SurfaceFlinger::run
```

```cpp
void SurfaceFlinger::onFirstRef()
{
    mEventQueue.init(this);

    run("SurfaceFlinger", PRIORITY_URGENT_DISPLAY);

    // Wait for the main thread to be done with its initialization
    mReadyToRunBarrier.wait();
}

void SurfaceFlinger::onFirstRef() {
    mEventQueue->init(this);
}

SurfaceFlinger::SurfaceFlinger(Factory& factory, SkipInitializationTag)
      : mFactory(factory),
        mEventQueue(mFactory.createMessageQueue()),

std::unique_ptr<MessageQueue> DefaultFactory::createMessageQueue() {
    return std::make_unique<android::impl::MessageQueue>();
}

```

run()方法在这里调用了

所以，在Framework中如果你找不到一个Thread在何处被启动，那么去它的onFirstRef()方法中去看看吧





```sh
frameworks/base/cmds/system_server/library/system_init.cpp
```

```cpp
extern"C" status_t system_init()
{
    ALOGI("Entered system_init()");

    sp<ProcessState> proc(ProcessState::self());

    sp<IServiceManager> sm = defaultServiceManager();
    ALOGI("ServiceManager: %p\n", sm.get());



    char propBuf[PROPERTY_VALUE_MAX];
    property_get("system_init.startsurfaceflinger", propBuf, "1");
    if (strcmp(propBuf, "1") == 0) {
        // Start the SurfaceFlinger
        SurfaceFlinger::instantiate();
    }


    // And now start the Android runtime.  We have to do this bit// of nastiness because the Android runtime initialization requires// some of the core system services to already be started.// All other servers should just start the Android runtime at// the beginning of their processes's main(), before calling// the init function.
    ALOGI("System server: starting Android runtime.\n");
    AndroidRuntime* runtime = AndroidRuntime::getRuntime();

    ALOGI("System server: starting Android services.\n");
    JNIEnv* env = runtime->getJNIEnv();
    if (env == NULL) {
        return UNKNOWN_ERROR;
    }
    jclass clazz = env->FindClass("com/android/server/SystemServer");
    if (clazz == NULL) {
        return UNKNOWN_ERROR;
    }
    jmethodID methodId = env->GetStaticMethodID(clazz, "init2", "()V");
    if (methodId == NULL) {
        return UNKNOWN_ERROR;
    }
    env->CallStaticVoidMethod(clazz, methodId);

    ALOGI("System server: entering thread pool.\n");
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();
    ALOGI("System server: exiting thread pool.\n");

    return NO_ERROR;
}
```







### DispSyncThread

再举个例子，

```sh
frameworks/native/services/surfaceflinger/Scheduler/DispSync.cpp
```

```cpp
class DispSyncThread : public Thread {
```





## SurfaceFlinger









# cpp 时间

[C++11 多线程（std::thread）详解-CSDN博客](https://blog.csdn.net/sjc_0910/article/details/118861539)

在C中已经有一个叫做pthread的东西来进行多线程编程，但是并不好用 （如果你认为句柄、回调式编程很实用，那请当我没说），所以c++11标准库中出现了一个叫作std::thread的东西。

## 时间chrono&sleep

### std::chrono

#### 优点

C++11 引入的时间标准库：std::chrono

利用 C++ 强类型的特点，

- 明确区分  **时间点**与**时间段**
  - 时间点：
    - 例如：2022年1月8日 13点07分10秒
  - 时间段：1分30秒
- 明确区分  **不同的时间单位** 类型
  - 时间点：`chrono::steady_clock::time_point` 等
  - 时间段：`chrono::milliseconds`，`chrono::seconds`，`chrono::minutes` 等
- 方便的运算符重载：
  - 时间点+时间段=时间点，
  - 时间点-时间点=时间段



#### 时间点/段

##### 时间点获取

```cpp
// 获取当前时间点
auto t0 = chrono::steady_clock::now();
```



##### 计算

```c
// 计算时间点
auto t1 = chrono::steady_clock::now(); + chrono::seconds(30);// 当前时间点的30秒后


// 计算时间段
auto dt = t1 - t0; // 获取两个时间点的差（时间段）

auto t0 = chrono::steady_clock::now()
for (volatile int i = 0; i < 10000; i++);
auto t1 = chrono::steady_clock::now()
auto dt = t1 - t0; // 获取两个时间点的差（时间段）
```





##### 时间段转换

duration_cast 可以在任意的 duration 类型之间转换

> `chrono::duration_cast<chrono::duration<T, R>>`

```c
// 类型转换 时间段
// - 时间段的秒数, 输出类型为int
int64_t sec = chrono::duration_cast<chrono::seconds>(dt).count(); 
// - 时间段的秒数, 输出类型为double
using double_ms = chrono::duration<double, std::milli>
double sec = chrono::duration_cast<double_ms>(dt).count(); 

// // 时间段的毫秒数,输出类型为int
int64_t msec = chrono::duration_cast<chrono::milliseconds>(dt).count();  
```

`duration<T, R>` 表示用 T 类型表示，且时间单位是 R

参数

- T  输出的类型
  - `int64_t`， `double`， `long` ...
- R  时间单位
  - 省略不写就是秒，
  - `std::milli`  毫秒，`std::micro` 微秒

`duration<T, R>`的常用封装

- `chrono::seconds` 是 `chrono::duration<int64_t>` 的类型别名

- `chrono::milliseconds` 是 `chrono::duration<int64_t, std::milli>` 的类型别名



### c time.h

C 语言如何处理时间

```c
#include <time.h>
```

C 语言原始的 API，没有类型区分，导致很容易弄错单位，混淆时间点和时间段。

> 比如 t0 * 3，乘法对时间点而言根本是个无意义的计算，然而 C 语言把他们看做一样的 long 类型，从而容易让程序员犯错。

```c
// 获取从1970年1月1日到当前时经过的秒数
long t0 = time(NULL);
// 让程序休眠
sleep(3);        // 3秒
usleep(3000000); // 3000000微秒，也就是3秒
// 计算时间点
long t1 = time(NULL) + 3; // 当前时间的三秒后

```



### std::this_thread::sleep

#### 优点

C++11提供跨平台的 sleep：

- 跨平台：
  - 可用它替代 Unix 类操作系统专有的的 `usleep`。
- 使用`std::chrono`
  - chrono 的强类型让时间单位选择更自由。
  - milliseconds 表示毫秒， microseconds 表示微秒，seconds 表示秒，



#### sleep_for

`std::this_thread::sleep_for`

- 参数：接受一个时间段

- 功能：让当前线程休眠一段时间，然后继续。

```c
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // sleep_for
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    return 0;
}
```





#### sleep_until

`std::this_thread::sleep_until`

- 参数：接受一个时间点

- 功能：表示让当前线程休眠直到某个时间点。

```cpp
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // sleep_until
    auto t = std::chrono::steady_clock::now() + std::chrono::milliseconds(400);
    std::this_thread::sleep_until(t);
    return 0;
}

```









# cpp 可调用对象

C++中有多种可调用对象,他们可以作为参数传给`std::bind()`,`std::thread()`, `std::async()`,`std::call_once()`等。



## 编译选项 cmake/make

std::thread 的实现背后是基于 pthread 的。

cmake

[CMake中使用pthread的坑与解决方案_cmake 链接pthread-CSDN博客](https://blog.csdn.net/jinking01/article/details/115217926)

```cmake
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(cpptest LANGUAGES CXX)

add_executable(cpptest main.cpp)
# 重点是这一步
target_link_libraries(cpptest PUBLIC pthread)

```

makefile

> -lpthread

```shell
excute=out
obj=test.cpp

$(excute): $(obj)
	gcc $(obj) -lpthread  -lstdc++ -o $(excute)

clean:
	rm $(excute)
```



## std::thread

### std::thread

#### 概述

C++11 开始，为多线程提供了语言级别的支持。用 std::thread 这个类来表示线程。

##### 执行与回收

何时执行与回收？

- 执行
  - 线程是在thread对象被定义的时候开始执行的。

  - 当那个线程启动时，就会执行这个 lambda 里的内容。

- 回收
  - join函数只是阻塞等待线程结束并回收资源
  - 线程不是在调用join函数时才执行的，

##### RAII

作为一个 C++ 类，std::thread 同样遵循 RAII 思想和三五法则。

因为管理着资源，所以

- 自定义了 解构函数
- 删除了拷贝构造/赋值函数，
- 提供了移动构造/赋值函数。

当 `std::thread对象` 所在的函数退出时，就会调用 `std::thread` 的解构函数，这会销毁线程。

可能导致 `std::thread`对象的函数还没开始执行，线程就被销毁了。

```c
void myfunc() {
    std::thread t1([&] {
        download("hello.zip");
    });
}

void main() {
    myfunc();
}
```





#### 构造函数

`std::thread` 构造函数的参数可以是任意 lambda 表达式。

```cpp
// 默认构造函数
// 创建一个线程，什么也不做
thread() noexcept;

// 初始化构造函数	
// 创建一个线程,以args为参数,执行fn函数(注意，开始执行)
template <class Fn, class… Args>
explicit thread(Fn&& fn, Args&&… args);

// 移动构造函数
// 构造一个与x相同的对象,会破坏x对象
thread(thread&& x) noexcept;
// 如果对象是joinable的，那么会调用std::terminate()结果程序
thread& operator=(thread &&rhs);
    
// 析构函数	析构对象
~thread();
```



#### 传参(右值引用)

划重点：`Args&&... args`

很明显的右值引用，那么我们该如何传递一个左值呢？

`std::ref`和`std::cref`很好地解决了这个问题。

- `std::ref` 可以包装`按引用传递`的值。
- `std::cref` 可以包装`按const引用传递`的值。

> ```cpp
> // Compiler: MSVC 19.29.30038.1
> #include <iostream>
> #include <thread>
> using namespace std;
> template<class T> void changevalue(T &x, T val) {
> 	x = val;
> }
> int main() {
> 	thread th[100];
> 	int nums[100];
> 	for (int i = 0; i < 100; i++)
> 		th[i] = thread(changevalue<int>, ref(nums[i]), i+1);
> 	for (int i = 0; i < 100; i++) {
> 		th[i].join();
> 		cout << nums[i] << endl;
> 	}
> 	return 0;
> }
> ```



#### 执行

##### 阻塞等待 join

**没有执行join或detach的线程在程序结束时会引发异常**

调用join函数(阻塞)

- 线程不是在调用join函数时才执行的
- 只是阻塞等待线程结束并回收资源

分离的线程（执行过detach的线程）

- 在调用它的线程结束或自己结束时释放资源。
  - 线程会在函数运行完毕后自动释放，
  - 不推荐利用其他方法强制结束线程，可能会因资源未释放而导致内存泄漏。



让主线程不要急着退出，等子线程也结束了再退出

```cpp
// 等待线程结束并清理资源（会阻塞）
void join();
// 返回线程是否可以执行join函数
bool joinable();


```

```c
std::thread t1([&] {
    download("hello.zip");
});

t1.join();
```







##### 分离 detach

detach

调用成员函数 detach() 分离该线程

- 意味着线程的生命周期不再由当前 std::thread 对象管理，
- 在线程退出以后自动销毁自己。

不过这样还是会在**进程退出时**候自动退出

```cpp
// 将线程与调用其的线程分离，彼此独立执行（此函数必须在线程创建时立即调用，且调用此函数会使其不能被join）
void detach();
```

```c
void myfunc() {
    std::thread t1([&] {
        download("hello.zip");
    });
    t1.detach();
}

void main() {
    myfunc();
}
```



##### 全局线程池

detach 的问题是**进程退出时，进程不会等待**所有子线程执行完毕。

另一种解法是把 t1 对象移动到一个全局变量去，再join它们，从而延长其生命周期到 myfunc 函数体外。

###### vector

从中取出thread，手动join

```c
std::vector<std::thread> pool;
void myfunc() {
    std::thread t1([&] {
        download("hello.zip");
    });
    pool.push_back(std::move(t1)); // 移交thread的控制权(所有权)给全局变量，延长生命周期
}

void main() {
    myfunc();
    for (auto &t: pool) t.join(); // 手动join
}
```



###### 线程池

在 main 里面手动 join 全部线程还是有点麻烦，

我们可以自定义一个类 ThreadPool，并用他创建一个全局变量，其解构函数会在 main 退出后自动调用。

```cpp
class ThreadPool {
    std::vector<std::thread> m_pool;

public:
    void push_back(std::thread thr) {
        m_pool.push_back(std::move(thr));
    }

    ~ThreadPool() {    // 即将离开作用域(进程要结束时),自动调用
        for (auto &t: m_pool) t.join(); // 等待线程池中的线程全部结束
    }
};

ThreadPool tpool;

void myfunc() {
    std::thread t1([&] {
        download("hello.zip");
    });
    tpool.push_back(std::move(t1));  // 移交thread的控制权(所有权)给全局变量，延长生命周期
}

int main() {
    myfunc();
    return 0;
}

```



#### this_thread

线程控制自己的方法，在`<thread>`头文件中，不仅有std::thread这个类，而且还有一个std::this_thread命名空间，它可以很方便地让线程对自己进行控制。

> `std::this_thread`是个命名空间，所以你可以使用`using namespace std::this_thread;`这样的语句来展开这个命名空间，不过我不建议这么做。



##### 常用方法

```cpp
// 获取当前线程id
std::thread::id get_id() noexcept	
    
// 等待sleep_duration（sleep_duration是一段时间）
template<class Rep, class Period>
void sleep_for( const std::chrono::duration<Rep, Period>& sleep_duration )	

// 暂时放弃线程的执行，将主动权交给其他线程（主动权还会回来）
void yield() noexcept	

```



##### get_id

```cpp
//获取线程id
std::thread::id get_id();
```



##### yield





##### demo

```cpp
#include <iostream>
#include <thread>
#include <atomic>
using namespace std;
atomic_bool ready = 0;
// uintmax_t ==> unsigned long long
void sleep(uintmax_t ms) {
	this_thread::sleep_for(chrono::milliseconds(ms));
}
void count() {
	while (!ready) this_thread::yield();
	for (int i = 0; i <= 20'0000'0000; i++);
	cout << "Thread " << this_thread::get_id() << " finished!" << endl;
	return;
}
int main() {
	thread th[10];
	for (int i = 0; i < 10; i++)
		th[i] = thread(::count);
	sleep(5000);
	ready = true;
	cout << "Start!" << endl;
	for (int i = 0; i < 10; i++)
		th[i].join();
	return 0;
}

```



### promise - thread函数 返回引用

#### 使用场景

`std::promise`实际上是`std::future`的一个包装，一般情况下：

- 使用`std::future`获取async创建线程的返回值。

- 使用`std::promise`获取thread创建线程的返回值。

  如果不想让 `std::async` 帮你自动创建线程，想要手动创建线程，可以直接用 `std::promise`。



##### 原理

如果使用thread以引用传递返回值的话，就必须要改变future的值，**std::future的值不能改变，那么如何利用引用传递返回值？**

答：改不了 我新建一个

- 可以**通过promise来创建一个拥有特定值的future**。

  返回的是新创建的future对象，没有改变任何已有future的值

- 在外面看起来就是，future的值不能改变，promise的值可以改变。



##### 过程

如何获得thread的返回值？过程是什么样的呢？

1. 在主线程

   - 声明一个 `std::promise` 变量。

   - 意味着这个值，虽然现在还没有，但是以后会有的，会有子线程把它填上的。


2. 在子线程中，

   - 设置在上一步承诺过会被填进去的值。

     调用 `std::promise`的 `set_value()`方法

3. 在主线程里，

     - 用 get_future() 获取其 std::future 对象，

     - 进一步 `std::future::get()` 可以等待并获取线程返回值。


  如果线程没有执行完，这里就阻塞。对应的场景就是，哎，你不是说过以后会有返回值的吗？你不能烂尾，就阻塞在这里，弄完了返回值再走。



#### 常用函数

```cpp
// 默认构造函数 构造一个空的promise对象
promise() 
// 带参构造函数
// 与默认构造函数相同，但使用特定的内存分配器alloc构造对象
template <class Alloc> 
promise(allocator_arg_t aa, const Alloc& alloc)
```

```cpp
promise (const promise&) = delete // 复制构造函数	（已删除）
```

```cpp
promise (promise&& x) noexcept	// 移动构造函数 构造一个与x相同的对象并破坏x
```

```cpp
~promise()	// 析构函数	析构对象
```



##### `set_value`

`std::promise<void>` 的 set_value() 不接受参数，仅仅作为同步用，不传递任何实际的值。

```cpp
// set_value函数
// - 设置promise的值
// - 将共享状态设为ready（将future_status设为ready）
// void特化：只将共享状态设为ready

// 一般
void set_value (const T& val)
void set_value (T&& val)
// 当类型为引用
void promise<R&>::set_value (R& val)
// 当类型为void
void promise::set_value (void)
```



##### `get_future`

```cpp
// 构造一个future对象，
// 其值与promise相同，status也与promise相同
future get_future()
```



#### demo

声明保存 int 的promise

```cpp
void download(std::string file, promise<int> &pret) {
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    pret.set_value(404);  // 向promise中写入值，创建future
}

int main() {
	//声明一个std::promise对象pret，其保存的值类型为int  
    std::promise<int> pret;
    std::thread t1([&] {
        auto ret = download("hello.zip", pret);
    });
    std::future<int> fret = pret.get_future(); // 从promise中取出future
    int ret = fret.get(); // 从future中取出返回值
    std::cout << "Download result: " << ret << std::endl;

    t1.join();
    return 0;
}

```

声明保存 string 的promise

```cpp
void download(std::string file, promise<string> &pret) {
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    pret.set_value("for promise");  // 向promise中写入值，创建future
    // return 1;
}

int main() {
	//声明一个std::promise对象pret，其保存的值类型为int  
    std::promise<string> pret;
    std::thread t1([&] {
        download("hello.zip", pret);
    });
    std::future<string> fret = pret.get_future(); // 从promise中取出future
    string ret = fret.get(); // 从future中取出返回值
    std::cout << "Download result: " << ret << std::endl;

    t1.join();
    return 0;
}
```



```cpp
// Compiler: MSVC 19.29.30038.1
// C++ Standard: C++17
#include <iostream>
#include <thread>
#include <future> // std::promise std::future
using namespace std;

template<class ... Args> decltype(auto) sum(Args&&... args) {
	return (0 + ... + args);
}

template<class ... Args> void sum_thread(promise<long long> &val, Args&&... args) {
	val.set_value(sum(args...));
}

int main() {
	promise<long long> sum_value;
	thread get_sum(sum_thread<int, int, int>, ref(sum_value), 1, 10, 100);
	cout << sum_value.get_future().get() << endl;
	get_sum.join();
	return 0;
}
```



## std::async

### std::async

std::async 接受一个带返回值的 lambda，自身返回一个 std::future 对象。

```cpp
template <class Fn, class… Args>
future<typename result_of<Fn(Args…)>::type> 
    async (Fn&& fn, Args&&… args)
```

> 不同于thread，async是一个模板函数，所以没有成员函数。

- 可以选择（手动选择或交给操作系统选择）
  - 同步执行 `launch::deferred`
  - 异步执行，创建新线程 `launch::async`
- 返回值操作比thread更加方便。

#### 引用传参

```cpp
template <class Fn, class… Args>
future<typename result_of<Fn(Args…)>::type> 
    async (Fn&& fn, Args&&… args)
```

以args为参数执行fn，传递引用参数需要std::ref或std::cref

- `std::ref` 可以包装`按引用传递`的值。
- `std::cref` 可以包装`按const引用传递`的值。

```cpp
std::future<bool> fret = std::async(
    std::launch::async, 
    [&](int x, int y) {
        return true;
    }, 
    1, 2
);
```





#### 异步/同步 policy

##### 指定

异步或同步，根据policy参数而定

`std::launch`有2个枚举值和1个特殊值：

```cpp
template <class Fn, class… Args>
future<typename result_of<Fn(Args…)>::type>
    async (launch policy, Fn&& fn, Args&&… args);
```

###### launch::deferred

`std::launch::deferred`也就是 `0x2`

- 同步启动

  - 创建 `std::async` 时，不会创建一个线程来执行，

  - 在调用`future::get`、`future::wait`时，才启动函数体

    推迟 lambda 函数体内的运算 到 future 的 get()或 wait() 被调用时

- 线程的执行仍在主线程中，

  这只是函数式编程范式意义上的异步，而不涉及到真正的多线程。

- 可以用这个实现惰性求值（lazy evaluation）之类。

```cpp
std::future<int> fret = std::async(std::launch::deferred, [&] {
    return download("hello.zip"); 
});
// 此时才开始启动
int ret = fret.get();
```



###### launch::async

`std::launch::async`也就是 `0x1`

- 异步启动
- 创建async时，创建一个线程来执行，

```cpp
// 此时就已经启动
std::future<int> fret = std::async(std::launch::async, [&] {
    return download("hello.zip"); 
});

int ret = fret.get();
```



##### 不指定

视操作系统而定。

```cpp
// 不显式指定，交给操作系统
std::future<int> fret = std::async([&] {
    return download("hello.zip"); 
});
int ret = fret.get();

// 不指定，显式的交给操作系统
std::future<int> fret = std::async(std::launch::async | std::launch::defereed, [&] {
    return download("hello.zip"); 
});

int ret = fret.get();
```



### future - async函数返回值

#### 作用

std::future的作用

- 检测 async 线程是否已结束

- 获取 async 返回值

  `std::async` 里 lambda 的返回类型可以为 void， 这时 future 对象的类型为 `std::future<void>`。

- 作为句柄，让async 阻塞等待/限时等待

  > 所以对于返回值是void的线程来说，future也同样重要。

```cpp
int main() {
	// 注：这里不能只写函数名sum，必须带模板参数
	future<int> val = async(launch::async, sum<int, int, int>, 1, 10, 100);
	// future::get() 阻塞等待线程结束并获得返回值
	cout << val.get() << endl;
	return 0;
}
```



```cpp
future() noexcept // 默认构造函数
// 	构造一个空的、无效的future对象，但可以移动分配到另一个future对象
```

```cpp
future(const future&) = delete // 复制构造函数	（已删除）
```

```cpp
future (future&& x) noexcept  // 移动构造函数	构造一个与x相同的对象并破坏x
```

```cpp
~future()	// 析构函数
```



#### 常用方法

需要调用 future 的方法，等待线程执行完毕。

##### get()

调用次数：只能调用一次

行为：

- 阻塞等待线程结束并获取返回值

  - 如果还没完成，阻塞等待，

    只要线程没有执行完，会无限等下去。

  - 获取返回值。

- 若future类型为void,则`future::get()`与`future::wait()`相同
- 如果是同步`launch::deferred`的async，启动的asycn函数

```cpp
T get() // 一般情况
R& future<R&>::get() // 类型为引用
void:void future::get() // 当类型为void,则与future::wait()相同
```

```cpp
std::future<int> fret = std::async([&] {
    return download("hello.zip"); 
});
// 等待线程执行结束
int ret = fret.get();
```



##### wait()

调用次数：只能调用一次

行为：

- 阻塞等待线程结束

  如果还没完成，阻塞等待，只要线程没有执行完，会无限等下去。

- 不会获取返回值。

  但是可以在`future::wait()`结束后，再次通过 `future::get()`获取返回值

- 如果是同步`launch::deferred`的async，启动的asycn函数

```cpp
std::future<int> fret = std::async([&] {
    return download("hello.zip"); 
});
// 等待线程执行结束,不获取返回值
fret.wait();
// 虽然已经结束，但还是可以获取返回值
int ret = fret.get();
```



##### wait_for() 

调用次数：无限制

行为：

- 阻塞等待   线程结束/限定时间到

  如果还没完成，阻塞等待

  - 可以用 chrono 里的类表示单位，指定一个最长等待时间。
  - 线程结束/限定时间到 结束阻塞

- 返回值

  类型： `std::future_status` 表示等待是否成功。

  - 返回 `std::future_status::timeout`

    线程在指定的时间内**没有执行完毕**，放弃等待

  - 返回 `std::future_status::ready`

    线程在指定的时间内**执行完毕**，等待成功

- 如果是同步`launch::deferred`的async，启动的asycn函数

```cpp
template <class Rep, class Period>
  future_status wait_for(const chrono::duration<Rep,Period>& rel_time) const;
```

```cpp
std::future<int> fret = std::async([&] {
    return download("hello.zip"); 
});

while (true) {
    // 循环多次等待
    auto stat = fret.wait_for(std::chrono::milliseconds(1000));
    if (stat == std::future_status::ready) {
        std::cout << "Future is ready!!" << std::endl;
        break;
    } else {
        std::cout << "Future not ready!!" << std::endl;
    }
}
// 虽然已经结束，但还是可以获取返回值
int ret = fret.get();
```



##### wait_until()

wait_until()

- 同理 wait_for() 
- 其参数是一个时间点。

```cpp
std::future<int> fret = std::async([&] {
    return download("hello.zip"); 
});

while (true) {
    // 循环多次等待
    auto stat = fret.wait_for(std::chrono::milliseconds(1000));
    if (stat == std::future_status::ready) {
        std::cout << "Future is ready!!" << std::endl;
        break;
    } else {
        std::cout << "Future not ready!!" << std::endl;
    }
}
// 虽然已经结束，但还是可以获取返回值
int ret = fret.get();
```



#### std::shared_future

future 为了三五法则，删除了拷贝构造/赋值函数。如果需要浅拷贝，实现共享同一个 future 对象，可以用 std::shared_future。

```cpp
void download(std::string file) {
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    std::cout << "Download complete: " << file << std::endl;
}

int main() {
    std::shared_future<void> fret = std::async([&] {
        download("hello.zip"); 
    });
    auto fret2 = fret;
    auto fret3 = fret;

    fret3.wait();
    std::cout << "Download completed" << std::endl;
    return 0;
}
```











## misc

### packaged_task 函数返回值

`std::packaged_task<F>`，绑定一个可调用对象，在未来期望的时候执行。

- 更方便的，天然异步的函数对象。

- 将可调对象传递给关联的`std::future`对象

  - 包装`std::promise<T= std::function<F>>`中的可调对象`T= std::function<F>`

  - 可调用对象的执行、返回值获取 需要透过 `future`。

- `std::packaged_task` 的返回类型是`void`

```cpp
int download02() {
    cout << "start download " << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    cout << "set promise value " << endl;
    return 2;
}

int main()
{
    // 给packaged_task绑定函数对象
    std::packaged_task<int()> t(download02);
    // 指定函数对象的返回值作为 future
    std::future<int> fu2 = t.get_future();
    // 执行packaged_task绑定的函数对象
    t();
    // 先执行之后，才可以
    // 如果要从中取出返回值 future, 需要
    int result = fu2.get();
    std::cout << result << std::endl;
    return 0;
}
```

考虑这样一个场景：多个线程共享一个任务队列

- 一个线程负责生产任务
  - 将任务放到任务队列中
  - 在这个任务执行后获取它的返回值.
- 多个子线程从任务队列中取出任务并执行

这里简化一下这个场景。主线程产生任务，一个子线程`t1`执行。

```sh
std::invoke_result_t

std::packaged_task
```



#### std::jthread 自动join

C++20 引入了 std::jthread 类，

和 std::thread 不同在于：

- 他的解构函数里会自动调用 join() 函数，
- 保证了解构时会自动等待线程执行完毕。

native_handler

jthread 自动join



### std::call_once 执行一次

在多线程的环境下， 某些可执行对象只需要执行一次。std::call_once 应运而生。

- 封装了加锁和修改flag的过程。**简化代码(不再需要判空/添加flag)，增强可读性。**
- **本身不具有类似 thread.join() 的功能。需要子线程join 或 主线程进行等待**

原理：判断全局变量标识符once_flag。如果这个once_flag

- **"未执行过"** 
  - 加锁，执行，结束后释放锁mutex
  - 修改标识符为**"执行过"**
- **"执行过"** 被标记过。不再执行。

```cpp
template< class Function, class... Args >
void call_once (std::once_flag& flag, Function&& f, Args&& args... );
// 参数解析：
// std::once_flag flag  判断是个需要执行。若执行，执行后关闭。
// f 和 args... 需要被调用的函数以及函数f的传参
//  lambda 常用 [this]
// 抛出异常 std::system_error if any condition prevents calls to call_once from executing as specified any exception thrown by f
```

```cpp
// 初始化资源(节省数组判空)
// 单例模式
#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
 
std::once_flag flag1;

class PySingleton {
public:
static PySingleton* mInstance;
static PySingleton* get_instance() {
    std::call_once(flag1, [] (int id) {
        mInstance = new PySingleton(id);
    }, 1);
    return mInstance;
}

private:
PySingleton(int id) {
    std::cout << " do init, id = " << id << " \n";
}
};

PySingleton* PySingleton::mInstance = nullptr; // 必须在这里 为static mInstance，编译期执行  


void test() {
    PySingleton* py = PySingleton::get_instance();
}

int main()
{
    std::thread mThread(test);
    mThread.join();
    return 0;
}
```

```cpp
#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
 
std::once_flag flag1;

void do_print1() {
    std::cout << "do_print1 example: called once\n"; 
}
void do_print2() {
    std::cout << "do_print2 example: called once\n"; 
}

void simple_do_once()
{
    std::function<void()> mFunc1 = do_print1;
    std::function<void()> mFunc2 = do_print2;
    std::call_once(flag1, mFunc1); // 仅仅执行了一次这个
    std::call_once(flag1, mFunc2);
}
 
int main()
{
    std::thread st1(simple_do_once);
    std::thread st2(simple_do_once); // 什么都没做
    st1.join();
    st2.join();
    return 0;
}
```



# cpp mutex

## 避免死锁

#### 总览

解决方法

1. 确定使用场景

   1. 当前资源是否真的存在竞争，是否真的需要使用mutex。
      不需要的话就不要用。自然就没有死锁

   2. 资源的所有权能不能简化，让 每个线程只持有一把锁。

2. 使用者的错误：从工具上避免

   忘记解锁 lock_guard，自由上/解锁unique_lock（更高的开销）

3. 非多线程的问题：同一线程中的 单一锁 多次上锁。

   > 在函数A，函数B同时都需要用一把mutex锁上锁。函数A会调用到函数B

   1. 会锁死就干脆不要在函数里面用

      - 取消各个使用处的这个锁

        > 顾虑就是各个部分单独的使用，也都要加锁。更加的麻烦。

      - 写注释告诉使用者

        - 标注某个锁是线程不安全的。
        - 要求使用者必须在执行前为某个mutex上锁。

   2. `std::recursive_mutex` 允许同一线程多次使用一把锁

      > 如果同时需要 try_lock_for() 还有 std::recursive_timed_mutex

      - 自动判断是不是同一个线程 lock() 了多次同一个锁。

        如果是则

        - lock 计数器加1

        - unlock() 会让计数器减1，减到0时才真正解锁。

      - 相比普通的 std::mutex 有一定性能损失

4. 多把锁

   1. 操作层面：

      1. 避免上锁顺序不同。
      2. 单一线程就不要持有多把锁，没有问题就不需要解决问题。

   2. std::scoped_lock 

      直接同时对多个 mutex 上锁，不关心顺序。

   3. 操作系统（jvm C#虚拟机）检测死锁。选择性打破。

#### 同一线程内 一把锁多次上锁

允许同一线程多次使用一把锁。

除了两个线程同时持有两个锁会造成死锁外，即使只有一个线程一个锁，如果 lock() 以后又调用 lock()，也会造成死锁。

有时候，我们确确实实的有如下需求：

> 如果要求这是一个连续的“原子的”操作，那么在函数A中调用函数B前，解锁mutex，执行结束后再上锁，这个解决方法就是不可行的。

- 在函数A，函数B同时都需要用一把mutex锁上锁。
- 函数A会调用到函数B



##### 标注线程不安全

会锁死就干脆不要在函数里面用，标注函数为线程不安全的，声明必须在执行前为某个mutex上锁。

- 取消各个使用处的这个锁

  > 顾虑就是各个部分单独的使用，也都要加锁。更加的麻烦。

- 写注释告诉使用者

  - 标注某个锁是线程不安全的。
  - 要求使用者必须在执行前为某个mutex上锁。

```cpp
std::mutex mtx1;
/// NOTE: please lock mtx1 before calling other()
void other() {
    // do something
}

void func() {
    mtx1.lock();
    other();
    mtx1.unlock();
}

int main() {
    func();
    return 0;
}

```



##### std::recursive_mutex 

如果实在不能改的话，可以用 `std::recursive_mutex`。

优点

- 允许同一线程多次使用一把锁（会帮我们判断是否是同一线程）

缺点

- 相比普通的 std::mutex 有一定性能损失

- 自动判断是不是同一个线程 lock() 了多次同一个锁，

  - 如果是则让计数器加1

  - 之后 unlock() 会让计数器减1，减到0时才真正解锁。

> 同理还有 std::recursive_timed_mutex，如果你同时需要 try_lock_for() 的话。

```cpp
std::recursive_mutex mtx1;

void other() {
    mtx1.lock();
    // do something
    mtx1.unlock();
}

void func() {
    mtx1.lock();
    other();
    mtx1.unlock();
}

int main() {
    func();
    return 0;
}
```



#### 多把锁 上锁顺序不同

##### 上锁顺序相同

复数个线程之间，保证多把锁的上锁顺序相同

```cpp
int main() {
    std::mutex mtx1;
    std::mutex mtx2;

    std::thread t1([&] {
        for (int i = 0; i < 1000; i++) {
            mtx1.lock();
            mtx2.lock();
            mtx2.unlock();
            mtx1.unlock();
        }
    });
    std::thread t2([&] {
        for (int i = 0; i < 1000; i++) {
            mtx1.lock();
            mtx2.lock();
            mtx2.unlock();
            mtx1.unlock();
        }
    });
    t1.join();
    t2.join();
    return 0;
}
```



##### 对多个 mutex 上锁

见上文  std::scoped_lock

-  std::scoped_lock 可以同时对多个 mutex 上锁，
-   std::lock 只能对单个锁上锁。

##### 单一线程 不要同时持有两个锁

同一线程，永远不要同时持有两个锁 （锁一个，就释放一个）

```cpp
int main() {
    std::mutex mtx1;
    std::mutex mtx2;

    std::thread t1([&] {
        for (int i = 0; i < 1000; i++) {
            mtx1.lock();
            mtx1.unlock();
            mtx2.lock();
            mtx2.unlock();
        }
    });
    std::thread t2([&] {
        for (int i = 0; i < 1000; i++) {
            mtx2.lock();
            mtx2.unlock();
            mtx1.lock();
            mtx1.unlock();
        }
    });
    t1.join();
    t2.join();
    return 0;
}
```



## 锁的种类

### mutex

`std::mutex`是 C++11 中最基本的互斥量，一个线程将mutex锁住时，其它的线程就不能操作mutex，直到这个线程将mutex解锁。

不同的对象，各有一个 mutex，独立地上锁，可以避免不必要的锁定，提升高并发时的性能。

##### 操作

###### lock

保证 mtx.lock() 和 mtx.unlock() 之间的代码段，同一时间只有一个线程在执行，从而避免数据竞争。

```cpp
void lock(); // 将mutex上锁。
```

调用 `std::mutex::lock()`  时，会检测 mutex 是否已经上锁。

- 如果没有锁定，则对 mutex 进行上锁。
- 如果已经锁定，则陷入等待，直到 mutex 被解锁后，才再次上锁。
  - mutex被其它线程上锁，那么会阻塞，直到解锁；
  - mutex被同一个线程锁住，那么会产生死锁。



###### unlock

```cpp
void unlock() // 解锁mutex，释放其所有权。
```

调用 `std::mutex::unlock()` 会进行解锁操作。

- 如果有线程因为调用lock()不能上锁而被阻塞，

  则调用此函数会将mutex的主动权随机交给其中一个线程；

- 如果mutex不是被此线程上锁，那么会引发未定义的异常。



###### try_lock

非阻塞的 `try_lock()`

- 如果mutex未被上锁（上锁成功），则将其上锁，返回true；
- 如果mutex已被锁（上锁失败），不会陷入等待，返回false。

```cpp
std::mutex mtx1;

int main() {
    if (mtx1.try_lock())
        printf("succeed\n");
    else
        printf("failed\n");

    if (mtx1.try_lock())
        printf("succeed\n");
    else
        printf("failed\n");
    
    mtx1.unlock();
    return 0;
}
```



###### try_lock_for

try_lock_for() 

- 如果mutex未被上锁（上锁成功），

  - 将其上锁，返回true；

- 如果mutex已被锁（上锁失败），

  设置、等待预设的最长等待时间（由 chrono 指定时间单位）

  - 这个时间内一直没有成功，返回false。
  - 这个时间内上锁成功，返回 true

同理还有接受时间点的 try_lock_until()。

```cpp
std::timed_mutex mtx1;

int main() {
    if (mtx1.try_lock_for(std::chrono::milliseconds(500)))
        printf("succeed\n");
    else
        printf("failed\n");

    if (mtx1.try_lock_for(std::chrono::milliseconds(500)))
        printf("succeed\n");
    else
        printf("failed\n");

    mtx1.unlock();
    return 0;
}

```





##### demo

```cpp
int main() {
    std::vector<int> arr;
    std::mutex mtx;
    std::thread t1([&] {
        for (int i = 0; i < 1000; i++) {
            mtx.lock();
            arr.push_back(1);
            mtx.unlock();
        }
    });
    std::thread t2([&] {
        for (int i = 0; i < 1000; i++) {
            mtx.lock();
            arr.push_back(2);
            mtx.unlock();
        }
    });
    t1.join();
    t2.join();
    return 0;
}
```



### 多次上锁 recursive_mutex 

##### recursive_mutex

##### recursive_timed_mutex

允许同一线程多次使用一把锁。

详情见下一章节，`避免死锁 - 一把锁多次上锁`





### 读写锁 shared_mutex

mutex 认为，同一时刻只有一个人能访问资源。但是如果“访问”有两种方式呢？

`std::shared_mutex` ：

读可以共享，写必须独占，且写和读不能共存。

允许的状态有：

- n个人读取，没有人写入。
- 1个人写入，没有人读取。
- 没有人读取，也没有人写入。

> 上锁时，要指定你的需求是读还是写，负责调度的读写锁会帮你判断要不要等待。

```cpp
class MTVector {
    std::vector<int> m_arr;
    mutable std::shared_mutex m_mtx;

public:
    void push_back(int val) {
        m_mtx.lock();			// 写上锁(独占)
        m_arr.push_back(val);
        m_mtx.unlock();			// 写解锁(独占)
    }

    size_t size() const {
        m_mtx.lock_shared(); 	// 读上锁(非独占)
        size_t ret = m_arr.size();
        m_mtx.unlock_shared();	// 读解锁(非独占)
        return ret;
    }
};
```







### RAII的mutex

用了 `{}` 包住 `std::lock_guard`，限制其变量的作用域，从而可以让他在 `}` 解构并调用 `unlock()`，也避免了和下面一个 lock_guard 变量名冲突。

#### lock_guard

`std::lock_guard` 符合 RAII 思想的上锁和解锁

> std::unique_lock 具有 mutex 的所有成员函数
>
> 除了他会在解构时按需自动调用 unlock()。

根据 RAII 思想，可将锁的持有视为资源，退出函数作用域时能够自动解锁，避免程序员粗心不小心忘记解锁。

- 构造函数会调用 mtx.lock()，上锁视为锁的获取
- 解构函数会调用 mtx.unlock()，解锁视为锁的释放

```cpp
int main() {
    std::vector<int> arr;
    std::mutex mtx;
    std::thread t1([&] {
        for (int i = 0; i < 1000; i++) {
            {
                std::lock_guard grd(mtx);
                arr.push_back(1);
            }
            {
                std::lock_guard grd(mtx);
                arr.push_back(1);
            }
        }
    });
    std::thread t2([&] {
        for (int i = 0; i < 1000; i++) {
            std::lock_guard grd(mtx);
            arr.push_back(2);
        }
    });
    t1.join();
    t2.join();
    return 0;
}

```



#### unique_lock

std::unique_lock：符合 RAII 思想，但自由度更高

自由度：

- 支持提前解锁/再次上锁

  `std::unique_lock(std::mutex);`

- 延迟上锁（不在构造函数上锁，lock后才上锁）

  `std::unique_lock(std::mutex, std::defer_lock);`

- 判断是否上锁（通过`owns_lock()`进行判断）

  `std::unique_lock(std::mutex, std::try_to_lock);`

##### 默认构造

```cpp
std::unique_lock unique_lock(std::mutex);
```

###### RAII

符合 RAII 思想

- 构造函数会调用 mtx.lock()，上锁视为锁的获取
- 解构函数会调用 mtx.unlock()，解锁视为锁的释放

###### 手动 unlock/lock

除了在解构时 unlock()，但是有时候我们会希望提前 unlock()。这一点区别于 `std::lock_guard`

- 可以直接调用 unique_lock 的 unlock() 函数来提前解锁，

- 原理

  - `std::unique_lock`，额外存储了一个 flag 表示是否已经被释放。

  - 在解构时检测这个 flag，如果没有释放，则调用 unlock()，否则不调用。
  
    即使忘记解锁也没关系，退出作用域时候他还会自动检查一遍要不要解锁。

```cpp
std::thread t2([&] {
    for (int i = 0; i < 1000; i++) {
        std::unique_lock grd(mtx);
        arr.push_back(2);
        grd.unlock();
        printf("outside of lock\n");
        // grd.lock();  // 如果需要，还可以重新上锁
    }
});
```



##### 重载构造函数

###### 延迟上锁 defer_lock

构造函数指定额外参数：`std::defer_lock`

延迟上锁（不在构造函数上锁，lock后才上锁。当然，也保留了再次手动上锁解锁的自由度）

`std::unique_lock(std::mutex, std::defer_lock);`

- 不会在构造函数中调用 mtx.lock()，

- 需要之后再手动调用 grd.lock() 才能上锁。


```cpp
std::thread t2([&] {
    for (int i = 0; i < 1000; i++) {
        std::unique_lock grd(mtx, std::defer_lock);
        printf("before the lock\n");
        grd.lock();
        arr.push_back(2);
        grd.unlock();
        printf("outside of lock\n");
    }
});
t2.join();


```







###### 判断上锁 try_to_lock

构造函数指定额外参数：

- `std::try_to_lock` 做参数

  `std::unique_lock(std::mutex, std::try_to_lock);`

- 尝试上锁
  
- 可以用 `owns_lock()` 判断是否 拥有锁 / 被其它线程上锁
  
  - true 当前线程拥有锁
  - false 当前线程不拥有锁，此时被其它线程上锁。

示例中，如果 t1先执行，t1拥有锁，所以此时 t1 success。t2会打印fail，但并不会因此阻塞。

```cpp
int main() {
    std::mutex mtx;
    std::thread t1([&] {
        std::unique_lock grd(mtx, std::try_to_lock);
        if (grd.owns_lock())
            printf("t1 success\n");
        else
            printf("t1 failed\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    });

    std::thread t2([&] {
        std::unique_lock grd(mtx, std::try_to_lock);
        if (grd.owns_lock())
            printf("t2 success\n");
        else
            printf("t2 failed\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    });

    t1.join();
    t2.join();
    return 0;
}

```





###### 析构时解锁 adopt_lock

构造函数指定额外参数：

- `std::adopt_lock` 做参数

  `std::unique_lock(std::mutex, std::adopt_lock);`

- 默认此时 当前mutex已经上锁

- 但是之后仍然希望用 RAII 思想，在解构时候自动调用 unlock()，

```cpp
int main() {
    std::mutex mtx;
    std::thread t2([&] {
        mtx.lock(); // 此时已经锁住了
        std::unique_lock grd(mtx, std::adopt_lock);  // 但是没关系，我们就认为已经锁住了，等会再解锁
        printf("t2 owns the lock\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    });  // unique_lock析构时释放mtx锁

    t2.join();
    return 0;
}

```



##### 示例demo

```cpp
int main() {
    std::vector<int> arr;
    std::mutex mtx;
    std::thread t1([&] {
        for (int i = 0; i < 1000; i++) {
            std::unique_lock grd(mtx); // 自动上锁
            arr.push_back(1);
        } // 离开作用域检查是否解锁
    });
    std::thread t2([&] {
        for (int i = 0; i < 1000; i++) {
            std::unique_lock grd(mtx);
            arr.push_back(2);
            grd.unlock();   // 手动解锁
            printf("outside of lock\n");
            // grd.lock();  // 再次手动上锁
        } // 离开作用域检查是否解锁
    });
    std::thread t3([&] {
        for (int i = 0; i < 1000; i++) {
            std::unique_lock grd(mtx, std::defer_lock); // 希望延迟上锁
            printf("before the lock\n");
            grd.lock();  // 上锁
            arr.push_back(2);
            grd.unlock(); // 手动解锁
            printf("outside of lock\n");
        } // 离开作用域检查是否解锁
    });
    std::thread t4([&] { // 构造函数有第二个入参 std::try_to_lock
        std::unique_lock grd(mtx, std::try_to_lock);
        if (grd.owns_lock()) // 判断是否已经上锁
            printf("t4 success\n"); // 已经上锁
        else
            printf("t4 failed\n"); // 还没有上锁
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    });
    std::thread t5([&] { // 构造函数有第二个入参 std::adopt_lock
        mtx.lock(); // 此时已经锁住了
        std::unique_lock grd(mtx, std::adopt_lock);  // 但是没关系，我们就认为已经锁住了，等会再解锁
        printf("t5 owns the lock\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }); // unique_lock析构时释放mtx锁
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    return 0;
}
```



#### shared_lock

std::shared_lock：符合 RAII 思想，但自由度更高，且支持读写锁

> 正如 `std::unique_lock` 针对 lock()
>
> 也可以用 `std::shared_lock` 针对 lock_shared()。

可以在函数体退出时自动调用 `unlock_shared()`，更加安全。

shared_lock 同样支持 defer_lock 做参数，owns_lock() 判断等，同学们自己研究。

```cpp
class MTVector {
    std::vector<int> m_arr;
    mutable std::shared_mutex m_mtx;

public:
    void push_back(int val) {	
        std::unique_lock grd(m_mtx); // 写锁
        m_arr.push_back(val);
    }

    size_t size() const {
        std::shared_lock grd(m_mtx); // 读锁
        return m_arr.size();
    }
};

int main() {
    MTVector arr;

    std::thread t1([&] () {
        for (int i = 0; i < 1000; i++) {
            arr.push_back(i);
        }
    });

    std::thread t2([&] () {
        for (int i = 0; i < 1000; i++) {
            arr.push_back(1000 + i);
        }
    });

    t1.join();
    t2.join();

    std::cout << arr.size() << std::endl;

    return 0;
}

```



#### 对多个 mutex 上锁

##### std::lock

如果没办法保证上锁顺序一致，

- 可以用标准库的 std::lock(mtx1, mtx2, ...) 函数
- 一次性对多个 mutex 上锁。
  - 接受任意多个 mutex 作为参数，
  - 保证在无论任意线程中调用的顺序是否相同，都不会产生死锁问题。

```cpp
int main() {
    std::mutex mtx1;
    std::mutex mtx2;

    std::thread t1([&] {
        for (int i = 0; i < 1000; i++) {
            std::lock(mtx1, mtx2);
            mtx1.unlock();
            mtx2.unlock();
        }
    });
    std::thread t2([&] {
        for (int i = 0; i < 1000; i++) {
            std::lock(mtx2, mtx1);
            mtx2.unlock();
            mtx1.unlock();
        }
    });
    t1.join();
    t2.join();
    return 0;
}
```



##### std::scoped_lock

std::lock 的 RAII 版本：std::scoped_lock

和 std::lock_guard 相对应，std::lock 也有 RAII 的版本 std::scoped_lock。

-  std::scoped_lock 可以同时对多个 mutex 上锁，
-  std::lock_guard 只对应一个mutex

```cpp
int main() {
    std::mutex mtx1;
    std::mutex mtx2;

    std::thread t1([&] {
        for (int i = 0; i < 1000; i++) {
            std::scoped_lock grd(mtx1, mtx2);
            // do something
        }
    });
    std::thread t2([&] {
        for (int i = 0; i < 1000; i++) {
            std::scoped_lock grd(mtx2, mtx1);
            // do something
        }
    });
    t1.join();
    t2.join();
    return 0;
}
```





### 条件变量 condition_variable

##### 陷入等待

wait() 函数 让当前线程陷入等待，等待被唤醒

- `cv.wait(lck)` 

  wait() 的过程中会暂时 unlock() 这个锁。

  - 在其他线程中调用 `cv.notify_one()`/`cv.notify_all()`

    会唤醒那个陷入等待的线程。

- `cv.wait(lck, expr)` 条件性唤醒

  - 在其他线程中调用 `cv.notify_one()`/`cv.notify_all()`

  - 并且 expr 表达式，返回值为 true 时才会真正唤醒，否则继续等待。

    才会唤醒那个陷入等待的线程。

还有 wait_for() 和 wait_until() 函数，分别接受 chrono 时间段和时间点作为参数。详见：https://en.cppreference.com/w/cpp/thread/condition_variable/wait_for。



##### 唤醒

- `cv.notify_one()` 只会唤醒其中一个等待中的线程，
- `cv.notify_all()` 会唤醒全部等待中的线程

`std::condition_variable` 必须和 `std::unique_lock<std::mutex>` 一起用，wait() 需要一个 unique_lock 作为参数

- 因为要保证多个线程被唤醒时，只有一个能够被启动

  如果不需要，在 wait() 返回后调用 lck.unlock() 即可。

- `std::condition_variable` 仅仅支持 `std::unique_lock<std::mutex>` 作为 wait 的参数，如果需要用其他类型的 mutex 锁，可以用 `std::condition_variable_any`。

##### 示例

```cpp
#include <thread>
#include <mutex>
#include <condition_variable>
```

```cpp
std::condition_variable cv;
std::mutex mtx;

std::thread t1([&] {
    std::unique_lock lck(mtx);
    std::cout << "t1 start waiting" << std::endl;
    cv.wait(lck);
    std::cout << "t1 is awake" << std::endl;
});

std::thread t2([&]{
    std::cout << "t2 notifying t1..." << std::endl;
    cv.notify_one();  // will awake t1
    std::cout << "t2 notify t1 done" << std::endl;
});

t1.detach();
t2.detach();
```



## 线程安全的容器

#### mutex

const成员函数，不能修改成员变量，除非成员变量被标记为 mutable

> 要为了支持 mutex 而放弃声明 size() 为 const 吗？
>
> 不必，size() 在逻辑上仍是 const 的。因此，为了让 this 为 const 时仅仅给 m_mtx 开后门，可以用 mutable 关键字修饰他，从而所有成员里只有他不是 const 的。

```cpp
class MTVector {
    std::vector<int> m_arr;
    mutable std::mutex m_mtx;

public:
    void push_back(int val) {
        m_mtx.lock();
        m_arr.push_back(val);
        m_mtx.unlock();
    }

    size_t size() const {
        m_mtx.lock();
        size_t ret = m_arr.size();
        m_mtx.unlock();
        return ret;
    }
};

int main() {
    MTVector arr;

    std::thread t1([&] () {
        for (int i = 0; i < 1000; i++) {
            arr.push_back(i);
        }
    });

    std::thread t2([&] () {
        for (int i = 0; i < 1000; i++) {
            arr.push_back(1000 + i);
        }
    });

    t1.join();
    t2.join();

    std::cout << arr.size() << std::endl;

    return 0;
}

```



#### 读写锁 shared_lock

```cpp
class MTVector {
    std::vector<int> m_arr;
    mutable std::shared_mutex m_mtx;

public:
    void push_back(int val) {	
        std::unique_lock grd(m_mtx); // 写锁
        m_arr.push_back(val);
    }

    size_t size() const {
        std::shared_lock grd(m_mtx); // 读锁
        return m_arr.size();
    }
};

int main() {
    MTVector arr;

    std::thread t1([&] () {
        for (int i = 0; i < 1000; i++) {
            arr.push_back(i);
        }
    });

    std::thread t2([&] () {
        for (int i = 0; i < 1000; i++) {
            arr.push_back(1000 + i);
        }
    });

    t1.join();
    t2.join();

    std::cout << arr.size() << std::endl;

    return 0;
}

```



#### visitor模式

一次性上锁，进行多次读/写。

类似的可以使用  ConstAccessor 和 Accessor 两种visitor，分别对应于读和写

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

class MTVector {
    std::vector<int> m_arr;
    std::mutex m_mtx;

public:
    class Accessor {
        MTVector &m_that;
        std::unique_lock<std::mutex> m_guard;

    public:
        Accessor(MTVector &that)
            : m_that(that), m_guard(that.m_mtx)
        {}

        void push_back(int val) const {
            return m_that.m_arr.push_back(val);
        }

        size_t size() const {
            return m_that.m_arr.size();
        }
    };

    Accessor access() {
        return {*this};
    }
};

int main() {
    MTVector arr;

    std::thread t1([&] () {
        auto axr = arr.access();
        for (int i = 0; i < 1000; i++) {
            axr.push_back(i);
        }
    });

    std::thread t2([&] () {
        auto axr = arr.access();
        for (int i = 0; i < 1000; i++) {
            axr.push_back(1000 + i);
        }
    });

    t1.join();
    t2.join();

    std::cout << arr.access().size() << std::endl;

    return 0;
}

```







## atomic原子对象

#### 原子操作 `atomic<>`

atomic能够直接当作普通变量使用。成员函数貌似没啥用。想搞明白的[点这里](http://cplusplus.com/reference/atomic/atomic/) 

atomic：有专门的硬件指令加持，最小的且不可并行化的操作。

- CPU 识别到该指令时，会锁住内存总线，放弃乱序执行等优化策略（将该指令视为一个同步点，强制同步掉之前所有的内存操作），从而向你保证该操作是原子 (atomic) 的（取其不可分割之意），不会加法加到一半另一个线程插一脚进来。

  - 即使是多线程，也能像同步进行一样同步操作atomic对象

    对他的 += 等操作，会被编译器转换成专门的指令。

  - 不必像 mutex 手动上锁解锁

    - 用起来也更直观
    - 也省去了上锁、解锁的时间消耗。

- 使用： `atomic<int>`替代 int  即可。

#### 支持的运算符

注意：请用 +=，不要让 + 和 = 分开

```cpp
counter = counter + 1;  // 错，不能保证原子性
counter += 1;           // OK，能保证原子性
counter++;              // OK，能保证原子性
```

除了用方便的运算符重载之外，还可以直接调用相应的函数名，比如：

- store 对应于 =

- load 用于读取其中的 int 值

追加值

- fetch_add 对应于 +=

  `int old = atm.fetch_add(val)`

  - 除了会导致 atm 的值增加 val 外，

  - 还会返回其旧值（增加前的值），存储到 old。

    这个特点使得他可以用于并行地往一个列表里追加数据：追加写入的索引就是 fetch_add 返回的旧值。

- `counter.fetch_add(n)` 追加值，避免多次 `counter++`

交换（设置）

- `exchange(val)` 读取的同时写入
  - 把 val 写入原子变量，
  - 同时返回其旧的值。

- `compare_exchange_strong`

  读取原子变量的值，比较他是否和 old 相等，相等则写入

  - 如果不相等，则把原子变量的值写入 old。
  - 如果相等，则把 val 写入原子变量。
  - 返回一个 bool 值，表示是否相等。

  > 注意 old 这里传的其实是一个引用，因此 compare_exchange_strong 可修改他的值

#### atomic_int

`std::atomic_int` 是 `std::atomic<int>`的别名。

构造函数

`std::atomic_int` 没有显式定义析构函数

```cpp
// 默认构造函数
// 未初始化，可通过atomic_init进行初始化
atomic() noexcept = default  
```

```cpp
//初始化构造函数
// 构造一个atomic对象，用val的值来初始化
constexpr atomic(T val) noexcept	
```



```cpp
// Compiler: MSVC 19.29.30038.1
// C++ Standard: C++17
#include <iostream>
#include <thread>
// #include <mutex> //这个例子不需要mutex了
#include <atomic>
using namespace std;
atomic_int n = 0; // atomic<int> n = 0;
void count10000() {
	for (int i = 1; i <= 10000; i++) {
		n++;
	}
}
int main() {
	thread th[100];
	for (thread &x : th)
		x = thread(count10000);
	for (thread &x : th)
		x.join();
	cout << n << endl;
	return 0;
}

```



