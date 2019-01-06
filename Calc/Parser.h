#pragma once

class SyntaxTree;
class Token;

class Parser
{
public:
	Parser(void);
		
	
	SyntaxTree Formula(const CString& stBuffer);

private:
	void Match(TokenIdentity eTokenId);
	SyntaxTree* Expression();
	SyntaxTree* NextExpression(SyntaxTree* pLeftTerm);
	SyntaxTree* Term();
	SyntaxTree* NextTerm(SyntaxTree* pLeftFactor);
	SyntaxTree* Factor();

	CString m_stBuffer;
	Token m_nextToken;
	TokenList* m_pTokenList;

};
