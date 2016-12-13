#include "mbed.h"
#include <VL6180x.h>
#include <TPA81.h>
#include <SRF05.h>
#include <PCA9547.h>
#include <Servo.h>
#include <Ping.h>

#define N_IRDIST1	0
#define N_IRDIST2	1
#define N_IRDIST3	2
#define N_IRDIST4	3
#define N_TEMP1	4
#define N_TEMP2	5
#define N_SRDIST	6

//Prototype
void rotateServo(int);
void SerialAvailavle();

//LEDs
mbed::DigitalOut ledDebug1(D7);
mbed::DigitalOut ledDebug2(D8);
mbed::DigitalOut ledDebug3(D11);
mbed::DigitalOut ledDebug4(D12);

//RS485インスタンス(複数の関数から使うためグローバル)
mbed::Serial rs(PA_9, PA_10);
mbed::DigitalOut rsSW(D3);
float sendData[] = {1, 2, 3, 4, 5, 6, 7};


//Servo
PwmOut servo(D9);

//==============================================================
void SerialAvailavle()
{
	int getData = rs.getc();
//	printf("%d\n", getData);
	
	switch (getData) {
		case 0:
			//noise
			break;

		case 1:
			wait_ms(1);
			rsSW = 1;
			for (int i = 0; i < 7; i++) {
				rs.putc(sendData[i]);
				wait_ms(1);
			}
			wait_ms(1);
			rsSW = 0;
			break;
		case 2:
			wait_ms(1);
			rsSW = 1;
			rs.putc(sendData[N_IRDIST1]);
			wait_ms(1);
			rsSW = 0;
			break;
		case 3:
			wait_ms(1);
			rsSW = 1;
			rs.putc(sendData[N_IRDIST2]);
			wait_ms(1);
			rsSW = 0;
			break;
		case 4:
			wait_ms(1);
			rsSW = 1;
			rs.putc(sendData[N_IRDIST3]);
			wait_ms(1);
			rsSW = 0;
			break;
		case 5:
			wait_ms(1);
			rsSW = 1;
			rs.putc(sendData[N_IRDIST4]);
			wait_ms(1);
			rsSW = 0;
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			rotateServo(0);
			break;
		case 9:
			rotateServo(180);
			break;
		default:
			break;
	}
}
//==============================================================

void rotateServo(int angle) {
	static const int angleOffset = 2000/180;
	
	servo.pulsewidth_us(500 + angleOffset * angle);
}

//==============================================================

int main(int MBED_UNUSED argc, const char MBED_UNUSED * argv[])
{
	
	const int IRDIST1	= 4;
	const int IRDIST2	= 5;
	const int IRDIST3	= 0;
	const int IRDIST4	= 1;
	const int TEMP1		= 3;
	const int TEMP2		= 2;
	const int SR		= NULL;

	mbed::Serial pc(USBTX, USBRX);
	pc.baud(9600);
	
	//超音波距離センサ
//	SRF05 usonic(D11, D12);
	Ping usonic(D12);
	
	PCA9547 mux(D4, D5, 0xE0);
	mux.select(0);
	
	//温度センサのインスタンス作成
	mux.select(TEMP1);
	TPA81 temp2(D4, D5, 0xD2);
	mux.select(TEMP2);
	TPA81 temp(D4, D5, 0xD0);
	
	//IR距離センサのインスタンス作成
	mux.select(IRDIST1);
	VL6180x irDist(D4, D5, 0x29 << 1);
	irDist.VL6180xInit();
	irDist.VL6180xDefautSettings();
	
	mux.select(IRDIST2);
	irDist.VL6180xInit();
	irDist.VL6180xDefautSettings();
	
	mux.select(IRDIST3);
	irDist.VL6180xInit();
	irDist.VL6180xDefautSettings();
	
	mux.select(IRDIST4);
	irDist.VL6180xInit();
	irDist.VL6180xDefautSettings();
	
	
	//RS485初期設定
	rs.baud(9600);
	rs.attach(SerialAvailavle);
	/* rsSW 0=受信, 1=送信 */
	rsSW = 0;
//
//	while(1){
//		rotateServo(0);
//		wait_ms(600);
//		rotateServo(80);
//		wait_ms(300);
//		rotateServo(70);
//		wait_ms(600);
//	}

	while(1) {
//		ping.Send();
//		wait_us(2500);
//		printf("usonic=%4d\t", ping.Read_cm());
		mux.select(IRDIST1);
		sendData[0] = irDist.getDistance()/2;
		mux.select(IRDIST2);
		sendData[1] = irDist.getDistance()/2;
		mux.select(IRDIST3);
		sendData[2] = irDist.getDistance()/2;
		mux.select(IRDIST4);
		sendData[3] = irDist.getDistance()/2;
		mux.select(TEMP1);
		sendData[4] = temp.getTemp(4);
		mux.select(TEMP2);
		sendData[5] = temp2.getTemp(4);
//		for(int i = 0; i < 7; i++){
//			printf("%4d\t", (int)sendData[i]);
//		}puts("");
	}
}


