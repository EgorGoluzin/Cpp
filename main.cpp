#include "xml.h"
#include "source.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>


int main() 
{
    std::unique_ptr<Source> example = Source::create("./test.xml");

    example->print();

    for (Source::iterator it = example->begin(); it != example->end(); it += 1)
    {
        std::cout << it->tag << ":" << it->value << std::endl;
    }

    std::cout << std::endl;

    Source::iterator it1 = example->find_by_tag("city");
    std::cout << "Family living in " << it1->value << std::endl;

    Source::iterator it2 = example->find_by_value("Igor");
    std::cout << "Igor is the name of " << it2->tag << std::endl;

    Source::iterator it4 = example->find_by_tag("name");
    example->del(it4);

    std::cout << "Anna is pregnant.." << std::endl;
    XML::Node* favorite_musician_node = new XML::Node("second_grandson", "Sergey");
    Source::iterator it3 = example->find_by_tag("first_daughter");
    example->add(it3, favorite_musician_node);
    std::cout << "Congratulations! There was born new grandson of family!" << std::endl;

    example->save("./res.xml");

    return 0;
};