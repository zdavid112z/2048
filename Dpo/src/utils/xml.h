#pragma once

#include "pch.h"
#include "string.h"

class XMLAttribute
{
public:
	std::string name;
	std::string value;
};

class XMLNode
{
public:
	XMLNode();
	~XMLNode();

	const XMLAttribute* GetAttribute(const std::string& name) const;
	XMLAttribute* GetAttribute(const std::string& name);
	const XMLNode* GetNode(const std::string& name, uint count = 0) const;
	XMLNode* GetNode(const std::string& name, uint count = 0);
	const XMLNode* GetNodeWithAttribute(const std::string& nodeName, const std::string& attributeName, const std::string& attributeValue) const;
	XMLNode* GetNodeWithAttribute(const std::string& nodeName, const std::string& attributeName, const std::string& attributeValue);

	std::vector<XMLAttribute> attributes;
	std::vector<XMLNode*> children;
	std::string content;
	std::string name;
};

class XMLFile
{
public:
	XMLFile();
	~XMLFile();
	
	// Finds the node that follows the path provided
	// The root node's name is ignored, as an example,
	// "graphics/fxaa" is a correct path for the file
	// <settings>
	//   <graphics>
	//     <fxaa>1</fxaa>
	//   ...
	//
	// If the node exists, it gets assigned that value
	// Returns true if the value was changed
	template <typename T>
	bool SetVariable(const std::string& path, T& value)
	{
		bool found;
		XMLNode* current = root;
		std::vector<std::string> parts = String::Split(path, "/\\");
		for(uint i = (parts[0] == "" || parts[0] == root->name) ? 1 : 0; i < parts.size(); i++)
		{
			found = false;
			for (uint j = 0; j < current->children.size(); j++)
			{
				if (current->children[j]->name == parts[i])
				{
					current = current->children[j];
					found = true;
					break;
				}
			}
			if (!found) 
				return false;
		}
		value = String::ValueOf<T>(current->content);
		return true;
	}

	XMLNode* prolog;
	XMLNode* root;
private:

};

class XMLFileLoader
{
public:
	static XMLFile* LoadFile(const std::string& path);
	static XMLFile* LoadFileFromMemory(const std::string& text);
private:
	static bool HasNode();
	static XMLNode* ParseNode();
	static bool HasAttribute();
	static XMLAttribute ParseAttribute();
	static bool HasContent();
	static std::string ParseContent();
	static void JumpSpaces();
	static void SkipToFirst(char ch);
	static void SkipToFirstSpace();
	static void SkipToFirstSpaceOr(char ch);
	static void SkipToFirstSpaceOr(char ch1, char ch2, char ch3);
	static std::string GetLineAndColumn(const std::string& text);
	static XMLNode* ParseProlog();
	static std::string RemoveComments(const std::string& str);
	static bool HasProlog();

	static const char* str;
	static std::string error;
};

struct XMLOutputOptions
{
	bool indent = true;
	int spacesToIndent = 2;
};

class XMLFileWriter
{
public:
	static std::string WriteToMemory(XMLFile* xml, XMLOutputOptions options = XMLOutputOptions());
	static void WriteToFile(XMLFile* xml, const std::string& path, XMLOutputOptions options = XMLOutputOptions());
private:
	static void WriteNode(XMLNode* node, uint depth);
	static void WriteFormatted(const std::string& text);
	static void WriteIndent(uint depth);
	static void WriteProlog(XMLNode* node);
	static std::stringstream ss;
	static XMLOutputOptions options;
};

