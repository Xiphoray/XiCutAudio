#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>

using namespace std;

struct wav_struct
{
	unsigned short encodinformat; //�����ʽ
	unsigned short channel;             //ͨ����
	unsigned long frequency;            //����Ƶ��
	unsigned short sample_num_bit;      //һ��������λ��
	unsigned long data_size;            //���ݳ���
	char* data;                         //��Ƶ���� 
	char* head;
};

class XiWave
{
public:

	bool readwav(char* filename);
	void writewav(char* filename);
	bool cutpro();
	bool checkcut();

	void OneC8bit();
	void TwoC8bit();
	void OneC16bit();
	void TwoC16bit();


	void SetThreshold(int t) {
		int bit = WAVdata.sample_num_bit;
		int test = pow(2, bit);
		threshold = (long int)(test*(t/100.0));
	}
	void SetLast(double t) {
		last = (long int)(t*WAVdata.frequency*(WAVdata.sample_num_bit/8));
	}

	char* get_data_buf() {
		return buf_data_c;
	}
	int get_data_size() {
		return sounddatasize;
	}
	char* get_head_buf() {
		return buf_head_c;
	}
	int get_head_size() {
		return soundheadsize;
	}

private:

	int sounddatasize;
	char* buf_data_c;
	vector<char> sounddata;

	int soundheadsize;
	char* buf_head_c;
	vector<char> soundhead;

	int cutsize = 0;
	vector<long int> cutstart;
	vector<long int> cutstop;

	wav_struct WAVdata;

	long int threshold = 50;
	long int last= 22050;

};

