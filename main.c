//log:当前进度：https://lotabout.me/2015/write-a-C-interpreter-2/
//log:当前时间: 20230204-1307


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <io.h>

int token;
char *src,*old_src;
int poolsize;
int line;

// eval():

int *text,
    *old_text,
    *stack;
char *data;

//virtual machine registers
int *pc, // 下一条要执行的计算机指令
    *bp, // 基址指针
    *sp, // 当前的栈顶
    ax, //一条指令执行后的结果
    cycle;

// instructions
enum { LEA ,IMM ,JMP ,CALL,JZ  ,JNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PUSH,
       OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,
       OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT };


void next(){
    token= *src++;
}

void expression(){
    //do nothing
}

void program(){
    next(); //get next token
    while ( token > 0 ){
        printf("token is %c\n",token);
        next();
    }
}

int eval() {
    int op, *tmp;
    while (1) {
        if (op == IMM)       {ax = *pc++;}                                     // load immediate value to ax
        else if (op == LC)   {ax = *(char *)ax;}                               // load character to ax, address in ax
        else if (op == LI)   {ax = *(int *)ax;}                                // load integer to ax, address in ax
        else if (op == SC)   {ax = *(char *)*sp++ = ax;}                       // save character to address, value in ax, address on stack
        else if (op == SI)   {*(int *)*sp++ = ax;}                             // save integer to address, value in ax, address on stack
    }


    return 0;
}


int main(int argc,char **argv){
    int i, fd;

    argc--;
    argv++;

    poolsize = 256*1024;
    line = 1;

    if ((fd=open(*argv,0))<0) {
        printf("Couldn't open (%s)\n", *argv);
        return -1;
    }
    if(!(src=old_src= malloc(poolsize))){
        printf("Couldn't malloc(%d) for source area\n",poolsize);
        return -1;
    }
    if((i=read(fd, src, poolsize-1))<=0){
        printf("read() return %d\n",i);
        return -1;
    }
    src[i]=0;
    close(fd);

    //allocate memory for virtual machine
    if(!(text=old_text=malloc(poolsize))){
        printf("Couldn't malloc(%d) for text area\n",poolsize);
        return -1;
    }
    if(!(data=malloc(poolsize))){
        printf("Couldn't malloc(%d) for data area\n",poolsize);
        return -1;
    }
    if(!(stack=malloc(poolsize))){
        printf("Couldn't malloc(%d) for stack\n",poolsize);
        return -1;
    }

    memset(text,0,poolsize);
    memset(data,0,poolsize);
    memset(stack,0,poolsize);

    bp = sp = (int*)((int)stack+poolsize);
    ax=0;


    program();
    return eval();
}
