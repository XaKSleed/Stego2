#define _CRT_SECURE_NO_WARNINGS
#include "Block_hiding.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <time.h>

using namespace std;

int Block_hiding::Read_message(string message_file) {

	ifstream fin(message_file);

	if (!fin.is_open())
	{
		cout << "File not found" << endl;
		return 1;
	}
	else
	{
		getline(fin, message);
	}

	message_size = message.length();
	fin.close();
	return 0;
}

int Block_hiding:: Read_image(FILE* in)
{
	
	fread(&bfh, sizeof(bfh), 1, in);
	fread(&bih, sizeof(bih), 1, in);
	
	int height = bih.biHeight;
	int width = bih.biWidth;
	image_height = height;
	image_width = width;
	cout << image_height << endl;
	cout << image_width << endl;
	

	RGBTRIPLE rgb;

	red_matrix = new unsigned int * [bih.biHeight];
	green_matrix = new unsigned int *[bih.biHeight];
	blue_matrix = new unsigned int *[bih.biHeight];
	old_blue_matrix = new unsigned int *[bih.biHeight];

	for (int i = 0; i < bih.biHeight; i++)
	{
		red_matrix[i] = new unsigned int[bih.biWidth];
		green_matrix[i] = new unsigned int[bih.biWidth];
		blue_matrix[i] = new unsigned int[bih.biWidth];
		old_blue_matrix[i] = new unsigned int[bih.biWidth];

		for (int j = 0; j < bih.biWidth; j++)
		{
			fread(&rgb, sizeof(rgb), 1, in);

			red_matrix[i][j] = rgb.rgbtRed;
			
			green_matrix[i][j] = rgb.rgbtGreen;
			
			blue_matrix[i][j] = rgb.rgbtBlue;

			old_blue_matrix[i][j] = rgb.rgbtBlue;
			
		}
		
	}

	fseek(in, 0, SEEK_SET);
	return 0;
}

int Block_hiding:: Write_image()
{

	FILE* out = fopen("C:\\Users\\Ёльдо\\source\\repos\\Stego_lab2\\Stego_lab2\\Image_with_Message.bmp", "wb");
	fwrite(&bfh, sizeof(bfh), 1, out);
	fwrite(&bih, sizeof(bih), 1, out);

	RGBTRIPLE rgb;

	for (int i = 0; i < image_height; i++)
	{
		for (int j = 0; j < image_width; j++)
		{
			rgb.rgbtRed = (BYTE) red_matrix[i][j];
			rgb.rgbtGreen = (BYTE) green_matrix[i][j];
			rgb.rgbtBlue = (BYTE) blue_matrix[i][j];
			fwrite(&rgb, sizeof(rgb), 1, out);
		}
	}
	cout << "I write message" << endl;
	fclose(out);
	return 0;
}

int Block_hiding::Insert_message(int block_size)
{
	bool bit;
	int xor = 0;
	bool b;
	int k = 7;
	int n_x = 0;
	int n_y = 0;
	int counter_msg = 0;


	if ((image_height % block_size != 0) || (image_width % block_size != 0))
	{
		cout << "Wrong size of block, please choose other size" << endl;
		return 0;
	}

	int possible_capacity = (image_width / block_size) * (image_height / block_size);
	cout << "Possible capacity with size of block = " << block_size << " is " << possible_capacity << " bit " << endl;

	for (int i = 0; i < image_height; i += block_size)
	{
		for (int j = 0; j < image_width; j += block_size)
		{
			if (counter_msg < message_size) 
			{
				unsigned char msg_char = message[counter_msg];
				bit = (bool((1 << k) & msg_char));
				for (int m = i; m < i + block_size; m++)
				{
					for (int l = j; l < j + block_size; l++)
					{
						xor = xor ^ blue_matrix[m][l];
						
					}
				}
			
				b  = (bool((1 << 7) & xor));
				xor = 0;
				if (b != bit)
				{
					srand(time(NULL));
					n_x = i + rand() % ((i + block_size) - i);
					n_y = j + rand() % ((j + block_size) - j);
					blue_matrix[n_x][n_y] ^= (1);
				}

				num_insert_bits++;
				k--;
				if (k < 0)
				{
					k = 7;
					counter_msg++;

				}

			}
			else
			{
				cout << "I insert bits "; cout << num_insert_bits << endl;
				get_PSNR(old_blue_matrix, blue_matrix);
				return 0;
			}
		}
	}
	get_PSNR(blue_matrix, old_blue_matrix);
	cout << "I insert bits "; cout << num_insert_bits << endl;
	cout << "Space ended" << endl;
	return 0;
}

int Block_hiding:: Get_message(string res_file, int block_size)
{
	string res_string = "";
	bool bits[8];
	int bit_number = 0;
	int simb_value = 0;
	int xor = 0;
	bool param = false;
	bool b;

	for (int i = 0; i < image_height; i += block_size)
	{
		for (int j = 0; j < image_width; j += block_size)
		{
				
				for (int m = i; m < i + block_size; m++)
				{
					for (int l = j; l < j + block_size; l++)
					{
						xor = xor ^ blue_matrix[m][l];

					}
				}
				
				b = (bool((1 << 7) & xor));
				xor = 0;
				
				bits[bit_number] = b;
				bit_number++;
				if (bit_number == 8)
				{
					
					bit_number = 0;
					char liter = Bits_to_byte(bits);
					res_string += liter;
				}
				if (res_string.length() == message_size)
				{	
					param = true;
					break;
				}
				
		}
		if (param)
		{
			break;
		}
	}

	ofstream fout;
	fout.open(res_file);
	fout << res_string;
	fout.close();
	cout << "I get message" << endl;
	return 0;
}

void Block_hiding:: get_PSNR(unsigned int** original, unsigned int** restored)
{
	float sum = 0;
	float PSNR = 0;
	for (int i = 0; i < image_height; i++)
		for (int j = 0; j < image_width; j++)
			sum = sum + pow(original[i][j] - restored[i][j], 2);

	PSNR = 10 * log10(((image_height * image_width) * pow(255, 2)) / sum);
	PSNR = PSNR + 200;
	cout << "PSNR is " << PSNR << endl;
}


int Block_hiding::Bits_to_byte(bool bits[])
{
	int byte_val = 0;
	int tmp = 0;
	int bit = 0;
	for (int i = 1; i <= 8; i++)
	{
		bit = (int)bits[i - 1];
		if (bit != 0)
		{
			tmp = pow(2 * bit, 8 - i);
			byte_val = byte_val + tmp;
		}

	}
	return byte_val;
}