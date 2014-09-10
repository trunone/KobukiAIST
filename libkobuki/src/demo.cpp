/**
 * @file demo.cpp
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <conio.h>
#else
#include <termios.h>
struct termios m_oldTermios;
#endif

#include "libkobuki.h"

// アールティ用ロボットネームスペース
using namespace rt_net;

// 特殊キーの定義
enum {
    UP = 65536,
    DOWN,
    LEFT,
    RIGHT,
    SPACE,
    ESCAPE,
};

// キーボードおよび画面処理関数群
static void init_scr();
static void clear_scr();
static void exit_scr();
static int myKbhit();
static int myGetch();


// main関数
int main(int argc, char* argv[])
{
  std::cout << "Starting Kobuki Demo" << std::endl;
  std::string arg = "/dev/ttyUSB0";
  if(argc > 1) {
    arg = argv[1];
  }
  Kobuki *kobuki = createKobuki(KobukiStringArgument(arg));
  //Kobuki *kobuki = createKobuki((arg));
  init_scr();
  bool endflag = false;
  double vx = 0;
  double va = 0;
  double ta = 0.02;
  double tx = 0.002;
  while(!endflag) {
    clear_scr();
    std::cout <<
      "Kobuki Demo: Press 'q' To Exit.\n"
      " KEY CONFIG:\n"
      "              Up(u)                   d:dock  \n"
      " Left(h)      Space(j)     Right(k)   8:LED1 GRN ON   9:LED1 RED ON   0:LED2 GRN ON  -:LED2 RED ON\n"
      "              Down(m)                 i:LED1 GRN OFF  o:LED1 RED OFF  p:LED2 GRN OFF @:LED2 RED OFF\n"
      " POSE     : \n"
      "   X :" << kobuki->getPoseX() << "\n"
      "   Y :" << kobuki->getPoseY() << "\n"
      "   TH:" << kobuki->getPoseTh() << "\n"
      " BUTTON   : "
	      << (kobuki->isButton0()  ? " B0 " : "    ")
	      << (kobuki->isButton1()  ? " B1 " : "    ")
	      << (kobuki->isButton2()  ? " B2 " : "    ")
	      << "\n"
      " BUMPER   : "
	      << (kobuki->isRightBump() ? " RIGHT " : "       ")
	      << (kobuki->isCenterBump()? " CENTER " : "        ")
	      << (kobuki->isLeftBump()  ? " LEFT " : "      ")
	      << "\n"
      " WHEELDROP: "
	      << (kobuki->isRightWheelDrop() ? " RIGHT " : "       ")
	      << (kobuki->isLeftWheelDrop()  ? " LEFT " : "      ")
	      << "\n"
      " CLIFF    : "
	      << (kobuki->isRightCliff() ? " RIGHT " : "       ")
	      << (kobuki->isCenterCliff()? " CENTER " : "        ")
	      << (kobuki->isLeftCliff()  ? " LEFT " : "      ")
	      << "\n"
      " DOCK IR RIGHT  : "
	      << (kobuki->isRightIRFarRight() ? " RIGHT_FAR  " : "            ")
	      << (kobuki->isRightIRFarCenter()? " CENTER_FAR " : "            ")
	      << (kobuki->isRightIRFarLeft()  ? " LEFT_FAR   " : "            ")
	      << "\n"
      "                  "
	      << (kobuki->isRightIRNearRight() ?" RIGHT_NEAR " : "            ")
	      << (kobuki->isRightIRNearCenter()?" CENTER_NEAR" : "            ")
	      << (kobuki->isRightIRNearLeft()  ?" LEFT_NEAR  " : "            ")
	      << "\n"
      " DOCK IR CENTER : "
	      << (kobuki->isCenterIRFarRight() ? " RIGHT_FAR  ": "            ")
	      << (kobuki->isCenterIRFarCenter()? " CENTER_FAR ": "            ")
	      << (kobuki->isCenterIRFarLeft()  ? " LEFT_FAR   ": "            ")
	      << "\n"
      "                  "
	      << (kobuki->isCenterIRNearRight() ?" RIGHT_NEAR ": "            ")
	      << (kobuki->isCenterIRNearCenter()?" CENTER_NEAR": "            ")
	      << (kobuki->isCenterIRNearLeft()  ?" LEFT_NEAR  ": "            ")
	      << "\n"
     " DOCK IR LEFT   : "
	      << (kobuki->isLeftIRFarRight() ? " RIGHT_FAR  " : "            ")
	      << (kobuki->isLeftIRFarCenter()? " CENTER_FAR " : "            ")
	      << (kobuki->isLeftIRFarLeft()  ? " LEFT_FAR   " : "            ")
	      << "\n"
      "                  "
	      << (kobuki->isLeftIRNearRight() ?" RIGHT_NEAR " : "            ")
	      << (kobuki->isLeftIRNearCenter()?" CENTER_NEAR" : "            ")
	      << (kobuki->isLeftIRNearLeft()  ?" LEFT_NEAR  " : "            ")
	      << "\n";
    if(myKbhit()) {
      int c = myGetch();
      c = tolower(c);
      switch(c) {
      case 'q':
	endflag = true;
	break;
	  case 'd':
	kobuki->dock(false);
	break;
      case UP:
      case 'u':
	//kobuki->setTargetVelocity(0.2, 0);
		  vx += tx;
	break;
      case DOWN:
      case 'm':
		  vx -= tx;
//	kobuki->setTargetVelocity(-0.2, 0);
	break;
      case RIGHT:
      case 'k':
	//kobuki->setTargetVelocity(0, -0.3);
		  va -= ta;
	break;
      case LEFT:
      case 'h':
		  va += ta;
	//kobuki->setTargetVelocity(0, 0.3);
	break;
      case SPACE:
      case 'j':
 	//kobuki->setTargetVelocity(0, 0);
		  va = 0;
		  vx = 0;
	break;
      case '8':
	kobuki->setLED1(GREEN, true);
	break;
      case 'i':
	kobuki->setLED1(GREEN, false);
	break;
      case '9':
	kobuki->setLED1(RED, true);
	break;
      case 'o':
	kobuki->setLED1(RED, false);
	break;
      case '0':
	kobuki->setLED2(GREEN, true);
	break;
      case 'p':
	kobuki->setLED2(GREEN, false);
	break;
      case '-':
	kobuki->setLED2(RED, true);
	break;
      case '@':
	kobuki->setLED2(RED, false);
	break;
	
      default:
	break;
      } // switch(c)
	  kobuki->setTargetVelocity(vx, va);
    } // if (myKbhit()) {
  }
  exit_scr();
  delete kobuki;
  return 0;
}


//// 画面用関数定義 ////


static void init_scr() {
#ifdef WIN32
	system("cls");
#else
  	struct termios myTermios;
	tcgetattr(fileno(stdin), &m_oldTermios);
	tcgetattr(fileno(stdin), &myTermios);
    
	myTermios.c_cc[VTIME] = 0;
#ifdef linux
	myTermios.c_cc[VMIN] = 0;
#else // MacOS
	myTermios.c_cc[VMIN] = 1;
#endif
	myTermios.c_lflag &= (~ECHO & ~ICANON);
	tcsetattr(fileno(stdin), TCSANOW, &myTermios);
#endif
}

static void clear_scr() {
#ifdef WIN32
  system("cls");
#else
  system("clear");
#endif
}

static void exit_scr() {
#ifdef WIN32
  system("cls");
#else
  system("reset");
#endif
}

static int myKbhit() {
#ifdef WIN32
  return _kbhit();
#else
  fd_set fdset;
  struct timeval timeout;
  FD_ZERO( &fdset ); 
  FD_SET( 0, &fdset );
  timeout.tv_sec = 0; 
  timeout.tv_usec = 0;
  int ret = select(0+1 , &fdset , NULL , NULL , &timeout );
  return ret;
#endif
}

static int myGetch() {
#ifdef WIN32
	char c = _getch();
	if(c == 0xFFFFFFE0) {
		c = _getch();
		if(c == 0x48) return UP;
		if(c == 0x50) return DOWN;
		if(c == 0x4B) return LEFT;
		if(c == 0x4D) return RIGHT;
	} else if(c == ' ') {
		 return SPACE;
	}

	return c;
#else
    int keys[5] = {-1, -1, -1, -1, -1};
    int key = getchar();
	switch(key) {
        case -1:
        case 0:
            return -1;
            
        case ' ':
            return SPACE;
        case 27:
            key = getchar();
            switch(key) {
                case -1:
                    return ESCAPE;
                case 79:
                    key = getchar();
                    return key;
                case '[':
                    for(int i = 0;i < 5;i++) {
                        if(key == -1 || key == 27) break;
                        keys[i] = key = getchar();
                    }
                    ungetc(key, stdin);
                    
                    switch(keys[0]) {
                        case 65: return UP;
                        case 66: return DOWN;
                        case 67: return RIGHT;
                        case 68: return LEFT;
                        default: return keys[0];
                    }
            }
	}
	return key;
#endif
}
