#include "eTree.h"

double expTree::eVal = 2.71828182845904523536;
double expTree::xVal = 2.0;
double expTree::tVal = 0.0;
double expTree::pi_2 = 3.14159265358979323846;
double expTree::aVal = 0.0;
double expTree::bVal = 0.0;
double expTree::cVal = 0.0;
double *expTree::varNum[VARNUM] = {&expTree::eVal,&expTree::xVal,&expTree::tVal,&expTree::aVal,&expTree::bVal,&expTree::cVal};

//함수를 수식으로 쓸 수 없으나 임의로 함숫값, 도함숫값을 알 수 있는 경우
double expTree::fVal[FUNCVARVALNUM] = {0,};	//f(x), f'(x), f''(x), ...를 담는 변수
double expTree::gVal[FUNCVARVALNUM] = {0,};	//g(x), g'(x), g''(x), ...를 담는 변수
double expTree::hVal[FUNCVARVALNUM] = {0,};	//h(x), h'(x), h''(x), ...를 담는 변수
double *expTree::funcVarNum[FUNCVARNUM][FUNCVARVALNUM] = {
	{&expTree::fVal[0],&expTree::fVal[1],&expTree::fVal[2],&expTree::fVal[3],&expTree::fVal[4]},
	{&expTree::gVal[0],&expTree::gVal[1],&expTree::gVal[2],&expTree::gVal[3],&expTree::gVal[4]},
	{&expTree::hVal[0],&expTree::hVal[1],&expTree::hVal[2],&expTree::hVal[3],&expTree::hVal[4]}
};
bool expTree::funcVar1Strict[] = {false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false};
bool expTree::funcVar2Strict1[] = {true,true,true,true,true,true,true,false,true};	//첫 번째 인수의 엄격 여부
bool expTree::funcVar2Strict2[] = {false,false,false,false,false,false,false,false,true};	//두 번째 인수의 엄격 여부

//미분하면서 등장하는 숫자
double *expTree::zero = new double(0.0);
double *expTree::one = new double(1.0);
double *expTree::minusone = new double(-1.0);
double *expTree::two = new double(2.0);
double *expTree::ln10 = new double(log(10.0));

map<double*,int> expTree::predefinedVariables;

bool expTree::initialized = false;
int expTree::identifierType = CONSTONLY;
map<string,pair<int,int>> expTree::funcByString;
map<int,pair<string,int>> expTree::stringByDef;

void expTree::initFuncNumber()
{
	expTree::funcByString["\\abs"] = pair<int,int>(ABSOLUTE,1 | CONSIDERSHAPE);

	expTree::funcByString["\\sqrt"] = pair<int,int>(SQRT,1 | CONSIDERSHAPE | HASOPTIONALPARAMETER);
	expTree::funcByString["nsqrt"] = pair<int,int>(NSQRT,2 | CONSIDERSHAPE);
	expTree::funcByString["\\frac"] = pair<int,int>(FRAC,2 | CONSIDERSHAPE);
	expTree::funcByString["\\dfrac"] = pair<int,int>(FRAC,2 | CONSIDERSHAPE);
	expTree::funcByString["\\pi"] = pair<int,int>(PI,0);

	expTree::funcByString["\\sin"] = pair<int,int>(SIN,1 | HASSUPERSCRIPTPARAMETER);
	expTree::funcByString["\\cos"] = pair<int,int>(COS,1 | HASSUPERSCRIPTPARAMETER);
	expTree::funcByString["\\tan"] = pair<int,int>(TAN,1 | HASSUPERSCRIPTPARAMETER);
	expTree::funcByString["\\sec"] = pair<int,int>(SEC,1 | HASSUPERSCRIPTPARAMETER);
	expTree::funcByString["\\csc"] = pair<int,int>(CSC,1 | HASSUPERSCRIPTPARAMETER);
	expTree::funcByString["\\cosec"] = pair<int,int>(CSC,1 | HASSUPERSCRIPTPARAMETER);
	expTree::funcByString["\\cot"] = pair<int,int>(COT,1 | HASSUPERSCRIPTPARAMETER);

	expTree::funcByString["\\sin^"] = pair<int,int>(SINP,2);
	expTree::funcByString["\\cos^"] = pair<int,int>(COSP,2);
	expTree::funcByString["\\tan^"] = pair<int,int>(TANP,2);
	expTree::funcByString["\\sec^"] = pair<int,int>(SECP,2);
	expTree::funcByString["\\csc^"] = pair<int,int>(CSCP,2);
	expTree::funcByString["\\cosec^"] = pair<int,int>(CSCP,2);
	expTree::funcByString["\\cot^"] = pair<int,int>(COTP,2);

	expTree::funcByString["\\arcsin"] = pair<int,int>(ARCSIN,1);
	expTree::funcByString["\\arccos"] = pair<int,int>(ARCCOS,1);
	expTree::funcByString["\\arctan"] = pair<int,int>(ARCTAN,1);
	expTree::funcByString["\\arcsec"] = pair<int,int>(ARCSEC,1);
	expTree::funcByString["\\arccsc"] = pair<int,int>(ARCCSC,1);
	expTree::funcByString["\\arccot"] = pair<int,int>(ARCCOT,1);

	expTree::funcByString["\\sinh"] = pair<int,int>(SINH,1 | HASSUPERSCRIPTPARAMETER);
	expTree::funcByString["\\cosh"] = pair<int,int>(COSH,1 | HASSUPERSCRIPTPARAMETER);
	expTree::funcByString["\\tanh"] = pair<int,int>(TANH,1 | HASSUPERSCRIPTPARAMETER);
	expTree::funcByString["\\sech"] = pair<int,int>(SECH,1 | HASSUPERSCRIPTPARAMETER);
	expTree::funcByString["\\csch"] = pair<int,int>(CSCH,1 | HASSUPERSCRIPTPARAMETER);
	expTree::funcByString["\\coth"] = pair<int,int>(COTH,1 | HASSUPERSCRIPTPARAMETER);

	expTree::funcByString["\\sinh^"] = pair<int,int>(SINHP,2);
	expTree::funcByString["\\cosh^"] = pair<int,int>(COSHP,2);
	expTree::funcByString["\\tanh^"] = pair<int,int>(TANHP,2);
	expTree::funcByString["\\sech^"] = pair<int,int>(SECHP,2);
	expTree::funcByString["\\csch^"] = pair<int,int>(CSCHP,2);
	expTree::funcByString["\\cosech^"] = pair<int,int>(CSCHP,2);
	expTree::funcByString["\\coth^"] = pair<int,int>(COTHP,2);

	expTree::funcByString["\\arcsinh"] = pair<int,int>(ARCSINH,1);
	expTree::funcByString["\\arccosh"] = pair<int,int>(ARCCOSH,1);
	expTree::funcByString["\\arctanh"] = pair<int,int>(ARCTANH,1);
	expTree::funcByString["\\arcsech"] = pair<int,int>(ARCSECH,1);
	expTree::funcByString["\\arccsch"] = pair<int,int>(ARCCSCH,1);
	expTree::funcByString["\\arccoth"] = pair<int,int>(ARCCOTH,1);

	expTree::funcByString["\\ln"] = pair<int,int>(LN,1);
	expTree::funcByString["\\log"] = pair<int,int>(LOG,1 | HASSUBSCRIPTPARAMETER);
	expTree::funcByString["\\log_"] = pair<int,int>(LOG2,2);
	expTree::funcByString["\\pow"] = pair<int,int>(POW,2 | CONSIDERSHAPE);
}

void expTree::initStringByDef()
{
	expTree::stringByDef[PLUS] = pair<string,int>("+",2);
	expTree::stringByDef[MINUS] = pair<string,int>("-",2);
	expTree::stringByDef[TIMES] = pair<string,int>("*",2);
	expTree::stringByDef[DIVIDE] = pair<string,int>("/",2);
	expTree::stringByDef[EXPONENT] = pair<string,int>("^",2);

	expTree::stringByDef[ABSOLUTE] = pair<string,int>("\\abs",1);
	expTree::stringByDef[FRAC] = pair<string,int>("\\frac(\\dfrac)",2);
	expTree::stringByDef[SQRT] = pair<string,int>("\\sqrt",1);
	expTree::stringByDef[NSQRT] = pair<string,int>("nsqrt",2);
	expTree::stringByDef[PI] = pair<string,int>("\\pi",0);
	expTree::stringByDef[SIGN] = pair<string,int>("sign",1);
	expTree::stringByDef[ABSDIFF] = pair<string,int>("absDiff",1);
	expTree::stringByDef[SIGNDIFF] = pair<string,int>("signDiff",1);

	expTree::stringByDef[SIN] = pair<string,int>("\\sin",1);
	expTree::stringByDef[COS] = pair<string,int>("\\cos",1);
	expTree::stringByDef[TAN] = pair<string,int>("\\tan",1);
	expTree::stringByDef[SEC] = pair<string,int>("\\sec",1);
	expTree::stringByDef[CSC] = pair<string,int>("\\csc(\\cosec)",1);
	expTree::stringByDef[COT] = pair<string,int>("\\cot" ,1);

	expTree::stringByDef[SINP] = pair<string,int>("\\sin^",2);
	expTree::stringByDef[COSP] = pair<string,int>("\\cos^",2);
	expTree::stringByDef[TANP] = pair<string,int>("\\tan^",2);
	expTree::stringByDef[SECP] = pair<string,int>("\\sec^",2);
	expTree::stringByDef[CSCP] = pair<string,int>("\\csc^(\\cosec^)",2);
	expTree::stringByDef[COTP] = pair<string,int>("\\cot^",2);

	expTree::stringByDef[ARCSIN] = pair<string,int>("\\arcsin",1);
	expTree::stringByDef[ARCCOS] = pair<string,int>("\\arccos",1);
	expTree::stringByDef[ARCTAN] = pair<string,int>("\\arctan",1);
	expTree::stringByDef[ARCSEC] = pair<string,int>("\\arcsec",1);
	expTree::stringByDef[ARCCSC] = pair<string,int>("\\arccsc(\\arccosec)",1);
	expTree::stringByDef[ARCCOT] = pair<string,int>("\\arccot" ,1);

	
	expTree::stringByDef[SINH] = pair<string,int>("\\sinh",1);
	expTree::stringByDef[COSH] = pair<string,int>("\\cosh",1);
	expTree::stringByDef[TANH] = pair<string,int>("\\tanh",1);
	expTree::stringByDef[SECH] = pair<string,int>("\\sech",1);
	expTree::stringByDef[CSCH] = pair<string,int>("\\csch",1);
	expTree::stringByDef[COTH] = pair<string,int>("\\coth",1);

	expTree::stringByDef[SINH] = pair<string,int>("\\sinh^",2);
	expTree::stringByDef[COSH] = pair<string,int>("\\cosh^",2);
	expTree::stringByDef[TANH] = pair<string,int>("\\tanh^",2);
	expTree::stringByDef[SECH] = pair<string,int>("\\sech^",2);
	expTree::stringByDef[CSCH] = pair<string,int>("\\csch^(\\cosech^)",2);
	expTree::stringByDef[COTH] = pair<string,int>("\\coth^",2);

	expTree::stringByDef[ARCSINH] = pair<string,int>("\\arcsinh",1);
	expTree::stringByDef[ARCCOSH] = pair<string,int>("\\arccosh",1);
	expTree::stringByDef[ARCTANH] = pair<string,int>("\\arctanh",1);
	expTree::stringByDef[ARCSECH] = pair<string,int>("\\arcsech",1);
	expTree::stringByDef[ARCCSCH] = pair<string,int>("\\arccsch",1);
	expTree::stringByDef[ARCCOTH] = pair<string,int>("\\arccoth",1);

	expTree::stringByDef[LN] = pair<string,int>("\\ln",1);
	expTree::stringByDef[LOG] = pair<string,int>("\\log",1);
	expTree::stringByDef[LOG2] = pair<string,int>("\\log_",2);
	expTree::stringByDef[POW] = pair<string,int>("\\pow",2);

}

void expTree::initPredefinedVariables()
{
	expTree::predefinedVariables[&expTree::eVal] = 0;
	expTree::predefinedVariables[&expTree::xVal] = 0;
	expTree::predefinedVariables[&expTree::tVal] = 0;
	expTree::predefinedVariables[&expTree::pi_2] = 0;
	expTree::predefinedVariables[&expTree::aVal] = 0;
	expTree::predefinedVariables[&expTree::bVal] = 0;
	expTree::predefinedVariables[&expTree::cVal] = 0;
	
	expTree::predefinedVariables[&expTree::fVal[0]]=0;
	expTree::predefinedVariables[&expTree::fVal[1]]=0;
	expTree::predefinedVariables[&expTree::fVal[2]]=0;
	expTree::predefinedVariables[&expTree::fVal[3]]=0;
	expTree::predefinedVariables[&expTree::fVal[4]]=0;
	expTree::predefinedVariables[&expTree::gVal[0]]=0;
	expTree::predefinedVariables[&expTree::gVal[1]]=0;
	expTree::predefinedVariables[&expTree::gVal[2]]=0;
	expTree::predefinedVariables[&expTree::gVal[3]]=0;
	expTree::predefinedVariables[&expTree::gVal[4]]=0;
	expTree::predefinedVariables[&expTree::hVal[0]]=0;
	expTree::predefinedVariables[&expTree::hVal[1]]=0;
	expTree::predefinedVariables[&expTree::hVal[2]]=0;
	expTree::predefinedVariables[&expTree::hVal[3]]=0;
	expTree::predefinedVariables[&expTree::hVal[4]]=0;

	expTree::predefinedVariables[expTree::zero] = 0;
	expTree::predefinedVariables[expTree::one] = 0;
	expTree::predefinedVariables[expTree::minusone] = 0;
	expTree::predefinedVariables[expTree::two] = 0;
	expTree::predefinedVariables[expTree::ln10] = 0;
}

//하위 4비트: e, x, t,  상위 4비트: f, g, h
void expTree::setIdentifierType(int type)
{
	expTree::identifierType = type;
}

void expTree::initExpTree()
{
	expTree::initFuncNumber();
	expTree::initStringByDef();
	expTree::initPredefinedVariables();
	expTree::initErrorMessage();
	expTree::setErrorMessageHead(NULL);
	expTree::initialized = true;
}



char expTree::eTreeErrorMessage[1024];
char expTree::eTreeErrorMessageHead[64];
int expTree::errorMessagePos = 0;
bool expTree::isErrorTree = false;

void expTree::initErrorMessage()
{
	expTree::errorMessagePos = 0;
	expTree::isErrorTree = false;
	memset(expTree::eTreeErrorMessage,0,1024);
}

void expTree::setErrorMessageHead(const char *p)
{
	if (p)
	{
		sprintf(expTree::eTreeErrorMessageHead,"%s",p);
	}
	else
	{
		sprintf(expTree::eTreeErrorMessageHead,"error message");
	}
}





eTree *eTree::deepCopyTree()
{
	eTree *t = new eTree;

	t->identifierType = this->identifierType;

	if(expTree::predefinedVariables.find(this->val) != expTree::predefinedVariables.end() || this->val == NULL)
		t->val = this->val;
	else
	{
		t->val = new double;
		*t->val = *this->val;
	}
	
	t->operatorType = this->operatorType;
	t->priority = this->priority;
	t->maxPNum = this->maxPNum;

	for(int i=0;i<this->parameter.size();i++)
	{
		t->parameter.emplace_back(this->parameter[i]->deepCopyTree());
	}

	return t;
}

eCharacterType expTree::getCharacterType(char p)
{
	if (p >= '0' && p <= '9' || p == '.')
		return eCharacterType::eNumberType;
	else if (p >= 'a' && p <= 'z' || p >= 'A' && p <= 'Z')
		return eCharacterType::eAlphabetType;
	else if (p == '+' || p == '-' || p == '*' || p == '/' || p == '^')
		return eCharacterType::eOperatorType;
	else if (p == '(' || p == ')' || p == '{' || p == '}' || p == '[' || p == ']')
		return eCharacterType::eParenthesisType;
	else if (p == '\\')
		return eCharacterType::eFunctionStartType;
	else if (p == '_')
		return eCharacterType::eSpecialType;
	else if (p == ' ' || p == '\t' || p == '\n' || p == '\r')
		return eCharacterType::eSpaceType;
	else if (p == 0)
		return eCharacterType::eNullType;
	else
		return eCharacterType::eUnknownType;
}

char expTree::getNextChar(int &i,const char *buf)
{
	return buf[++i];
}

void expTree::ungetNextChar(int &i)
{
	--i;
}




int expTree::getTokenList(vector<eTreeToken> &tokenList,const char *buf)
{
	eParseState state = eParseState::eParseStart;
	eTokenType tokenType;
	int i = -1;
	int tokenStart, tokenEnd;
	string *prevTok = NULL;
	int parLev = 0;
	vector<int> parameterNum;
	vector<int> parameterType;

	parameterNum.emplace_back(0);
	parameterType.emplace_back(0);
	
	while(1)
	{
		char c = expTree::getNextChar(i,buf);

		if (state == eParseState::eParseStart)
		{
			switch (expTree::getCharacterType(c))
			{
				case eCharacterType::eNumberType:
					//마지막 인수가 아니거나, 마지막 인수이면서 모양을 고려하는 함수의 경우 한 글자만 인수로 취급
					if(parameterNum.back() > 1 || (parameterNum.back() == 1 && (parameterType.back() & CONSIDERSHAPE) != 0))
					{
						tokenStart = i;
						tokenEnd = i + 1;
						tokenType = eTokenType::eNumberTok;
						state = eParseState::eReadTokenComplete;

						if(parameterNum.back() > 0)
							--parameterNum.back();
					}
					else
					{
						tokenStart = i;
						state = eParseState::eReadNumber;
					}
					
					break;

				case eCharacterType::eAlphabetType:
					tokenStart = i;
					tokenEnd = i + 1;
					tokenType = eTokenType::eIdentifierTok;
					state = eParseState::eReadTokenComplete;
					break;

				case eCharacterType::eOperatorType:
					tokenStart = i;
					tokenEnd = i + 1;
					tokenType = eTokenType::eOperatorTok;
					state = eParseState::eReadTokenComplete;
					break;

				case eCharacterType::eParenthesisType:
					tokenStart = i;
					tokenEnd = i + 1;
					if(c=='(' || c=='{' || c=='[')
					{
						tokenType = eTokenType::eOpenParTok;
						++parLev;
						parameterNum.emplace_back(0);
						parameterType.emplace_back(0);
					}
					else
					{
						tokenType = eTokenType::eCloseParTok;
						--parLev;
						parameterNum.pop_back();
						parameterType.pop_back();
					}
					state = eParseState::eReadTokenComplete;
					break;

				case eCharacterType::eFunctionStartType:
					tokenStart = i;
					state = eParseState::eReadFunction;
					break;

				case eCharacterType::eSpecialType:
					tokenStart = i;
					tokenEnd = i + 1;
					tokenType = eTokenType::eSpecialTok;
					state = eParseState::eReadTokenComplete;
					break;

				case eCharacterType::eSpaceType:
					state = eParseState::eParseStart;
					break;

				case eCharacterType::eNullType:
					state = eParseState::eParseEnd;
					break;

				case eCharacterType::eUnknownType:
					tokenStart = i;
					tokenEnd = i + 1;
					tokenType = eTokenType::eUnknownTok;
					state = eParseState::eReadTokenComplete;
					break;
			}
		}
		else if (state == eParseState::eParseEnd)
		{
			break;
		}
		else if (state == eParseState::eReadNumber)
		{
			if (expTree::getCharacterType(c) != eCharacterType::eNumberType)
			{
				tokenEnd = i;
				expTree::ungetNextChar(i);
				tokenType = eTokenType::eNumberTok;
				state = eParseState::eReadTokenComplete;
			}
		}
		else if (state == eParseState::eReadFunction)
		{
			if (expTree::getCharacterType(c) != eCharacterType::eAlphabetType)
			{
				if(i - tokenStart == 1)
				{
					tokenEnd = i+1;
					tokenType = eTokenType::eSpecialTok;
					state = eParseState::eReadTokenComplete;
				}
				else
				{
					tokenEnd = i;
					expTree::ungetNextChar(i);
					tokenType = eTokenType::eFunctionTok;
					state = eParseState::eReadTokenComplete;
				}
			}
		}

		if (state == eParseState::eReadTokenComplete)
		{
			string tokenString = string(buf+tokenStart,buf+tokenEnd);
			state = eParseState::eParseStart;
			bool changePrevTok = false;

			if(tokenString == "^")
			{
				if(prevTok)
				{
					if(funcByString[*prevTok].second & HASSUPERSCRIPTPARAMETER)
					//if(*prevTok == "\\sin" || *prevTok == "\\cos" || *prevTok == "\\tan" || *prevTok == "\\sec" || *prevTok == "\\csc" || *prevTok == "\\cosec" || *prevTok == "\\cot"
					// || *prevTok == "\\sinh" || *prevTok == "\\cosh" || *prevTok == "\\tanh" || *prevTok == "\\sech" || *prevTok == "\\csch" || *prevTok == "\\cosech" || *prevTok == "\\coth")
					{
						*prevTok += "^";
						changePrevTok = true;
					}
				}
			}
			else if(tokenString == "_")
			{
				if(prevTok)
				{
					if(funcByString[*prevTok].second & HASSUBSCRIPTPARAMETER)
					//if(*prevTok == "\\log")
					{
						*prevTok += "_";
						changePrevTok = true;
					}
				}
			}
			else if(tokenString == "[")
			{
				if(prevTok)
				{
					if(funcByString[*prevTok].second & HASOPTIONALPARAMETER)
					//if(*prevTok == "\\sqrt")
					{
						(*prevTok)[0] = 'n';
					}
				}
			}

			if(changePrevTok)
			{
				if(funcByString.find(*prevTok) != funcByString.end())
				{
					parameterNum.back() = (funcByString[*prevTok].second & 0x0F);
					parameterType.back() = (funcByString[*prevTok].second & CONSIDERSHAPE);
				}
			}
			else
			{
				if(funcByString.find(tokenString) != funcByString.end())
				{
					parameterNum.back() = (funcByString[tokenString].second & 0x0F);
					parameterType.back() = (funcByString[tokenString].second & CONSIDERSHAPE);
				}

				tokenList.emplace_back(eTreeToken(tokenType,tokenString));
				prevTok = &tokenList.back().name;
			}
		}
	}

	return parLev;
}

int expTree::getPriority(eTreeToken &e)
{
	if(e.type == eTokenType::eNumberTok)
	{
		return PRIORNUM;
	}
	else if(e.type == eTokenType::eIdentifierTok)
	{
		return PRIORNUM;
	}
	else if(e.type == eTokenType::eOperatorTok)
	{
		if(e.name == "+" || e.name == "-")
			return PRIORPM;
		else if(e.name == "*" || e.name == "/")
			return PRIORMD;
		else if(e.name == "^")
			return PRIOREXP;
	}
	else if(e.type == eTokenType::eOpenParTok || e.type == eTokenType::eCloseParTok)
	{
		return PRIORNUM;
	}
	else if(e.type == eTokenType::eFunctionTok)
	{
		return PRIORFUNC;
	}
	else if(e.type == eTokenType::eSpecialTok)
	{
		return PRIORNUM;
	}
	else if(e.type == eTokenType::eUnknownTok)
	{
		return PRIORNUM;
	}

	return -1;
}

void expTree::splitNumberToken(vector<eTreeToken> &tokenList,int idx)
{
	if(tokenList[idx].name.size()>1)
	{
		eTreeToken split(eTokenType::eNumberTok,tokenList[idx].name.substr(1));
		tokenList.insert(tokenList.begin()+idx+1,split);
		tokenList[idx].name = tokenList[idx].name.substr(0,1);
	}
}

//시작 위치에서 함수의 인수를 하나 찾아서 인수의 끝 위치를 리턴함.
int expTree::getParameterRange(vector<eTreeToken> &tokenList,bool considerShape,int start)
{
	//printf("startFrom:%d, tokenType:%d, considerShape:%d\n",start,tokenList[start].type,considerShape);
	int result = -1;

	if(tokenList[start].type == eTokenType::eOpenParTok)
	{
		int parLev = 1;
		for(int i=start+1;i<tokenList.size();i++)
		{
			if(tokenList[i].type == eTokenType::eOpenParTok)
				++parLev;
			else if(tokenList[i].type == eTokenType::eCloseParTok)
				--parLev;
			if(parLev == 0 || i == tokenList.size()-1)
			{
				result = i+1;
				break;
			}
		}
	}
	
	else if(tokenList[start].type == eTokenType::eFunctionTok)
	{
		int funcType = funcByString[tokenList[start].name].first;
		int parNum = (funcByString[tokenList[start].name].second & 0x0F);
		int parType = (funcByString[tokenList[start].name].second & CONSIDERSHAPE);
		
		if(parNum == 0)
			result = start+1;
		else
		{
			int startPos = start+1;
			int endPos = startPos;
			
			//startPos가 유효한 범위일 때만 인수 추가. 
			for(int j=0;j<parNum && startPos<tokenList.size();j++)
			{
				bool considerShape2 = (parType != 0 || j < parNum-1);
				endPos = getParameterRange(tokenList,considerShape2,startPos);
				startPos = endPos;
			}

			result = endPos;
		}
	}
	else
	{
		if(considerShape)
			result = start+1;
		else
		{
			int i=start;
			for(;i<tokenList.size();i++)
			{
				if(tokenList[i].type == eTokenType::eFunctionTok)
				{
					int parNum = (funcByString[tokenList[i].name].second & 0x0F);

					if(parNum > 0)
					{
						//printf("function %s parNum>0\n",tokenList[i].name.data());
						break;
					}
				}
				else if(tokenList[i].type == eTokenType::eOperatorTok && tokenList[i].name == "^")
				{
					i = expTree::getParameterRange(tokenList,true,i+1) - 1;
				}
				else if(tokenList[i].type != eTokenType::eNumberTok && tokenList[i].type != eTokenType::eIdentifierTok)
				{
					//printf("split token, name=%s, type=%d\n",tokenList[i].name.data(),tokenList[i].type);
					break;
				}
			}

			result = i;
		}
	}
	
	//printf("startFrom %d, end at %d\n",start,result);
	return result;
}

double *expTree::getIdentifierPointer(int idType,string identifier)
{
	if(identifier == "e")
		return &expTree::eVal;
	else if(identifier == "x") 
		return ((idType & 0x02) ? &expTree::xVal : NULL);
	else if(identifier == "t") 
		return ((idType & 0x04) ? &expTree::tVal : NULL);
	else if(identifier == "a")
		return &expTree::aVal;
	else if(identifier == "b")
		return &expTree::bVal;
	else if(identifier == "c")
		return &expTree::cVal;
	else if(identifier == "f") 
		return ((idType & 0x10) ? &expTree::fVal[0] : NULL);
	else if(identifier == "g") 
		return ((idType & 0x20) ? &expTree::gVal[0] : NULL);
	else if(identifier == "h") 
		return ((idType & 0x40) ? &expTree::hVal[0] : NULL);
	else
		return NULL;
}

bool expTree::validTokenCheck(vector<eTreeToken> &tokenList,int *idType)
{
	bool res = true;

	for(int i=0;i<tokenList.size();i++)
	{
		if(tokenList[i].type == eTokenType::eUnknownTok)
		{
			writeErrorMessage("Unknown token: %s\n",tokenList[i].name.data());
			res = false;
		}
		else if(tokenList[i].type == eTokenType::eIdentifierTok)
		{
			if(*idType == 0)
			{
				if(tokenList[i].name == "x")
				{
					*idType &= 0xF0;
					*idType |= XONLY;
				}
				else if(tokenList[i].name == "t")
				{
					*idType &= 0xF0;
					*idType |= TONLY;
				}
			}
			else
			{
				if(expTree::getIdentifierPointer(*idType,tokenList[i].name) == NULL)
				{
					writeErrorMessage("Cannot use identirier: %s\n",tokenList[i].name.data());
					res = false;
				}
			}
		}
		else if(tokenList[i].type == eTokenType::eFunctionTok)
		{
			if(expTree::funcByString.find(tokenList[i].name) == expTree::funcByString.end())
			{
				writeErrorMessage("Unknown function: %s\n",tokenList[i].name.data());
				res = false;
			}
		}
	}

	if(*idType == 0)
		*idType = CONSTONLY;

	return res;
}





void expTree::SetValue(const char* identifier, double value)
{
	if (strcmp(identifier, "x") == 0)
		expTree::xVal = value;
	else if (strcmp(identifier, "t") == 0)
		expTree::tVal = value;
	else if (strcmp(identifier, "a") == 0)
		expTree::aVal = value;
	else if (strcmp(identifier, "b") == 0)
		expTree::bVal = value;
	else if (strcmp(identifier, "c") == 0)
		expTree::cVal = value;
}

void expTree::SetValue(const char* identifier, const char* value)
{
	eTree *val = new eTree;
	val->setIdentifierType(CONSTONLY);
	val->parse(value);

	if(val->parseSuccess)
	{
		double res = val->calculate();

		if (strcmp(identifier, "x") == 0)
			expTree::xVal = res;
		else if (strcmp(identifier, "t") == 0)
			expTree::tVal = res;
		else if (strcmp(identifier, "a") == 0)
			expTree::aVal = res;
		else if (strcmp(identifier, "b") == 0)
			expTree::bVal = res;
		else if (strcmp(identifier, "c") == 0)
			expTree::cVal = res;
	}

	delete val;
}

bool expTree::isSameValue(double a,double b)
{
	return (a-b<0.000001 && b-a<0.000001);
}

bool expTree::isNaN(double x)
{
	return (x!=x);
}

double expTree::my_pow(double a,double b)
{
	if(a>=-0.000001 && a<=0.000001 && b>0)
		return 0;
	if(abs(fmod(b,1.0)) >= 0.999999 || abs(fmod(b,1.0)) <= 0.000001)	//정수 지수
	{
		int n = floor(b+0.5);
		if(n==0)
			return 1;
		else if(n==1)
			return a;
		else if(n==-1)
			return 1/a;
		else
		{
			if(n%2)	//n이 홀수
			{
				return a*expTree::my_pow(a,(n-1)/2)*expTree::my_pow(a,(n-1)/2);
			}
			else	//n이 짝수
			{
				return expTree::my_pow(a,n/2)*expTree::my_pow(a,n/2);
			}
		}
	}
	else
	{
		return pow(a,b);
	}
}

double expTree::my_ln(double a)
{
	if(a>0)
		return log(a);
	else
		return 0;
}

double expTree::sign(double x)
{
	if(expTree::isSameValue(x,0))
		return 0;
	else if(x>0)
		return 1;
	else
		return -1;
}

double expTree::absDiff(double x)
{
	if(expTree::isSameValue(x,0))
		return NAN;
	else if(x>0)
		return 1;
	else
		return -1;
}

double expTree::signDiff(double x)
{
	if(expTree::isSameValue(x,0))
		return NAN;
	else
		return 0;
}


bool lastCalculateSucceed = false;

double atof_latex(const char* function)
{
	double res = 0;
	eTree* tree = new eTree;
	tree->setIdentifierType(CONSTONLY);
	tree->parse(function);

	if(tree->parseSuccess)
	{
		res = tree->calculate();
	}

	delete tree;
	return res;
}

double atof_latex(const char* function,double value)
{
	double res = 0;
	eTree* tree = new eTree;
	tree->parse(function);

	if(tree->parseSuccess)
	{
		res = tree->calculate(value);
	}

	delete tree;
	return res;
}

double atof_latex(const char* function,const char *value)
{
	return atof_latex(function,atof_latex(value));
}

// 시험용 코드

string expTree::operatorNameList[] = {"plus", "minus", "times", "divide","exponent","abs","sin","cos","tan","sec","csc","cot","ln","sqrt","nsqrt","sinh","cosh","tanh","sech","csch","coth","log","sinp","cosp","tanp","secp","cscp","cotp","log2","pow","frac","pi"};
const char *expTree::tokenTypeStr[] = {"숫자","변수","연산자","여는 괄호","닫는 괄호","함수","특수문자","알려지지 않음"};


eTree::eTree()	//루트 노드의 우선순위는 -1
{
	if(expTree::initialized == false)
		expTree::initExpTree();

	identifierType = 0;
	val = NULL;
	priority = -1;
	operatorType = -1;
	maxPNum = 1;
	
	parseSuccess = true;
}

eTree::eTree(double *n,int prior,int operatorKind,int MPN,int idType)
{
	if(expTree::initialized == false)
		expTree::initExpTree();

	identifierType = idType;
	val = n;
	priority = prior;
	operatorType = operatorKind;
	maxPNum = MPN;
	
	parseSuccess = true;
}

eTree::eTree(double n)
{
	if(expTree::initialized == false)
		expTree::initExpTree();

	identifierType = CONSTONLY;
	val = NULL;
	priority = -1;
	operatorType = -1;
	maxPNum = 1;

	insertNode(new double(n),PRIORNUM,NUMBER,0,CONSTONLY);
	parseSuccess = true;
}

eTree::eTree(const char *str)
{
	if(expTree::initialized == false)
		expTree::initExpTree();

	identifierType = 0;
	val = NULL;
	priority = -1;
	operatorType = -1;
	maxPNum = 1;

	parseSuccess = parse(str,"");
}

eTree::~eTree()
{
	if(expTree::predefinedVariables.find(val) == expTree::predefinedVariables.end())
		delete val;
		
	for(int i=0;i<parameter.size();i++)
	{
		delete parameter[i];
	}
}

//하위 4비트: e, x, t,  상위 4비트: f, g, h
void eTree::setIdentifierType(int type)
{
	this->identifierType = type;
}

eTree *eTree::insertNode(double *n,int prior,int kind,int MPN,int idType)
{
	if(prior<0) return NULL;
	eTree *tmp = this;
	eTree *tmp2;

	while(1)
	{
		if(tmp->parameter.size() < tmp->maxPNum)//연산자가 최대 연산자 개수보다 작으면 스택에 푸시
		{
			tmp2 = new eTree(n,prior,kind,MPN,idType);
			tmp->parameter.emplace_back(tmp2);
			break;
		}
		else		//연산자가 최대 개수에 도달했거나 인수의 개수가 무한일 때
		{
			int sz = tmp->parameter.size()-1;
			if(prior > tmp->priority && prior <= tmp->parameter[sz]->priority)
			{
				//삽입하는 연산자의 우선순위가 자신의 우선순위(this->priority)보다는 크고 마지막 연산자의 우선순위(p[mPN-1]->priority)보다는 작을 때
				tmp2 = tmp->parameter[sz];
				tmp->parameter[sz] = new eTree(n,prior,kind,MPN,idType);
				tmp->parameter[sz]->parameter.emplace_back(tmp2);
				break;
				//해당 연산자(n)을 마지막 연산자로 하고, 원래의 마지막 연산자(tmp)는 새로운 마지막 연산자(parameter[maxPNum-1])의 첫 번째 연산자로 한다.
			}
			else
			{
				//연산자 n의 우선순위가 this의 우선순위보다 작을 때, 마지막 연산자(parameter[maxPNum-1])를 루트 노드로 하는 서브트리에 같은 과정을 반복
				tmp = tmp->parameter[sz];
				//this->parameter[maxPNum-1]->insertNode(n,prior);
			}
		}
	}
	return tmp2;
}

bool eTree::parse(const char* p)
{
	return parse(p, "");
}

bool eTree::parse(const char *p,const char *name)
{
	bool res = true;
	expTree::setErrorMessageHead(name);

	vector<eTreeToken> tokenList;
	if(expTree::getTokenList(tokenList,p))
	{
		writeErrorMessage("Parenthesis mismatch: Number of '(' and ')' is not same.\n");
		res = false;
	}
	
	double *value = NULL;

	if(this->identifierType == USEGLOBAL)
	{
		if(expTree::identifierType & 0x02)
			value = &expTree::xVal;
		else if(expTree::identifierType & 0x04)
			value = &expTree::tVal;
		else
			value = NULL;

		this->identifierType = expTree::identifierType;
	}
	

	if(expTree::validTokenCheck(tokenList,&this->identifierType))
	{
		this->makeTree(tokenList,0,0,tokenList.size());
		
		if(this->validExprCheck())
		{
			if(value)
				simplify(value);

			res = true;
		}
		else
		{
			res = false;
		}
	}
	else
	{
		res = false;
	}

	parseSuccess = res;
	return res;
}

void eTree::makeTree(vector<eTreeToken> &tokenList,int pr,int start,int end)
{
	int prior = 0;
	eTokenType prevTokenType = eTokenType::eUnknownTok;

	for(int i=start;i<end;i++)
	{
		if(tokenList[i].type == eTokenType::eNumberTok)
		{
			if(prevTokenType == eTokenType::eNumberTok || prevTokenType == eTokenType::eIdentifierTok || prevTokenType == eTokenType::eCloseParTok || prevTokenType == eTokenType::eFunctionTok)
			{
				insertNode(NULL,PRIORMD+prior,TIMES,2,identifierType);
			}

			double *value = new double;
			*value = atof(tokenList[i].name.data());
			insertNode(value,PRIORNUM+prior,NUMBER,0,identifierType);
		}
		else if(tokenList[i].type == eTokenType::eIdentifierTok)
		{
			if(prevTokenType == eTokenType::eNumberTok || prevTokenType == eTokenType::eIdentifierTok)
			{
				insertNode(NULL,PRIORMD+prior,TIMES,2,identifierType);
			}

			double *value = expTree::getIdentifierPointer(this->identifierType,tokenList[i].name);

			if(value)
				insertNode(value,PRIORNUM+prior,NUMBER,0,identifierType);
			else
				insertNode(NULL,PRIORNUM+prior,NUMBER,0,identifierType);
		}
		else if(tokenList[i].type == eTokenType::eOperatorTok)
		{
			if(tokenList[i].name == "+")
			{
				if(i==start || prevTokenType == eTokenType::eOpenParTok)
					insertNode(expTree::zero,PRIORNUM+prior,NUMBER,0,identifierType);
				insertNode(NULL,PRIORPM+prior,PLUS,2,identifierType);
			}
			else if(tokenList[i].name == "-")
			{
				if(i==start || prevTokenType == eTokenType::eOpenParTok)
					insertNode(expTree::zero,PRIORNUM+prior,NUMBER,0,identifierType);
				insertNode(NULL,PRIORPM+prior,MINUS,2,identifierType);
			}
			else if(tokenList[i].name == "*")
				insertNode(NULL,PRIORMD+prior,TIMES,2,identifierType);
			else if(tokenList[i].name == "/")
				insertNode(NULL,PRIORMD+prior,DIVIDE,2,identifierType);
			else if(tokenList[i].name == "^")
				insertNode(NULL,PRIOREXP+prior,EXPONENT,2,identifierType);
		}
		else if(tokenList[i].type == eTokenType::eOpenParTok)
		{
			if(prevTokenType == eTokenType::eNumberTok || prevTokenType == eTokenType::eIdentifierTok || prevTokenType == eTokenType::eCloseParTok || prevTokenType == eTokenType::eFunctionTok)
			{
				insertNode(NULL,PRIORMD+prior,TIMES,2,identifierType);
			}
			prior += PRIORLEVEL;
		}
		else if(tokenList[i].type == eTokenType::eCloseParTok)
		{
			prior -= PRIORLEVEL;
		}
		if(tokenList[i].type == eTokenType::eFunctionTok)
		{
			if(prevTokenType == eTokenType::eNumberTok || prevTokenType == eTokenType::eIdentifierTok || prevTokenType == eTokenType::eCloseParTok)
			{
				insertNode(NULL,PRIORMD+prior,TIMES,2,identifierType);
			}

			if(expTree::funcByString.find(tokenList[i].name) != expTree::funcByString.end())
			{
				int funcType = expTree::funcByString[tokenList[i].name].first;
				int parNum = (expTree::funcByString[tokenList[i].name].second & 0x0F);
				int parType = (expTree::funcByString[tokenList[i].name].second & CONSIDERSHAPE);

				if(parNum == 0)
					insertNode(&expTree::pi_2,PRIORNUM+prior,NUMBER,0,identifierType);
				else
				{
					eTree *funcNode = insertNode(NULL,PRIORFUNC+prior,funcType,parNum,identifierType);
					int startPos = i+1;

					//startPos가 유효한 범위일 때만 인수 추가. 
					for(int j=0;j<parNum && startPos<tokenList.size();j++)
					{
						eTree *par = new eTree;
						par->setIdentifierType(this->identifierType);
						bool considerShape = (parType != 0 || j < parNum-1);

						int endPos = expTree::getParameterRange(tokenList,considerShape,startPos);
						par->makeTree(tokenList,pr+PRIORLEVEL,startPos,endPos);
						funcNode->parameter.emplace_back(par->parameter[0]);

						startPos = endPos;
					}
					i = startPos-1;
				}
			}
			else
			{
				insertNode(NULL,PRIORNUM+prior,UNKNOWNFUNC,0,identifierType);
			}
			
		}
		
		prevTokenType = tokenList[i].type;
	}
}

double *eTree::calc()
{
	if(operatorType == NUMBER)		//피연산자인 경우 값을 반환
		return val;
	else if(priority != -1)		//연산자인 경우
	{
		double *res = new double(0);

		switch(operatorType)
		{
		case PLUS:	//덧셈
			*res = (*parameter[0]->calc()) + (*parameter[1]->calc());
			break;

		case MINUS:	//뺄셈
			*res = (*parameter[0]->calc()) - (*parameter[1]->calc());
			break;

		case TIMES:	//곱셈
			*res = (*parameter[0]->calc()) * (*parameter[1]->calc());
			break;

		case DIVIDE:	//나눗셈
			*res = (*parameter[0]->calc()) / (*parameter[1]->calc());
			break;

		case EXPONENT:	//지수
			*res = expTree::my_pow((*parameter[0]->calc()) , (*parameter[1]->calc()));
			break;

		case ABSOLUTE:	//절댓값
			*res = abs(*parameter[0]->calc());
			break;


		case SQRT:	//sqrt함수
			*res = sqrt(*parameter[0]->calc());
			break;
			
		case NSQRT:	//sqrt함수
			*res = pow(*parameter[1]->calc(),1.0/(*parameter[0]->calc()));
			break;

		case FRAC:	//dfrac
			*res = (*parameter[0]->calc()) / (*parameter[1]->calc());
			break;

		case PI:	//pi
			*res = expTree::pi_2;
			break;

		case SIGN:	//sqrt함수
			*res = expTree::sign(*parameter[0]->calc());
			break;

		case ABSDIFF:	//sqrt함수
			*res = expTree::absDiff(*parameter[0]->calc());
			break;

		case SIGNDIFF:	//sqrt함수
			*res = expTree::signDiff(*parameter[0]->calc());
			break;


		case SIN:	//사인
			*res = sin(*parameter[0]->calc());
			break;

		case COS:	//코사인
			*res = cos(*parameter[0]->calc());
			break;

		case TAN:	//탄젠트
			*res = tan(*parameter[0]->calc());
			break;

		case SEC:	//시컨트
			*res = 1/cos(*parameter[0]->calc());
			break;

		case CSC:	//코시컨트
			*res = 1/sin(*parameter[0]->calc());
			break;

		case COT:	//코탄젠트
			*res = 1/tan(*parameter[0]->calc());
			break;

		case SINP:
			*res = expTree::my_pow(sin(*parameter[1]->calc()),*parameter[0]->calc());
			break;

		case COSP:
			*res = expTree::my_pow(cos(*parameter[1]->calc()),*parameter[0]->calc());
			break;

		case TANP:
			*res = expTree::my_pow(tan(*parameter[1]->calc()),*parameter[0]->calc());
			break;

		case SECP:
			*res = expTree::my_pow(1/cos(*parameter[1]->calc()),*parameter[0]->calc());
			break;

		case CSCP:
			*res = expTree::my_pow(1/sin(*parameter[1]->calc()),*parameter[0]->calc());
			break;

		case COTP:
			*res = expTree::my_pow(1/tan(*parameter[1]->calc()),*parameter[0]->calc());
			break;

		case ARCSIN:
			*res = asin(*parameter[0]->calc());
			break;

		case ARCCOS:
			*res = acos(*parameter[0]->calc());
			break;

		case ARCTAN:
			*res = atan(*parameter[0]->calc());
			break;

		case ARCSEC:
			*res = acos(1/(*parameter[0]->calc()));
			break;

		case ARCCSC:
			*res = asin(1/(*parameter[0]->calc()));
			break;

		case ARCCOT:
			{double tmpVal = *parameter[0]->calc();
			if(tmpVal>0)
				*res = atan(1/tmpVal);
			else
				*res = atan(1/tmpVal)+expTree::pi_2;}
			break;


		case SINH:	//sinh함수
			*res = sinh(*parameter[0]->calc());
			break;

		case COSH:	//cosh함수
			*res = cosh(*parameter[0]->calc());
			break;

		case TANH:	//tanh함수
			*res = tanh(*parameter[0]->calc());
			break;

		case SECH:	//sech함수
			*res = 1/cosh(*parameter[0]->calc());
			break;

		case CSCH:	//csch함수
			*res = 1/sinh(*parameter[0]->calc());
			break;

		case COTH:	//coth함수
			*res = 1/tanh(*parameter[0]->calc());
			break;

		case SINHP:
			*res = expTree::my_pow(sinh(*parameter[1]->calc()),*parameter[0]->calc());
			break;

		case COSHP:
			*res = expTree::my_pow(cosh(*parameter[1]->calc()),*parameter[0]->calc());
			break;

		case TANHP:
			*res = expTree::my_pow(tanh(*parameter[1]->calc()),*parameter[0]->calc());
			break;

		case SECHP:
			*res = expTree::my_pow(1/cosh(*parameter[1]->calc()),*parameter[0]->calc());
			break;

		case CSCHP:
			*res = expTree::my_pow(1/sinh(*parameter[1]->calc()),*parameter[0]->calc());
			break;

		case COTHP:
			*res = expTree::my_pow(1/tanh(*parameter[1]->calc()),*parameter[0]->calc());
			break;

		case ARCSINH:	//sinh함수
			*res = asinh(*parameter[0]->calc());
			break;

		case ARCCOSH:	//cosh함수
			*res = acosh(*parameter[0]->calc());
			break;

		case ARCTANH:	//tanh함수
			*res = atanh(*parameter[0]->calc());
			break;

		case ARCSECH:	//sech함수
			*res = acosh(1/(*parameter[0]->calc()));
			break;

		case ARCCSCH:	//csch함수
			*res = asinh(1/(*parameter[0]->calc()));
			break;

		case ARCCOTH:	//coth함수
			*res = atanh(1/(*parameter[0]->calc()));
			break;


		case LN:	//자연로그
			*res = expTree::my_ln(*parameter[0]->calc());
			break;

		case LOG:	//상용로그
			*res = expTree::my_ln(*parameter[0]->calc())/(*expTree::ln10);
			break;

		case LOG2:
			*res = expTree::my_ln(*parameter[1]->calc())/expTree::my_ln(*parameter[0]->calc());
			break;

		case POW:	//지수
			*res = expTree::my_pow(*parameter[0]->calc() , *parameter[1]->calc());
			break;

		default:
			res = NULL;
			break;
		}

		return res;
	}
	else if(priority == -1)	//루트 노드
		return parameter[0]->calc();
	return expTree::zero;
}

double eTree::calculate()
{
	return *calc();
}

double eTree::calculate(double value)
{
	double prevValue = 0;
	double *var = NULL;

	if(this->identifierType & 0x02)
	{
		prevValue = expTree::xVal;
		var = &expTree::xVal;
	}
	else if(this->identifierType & 0x04)
	{
		prevValue = expTree::tVal;
		var = &expTree::tVal;
	}

	if(var)
		*var = value;

	double res = *calc();

	if(var)
		*var = prevValue;

	return res;
}

double eTree::calculate(const char *value)
{
	eTree* eq = new eTree(value);
	eq->setIdentifierType(CONSTONLY);
	double result = this->calculate(eq->calculate());

	delete eq;
	return result;
}

//상수가 포함된 하위 수식을 간략화함.
//변수를 찾으면 false(간략화 불가능), 못 찾으면 true(간략화 가능)리턴
bool eTree::simplify(double *var)
{
	bool res = true;
	if(this->val == var)
		res = false;
	else
	{
		for(int i=0;i<FUNCVARNUM && res==true;i++)
		{
			for(int j=0;j<FUNCVARVALNUM;j++)
			{
				if(val == expTree::funcVarNum[i][j])
				{
					res = false;
					break;
				}
			}
		}
	}
	
	if(res == true)
	{
		if(parameter.size() == 1)
		{
			if(priority == -1)
				return parameter[0]->simplify(var);
			if(parameter[0]->simplify(var) == true)
			{
				this->val = this->calc();
				this->priority = (priority/PRIORLEVEL)*PRIORLEVEL+PRIORNUM;
				this->operatorType = NUMBER;
				this->maxPNum = 0;

				parameter.clear();
				res = true;
			}
			else
				res = false;
		}
		else if(parameter.size() == 2)
		{
			bool simplifyRes1;
			bool simplifyRes2;
			simplifyRes1 = parameter[0]->simplify(var);
			simplifyRes2 = parameter[1]->simplify(var);
			
			if(simplifyRes1 == true && simplifyRes2 == true)
			{
				this->val = this->calc();
				this->priority = (priority/PRIORLEVEL)*PRIORLEVEL+PRIORNUM;
				this->operatorType = NUMBER;
				this->maxPNum = 0;

				parameter.clear();
				
				res = true;
			}
			else if(simplifyRes1 == true)
			{
				//덧셈:1 뺄셈:2 곱셈:3 나눗셈:4 지수:5
				//double *tmp1 = parameter[0]->calc();
				double *tmp1 = parameter[0]->val;
				
				if(operatorType == PLUS && expTree::isSameValue(*tmp1,*expTree::zero)			//0+x
					|| operatorType == TIMES && expTree::isSameValue(*tmp1,*expTree::zero)		//0*x
					|| operatorType == TIMES && expTree::isSameValue(*tmp1,*expTree::one)		//1*x
					|| operatorType == EXPONENT && expTree::isSameValue(*tmp1,*expTree::one)		//1^x
					|| operatorType == EXPONENT && expTree::isSameValue(*tmp1,*expTree::zero)		//0^x
					)
				{
					eTree *par = parameter[1];

					int constantType = 0;	//변수이면 -1, 0이면 0, 1이면 1
					
					if(operatorType==PLUS || operatorType==TIMES)		//0+x, 0*x, 1*x
					{
						if( operatorType == TIMES && expTree::isSameValue(*tmp1,*expTree::zero) )	//0*x
						{
							res = true;
							constantType = 0;
						}
						else	//0+x, 1*x
						{
							res = false;
							constantType = -1;
						}
					}
					else
					{
						res = true;
						if( operatorType == EXPONENT && expTree::isSameValue(*tmp1,*expTree::one) )	//1^x
							constantType = 1;
						else		//0^x
							constantType = 0;
					}


					if(constantType == -1)	//변수인 경우
					{
						this->val = par->val;
						this->priority = (priority/PRIORLEVEL)*PRIORLEVEL + par->priority%PRIORLEVEL;
						this->operatorType = par->operatorType;
						this->maxPNum = par->maxPNum;

						eTree **tmpTree = new eTree*[par->parameter.size()];
						int sz = par->parameter.size();

						for(int i=0;i<sz;i++)	//자식의 child 임시 저장
							tmpTree[i] = par->parameter[i];

						this->parameter.clear();

						for(int i=0;i<sz;i++)	//자식의 child 가져오기
							parameter.emplace_back(tmpTree[i]);

						delete [] tmpTree;
					}
					else if(constantType == 0)	//0으로 계산되는 수식
					{
						this->val = expTree::zero;
						this->priority = (priority/PRIORLEVEL)*PRIORLEVEL + PRIORNUM;
						this->operatorType = NUMBER;
						this->maxPNum = 0;
						this->parameter.clear();
					}
					else if(constantType == 1)	//1로 계산되는 수식
					{
						this->val = expTree::one;
						this->priority = (priority/PRIORLEVEL)*PRIORLEVEL + PRIORNUM;
						this->operatorType = NUMBER;
						this->maxPNum = 0;
						this->parameter.clear();
					}
				}
				else
					res = false;
			}
			else if(simplifyRes2 == true)
			{
				//덧셈:1 뺄셈:2 곱셈:3 나눗셈:4 지수:5
				//double *tmp2 = parameter[1]->calc();
				double *tmp2 = parameter[1]->val;
				
				if(operatorType == PLUS && expTree::isSameValue(*tmp2,*expTree::zero)			//x+0
					|| operatorType == MINUS && expTree::isSameValue(*tmp2,*expTree::zero)		//x-0
					|| operatorType == TIMES && expTree::isSameValue(*tmp2,*expTree::zero)		//x*0
					|| operatorType == TIMES && expTree::isSameValue(*tmp2,*expTree::one)		//x*1
					|| operatorType == DIVIDE && expTree::isSameValue(*tmp2,*expTree::one)		//x/1
					|| operatorType == EXPONENT && expTree::isSameValue(*tmp2,*expTree::one)		//x^1
					|| operatorType == EXPONENT && expTree::isSameValue(*tmp2,*expTree::zero)		//x^0
					)
				{
					eTree *par = parameter[0];

					int constantType = 0;

					if(operatorType>=PLUS && operatorType<=DIVIDE)		//x+0, x-0, x*0, x*1, x/1
					{
						if( operatorType == TIMES && expTree::isSameValue(*tmp2,*expTree::zero) )	//x*0
						{
							res = true;
							constantType = 0;
						}
						else
						{
							res = false;
							constantType = -1;
						}
						
					}
					else
					{
						if( operatorType == EXPONENT && expTree::isSameValue(*tmp2,*expTree::one) )	//x^1
						{
							res = false;
							constantType = -1;
						}
						else				//x^0
						{
							res = true;
							constantType = 1;
						}
					}


					if(constantType == -1)	//변수인 경우
					{
						this->val = par->val;
						this->priority = (priority/PRIORLEVEL)*PRIORLEVEL + par->priority%PRIORLEVEL;
						this->operatorType = par->operatorType;
						this->maxPNum = par->maxPNum;

						eTree **tmpTree = new eTree*[par->parameter.size()];
						int sz = par->parameter.size();

						for(int i=0;i<sz;i++)	//자식의 child 임시 저장
							tmpTree[i] = par->parameter[i];

						this->parameter.clear();

						for(int i=0;i<sz;i++)	//자식의 child 가져오기
							parameter.emplace_back(tmpTree[i]);

						delete [] tmpTree;
					}
					else if(constantType == 0)	//0으로 계산되는 수식
					{
						this->val = expTree::zero;
						this->priority = (priority/PRIORLEVEL)*PRIORLEVEL + PRIORNUM;
						this->operatorType = NUMBER;
						this->maxPNum = 0;
						this->parameter.clear();
					}
					else if(constantType == 1)	//1로 계산되는 수식
					{
						this->val = expTree::one;
						this->priority = (priority/PRIORLEVEL)*PRIORLEVEL + PRIORNUM;
						this->operatorType = NUMBER;
						this->maxPNum = 0;
						this->parameter.clear();
					}
				}
				else
					res = false;
			}
			else
				res = false;
		}
	}
	return res;
}

bool eTree::validExprCheck()
{
	bool res = true;

	if(this->parameter.size() != this->maxPNum)
	{
		writeErrorMessage("Number of Operand is Insufficient: operator '%s' need %d operands.\n",expTree::stringByDef[this->operatorType].first.data(), expTree::stringByDef[this->operatorType].second);
		res = false;
	}

	for(int i=0;i<this->parameter.size();i++)
	{
		if(parameter[i]->validExprCheck() == false)
			res = false;
	}

	return res;
}

eTree* eTree::differential()
{
	double* var = NULL;

	if (this->identifierType & 0x02)
		var = &expTree::xVal;
	else if (this->identifierType & 0x04)
		var = &expTree::tVal;
	else
		var = NULL;

	return differential(var);
}

eTree *eTree::differential(double *var)
{
	int plev = priority/PRIORLEVEL;
	eTree *res = new eTree(expTree::zero,plev*PRIORLEVEL+PRIORNUM,NUMBER,maxPNum,identifierType);
	eTree *tmp;

	if(priority != -1 && operatorType == NUMBER)		//피연산자인 경우 값을 반환
	{
		if(val == var)
			res->val = expTree::one;
		else
		{
			bool findVar = false;

			for(int i=0;i<FUNCVARNUM && findVar==false;i++)
			{
				for(int j=0;j<FUNCVARVALNUM-1;j++)
				{
					if(val == expTree::funcVarNum[i][j])
					{
						res->val = expTree::funcVarNum[i][j+1];
						findVar = true;
						break;
					}
				}
			}

			if(findVar == false)
				res->val = expTree::zero;
		}
		
		return res;
	}
	else if(priority != -1)		//연산자인 경우
	{
		//printf("operatorType:%d\n",operatorType);
		switch(operatorType)
		{
		case PLUS:	//덧셈
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter.emplace_back(parameter[1]->differential(var));
			break;

		case MINUS:	//뺄셈
			res->val = NULL;
			res->operatorType = MINUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter.emplace_back(parameter[1]->differential(var));
			break;

		case TIMES:	//곱셈
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter[0]->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			res->parameter[1]->parameter.emplace_back(parameter[0]->deepCopyTree());
			res->parameter[1]->parameter.emplace_back(parameter[1]->differential(var));
			break;

		case DIVIDE:	//나눗셈
			res->val = NULL;
			res->operatorType = DIVIDE;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp->parameter[0]->parameter.emplace_back(parameter[0]->differential(var));
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter[1]->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter[1]->parameter.emplace_back(parameter[1]->differential(var));
			res->parameter[1]->parameter.emplace_back(parameter[1]->deepCopyTree());
			res->parameter[1]->parameter.emplace_back(parameter[1]->deepCopyTree());
			break;

		case EXPONENT:	//지수
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[1]->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(this->deepCopyTree());
			tmp->parameter.emplace_back(parameter[1]->differential(var));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			break;

		case ABSOLUTE:	//절댓값
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,ABSDIFF,1,identifierType));
			res->parameter[1]->parameter.emplace_back(parameter[0]->deepCopyTree());
			break;



		case SQRT:	//sqrt함수
			res->val = NULL;
			res->operatorType = DIVIDE;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(expTree::two,plev*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SQRT,1,identifierType));
			tmp->parameter[1]->parameter.emplace_back(parameter[0]->deepCopyTree());

			break;

		case NSQRT:	//nsqrt함수
			res->val = NULL;
			res->operatorType = MINUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,DIVIDE,2,identifierType));
			tmp->parameter[1]->parameter.emplace_back(parameter[1]->differential(var));
			tmp->parameter[1]->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,DIVIDE,2,identifierType));
			tmp->parameter.emplace_back(new eTree(expTree::one,plev*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(expTree::one,plev*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());


			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(parameter[0]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,DIVIDE,2,identifierType));


			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::two,plev*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));


			tmp = res->parameter[1]->parameter[1];
			tmp->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,DIVIDE,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(expTree::one,plev*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());

			break;
		
		case FRAC:	//dfrac
			res->val = NULL;
			res->operatorType = DIVIDE;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp->parameter[0]->parameter.emplace_back(parameter[0]->differential(var));
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter[1]->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter[1]->parameter.emplace_back(parameter[1]->differential(var));
			res->parameter[1]->parameter.emplace_back(parameter[1]->deepCopyTree());
			res->parameter[1]->parameter.emplace_back(parameter[1]->deepCopyTree());
			break;

		case PI:	//pi
			res->val = expTree::zero;
			res->operatorType = NUMBER;
			res->priority = plev*PRIORLEVEL + PRIORNUM;
			res->maxPNum = 0;
			break;

		case SIGN:	//부호 함수
		case ABSDIFF:
		case SIGNDIFF:
			res->val = NULL;
			res->operatorType = SIGNDIFF;
			res->priority = plev*PRIORLEVEL+PRIORFUNC;
			res->maxPNum = 1;
			res->parameter.emplace_back(parameter[0]->deepCopyTree());
			break;


		case SIN:	//사인
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COS,1,identifierType));
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());
			break;

		case COS:	//코사인
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(expTree::minusone,plev*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SIN,1,identifierType));
			tmp->parameter.emplace_back(parameter[0]->differential(var));
			tmp->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());
			break;

		case TAN:	//탄젠트
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SEC,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(tmp->parameter[0]->deepCopyTree());

			break;

		case SEC:	//시컨트
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,TAN,1,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SEC,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter[1]->parameter.emplace_back(parameter[0]->deepCopyTree());

			break;

		case CSC:	//코시컨트
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(expTree::minusone,plev*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COT,1,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,CSC,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter[1]->parameter.emplace_back(parameter[0]->deepCopyTree());

			break;

		case COT:	//코탄젠트
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(expTree::minusone,plev*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,CSC,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(tmp->parameter[0]->deepCopyTree());

			break;

		case SINP:
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[1]->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SIN,1,identifierType));	//원래 함수
			tmp->parameter[1]->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(this->deepCopyTree());
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SIN,1,identifierType));	//원래 함수 
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));
			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[1]->parameter[0]->parameter[1];
			tmp->parameter.emplace_back(parameter[1]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COS,1,identifierType));	//원래 함수의 도함수
			tmp->parameter[1]->parameter.emplace_back(parameter[1]->deepCopyTree());

			break;

		case COSP:
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[1]->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COS,1,identifierType));	//원래 함수
			tmp->parameter[1]->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(this->deepCopyTree());
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COS,1,identifierType));	//원래 함수 
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));
			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[1]->parameter[0]->parameter[1];
			tmp->parameter.emplace_back(parameter[1]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SIN,1,identifierType));	//원래 함수의 도함수
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::minusone,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			break;

		case TANP:
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[1]->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,TAN,1,identifierType));	//원래 함수
			tmp->parameter[1]->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(this->deepCopyTree());
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,TAN,1,identifierType));	//원래 함수 
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));
			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[1]->parameter[0]->parameter[1];
			tmp->parameter.emplace_back(parameter[1]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SEC,1,identifierType));	//원래 함수의 도함수
			tmp->parameter.emplace_back(new eTree(expTree::two,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			break;

		case SECP:
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[1]->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SEC,1,identifierType));	//원래 함수
			tmp->parameter[1]->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(this->deepCopyTree());
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SEC,1,identifierType));	//원래 함수 
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));
			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[1]->parameter[0]->parameter[1];
			tmp->parameter.emplace_back(parameter[1]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,TAN,1,identifierType));	//원래 함수의 도함수
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SEC,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter[1]->parameter.emplace_back(parameter[1]->deepCopyTree());
			break;

		case CSCP:
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[1]->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,CSC,1,identifierType));	//원래 함수
			tmp->parameter[1]->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(this->deepCopyTree());
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,CSC,1,identifierType));	//원래 함수 
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));
			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[1]->parameter[0]->parameter[1];
			tmp->parameter.emplace_back(parameter[1]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(expTree::minusone,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			
			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,CSC,1,identifierType));	//원래 함수의 도함수
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COT,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter[1]->parameter.emplace_back(parameter[1]->deepCopyTree());
			break;

		case COTP:
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[1]->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COT,1,identifierType));	//원래 함수
			tmp->parameter[1]->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(this->deepCopyTree());
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COT,1,identifierType));	//원래 함수 
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));
			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[1]->parameter[0]->parameter[1];
			tmp->parameter.emplace_back(parameter[1]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(expTree::minusone,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			
			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,CSC,1,identifierType));	//원래 함수의 도함수
			tmp->parameter.emplace_back(new eTree(expTree::two,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			break;

		case ARCSIN:
			res->val = NULL;
			res->operatorType = DIVIDE;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SQRT,1,identifierType));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::two,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			break;

		case ARCCOS:
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(expTree::minusone,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,DIVIDE,2,identifierType));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SQRT,1,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::two,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			break;

		case ARCTAN:
			res->val = NULL;
			res->operatorType = DIVIDE;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORPM,PLUS,2,identifierType));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::two,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			break;

		case ARCSEC:
			res->val = NULL;
			res->operatorType = DIVIDE;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,ABSOLUTE,1,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SQRT,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::two,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			break;

		case ARCCSC:
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(expTree::minusone,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,DIVIDE,2,identifierType));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,ABSOLUTE,1,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SQRT,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::two,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			break;

		case ARCCOT:
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(expTree::minusone,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORPM,DIVIDE,2,identifierType));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORPM,PLUS,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::two,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			break;

			
		case SINH:	//sinh함수
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COSH,1,identifierType));
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());

			break;


		case COSH:	//cosh함수
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SINH,1,identifierType));
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());

			break;


		case TANH:	//tanh함수
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SECH,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(tmp->parameter[0]);

			break;


		case SECH:	//sech함수
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(expTree::minusone,plev*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,TANH,1,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SECH,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter[1]->parameter.emplace_back(parameter[0]->deepCopyTree());

			break;


		case CSCH:	//csch함수
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(expTree::minusone,plev*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COTH,1,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,CSCH,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter[1]->parameter.emplace_back(parameter[0]->deepCopyTree());

			break;


		case COTH:	//coth함수
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(expTree::minusone,plev*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,CSCH,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(tmp->parameter[0]);

			break;

		case SINHP:
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[1]->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SINH,1,identifierType));	//원래 함수
			tmp->parameter[1]->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(this->deepCopyTree());
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SINH,1,identifierType));	//원래 함수 
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));
			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[1]->parameter[0]->parameter[1];
			tmp->parameter.emplace_back(parameter[1]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COSH,1,identifierType));	//원래 함수의 도함수
			tmp->parameter[1]->parameter.emplace_back(parameter[1]->deepCopyTree());

			break;

		case COSHP:
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[1]->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COSH,1,identifierType));	//원래 함수
			tmp->parameter[1]->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(this->deepCopyTree());
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COSH,1,identifierType));	//원래 함수 
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));
			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[1]->parameter[0]->parameter[1];
			tmp->parameter.emplace_back(parameter[1]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SINH,1,identifierType));	//원래 함수의 도함수
			tmp->parameter[1]->parameter.emplace_back(parameter[1]->deepCopyTree());

			break;

		case TANHP:
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev * PRIORLEVEL + PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL, (plev + 1) * PRIORLEVEL + PRIORMD, TIMES, 2, identifierType));
			res->parameter.emplace_back(new eTree(NULL, (plev + 1) * PRIORLEVEL + PRIORMD, TIMES, 2, identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL, plev * PRIORLEVEL + PRIORMD, TIMES, 2, identifierType));
			tmp->parameter.emplace_back(new eTree(NULL, plev * PRIORLEVEL + PRIORFUNC, LN, 1, identifierType));
			tmp->parameter[1]->parameter.emplace_back(new eTree(NULL, plev * PRIORLEVEL + PRIORFUNC, TANH, 1, identifierType));	//원래 함수
			tmp->parameter[1]->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(this->deepCopyTree());
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL, plev * PRIORLEVEL + PRIORMD, TIMES, 2, identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL, plev * PRIORLEVEL + PRIOREXP, EXPONENT, 2, identifierType));
			tmp->parameter.emplace_back(new eTree(NULL, plev * PRIORLEVEL + PRIORMD, TIMES, 2, identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL, plev * PRIORLEVEL + PRIORFUNC, TANH, 1, identifierType));	//원래 함수 
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL, (plev + 1) * PRIORLEVEL + PRIORPM, MINUS, 2, identifierType));
			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::one, (plev + 1) * PRIORLEVEL + PRIORNUM, NUMBER, 0, identifierType));

			tmp = res->parameter[1]->parameter[0]->parameter[1];
			tmp->parameter.emplace_back(parameter[1]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL, plev * PRIORLEVEL + PRIOREXP, EXPONENT, 2, identifierType));//원래 함수의 도함수

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL, plev * PRIORLEVEL + PRIORFUNC, SECH, 1, identifierType));
			tmp->parameter.emplace_back(new eTree(expTree::two, (plev + 1) * PRIORLEVEL + PRIORNUM, NUMBER, 0, identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			break;

		case SECHP:
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[1]->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SECH,1,identifierType));	//원래 함수
			tmp->parameter[1]->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(this->deepCopyTree());
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SECH,1,identifierType));	//원래 함수 
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));
			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[1]->parameter[0]->parameter[1];
			tmp->parameter.emplace_back(parameter[1]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));	//원래 함수의 도함수
			tmp->parameter.emplace_back(new eTree(expTree::minusone,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			
			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SECH,1,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,TANH,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter[1]->parameter.emplace_back(parameter[1]->deepCopyTree());
			break;

		case CSCHP:
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[1]->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,CSCH,1,identifierType));	//원래 함수
			tmp->parameter[1]->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(this->deepCopyTree());
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,CSCH,1,identifierType));	//원래 함수 
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));
			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[1]->parameter[0]->parameter[1];
			tmp->parameter.emplace_back(parameter[1]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));	//원래 함수의 도함수
			tmp->parameter.emplace_back(new eTree(expTree::minusone,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			
			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,CSCH,1,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COTH,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter[1]->parameter.emplace_back(parameter[1]->deepCopyTree());
			break;

		case COTHP:
			res->val = NULL;
			res->operatorType = PLUS;
			res->priority = plev*PRIORLEVEL+PRIORPM;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[1]->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COTH,1,identifierType));	//원래 함수
			tmp->parameter[1]->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(this->deepCopyTree());
			tmp->parameter.emplace_back(parameter[0]->differential(var));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,COTH,1,identifierType));	//원래 함수 
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));
			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = res->parameter[1]->parameter[0]->parameter[1];
			tmp->parameter.emplace_back(parameter[1]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));	//원래 함수의 도함수
			tmp->parameter.emplace_back(new eTree(expTree::minusone,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			
			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,CSCH,1,identifierType));
			tmp->parameter.emplace_back(new eTree(expTree::two,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[1]->deepCopyTree());
			break;

		case ARCSINH:
			res->val = NULL;
			res->operatorType = DIVIDE;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SQRT,1,identifierType));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORPM,PLUS,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::two,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			break;

		case ARCCOSH:
			res->val = NULL;
			res->operatorType = DIVIDE;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SQRT,1,identifierType));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::two,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			break;

		case ARCTANH:
		case ARCCOTH:
			res->val = NULL;
			res->operatorType = DIVIDE;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::two,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			break;

		case ARCSECH:
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(expTree::minusone,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,DIVIDE,2,identifierType));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SQRT,1,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::two,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			break;

		case ARCCSCH:
			res->val = NULL;
			res->operatorType = TIMES;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(expTree::minusone,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,DIVIDE,2,identifierType));

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(parameter[0]->differential(var));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,ABSOLUTE,1,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORFUNC,SQRT,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = tmp->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORPM,PLUS,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIOREXP,EXPONENT,2,identifierType));
			tmp->parameter.emplace_back(new eTree(expTree::one,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(new eTree(expTree::two,(plev+1)*PRIORLEVEL+PRIORNUM,NUMBER,0,identifierType));
			break;


		case LN:	//자연로그
			res->val = NULL;
			res->operatorType = DIVIDE;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter.emplace_back(parameter[0]->deepCopyTree());
			break;

		case LOG:	//상용로그
			res->val = NULL;
			res->operatorType = DIVIDE;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(parameter[0]->differential(var));
			res->parameter.emplace_back(new eTree(NULL,plev*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			res->parameter[1]->parameter.emplace_back(parameter[0]->deepCopyTree());
			res->parameter[1]->parameter.emplace_back(new eTree(expTree::ln10,plev*PRIORLEVEL+PRIORNUM,NUMBER,1,identifierType));
			break;
		
		case LOG2:	//밑이 있는 로그
			res->val = NULL;
			res->operatorType = DIVIDE;
			res->priority = plev*PRIORLEVEL+PRIORMD;
			res->maxPNum = 2;
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORPM,MINUS,2,identifierType));
			res->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));

			tmp = res->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,DIVIDE,2,identifierType));
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,DIVIDE,2,identifierType));

			tmp = tmp->parameter[0];
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp->parameter[0]->parameter.emplace_back(parameter[1]->differential(var));
			tmp->parameter[0]->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[0]->parameter[1]->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp = res->parameter[0]->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORMD,TIMES,2,identifierType));
			tmp->parameter.emplace_back(parameter[0]->deepCopyTree());

			tmp->parameter[0]->parameter.emplace_back(parameter[0]->differential(var));
			tmp->parameter[0]->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[0]->parameter[1]->parameter.emplace_back(parameter[1]->deepCopyTree());

			tmp = res->parameter[1];
			tmp->parameter.emplace_back(new eTree(NULL,(plev+1)*PRIORLEVEL+PRIORFUNC,LN,1,identifierType));
			tmp->parameter[0]->parameter.emplace_back(parameter[0]->deepCopyTree());
			tmp->parameter.emplace_back(tmp->parameter[0]);

			break;

		case POW:	//지수

			break;

		

		}
	}
	else if(priority == -1)	//루트 노드
	{
		res->val = NULL;
		res->priority = -1;
		res->operatorType = -1;
		res->maxPNum = 1;
		res->parameter.emplace_back(parameter[0]->differential(var));
	}
		
	res->simplify(var);
	
	return res;
}

void eTree::printTree(const char* path, const char* mode, int level)
{
	FILE* fp = fopen(path, mode);

	for (int i = 0; i < level; i++)
	{
		fprintf(fp, "    ");
	}
	if (this->priority == -1)
	{
		fprintf(fp, "Root of tree. 0x%p\n", this);
	}
	else if (this->operatorType == NUMBER)
	{
		if (this->val == &expTree::xVal)
			fprintf(fp, "variable: x\n");
		else if (this->val == &expTree::tVal)
			fprintf(fp, "variable: t\n");
		else if (this->val == &expTree::eVal)
			fprintf(fp, "constant: e(%f)\n",expTree::eVal);
		else if (this->val == &expTree::aVal)
			fprintf(fp, "constant: a(%f)\n",expTree::aVal);
		else if (this->val == &expTree::bVal)
			fprintf(fp, "constant: b(%f)\n",expTree::bVal);
		else if (this->val == &expTree::cVal)
			fprintf(fp, "constant: c(%f)\n",expTree::cVal);
		else
			fprintf(fp, "constant: %f\n", *(this->val));
	}

	else
		fprintf(fp, "operator: %s, priority: %d, maxPNum: %d, memory:0x%p\n", expTree::stringByDef[this->operatorType].first.data(), this->priority, this->maxPNum, this);

	fclose(fp);

	for (int i = 0; i < this->parameter.size(); i++)
	{
		if (this->parameter[i] != NULL)
			this->parameter[i]->printTree(path, mode, level + 1);
	}
}


