#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include"utill.h"
int main(){
	struct sockaddr_in serv,cli;
	int sd,sz,nsd;
	char buff[80];
	pid_t pid;
	//sd=socket(AF_UNIX,SOCK_STREAM,0);

	sd=socket(AF_INET,SOCK_STREAM,0);
	//serv.sin_family=AF_UNIX;
	serv.sin_family=AF_INET;
	serv.sin_addr.s_addr=INADDR_ANY;
	serv.sin_port=htons(7791);




	int bind_result=bind(sd,(void *)(&serv),sizeof(serv));
	int listen_result=listen(sd,5);
	sz=sizeof(cli);
	printf("Server Initializing...\nSocket returns :: %d\nBind result:: %d\nListening status :: %d\n",sd,bind_result,listen_result);
	for(;;){
	nsd=accept(sd,(void *)(&cli),&sz);
	if ( (pid = fork()) == 0 ) {
		close(sd);
		ssize_t s=0;
		struct data_packet request={0},response={0};
		read(nsd,&request,sizeof(request));
		printf("Message Recieve From client :: Action %d\n",request.result);
		switch(request.action){
		case CREATE:{
			response.action=request.action;
			response.result=openAccount(request.account);
			response.account=request.account;
		//	response.result_msg="CREATE_OK";
			break;}
		case UPDATE:{
			response.action=request.action;
			int record_no=getRecordNoByAccountNo(request.account.account_no);
			response.result=updateRecord(request.account,record_no);
			response.account=request.account;
		//	response.result_msg="DELETE_OK";
			break;}
		case GET_ACC:{
			response.result=1;
			response.action=request.action;
			int record_no=getRecordNoByAccountNo(request.account.account_no);
			response.account=getRecord(record_no);
		//	response.result_msg="INVALID_ACTION";
			break;}
		default:{
			response.result=0;
			response.action=request.action;
			response.account=request.account;
		//	response.result_msg="INVALID_ACTION";
		}
		}
		s=write(nsd,&response,sizeof(response));
		close(nsd);
		printf("Message  sent to client :: %d\n Write Response %d",response.result,(int)s);
		exit(0);
		}
		close(nsd);
	}
	}


