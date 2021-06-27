////////////////////////////////////////////////////////////////////////
//	llint.h
//
//		多倍長精度整数クラスのインターフェース
//

#ifndef __LLINT_H__
#define __LLINT_H__

#include <iostream.h>

#define		KETAMAX		5000		// 扱う桁数

enum	bool	{TRUE = 1, FALSE = 0};

class LLint
{
private:
	unsigned int __far	*LL;		// 多倍長精度整数本体
	unsigned int		LLKeta;		// 桁数
	int		LLSign;					// 符号(+1 or -1)
	bool	OverFlow;				// overflowフラグ
	bool	OverAbote;				// overflowしたときaboteするかどうか
public:
	LLint(char *num = "0");
	LLint(long num);
	~LLint();
	void	SetOverAbote(bool sw);
	bool	IsOverFlow(void);
	friend	bool	IsOverFlow(LLint &num);
	friend	void	disp(LLint &num);
	void operator = (char *num);
	void operator = (long num);
	void operator = (LLint num);
	void operator *= (int num);
	friend	ostream& operator << (ostream &s, LLint &num);
};

#endif	// __LLINT_H__
