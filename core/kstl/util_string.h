/* 
Class describing string

*/


class string
{
  public:
    typedef size_t size_type;
    typedef char value_type;
  
    //constructors
    string();
    string(const char* pStr);
    string(const char* pStr, size_t len);
    string(const string& str);
    string(const string& str, size_t pos, size_t len = npos);
    ~string();
  
    //Get substring that starts at position i up to len char
    string substr(size_t i = 0, size_t len = npos) const;
  
    //Assignment Operator
    string &operator = (const string &rhs);
    string &operator = (const char *);
    
    //Return the character at the specified index in the string
    const char& operator [](size_t i) const;
    char &operator[](size_t i);
  
  
    //Performs a case sensitive compare with the provided string str
    int compare(size_type pos, size_type len, const string& str) const;
    int compare(const string& str) const;
    int compare(const char *pStr) const;
  
  
    //Append the specified string to the end of this string
    string &operator +=(const string &str);
    string &operator +=(const char *pStr);
    string &append(const string &str);
    string &append(const char *, size_t);
    string &append(const char *);
  
    //Concatenate
    friend string operator +(const string &lhs, const string &rhs);
    
    //Return a hash of the string
    size_t hash() const;
  
    //size...
    size_t size() const;
    size_t length() const;
    bool empty() const;
  
    //Return const pointer to null terminated contents
    const char* c_str() const;
  
    //Assign value to a C string
    string& assign(const char* pStr, uint32_t flag = SKMA_WAITOK);
    string& assign(const string& str);
  
    //Erase string
    void clear();
    string &erase(size_type pos = 0, size_type len = npos);
  
    //Replace characters with a C substring
    string &replace(size_type pos, size_type len1, const char *pStr, size_type len2); 

    
    //Find
    

};
    
    
  
