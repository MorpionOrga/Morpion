#pragma once
//Empeche d'avoir certains avertissements li�s � l'utilisation de fonctions risqu�es
#define _CRT_SECURE_NO_WARNINGS

//On se lie � la biblioth�que ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

//Include que nous allons utiliser
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"


//D�finie le port
//#define PORT 80
#define PORT 14843
#define WM_ACCEPT (WM_USER + 1) 
#define WM_READ (WM_USER + 2) 

using namespace rapidjson;