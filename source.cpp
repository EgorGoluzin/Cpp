#include "source.h"

std::unique_ptr<Source> Source::create() 
{
	return std::unique_ptr<Source>(new Source());
};

std::unique_ptr<Source> Source::create(const std::string& path)
{
	std::unique_ptr<Source> instance = Source::create();
	instance->load(path);
	return instance;
};

Source::Source() : Doc(std::make_unique<XML::Doc>()) {};

XML::Node::iterator Source::find(std::function<bool(XML::Node* node)> callback)
{
	return Doc->find(callback);
};

XML::Node::iterator Source::find_by_tag(const std::string& tag)
{
	return Doc->find_by_tag(tag);
};

XML::Node::iterator Source::find_by_value(const std::string& tag)
{
	return Doc->find_by_value(tag);
};

XML::Node::iterator Source::add(XML::Node::iterator it, XML::Node* node)
{
	return Doc->add(it, node);
};

bool Source::del(XML::Node::iterator it)
{
	return Doc->del(it);
};

void Source::parse(const std::string& xml) 
{
	return Doc->parse(xml);
};

void Source::load(const std::string& path) 
{
	return Doc->load(path);
};

void Source::save(const std::string& path) 
{
	return Doc->save(path);
};

void Source::print()
{
	return Doc->print();
};

void Source::for_each(std::function<void(const XML::Node*)> callback) 
{
	return Doc->for_each(callback);
};

XML::Node::iterator Source::begin() 
{
	return Doc->begin();
};

XML::Node::iterator Source::end()
{
	return Doc->end();
};
