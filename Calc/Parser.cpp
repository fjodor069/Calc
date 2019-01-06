#include "stdafx.h"


#include "Calc.h"
//#include "Parser.h"
//
//#include "Scanner.h"
//#include "Token.h"
//#include "Reference.h"
//#include "CellMatrix.h"

#include <cassert>


Parser::Parser(void)
{
}

SyntaxTree Parser::Formula(const CString& stBuffer)
{
	m_stBuffer = stBuffer;
	Scanner scanner(m_stBuffer);
	m_pTokenList = scanner.GetTokenList();
	m_nextToken = *m_pTokenList->begin(); 
	SyntaxTree* pExpr = Expression();
	Match(T_EOL);
	SyntaxTree syntaxTree(*pExpr);
	delete pExpr;
	return syntaxTree;
}

void Parser::Match(TokenIdentity eTokenId)
{
	if (m_nextToken.GetId () != eTokenId || m_pTokenList->empty())
	{
		//throw Error
		CString stMessage;
		stMessage.Format(TEXT("Invalid Expression: \"") + 
							  m_stBuffer + TEXT("\"."));
		throw stMessage;
	}
	m_pTokenList->pop_front();
}

SyntaxTree* Parser::Expression()
{
	SyntaxTree* pTerm = Term();
	SyntaxTree* pNextExpression = NextExpression(pTerm);

	return pNextExpression;

}

SyntaxTree* Parser::NextExpression(SyntaxTree* pLeftTerm)
{
	switch(m_nextToken.GetId())
	{
	case T_ADD:
		{
			Match(T_ADD);
			SyntaxTree* pRightTerm = Term(), *pResult;
			//check_memory(pResult = new SyntaxTree(ST_ADD,pLeftTerm,pRightTerm));
			pResult = new SyntaxTree(ST_ADD,pLeftTerm,pRightTerm);
			assert(pResult != nullptr);
			SyntaxTree* pNextExpression = NextExpression(pResult);
			return pNextExpression;
		}
	case T_SUB:
		{
			Match(T_SUB);
			SyntaxTree* pRightTerm = Term(), *pResult;
			//check_memory(pResult = new SyntaxTree(ST_ADD,pLeftTerm,pRightTerm));
			pResult = new SyntaxTree(ST_SUB,pLeftTerm,pRightTerm);
			assert(pResult != nullptr);
			SyntaxTree* pNextExpression = NextExpression(pResult);
			return pNextExpression;
		}
	default:
		return pLeftTerm;
	}
}


SyntaxTree* Parser::Term()
{
	SyntaxTree* pFactorTree = Factor();
	return NextTerm(pFactorTree);
}

SyntaxTree* Parser::NextTerm(SyntaxTree* pLeftTerm)
{
	//SynTaxTree* pRightTerm;


	Token token = m_pTokenList->front();
	switch (token.GetId())
	{
		case T_MUL:
		{
			Match(T_MUL);
			SyntaxTree* pRightTerm = Factor(), *pResult;
			pResult = new SyntaxTree(ST_MUL, pLeftTerm, pRightTerm);
			assert(pResult != nullptr);
			SyntaxTree* pNextExpression = NextExpression(pResult);
			return pNextExpression;
		}
		case T_DIV:
		{
			Match(T_DIV);
			SyntaxTree* pRightTerm = Factor(), *pResult;
			pResult = new SyntaxTree(ST_DIV, pLeftTerm, pRightTerm);
			assert(pResult != nullptr);
			SyntaxTree* pNextExpression = NextExpression(pResult);
			return pNextExpression;
		}
		default:
			return pLeftTerm;
	}

}

SyntaxTree* Parser::Factor()
{
	switch (m_nextToken.GetId())
	{
	case T_LEFT_PAREN:
	case T_REFERENCE:
		{
			Reference reference = m_nextToken.GetReference();
			Match(T_REFERENCE);
			
			int iRow = reference.GetRow();
			int iCol = reference.GetCol();

			if ( (iRow < 0) || (iRow >= ROWS) ||
				 (iCol < 0) || (iCol >= COLS))
			{
				CString stMessage;
				stMessage.Format(TEXT("Reference Out of Range: \"") + 
							  m_stBuffer + TEXT("\"."));
				throw stMessage;
			}
		}

	case T_VALUE:
		{
			double dValue = m_nextToken.GetValue ();
			Match(T_VALUE);
			SyntaxTree* pResult = new SyntaxTree(dValue);
			assert(pResult != nullptr);
			return pResult;
		}

	default:
			//throw Error(SyntaxError)
		CString stMessage;
		stMessage.Format(TEXT("Invalid Expression: \"") + 
							  m_stBuffer + TEXT("\"."));
		throw stMessage;


	}
}