#include "pch.h"
#include "XiWave.h"

bool XiWave::readwav(char* filename) {

    fstream wavfile;
    wav_struct WAV;
    wavfile.open(filename, ios::binary | ios::in);


    unsigned short bit = 0x00;


    wavfile.seekg(0x14);
    wavfile.read((char*)&WAV.encodinformat, sizeof(WAV.encodinformat));
    WAVdata.encodinformat = WAV.encodinformat;

    if (WAVdata.encodinformat != 1) {
        cout << "非PCM编码" << endl;
        return false;
    }

    wavfile.seekg(0x16);
    wavfile.read((char*)&WAV.channel, sizeof(WAV.channel));
    WAVdata.channel = WAV.channel;

    wavfile.seekg(0x18);
    wavfile.read((char*)&WAV.frequency, sizeof(WAV.frequency));
    WAVdata.frequency = WAV.frequency;

    wavfile.seekg(0x22);
    wavfile.read((char*)&WAV.sample_num_bit, sizeof(WAV.sample_num_bit));
    WAVdata.sample_num_bit = WAV.sample_num_bit;

    bit = 0x22;

    char tt[4];

    while (1) {
        bit += 0x2;
        wavfile.seekg(bit);
        wavfile.read((char*)&tt, sizeof(tt));

        if (tt[0] == 'd' && tt[1] == 'a' && tt[2] == 't' && tt[3] == 'a') {
            break;
        }
    }
    bit += 4;

    wavfile.seekg(bit);
    wavfile.read((char*)&WAV.data_size, sizeof(WAV.data_size));

    bit += 4;
    //begin to read data.
    WAV.data = new char[WAV.data_size];
    WAV.head = new char[bit];

    wavfile.seekg(0x00);
    wavfile.read((char*)WAV.head, sizeof(char) * bit);


    wavfile.seekg(bit);
    wavfile.read((char*)WAV.data, sizeof(char) * WAV.data_size);

    cout << "音频通道数  ：" << WAV.channel << endl;
    cout << "采样频率    ：" << WAV.frequency << endl;
    cout << "样本位数    ：" << WAV.sample_num_bit << endl;
    cout << "音频数据大小：" << WAV.data_size << endl;

    for (unsigned long i = 0; i < bit; i++) {
        char head_sound = WAV.head[i];
        soundhead.push_back(head_sound);
    }
    soundheadsize = bit;
    buf_head_c = &soundhead[0];

    for (unsigned long i = 0; i < WAV.data_size; i++) {
        char data_sound = WAV.data[i];
        sounddata.push_back(data_sound);
    }

    sounddatasize = WAV.data_size;
    buf_data_c = &sounddata[0];
    wavfile.close();
    delete[] WAV.data;
    return true;
}
void XiWave::writewav(char* filename) {

    fstream wavfile;
    char head_sound;
    char data_sound;

    wavfile.open(filename, ios::binary | ios::out | ios::trunc);

    for (int i = 0; i < soundheadsize; i++) {
        head_sound = soundhead[i];
        wavfile.write((char*)&head_sound, sizeof(char));
    }


    for (int j = 0; j < cutstart[0]; j++) {
        data_sound = sounddata[j];
        wavfile.write((char*)&data_sound, sizeof(char));
    }
    for (int i = 0; i < cutsize - 1; i++) {
        for (int j = cutstop[i]; j < cutstart[i + 1]; j++) {
            data_sound = sounddata[j];
            wavfile.write((char*)&data_sound, sizeof(char));
        }
    }
    for (int j = cutstop[cutsize - 1]; j < sounddatasize; j++) {
        data_sound = sounddata[j];
        wavfile.write((char*)&data_sound, sizeof(char));
    }


    wavfile.close();

}

bool XiWave::cutpro() {
    if (WAVdata.channel == 1) {
        if (WAVdata.sample_num_bit == 8) {
            OneC8bit();
        }
        else if (WAVdata.sample_num_bit == 16) {
            OneC16bit();
        }
        else {
            return false;
        }
    }
    else if (WAVdata.channel == 2) {
        if (WAVdata.sample_num_bit == 8) {
            TwoC8bit();
        }
        else if (WAVdata.sample_num_bit == 16) {
            TwoC16bit();
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
    if (!checkcut()) {
        return false;
    }
    return true;
}

bool XiWave::checkcut() {
    if (cutstart.size() != cutstop.size()) {
        cutstart.pop_back();
    }
    if (cutstart.size() != cutstop.size()) {
        cout << "剪切数量有问题。" << endl;
        return false;
    }
    if (cutstart.size() == 0) {
        cout << "剪切数量有问题。" << endl;
        return false;
    }
    cutsize = cutstart.size();
    return true;
}


void XiWave::OneC8bit() {
    long int value;
    char data_sound;
    int tag = 0;
    bool cutflag = false;

    for (long int i = 0; i < sounddatasize; i++) {
        data_sound = sounddata[i];
        value = (long int)data_sound;
        //TODO: 后续操作
        if (value < threshold) {
            if (tag < last && !cutflag) {
                tag++;
            }
            else if (tag >= last && !cutflag) {
                cutstart.push_back((i - last / 2));
                cutflag = true;
                tag = 0;
            }
        }
        else {
            if (cutflag) {
                cutstop.push_back((i - last / 2));
                cutflag = false;
            }
            tag = 0;
        }
    }

}

void XiWave::TwoC8bit() {
    long int valuel, valuer;

    long int tag = 0;
    bool cutflag = false;

    long int ll;

    char data_soundl, data_soundr;
    for (long int i = 0; i < sounddatasize; i++) {
        data_soundl = sounddata[i];
        data_soundr = sounddata[++i];
        valuel = (long int)data_soundl;
        valuer = (long int)data_soundr;
        //TODO: 后续操作

        if (valuel < threshold && valuer < threshold) {
            if (tag < last && !cutflag) {
                tag++;
            }
            else if (tag >= last && !cutflag) {
                ll = (i - last / 2);
                ll -= ll % 2;
                cutstart.push_back(ll);
                cutflag = true;
                tag = 0;
            }
        }
        else {
            if (cutflag) {
                ll = (i - last / 2);
                ll -= ll % 2;
                cutstop.push_back(ll);
                cutflag = false;
            }
            tag = 0;
        }

    }
}

void XiWave::OneC16bit() {
    long int value;
    long int tag = 0;
    bool cutflag = false;
    long int ll;

    char data_soundhigh, data_soundlow;
    for (long int i = 0; i < sounddatasize; i++) {
        data_soundlow = sounddata[i];
        data_soundhigh = sounddata[++i];
        value = (long)(data_soundlow | data_soundhigh << 8);
        //TODO: 后续操作

        if (value < threshold) {
            if (tag < last && !cutflag) {
                tag++;
            }
            else if (tag >= last && !cutflag) {
                ll = (i - last / 2);
                ll -= ll % 2;
                cutstart.push_back(ll);
                cutflag = true;
                tag = 0;
            }
        }
        else {
            if (cutflag) {
                ll = (i - last / 2);
                ll -= ll % 2;
                cutstop.push_back(ll);
                cutflag = false;
            }
            tag = 0;
        }

    }
    
}

void XiWave::TwoC16bit() {

    long int tag = 0;
    bool cutflag = false;
    long int ll;

    long int valuel, valuer;
    char data_lhigh, data_llow, data_rhigh, data_rlow;
    for (long int i = 0; i < sounddatasize; i++) {
        data_llow = sounddata[i];
        data_lhigh = sounddata[++i];
        data_rlow = sounddata[++i];
        data_rhigh = sounddata[++i];
        valuel = (long int)(data_llow | data_lhigh << 8);
        valuer = (long int)(data_rlow | data_rhigh << 8);
        //TODO: 后续操作

        if (valuel < threshold && valuer < threshold) {
            if (tag < last && !cutflag) {
                tag++;
            }
            else if (tag >= last && !cutflag) {
                ll = (i - last / 2);
                ll -= ll % 4;
                cutstart.push_back(ll);
                cutflag = true;
                tag = 0;
            }
        }
        else {
            if (cutflag) {
                ll = (i - last / 2);
                ll -= ll % 4;
                cutstop.push_back(ll);
                cutflag = false;
            }
            tag = 0;
        }


    }
}
