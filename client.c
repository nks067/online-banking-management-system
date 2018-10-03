#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include<string.h>
#include"client_utill.h"
struct data{
  int action;
  int result;
//  char *result_msg;
  };

struct data_packet execute(struct data_packet packet);
int main(){
	struct data_packet request,response={0};
	int z=0;
	char c;
	/*request.action=GET_ACC;
	request.result=-1;
	printf("Enter Account No \n");
	scanf("%lld",&request.account.account_no);
	response=execute(request);
	printf("Response Status :: %d\n",response.result);
	printAccountDetails(response.account);*/
//	printf("Response Status :: %s\n",response.result_msg);
//	printf("**************** WELCOME TO ONLINE BANKING ********************\n");
	printf("\n\n");
	printf("\n\n\t\t\t\tWELCOME TO LOG IN ZONE");
      	printf("\n\t\t\t\t^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("ENTER ACCOUNT NO: \t");
	scanf("%lld",&request.account.account_no);
	printf("ENTER PASSWORD: \t");
	scanf(" %10[^\n]",request.account.user.password);
	printf("%lld,%s",request.account.account_no,request.account.user.password);
	request.action=GET_ACC;
	request.result=-1;
	response=execute(request);
	printf("Response Status :: %d\n",response.result);
	if(request.account.account_no==response.account.account_no){
	if(strcmp(request.account.user.password,response.account.user.password) == 0){
		//login success
		printf("LOGIN SUCCESS : Account Details Are\n");
		printAccountDetails(response.account);
		if(response.account.user.user_type=='n'){
		//normal user
			printf("Weclome User :: %s\n",response.account.user.name_1);
			int choice,ch;
			do{
			printf("Enter Choice\n");
			printf("1.Deposit\n");
			printf("2.Withdraw\n");
			printf("3.Balance Enquiry\n");
			printf("4.Password Change\n");
			printf("5.View details\n");
			printf("6.Exit\n");
			scanf("%d",&choice);
			switch(choice){
			case Deposit:{
				double amount;
				printf("Enter Deposit Amount\n");
				scanf("%lf",&amount);
				if(amount>0){
					struct data_packet a_request,a_response={0};
					a_request.account.account_no=response.account.account_no;
					a_request.action=GET_ACC;
					a_request.result=-1;
					a_response=execute(a_request);
					if(a_response.result>0&&a_response.account.status>0){
						a_response.account.balance=a_response.account.balance+amount;
						a_response.action=UPDATE;
						a_response.result=-1;
						response=execute(a_response);
						printAccountDetails(response.account);
						if(response.result>0){printf("Transaction Success\n");}
						else{printf("Sorry !! Transaction Fail\n");}
					}else{printf("Sorry !! Unable To Fatch Account Details To Deposit.\n");}
					}else{printf("Sorry !! Invalid Amount\n");}
				break;}
			case Withdraw:{
				double amount;
				printf("Enter Withdraw Amount\n");
				scanf("%lf",&amount);
				struct data_packet a_request,a_response={0};
				a_request.account.account_no=response.account.account_no;
				a_request.action=GET_ACC;
				a_request.result=-1;
				a_response=execute(a_request);
				if(a_response.result>0&&a_response.account.status>0){
					response=a_response;
					if(response.account.balance>=amount){
					response.account.balance=response.account.balance-amount;
					response.action=UPDATE;
					response.result=-1;
					response=execute(response);
					if(response.result>0){printf("Transaction Success\n");}
					else{printf("Sorry !! Transaction Fail\n");}
					}
					else{printf("Sorry !! Not Enough Balance\n");}
					}else{printf("Sorry !! Unable To Fatch Account Details To Withdraw.\n");}
				break;}
			case Balance_Enquiry:{
				response.action=GET_ACC;
				response.result=-1;
				response=execute(response);
				if(response.result>0){printf("Transaction Success\n");
				printf("Available Balance Is INR %lf\n",response.account.balance);
				}
				else{printf("Sorry !! Transaction Fail\n");}
				break;}
			case Password_Change:{
				char password[10];
				printf("Enter New Password\n");
				scanf(" %10[^\n]",response.account.user.password);
				printf("Confirm Password\n");
				scanf(" %10[^\n]",password);
				if(strcmp(password,response.account.user.password) == 0){
				response.action=UPDATE;
				response.result=-1;
				response=execute(response);
				if(response.result>0){printf("Transaction Success !! Password Successfully Changed\n");}
				else{printf("Sorry !! Transaction Fail. Something Going Wrong\n");}
				}else{printf("Sorry !! Password Not Match\n");}
				break;}
			case View_details:{
				response.action=GET_ACC;
				response.result=-1;
				response=execute(response);
				printAccountDetails(response.account);
				break;}
			case Exit:{
				exit(0);
				break;}
			default:{printf("Invalid Choice :: %d\n",choice);}
			}
			printf("Do You Want To Continue (Press 1)\n");
			scanf("%d",&ch);
			}while(ch==1);
		}else if(response.account.user.user_type=='a'){
		//admin
			int choice,ch;
			printf("Weclome Admin :: %s\n",response.account.user.name_1);
			do{
			printf("Enter Choice\n");
			printf("1.Add New Account\n");
			printf("2.Delete An Account\n");
			printf("3.Modify\n");
			printf("4.Search An Account\n");
			printf("5.Exit\n");
			scanf("%d",&choice);
			switch(choice){
			case ADD:{
				struct account_info account=getNewAccountDetails();
				if(account.account_no>0){
				struct data_packet a_request,a_response={0};
				a_request.action=CREATE;
				a_request.result=-1;
				a_request.account=account;
				a_response=execute(a_request);
				if(response.result>0){printf("Transaction Success !! New Account Successfully Created\nDetails Are..\n");
				printAccountDetails(a_response.account);
				}
				else{printf("Sorry !! Transaction Fail. Account Creating Fail\n");}
				}
				else{printf("Sorry !! Account Creating Fail \n");}
				break;}
			case DELETE:{
				struct data_packet a_request,a_response={0};
				printf("Enter Account No To Delete\n");
				scanf("%lld",&a_request.account.account_no);
				a_request.action=GET_ACC;
				a_request.result=-1;
				a_response=execute(a_request);
				if(a_response.result>0&&a_response.account.status>0){
					a_response.account.status=0;
					a_request=a_response;
					a_request.action=UPDATE;
					a_request.result=-1;
					a_response=execute(a_request);
					if(a_response.result>0){
					printf("Transaction Success !! Account Deleted Successfully\n");
					}
					else{printf("Sorry !! Unable To Delete Account For Account No %lld\n",a_request.account.account_no);}
				}
				else{printf("Sorry !! Unable To Fatch Account For Account No %lld\n",a_request.account.account_no);}
				break;}
			case MODIFY:{
				int m_choice,m_ch;
				do{
					printf("Choose What Wants To Update\n");
					printf("1.Update Name\n");
					printf("2.City\n");
					printf("3.State\n");
					printf("4.Country\n");
					printf("5.Pin Code\n");
					printf("6.Mobile No\n");
					printf("7.Email\n");
					printf("8.Exit\n");
					scanf("%d",&m_choice);
					switch(m_choice){
					case 1:{
						struct data_packet a_request,a_response={0};
						printf("Enter Account No To Update Name\n");
						scanf("%lld",&a_request.account.account_no);
						a_request.action=GET_ACC;
						a_request.result=-1;
						a_response=execute(a_request);
						if(a_response.result>0&&a_response.account.status>0){
							if(a_response.account.account_type=='j'){
							printf("Enter User Name\n");
							scanf(" %20[^\n]",a_response.account.user.name_1);
							printf("Enter Second User Name\n");
							scanf(" %20[^\n]",a_response.account.user.name_2);
							}else{
							printf("Enter User Name\n");
							scanf(" %20[^\n]",a_response.account.user.name_1);
							}
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Name Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Name For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fatch Account For Account No %lld\n",a_request.account.account_no);}		
					break;}
					case 2:{
						struct data_packet a_request,a_response={0};
						printf("Enter Account No To Update City\n");
						scanf("%lld",&a_request.account.account_no);
						a_request.action=GET_ACC;
						a_request.result=-1;
						a_response=execute(a_request);
						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter City\n");
							scanf(" %10[^\n]",a_response.account.user.city);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! City Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update City For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fatch Account For Account No %lld\n",a_request.account.account_no);}							
					break;}
					case 3:{
						struct data_packet a_request,a_response={0};
						printf("Enter Account No To Update State\n");
						scanf("%lld",&a_request.account.account_no);
						a_request.action=GET_ACC;
						a_request.result=-1;
						a_response=execute(a_request);
						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter State\n");
							scanf(" %10[^\n]",a_response.account.user.state);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! State Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update State For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fatch Account For Account No %lld\n",a_request.account.account_no);}		
				
					break;}
					case 4:{
						struct data_packet a_request,a_response={0};
						printf("Enter Account No To Update Country\n");
						scanf("%lld",&a_request.account.account_no);
						a_request.action=GET_ACC;
						a_request.result=-1;
						a_response=execute(a_request);
						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter Country\n");
							scanf(" %10[^\n]",a_response.account.user.country);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Country Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Country For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fatch Account For Account No %lld\n",a_request.account.account_no);}		
				
					break;}
					case 5:{
						struct data_packet a_request,a_response={0};
						printf("Enter Account No To Update Pin Code\n");
						scanf("%lld",&a_request.account.account_no);
						a_request.action=GET_ACC;
						a_request.result=-1;
						a_response=execute(a_request);
						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter Pin Code\n");
							scanf("%lld",&a_response.account.user.pin_code);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Pin Code Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Pin Code For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fatch Account For Account No %lld\n",a_request.account.account_no);}		

					break;}
					case 6:{
						struct data_packet a_request,a_response={0};
						printf("Enter Account No To Update Mobile No\n");
						scanf("%lld",&a_request.account.account_no);
						a_request.action=GET_ACC;
						a_request.result=-1;
						a_response=execute(a_request);
						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter Mobile No\n");
							scanf("%lld",&a_response.account.user.mobile_no);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Mobile No Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Mobile No For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fatch Account For Account No %lld\n",a_request.account.account_no);}		
					
					break;}
					case 7:{
						struct data_packet a_request,a_response={0};
						printf("Enter Account No To Update Email\n");
						scanf("%lld",&a_request.account.account_no);
						a_request.action=GET_ACC;
						a_request.result=-1;
						a_response=execute(a_request);
						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter Email Id\n");
							scanf(" %50[^\n]",a_response.account.user.email);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Email Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Email For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fatch Account For Account No %lld\n",a_request.account.account_no);}		
					break;}
					case 8:{
						exit(0);
					break;}
					default:{printf("Invalid Choice :: %d\n",m_choice);}
					}
					printf("Do You Want To Continue (Press 1 Modify Menu)\n");
					scanf("%d",&m_ch);
					}while(m_ch==1);			
				break;}
			case SEARCH:{	
				struct data_packet a_request,a_response={0};
				printf("Enter Account No To Search\n");
				scanf("%lld",&a_request.account.account_no);
				a_request.action=GET_ACC;
				a_request.result=-1;
				a_response=execute(a_request);
				if(a_response.result>0&&a_response.account.account_no>0&&a_response.account.status>0){
				printf("Transaction Success !! Account Found\n");
				printAccountDetails(a_response.account);
				}
				else{printf("Sorry !! Unable To Fatch Account For Account No %lld\n",a_request.account.account_no);}
				break;}
			case ADMIN_EXIT:{
				exit(0);
				break;}
			default:{printf("Invalid Choice :: %d\n",choice);}
			}
			printf("Do You Want To Continue (Press 1)\n");
			scanf("%d",&ch);
			}while(ch==1);
		}else{
			printf("Invalid Password :: %c\n",request.account.user.user_type);
		}

	}else{printf("Invalid Password :: %s\n",request.account.user.password);}
	}else{printf("Invalid Account No :: %lld\n",request.account.account_no);}
}

struct data_packet execute(struct data_packet request_packet){
	struct data_packet response_packet={0};
	struct sockaddr_in serv,cli;
	int sd;
	char buff[80];
	struct data d;
	char *ip_addr="172.16.132.56";
	sd=socket(AF_INET,SOCK_STREAM,0);
	serv.sin_family=AF_INET;
	serv.sin_addr.s_addr=inet_addr (ip_addr);
	serv.sin_port=htons(7791);
	int result=connect(sd,(void *)(&serv),sizeof(serv));
	if(result<0){printf("connection status :: %d\t\n",result);}
	ssize_t s=write(sd,&request_packet,sizeof(struct data_packet));
	ssize_t r=read(sd,&response_packet,sizeof(struct data_packet));
	close(sd);
	//printf("Write %d Bytes | Read %d Bytes\nClosing Connection...",(int)s,(int)r);
	return response_packet;
}
