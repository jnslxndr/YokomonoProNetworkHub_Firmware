/*
 OSCClass
 2009/03/22 version 1.0.1 add errror process。change Doc
 2009/03/21 version 1.0.0
 
 OSCClass.h - OSC Library code for Arduino.
 
 This library works with arduino firmware0014.
 
 Copyright (c) recotana(http://recotana.com).  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License version 2.1 as published by the Free Software Foundation.
 
 Open Sound Control  http://opensoundcontrol.org/
*/					   

//#define _DEBUG_

#ifdef _DEBUG_
#include "HardwareSerial.h"
#endif

#include "OSCClass.h"


/*!
 @~english
 Gets a IP Address
 @param[in] None
 @return IP Address Array (byte *)
 @~japanese
 IPアドレスを取得
 @param[in] 無し
 @return IPアドレス配列のポインタ (byte *)
 @~
 
 @note IP Address Array ip[]={192,168,0,100}
 */
uint8_t * OSCMessage::getIp(){
	
	return ip;
}


/*!
 @~english
 Gets a PortNo
 @param[in] None
 @return PortNo (unsigned int)
 @~japanese
 ポート番号を取得
 @param[in] 無し
 @return ポート番号(unsigned int)
 @~
 */
uint16_t OSCMessage::getPort(){
	
	return port;
}


/*!
 @~english
 Gets the number of the OSC message address
 @param[in] None
 @return number of the OSC message address (byte)
 @note "/ard", the number of the addresses is 1<br>
 "/ard/test", the number of the addresses is 2
 @attention It is maximum number of the addresses 2
 @~japanese
 OSCメッセージアドレス数を取得
 @param[in] 無し
 @return アドレス数 (byte)
 @note "/ard"の場合アドレス数は1<br>
 "/ard/test"の場合アドレス数は2
 @attention 認識できる最大アドレス数2までに制限されています
 @~
 */
uint8_t	OSCMessage::getAddressNum(){
	
	return addressNum;
}


/*!
 @~english
 Gets the number of the OSC message args
 @param[in] None
 @return number of the args (byte)
 @note "i" 123,number of the OSC message args is 1<br>
 "if" 123 54.24,number of the OSC message args is 2
 @attention It is maximum number of the args 2
 @~japanese
 OSCメッセージの値数を取得
 @param[in] 無し
 @return 値数 (byte)
 @note "i" 123 の場合、値数は 1<br>
 "if" 123 54.24 の場合、値数は 2
 @attention 認識できる最大値数は2までに制限されています
 
 @~
 */
uint8_t	OSCMessage::getArgNum(){
	
	return argNum;
}


/*!
 @~english
 Gets the address string of the OSC message
 @param [in] _index indexNo of the address string (byte)
 @return pointer of the address string (char *)
 @note ex. "/ard/test"<br>
 getAddress(0) = "/ard"<br>
 getAddress(1) = "/test"
 @attention It is maximum number of the addresses is 2<br>
 In this case "/ard/test1/test2"<br>
 ignore it after "/test2"
 @~japanese
 OSCメッセージアドレス文字列を取得
 @param [in] _index アドレスのindex番号
 @return アドレス文字列のポインタ(char *)
 @note "/ard/test"の場合<br>
 getAddress(0) = "/ard"
 getAddress(1) = "/test"
 @attention 認識できる最大アドレス数2までに制限されているため<br>
 "/ard/test1/test2"のような場合<br>
 /test2以降は削除されます
 @~
 */
char * OSCMessage::getAddress(uint8_t _index){
	if(_index>MAX_ADDRESS) _index=MAX_ADDRESS-1;
	return address[_index];
	
}


/*!
 @~english
 Gets the TopAddress string of the OSC message
 @param[in] None
 @return pointer of the TopAddress string (char *)
 @note In this case "/ard/test"<br>
 getTopAddress() = "/ard"
 @~japanese
 OSCメッセージTopアドレス文字列を取得
 @param[in] 無し
 @return Topアドレス文字列のポインタ (char *)
 @note "/ard/test"の場合
 getTopAddress() = "/ard"
 @~
 */
char * OSCMessage::getTopAddress(){
	
	return getAddress(0);
	
}

/*!
 @~english
 Gets the SubAddress string of the OSC message
 @param[in] None
 @return pointer of the SubAddress string (char *)
 @note In this case "/ard/test"<br>
 getSubAddress() = "/test"
 @~japanese
 OSCメッセージSubアドレス文字列を取得
 @param[in] 無し
 @return Subアドレス文字列のポインタ (char *)
 @note "/ard/test"の場合
 getSubAddress() = "/test"
 @~
 */

char * OSCMessage::getSubAddress(){
	
	return getAddress(1);
	
}

/*!
 @~english
 Gets the TypeTag string of the OSC message
 @param[in] _index indexNo of the TypeTag string (byte)
 @return TypeTag char (char)
 @note In this case "if"<br>
 getTypeTag(0) = 'i'<br>
 getTypeTag(1) = 'f'
 @attention It is maximum number of the args is 2<br>
 Please do below indexNo 1
 @~japanese
 OSCメッセージのTypeTag文字を取得
 @param[in] _index TypeTagのindex番号 (byte)
 @return typeTag文字 (char)
 @note "if"の場合
 @note getTypeTag(0) = 'i'
 getTypeTag(1) = 'f'
 @attention 認識できる最大値数は2までに制限されているため<br>
 indexは2以上の指定しても、1となります
 @~
 */
char  OSCMessage::getTypeTag(uint8_t _index){
	if(_index>MAX_ARG) _index=MAX_ARG-1;
	return typeTag[_index];
}

/*!
 @~english
 Get the args of the OSC message with an integer value
 @param[in] _index indexNo of the args (byte)
 @return integer value (long)
 @note In this case "if" 123 54.24<br>
 getArgInt(0) = 123
 @attention This is integer declaration, but the return type is "long"
 @attention When an indexNo is bigger than the number of the args, it is set to the number of the args
 @~japanese
 OSCメッセージの値を整数値で取得
 @param[in] _index 値のindex番号 (byte)
 @return 整数値 (long)
 @note "if" 123 54.24 の場合<br>
 getArgInt(0) = 123
 @attention int表記ですが、返値の型は long です
 @attention index番号が値数より大きい場合は値数にセットされます
 
 @~
 */
int32_t OSCMessage::getArgInt(uint8_t _index){
	
	int32_t *value;
	if(_index > argNum) _index=argNum;
	value = (int32_t *)arg[_index];
	
	return *value;
	
}

/*!
 @~english
 Get the args of the OSC message with an float value
 @param[in] _index indexNo of the args
 @return float value (double)
 @note In this case "if" 123 54.24<br>
 getArgFloat(1) = 54.24
 @attention This is float declaration, but the return type is "double"
 @attention When an indexNo is bigger than the number of the args, it is set to the number of the args
 @~japanese
 OSCメッセージの値を浮動小数点値で取得
 @param[in] _index 値のindex番号 (byte)
 @return 浮動小数点値 (double)
 @note "if" 123 54.24 の場合<br>
 getArgFloat(1) = 54.24
 @attention float表記ですが、返値の型は double です
 @attention index番号が値数より大きい場合は値数にセットされます
 @~
 */

double OSCMessage::getArgFloat(uint8_t _index){
	
	double *value;
	if(_index > argNum) _index=argNum;
	value = (double *)arg[_index];
	
	return *value;
	
	
}

/*!
 @~english
 Set TopAddress string to an OSC Message container
 @param[in] _address Pointer of the TopAddress String (char *)
 @return None
 @note In this case "/ard/test"<br>
 char top[]="/ard"<br>
 setTopAddress(top)
 @~japanese
 Topアドレス文字列をOSCコンテナへセット
 @param[in] _address Topアドレス文字列ポインタ (char *)
 @return 無し
 @note "/ard/test"の場合<br>
 char top[]="/ard"<br>
 setTopAddress(top)
 
 @~
 */
void OSCMessage::setTopAddress(char *_address){
	
	address[0]=_address;
	address[1]=0;
	addressNum=1;
	
}

/*!
 @~english
 Set SubAddress string to an OSC Message container
 @param[in] _address Pointer of the SubAddress String (char *)
 @return 無し
 @note  In this case "/ard/test"<br>
 char sub[]="/test"<br>
 setSubAddress(sub)
 @~japanese
 Subアドレス文字列をOSCコンテナへセット
 @param[in] _address Subアドレス文字列ポインタ (char *)
 @return 無し
 @note "/ard/test"の場合<br>
 char sub[]="/test"<br>
 setSubAddress(sub)
 
 @~
 */
void OSCMessage::setSubAddress(char *_address){
	
	address[1]=_address;
	addressNum=2;
	
}


/*!
 @~english
 Set Address string to an OSC Message container
 @param[in] _topAddress Pointer of the TopAddress String (char *)
 @param[in] _subAddress Pointer of the SubAddress String (char *)
 @return None
 @note In this case "/ard/test"<br>
 char top[]="/ard"<br>
 char sub[]="/test"<br>
 setAddress(top,sub)
 @attention It is maximum number of the Address string is 2<br>
 cannot set "/ard/test1/test2"
 @~japanese
 アドレス文字列をOSCコンテナへセット
 @param[in] _topAddress Topアドレス文字列ポインタ (char *)
 @param[in] _subAddress Subアドレス文字列ポインタ (char *)
 @return 無し
 @note "/ard/test"の場合<br>
 char top[]="/ard"<br>
 char sub[]="/test"<br>
 setAddress(top,sub)
 @attention アドレス文字列は2つまでに制限されています<br>
 "/ard/test1/test2"はセットできません
 @~
 */
void OSCMessage::setAddress(char *_topAddress,char *_subAddress){
	
	setTopAddress(_topAddress);
	setSubAddress(_subAddress);
	addressNum=2;
	
}

/*!
 @~english
 Set address string to the indexNo of the OSC Message container
 @param[in] _index An indexNo to do set (byte)
 @param[in] _address Pointer of the Address String (char *)
 @return None
 @note In this case "/ard/test"<br>
 char adr[]="/ard"<br>
 setAddress(0,adr)<br>
 <br>
 char adr2[]="/test"<br>
 setAddress(1,adr)
 @attention It is maximum number of the Address string is 2<br>
 When an indexNo is bigger than the number of the Address, it is set to the number of the Address
 @~japanese
 アドレス文字列をOSCコンテナのindex番号へセット
 @param[in] _index setするindex番号 (byte)
 @param[in] _address アドレス文字列ポインタ (char *)
 @return 無し
 @note "/ard/test"の場合<br>
 char adr[]="/ard"<br>
 setAddress(0,adr)<br>
 <br>
 char adr2[]="/test"<br>
 setAddress(1,adr)
 @attention アドレス文字列は2つまでに制限されています<br>
 index番号が2以上を指定しても1の指定となります 
 @~
 */
void OSCMessage::setAddress(uint8_t _index, char *_address){
	if(_index>MAX_ADDRESS) _index=MAX_ADDRESS-1;
	address[_index]=_address;
	addressNum=_index+1;
	
}




/*!
 @~english
 Set PortNo to the OSC Message container
 @param[in] _port PortNo (unsigned int)
 @return None
 @~japanese
 ポート番号をOSCコンテナへセット
 @param[in] _port ポート番号 (unsigned int)
 @return 無し
 @~
 */
void OSCMessage::setPort(uint16_t _port){
	
	port = _port;
	
}

/*!
 @~english
 Set pointer of IP Address array to the OSC Message container
 @param[in] _ip pointer of IP Address array (byte *)
 @return None
 @note In this case IP=192.168.0.99<br>
 byte ip[]={192,168,0,1}<br>
 setIp(ip)
 @~japanese
 IPアドレスをOSCコンテナへセット
 @param[in] _ip IPアドレス配列のポインタ (byte *)
 @return 無し
 @note IP=192.168.0.99の場合<br>
 byte ip[]={192,168,0,1}<br>
 setIp(ip)
 @~
 */
void OSCMessage::setIp(uint8_t *_ip){
	
	ip[0] = _ip[0];
	ip[1] = _ip[1];
	ip[2] = _ip[2];
	ip[3] = _ip[3];
	
}


/*!
 @~english
 Set IP Address to the OSC Message container
 @param[in] _ip1 IP address No1 (byte)
 @param[in] _ip2 IP address No2 (byte)
 @param[in] _ip3 IP address No3 (byte)
 @param[in] _ip4 IP address No4 (byte)
 @return None
 @note In this case IP=192.168.0.99<br>
 setIp(192,168,0,99)
 @~japanese
 IPアドレスをOSCコンテナへセットし
 @param[in] _ip1 IPアドレス No1 (byte)
 @param[in] _ip2 IPアドレス No2 (byte)
 @param[in] _ip3 IPアドレス No3 (byte)
 @param[in] _ip4 IPアドレス No4 (byte)
 @return 無し
 @note IP=192.168.0.99の場合<br>
 setIp(192,168,0,99)
 @~
 */
void OSCMessage::setIp(	uint8_t _ip1,
						uint8_t _ip2,
						uint8_t _ip3,
						uint8_t _ip4 ){
	
	ip[0] = _ip1;
	ip[1] = _ip2;
	ip[2] = _ip3;
	ip[3] = _ip4;
	
}

/*!
 @~english
 Set TypeTag and args to the OSC Message container
 @param[in] types TypeTag string "i"(integer) or"f"(float) (char *)
 @param[in] ... Pointer of the Args(variable argument) ..
 @return None
 @note In this case, integer:123<br>
 long v1=123<br>
 setArgs("i",&v1)<br>
 <br>
 In this case, integer:123 float:52.14<br>
 long v1=123<br>
 double v2=52.14<br>
 setArgs("if",&v1,&v2)
 @attention It is maximum number of the args is 2<br>
 In this case,　setArgs("iff",&v1,&v2,&v3)<br>
 ignored after &v3
 @~japanese
 TypeTag、値をOSCコンテナへセットし
 @param[in] types TypeTag文字列 "i"(整数値)or"f"(浮動小数点値) (char *)
 @param[in] ... 値変数のポインタ(可変引数) 
 @return 無し
 @note 整数値:123の場合<br>
 long v1=123<br>
 setArgs("i",&v1)<br>
 <br>
 整数値:123 浮動小数点値:52.14の場合<br>
 long v1=123<br>
 double v2=52.14<br>
 setArgs("if",&v1,&v2)
 @attention 認識できる最大値数は2つまでに制限されています<br>
 setArgs("iff",&v1,&v2,&v3)と指定しても&v3以降は無視されます
 @~
 */

void OSCMessage::setArgs(char *types,...){
	
	va_list argList;
	
	argNum = strlen(types);
	if(argNum>MAX_ARG) argNum=MAX_ARG-1;
	
	va_start( argList, types );
	for(uint8_t i=0 ; i < argNum ; i++){
		
		typeTag[i]=types[i];
		
		switch(types[i]){
				
			case 'i':
				arg[i]=(uint32_t *)va_arg(argList, uint32_t *);
				
				break;
				
			case 'f':
				arg[i]=va_arg(argList, double *);
				break;
				
				
		}
		
	}
	
}


/*!
 @~english
 This is an object communicating OSC
 @~japanese
 OSCメッセージを送受するオブジェクトを生成
 @~
 
 */
OSCClass::OSCClass(){
	
//	recieveContainer = _mes;
	socketNo = 0;
	
}

/*!
 @~english
 This is an object communicating OSC
 @param[in] _mes Pointer of the OSC recieve message (OSCMessage *)
 @~japanese
 OSCメッセージを送受するオブジェクトを生成
 @param[in] _mes OSC受信メッセージのポインタ (OSCMessage *)
 @~
 */
OSCClass::OSCClass(OSCMessage *_mes){
	
	recieveContainer = _mes;
	socketNo = 0;
	
}



/*!
 @~english
 initialize an OSC object
 @param[in] None
 @return None
 @~japanese
 OSC送受オブジェクトを初期化します
 @param[in] 無し
 @return 無し
 
 @~
 */
void OSCClass::begin()
{
	uint16_t _recievePort=10000;
  begin(_recievePort);
}

/*!
 @~english
 initialize an OSC object
 @param[in] _recievePort recieve PortNo (unsigned int)
 @return None
 @~japanese
 OSC送受オブジェクトを初期化
 @param[in] _recievePort 受信待ち受けポート番号(unsigned int)
 @return 無し
 @~
 */
void OSCClass::begin(uint16_t _recievePort)
{
#ifdef _DEBUG_
	Serial.begin(19200);
#endif
  socket(socketNo, SnMR::UDP, _recievePort, 0);
}


void OSCClass::beginMulti(uint16_t _recievePort, uint8_t *address)
{
#ifdef _DEBUG_
	Serial.begin(19200);
#endif
	  multicastsocket(socketNo, SnMR::UDP, _recievePort, address);
}

/*!
 @~english
 check the arrival of the OSC message
 @param[in] None
 @return 1 OSC message arraive
 @return 0 OSC message not arrive
 @~japanese
 OSCメッセージの受信をチェック
 @param[in] 無し
 @return 1 OSC受信あり
 @return 0 OSC受信無し
 @~
 */

bool OSCClass::available()
{
	
	  if( (W5100.getTXFreeSize( socketNo )&&SnIR::RECV) ) {
	
		  if( W5100.getRXReceivedSize(socketNo)>0 ){

			  messageProcess();
			  
			  return 1;
		  }
	  }

	return 0;

}


void OSCClass::messageProcess() {
	
	uint16_t	lenBuff;
	uint8_t		d;	
	uint8_t		messagePos=0;	
	uint8_t		adrCount=0;
	uint8_t		adrMesPos=0;	
	uint8_t		packetCount=0;
	uint8_t		packetPos=4;
	
	
	//W5100.writeSn(socketNo, SnIR, SnIR::RECV);

	lenBuff=recvfrom(	socketNo,
						rcvBuff, 
						1 ,
						recieveContainer->ip ,
					   &recieveContainer->port );	
	



	recieveContainer->address[0]=tempAddress[0];

#ifdef _DEBUG_
	Serial.println("");
#endif

	//address process start
	do{
		d=rcvBuff[messagePos];

#ifdef _DEBUG_
		Serial.print(d);		
		Serial.print(" messagePos:");
		Serial.println(messagePos,DEC);
#endif
		
		if( (d=='/') && (messagePos>0) ){

			if(adrCount<MAX_ADDRESS){
				tempAddress[adrCount][adrMesPos]=0;

				adrCount++;
				adrMesPos=0;

				recieveContainer->address[adrCount]=tempAddress[adrCount];
			}

		}
		
		if(adrCount<MAX_ADDRESS){
		//Added this in to remove the slashes out of final output
		if(d!='/'){
		tempAddress[adrCount][adrMesPos]=d;			
	
		if(packetCount>3)  {
			packetCount=0;
			packetPos+=4;
		}
		
		adrMesPos++;
		}
		}
		messagePos++;
		packetCount++;
		
	}while(d!=0);

#ifdef _DEBUG_
	Serial.print(" messagePos:");
	Serial.print(messagePos,DEC);
	
	Serial.print(" packetCount:");
	Serial.print(packetCount,DEC);
	
	Serial.print(" packetPos:");
	Serial.println(packetPos,DEC);
#endif
	
	
	if(adrCount<MAX_ADDRESS) adrCount++;
	recieveContainer->addressNum=adrCount;
	
	messagePos=packetPos;


	//name space  process end

	//type tag process start

	packetCount=0;
	packetPos+=4;
	


	uint8_t  typeTagPos=0;
	uint8_t	 tempArgNum=0;

	while(rcvBuff[messagePos]!=0 ){

#ifdef _DEBUG_
		Serial.print(rcvBuff[messagePos]);		
		Serial.print(" messagePos:");
		Serial.println(messagePos,DEC);
#endif
		
			
		if(rcvBuff[messagePos] != ',') {
		
				if(typeTagPos<MAX_ARG){
					recieveContainer->typeTag[tempArgNum]=rcvBuff[messagePos];
					tempArgNum++;
				}
				typeTagPos++;
				
			}
		
		packetCount++;
		
		if(packetCount>3)  {
			packetCount=0;
			packetPos+=4;
		}
		
		messagePos++;
	}

#ifdef _DEBUG_
	Serial.print(" messagePos:");
	Serial.print(messagePos,DEC);
	
	Serial.print(" packetCount:");
	Serial.print(packetCount,DEC);
	
	Serial.print(" packetPos:");
	Serial.println(packetPos,DEC);
#endif
	
	recieveContainer->argNum=tempArgNum;

	messagePos=packetPos;

	//type tag process end

	//tempArg process start

	
	for(int i=0;i<tempArgNum;i++){
		
		adrMesPos=3;

		recieveContainer->arg[i]=tempArg[i];

		
		for(int j=0;j<4;j++){

			tempArg[i][adrMesPos]=rcvBuff[messagePos];

			messagePos++;
			adrMesPos--;
		}
	
	}


//return &mes;


}


/*!
 @~english
 Get an OSC recieve message
 @param[in] None
 @return Pointer of the OSC recieve message container (OSCMessage *)
 @~japanese
 受信したOSCメッセージコンテナを取得
 @param[in] 無し
 @return 受信したOSCメッセージクラスのポインタ (OSCMessage *)
 @~
 */

OSCMessage * OSCClass::getMessage(){
	
	return recieveContainer;
}





/*!
 @~english
 send an OSC Message
 @param[in] _mes Pointer of the OSC message container (OSCMessage *)
 @return None
 @~japanese
 OSCメッセージを送信
 @param[in] _mes OSCメッセージクラスのポインタ (OSCMessage *)
 @return 無し
 
 @~
 */
void OSCClass::sendOsc( OSCMessage *_mes )
{
	uint8_t lengthEnd;
	uint8_t lengthStart;	
	char  buff[100];
	
	sendContainer = _mes;
	
	//バッファ初期値
	buff[0]=0;
	
	//name space文字列を格納
	for(int i=0;i<sendContainer->addressNum;i++){
		
		strcat(buff,sendContainer->address[i]);

		
	}

	//パケット倍数での文字数を取得
	lengthStart=strlen(buff);
	lengthEnd=lengthStart+(4-(lengthStart%4));
	
	//文字列最後尾からパケット倍数文字数まで０で埋める
	for(int i=lengthStart ; i<lengthEnd; i++){
		buff[i]=0;
	}
	
	lengthStart=lengthEnd;
	
	
	//TypeTagを格納
	buff[lengthEnd++]=',';
	for(int i=0;i<sendContainer->argNum;i++){
		

		buff[lengthEnd++]=sendContainer->typeTag[i];
		
	}
	
	//パケット倍数での文字数を取得
	lengthStart=lengthEnd;
	lengthEnd=lengthStart+(4-(lengthStart%4));
	
	//文字列最後尾からパケット倍数文字数まで０で埋める
	for(int i=lengthStart ; i<lengthEnd; i++){
		buff[i]=0;
	}
	
	//Argをビッグエンディアン化し格納
	uint8_t *v;
	for(int i=0;i<sendContainer->argNum;i++){
		uint8_t valuePos=3;
		v=(uint8_t *)sendContainer->arg[i];

		buff[lengthEnd++]=v[valuePos--];
		buff[lengthEnd++]=v[valuePos--];
		buff[lengthEnd++]=v[valuePos--];
		buff[lengthEnd++]=v[valuePos]; 
		
		
	}
	
	//udp送信	
	sendto(	socketNo ,
			(uint8_t *)buff ,
			lengthEnd ,
			sendContainer->ip ,
			sendContainer->port );
}

/*!
 @~english
 set a OSC recieve message container
 @param[in] _mes Pointer of the OSC recieve message container (OSCMessage *)
 @return None
 @~japanese
 受信用OSCメッセージコンテナをセットし
 @param[in] _mes 受信用OSCメッセージのポインタ (OSCMessage *)
 @return 無し
 @~
 */
void OSCClass::setRecieveMessage(OSCMessage *_mes){
	
	recieveContainer = _mes;
	
}


/*!
 @~english
 flush a recieve buffer
 @param[in] None
 @return None
 @~japanese
 受信バッファに保存されているのメッセージをクリア
 @param[in] 無し
 @return 無し
 
 @~
 */
void OSCClass::flush() {
	
	while ( available() ){}

}

/*!
 @~english
 stop OSC communication
 @param[in] None
 @return None
 @~japanese
 OSC通信を終了
 @param[in] 無し
 @return 無し
 @~
 */
void OSCClass::stop() {
	close( socketNo );

}


