#include <iostream>

std::wstring tetromino[7];

int nFieldWidth = 12;
int nFieldHeight  = 18;

unsigned char *pField = nullptr;

int Rotate( int px, int py, int r ){
  switch ( r % 4 ){
      case 0: return py * 4 + px;
      case 1: return 12 + py - ( px * 4 );
      case 2: return 15 - ( py * 4 ) - px;
      case 3: return 3 - py + ( px * 4 );
    }
    return 0;
}

int main (){

  tetromino[0].append(L"..X.");
  tetromino[0].append(L"..X.");
  tetromino[0].append(L"..x.");
  tetromino[0].append(L"..x.");

  tetromino[1].append(L"..X.");
  tetromino[1].append(L".XX.");
  tetromino[1].append(L".X..");
  tetromino[1].append(L"....");

  tetromino[2].append(L"....");
  tetromino[2].append(L".XX.");
  tetromino[2].append(L".XX.");
  tetromino[2].append(L"....");


}
