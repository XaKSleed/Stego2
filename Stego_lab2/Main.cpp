#define _CRT_SECURE_NO_WARNINGS
#include "Block_hiding.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdio>
#include <vector>
#include <windows.h>

using namespace std;

int main(int argc, char const *argv[])
{

	FILE* in = fopen("C:\\Users\\Ёльдо\\source\\repos\\Stego_lab2\\Stego_lab2\\kodim09.bmp", "rb");
	FILE* in2 = fopen("C:\\Users\\Ёльдо\\source\\repos\\Stego_lab2\\Stego_lab2\\Image_with_Message.bmp", "rb");
	Block_hiding hide1;
	hide1.Read_message("Message.txt");
	hide1.Read_image(in);
	hide1.Insert_message(128);
	hide1.Write_image();

	hide1.Get_message("Result.txt", 128);

	getchar();
	return 0;

}