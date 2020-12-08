# Anubadak
The goal of this Project was to implement a lexer, parser, interpreter and Read-Eval-Print-Loop environment for 
the programming language Anubadhak. Anubadhak is an expression-oriented strongly-typed programming language. It
will supports C-style comments, that is, //... for single-line comments and /*...*/ for multiline comments, is 
case-sensitive, and every function is expected to return a value. There will be four data types in the language: 
int for integer values, real for ﬂoating point numbers, bool for Boolean values and string for string literals. 
C++ will be used in backend to convert the provided input text file program to the native language  of the computers.

# To Run this project(in Visual studio)
  * first you need to download required version of wxWidget from [Click here to download wxWidget](https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.3/wxWidgets-3.1.3.zip)
  * Now search for build/msw directory in the downloaded directory
  * Open your required version of visual studio solution for Visual Studio 2019 (wx_vc16.sln) then VS is opened.
  * Now we need to build the wxWidget Library so build the Library in Debug configuration by clicking build.

# Final step
  Now you can clone this project wherever you like and  Run from Visual Studio Code and you may see the Running application.
  
# some properties of class of language accepted by anubadak
- Strongly typed language
- Support functions, conditionals, loop etc.
- Supports single line comment and multi line comment
- Four data types int , real, string, bool.
- Supports function overloading
- int to  real auto conversion during assignment
- Supports variable scope and variable shadowing

example code to calculate cos(x) is Anubadak

```C
// Pi
var pi : real = 3.1415926535897932;
// Cosine function
def cos(x : real) : real {
        set x=pi *x/180.0;
    // positive integer power
    def pow(x : real, n : int) : real {
        var y : real = 1;
        while(n > 0){
            set y = y*x;
            set n = n-1;
        }
        return y;
    }
    // Factorial
    def fac(n : int) : int {
        if (n == 0) { return 1; } else { return n * fac(n-1); }
    }
    // Take 8 terms of Maclaurin series for cos(x)
    var k : int = 0;
    var cos_x : real = 0;
        print("\n");
    while(k < 8){
         set cos_x = cos_x + pow( -1.0, k) * pow(x, 2*k) / fac(2*k);
         set k = k + 1;
    }
    return cos_x;
}
var angle :real=0;
while(angle<=180){
	var coss: real=cos(angle);
	print("value of cos  ( ");
        print(angle);
	print(") =");
        print(coss);
        print("\n");
        set angle=angle+30;

}

```
