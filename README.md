# evalLaTeX
Calculates the value of an equation written in LaTeX.

## Usage
1. Add eTree.cpp file to your project.
2. include eTree.h where you want to use evalLaTeX.


## Example
This example shows how to make a function from LaTeX equation code.
Then calculate a value of a function.

1. For $f(x)=x\sin 2x$, caluelate $f(2)$ and $f\left( \dfrac{\pi}{12} \right) $
```C++
latexEquation* f = new latexEquation("x\\sin2x");    // f(x)=x*sin2x
double f1 = f->calculate(2);    //f1 = -1.5136 (=2sin4)
double f2 = f->calculate("\\pi/12");    //f2 = 0.1309 (=pi/24)
printf("%.4f, %.4f\n", f1, f2);            // -1.5136, 0.1309
delete f;
```
<br>

2. For $g(x) = 4e^{x-1}+\dfrac{1}{x}$, calculate $g(1)$ and $g(\ln3+1)$
```C++
latexEquation g;
g.parse("4e^{x-1}+\\dfrac{1}{x}");    // g(x)=4e^{x-1}+1/x
double g1 = g.calculate(1);    //g1 = 5
double g2 = g.calculate("\\ln3+1");    //g2 = 12.4765 (=12+1/(1+ln3))
printf("%.4f, %.4f\n", g1, g2);            // 5, 12.4765
```
<br>

3. For $h(x) = x^3-3x^2+2x$, calculate $h(3)$ and $h'(2)$
```C++
latexEquation* h = new latexEquation("x^3-3x^2+2x");    // h(x)=x^3-3x^2+2x
latexEquation* hDifferential = h->differential();    // h'(x)=3x^2-6x+2
double h1 = h->calculate(3);    //h1 = 6 (=h(3))
double hDiff1 = hDifferential->calculate("2");    //hDiff = 2 (=h'(2))
printf("%.4f, %.4f\n", h1, hDiff1);            // 6, 2
```
<br>

## Supported Operators and Functions
evalLaTeX can parse operators below.
```LaTeX
+, -, *, /, ^
```
<br>

evalLaTeX can parse LaTeX Functions below.
```LaTeX
\sin, \cos, \tan, \sec, \csc, \cosec, \cot,
\sin^, \cos^, \tan^, \sec^, \csc^, \cosec^, \cot^,
\arcsin, \arccos, \arctan, \arcsec, \arccsc, \arccot,
\sinh, \cosh, \tanh, \sech, \csch, \coth,
\sinh^, \cosh^, \tanh^, \sech^, \csch^, \cosech^, \coth^,
\arcsinh, \arccosh, \arctanh, \arcsech, \arccsch, \arccoth,
\ln, \log, \sqrt, \log_, \pow, \frac, \dfrac, \pi
```
Trigonometric functions, hyperbolic functions and their inverse functions are supported.
Logarithm, Power, Root and Fraction are also supported.
<br>

evalLaTeX parses $a$, $b$, $c$, $e$ as constant. 
value of $a$, $b$ and $c$ can be changed by expTree::SetValue function like below.
```C++
expTree::SetValue("a", "4");             // set a = 4
expTree::SetValue("b", 3.6);             // set b = 3.6
expTree::SetValue("c", "4+3\\pi");       // set c = 4+3pi
double x = atof_latex("a+b+c");          // x = a+b+c = 11.6+3pi = 21.0248
```
value of $e$ is fixed to $\lim_{x \to 0}\left(1+x  \right)^{\frac{1}{x}} = 2.71828...$ and cannot be changed.

## Available Methods and Variables
Methods of a class latexEquation.
<br><br>

```C++
latexEquation();
```
Default Constructor
<br><br>
```C++
latexEquation(double n);
latexEquation(const char *str);
```
Constructor with default value. 
<br><br>
```C++
~latexEquation();
```
Destructor
<br><br>

```C++
void setIdentifierType(int type);
```
CONSTONLY, XONLY and TONLY are available.
- When identifierType is CONSTONLY, independent variable is not allowed. Using independent variable like "x" or "t" will get error when parsing the equation.
- When identifierType is XONLY, independent variable "x" is allowed.
- When identifierType is TONLY, independent variable "t" is allowed.

When object's identifierType is USEGLOBAL, use global value of identifierType instead.<br>
identifier "e" is allowed for any identifierType, which means $e=\lim_{x \to 0}\left(1+x  \right)^{\frac{1}{x}} = 2.71828...$
<br><br>

```C++
bool parse(const char* p);
```
Parse from LaTeX equation. After making object with default constructor, calling parse() is needed.<br>
Check parseSuccess to check if parse() is done successfully.

---
When object is created by default constructor and parse() is called, object's identifierType is set to appropriately considering equation's used variable.<br>
When object's identifierType is USEGLOBAL and the condition of the independent variable is not violated, calling parse() sets object's identifierType to global identifierType, regardless of parse()'s success.
<br><br>
```C++
bool parse(const char* p, const char* name);
```
Parse from LaTeX equation. When parsing error occured, error message will be recorded with function name.
<br><br>
```C++
bool parseSuccess;
```
After calling parse(), check this variable if equation is parsed correctly.
<br><br>


```C++
double calculate();
```
Calculate LaTeX Equation and return it's value.
Value of independent variable is derived from global value.
To set global value of independent variable, use expTree::SetValue().
<br><br>
```C++
double calculate(double value);
double calculate(const char* equation);
```
Calculate LaTeX Equation with given independent variable's value, or value calculated from string.
<br><br>
```C++
latexEquation* differential();
```
Get derivative of LaTeX Equation. Differential is performed with respect to the independent variable $x$ or $t$.
<br><br>

## Functions
Functions to control calculating process.
<br><br>

```C++
void expTree::setIdentifierType(int type);
```
When object's identifierType is set to -1, object's identifierType is set to global identifierType.
CONSTONLY, XONLY and TONLY are available.
- When identifierType is CONSTONLY, independent variable is not allowed. Using independent variable like "x" or "t" will get error when parsing the equation.
- When identifierType is XONLY, independent variable "x" is allowed.
- When identifierType is TONLY, independent variable "t" is allowed.
<br><br>

```C++
void expTree::SetValue(const char* identifier, double value)
void expTree::SetValue(const char* identifier, double value)
```
Set global value of independent variable.
When latexEquation::calculate() is called without any parameter, global value of independent variable is used.
Only "x" and "t" are allowed for identifiers.
expTree::SetValue("x",0.25) will set global x to 0.25.
<br><br>

```C++
double atof_latex(const char* equation);
```
Calculate equation without construct latexEquation object.
<br><br>

```C++
double atof_latex(const char* equation,double value);
double atof_latex(const char* equation,const char *value);
```
Calculate equation with independent variable, without construct latexEquation object.

## Type Definition
- eTree : Parsing tree to calculate mathematic equation.
- latexEquation : Another intuitive name of the type eTree. 

## Caution
All calculation is performed in real space $\mathbb{R}$. Independent variables must be in domain of a function.
```C++
latexEquation* w = new latexEquation("\\sqrt{x}");    // h(x)=\\sqrt{x}
double w1 = w->calculate(-1);    //h1 = \\sqrt{-1}  (error)
```
Code like above can cause an error, because $-1$ is not in domain of a function $f(x)=\sqrt{x}$