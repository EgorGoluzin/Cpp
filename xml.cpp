#include "xml.h"

XML::Node::Node(const std::string& tag, const std::string& value) : tag(tag), value(value) {};

void XML::Node::add(std::unique_ptr<XML::Node> new_child) 
{
    child.push_back(std::move(new_child));
};

XML::Node::iterator::iterator(Node* node)
{
    if (node)
    {
        nodes = node->get_descendants();
        nodes.insert(nodes.begin(), node);
    }
    cur_node_index = 0;
};

XML::Node::iterator& XML::Node::iterator::operator ++ ()
{
    *this += 1;
    return *this;
};

XML::Node::iterator XML::Node::iterator::operator ++ (int)
{
    iterator temp(*this);
    ++(*this);
    return temp;
};

void XML::Node::for_each(std::function<void(const Node*)> callback)
{
    callback(this);
    for (const auto& child : child) 
        child->for_each(callback);
};

std::vector<XML::Node*> XML::Node::get_descendants() 
{
    std::vector<Node*> descendants = {};

    for (const auto& child : child)
    {
        descendants.push_back(child.get());
        std::vector<Node*> child_descendants = child->get_descendants();
        descendants.insert(descendants.end(), child_descendants.begin(), child_descendants.end());
    }

    return descendants;
};

std::unique_ptr<XML::Node> XML::Node::clone() const 
{
    std::unique_ptr<XML::Node> copy = std::make_unique<Node>(tag, value);

    for (const auto& child : child) 
        copy->child.push_back(child->clone());

    return copy;
};

XML::Node::iterator XML::Node::find(std::function<bool(Node* node)> callback) 
{
    if (callback(this)) 
        return XML::Node::iterator(this);

    for (const auto& child : child) 
    {
        XML::Node::iterator node = child->find(callback);
        if (*node != nullptr)
            return node;
    }

    return XML::Node::iterator(nullptr);
};

XML::Node::iterator& XML::Node::iterator::operator += (int n)
{
    cur_node_index += n;
    if (cur_node_index > nodes.size()) 
        cur_node_index = nodes.size();

    return *this;
};

bool XML::Node::iterator::operator == (const iterator& other) const 
{
    return **this == *other;
};

bool XML::Node::iterator::operator != (const iterator& other) const 
{
    return !(*this == other);
};

XML::Node* XML::Node::iterator::operator * () const 
{
    if (nodes.empty() || cur_node_index == nodes.size()) 
        return nullptr;

    return nodes[cur_node_index];
};

XML::Node* XML::Node::iterator::operator -> () const 
{
    return **this;
};

XML::Node::iterator XML::Node::begin() 
{
    return XML::Node::iterator(this);
};

XML::Node::iterator XML::Node::end()
{
    return XML::Node::iterator(nullptr);
};

XML::Doc::Doc() : root_node(nullptr) {};

XML::Doc::Doc(const std::string& path) : root_node(nullptr) 
{
    load(path);
};

void XML::Doc::parse(const std::string& xml) 
{
    int pos = 0;
    root_node = parse_node(xml, pos);
};

void XML::Doc::load(const std::string& path) 
{
    const std::string xml = read_file(path);
    parse(xml);
};

void XML::Doc::save(const std::string& path) 
{
    if (!root_node) 
        throw std::runtime_error("XML tree is not loaded");
    
    std::string xml = stringify();
    write_file(path, xml);
};

std::string XML::Node::stringify(const int depth = 0)
{
    const std::string indent = std::string(depth * 2, ' ');
    std::string result = "";

    result += indent + "<" + tag + ">" + value;
    if (child.size()) {
        result += "\n";
    }
    for (const auto& child : child) {
        result += child->stringify(depth + 1);
    }
    if (child.size()) {
        result += indent;
    }
    result += "</" + tag + ">" + "\n";

    return result;
};

void XML::Doc::print()
{
    if (!root_node) 
        throw std::runtime_error("XML tree is not loaded");
    
    std::string xml = stringify();
    std::cout << xml << std::endl;
};
void XML::Doc::for_each(std::function<void(const Node*)> callback) 
{
    if (!root_node)
        return;

    std::unique_ptr<Node> backup = root_node->clone();

    try 
    {
        root_node->for_each(callback);
    }
    catch (...)
    {
        root_node = std::move(backup);
        throw;
    }
};

std::unique_ptr<XML::Node> XML::Doc::parse_node(const std::string& xml, int& pos) 
{
    std::string tag = get_next_tag(xml, pos);
    std::string value = get_next_value(xml, pos);
    std::unique_ptr<XML::Node> node(new XML::Node(tag, value));

    std::string next_tag = get_next_tag(xml, pos);
    while (next_tag != ("/" + tag) && pos < xml.size()) 
    {
        pos -= next_tag.size() + 2;
        if (pos < 0)
            throw std::runtime_error("Invalid XML");

        node->add(parse_node(xml, pos));
        next_tag = get_next_tag(xml, pos);
    }

    return node;
};

std::string XML::Doc::get_next_tag(const std::string& xml, int& pos) 
{
    pos = xml.find("<", pos) + 1;
    int end = xml.find(">", pos);
    std::string tag = xml.substr(pos, end - pos);
    pos = end + 1;
    return tag;
};

std::string XML::Doc::get_next_value(const std::string& xml, int& pos) 
{
    int start = pos;
    pos = xml.find("<", pos);
    return trim(xml.substr(start, pos - start));
};

std::string XML::Doc::read_file(const std::string& path) 
{
    std::ifstream file(path);

    if (!file) 
        throw std::runtime_error("File not found");

    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
};

void XML::Doc::write_file(const std::string& path, const std::string& content) 
{
    std::ofstream file(path);

    if (!file) 
        throw std::runtime_error("File not found");

    file << content;
};

std::string XML::Doc::stringify() 
{
    if (!root_node)
        throw std::runtime_error("Tree isn't load");

    return root_node->stringify();
};

std::string XML::Doc::trim(const std::string& str) 
{
    auto front = std::find_if_not(str.begin(), str.end(), [](int c) 
        {
        return std::isspace(c);
        });

    auto back = std::find_if_not(str.rbegin(), str.rend(), [](int c) 
        {
        return std::isspace(c);
        }).base();
        return (back <= front ? std::string() : std::string(front, back));
};

XML::Node::iterator XML::Doc::begin() 
{
    return root_node->begin();
};

XML::Node::iterator XML::Doc::end() 
{
    return root_node->end();
};

XML::Node::iterator XML::Doc::find(std::function<bool(Node* node)> callback) 
{
    if (!root_node) 
        return end();

    return root_node->find(callback);
};

XML::Node::iterator XML::Doc::find_by_tag(const std::string& tag) 
{
    if (!root_node) 
        return end();

    return root_node->find([&tag](Node* node)
        {
        return node->tag == tag;
        });
};

XML::Node::iterator XML::Doc::find_by_value(const std::string& value) 
{
    if (!root_node) 
        return end();

    return root_node->find([&value](Node* node) 
        {
        return node->value == value;
        });
};

XML::Node::iterator XML::Doc::add(XML::Node::iterator it, Node* node)
{
    if (!root_node) 
        throw std::runtime_error("Tree isn't load");
 
    if (!*it) 
        throw std::runtime_error("Node doesn't exist");

    it->add(std::unique_ptr<Node>(node));
    return XML::Node::iterator(node);
};

bool XML::Doc::del(Node::iterator it)
{
    if (!root_node) 
        return false;

    if (*it == root_node.get())
        return false;

    auto parent_it = find([&](Node* node)
        {
        auto vec_it = std::find_if(
            node->child.begin(),
            node->child.end(),
            [&](const std::unique_ptr<Node>& ptr) 
            {
                return ptr.get() == *it;
            }
        );

        return vec_it != node->child.end();
        });

    if (parent_it == end()) 
        return false;

    std::move(
        it->child.begin(),
        it->child.end(),
        std::back_inserter(parent_it->child)
    );

    auto vec_it = std::find_if(
        parent_it->child.begin(),
        parent_it->child.end(),
        [&](const std::unique_ptr<Node>& ptr) {
            return ptr.get() == *it;
        }
    );
    parent_it->child.erase(vec_it);

    return true;
};