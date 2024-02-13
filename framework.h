#pragma once
//Include que nous allons utiliser
#include <SFML/Graphics.hpp>
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>
#include <iostream>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"


//Définie le port
//#define PORT 80
#define PORT 14843
#define WM_ACCEPT (WM_USER + 1) 
#define WM_READ (WM_USER + 2) 
