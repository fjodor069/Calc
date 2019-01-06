#pragma once


#include <list>

enum TokenIdentity {T_ADD, T_SUB, T_MUL, T_DIV, T_LEFT_PAREN, T_RIGHT_PAREN, T_REFERENCE, T_VALUE, T_EOL};

class Token
{
public:
	Token(void);
	~Token(void);

	Token(const Token& token);
	Token& operator=(const Token& token);
	Token (double dValue);
	Token (Reference reference);
	Token (TokenIdentity eTokenId);
	TokenIdentity GetId() const {return m_eTokenId;}
	double GetValue() const { return m_dValue;}
	Reference GetReference() const {return m_reference;}
private:
	TokenIdentity m_eTokenId;
	double m_dValue;
	Reference m_reference;
	

};

//iterator type for document items
//typedef std::list<std::shared_ptr<Token>> TokenList;

typedef std::list<Token> TokenList;
