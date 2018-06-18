#include <iostream>
#pragma comment(lib,"winmm.lib")
#include <windows.h>
using namespace std;
int main(){
	PlaySound("../music.wav",NULL,SND_FILENAME|SND_ASYNC|SND_LOOP);
	system("PAUSE");
	return 0;
}
