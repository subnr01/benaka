/*

https://en.wikibooks.org/wiki/C%2B%2B_Programming/Idioms#Pointer_To_Implementation_.28pImpl.29
*/


/*

The "pointer to implementation" (pImpl) idiom, also called the "opaque pointer" idiom, is a method of providing data and thus further implementation abstraction for Classes.
*/

/* public.h */
class Book
{
public:
  Book();
  ~Book();
  void print();
private:
  class BookImpl;
  BookImpl* const m_p;
};


class Book::BookImpl
{
public:
  void print();
private:
  std::string  m_Contents;
  std::string  m_Title;
}

Book::Book(): m_p(new BookImpl())
{
}

Book::~Book()
{
  delete m_p;
}

void Book::print()
{
  m_p->print();
}

/* then BookImpl functions */

void Book::BookImpl::print()
{
  std::cout << "print from BookImpl" << std::endl;
}


int main()
{
  Book b;
  b.print();
}


