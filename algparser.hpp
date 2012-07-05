/**

/+ The AlgorithmParser class is: +/

   Copyright (C) 2012 Edward Chernysh

   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
   documentation files (the "Software"), to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
   to permit persons to whom the Software is furnished to do so, subject to the following conditions:

   - The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**/

#pragma once

#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <sstream>

typedef unsigned int uint;
namespace AlgParser {

#define __DEADCHAR (-1)

#ifndef __CBOOLS
#define __CFALSE 0
#define __CTRUE 1
#endif
          
class AlgorithmParser {
      public:          
             AlgorithmParser(double _value) { this->Value = _value; }
             AlgorithmParser(void) { this->Value = 0.0; }
             volatile double Value;              
             
             /* <operator_management>
                * BEGIN
             */ 
             
             template<typename T>             
             double operator+ (T _increment) { return (this->Value + _increment); }             
             template<typename T>
             double operator- (T _decrement) { return (this->Value - _decrement); }
             template<typename T>
             double operator* (T _factor) { return (this->Value * _factor); }
             template<typename T>
             double operator/ (T _div) {
                     return (_div != 0) ? (this->Value / _div) : this->Value;
             }
             template<typename T>
             double operator^ (T _exp) { return (this->Value ^ _exp); }
             template<typename T>
             double operator% (T _mod) { return (this->Value % _mod); }
             
             // <internal access operators>             
             template<typename T>
             void operator+= (T _val) { (*this).Value += _val; }
             template<typename T>
             void operator-= (T _val) { (*this).Value -= _val; }
             template<typename T>
             void operator*= (T _val) { (*this).Value *= _val; }
             template<typename T>
             void operator/= (T _val) { 
                  if (_val > 0)
                     (*this).Value /= _val; 
             }
             template<typename T>
             void operator^= (T _val) { (*this).Value ^= _val; }
             template<typename T>
             void operator%= (T _val) { (*this).Value %= _val; }
             template<typename T>
             void operator++ (T) { ((*this).Value)++; }
             template<typename T>
             void operator-- (T) { ((*this).Value)--; }
             // </internal access operators>
             // <comparison_operators>
             template<typename T>
             bool operator< (T _cmp) { return (this->Value < _cmp) ? true : false; }
             template<typename T>
             bool operator<= (T _cmp) { return (this->Value <= _cmp) ? true : false; }
             template<typename T>
             bool operator> (T _cmp) { return (this->Value > _cmp) ? true : false; }
             template<typename T>
             bool operator>= (T _cmp) { return (this->Value >= _cmp) ? true : false; }
             template<typename T>
             bool operator== (T _cmp) { return (this->Value == _cmp) ? true : false; }
             template<typename T>
             bool operator!= (T _cmp) { return (this->Value != _cmp) ? true : false; }
             // </comparison_operators>
                          
             /* */
             /* </operator_management>
                * END
             */
             
             // ...
             // Define AlgorithmParser's const Order of Operations handler
             // Does not /yet/ support nesting!
             void Bedmass(std::string &s)
             {
                  std::string subtext = s.substr(s.find("(") + 1, s.find(")") - 1);
                  if (this->isformatted(subtext) == true && (s).size() >= 3) {                     
                     std::stringstream sObj;
                     sObj << ((*this).calculate(subtext));
                     std::string sresult = sObj.str();                                             
                     s.replace(s.find("(", 0), s.find("(", 0) + subtext.size() + 1, sresult);                                        
                     delete &sresult;
                  }
                  delete &subtext;
             }             
             // <end_const_definition>                                  
             
             static bool isformatted(std::string str)
             {
                 bool ret = true;   
                 if (str.find("(", 0) != (__DEADCHAR))
                    if (cOccur(str, '(') != cOccur(str, ')'))
                       ret = false;                  
                 if (str.find(")", 0) != (__DEADCHAR))
                    if (cOccur(str, '(') != cOccur(str, ')'))
                       ret = false;
                 return ret;                    
             }             
             static size_t tokenCount(std::string str)
             {
                 int occur = 0;
                 for (int i = 0; i < str.size(); ++i)
                     if (str[i] == '+' || str[i] == '-' 
                        || str[i] == '*' || str[i] == '/' || str[i] == '^' )
                           occur++;
                 return occur;          
             }
             
             volatile double calculate(std::string math)
             {
                 volatile double result = (isdigit(math[0])) ? (atof(&(math[0])) + (0.0)) : 0.0;
                 for (int i = 0; i < math.size(); i++)
                 {
                     if (isToken(math[i]) == true)
                     {
                         if (i < (math.size() - 1))
                            if (isToken(math[i + 1]) == true) {
                               std::cout << "\tError:Syntax\n" << std::endl;
                               break;
                            }                                                
                         if (i >= 1 && i < (math.size() - 1))
                            switch (get_token(math[i]))
                            {
                                case PLS:
                                     result += atof(&math[i + 1]);
                                     i++; 
                                break;  
                                case MIN:
                                     result -= atof(&math[i + 1]);
                                     i++;
                                break;
                                case MUL:
                                     result *= atof(&math[i + 1]);
                                     i++;
                                break;  
                                case DIV:
                                     result /= atof(&math[i + 1]);
                                     i++;
                                break; 
                                case EXP:
                                     result = (pow(result, atof(&math[i + 1])));
                                     i++;
                                break;
                            }                      
                     }               
                 }
                 this->Value = result;
                 return result;
             }             
             static int bracketCount(std::string s)
             {
                 int left = 0, right = 0;
                 for (int i = 0; i < s.size(); ++i)
                     if (s[i] == '(')
                        left++;
                     else if (s[i] == ')')
                        right++;
                 if (left == right)
                    return left;
                 else
                    return (EOF); 
             }
             
             static size_t digitCount(std::string str)
             {
                 int count = 0;
                 for (int i = 0; i < str.size(); i++)
                     if (isdigit(str[i]) == __CTRUE)
                        count++;
                 return count;                                         
             }
      private:              
             enum Tokens { PLS, MIN, MUL, DIV, EXP, MOD , NONE='0'};   
             static int cOccur(std::string s, char _token)
             {
                 int count = 0;
                 for (int i = 0; i < s.size(); ++i)
                     if (s[i] == _token)
                        ++count;
                 return count;                       
             }
             virtual bool isToken(char c)
             {
                  if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
                      return true;
                  else
                      return false;
             }
             virtual Tokens get_token(char c)
             {
                     Tokens tok = NONE;
                     switch (c) {
                            case '+':
                                 tok = PLS;
                            break;
                            case '-':
                                 tok = MIN;
                            break;
                            case '*':
                                 tok = MUL;
                            break;     
                            case '/':
                                 tok = DIV;
                            break;
                            case '^':
                                 tok = EXP;
                            break;
                            default:
                                    ;                            
                     };
                     return tok;                     
             }
             long factorial(double dval)
             {
                    long fact = (long)dval;
                    for (short int i = ((int)dval - 1); i > 0; i--)
                        dval *= (long)i;
                    return fact;      
             }
             inline size_t FirstIndexOf(std::string str, char c) const {
                 int index = 0;
                 for (uint i = 0; i < str.size(); ++i)
                     if (str[i] == c)
                        break;
                 return index;                
             }
             
};        
};
