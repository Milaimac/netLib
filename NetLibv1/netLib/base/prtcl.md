
# prtcl函数

~~~cpp
int prctl ( int option,unsigned long arg2,unsigned long arg3,unsigned long arg4,unsigned long arg5 )
~~~

prtcl -- operations on a process or thread 

一个对进程或者线程操作的函数

第一个参数 option 决定了这个函数要做什么事

这个系统调用指令是为进程制定而设计的，明确的选择取决于 option:
`PR_GET_PDEATHSIG `:返回处理器信号；

`PR_SET_PDEATHSIG `:arg2作为处理器信号pdeath被输入，正如其名，如果父进程不能再用，进程接受这个信号。
 
`PR_GET_DUMPABLE `:返回处理器标志dumpable;
 
PR_SET_DUMPABLE :arg2作为处理器标志dumpable被输入。
 
PR_GET_NAME :返回调用进程的进程名字给参数arg2; （Since Linux2.6.9）
 
PR_SET_NAME :把参数arg2作为调用进程的经常名字。（SinceLinux 2.6.11）
 
PR_GET_TIMING :
 
PR_SET_TIMING :判定和修改进程计时模式,用于启用传统进程计时模式的
 
PR_TIMING_STATISTICAL，或用于启用基于时间戳的进程计时模式的
 
PR_TIMING_TIMESTAMP。
 
CAP_CHOWN功能：
在一个_POSIX_CHOWN_RESTRICTED功能定义的系统。这会越过改变系统文件所有者和组所有的权限
 
CAP_DAC_OVERRIED功能：
如果_POSIX_ACL定义，就会越过所有的DAC访问，包括ACL执行访问，用CAP_LINUX_IMMUTABLE功能来排除
DAC的访问
 
CAP_DAC_READ_SEARCH功能：
如果_POSIX_ACL定义，就会越过所有的DAC的读限制，
 并在所有的文件和目录里搜索，包括ACL限制。用CAP_LINUX_IMMUTABLE来限制DAC访问
 
CAP_FOWNER功能：
 越过文件说有的允许限制，如文件的所有者ID必须和用户ID一样，除了CAP_FSETID可用。它不会越过MAC和DAC限制
 
CAP_FSETID功能：
越过当设置文件的S_ISUID和S_ISGID位的时候，用户的ID必须和所有者ID匹配的限制，设置S-ISGID位的时候，组ID
必须和所有者ID匹配的限制，用chown来设置S_ISUID和S_ISGID为的功能限制
 
CAP_FS_MASK功能：
用来回应suser（）或是fsuser（）。
 
CAP_KILL功能：
一个有有效用户ID的进程发送信号时必须匹配有效用户ID的功能会越过
 
CAP_SETGID功能：
允许setgid（） 功能， 允许setgroups（）
允许在socket里伪造gid
 
CAP_SETUID功能：
允许set*uid（）功能 允许伪造pid在socket
 
CAP_SETPCAP 功能：
把所有的许可给所有的pid。或是把所有的许可删除
 
CAP_LINUX_IMMUTABLE功能：
允许更改S_IMMUTABLE和S_APPEND文件属性
 
CAP_NET_BIND_SERVICE功能：
允许绑定1024下的TCP/UDP套接字
 
CAP_NET_BROADCAST功能：
允许广播，监听多点传送
 
CAP_NET_ADMIN功能：
 允许配置接口
允许管理IP防火墙IP伪装和帐户
允许配置socket调试选项
允许修改路由表
允许配置socket上的进程的组属性
允许绑定所有地址的透明代理
允许配置TOS（服务类型）
允许配置混杂模式
允许清除驱动状态
允许多点传送
允许读或写系统记录
 
CAP_NET_RAW功能：
允许用RAW套接字
允许用PACKET套接字
 
CAP_IPC_LOCK功能：
允许琐定共享内存段
允许mlock和mlockall
 
CAP_IPC_OWNER功能：
越过IPC所有权检查
 
CAP_SYS_MODULE功能：
 插入或删除内核模块
 
CAP_SYS_RAWIO功能：
允许ioperm/iopl和/dev/prot的访问
允许/dev/mem和/dev/kmem访问
允许块设备访问（/dev/[sh]d？？）
 
CAP_SYS_CHROOT功能：
允许chroot（）
 
CAP_SYS_PTRACE功能：
允许ptrace（）任何进程
 
CAP_SYS_PACCT功能：
允许配置进程帐号
 
CAP_SYS_ADMIN功能：
允许配置安全钥匙
允许管理随机设备
允许设备管理
允许检查和配置磁盘限额
允许配置内核日志
允许配置域名
允许配置主机名
允许调用bdflush（）命令
允许mount（）和umount（）命令
允许配置smb连接
允许root的ioctls
允许nfsservctl
允许VM86_REQUEST_IRQ
允许在alpha上读写pci配置
允许在mips上的irix_prctl
允许刷新所有的m68k缓存
 允许删除semaphores
用CAP_CHOWN去代替"chown"IPC消息队列，标志和共享内存
允许锁定或是解锁共享内存段
允许开关swap
允许在socket伪装pids
允许设置块设备的缓存刷新
允许设置软盘驱动器
允许开关DMA开关
允许管理md设备
允许管理ide驱动
允许访问nvram设备
允许管理apm_bios，串口或是bttv电视设备
允许在isdn CAPI的驱动下生成命令
允许读取pci的非标准配置
允许DDI调试ioctl
允许发送qic-117命令
允许启动或禁止SCSI的控制和发送SCSI命令 允许配置加密口令在回路文件系统上
 
CAP_SYS_BOOT功能：
允许用reboot（） 命令
 
CAP_SYS_NICE功能：
允许提高或设置其他进程的优先权
允许在自己的进程用FISO和实时的安排和配置
 
CAP_SYS_RESOURCE功能：
越过资源限制，设置资源限制
越过配额限制
越过保留的ext2文件系统
允许大于64hz的实时时钟中断
越过最大数目的控制终端
越过最大数目的键
 
CAP_SYS_TIME功能：
允许处理系统时钟
允许_stime
允许设置实时时钟
 
CAP_SYS_TTY_CONFIG功能：
 允许配置终端设备
允许vhangup（）终端
 
返回值
 
PR_GET_DUMPABLE 和 PR_GET_KEEPCAPS 成功时返回0或者1。其他的option值都是成功时返回0。
错误时返回 －1，并设置相应的错误号。
 
EINVAL————option的值不正确，或者当它是PR_SET_PDEATHSIG时，参数arg2的值不是0或者信号数字。
 
EBADF————无效的描述符
 
实例：于多线程应用程序，如果能够给每个线程命名，那么调试起来的便利是不言而喻的。
```cpp
#include<stdio.h>
#include<pthread.h>
#include<sys/prctl.h>


void* tmain(void*arg)
{
char name[32];
prctl(PR_SET_NAME,(unsignedlong)"xx");
prctl(PR_GET_NAME,(unsignedlong)name);
printf("%s/n", name);
while(1)
sleep(1);
}


int main(void)
{
pthread_t tid;
pthread_create(&tid,NULL, tmain,NULL);
pthread_join(tid,NULL);


return 0;
}
```
编 译并运行：
```
xiaosuo@gentux test $ gcc t_threadname.c -lpthread
xiaosuo@gentux test $ ./a.out
xx
```
在 另一个终端，通过ps找到a.out的pid：
xiaosuo@gentux test $ ps aux | grep a.out
 xiaosuo 29882  0.0  0.0 14144   544 pts/6    Sl+ 16:23   0:00 ./a.out
 
看命名是否奏效：
 
xiaosuo@gentux test $ cd /proc/29882/task/
xiaosuo@gentux task $ ls
29882 29883
xiaosuo@gentux task $ cd 29883/
xiaosuo@gentux 29883 $ cat cmdline
./a.outxiaosuo@gentux 29883 $
 
有点儿郁闷，cmdline显示的竟然还是./a.out。通过 运行时打印的xx和strace检查prctl的返回值确认prctl确实成功运行。怀疑这个名字只能通过prctl获得，有点儿失落，可心仍不甘。查看 ps的man，并实验，终于找到了"xx":
xiaosuo@gentux 29883 $ ps -L -p 29882
  PID   LWP TTY          TIME CMD
 29882 29882 pts/6    00:00:00 a.oute

 29882 29883 pts/6    00:00:00 xx


Linux下进程重命名的方法：


使用系统函数prctl()，声明如下：


#include <sys/prctl.h>


int prctl(int option, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
具体用法请参考http://www.kernel.org/doc/man-pages/online/pages/man2/prctl.2.html


 进程重命名代码：


prctl(PR_SET_NAME, “process_name”, NULL, NULL, NULL);
第一个参数是操作类型，指定PR_SET_NAME，即设置进程名


第二个参数是进程名字符串，长度至多16字节
 

