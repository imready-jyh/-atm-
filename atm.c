#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>

//定义一个银行用户信息
typedef struct Person
{
	char name[18];              //用户名 
	char IDnum[19];             //身份证号 
	char password[7];           //密码 
	char accountnum[20];        //银行卡号 
	float money;                //余额
	struct Person* next;
}PERSON;

int id_check(char str[]);
int pw_check(char str[]);
void CreateAccount();
void Login();
void Login_success(PERSON* n);
void Deposit(PERSON* n);
void Withdraw(PERSON* n);
void Transfer(PERSON* n);
void Readbills(PERSON* p);
void ChangePass(PERSON* n);
void Exit();
void Load();
void SecretCover(char secret[]);
void Help();


PERSON* pHead;

int main()
{
	system("cls");
	Load();
	//设置颜色 
	system("color 2F");
	while(1)
	{
		printf("******************************************************\n");
		printf("**********欢迎使用中国人民银行自动存取款系统**********\n");
		printf("\n");
		printf("    ----------------------------------------------    \n");
		printf("    1.                开户                            \n");
		printf("    ----------------------------------------------    \n");
		printf("    2.                登录                            \n");
		printf("    ----------------------------------------------    \n");
		printf("    3.              系统帮助                          \n");
		printf("    ----------------------------------------------    \n");
		printf("    4.                退出                            \n");
		printf("    ----------------------------------------------    \n");
		printf("(业务办理完成后，请输入数字4安全退出)\n");
		
		char ch = getch();
		switch(ch)
		{
			case '1'://开户 
				CreateAccount();
				break;
			case '2'://登录 
				Login();
				break;
			case '3'://系统帮助 
				Help();
				break;
			case '4'://退出 
				Exit();
				exit(0);
		}
	}
	return 0;
}

//检验身份证号是否合法
int id_check(char str[])
{
	if (strlen(str) != 18) 
	{
		return 1;
	}
	int i;
	for (i = 0; i<18; i++)
	{
		if (str[i] < '0' || str[i] > '9')
		{
			return 1;
		}
	}
	return 0;
}

//检验密码是否合法 
int pw_check(char str[])
{
	if (strlen(str) != 6)
	{
		{
			return 1; //格式错误 
		}
		int i;
		for (i = 0; i<6; i++)
		{
			if (str[i] < '0' || str[i] > '9')
			{
				return 1; //格式错误 
			}
		}
	}
	return 0; 
} 

//开户 
void CreateAccount()
{
	system("cls");
	//一个新节点 
	PERSON* pNode = (PERSON*)malloc(sizeof(PERSON));
	pNode->next = NULL;
	PERSON* p = pHead;
	
	printf("请输入您的姓名:\n");
	scanf("%s", &pNode->name);
	
	char srIDnum[19];

	//规范身份证号格式 
	int error_num = 3; //三次容错机会 
	while(error_num >= 0)
	{
		printf("请输入您的身份证号:\n");
		scanf("%s", srIDnum);
		if (id_check(srIDnum) == 1)
		{
			printf("身份证号格式错误，你还有%d输入机会\n", error_num);
			error_num--;
		}
		else
		{
			break;
		}
	}
	// 容错机会用完，跳出 
	if (error_num < 0)
	{
		return;
	}

	//一个身份证号只能开一个户 
	while (p != NULL)
	{
		if (strcmp(srIDnum, p->IDnum) == 0)
		{
			printf("用户已存在！\n");
			return;
		}
		p = p->next;
	}
	strcpy(pNode->IDnum, srIDnum);
	
	//设置密码
	char secret[7]; 
	//规范密码格式 
	error_num = 3; //三次容错机会 
	while (error_num >= 0) 
	{
		printf("请设置您的银行卡密码（6位）:\n");
		SecretCover(secret);
		if (pw_check(secret) == 1)
		{
			printf("密码格式错误，你还有%d次输入机会\n", error_num);
			error_num--; 
		}
		else
		{
			break;
		}
	}
	// 容错机会用完，跳出 
	if (error_num < 0)
		return;
		
	char szPassword[7];
	error_num = 3; //三次容错机会
	while (error_num >= 0)
	{
		printf("请再次输入您的银行卡新密码:\n");
		SecretCover(szPassword);
		if (strcmp(szPassword, secret) != 0)
		{
			printf("两次输入的密码不一致,你还有%d次输入机会\n", error_num);
			error_num--;
		}
		else
		{
			break;
		} 
	}
	// 容错机会用完，跳出 
	if (error_num < 0)
	{
		return;
	}
	
	strcpy(pNode->password, szPassword);
	
	//生成银行卡账号 19位 xxxx xxxx xxxx xxxx xxx
	srand(time(NULL));
	sprintf(pNode->accountnum, "%04d%04d%04d%04d%03d", rand()%10000, rand()%10000, rand()%10000, rand()%10000, rand()%1000);
	
	//余额 
	pNode->money = 0.00;
	
	if (pHead == NULL)
	{
		pHead = pNode;
	}
	else
	{
		pNode->next = pHead;
		pHead = pNode;
	}
	
	printf("您的账户信息如下:\n");
	printf("\t姓名:%s\n", pNode->name);
	printf("\t身份证号:%s\n", pNode->IDnum);
	printf("\t账号:%s\n", pNode->accountnum);
	printf("\t余额:%.2f\n", pNode->money);
	
	//创建一个个人数据库文件，用于储存业务信息 
	char filename[20];
	sprintf(filename, "%s.txt", pNode->name);
	FILE *file = fopen(filename, "w");
	fclose(file);
}

//登录 
void Login()
{
	system("cls");
	int i;
	char srAccountnum[20], srPassword[7];
	printf("请输入您的账号:\n");
	scanf("%s", srAccountnum);
	PERSON* p = pHead;
	while(p != NULL)
	{
		if (strcmp(srAccountnum, p->accountnum) == 0)
		{
			break;
		}
		if (p->next == NULL)
		{
			printf("账号输入错误！\n");
			return;
		}
		p = p->next;
	}
	printf("请输入您的密码:\n");
	SecretCover(srPassword);
	if (strcmp(srPassword, p->password) == 0)
	{
		printf("登录成功！\n");
		Login_success(p);
	}
	else
	{
		printf("密码输入错误！\n");
		return;
	}
}

//可多次办理业务，不用一直重新登录 
void Login_success(PERSON* p)
{
		printf("尊敬的用户%s，您好\n", p->name);
		printf("请选择您要办理的业务:\n");
		printf("\n");
		printf("    ----------------------------------------------    \n");
		printf("    1.                存款                            \n");
		printf("    ----------------------------------------------    \n");
		printf("    2.                取款                            \n");
		printf("    ----------------------------------------------    \n");
		printf("    3.                转账                            \n");
		printf("    ----------------------------------------------    \n");
		printf("    4.            查询业务明细                        \n");
		printf("    ----------------------------------------------    \n");
		printf("    5.              修改密码                          \n");
		printf("    ----------------------------------------------    \n");
		printf("    6.                退出                            \n");
		printf("    ----------------------------------------------    \n");
		
		char ch = getch();
		switch(ch)
		{
			case '1'://存款 
				Deposit(p);
				break;
			case '2'://取款 
				Withdraw(p);
				break;
			case '3'://转账 
				Transfer(p);
				break;
			case '4'://查询业务明细 
				Readbills(p); 
				break;
			case '5'://修改密码 
				ChangePass(p);
				break;
			case '6'://退出 
				break;
		}
}

//存款 
void Deposit(PERSON* p)
{
	system("cls");
	float deposit;
	printf("您目前的余额为%.2f元\n", p->money);
	printf("请输入您要存款的金额:\n");
	scanf("%f", &deposit);
	p->money = p->money + deposit;
	printf("存款成功，您当前的余额为%.2f元\n", p->money);
	
	// 获取当前系统时间
    time_t currentTime;
    struct tm *timeInfo;

    time(&currentTime);
    timeInfo = localtime(&currentTime);

    // 格式化系统时间
    char timeString[20];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeInfo);
    
    // 将办理的业务写入个人数据库 
    char filename[20];
	sprintf(filename, "%s.txt", p->name);
	FILE *file = fopen(filename, "a");
	fprintf(file, "%s\n", timeString);
	fprintf(file, "%s存入%.2f元，当前余额为%.2f元\n", p->name, deposit, p->money);
	fprintf(file, "\n");
	fclose(file);
	
	Login_success(p);
}

//取款 
void Withdraw(PERSON* p)
{
	system("cls");
	float withdraw;
	printf("您目前的余额为%.2f元\n", p->money);
	printf("请输入您要取款的金额:\n");
	scanf("%f", &withdraw);
	if (withdraw <= p->money)
	{
		p->money = p->money - withdraw;
		printf("取款成功，您当前的余额为%.2f元\n", p->money);
		
		// 获取当前系统时间
	    time_t currentTime;
	    struct tm *timeInfo;
	
	    time(&currentTime);
	    timeInfo = localtime(&currentTime);
	
	    // 格式化系统时间
	    char timeString[20];
	    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeInfo);
		
		// 将办理的业务写入个人数据库 
	    char filename[20];
		sprintf(filename, "%s.txt", p->name);
		FILE *file = fopen(filename, "a");
		fprintf(file, "%s\n", timeString);
		fprintf(file, "%s取出%.2f元，当前余额为%.2f元\n", p->name, withdraw, p->money);
		fprintf(file, "\n");
		fclose(file);
		
		Login_success(p);
	}
	else
	{
		printf("余额不足\n");
		Login_success(p);
	}
}

//转账 
void Transfer(PERSON* p)
{
	system("cls");
	float transfer;
	char srAccountnum[24];
	printf("您目前的余额为%.2f元\n", p->money);
	printf("请输入您想转入的账户:\n");
	scanf("%s", srAccountnum);
	PERSON* p_target = pHead;
	while (p_target != NULL)
	{
		if (strcmp(srAccountnum, p_target->accountnum) == 0)
		{
			break;
		}
		if (p_target->next == NULL)
		{
			printf("账号输入错误！\n");
			Login_success(p);
		}
		p_target = p_target->next;
	}
	printf("请输入您要转入的金额:\n");
	scanf("%f", &transfer);
	if (transfer <= p->money)
	{
		p->money = p->money - transfer;
		p_target->money = p_target->money + transfer;
		printf("转账成功，您当前的余额为%.2f元\n", p->money);
		
		// 获取当前系统时间
	    time_t currentTime;
	    struct tm *timeInfo;
	
	    time(&currentTime);
	    timeInfo = localtime(&currentTime);
	
	    // 格式化系统时间
	    char timeString[20];
	    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeInfo);
		
		// 将办理的业务写入个人数据库 
	    char filename[20];
		sprintf(filename, "%s.txt", p->name);
		FILE *file = fopen(filename, "a");
		fprintf(file, "%s\n", timeString);
		fprintf(file, "%s向%s转出%.2f元，您当前的余额为%.2f元\n", p->name, p_target->name, transfer, p->money);
		fprintf(file, "\n");
		fclose(file);
		
		char filename1[20];
		sprintf(filename1, "%s.txt", p_target->name);
		FILE *file1 = fopen(filename1, "a");
		fprintf(file1, "%s\n", timeString);
		fprintf(file1, "%s向%s转入%.2f元，您当前的余额为%.2f元\n", p->name, p_target->name, transfer, p_target->money);
		fprintf(file1, "\n");
		fclose(file1);
		
		Login_success(p);
	}
	else
	{
		printf("余额不足\n");
		Login_success(p);
	}	
}

//查询业务明细 
void Readbills(PERSON* p)
{
	system("cls");
	
	char filename[20];
	sprintf(filename, "%s.txt", p->name);
	FILE *file = fopen(filename, "r");
	// 逐行读取并输出文件内容
    char line[1000];
    while (fgets(line, sizeof(line), file) != NULL) 
	{
        printf("%s", line);
    }
	fclose(file);
	
	Login_success(p);
	
}

//修改密码 
void ChangePass(PERSON* p)
{
	system("cls");
	char oldpass[7], newpass1[7], newpass2[7];
	printf("请输入您的旧密码:\n");
	SecretCover(oldpass);
	if (strcmp(oldpass, p->password) == 0)
	{
		printf("请输入您的新密码:\n");
		SecretCover(newpass1);
		printf("请再次输入您的新密码:\n");
		SecretCover(newpass2);
		if (strcmp(newpass1, newpass2) == 0)
		{
			strcpy(p->password, newpass1);
			printf("密码修改成功！\n");
			
			// 获取当前系统时间
	    	time_t currentTime;
	    	struct tm *timeInfo;
	
	    	time(&currentTime);
	    	timeInfo = localtime(&currentTime);
	
	    	// 格式化系统时间
	    	char timeString[20];
	    	strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeInfo);
	    	
	    	// 将办理的业务写入个人数据库 
	    	char filename[20];
			sprintf(filename, "%s.txt", p->name);
			FILE *file = fopen(filename, "a");
			fprintf(file, "%s\n", timeString);
			fprintf(file, "%s修改了密码\n", p->name);
			fprintf(file, "\n");
			fclose(file);
		
			Login_success(p);
		}
		else
		{
			printf("两次输入密码不一致！\n");
			Login_success(p);
		}
	}
	else
	{
		printf("密码输入错误！\n");
		Login_success(p);
	}
}

void Exit()
{
	// save Person list
	FILE* fp = fopen("person.txt", "w");	//将更新的数据写入记事本中 
	if (fp == NULL)
	{
		printf("读取错误\n");
		exit(0);
	}
	PERSON* p = pHead;
	while (p != NULL)
	{
		//将数据写入person.txt中 
		fprintf(fp, "%s %s %s %s %.2f\n", p->name, p->IDnum, p->password, p->accountnum, p->money);
		p = p->next;
	}
	fclose(fp);
}

void Load()
{
	// load Person list
	FILE* fp = fopen("person.txt", "r");	//打开并读取记事本，指针fp指向txt文件地址 
	if (fp == NULL)
	{
		printf("找不到文件，读取错误\n");
		exit(0);
	}
	while (!feof(fp))		//流上文件未结束时 
	{
		//申请大小等于PERSON的内存
		PERSON* pNode = (PERSON*)malloc(sizeof(PERSON));
		
		//从输入流中读入数据，并将其返回值存入ret变量中 
		int ret = fscanf(fp, "%s %s %s %s %f\n", &(pNode->name), &(pNode->IDnum), &(pNode->password), &(pNode->accountnum), &(pNode->money));
		if (ret != 5)
		{
			continue;	//如果读入数据不完整，将直接开始下一次循环重新读入数据 
		}
		pNode->next = pHead;
		pHead = pNode;
	}
	fclose(fp);
}

//隐藏密码
void SecretCover(char secret[])
{
	int i = 0;
	char c;
	while (1) 
	{
    	c = getch();
    	if (c == '\r') //回车表示密码输入结束 
		{
        	break;
    	}
    	else if (c == '\b') //遇到退格删除前一个* 
		{
        	printf("\b \b"); //用空格覆盖掉* 
        	i--;
    	}
    	else
		{
        	secret[i] = c; //将字符存入数组 
        	i++;
        	printf("*");
    	}
	}
	printf("\n");
}

//系统帮助
void Help()
{
	system("cls");
	printf("*******************银行系统帮助*******************\n");
	printf("**************************************************\n");
	printf("开户: 一个身份证号只能开通一个账户\n");
	printf("登录: 需输入您的银行卡号，并通过密码进行身份认证\n");
	printf("查询业务明细: 查看您在本银行办理的业务信息\n");
	printf("存款: 客户存入现金，系统通过现金数额存入余额\n");
	printf("取款: 客户取出现金，系统通过现金数额取出余额\n");
	printf("转账: 客户将输入的待转金额转入他人账户中\n");
	printf("查询业务明细: 查看您在本银行办理的业务信息\n");
	printf("修改密码: 设置一个新密码来替换旧密码\n");
}
