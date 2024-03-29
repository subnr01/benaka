/*

Design String class using OOAD
*/


#ifndef MY_STRING_H
#define MY_STRING_H

#include <iostream.h>
#include <string.h>         // needed for strlen, strcpy

// Helper functions:
//
// my_strlen : count the length of a string until a null char
//
// my_strncpy : copy n chars from source to destination

int my_strlen( char * ptr ){    
  int len = 0;
  char * p = ptr;
  while( *p != '\0' ) {
    len++;
    p++;
  }
  return len;
}

void my_strncpy( char * ptr_dest, char * ptr_src, int n ){
  for( int i=0; i < n; i++ )
    ptr_dest[i] = ptr_src[i];
}

class myString {
  int length;               // length of the string
  char * buff;              // pointer to strorage
public:

  // Constructors: default, from a string literal (char*), copy constr.
  // When possible, constructors should use initialization lists rather 
  // than assignments. Thus:
  //        buff( new char[length] )   in the init list is better that
  //        buff = new char[length]    in the body of the constructor,
  //   although they do the same thing -- allocate dyn. memory

  myString() : length(0), buff(NULL) 
    {
      cout << "  myString default constr.\n";
    }

  myString( char* init_val ) : 
    length( my_strlen(init_val) ),    // count the length of init value
    buff(   new char[length] )     // allocate storage 
    { 
      cout << "  myString( char * ) constr.\n";
      my_strncpy( buff, init_val, length );  // copy init value into storage
    }

  myString( const myString& other ) :
    length( other.length ),        
    buff(   new char[length] ) 
    {
      cout << "  myString copy const.\n";
      my_strncpy( buff, other.buff, length );
    }

  ~myString() 
    { 
      cout << "  myString destr.\n";
      delete [] buff; 
    }

  int size() { return length; }
  
  // operator= returns  myString&  to allow multiple assignments

  myString& operator= ( const myString& other )
    {
      cout << "  myString::operator=\n";
      if( this != &other ){          // guard against  a = a;  
	delete [] buff;              // release old memory & then
	length = other.length;       // allocate new memory 
	buff = new char[length];        
	my_strncpy( buff, other.buff, length );
      }
      return *this;                  // return a reference to itself
    }                                // to allow a = b = c; 
    
  // concatenation of strings; we should also handle chars.
  // notice that + is overloaded thrice: 
  //    myString + myString,  myString + char, char + myString 
  // making operator+ a _friend_ allows for all three types; while
  // the first two can be done with a _member_ as well, the last cannot!

  friend myString operator+( const myString& s1, const myString& s2 );
  friend myString operator+( const myString& s, char c );
  friend myString operator+( char c, const myString& s );
  
  friend ostream& operator<< ( ostream&, const myString& );

  // finally, we want to reveal characters of a string for lookup
  // and assignment (hence we return a _reference_ to a char!)

  char& operator[] (int index )
    {
      if( index < 0 || index > length ) {
	cerr << "Invalid index in myString::operator[]. Aborting...\n";
	exit(-1);   // exit immediately. This is BAD -- somebody has to
	            // first deallocate the strings we allocated, or else
	            // we have a memory leak!  
      }
      return buff[index];
    }
};

// Notice that each one of these operators calls a constructor,
// so a new object (with its own private memory) is created each time.
// Most of the time it's a temporary that will shortly be destroyed.

myString operator+( const myString& s1, const myString& s2 )
{
  myString res;
  res.length = s1.length + s2.length;
  res.buff = new char[ res.length ];
  my_strncpy( res.buff, s1.buff, s1.length );
  my_strncpy( res.buff + s1.length,            // pointer arithmetic! 
	      s2.buff, 
	      s2.length );
  return res;
}

// implement the other two guys yourself

ostream& operator<< ( ostream& os, const myString& s )
{
  // print char after char from buff
  for( int i=0; i < s.length; i++ )  os.put( s.buff[i] );
  return os;         // this is to allow multiple <<, as in  cout << a << b;
}

#endif  // MY_STRING_H

//----------------------------------------------------------------------
//
// File  mystring-test.cc  (.cpp if Visual C++)   -- tests
// 
//----------------------------------------------------------------------

#include <iostream.h>
#include "mystring.h"

void foo( myString& s ); 
void bar( myString  s ); 

int main(){
  
  // construction and initialization

  myString a = "Hello ";
  myString b = a;
  myString c;

  cout << a << b << c << endl;

  cout << "------------------------------------------" << endl; 

  // value swap

  myString u = "123";
  myString v = "class";
  
  c = u;
  u = v;
  v = c;

  cout << u << v << c << endl;

  cout << "------------------------------------------" << endl; 

  // creating a few temporaries and testing +

  myString w = a + u;

  cout << w << endl; 

  cout << myString( "Beam me up" ) + myString( ", Scotty!") << endl;

  cout << "------------------------------------------" << endl; 

  // a miracle: although operator= is defined only for another myString,
  // the following line has the same effect as 
  //                                           a = myString( "Hi ");
  // Notice that a temporary is contsructed and then destroyed.
  // Lesson: C++ uses your constructor to construct objects as it sees fit.  

  a = "Hi ";

  cout << a << b << endl;

  cout << "------------------------------------------" << endl; 

  // Copy contructor is also used when passing parameters to functions
  // by value (but not by reference)

  myString x = "i am given to foo"; 
  myString y = "i am given to bar"; 

  foo( x );  

  cout << "And now x is: " << x << endl;

  bar( y );                 // notice the call to the copy constr.

  cout << "And now y is: " << y << endl; 

  cout << "------------------------------------------" << endl; 

  // check operator[]

  myString t = "hello old friend.";
  t[0] = 'H';
  t[6] = 'O';
  t[10] = 'F';

  cout << t << endl;

  cout << "------------------------------------------" << endl; 

  return 0;  // return 0 to signal normal completion to the OS
}
  
// both foo and bar attempt to clobber the string. See which one succeeds.

void foo( myString& s ){
  s = "FOO was here!";
}

void bar( myString s ){
  s = "BAR was here!";
}

//----------------------------------------------------------------------
//
//  Output of running  mystring-test  .
// 
//----------------------------------------------------------------------
/*

  myString( char * ) constr.
  myString copy const.
  myString default constr.
Hello Hello 
------------------------------------------
  myString( char * ) constr.
  myString( char * ) constr.
  myString::operator=
  myString::operator=
  myString::operator=
class123123
------------------------------------------
  myString default constr.
  myString copy const.
  myString destr.
Hello class
  myString( char * ) constr.
  myString( char * ) constr.
  myString default constr.
  myString copy const.
  myString destr.
Beam me up, Scotty!
  myString destr.
  myString destr.
  myString destr.
------------------------------------------
  myString( char * ) constr.
  myString::operator=
  myString destr.
Hi Hello 
------------------------------------------
  myString( char * ) constr.
  myString( char * ) constr.
  myString( char * ) constr.
  myString::operator=
  myString destr.
And now x is: FOO was here!
  myString copy const.
  myString( char * ) constr.
  myString::operator=
  myString destr.
  myString destr.
And now y is: i am given to bar
------------------------------------------
  myString( char * ) constr.
Hello Old Friend.
------------------------------------------
  myString destr.
  myString destr.
  myString destr.
  myString destr.
  myString destr.
  myString destr.
  myString destr.
  myString destr.
  myString destr.

*/
