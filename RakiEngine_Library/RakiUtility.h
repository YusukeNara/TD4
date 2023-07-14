#pragma once
#include <iostream>
#include <cstdlib>
#include <math.h>

#include "RVector.h"

#include <string>

//既存の関数とかぶらないように
//種類が増えたらこの中からさらに名前空間分けする
namespace rutility
{

	//数値から特定の桁を整数で出力（桁は、整数第1位を0、整数部は正、小数部は負で示す）
	int GetDigits(float value, int left, int right);





	RVector3 randomRV3(RVector3 pos1, RVector3 pos2);


	std::wstring charTowstring(const char* orig);
}


