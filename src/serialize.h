#pragma once

#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "glm/glm.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

using namespace rapidxml;
using namespace std;
using namespace glm;

class ISerializer
{
public:
    virtual bool InOut(string name, int& inout_val) = 0;
    virtual bool InOut(string name, float& inout_val) = 0;
    virtual bool InOut(string name, vec3& inout_val) = 0;
    virtual bool InOut(string name, vec2& inout_val) = 0;
    virtual bool InOut(string name, uvec2& inout_val) = 0;
    virtual bool InOut(string name, string& inout_val) = 0;
};

class XMLSerializerReader: public ISerializer
{
    string filename;
    xml_document<> doc;
    istringstream getStream(string name);
public:
    XMLSerializerReader(const string& s) : filename(s) {}
    bool open();
    bool close();
    bool InOut(string name, int& inout_val);
    bool InOut(string name, float& inout_val);
    bool InOut(string name, vec3& inout_val);
    bool InOut(string name, vec2& inout_val);
    bool InOut(string name, uvec2& inout_val);
    bool InOut(string name, string& val);
};
 
class XMLSerializerWriter: public ISerializer
{
    string filename;
    xml_document<> doc;
public:
    XMLSerializerWriter(const string& s) : filename(s) {}
    bool open();
    bool close();
    bool InOut(string name, int& inout_val);
    bool InOut(string name, float& inout_val);
    bool InOut(string name, vec3& inout_val);
    bool InOut(string name, vec2& inout_val);
    bool InOut(string name, uvec2& inout_val);
    bool InOut(string name, string& val);
};
