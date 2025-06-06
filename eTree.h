#ifndef __ETREE__
#define __ETREE__

#define _CRT_SECURE_NO_WARNINGS
#define WIN_ENV
#define PRINTERROR

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<complex>
#include<algorithm>
#include<string.h>

//연산자 정의
#define NUMBER 0
#define PLUS 1
#define MINUS 2
#define TIMES 3
#define DIVIDE 4
#define EXPONENT 5
#define ABSOLUTE 6

#define SQRT 10
#define NSQRT 11
#define FRAC 12
#define PI 13
#define SIGN 14
#define ABSDIFF 15
#define SIGNDIFF 16

#define	SIN 20
#define COS 21
#define TAN 22
#define SEC 23
#define CSC 24
#define COT 25
#define	SINP 30
#define COSP 31
#define TANP 32
#define SECP 33
#define CSCP 34
#define COTP 35
#define	ARCSIN 40
#define ARCCOS 41
#define ARCTAN 42
#define ARCSEC 43
#define ARCCSC 44
#define ARCCOT 45

#define	SINH 50
#define COSH 51
#define TANH 52
#define SECH 53
#define CSCH 54
#define COTH 55
#define	SINHP 60
#define COSHP 61
#define TANHP 62
#define SECHP 63
#define CSCHP 64
#define COTHP 65
#define	ARCSINH 70
#define ARCCOSH 71
#define ARCTANH 72
#define ARCSECH 73
#define ARCCSCH 74
#define ARCCOTH 75

#define LN 100
#define LOG 101
#define LOG2 102
#define POW 103

#define UNKNOWNFUNC 200

//우선순위 정의
#define PRIORPM 6
#define PRIORMD 7
#define PRIOREXP 8
#define PRIORFUNC 11
#define PRIORNUM 12
#define PRIORLEVEL 20

//함수의 인수를 취할 때 수식의 모양을 고려해야 하는 경우
//숫자가 연속된 문자열도 한 글자씩 인수를 취함
//함수의 인수가 여러 개인 경우, 마지막 인수에만 적용
#define CONSIDERSHAPE 0x10

//^ 다음 요소를 인수로 취하는 함수
#define HASSUPERSCRIPTPARAMETER 0x20

//_ 다음 요소를 인수로 취하는 함수
#define HASSUBSCRIPTPARAMETER 0x40

//옵션 변수 유무
#define HASOPTIONALPARAMETER 0x80


using namespace std;

//----문자열 파싱 관련----
enum class eParseState {
	eParseStart,			//초기 상태
	eParseEnd,				//파싱이 모두 상태
	eReadTokenComplete,		//토큰 하나 읽기를 완료한 상태
	eReadNumber,			//숫자를 읽는 중
	eReadFunction,			//함수를 읽는 중
	eParseError				//파싱 실패
};

enum class eTokenType {
	eNumberTok,				//숫자 토큰
	eIdentifierTok,			//변수 토큰
	eOperatorTok,			//연산자 토큰
	eOpenParTok,			//여는 괄호 토큰
	eCloseParTok,			//닫는 괄호 토큰
	eFunctionTok,			//함수 토큰
	eSpecialTok,			//특수문자로만 이루어진 토큰
	eUnknownTok				//인식하지 않는 토큰
};

enum class eCharacterType {
	eNumberType,			//숫자 문자
	eAlphabetType,			//알파벳 문자
	eOperatorType,			//연산자 문자
	eParenthesisType,		//괄호 문자
	eOptionalParenthesisType,		//[] 문자
	eFunctionStartType,		//백슬래쉬 문자
	eSpecialType,			//특수문자: _
	eSpaceType,				//공백 문자
	eNullType,				//널 문자
	eUnknownType			//인식하지 않는 문자
};

class eTreeToken
{
public:
	eTokenType type;
	string name;
	eTreeToken(eTokenType t,string s)
	{
		type = t;
		name = s;
	}
};

class eTree
{

private:
	eTree(double *n,int prior,int operatorKind,int MPN,int idType);

	int identifierType;		//-1:전역 기본값 따라감, 0:파싱 후 결정

	double *val;					//피연산자, NULL이면 연산자를 의미함
	int operatorType;				//연산자 종류
	int priority;					//연산자 우선순위
	int maxPNum;					//연산자 개수의 최댓값, 이항 연산자는 항상 2이며, sin, cos와 같은 함수는 1임
	vector<eTree *> parameter;		//피연산자 스택

	eTree *deepCopyTree();
	eTree *insertNode(double *n,int prior,int kind,int MPN,int idType);
	bool simplify(double *var);
	void makeTree(vector<eTreeToken> &tokenList,int pr,int start,int end);
	double *calc();
	eTree* differential(double* var);
	
	bool validExprCheck();

public:
	
	eTree();
	eTree(double n);
	eTree(const char *str);
	~eTree();

	void setIdentifierType(int type);
	bool parse(const char* p);
	bool parse(const char* p, const char* name);
	bool parseSuccess;
	
	double calculate();
	double calculate(double value);
	double calculate(const char* value);
	eTree* differential();
	void printTree(const char* path, const char* mode, int level);
};


namespace expTree
{

	#define VARNUM 6
	#define FUNCVARNUM 3
	#define FUNCVARVALNUM 5

	//----함수, 변수 검색 관련----
	#define USEGLOBAL -1
	#define CONSTONLY	0x01
	#define XONLY		0x03
	#define TONLY		0x05
	#define XFUNCARR	0x73
	#define TFUNCARR	0x75

	extern double eVal;
	extern double xVal;
	extern double tVal;
	extern double pi_2;
	extern double aVal;
	extern double bVal;
	extern double cVal;

	extern double *varNum[VARNUM];
	extern double fVal[FUNCVARVALNUM];
	extern double gVal[FUNCVARVALNUM];
	extern double hVal[FUNCVARVALNUM];
	extern double *funcVarNum[FUNCVARNUM][FUNCVARVALNUM];

	extern bool funcVar1Strict[];
	extern bool funcVar2Strict1[];	//첫 번째 인수의 엄격 여부
	extern bool funcVar2Strict2[];	//두 번째 인수의 엄격 여부

	//미분하면서 등장하는 숫자
	extern double *zero;
	extern double *one;
	extern double *minusone;
	extern double *two;
	extern double *ln10;

	extern map<double*,int> predefinedVariables;

	extern bool initialized;
	extern int identifierType;
	extern map<string,pair<int,int>> funcByString;
	extern map<int,pair<string,int>> stringByDef;

	extern void initFuncNumber();
	extern void initStringByDef();
	extern void initPredefinedVariables();
	extern void setIdentifierType(int type);
	extern void initExpTree();

	//----에러 메시지 관련----
	extern char eTreeErrorMessage[1024];
	extern char eTreeErrorMessageHead[64];
	extern int errorMessagePos;
	extern bool isErrorTree;
	extern void initErrorMessage();
	extern void setErrorMessageHead(const char *p);
	extern bool lastCalculateSucceed;

	#ifdef PRINTERROR
	#define writeErrorMessage(a,...) {char tmp123211[256];sprintf(tmp123211,"%%s: %s",a);expTree::errorMessagePos+=sprintf(expTree::eTreeErrorMessage+expTree::errorMessagePos,tmp123211,expTree::eTreeErrorMessageHead,##__VA_ARGS__);}
	#else
	#define writeErrorMessage(a,...) NULL
	#endif


	extern eCharacterType getCharacterType(char p);
	extern char getNextChar(int &i,const char *buf);
	extern void ungetNextChar(int &i);

	extern int getTokenList(vector<eTreeToken> &tokenList,const char *buf);
	extern int getPriority(eTreeToken &e);
	extern void splitNumberToken(vector<eTreeToken> &tokenList,int idx);
	extern int getParameterRange(vector<eTreeToken> &tokenList,bool considerShape,int start);

	extern double *getIdentifierPointer(int idType,string identifier);
	extern bool validTokenCheck(vector<eTreeToken> &tokenList,int *idType);
	

	//----수식 계산 관련----
	extern void SetValue(const char* identifier, double value);
	

	//사용자 정의 함수
	extern bool isSameValue(double a,double b);
	extern bool isNaN(double x);
	extern double my_pow(double a,double b);
	extern double my_ln(double a);
	extern double sign(double x);
	extern double absDiff(double x);
	extern double signDiff(double x);

	extern string operatorNameList[];
	extern const char *tokenTypeStr[];
};

extern double atof_latex(const char* equation);
extern double atof_latex(const char* equation,double value);
extern double atof_latex(const char* equation,const char *value);

typedef eTree latexEquation;
#endif //__ETREE__
