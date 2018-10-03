#include <stdio.h> 
#include <stdlib.h> 
#include<time.h>
#define CREATE 1
#define UPDATE 2
#define GET_ACC 3
#define Deposit 1
#define Withdraw 2
#define Balance_Enquiry 3
#define Password_Change 4
#define View_details 5
#define Exit 6
#define ADD 1
#define DELETE 2
#define MODIFY 3
#define SEARCH 4
#define ADMIN_EXIT 5
long long int generateAccountNo();
struct sequence{
	int s_no;
};
struct user_info{
	char name_1[20];
	char name_2[20];
	long long int u_id;
	char password[10];
	char city[10];
	char state[10];
	char country[10];
	long long int pin_code;
	long long int mobile_no;
	char email[50];
	char user_type;//n-normal user,a-admin user
};
struct account_info{
	long long int account_no;
	double balance;
	char account_type;//c-current,s-saveing,o-other
	int s_no;
	struct user_info user;
	int status;
};
struct data_packet{
	int action;
	struct account_info account;
	int result;
//	char *result_msg;
};
int addAccount(struct account_info account);
int openAccount(struct account_info account){
	int result1=addAccount(account);
	return (result1);
}
void printAccountDetails(struct account_info account){
	printf("**************** %s ******************\n",account.user.name_1);
	printf("Account_No\t :: %lld\nUser_Id\t\t :: %lld\nPassword\t :: %s\nEmail\t\t :: %s\n",account.account_no,account.user.u_id,account.user.password,account.user.email);
	if(account.account_type=='j'){printf("Name\t\t :: %s\nName\t\t :: %s\n",account.user.name_1,account.user.name_2);}
	else{printf("Name\t\t :: %s\n",account.user.name_1);}
	printf("City \t\t :: %s\nState \t\t :: %s\nCountry \t :: %s\nMobile no \t :: %lld\nPin code \t :: %lld\nSequence No \t :: %d\nBalance \t :: %lf\nStatus \t\t :: %d\n",account.user.city,account.user.state,account.user.country,account.user.mobile_no,account.user.pin_code,account.s_no,account.balance,account.status);
	

}
int addAccount(struct account_info account){
	int fd;
	fd=open("account",O_WRONLY|O_APPEND);
	//printf("opening db status %d \n",fd);
	ssize_t s=write(fd,&account,sizeof(account));
	close(fd);
	if(s>0){printf("Write Success\n");return 1;}
	else return -1;
	}
/*
int addUser(struct user_info user){
	fd=open("user",O_WRONLY|O_APPEND);
        printf("opening db status %d \n",fd);
        ssize_t s=write(fd,&user,sizeof(user));
	close(fd);
	if(s>0){printf("Write Success\n");return 1;}
        else return -1;
}*/
int readSequence(){
	int fd;
	size_t s;
	fd=open("sequence",O_RDONLY);
	struct sequence ds;
	struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;
	lock.l_pid=getpid();
//	printf("File Discripter value :: %d, Process id %ld\n",fd,(long)getpid());
//	printf("Before Entering To Critical Section\n");
	int i=fcntl(fd,F_SETLKW,&lock);
	if(i!=-1){
		//printf("Locking Status :: %d",i);
		//printf("Entering Critical Section\n");
		/*CS Start*/
		s=read(fd,&ds,sizeof(ds));
		//printf("CS :: Initial Ticket No :: %d\n Press <Enter> To Exit..\n",ds.ticketNo);
		lock.l_type=F_UNLCK;
		int u=fcntl(fd,F_SETLK,&lock);
//		printf("Exiting CS... Unlocking Status :: %d",u);
		/*CS End*/
		}
		close(fd);
		if(s>0)return ds.s_no;
		else return -1;
}
int getSquence(struct sequence ds){
	int fd;
	size_t s;
	fd=open("sequence",O_WRONLY);
//	struct sequence ds;
	struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;
	lock.l_pid=getpid();
//	printf("File Discripter value :: %d, Process id %ld\n",fd,(long)getpid());
//	printf("Before Entering To Critical Section\n");
	int i=fcntl(fd,F_SETLKW,&lock);
	if(i!=-1){
		//printf("Locking Status :: %d",i);
		//printf("Entering Critical Section\n");
		/*CS Start*/
		
		//printf("CS :: Initial Ticket No :: %d\n",ds.ticketNo);
		//ds.ticketNo++;
		//fd=open("sequence",O_WRONLY);
		//lseek(fd,0,SEEK_CUR);
		s=write(fd,&ds,sizeof(ds));
		//close(fd);
		//printf("CS :: Ticket Booked!! Ticket No :: %d\nPress Enter To Exit\n",ds.ticketNo);
		//getchar();
		lock.l_type=F_UNLCK;
		int u=fcntl(fd,F_SETLK,&lock);
		//printf("Exiting CS... Unlocking Status :: %d",u);
		/*CS End*/
		close(fd);
		}
		if(s>0)return ds.s_no;
		else return -1;
}
struct account_info getDetails(int seq_no){
		struct account_info account;
		printf("****************** Enter User Details ****************");
		account.account_no=generateAccountNo();
		account.s_no=seq_no;
		account.status=1;//active
		struct user_info user;
		 while(1){
                 printf("Enter Account Type\nc - Current Account\ns - Saving Account\nj - Joint Account\n");
                 scanf("%c",&account.account_type);
                 if(account.account_type=='c'||account.account_type=='s'||account.account_type=='j')break;
                 }
		if(account.account_type=='j'){
		printf("Enter User Name\n");
		scanf(" %20[^\n]",user.name_1);
		printf("Enter Second User Name\n");
		scanf(" %20[^\n]",user.name_2);
		}else{
		printf("Enter User Name\n");
		scanf(" %20[^\n]",user.name_1);
		}
		user.u_id=account.account_no;
		printf("Enter Password\n");
		scanf(" %10[^\n]",user.password);
		/*while(1){
		printf("Enter Account Type\nc - Current Account\ns - Saving Account\nj - Joint Account\n");
		scanf("%c",&account.account_type);
		if(account.account_type=='c'||account.account_type=='s'||account.account_type=='j')break;
		}*/
		printf("Enter City\n");
		scanf(" %10[^\n]",user.city);
		printf("Enter State\n");
		scanf(" %10[^\n]",user.state);
		printf("Enter Country\n");
		scanf(" %10[^\n]",user.country);
		printf("Enter Pin Code\n");
		scanf("%lld",&user.pin_code);
		printf("Enter Mobile No\n");
		scanf("%lld",&user.mobile_no);
		printf("Enter Email Id\n");
		scanf(" %50[^\n]",user.email);
		while(1){
		printf("Enter User Type\na - Admin\nn - Normal User\n");
		scanf("%c",&user.user_type);
		if(user.user_type=='a'||user.user_type=='n')break;
		}
		account.user=user;
		return account;
}
long long int generateAccountNo(){
	srand(time(0));
 	long long int random=rand()%10000000;
	return random;
}
void print_all(){
	int fd=fd=open("account",O_RDONLY);
	struct account_info account;
	while(read(fd,&account,sizeof(account))){
		printAccountDetails(account);
	}
	close(fd);
}
struct account_info searchByAccountNo(long long int account_no){	
	int fd=fd=open("account",O_RDONLY);
	struct account_info account={0},acc={0};
	while(read(fd,&account,sizeof(account))){
		if(account.account_no==account_no&&account.status>0){
		acc=account;
		break;}
	}
	close(fd);
	return acc;
}
int getRecordNoByAccountNo(long long int account_no){	
	int fd=fd=open("account",O_RDONLY);
	struct account_info account={0},acc={0};
	int record_no=-1,i=-1;
	while(read(fd,&account,sizeof(account))){
		i++;
		if(account.account_no==account_no){
		//acc=account;
		record_no=i;
		break;}
	}
	close(fd);
	return record_no;
}
int updateRecord(struct account_info account,int record_no){
	int fd;
	fd=open("account",O_WRONLY);
	struct flock lock;
	size_t s=0;
	off_t off=lseek(fd,(record_no*192L),SEEK_SET);
	/*printf("Seek Return :: %d\n",(int)off);
	printf("%d\n",(int)SEEK_SET);
	printf("%d\n",(int)SEEK_CUR);*/
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_CUR;
	lock.l_start=0;
	lock.l_len=192;
	lock.l_pid=getpid();
	printf("File Discripter value :: %d, Process id %ld\n",fd,(long)getpid());
	printf("Before Entering To Critical Section\n");
	int i=fcntl(fd,F_SETLKW,&lock);
	if(i!=-1){
		printf("Locking Status :: %d\n",i);
		printf("Entering Critical Section\n");
		/*CS Start*/
		//s=write(fd,&account,sizeof(struct account_info));
		//getchar();
		/*For testing locking*/
		/*char ch;
		printf("Enter (y,Y) To Confirm\n");
		scanf(" %c", &ch);*/
		s=write(fd,&account,sizeof(struct account_info));
		lock.l_type=F_UNLCK;
		int u=fcntl(fd,F_SETLK,&lock);
		printf("Exiting CS... Unlocking Status :: %d\n",u);
		/*CS End*/
		}else{
		printf("Lock Not Accquired");
		}
		close(fd);
		return s;
}

struct account_info getRecord(int record_no){
	int fd;
	struct account_info account;
	fd=open("account",O_RDONLY);
	struct flock lock;
	size_t s=0;
	off_t off=lseek(fd,(record_no*192L),SEEK_SET);
	/*printf("Seek Return :: %d\n",(int)off);
	printf("%d\n",(int)SEEK_SET);
	printf("%d\n",(int)SEEK_CUR);*/
	lock.l_type=F_RDLCK;
	lock.l_whence=SEEK_CUR;
	lock.l_start=0;
	lock.l_len=192;
	lock.l_pid=getpid();
	printf("File Discripter value :: %d, Process id %ld\n",fd,(long)getpid());
	printf("Before Entering To Critical Section\n");
	int i=fcntl(fd,F_SETLKW,&lock);
	if(i!=-1){
		printf("Locking Status :: %d\n",i);
		printf("Entering Critical Section\n");
		/*CS Start*/
		//s=write(fd,&account,sizeof(struct account_info));
		//getchar();
		/*For testing locking*/
		/*char ch;
		printf("Enter (y,Y) To Confirm\n");
		scanf(" %c", &ch);*/
		s=read(fd,&account,sizeof(struct account_info));
		lock.l_type=F_UNLCK;
		int u=fcntl(fd,F_SETLK,&lock);
		printf("Exiting CS... Unlocking Status :: %d\n",u);
		/*CS End*/
		}else{
		printf("Lock Not Accquired");
		}
		close(fd);
		return account;
}
int createNewAccount(){
		struct sequence seq;
		int seq_no=readSequence();
		//printf("Initial Sequence No %d\n",seq.s_no);
		if(seq_no>=0){
		seq.s_no=seq_no+1;
		//printf("New Sequence No %d\n",seq.s_no);
		seq_no=getSquence(seq);
		//printf("Final Sequence No %d\n",seq_no);
		}	
		if(seq_no>0){
		struct account_info account=getDetails(seq_no);
		int status=openAccount(account);
		if(status>0){
			printf("Account Created ... Account details are\n");
			printAccountDetails(account);
		}else{
			printf("Sorry :: Account Creation Fail..\n");
		}
		}else{printf("Sorry :: Account Creation Fail..\n");}
}
struct account_info getNewAccountDetails(){
		struct sequence seq;
		struct account_info account={0};
		int seq_no=readSequence();
		//printf("Initial Sequence No %d\n",seq.s_no);
		if(seq_no>=0){
		seq.s_no=seq_no+1;
		//printf("New Sequence No %d\n",seq.s_no);
		seq_no=getSquence(seq);
		//printf("Final Sequence No %d\n",seq_no);
		}	
		if(seq_no>0){
		account=getDetails(seq_no);
		}
		return account;
}
int createAdmin(){
		struct account_info account=getDetails(0);
		int status=updateRecord(account,0);
		if(status>0){
			printf("Account Created ... Account details are\n");
			printAccountDetails(account);
		}else{
			printf("Sorry :: Account Creation Fail..\n");
		}			
}
