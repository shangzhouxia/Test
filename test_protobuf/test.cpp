
#include "test.pb.h"		//解析出来的.h文件
#include "stdio.h"

void sendHeart();
void receHeart(TopMessage* topMessage);
void receHeartResp(TopMessage* topMessage);

/*
** ===================================================================
**     Method      :  sendHeart 
**
**     Description :  数据写入
** 
** ===================================================================
*/
void sendHeart(){
	
	TopMessage message;
	message.set_message_type(REQUEST_HEARTBEAT_SIGNAL);
	printf("sendHeart %d\n",message.message_type());
	receHeart(&message);
}

/*
** ===================================================================
**     Method      :  receHeart 
**
**     Description :  数据读取然后写入
** 
** ===================================================================
*/
void receHeart(TopMessage* topMessage){

	if (topMessage->message_type() == REQUEST_HEARTBEAT_SIGNAL)
	{
		
		printf("request_heartbeat_signal\n");
		TopMessage topMessageResp;

		MsgResult mesResult;
		mesResult.set_result(true);
	
		mesResult.set_error_code("error");
		
		topMessageResp.set_message_type(RESPONSE_HEARTBEAT_RESULT);
		
		*topMessageResp.mutable_msg_result() = mesResult;

		receHeartResp(&topMessageResp);
	}
	
}

/*
** ===================================================================
**     Method      :  receHeartResp 
**
**     Description :  数据读取
** 
** ===================================================================
*/
void receHeartResp(TopMessage* topMessage){

	if (topMessage->message_type() == RESPONSE_HEARTBEAT_RESULT)
	{
		printf("response_heartbeat_result\n");
		
		printf("%s\n",topMessage->msg_result().error_code().c_str());

	}
}

int main()
{
	sendHeart();
	google::protobuf::ShutdownProtobufLibrary();
}

