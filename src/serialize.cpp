#include "serialize.h"

//----------

class NotFoundException {};

bool XMLSerializerReader::open()
{
    string input_xml;
    string line;
    ifstream fin(filename.c_str());
    while(getline(fin,line))
        input_xml += line;

    char *text = doc.allocate_string(input_xml.c_str());        
    doc.parse<0>(text);
    return true;
}

bool XMLSerializerReader::close()
{
    doc.clear();
    return true;
}

bool XMLSerializerReader::InOut(string name, int& inout_val)
{
    istringstream ss;
    try {
        ss = getStream(name);
    } catch(NotFoundException) {
        return false;
    }
    ss >> inout_val;   
    return true;
}

bool XMLSerializerReader::InOut(string name, float& inout_val)
{
    istringstream ss;
    try {
        ss = getStream(name);
    } catch(NotFoundException) {
        return false;
    }
    ss >> inout_val;   
    return true;
}

bool XMLSerializerReader::InOut(string name, vec3& inout_val)
{
    istringstream ss = getStream(name);
    ss >> inout_val.x >> inout_val.y >> inout_val.z;
    return true;
}

bool XMLSerializerReader::InOut(string name, vec2& inout_val)
{
    istringstream ss;
    try {
        ss = getStream(name);
    } catch(NotFoundException) {
        return false;
    }
    ss >> inout_val.x >> inout_val.y;    
    return true;
}

bool XMLSerializerReader::InOut(string name, uvec2& inout_val)
{
    istringstream ss;
    try {
        ss = getStream(name);
    } catch(NotFoundException) {
        return false;
    }
    ss >> inout_val.x >> inout_val.y;    
    return true;
}

bool XMLSerializerReader::InOut(string name, string& inout_val)
{
    char *node_name = doc.allocate_string(name.c_str());
    xml_node<>* node = doc.first_node(node_name);
    if(!node) {
        return false;
    }
    inout_val = node->value();
    return true;
}

istringstream XMLSerializerReader::getStream(string name)
{
    char *node_name = doc.allocate_string(name.c_str());
    xml_node<>* node = doc.first_node(node_name);
    if(!node) {
        throw NotFoundException();
    }
    string s(node->value());
    istringstream ss(s);
    return ss;
}

//----------

bool XMLSerializerWriter::open()
{
    doc.clear();
    return true;
}

bool XMLSerializerWriter::close()
{
    ofstream fout(filename.c_str());
    fout << doc;
    fout.close();
    return true;
}

bool XMLSerializerWriter::InOut(string name, int& inout_val)
{
    stringstream ss;
    ss << inout_val;
    InOut(name, ss.str());
    return true;
}

bool XMLSerializerWriter::InOut(string name, float& inout_val)
{
    stringstream ss;
    ss << inout_val;
    InOut(name, ss.str());
    return true;
}

bool XMLSerializerWriter::InOut(string name, vec3& inout_val)
{
    stringstream ss;
    ss << inout_val.x << ' ' << inout_val.y << ' ' << inout_val.z;
    InOut(name, ss.str());
    return true;
}

bool XMLSerializerWriter::InOut(string name, vec2& inout_val)
{
    stringstream ss;
    ss << inout_val.x << ' ' << inout_val.y;
    InOut(name, ss.str());
    return true;
}

bool XMLSerializerWriter::InOut(string name, uvec2& inout_val)
{
    stringstream ss;
    ss << inout_val.x << ' ' << inout_val.y;
    InOut(name, ss.str());
    return true;
}

bool XMLSerializerWriter::InOut(string name, string& val)
{
    char *node_name = doc.allocate_string(name.c_str());        
    char *node_data = doc.allocate_string(val.c_str());       

    xml_node<> *node = doc.allocate_node(node_element, node_name, node_data);
    doc.append_node(node);
    return true;
}