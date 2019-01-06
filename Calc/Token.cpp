#include "stdafx.h"
//#include "Reference.h"
//
//
//#include "Token.h"
#include "Calc.h"

Token::Token(void)
{
	m_eTokenId = TokenIdentity::T_VALUE;
	m_dValue = 0.0;
	m_reference = Reference(0,0);
}

Token::~Token(void)
{
}



//copy constructor
Token::Token(const Token& token)
{
	m_eTokenId = token.m_eTokenId;
	m_dValue = token.m_dValue;
	m_reference = token.m_reference;
}

//assignment operator
Token& Token::operator=(const Token& token)
{
	if (this != &token)
	{
		m_eTokenId = token.m_eTokenId;
		m_dValue = token.m_dValue;
		m_reference = token.m_reference;	
	}
	return *this;
}


Token::Token(double dValue)
{
	m_dValue = dValue;
}

Token::Token(Reference reference)
{
	m_reference = reference;
}


Token::Token(TokenIdentity eTokenId)
{
	m_eTokenId = eTokenId;
}