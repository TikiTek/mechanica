/*
* dbalster's XML DOM parser
*
* Copyright (c) Daniel Balster
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Daniel Balster nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY DANIEL BALSTER ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL DANIEL BALSTER BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef DBALSTER_XML_H
#include "xml.h"
#endif

#include <string.h>             // strstr, strchr

// internal data representation

typedef struct _XmlNamedElement XmlNamedElement;
typedef struct _XmlNamedAttribute XmlNamedAttribute;
typedef struct _XmlScannerContext XmlScannerContext;
typedef struct _XmlContext XmlContext;

struct _XmlNamedElement
{
	const char*   name;
	XmlElement*   element;
};

struct _XmlNamedAttribute
{
	const char*   name;
	XmlAttribute* attribute;
};

struct _XmlContext
{
};

struct _XmlScannerContext
{
	XmlElement*         pRoot;
	XmlErrorHandler     errorHandler;
	XmlSizeofHint*      sizeofHints;
	const char*         begin;
	const char*         end;
	unsigned int        nChars;           // byte-aligned
	unsigned int        nBytes;           // struct-aligned
	unsigned int        nUsedChars;
	unsigned int        nUsedBytes;
};

// private methods.

// alloc memory, if _string is true the string pool is used
static void* xml_alloc_memory( XmlScannerContext* _ctx, const unsigned int _bytes, bool _string /*= false*/ );
// duplicate string (automatically adds a null byte)
static char* xml_clone_string( XmlScannerContext* _ctx, const char* _str, const unsigned int _size, const bool _escape /*= true*/ );
// build the XML document tree in two passes (_scanonly = false,true)
static const char* xml_document_scan( XmlScannerContext* _ctx, XmlElement* _element, const char* _begin, const char* _end, bool _scanonly );
// link attribute to attributes list
static void xml_element_add_attribute( XmlElement* _elem, XmlAttribute* _attr );
// link element to elements list
static void xml_element_add_element( XmlElement* _elem, XmlElement* _child );

static bool xml_namespace_compare(const char* _name, const char* _value);

static size_t xml_sizeof( XmlSizeofHint* _sizeofHints, const char* _element, size_t _elementSize, const char* _attribute, size_t _attributeSize )
{
	XmlSizeofHint* iter = _sizeofHints;
	while (iter && (iter->element || iter->attribute || iter->size)) // three zeros terminate the list
	{
		if (iter->element==0 || xml_compare(_element,iter->element))
		{
			return iter->size;
		}
		iter++;
	}
	// again, but with relaxed namespaces
	iter = _sizeofHints;
	while (iter && (iter->element || iter->attribute || iter->size)) // three zeros terminate the list
	{
		if (iter->element==0 || xml_namespace_compare(_element,iter->element))
		{
			return iter->size;
		}
		iter++;
	}
	return 0;
}

// scan for the next character not in [ \t\n\r]* in the range [_begin,_end]
static const char* scan_whitespace( const char* _begin, const char* _end )
{
	char ch;
	while( 0!=(ch = *_begin++) )
	{
		if ((_begin <= _end) && (' '==ch || '\t'==ch || '\n'==ch || '\r'==ch)) continue;
		break;
	}
	return _begin-1;
}

// scan for the first char not in [a-zA-Z0-9\.\:_\-\/]+ in the range [_begin,_end]
// any prefixing whitespace is ignored.
static const char* scan_identifier( const char* _begin, const char* _end )
{
	char ch;
	_begin = scan_whitespace(_begin,_end);
	while( 0 !=(ch = *_begin++) )
	{
		if (_begin > _end) break;
		if ((ch>='a' && ch<='z') || (ch>='A' && ch<='Z')
			|| (ch>='0' && ch<='9') || (ch=='.')      || (ch==':')
			|| (ch=='_') || (ch=='-') || (ch=='/')) continue;
		else break;
	}
	return _begin-1;
}

// string xml_compare. I could have used strcmp or similar, but I want to extend this library to support
// different encodings and esXML_C_APIngs (i.e. quoted html entities and plain entities)
XML_C_API bool xml_compare( const char* _str, const char* _text )
{
	int i=0;
	if (0==_str) return false;
	while (_text[i])
	{
		if (_str[i]!=_text[i]) return false;
		++i;
	}
	return true;
}

XML_C_API bool xml_compare2( const char* _str, const char* _text )
{
	int i=0;
	if (0==_str) return false;
	while (_text[i])
	{
		if (_str[i]!=_text[i]) return false;
		++i;
	}
	return _str[i]=='\0';
}

static bool xml_namespace_compare(const char* _name, const char* _value)
{
	const char* name = strchr(_name,':');
	const char* value = strchr(_value,':');
	if (name) name++; else name=_name;
	if (value) value++; else value=_value;
	return xml_compare2(name,value);
}

XML_C_API bool xml_element_name( XmlElement* _elem, const char* _value )
{
	return (_elem && _elem->name && xml_namespace_compare(_elem->name,_value));
}

XML_C_API bool xml_attribute_name( XmlAttribute* _attr, const char* _value )
{
	return (_attr && _attr->name && xml_namespace_compare(_attr->name,_value));
}

// enqueue the attribute entry at the end of the attributes list
static void xml_element_add_attribute( XmlElement* self, XmlAttribute* attribute )
{
	if (0==self->attributes)
	{
		self->attributes = attribute;
	}
	else
	{
		XmlAttribute* iter = self->attributes;
		while (iter->next) iter = iter->next;
		iter->next = attribute;
	}
}

// enqueue the element at the end of the elements list
static void xml_element_add_element( XmlElement* self, XmlElement* child )
{
	child->parent = self;
	if (0==self->elements)
	{
		self->elements = child;
	}
	else
	{
		XmlElement* iter = self->elements;
		if (self->tail)
		{
			self->tail->next = child;
		}
		else
		{
			while (iter->next) iter = iter->next;
			iter->next = child;
		}
	}
	self->tail = child;
}

XML_C_API XmlElement* xml_element_find_element_by_attribute_value( XmlElement* self, const char* _elemName, const char* _attrName, const char* _attrValue )
{
	if (self->name && xml_element_name(self,_elemName))
	{
		XmlAttribute* iter = self->attributes;
		while (iter)
		{
			if (iter->name && xml_attribute_name(iter,_attrName))
			{
				if (iter->content && xml_compare(iter->content,_attrValue))
				{
					return self;
				}
			}
			iter = iter->next;
		}
	}

	XmlElement* iter = self->elements;
	while (iter)
	{
		XmlElement* e = xml_element_find_element_by_attribute_value(iter,_elemName,_attrName,_attrValue);
		if (e) return e;
		iter = iter->next;
	}

	return 0;
}

XML_C_API XmlAttribute* xml_element_find_attribute_by_name( XmlElement* self, const char* _elemName, const char* _attrName )
{
	if (self->name && xml_element_name(self,_elemName))
	{
		XmlAttribute* iter = self->attributes;
		while (iter)
		{
			if (iter->name && xml_attribute_name(iter,_attrName))
			{
				return iter;
			}
			iter = iter->next;
		}
	}

	XmlElement* iter = self->elements;
	while (iter)
	{
		XmlAttribute* a = xml_element_find_attribute_by_name(iter,_elemName,_attrName);
		if (a) return a;
		iter = iter->next;
	}

	return 0;
}

XML_C_API XmlElement* xml_element_find_element( XmlElement* self, const char* _name, XmlElement* _element )
{
	XmlElement* iter = _element ? _element->next : self->elements;
	while (iter)
	{
		if (iter->name && xml_element_name(iter,_name)) return iter;
		iter = iter->next;
	}
	return 0;
}

// iterator helper
XML_C_API void xml_element_foreach( XmlElement* _elem, XmlForEachFunc _func, void* _param )
{
	if (_func) _func(_elem,_param);

	XmlElement* iter = _elem->elements;

	while (iter)
	{
		xml_element_foreach(iter, _func, _param);
		iter = iter->next;
	}
}

XML_C_API XmlElement* xml_element_find_any( XmlElement* _elem, const char* _name )
{
	if (xml_element_name(_elem,_name)) return _elem;

	XmlElement* iter = _elem->elements;
	while (iter)
	{
		XmlElement* e = xml_element_find_any(iter, _name);
		if (e) return e;
		iter = iter->next;
	}
	return 0;
}

XML_C_API unsigned int xml_element_find_elements( XmlElement* self, const char* _name, XmlElement* _begin[], XmlElement* _end[] )
{
	unsigned int count = 0;

	if (_name==0 || xml_element_name(self,_name))
	{
		if (_begin < _end)
		{
			*_begin = self;
		}
		count++;
	}

	XmlElement* iter = self->elements;

	while (iter)
	{
		count += xml_element_find_elements(iter,_name,_begin ? _begin+count : 0,_end);
		iter = iter->next;
	}

	return count;
}

XML_C_API unsigned int xml_element_find_elements_by_attribute( XmlElement* self, const char* _name, XmlElement* _begin[] /*= 0*/, XmlElement* _end[] /*= 0*/ )
{
	unsigned int count = 0;

	XmlAttribute* attr = self->attributes;
	while (attr)
	{
		if (xml_attribute_name(attr,_name))
		{
			if (_begin < _end)
			{
				*_begin = self;
			}
			count++;
		}
		attr = attr->next;
	}

	XmlElement* iter = self->elements;

	while (iter)
	{
		count += xml_element_find_elements_by_attribute(iter,_name,_begin ? _begin+count : 0,_end);
		iter = iter->next;
	}

	return count;
}

XML_C_API unsigned int xml_element_find_attributes( XmlElement* self, const char* _name, XmlAttribute* _begin[], XmlAttribute* _end[] )
{
	unsigned int count = 0;
	XmlAttribute* iter = self->attributes;

	while (iter)
	{
		if (xml_attribute_name(iter,_name))
		{
			if (_begin < _end)
			{
				*_begin = iter;
				_begin++;
			}
			count++;
		}

		iter = iter->next;
	}

	return count;
}

XML_C_API XmlAttribute* xml_element_find_attribute( XmlElement* self, const char* _name, XmlAttribute* _attribute )
{
	if (self==0) return 0;
	XmlAttribute* iter = _attribute ? _attribute : self->attributes;
	while (iter)
	{
		if (xml_attribute_name(iter,_name)) return iter;
		iter = iter->next;
	}
	return 0;
}

XML_C_API XmlElement* xml_element_get_root(XmlElement* _e)
{
	while (_e && _e->parent) _e = _e->parent;
	return _e;
}

// concatenate all child elements that are content elements (name==0)
// *NOT* useful for SVG and HTML
XML_C_API unsigned int xml_element_get_content( XmlElement* self, char* _buffer, unsigned int _size )
{
	unsigned int size = 0;
	XmlElement* iter = self->elements;
	while (iter)
	{
		if (0==iter->name)
		{
			int i=0;
			while (iter->content[i])
			{
				if (_buffer && size<_size) _buffer[size] = iter->content[i];
				size++;
				i++;
			}
		}
		//    size += xml_element_get_content(iter,_buffer+size,_size);
		iter = iter->next;
	}
	return size;
}

// allocMem memory. two pools are used - one for strings and one for 4-byte aligned structs
static void* xml_alloc_memory( XmlScannerContext* _ctx, const unsigned int _bytes, bool _string )
{
	char* pointer = (char*) _ctx->pRoot;
	if (_string)
	{
		if ( (_ctx->nUsedChars + _bytes) > _ctx->nChars ) return 0;
		pointer += ( _ctx->nBytes + _ctx->nUsedChars );
		_ctx->nUsedChars += _bytes;
	}
	else
	{
		if ( (_ctx->nUsedBytes + _bytes) > _ctx->nBytes ) return 0;
		pointer += _ctx->nUsedBytes;
		_ctx->nUsedBytes += _bytes;
	}
	return pointer;
}

// create a zero-terminated string clone
static char* xml_clone_string( XmlScannerContext* _ctx, const char* _str, const unsigned int _size, const bool _escape )
{
	char* str = (char*) xml_alloc_memory(_ctx,_size+1,true);
	if (str)
	{
		unsigned int i=0, j=0;
		for (; i<_size;i++,j++)
		{
			if (_escape && _str[i]=='&')
			{
				if (xml_compare(_str+i,"&lt;")) { str[j]='<'; i+=3; }
				else if (xml_compare(_str+i,"&gt;")) { str[j]='>'; i+=3; }
				else if (xml_compare(_str+i,"&amp;")) { str[j]='&'; i+=4; }
				else if (xml_compare(_str+i,"&quot;")) { str[j]='\''; i+=5; }
				else
				{
					// skip unknown entity
					while (_str[i]!=';' && i<_size) i++;
				}
			}
			else str[j] = _str[i];
		}
		str[j]=0;
	}
	return str;
}

// scanning is performed in two passes, the first one (_scanonly=true) is used to estimate the memory usage,
// whereas the second pass (_scanonly=false) will construct the XML document tree.
// the complete document will be placed into one single memory block, this is cache friendly and does not
// fragment the memory manager.
static const char* xml_document_scan( XmlScannerContext* _ctx, XmlElement* _element, const char* _begin, const char* _end, bool _scanonly )
{
	const char* marker = 0;
	// TODO check if _begin<_end is correct (valgrind demanded this!)
	while( _begin && _begin<_end && *_begin )
	{
		if (_begin >= _end) return 0;
		char c = *_begin++;
		if ('<'==c)
		{
			if (marker)
			{
				int n = (int)(_begin-marker-1);
				if (_scanonly)
				{
					_ctx->nChars += n+1;
					_ctx->nBytes += sizeof(XmlElement);
				}
				else
				{
					XmlElement* text = (XmlElement*) xml_alloc_memory(_ctx,sizeof(XmlElement),false);
					text->content = xml_clone_string(_ctx,marker,n,true);
					text->name = 0;

					// convenience
					_element->content = text->content;

					xml_element_add_element( _element, text );
				}
				marker = 0;
			}
			bool recurse = true;
			bool allocate = false;
			XmlElement* element = 0;
			if ('!' == *_begin) // skip comments, cdata, dtds, doctypes. not supported
			{
				int nesting=1;
				if (xml_compare(_begin,"![CDATA["))
				{
					_begin+=8;    // skip '![CDATA['
					const char* end = strstr(_begin,"]]>");
					if (end)
					{
						int n = (int)(end - _begin);
						if (_scanonly)
						{
							_ctx->nChars += n+1;
							_ctx->nBytes += sizeof(XmlElement);
						}
						else
						{
							XmlElement* text = (XmlElement*) xml_alloc_memory(_ctx,sizeof(XmlElement),false);
							text->content = xml_clone_string(_ctx,_begin,n,false);
							text->name = 0;

							// convenience
							_element->content = text->content;

							xml_element_add_element( _element,text );
						}

						_begin = end+3; // "]]>"
					}
					else
					{
						if (_ctx->errorHandler) _ctx->errorHandler("unterminated CDATA",_ctx->begin,_begin);
						return 0;
					}
				}
				else if (xml_compare(_begin,"!--"))     // TODO: create comment element?
				{
					_begin = strstr(_begin,"-->");
					if (_begin) _begin+=3;
					else
					{
						if (_ctx->errorHandler) _ctx->errorHandler("unterminated comment",_ctx->begin,_begin);
						return 0;
					}
				}
				else do
				{
					if ('<' == *_begin) nesting++;
					if ('>' == *_begin) nesting--;
					_begin++;
				}
				while ( nesting>0 && _begin < _end );
				continue;
			}

			// processing instructions: not well supported
			if ('?' == *_begin)
			{
				_begin++;
				recurse = false;
			}

			const char* end = scan_identifier(_begin,_end);
			if ('/' != *_begin)       // this is not a terminating element
			{
				if (end[-1]=='/') --end;
				allocate = true;
				size_t elementSize = sizeof(XmlElement) + xml_sizeof(_ctx->sizeofHints,_begin,end-_begin,0,0);
				if (_scanonly)
				{
					_ctx->nChars += (unsigned int)(end-_begin) + 1;
					_ctx->nBytes += (unsigned int)elementSize;
				}
				else
				{
					element = (XmlElement*) xml_alloc_memory(_ctx,(unsigned int)elementSize,false);
					element->name = xml_clone_string(_ctx,_begin,(unsigned int)(end-_begin),true);
					element->content = 0;
					xml_element_add_element( _element,element );
				}
			}
			else // this is a terminating element (</name>)
			{
				_begin = scan_whitespace(end,_end);
				if ('>' != _begin[0] && '>' != _begin[1])
				{
					if (_ctx->errorHandler) _ctx->errorHandler("'>' expected",_ctx->begin,_begin);
					return 0;
				}
				return _begin;
			}
			// scan the element (and all attributes)
			_begin = scan_whitespace(end,_end);
			while ( '>' != *_begin )
			{
				_begin = scan_whitespace(_begin,_end);
				if ('?' == *_begin)     // ending of <?tag ... ?>
				{
					_begin++;
					if ('>' != *_begin)
					{
						if (_ctx->errorHandler) _ctx->errorHandler("'>' expected",_ctx->begin,_begin);
						return 0;
					}
					break;
				}
				if ('/' == *_begin)             // element has no content and is complete
				{
					_begin = scan_whitespace(_begin+1,_end);
					if ('>' != *_begin)
					{
						if (_ctx->errorHandler) _ctx->errorHandler("'>' expected",_ctx->begin,_begin);
						return 0;
					}
					recurse = false;
					break;
				}
				if (allocate) // scan all attributes
				{
					XmlAttribute* attribute = 0;
					end = scan_identifier(_begin,_end);
					if (_scanonly)
					{
						_ctx->nChars += (unsigned int)(end-_begin) + 1;
						_ctx->nBytes += sizeof(XmlAttribute);
					}
					else
					{            
						attribute = (XmlAttribute*) xml_alloc_memory(_ctx,sizeof(XmlAttribute),false);
						attribute->name = xml_clone_string(_ctx,_begin,(unsigned int)(end-_begin),true);
						attribute->content = "";
						xml_element_add_attribute( element, attribute );
					}
					_begin = scan_whitespace(end,_end);
					if ('=' == *_begin)   // attribute with assignment
					{
						_begin = scan_whitespace(_begin+1,_end);
						char quote = *_begin;
						if (quote!='"' && quote!='\'')
						{
							if (_ctx->errorHandler) _ctx->errorHandler("quoted string (\" or ') expected",_ctx->begin,_begin);
							return 0;
						}
						for (_begin++,end=_begin;*end!=quote && end<_end;++end);            // scan end of quoted string
						if (end-_begin>0)
						{
							if (_scanonly)
							{
								_ctx->nChars += (unsigned int)(end-_begin) + 1;
							}
							else
							{
								attribute->content = xml_clone_string(_ctx,_begin,(unsigned int)(end-_begin),true);
							}
						}
					}
					_begin = scan_whitespace(end+1,_end);
				}
				else
				{
					_begin++;
				}
			}
			if (allocate && recurse)
			{
				// so, tag ist offen und gescanned, dann rekursion
				_begin = xml_document_scan(_ctx,element,_begin+1,_end,_scanonly);
			}
			if (_begin) _begin++;     // skip '>'
		}
		else
		{
			if (0==marker) marker = _begin-1;
		}
	}
	return _begin;
}

XML_C_API XmlElement* xml_create( const char* _begin, const char* _end, XmlErrorHandler _errorHandler, XmlAllocator _allocate, XmlSizeofHint* _sizeofHints )
{
	if (_allocate==0) return 0;

	XmlScannerContext context = {0};

	context.errorHandler = _errorHandler;
	context.sizeofHints = _sizeofHints;
	context.begin = _begin;
	context.end = _end;

	// phase #1: estimate exact memory usage
	context.nChars = 0;
	context.nBytes = sizeof(XmlElement);          // pRoot element
	context.nUsedBytes = context.nBytes;                          // initial allocation
	const char* iter = xml_document_scan(&context,0,_begin,_end,true);
	if (iter != 0)
	{
		// phase #2: scan and construct document tree
		context.pRoot = (XmlElement*) _allocate( context.nChars + context.nBytes );
		memset(context.pRoot,0,context.nChars + context.nBytes);
		context.pRoot->name = "";
		context.pRoot->content = "";
		xml_document_scan(&context,context.pRoot,_begin,_end,false);
	}

	return context.pRoot;
}
