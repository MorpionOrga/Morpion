#pragma once
#include <Windows.h>
#include <iostream>

class Thread
{
public: 
    Thread() : m_threadHandle(nullptr) { 
        std::cout << "Thread create" << std::endl; 
        //Start();
    };

	virtual ~Thread() {};
	
    virtual bool Run() = 0;
    virtual bool Start();
    virtual void Wait();
private:
    static DWORD WINAPI Static_ThreadProc(LPVOID param)
    {
        Thread* thread = static_cast<Thread*>(param);
        if (thread != nullptr) {
            if (!thread->Run())
                std::cerr << "error run thread" << std::endl;
        }
        return 0;
    }

    HANDLE m_threadHandle;
    LPDWORD threadID;
};

