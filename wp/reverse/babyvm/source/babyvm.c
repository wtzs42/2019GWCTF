#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
// flag: GWHT{Y0u_hav3_r3v3rs3_1t!}
char *vm_stack;
int len = 0;
char enc[] = {0x69, 0x45, 0x2a, 0x37, 0x9, 0x17, 0x6dc5, 0x5b0b, 0x705c, 0x72, 0x33, 0x76, 0x33, 0x21, 0x74, 0x31, 0x5f, 0x33, 0x73, 0x72};
char enc_flag[] = {0x46, 0x7a, 0x7b, 0x61, 0x4d, 0x7b, 0x61, 0x4d, 0x7c, 0x7d, 0x66, 0x4d, 0x74, 0x7e, 0x73, 0x75, 0x4d, 0x20, 0x21, 0x21};
enum opcodes
{
	MOV = 0xf1,
	XOR = 0xf2,
	RET = 0xf4,
	READ = 0xf5,
	ADD = 0xf6,
	MUL = 0xf7,
	EXCH = 0xf8,
};
unsigned char vm_code[] = {
	0xf5,
    0xf1,0xe1,0x0,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x20,0x00,0x00,0x00,
    0xf1,0xe1,0x1,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x21,0x00,0x00,0x00,
    0xf1,0xe1,0x2,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x22,0x00,0x00,0x00,
    0xf1,0xe1,0x3,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x23,0x00,0x00,0x00,
    0xf1,0xe1,0x4,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x24,0x00,0x00,0x00,
    0xf1,0xe1,0x5,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x25,0x00,0x00,0x00,
    0xf1,0xe1,0x6,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x26,0x00,0x00,0x00,
    0xf1,0xe1,0x7,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x27,0x00,0x00,0x00,
    0xf1,0xe1,0x8,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x28,0x00,0x00,0x00,
    0xf1,0xe1,0x9,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x29,0x00,0x00,0x00,
    0xf1,0xe1,0xa,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x2a,0x00,0x00,0x00,
    0xf1,0xe1,0xb,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x2b,0x00,0x00,0x00,
    0xf1,0xe1,0xc,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x2c,0x00,0x00,0x00,
    0xf1,0xe1,0xd,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x2d,0x00,0x00,0x00,
    0xf1,0xe1,0xe,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x2e,0x00,0x00,0x00,
    0xf1,0xe1,0xf,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x2f,0x00,0x00,0x00,
    0xf1,0xe1,0x10,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x30,0x00,0x00,0x00,
    0xf1,0xe1,0x11,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x31,0x00,0x00,0x00,
    0xf1,0xe1,0x12,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x32,0x00,0x00,0x00,
    0xf1,0xe1,0x13,0x00,0x00,0x00,0xf2,0xf1,0xe4,0x33,0x00,0x00,0x00,
    0xf4
};
enum regist
{
	R1 = 0xe1,
	R2 = 0xe2,
	R3 = 0xe3,
	R4 = 0xe5,
};

typedef struct 
{
	unsigned char opcode;
	void (*handle)(void *);
}vm_opcode;

typedef struct
{
	int r1;
	int r2;
	int r3;
	int r4;
	unsigned char *eip;
	vm_opcode op_list[7];
}vm_cpu;


unsigned char true_vm_code[] = {
	0xf5,//read(0,buf,0x20)
	0xf1,0xe1,0x0,0x0,0x0,0x0,//r1 = flag[0]
	0xf1,0xe2,0x1,0x0,0x0,0x0,//r2 = flag[1]
	0xf2,//r1 = 0x12^r1^r2
	0xf1,0xe4,0x0,0x0,0x0,0x0,//stack[0] = r1
	
	0xf1,0xe1,0x1,0x0,0x0,0x0,//r1 = flag[1]
	0xf1,0xe2,0x2,0x0,0x0,0x0,//r2 = flag[2]
	0xf2,//r1 = 0x12^r1^r2
	0xf1,0xe4,0x1,0x0,0x0,0x0,//stack[1] = r1
	
	0xf1,0xe1,0x2,0x0,0x0,0x0,//r1 = flag[2]
	0xf1,0xe2,0x3,0x0,0x0,0x0,//r2 = flag[3]
	0xf2,//r1 = 0x12^r1^r2
	0xf1,0xe4,0x2,0x0,0x0,0x0,//stack[2] = r1

	0xf1,0xe1,0x3,0x0,0x0,0x0,//r1 = flag[3]
	0xf1,0xe2,0x4,0x0,0x0,0x0,//r2 = flag[4]
	0xf2,//r1 = 0x12^r1^r2
	0xf1,0xe4,0x3,0x0,0x0,0x0,//stack[3] = r1

	0xf1,0xe1,0x4,0x0,0x0,0x0,//r1 = flag[4]
	0xf1,0xe2,0x5,0x0,0x0,0x0,//r2 = flag[5]
	0xf2,//r1 = 0x12^r1^r2
	0xf1,0xe4,0x4,0x0,0x0,0x0,//stack[4] = r1

	0xf1,0xe1,0x5,0x0,0x0,0x0,//r1 = flag[5]
	0xf1,0xe2,0x6,0x0,0x0,0x0,//r2 = flag[6]
	0xf2,//r1 = 0x12^r1^r2
	0xf1,0xe4,0x5,0x0,0x0,0x0,//stack[5] = r1

	0xf1,0xe1,0x6,0x0,0x0,0x0,//r1 = flag[6]
	0xf1,0xe2,0x7,0x0,0x0,0x0,//r2 = flag[7]
	0xf1,0xe3,0x8,0x0,0x0,0x0,//r3 = flag[8]
	0xf1,0xe5,0xC,0x0,0x0,0x0,//r4 = flag[12]
	0xf6, //r1 = (3*r1+2*r2+r3)
	0xf7, //r1 = r1*r4
	0xf1,0xe4,0x6,0x0,0x0,0x0,//stack[6] = r1

	0xf1,0xe1,0x7,0x0,0x0,0x0,//r1 = flag[7]
	0xf1,0xe2,0x8,0x0,0x0,0x0,//r2 = flag[8]
	0xf1,0xe3,0x9,0x0,0x0,0x0,//r3 = flag[9]
	0xf1,0xe5,0xC,0x0,0x0,0x0,//r4 = flag[12]
	0xf6, //r1 = (3*r1+2*r2+r3)
	0xf7, //r1 = r1*r4
	0xf1,0xe4,0x7,0x0,0x0,0x0,//stack[7] = r1

	0xf1,0xe1,0x8,0x0,0x0,0x0,//r1 = flag[8]
	0xf1,0xe2,0x9,0x0,0x0,0x0,//r2 = flag[9]
	0xf1,0xe3,0xA,0x0,0x0,0x0,//r3 = flag[10]
	0xf1,0xe5,0xC,0x0,0x0,0x0,//r4 = flag[12]
	0xf6, //r1 = (3*r1+2*r2+r3)
	0xf7, //r1 = r1*r4
	0xf1,0xe4,0x8,0x0,0x0,0x0,//stack[8] = r1

	0xf1,0xe1,0xD,0x0,0x0,0x0,//r1 = flag[13]
	0xf1,0xe2,0x13,0x0,0x0,0x0,//r2 = flag[19]
	0xf8,//r1 = r2,r2 = r1
	0xf1,0xe4,0xD,0x0,0x0,0x0,//stack[13] = r1
	0xf1,0xe7,0x13,0x0,0x0,0x0,//stack[19] = r2

	0xf1,0xe1,0xE,0x0,0x0,0x0,//r1 = flag[14]
	0xf1,0xe2,0x12,0x0,0x0,0x0,//r2 = flag[18]
	0xf8,//r1 = r2,r2 = r1
	0xf1,0xe4,0xE,0x0,0x0,0x0,//stack[14] = r1
	0xf1,0xe7,0x12,0x0,0x0,0x0,//stack[18] = r2

	0xf1,0xe1,0xF,0x0,0x0,0x0,//r1 = flag[15]
	0xf1,0xe2,0x11,0x0,0x0,0x0,//r2 = flag[17]
	0xf8,//r1 = r2,r2 = r1
	0xf1,0xe4,0xF,0x0,0x0,0x0,//stack[15] = r1
	0xf1,0xe7,0x11,0x0,0x0,0x0,//stack[17] = r2
	0xf4//ret
};

void mov(vm_cpu *cpu);
void add(vm_cpu *cpu);
void xor(vm_cpu *cpu);
void read_(vm_cpu *cpu);
void ret(vm_cpu *cpu);
void mul(vm_cpu *cpu);
void exch(vm_cpu *cpu);

void exch(vm_cpu *cpu)
{
	int temp1;
	temp1 = cpu->r1;
	cpu->r1 = cpu->r2;
	cpu->r2 = temp1; 
	cpu->eip += 1;
}

void ret(vm_cpu *cpu)
{
	cpu->eip += 1;
	return 0;
}
void add(vm_cpu *cpu)
{
	cpu->r1 = 3*(cpu->r1) + 2*(cpu->r2) + cpu->r3;
	cpu->eip += 1;
}
void mul(vm_cpu *cpu)
{
	cpu->r1 = cpu->r1*cpu->r4;
	cpu->eip += 1;
}

void xor(vm_cpu *cpu)
{
	int temp;
	temp = cpu->r1^cpu->r2;
	cpu->r1 = temp;
	cpu->eip += 1;
}

void read_(vm_cpu *cpu)
{
	char *dest = vm_stack;
	read(0,dest,0x20);
	len = strlen(dest);
	if(len!=21)
	{
		puts("WRONG!");
		exit(0);
	}
	cpu->eip += 1;
}

void mov(vm_cpu *cpu)
{
	 //mov指令的参数都隐藏在字节码中，指令表示后的一个字节是寄存器标识，第二到第五是要mov的数据在vm_stack上的偏移
	unsigned char *res = cpu->eip + 1;
	int *offset = (int *)(cpu->eip + 2);
	char *dest = 0;
	dest = vm_stack;
	switch(*res){
		
		case 0xe1:
			cpu->r1 = *(dest + *offset);
			break;
		case 0xe2:
			cpu->r2 = *(dest + *offset);
			break;
		case 0xe3:
			cpu->r3 = *(dest + *offset);
			break;
		case 0xe5:
			cpu->r4 = *(dest + *offset);
			break;
		case 0xe4:
			{
				int x;
				x = cpu->r1;
				*(dest + *offset) = x;
				break;
    		}
    	case 0xe7:
			{
				int y;
				y = cpu->r2;
				*(dest + *offset) = y;
				break;
    		}

	}
	cpu->eip += 6;//control by ourselves
}

void vm_init(vm_cpu *cpu)
{
	cpu->r1 = 0;
	cpu->r2 = 0x12;
	cpu->r3 = 0;
	cpu->r4 = 0;
	cpu->eip = (unsigned char *)vm_code;
	cpu->op_list[0].opcode = 0xf1;
	cpu->op_list[0].handle = (void (*)(void *))mov;

	cpu->op_list[1].opcode = 0xf2;
	cpu->op_list[1].handle = (void (*)(void *))xor;

	cpu->op_list[2].opcode = 0xf5;
	cpu->op_list[2].handle = (void (*)(void *))read_;

	cpu->op_list[3].opcode = 0xf4;
	cpu->op_list[3].handle = (void (*)(void *))ret;

	cpu->op_list[4].opcode = 0xf7;
	cpu->op_list[4].handle = (void (*)(void *))mul;

	cpu->op_list[5].opcode = 0xf8;
	cpu->op_list[5].handle = (void (*)(void *))exch;

	cpu->op_list[6].opcode = 0xf6;
	cpu->op_list[6].handle = (void (*)(void *))add;

	vm_stack = malloc(0x512);
	memset(vm_stack,0,0x512);
}

void vm_start(vm_cpu *cpu)
{
	cpu->eip = (unsigned char*)vm_code;
	while (*cpu->eip!=RET)
	{
		vm_dispatcher(cpu);
	}
}

void vm_dispatcher(vm_cpu *cpu)
{
	int i = 0;
	while (1)
	{
		if(*cpu->eip == cpu->op_list[i].opcode)
		{
			cpu->op_list[i].handle(cpu);
			break;
		}
		else
		{
			i += 1;
		}
	}	
}
void check2()
{
	int i;
	char *target = vm_stack;
	for(i = 0; i < len - 1; i++)
	{ 
		if((char)target[i] != enc[i])
		{
			exit(0);
		}
	}
}

void check()
{
	int i;
	char *target = vm_stack;
	for(i = 0; i < len - 1; i++)
	{ 
		if((char)target[i+0x20] != enc_flag[i])
		{
			puts("WRONG!");
			exit(0);
		}
	}
	puts("Congratulation?");
	puts("tips: input is the start");
}

int main(int argc, char const *argv[])
{
	vm_cpu *cpu = {0};
	puts("Please input something:");
	vm_init(&cpu);
	vm_start(&cpu);
	check();
	puts("And the flag is GWHT{true flag}");
	exit(0);
}
