#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "xml.h"

class Source 
{
public:
    static std::unique_ptr<Source> create();
    static std::unique_ptr<Source> create(const std::string& path);

    using iterator = XML::Node::iterator;

    void load(const std::string& path);
    void save(const std::string& path);
    void parse(const std::string& xml);
    void for_each(std::function<void(const XML::Node*)> callback);
    void print();

    XML::Node::iterator begin();
    XML::Node::iterator end();
    XML::Node::iterator find(std::function<bool(XML::Node* node)> callback);
    XML::Node::iterator find_by_tag(const std::string& tag);
    XML::Node::iterator find_by_value(const std::string& value);
    XML::Node::iterator add(XML::Node::iterator it, XML::Node* node);
    bool del(XML::Node::iterator it);

private:
    std::unique_ptr<XML::Doc> Doc;

    Source();
};