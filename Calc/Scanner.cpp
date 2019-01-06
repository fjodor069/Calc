#include "stdafx.h"

//#include "Scanner.h"
//#include "Token.h"
//#include "Reference.h"

#include "Calc.h"

Scanner::Scanner (const CString& m_stBuffer) : m_stBuffer(m_stBuffer + TEXT('\0'))
{
	Token token;
	do
	{
		token = NextToken();
		m_tokenList.push_back(token);		//put it in the STL list
		
	} while (token.GetId() != T_EOL);

}

//
// this routine does the actual work of the scanner
//
Token Scanner::NextToken() 
{
	while ( (m_stBuffer[0] == TEXT(' ')) ||
		    (m_stBuffer[0] == TEXT('\t')))
	{
		m_stBuffer.Delete(0);
	}
	
	switch (m_stBuffer[0])
	{
	  case TEXT('\0'): 
		return Token(TokenIdentity::T_EOL);

	  case TEXT('+'):
		  {
			  double dValue;
			  if (ScanValue(dValue))
			  {
				  return Token(dValue);
			  }
			  else
			  {
				  m_stBuffer.Delete(0);
				  return Token(TokenIdentity::T_ADD);
			  }
		  }
		  break;

	  //case ....
	  case TEXT('-'):
		  m_stBuffer.Delete(0);
		  return Token(TokenIdentity::T_SUB);
		  break;

	  case TEXT('*'):
		  return Token(TokenIdentity::T_MUL); 
		  break;

	  case TEXT('/'):
		  return Token(TokenIdentity::T_DIV); 
		  break;

	  case TEXT('('):
		  return Token(TokenIdentity::T_LEFT_PAREN); 
		  break;

	  case TEXT(')'):
		  return Token(TokenIdentity::T_RIGHT_PAREN); 
		  break;

	  //if none of the above apply, the token may be a value or a reference
	  default:
		  double dValue;
		  Reference reference;
		  if (ScanValue(dValue))
		  {
			  return Token(dValue);
		  }
		  else if (ScanReference(reference))
		  {
			 
			  return Token(reference);
		  }
		  else
		  {
			  //throw Error();
			  CString stMessage;
			  stMessage.Format(TEXT("Unknown character: \"%c\"."), m_stBuffer[0]);
			  throw stMessage;
		  }
		  break;
	}


}


// ScanValue
// first scan for possible + or - signs and then for digits
// if the last digit is followed by a decimal point it scans for more digits
// after finding at least one digit, the value is converted to a double and
// returns true
BOOL Scanner::ScanValue(double& dValue)
{
	int charCount;

	//this piece is from Björnander
	int fieldCount = _stscanf_s(m_stBuffer,TEXT("%lf%n"),&dValue,&charCount);
	//CString stValue = ScanSign();
	//stValue.Append (ScanDigits());
	if (fieldCount > 0)
	{
		m_stBuffer.Delete(0,charCount);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL Scanner::ScanReference(Reference& reference)
{
	//check if the first two characters are a character and a digit
	//and extract column and row from the reference
	if (isalpha(m_stBuffer[0]) && isdigit(m_stBuffer[1]))
	{
		reference.SetCol(tolower(m_stBuffer[0]) - TEXT('a'));
		m_stBuffer.Delete(0);
		//CString stRow = ScanDigits();
		//this piece is from Björnander
		int row;
		int charCount;
		_stscanf_s(m_stBuffer,TEXT("%d%n"),&row,&charCount);
		reference.SetRow (row-1);
		m_stBuffer.Delete(0,charCount);
		return TRUE;
	}
	else
		return FALSE;

}

