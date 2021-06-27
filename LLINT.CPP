////////////////////////////////////////////////////////////////////////
//	llint.cpp
//
//		多倍長精度整数クラスの実現
//

#include <conio.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "LLint.h"

//////////////////////////////////////////////////////////////////////
// コンストラクタ 文字列バージョン
//
LLint::LLint(char *num)
{
	// 多倍長整数本体のための配列確保
	LL = (unsigned int __far *)_fcalloc(KETAMAX, sizeof(int));
	if(LL == NULL) {
		fputs("LLint(cnstr char*) : メモリが足りません\n",stderr);
		exit(-1);
	}
	
	// 桁数の設定と符号処理
	LLKeta = strlen(num);
	LLSign = 1;
	if(*num == '-') {
		LLSign = -1;
		num++;
		LLKeta--;
	} else if(*num == '+') {
		num++;
		LLKeta--;
	}
	
	// 数値代入と桁数のセット
	int		i = LLKeta - 1;
	while(i >= 0) {
		LL[i] = (unsigned int)((*num)-0x30);
		i--;
		num++;
	}
	
	// overflow時の処理の設定
	OverFlow = FALSE;
	OverAbote = TRUE;
}

//////////////////////////////////////////////////////////////////////
// コンストラクタ 整数バージョン
//
LLint::LLint(long num)
{
	// 多倍長整数本体のための配列確保
	LL = (unsigned int __far *)_fcalloc(KETAMAX, sizeof(int));
	if(LL == NULL) {
		fputs("LLint(cnstr long) : メモリが足りません\n",stderr);
		exit(-1);
	}
	
	// 符号処理
	if(num < 0) {
		num *= -1;
		LLSign = -1;
	} else {
		LLSign = 1;
	}
	
	// 数値代入と桁数のセット
	LLKeta = 0;
	while(num > 0) {
		LL[LLKeta] = (int)(num % 10);
		num /= 10;
		LLKeta++;
	}
	
	// overflow時の処理の設定
	OverFlow = FALSE;
	OverAbote = TRUE;
}

//////////////////////////////////////////////////////////////////////
// ~LLint (デストラクタ)
//
LLint::~LLint()
{
	_ffree(LL);
}


//////////////////////////////////////////////////////////////////////
// オーバーフロー時の対処のセット
//
void LLint::SetOverAbote(bool sw)
{
	OverAbote = sw;
}

//////////////////////////////////////////////////////////////////////
// オーバーフローしてるかどうか(メンバ関数バージョン)
//
bool LLint::IsOverFlow(void)
{
	return OverFlow;
}

//////////////////////////////////////////////////////////////////////
// オーバーフローしてるかどうか(フレンド関数バージョン)
//
bool IsOverFlow(LLint &num)
{
	return num.OverFlow;
}

//////////////////////////////////////////////////////////////////////
// 表示する
//
void	disp(LLint &num)
{
	if(num.LLSign == -1) {
		fputchar('-');
	}
	for(int i=num.LLKeta-1; i>=0; i--) {
		fputchar((char)(num.LL[i]+0x30));
	}
}

//////////////////////////////////////////////////////////////////////
// 代入演算子 "=" 文字列バージョン
//
void LLint::operator = (char *num)
{
	// 桁数の設定と符号処理
	LLKeta = strlen(num);
	LLSign = 1;
	if(*num == '-') {
		LLSign = -1;
		num++;
		LLKeta--;
	} else if(*num == '+') {
		num++;
		LLKeta--;
	}
	
	// 数値代入と桁数のセット
	int		i = LLKeta - 1;
	while(i >= 0) {
		LL[i] = (unsigned int)((*num)-0x30);
		i--;
		num++;
	}
}

//////////////////////////////////////////////////////////////////////
// 代入演算子 "=" 整数バージョン
//
void LLint::operator = (long num)
{
	// 符号処理
	if(num < 0) {
		num *= -1;
		LLSign = -1;
	} else {
		LLSign = 1;
	}
	
	// 数値代入と桁数のセット
	LLKeta = 0;
	while(num > 0) {
		LL[LLKeta] = (int)(num % 10);
		num /= 10;
		LLKeta++;
	}
}

//////////////////////////////////////////////////////////////////////
// 代入演算子 "=" LLintバージョン
//
void LLint::operator = (LLint num)
{
	LLSign = num.LLSign;
	for(unsigned int i=0; i<num.LLKeta; i++) {
		LL[i] = num.LL[i];
	}
	LLKeta = num.LLKeta;
	OverFlow = num.OverFlow;
	OverAbote = num.OverAbote;
}

//////////////////////////////////////////////////////////////////////
// 演算子 "*=" intバージョン
//
void LLint::operator *= (int num)
{
	// 符号の処理
	if(num < 0) {
		LLSign *= -1;
		num *= -1;
	}
	
	// メッセージの通り(汗;)
	if(num >= 10) {
		fputs("LLint(*=) : 2桁以上の演算には対応していない\n",stderr);
		exit(-1);
	}
	
	// 演算本体
	int		temp, kuri = 0;
	for(unsigned int i=0; i<LLKeta; i++) {
		temp = LL[i] * num + kuri;
		if(temp < 10) {				// 桁上がりしてなかったら
			LL[i] = temp;
			kuri = 0;
		} else {					// 桁上がりしてたら
			LL[i] = temp % 10;
			kuri = temp / 10;
			if((i+1) == LLKeta) {
				LLKeta++;
				LL[i+1] = 0;
				// オーバーフローだったら
				if(LLKeta >= KETAMAX) {
					if(OverAbote) {
						fputs("LLint(*=) : オーバーフロー\n",stderr);
						exit(-1);
					} else {
						OverFlow = TRUE;
						return;
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 演算子 "<<"
//
ostream& operator << (ostream &s, LLint &num)
{
	char	*dsp = new char[num.LLKeta+1];
	
	if(dsp == 0) {
		fputs("LLint(<<) : メモリが足りません\n",stderr);
		exit(-1);
	}
	for(unsigned int i=0; i<num.LLKeta; i++) {
		dsp[i] = (char)(num.LL[num.LLKeta-i-1] + 0x30);
	}
	dsp[num.LLKeta] = 0;
	if(num.LLSign > 0) {
		s << dsp;
	} else {
		s << "-" << dsp;
	}
	delete dsp;
	return s;
}
