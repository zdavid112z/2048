#include "pch.h"
#include "xml.h"

#include <sstream>

#include "string.h"
#include "files.h"

#define EOF_CHECK() if(*str == 0) {error = "Reached end of file too soon!"; return nullptr;}
#define SAFE_ADVANCE() str++; EOF_CHECK()
#define EOF_CHECK_ATTRIBUTE() if(*str == 0) return res
#define SAFE_ADVANCE_ATTRIBUTE() str++; EOF_CHECK_ATTRIBUTE()



const char* XMLFileLoader::str = nullptr;
std::string XMLFileLoader::error = "";
std::stringstream XMLFileWriter::ss;
XMLOutputOptions XMLFileWriter::options;

XMLNode::XMLNode()
{

}

XMLNode::~XMLNode()
{
	for (uint i = 0; i < children.size(); i++)
		if(children[i])
			delete children[i];
}

const XMLAttribute* XMLNode::GetAttribute(const std::string& name) const
{
	for (uint i = 0; i < attributes.size(); i++)
		if (attributes[i].name == name)
			return &attributes[i];
	return nullptr;

}

XMLAttribute* XMLNode::GetAttribute(const std::string& name)
{
	for (uint i = 0; i < attributes.size(); i++)
		if (attributes[i].name == name)
			return &attributes[i];
	return nullptr;
}

const XMLNode* XMLNode::GetNode(const std::string& name, uint count) const
{
	for (uint i = 0; i < children.size(); i++)
		if (children[i]->name == name)
		{
			if (count == 0)
				return children[i];
			count--;
		}
	return nullptr;
}

XMLNode* XMLNode::GetNode(const std::string& name, uint count)
{
	for (uint i = 0; i < children.size(); i++)
		if (children[i]->name == name)
		{
			if (count == 0)
				return children[i];
			count--;
		}
	return nullptr;
}

const XMLNode* XMLNode::GetNodeWithAttribute(const std::string& nodeName, const std::string& attributeName, const std::string& attributeValue) const
{
	for(uint i = 0; i < children.size(); i++)
		if (children[i]->name == nodeName)
		{
			const XMLAttribute* attribute = children[i]->GetAttribute(attributeName);
			if (attribute != nullptr)
			{
				if (attribute->value == attributeValue)
					return children[i];
			}
		}
	return nullptr;
}

XMLNode* XMLNode::GetNodeWithAttribute(const std::string& nodeName, const std::string& attributeName, const std::string& attributeValue)
{
	for (uint i = 0; i < children.size(); i++)
		if (children[i]->name == nodeName)
		{
			const XMLAttribute* attribute = children[i]->GetAttribute(attributeName);
			if (attribute != nullptr)
			{
				if (attribute->value == attributeValue)
					return children[i];
			}
		}
	return nullptr;
}

XMLFile::XMLFile()
{

}

XMLFile::~XMLFile()
{
	if (root) delete root;
	if (prolog) delete prolog;
}

std::string XMLFileLoader::GetLineAndColumn(const std::string& text)
{
	if (*str == 0) return "";
	uint col = 0, line = 1;
	uint len = str - text.data();
	for (uint i = 0; i < len; i++)
	{
		if (text[i] == '\n')
		{
			line++;
			col = 0;
		}
		else col++;
	}
	return "Line: " + String::ToString(line) + "; Col: " + String::ToString(col);
}

XMLFile* XMLFileLoader::LoadFile(const std::string& path)
{
	return LoadFileFromMemory(Files::ReadFileText(path));
}

XMLFile* XMLFileLoader::LoadFileFromMemory(const std::string& text)
{
	std::string preparedText = RemoveComments(text);
	str = preparedText.data();
	error = "";
	XMLFile* file = new XMLFile();
	if (HasProlog())
		file->prolog = ParseProlog();
	else file->prolog = nullptr;
	file->root = ParseNode();
	if (error != "")
	{
		LOG_INFO("Could not parse the XML file!\n");
		LOG_INFO("Error: %s\n", error.c_str());
		LOG_INFO("%s\n", GetLineAndColumn(preparedText).c_str());
		return nullptr;
	}
	return file;
}

void XMLFileLoader::SkipToFirst(char ch)
{
	while (*str != ch && *str != 0)
		str++;
}

void XMLFileLoader::SkipToFirstSpace()
{
	while (*str != ' ' && *str != '\n' && *str != '\t' && *str != '\r' && *str != 0)
		str++;
}

void XMLFileLoader::SkipToFirstSpaceOr(char ch)
{
	while (*str != ' ' && *str != '\n' && *str != '\t' && *str != '\r' && *str != ch && *str != 0)
		str++;
}

void XMLFileLoader::SkipToFirstSpaceOr(char ch1, char ch2, char ch3)
{
	while (*str != ' ' && *str != '\n' && *str != '\t' && *str != '\r' && *str != ch1 && *str != ch2 && *str != ch3 && *str != 0)
		str++;
}

bool XMLFileLoader::HasNode()
{
	const char* old = str;
	if (*str != '<') return false;
	str++;
	JumpSpaces();
	if (*str == '/' || *str == '\\')
	{
		str = old;
		return false;
	}
	str = old;
	return true;
}

bool XMLFileLoader::HasProlog()
{
	const char* old = str;
	if (*str != '<') return false;
	str++;
	JumpSpaces();
	if (*str == '?')
	{
		str = old;
		return true;
	}
	str = old;
	return false;
}

XMLNode* XMLFileLoader::ParseProlog()
{
	XMLNode* res = new XMLNode();
	JumpSpaces();
	EOF_CHECK();
	if (*str != '<')
	{
		error = std::string("Unexpected token '") + *str + "' found! Expected '<'!";
		return nullptr;
	}
	SAFE_ADVANCE();
	if (*str != '?')
	{
		error = std::string("Unexpected token '") + *str + "' found! Expected '?'!";
		return nullptr;
	}
	SAFE_ADVANCE();
	const char* nameBegin = str;
	SkipToFirstSpaceOr('>', '?', '?');
	EOF_CHECK();
	res->name = std::string(nameBegin, str);
	if (res->name != "xml")
	{
		error = "The prolog's name must be 'xml', not '" + res->name + "'!";
		return nullptr;
	}
	while (HasAttribute())
	{
		res->attributes.push_back(ParseAttribute());
		EOF_CHECK();
	}
	EOF_CHECK();
	if (*str != '?')
	{
		error = std::string("Unexpected token '") + *str + "' found! Expected '?'!";
		return nullptr;
	}
	SAFE_ADVANCE();
	JumpSpaces();
	EOF_CHECK();
	if (*str != '>')
	{
		error = std::string("Unexpected token '") + *str + "' found! Expected '>'!";
		return nullptr;
	}
	SAFE_ADVANCE();
	return res;
}

XMLNode* XMLFileLoader::ParseNode()
{
	XMLNode* res = new XMLNode();
	JumpSpaces();
	EOF_CHECK();
	if (*str != '<')
	{
		error = std::string("Unexpected token '") + *str + "' found! Expected '<'!";
		return nullptr;
	}
	SAFE_ADVANCE();
	const char* nameBegin = str;
	SkipToFirstSpaceOr('>', '/', '\\');
	EOF_CHECK();
	res->name = std::string(nameBegin, str);
	while (HasAttribute())
	{
		res->attributes.push_back(ParseAttribute());
		EOF_CHECK();
	}
	EOF_CHECK();
	if (*str == '/' || *str == '\\')
	{
		SAFE_ADVANCE();
		JumpSpaces();
		EOF_CHECK();
		if (*str != '>')
		{
			error = std::string("Unexpected token '") + *str + "' found! Expected '>'!";
			return nullptr;
		}
		SAFE_ADVANCE();
		return res;
	}
	if (*str != '>')
	{
		error = std::string("Unexpected token '") + *str + "' found! Expected '>'!";
		return nullptr;
	}
	SAFE_ADVANCE();
	JumpSpaces();
	EOF_CHECK();
	if (HasNode())
	{
		while (HasNode())
		{
			XMLNode* node = ParseNode();
			if (node == nullptr)
				return nullptr;
			res->children.push_back(node);
			JumpSpaces();
			EOF_CHECK();
		}
	}
	else // has content
	{
		res->content = ParseContent();
		EOF_CHECK();
		if (error != "")
			return nullptr;
	}
	if (*str != '<')
	{
		error = std::string("Unexpected token '") + *str + "' found! Expected '<'!";
		return nullptr;
	}
	SAFE_ADVANCE();
	JumpSpaces();
	EOF_CHECK();
	if (*str != '/' && *str != '\\')
	{
		error = std::string("Unexpected token '") + *str + "' found! Expected '/'!";
		return nullptr;
	}
	SAFE_ADVANCE();
	JumpSpaces();
	EOF_CHECK();
	const char* endNameBeg = str;
	SkipToFirstSpaceOr('>');
	EOF_CHECK();
	std::string endname = std::string(endNameBeg, str);
	if (endname != res->name)
	{
		error = "The closing tag ('" + endname + "') does not match the starting tag ('" + res->name + "')!";
		return nullptr;
	}
	JumpSpaces();
	EOF_CHECK();
	if (*str != '>')
	{
		error = std::string("Unexpected token '") + *str + "' found! Expected '>'!";
		return nullptr;
	}
	str++;
	return res;
}

bool XMLFileLoader::HasAttribute()
{
	JumpSpaces();
	return isalpha(*str) || *str == '_';
}

XMLAttribute XMLFileLoader::ParseAttribute()
{
	XMLAttribute res;
	const char* attNameBeg = str;
	SkipToFirst('=');
	EOF_CHECK_ATTRIBUTE();
	res.name = std::string(attNameBeg, str);
	SAFE_ADVANCE_ATTRIBUTE();
	char quote = *str;
	if (quote != '\'' && quote != '\"')
	{
		error = "Attributes must be surrounded by quotes ot apostrophies!";
		return res;
	}
	SAFE_ADVANCE_ATTRIBUTE();
	const char* attValueBeg = str;
	SkipToFirst(quote);
	EOF_CHECK_ATTRIBUTE();
	res.value = std::string(attValueBeg, str);
	SAFE_ADVANCE_ATTRIBUTE();
	return res;
}

bool XMLFileLoader::HasContent()
{
	return *str != '<';
}

std::string XMLFileLoader::ParseContent()
{
	std::stringstream ss;
	while (*str != '<' && *str != 0)
	{
		if (*str == '&')
		{
			if (strncmp(str, "&lt;", 4) == 0) ss << '<', str += 4;
			else if (strncmp(str, "&gt;", 4) == 0) ss << '>', str += 4;
			else if (strncmp(str, "&amp;", 5) == 0) ss << '&', str += 5;
			else if (strncmp(str, "&apos;", 6) == 0) ss << '\'', str += 6;
			else if (strncmp(str, "&quot;", 6) == 0) ss << '\"', str += 6;
			else
			{
				error = "Unexpected token found! Did you forget the ';' at the end of the entity reference?";
				return "";
			}
		}
		else ss << *str, str++;
	}
	return ss.str();
}

void XMLFileLoader::JumpSpaces()
{
	while (*str == ' ' || *str == '\n' || *str == '/r' || *str == '\t')
		str++;
}

std::string XMLFileLoader::RemoveComments(const std::string& str)
{
	std::string res;
	size_t last = 0;
	size_t beg;
	while ((beg = str.find("<!--", last)) != std::string::npos)
	{
		size_t end = str.find("-->", beg + 4);
		res += str.substr(last, beg - last);
		if (end == std::string::npos)
			return res;
		last = end + 3;
	}
	res += str.substr(last);
	return res;
}

std::string XMLFileWriter::WriteToMemory(XMLFile* xml, XMLOutputOptions options)
{
	ss.clear();
	XMLFileWriter::options = options;
	if (xml->prolog != nullptr)
		WriteProlog(xml->prolog);
	WriteNode(xml->root, 0);
	return ss.str();
}

void XMLFileWriter::WriteToFile(XMLFile* xml, const std::string& path, XMLOutputOptions options)
{
	Files::WriteTextFile(path, WriteToMemory(xml, options));
}

void XMLFileWriter::WriteProlog(XMLNode* node)
{
	ss << "<?" << node->name;
	for (const XMLAttribute& at : node->attributes)
	{
		ss << ' ' << at.name << "=\"";
		WriteFormatted(at.value);
		ss << '\"';
	}
	ss << "?>";
	if (options.indent)
		ss << '\n';
}

void XMLFileWriter::WriteNode(XMLNode* node, uint depth)
{
	if(options.indent)
		WriteIndent(depth);
	ss << '<' << node->name;
	for (const XMLAttribute& at : node->attributes)
	{
		ss << ' ' << at.name << "=\"";
		WriteFormatted(at.value);
		ss << '\"';
	}
	if (node->children.size() == 0 && node->content == "")
		ss << "/>";
	else
	{
		ss << '>';
		if (node->children.size() != 0)
		{
			for (XMLNode* c : node->children)
			{
				if (options.indent)
					ss << '\n';
				WriteNode(c, depth + 1);
			}
			if (options.indent)
			{
				ss << '\n';
				WriteIndent(depth);
			}
			ss << "</" << node->name << '>';
		}
		else
		{
			WriteFormatted(node->content);
			ss << "</" << node->name << '>';
		}
	}
}

void XMLFileWriter::WriteIndent(uint depth)
{
	for (uint i = 0; i < depth * options.spacesToIndent; i++)
		ss << ' ';
}

void XMLFileWriter::WriteFormatted(const std::string& text)
{
	for (char c : text)
	{
		switch (c)
		{
		case '<':
			ss << "&lt;"; break;
		case '>':
			ss << "&gt;"; break;
		case '&':
			ss << "&amp;"; break;
		case '\'':
			ss << "&apos;"; break;
		case '\"':
			ss << "&quot;"; break;
		default:
			ss << c; break;
		}
	}
}
