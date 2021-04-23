#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <windows.h>

using namespace std;
class Block_hiding
{
private:

	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	string message;
	int message_size;
	int num_insert_bits = 0;
	int image_height = 0;
	int image_width = 0;
	unsigned int** blue_matrix;
	unsigned int** red_matrix;
	unsigned int** green_matrix;
	unsigned int** old_blue_matrix;
	int block_size = 0;


	bool is_code = false;
	char delimeters[4];
	int num_delimeters = 0;
public:

	Block_hiding()
	{
		
	}

	~Block_hiding() {}

	int Read_message(string message_file);
	int Read_image(FILE* in);
	int Insert_message(int block_size);
	int Get_message(string res_file, int block_size);
	int Write_image();
	int Bits_to_byte(bool bits[]);
	void get_PSNR(unsigned int** original, unsigned int** restored);
};
