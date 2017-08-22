#ifndef _SHANREN_ANALYSIS_H_
#define _SHANREN_ANALYSIS_H_


#include <vector>
#include <map>

class SRAnalysis
{
public:
	struct stKindKind {

	};

public:
	SRAnalysis();
	virtual ~SRAnalysis();

public:


	//扑克转换
	static unsigned char switchToCardIndex(unsigned char cbCardData);

	//扑克转换
	static unsigned char switchToCardData(unsigned char cbCardIndex);

	static int isTing(const unsigned char* data, unsigned char count,
		const unsigned char * out_data = nullptr, unsigned char* out_count = nullptr);

	static int isWin(const unsigned char* data, unsigned char count);


protected:

	static int isWin(const unsigned char _cardIndex[], int _duiIndex = 0);

	static int getKeAndShun(const unsigned char _cardIndex[]);

private:


};

#endif // _SHANREN_ANALYSIS_H_