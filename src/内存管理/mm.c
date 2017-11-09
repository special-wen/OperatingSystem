#include<stdio.h>
#include<stdlib.h>

#define PROCESS_NAME_LEN 32    //进程名长度
#define MIN_SLICE 10           //最小碎片的大小
#define DEFAULT_MEM_SIZE 1024  //内存大小
#define DEFAULT_MEM_START 0    //起始位置

//内存分配算法
#define MA_FF 1
#define MA_BF 2
#define MA_WF 3
int mem_size = DEFAULT_MEM_SIZE; //内存大小
int ma_algorithm = MA_FF;        //当前分配算法
static int pid = 0; 			//初始pid
int flag = 0;					//设置内存大小标志

//初始化空闲块，默认为一块，可以指定大小及起始地址
struct free_block_type* init_free_block(int mem_size);
//显示菜单
void display_menu();
//设置内存大小
int set_mem_size();
//设置当前的分配算法
void set_algorithm();
//按指令的算法整理内存空闲块链表
void rearrange(int algorithm);
//按FF算法重新整理内存空闲块链表
void rearrange_FF();
//按BF算法重新整理内存空闲块链表
void rearrange_BF();
//按WF算法重新整理内存空闲块链表
void rearrange_WF();
//创建新的进程
int new_process();
//分配内存模块
int allocate_mem(struct allocated_block *ab);
//删除进程
void kill_process();
//将ab所表示的已分配区归还，并进行可能的合并
int free_mem(struct allocated_block *ab);
//释放ab数据结构节点
int dispose(struct allocated_block *free_ab);
//显示当前的内存的使用
int display_menu_usage();

//描述每一个空闲块的数据结构
struct free_block_type{
	int size;
	int start_addr;
	struct free_block_type *next;
};
//指向空闲块链表的首指针
struct free_block_type *free_block;

struct allocated_block{
	int pid;
	int size;
	int start_addr;
	char process_name[PROCESS_NAME_LEN];
	struct allocated_block *next;
};
//进程分配内存块链表的首指针
struct allocated_block *allocated_block_head = NULL;

int main(void){
	char choice;
	pid = 0;
	//初始化空闲区
	free_block = init_free_block(mem_size);
	while(1){
		display_menu();   //显示菜单
		fflush(stdin);
		choice = getchar(); //获取用户输入
		switch(choice){
			//设置内存大小
			case '1':set_mem_size(); break;
			//设置算法
			case '2':set_algorithm();flag = 1;break;
			//创建新的进程
			case '3':new_process();flag = 1;break;
			//删除进程
			case '4':kill_process();flag = 1;break;
			//显示内存使用
			case '5':display_menu_usage();flag = 1;break;
			//释放链表并退出
			case '0':do_exit();exit(0);
			default:break;	
		}
	} 

}
//初始化空闲块，默认为一块，可指定大小及起始地址
struct free_block_type* init_free_block(int mem_size){
	struct free_block_type *fb;
	fb = (struct free_block_type*)malloc(sizeof(struct free_block_type));
	if(fb == NULL){
		printf("No mem\n");
		return NULL;
	}
	fb->size = mem_size;
	fb->start_addr = DEFAULT_MEM_START;
	return fb;
}

//显示菜单
void display_menu(){
	printf("\n");
	printf("1 - Set memory size (default = %d)\n",DEFAULT_MEM_SIZE);
	printf("2 - Seclect memory allocation algorithm\n");
	printf("3 - New process \n");
	printf("4 - Terminate a process \n");
	printf("5 - Display memory usage \n");
	printf("0 - Exit\n");
}

//设置内存的大小
int set_mem_size(){
	int size;
	if(flag != 0){
		//防止重复设置
		printf("Cannot set memory size again\n");
		return 0;
	}
	printf("Total memory size = ");
	scanf("%d",&size);
	if(size > 0){
		mem_size = size;
		free_block->size = mem_size;
	}
	flag = 1;
	return 1;
}

//设置当前的分配算法
void set_algorithm(){
	int algorithm;
	printf("\t1 - First Fit\n");
	printf("\t2 - Best Fit \n");
	printf("\t3 - Worst Fit \n");
	scanf("%d",&algorithm);
	if(algorithm>=1 && algorithm <= 3)
		ma_algorithm = algorithm;
	//按指定算法重新排列空闲去链表
	rearrange(ma_algorithm);
}

//按指定的算法整理内存空闲块链表
void rearrange(int algorithm){
	switch(algorithm){
		case MA_FF:rearrange_FF();break;
		case MA_BF:rearrange_BF();break;
		case MA_WF:rearrange_WF();break;
	}
}

//按FF算法重新整理内存空闲块链表
void rearrange_FF(){
	//请自行补充
	//m代表开头，p，q分别表示交换的两个节点，n代表交换截止位置
	struct free_block_type *p,*q,*m,*n;
	if(free_block->next->next != NULL){
		n = NULL;
		while(n != free_block->next->next){
			m = free_block;
			p = free_block->next;
			q = p->next;
			while(q != n){
				if(p->start_addr > q->start_addr){
					p->next = q->next;
					q->next = p;
					m->next = q;
				}
				m = m->next;
				p = m->next;
				q = p->next;
			}
			n = p;
		}
	}
}
//按BF算法重新整理内存空闲块链表
void rearrange_BF(){
	//请自行补充
	//m代表开头，p，q分别代表交换的两个节点，n代表交换截止位置
	struct free_block_type *p,*q,*m,*n;
	if(free_block->next->next != NULL){
		n = NULL;
		while(n != free_block->next->next){
			m = free_block;
			p = free_block->next;
			q = p->next;
			while(q != n){
				if(p->size > q->size){
					p->next = q->next;
					q->next = p;
					m->next = q;
				}
				m = m->next;
				p = m->next;
				q = p->next;
			}
			n = p;
		}
	}
}
//按WF算法重新整理内存空闲块链表
void rearrange_WF(){
	//请自行补充
	//m代表开头，p，q分别表示交换的两个节点，n代表交换截止位置
	struct free_block_type *p,*q,*m,*n;
	if(free_block->next->next != NULL){
		n = NULL;
		while(n != free_block->next->next){
			m = free_block;
			p = free_block->next;
			q = p->next;
			while(q != n){
				if(p->size < q->size){
					p->next = q->next;
					q->next = p;
					m->next = q;
				}
				m = m->next;
				p = m->next;
				q = p->next;
			}
			n = p;
		}
	}
}
//创建新的进程，主要是获取内存的申请数量
int new_process(){
	struct allocated_block *ab;
	int size;int ret;
	ab = (struct allocated_block*)malloc(sizeof(struct allocated_block));
	if(!ab) exit(-5);
	ab->next = NULL;
	pid++;
	sprintf(ab->process_name,"PROCESS-%02d",pid);
	ab->pid = pid;
	printf("Memory for %s:",ab->process_name);
	scanf("%d",&size);
	if(size>0) ab->size = size;
	ret = allocate_mem(ab);  //从空闲区分配内存，ret == 1 表示分配ok
	//如果此时的allocated_block_head尚未赋值，则赋值
	if((ret == 1) && (allocated_block_head == NULL)){
		allocated_block_head = ab;
		return 1;
	}
	//分配成功，将该已分配块的描述插入已分配链表
	else if(ret == 1){
		ab->next = allocated_block_head;
		allocated_block_head = ab;
		return 2; 
	}
	//分配不成功
	else if(ret == -1){
		printf("Allocation fail\n");
		free(ab);
		return -1;
	}
	return 3;
}

//分配内存模块
int allocate_mem(struct allocated_block *ab){  
    struct free_block_type *p, *q, *temp;  
    int sum = 0;  
    if (free_block->next == NULL)  {  
        printf("no  memory");  
        return;  
    }  
    //当前算法进行整理内存  
    rearrange(ma_algorithm);  
    //根据当前算法在空闲分区链表中搜索合适空闲分区进行分配，分配时注意以下情况：  
    // 1. 找到可满足空闲分区且分配后剩余空间足够大，则分割  
    // 2. 找到可满足空闲分区且但分配后剩余空间（最小碎片化）比较小，则一起分配  
    // 3. 找不可满足需要的空闲分区但空闲分区之和能满足需要，则采用内存紧缩技术，进行空闲分区的合并，然后再分配  
    // 4. 在成功分配内存后，应保持空闲分区按照相应算法有序  
    // 5. 分配成功则返回1，否则返回-1  
  
    //如果首次适应 和最佳适应差不多 从链首开始查找，如果找到，则开始分配，如果没有找到，采用分区紧缩技术，如果可以分配，则分配，不能分配，返回-1  
    if (ma_algorithm == MA_FF || ma_algorithm == MA_BF){  
        //开始寻找符合条件的节点  
        for (p = free_block, q = p->next; q && q->size < ab->size; p = p->next, q = p->next);  
        //找到  
        if (q){  
            //判断是否达到最小碎片程度  
            if ((q->size - ab->size) <= MIN_SLICE){  
                //全部分配  
                ab->start_addr = q->start_addr;  
                ab->size = q->size;  
                //释放q节点  
                p->next = q->next;  
                free(q);  
            }  
            else{  
                ab->start_addr = q->start_addr;  
                q->start_addr = q->start_addr + ab->size;  
                q->size = q->size - ab->size;  
            }  
        }  
        //开始内存紧缩  
        else{  
            for (q = free_block->next; q; q = q->next){  
                sum += q->size;  
                if (sum >= ab->size){  
                    break;  
                }  
            }  
            if (q){  
                //第一个空闲分区大小更改为sum，并且除了第一个，其他结点全部删除      
                free_block->next->size = sum;  
                p = free_block->next;  
                for (temp = p->next; temp != q; temp = p->next){  
                    p->next = temp->next;  
                    free(temp);  
                }  
                p->next = q->next;  
                free(q);  
  
                p->start_addr = p->start_addr + ab->size;  
                p->size = p->size - ab->size;  
                //如果剩余空间为0 则直接释放当前的空间  
            }  
            else{  
                return -1;  
            }  
        }  
    }  
    //最差适应  
    else if (ma_algorithm == MA_WF){  
        //只需要找到第一个最大的空闲分区  如果满足，则分配，不满足则判断能否进行合并，如果可以，分配  
        if (free_block->next->size >= ab->size){  
            if (free_block->next->size - ab->size >= 10){  
                ab->start_addr = free_block->next->start_addr;  
                free_block->next->start_addr += ab->size;  
                free_block->next->size -= ab->size;  
            }  
            else{  
                p = free_block->next;  
                free_block->next = p->next;  
                free(p);  
            }  
        }  
        else{  
            for (q = free_block->next; q; q = q->next){  
                sum += q->size;  
                if (sum >= ab->size){  
                    break;  
                }  
            }
            if (q){  
                //第一个空闲分区大小更改为sum，并且除了第一个，其他结点全部删除      
                free_block->next->size = sum;  
                p = free_block->next;  
                for (temp = p->next; temp != q; temp = p->next){  
                    p->next = temp->next;  
                    free(temp);  
                }  
                p->next = q->next;  
                free(q);  
                ab->start_addr = free_block->next->start_addr;  
                p->start_addr = p->start_addr + ab->size;  
                p->size = p->size - ab->size;  
                //如果剩余空间为0 则直接释放当前的空间  
            }  
            else{  
                return -1;  
            }  
        }  
    }
    return 1;  
}  

//删除进程，归还分配的存储空间，并删除改进程内存分配的节点
void kill_process(){
	struct allocated_block *ab;
	int pid;
	printf("Kill Process,pid = ");
	scanf("%d",&pid);
	ab = find_process(pid);
	if(ab != NULL){
		//释放ab所表示的分配区
		free_mem(ab);
		//释放ab数据结构节点
		dispose(ab);
	}
}

//将ab所表达的已分配区归还，并进行可能的合并
int free_mem(struct allocated_block *ab){
	int algorithm = ma_algorithm;
	struct free_block_type *fbt,*pre,*work;
	fbt = (struct free_block_type*)malloc(sizeof(struct free_block_type));
	if(!fbt) return -1;
	//进行可能的合并，基本策略如下
	//1.将新释放的节点插入到空闲分区队列末尾
	//2.对空闲链表按照地址有序排列
	//3.检查并合并相邻的空闲分区
	//4.将空闲链表重新按照当前算法排序
	//请自行补充
	fbt->next = free_block->next;
	free_block->next = fbt;
	fbt->size = ab->size;
	fbt->start_addr = ab->start_addr;
	return 1;
}

//释放ab数据结构节点
int dispose(struct allocated_block *free_ab){
	struct allocated_block *pre,*ab;
	//如果要释放第一个节点
	if(free_ab == allocated_block_head){
		allocated_block_head = allocated_block_head->next;
		free(free_ab);
		return 1;
	}
	pre = allocated_block_head;
	ab = allocated_block_head->next;
	while(ab != free_ab){
		pre = ab;
		ab = ab->next;
	}
	pre->next = ab->next;
	free(ab);		
	return 2;
}

//显示当前内存的使用情况，包括空闲去的情况和已经分配的情况
int display_menu_usage(){
	struct free_block_type *fbt = free_block;
	struct allocated_block *ab = allocated_block_head;
	if(fbt == NULL)
		return -1;
	printf("--------------------------\n");
	//显示空闲区
	printf("Free Memory:\n");
	printf("%20s %20s\n", "start_addr","size");
	while(fbt != NULL){
		printf("%20d %20d\n",fbt->start_addr,fbt->size);
		fbt = fbt->next;
	}
	//显示已分配区
	printf("\nUsed Memory:\n");
	printf("%10s %20s %10s %10s\n", "PID","ProcessName","start_addr","size");
	while(ab != NULL){
		printf("%10d %20s %10d %10d\n", ab->pid,ab->process_name,ab->start_addr,ab->size);
		ab = ab->next; 
	}
	printf("---------------------------\n");
	return 0;
}

