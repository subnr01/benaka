
/*
http://stackoverflow.com/questions/3559412/how-to-store-different-data-types-in-one-list-c

*/


class Property
{
public:
    Property(const std::string& name) //note: we don't lightly copy strings in C++
      : m_name(name) {}
    virtual ~Property() {}
private:
    std::string m_name;
};

template< typename T >
class TypedProperty : public Property
{
public:
    TypedProperty (const std::string& name, const T& data)
      : Property(name), m_data(data);
private:
    T m_data;
};

typedef std::vector< std::shared_ptr<Property> > property_list_type;


void f()
{
  std::vector<Property*> my_property_list;
  for(unsigned int u=0; u<10; ++u)
    my_property_list.push_back(new Property(u));

  use_property_list(my_property_list);

  for(std::vector<Property*>::iterator it=my_property_list.begin();
                                      it!=my_property_list.end(); ++it)
    delete *it;
}
