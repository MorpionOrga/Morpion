#include "Thread.h"
#include <Windows.h>

bool Thread::Start()
{
    m_threadHandle = CreateThread(nullptr, 0, Static_ThreadProc, (void*)this, 0, nullptr);
    //std::cout << "Id : " << threadID << std::endl;
	return (m_threadHandle != nullptr);
}

void Thread::Wait()
{
    if (m_threadHandle != nullptr) {
        WaitForSingleObject(m_threadHandle, INFINITE);
        CloseHandle(m_threadHandle);
        m_threadHandle = nullptr;
    }
    std::cout << "thread close " << std::endl;
}