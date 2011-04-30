/*! @OSCClass.h
 
 @~english
 @brief This is a library communicating OSC in Arduino+EthernetShield
 
 @~japanese
 @brief Arduino+EthernetShieldにてOSC通信を行うライブラリ
 
 @author recotana
 @date 2009/03/22, ver1.0.1
 
 */



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
 
 
 
 OSCClass version 1.0.1 Specification.

 ********
 Address : max 2
 	"/ard"
	"/ard/output"	--address[0]="/ard"		:max 15character
					--address[1]="/output"	:max 15character

 *******
 TypeTag	:	max 2
 
 "i" - long or unsigned long
 "f" - double
 
 ******* 
 arg	:	max 2
 
 
*/



#ifndef OSC_h
#define OSC_h

#define MAX_ADDRESS	2
#define MAX_ARG		2


#define TYPE_INT	1
#define TYPE_FLOAT	2


//extern "C" {
//#include "utility/types.h"
#include "utility/w5100.h"
#include "utility/socket.h"
//}
#include <string.h>
#include <stdarg.h>

#include "Ethernet.h"

/*!
 @~english
 @brief This is a container class of the OSC message

 @~japanese
 @brief OSCメッセージを格納するクラス
 
 */
class OSCMessage{
	
	private:
	
		char		*address[MAX_ADDRESS];
		uint8_t		 addressNum;		
	
		char		 typeTag[MAX_ARG];
	
		void		*arg[MAX_ARG];
		uint8_t		 argNum;
	
		uint8_t		 ip[4];			
		uint16_t	 port;
	
	
	
	public:
		//ex. ip[]={192,168,0,100}
		uint8_t		*getIp();	//return ip array pointer. ip
		uint16_t	 getPort();
	
		//ex. address patern "/adr/test"
		//    address[2]={"/ard" , "/test"}
		char		*getAddress(uint8_t _index);	//retturn address
		char		*getTopAddress();	//return address[0] :"/ard"
		char		*getSubAddress();	//return address[1] :"/test"
		uint8_t		 getAddressNum();	//return 2		
	
		// 'i': long(int32_t)
		// 'f': double
		//ex 'if' 123 54.21
		char		 getTypeTag(uint8_t _index);	//_index=0 ->'i'
													//_index=1 ->'f'

		uint8_t		 getArgNum();	//return 2
	
		int32_t		 getArgInt(uint8_t _index);		//_index=0 -> 123
		double		 getArgFloat(uint8_t _index);	//_index=1 -> 54.21
	
	
		void setTopAddress(char *_address);		//set address[0]
		void setSubAddress(char *_address);		//set address[1]
		void setAddress(char *_topAddress,	 
						char *_subAddress);
		void setAddress(uint8_t _index,		//set 0,address[0]
						char *_address);	
											//set 1,address[1]
	
		void setIp( uint8_t *_ip );	//set ip
	
		void setIp(uint8_t _ip1,	//set(192,
				   uint8_t _ip2,	//	  168,
				   uint8_t _ip3,	//    0,
				   uint8_t _ip4);	//    100)
	
		void setPort( uint16_t _port );
	
		//ex. long   v1=100
		//    double v2=123.21
		void setArgs( char *types , ... );	//set ("if",&v1,&v2)
	
		friend class OSCClass;
	
	
};



/*!
 @~english
 @brief The send&recieve class an OSC message
 
 @~japanese
 @brief OSCメッセージを送受するクラス
 
 */
class OSCClass {
	
private:
	
	uint8_t		 socketNo;
  	uint8_t		 rcvBuff[100];
	
	char		 tempAddress[MAX_ADDRESS][16];
	uint8_t		 tempArg[MAX_ARG][4];	
	
	void		 messageProcess();
	
	OSCMessage *recieveContainer;
	OSCMessage *sendContainer;
	
public:
	

		
	OSCClass();
	OSCClass(OSCMessage *_mes);
	
	//init osc (and inside socket No  "0" setting )
	void begin();
	void begin(uint16_t _recievePort);
  void beginMulti(uint16_t _recievePort, uint8_t *addr);
	void stop();
	
	//OSC recieve
	bool available();	//when OSC recieved return "1" 

	void setRecieveMessage( OSCMessage *_mes ); //set recieve OSCmessage container
	OSCMessage	*getMessage();	//return recieve OSCmessage	

	//buffer clear
	void flush();	
	
	//OSC send
	void sendOsc( OSCMessage *_mes ); //set&send OSCmessage


};

#endif
