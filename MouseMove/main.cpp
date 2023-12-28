#include <iostream>
#include <Windows.h>

//https://github.com/ekknod/MouseClassServiceCallbackTrick also gets caught
//https://github.com/ekknod/MouseClassServiceCallbackMeme 
//NtInjectedMouseInput gets caught
//mouse_event gets caught. was some weird here tho with admin etc.
//Hardware(kmbox etc) prob doesn't get caught, never tested never will test :D

void main() {
	//Move mouse with software every 500 ms.
	while (1) {
		mouse_event(MOUSEEVENTF_MOVE, 1, 0, 0, 0);
		Sleep(500);
	}
}
