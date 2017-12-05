//------------------------------------------------------------------------------------
// 1.2 Introductory Example or How to Replace a Switch-Statement
// Task: Perform one of the four basic arithmetic operations specified by the
//       characters '+', '-', '*' or '/'.


// The four arithmetic operations ... one of these functions is selected
// at runtime with a swicth or a function pointer
float Plus    (float a, float b) { return a+b; }
float Minus   (float a, float b) { return a-b; }
float Multiply(float a, float b) { return a*b; }
float Divide  (float a, float b) { return a/b; }


// Solution with a switch-statement - <opCode> specifies which operation to execute
void Switch(float a, float b, char opCode)
{
   float result;

   // execute operation
   switch(opCode)
   {
      case '+' : result = Plus     (a, b); break;
      case '-' : result = Minus    (a, b); break;
      case '*' : result = Multiply (a, b); break;
      case '/' : result = Divide   (a, b); break;
   }

   cout << "Switch: 2+5=" << result << endl;         // display result
}


// Solution with a function pointer - <pt2Func> is a function pointer and points to
// a function which takes two floats and returns a float. The function pointer
// "specifies" which operation shall be executed.
void Switch_With_Function_Pointer(float a, float b, float (*pt2Func)(float, float))
{
   float result = pt2Func(a, b);    // call using function pointer

   cout << "Switch replaced by function pointer: 2-5=";  // display result
   cout << result << endl;
}


// Execute example code
void Replace_A_Switch()
{
   cout << endl << "Executing function 'Replace_A_Switch'" << endl;

   Switch(2, 5, /* '+' specifies function 'Plus' to be executed */ '+');
   Switch_With_Function_Pointer(2, 5, /* pointer to function 'Minus' */ &Minus);
}
