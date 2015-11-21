#include "stdafx.h"

// #include<msxml2.h>
#include<comutil.h>
#import "msxml3.dll"

#include "LoadXML.h"

#pragma   comment(lib, "comsupp.lib ")
// #pragma comment(lib,"msxml2.lib")

CLoadXML::CLoadXML(const CString& szFileName)
:m_szFileName(szFileName)
{
}
CLoadXML::CLoadXML()
: m_szFileName()
{
}

CLoadXML::~CLoadXML()
{
    release();
}
bool CLoadXML::init()
{
    if (FAILED(CoInitialize(NULL)))
    {
        return false;
    }
    if (NULL != m_xmlDocument.p)
        m_xmlDocument.Release();

    //Create Document
    HRESULT hr = m_xmlDocument.CoCreateInstance(__uuidof(DOMDocument));

    if (FAILED(hr))
        return false;

    if (NULL == m_xmlDocument.p)
        return false;

    //Load XMLFile
    VARIANT_BOOL bSuccess;

    //CString xmlPath = CString(CPublicFun::GetWorkPath()) + _T("\\Config\\Global.xml");

    CComBSTR bstr(m_szFileName);

    hr = m_xmlDocument->load(CComVariant(bstr), &bSuccess);

    if (FAILED(hr))
        return false;

    if (!bSuccess)
        return false;

    return true;
}

void CLoadXML::release()
{
    CoUninitialize();
    if (NULL != m_xmlDocument.p)
    {
        m_xmlDocument.Release();
    }
}

CString CLoadXML::GetXMLString(const CString& path)
{
    if (NULL != m_xmlDocument.p)
        return CString();
    
    CComBSTR bstr;
    get_attr_string(CComBSTR(path), CComBSTR(_T("value")), bstr);
    return CString(bstr);
}

// 获取节点属性值
bool CLoadXML::get_attr_string(const CComBSTR& nodeName, const CComBSTR& attrName, CComBSTR& outValue)
{
    if (NULL == this->m_xmlDocument.p)
        return false;

    CComPtr<IXMLDOMNode> node;
    do 
    {
        HRESULT hr;
        CComPtr<IXMLDOMNode> attr;
        CComPtr<IXMLDOMNamedNodeMap> attrs;
        VARIANT value;

        hr = m_xmlDocument->selectSingleNode(nodeName, &node);
        if (FAILED(hr) || NULL == node.p)
            break;

        hr = node->get_attributes(&attrs);
        if (FAILED(hr) || NULL == attrs.p)
            break;

        hr = attrs->getNamedItem(attrName, &attr);
        if (FAILED(hr) || NULL == attr.p)
        {
            attrs.Release();
            break;
        }
        
        hr = attr->get_nodeValue(&value);
        if (FAILED(hr))
        {
            attr.Release();
            break;
        }

        node.Release();
        outValue = value.bstrVal;
        attrs.Release();
        attr.Release();
        VariantClear(&value);
        return true;

    } while (0);

    node.Release();
    return false;
}


// 获取节点text值
bool CLoadXML::get_node_text(const CComBSTR& nodeName, CComBSTR& outValue)
{
    if (NULL == this->m_xmlDocument.p)
    {
        TRACE("%s ERROR!", m_szFileName);
        return false;
    }

    CComPtr<IXMLDOMNode> node;
    do 
    {
        HRESULT hr;

        hr = m_xmlDocument->selectSingleNode(nodeName, &node);
        if (FAILED(hr) || NULL == node.p)
        {
            TRACE("%s not find!", CString(nodeName));
            break;
        }
        
        hr = node->get_text(&outValue);

        if (FAILED(hr))
        {
            TRACE("%s text null!", CString(nodeName));
            break;
        }

        node.Release();
        return true;

    } while (0);

    node.Release();
    return false;
}


// set节点text值
bool CLoadXML::set_node_text(const CComBSTR& nodeName, const CComBSTR& inValue)
{
    if (NULL == this->m_xmlDocument.p)
        return false;

    CComPtr<IXMLDOMNode> node;
    do 
    {
        HRESULT hr;

        hr = m_xmlDocument->selectSingleNode(nodeName, &node);
        if (FAILED(hr) || NULL == node.p)
            break;
        
        hr = node->put_text(inValue);
        
        if (FAILED(hr))
            break;
//         COleVariant V = m_szFileName;
//         VARIANT var = V;
//         m_xmlDocument->save(var);
        node.Release();
        return true;

    } while (0);

    node.Release();
    return false;
}

bool CLoadXML::add_node_text(const CComBSTR& parentNodeName, const CComBSTR& nodeName, const CComBSTR& text)
{
	if (NULL == this->m_xmlDocument.p)
		return false;

	CComPtr<IXMLDOMNode> node;
	do
	{
		HRESULT hr = NULL;
		hr = m_xmlDocument->selectSingleNode(parentNodeName, &node);
		if (FAILED(hr) || NULL == node.p)
			break;
		IXMLDOMElementPtr newNode = NULL;
		hr = m_xmlDocument->createElement(nodeName, &newNode);
		if (FAILED(hr))
			break;
		IXMLDOMTextPtr newText = NULL;
		hr = m_xmlDocument->createTextNode(text, &newText);
		if (FAILED(hr))
			break;
		newNode->put_text(text);
		if (FAILED(hr))
			break;
		IXMLDOMNodePtr lastChild = NULL;
		hr = node->appendChild(newNode, &lastChild);
		if (FAILED(hr))
			break;
		IXMLDOMTextPtr newLineText = NULL;
		static CComBSTR newLine("\n");
		hr = m_xmlDocument->createTextNode(newLine, &newLineText);
		if (FAILED(hr))
			break;
		hr = node->appendChild(newLineText, &lastChild);
		if (FAILED(hr))
			break;
// 		COleVariant V = m_szFileName;
// 		VARIANT var = V;
// 		m_xmlDocument->save(var);
		node.Release();
		return true;

	} while (0);
	node.Release();
	return false;
}

// get节点
bool CLoadXML::get_node(const CComBSTR& nodeName, IXMLDOMNodePtr& outNode)
{
	if (NULL == this->m_xmlDocument.p)
		return false;
	do
	{
		HRESULT hr;
		hr = m_xmlDocument->selectSingleNode(nodeName, &outNode);
		if (FAILED(hr) || NULL == outNode)
			break;
		
		return true;

	} while (0);
	return false;
}
bool CLoadXML::add_node(const CComBSTR& parentNodeName, IXMLDOMNodePtr& inNode)
{
	if (NULL == this->m_xmlDocument.p)
		return false;

	CComPtr<IXMLDOMNode> node;
	do
	{
		HRESULT hr = NULL;
		hr = m_xmlDocument->selectSingleNode(parentNodeName, &node);
		if (FAILED(hr) || NULL == node.p)
		{
			TRACE("selectSingleNode error %s\n", parentNodeName);
			break;
		}
		IXMLDOMNodePtr newNode;
		hr = inNode->cloneNode(VARIANT_TRUE, &newNode);
		if (FAILED(hr))
		{
			TRACE("cloneNode error \n");
			break;
		}
		IXMLDOMNodePtr lastChild = NULL;
		hr = node->appendChild(newNode, &lastChild);
		if (FAILED(hr))
		{
			TRACE("appendChild error %s\n", parentNodeName);
			break;
		}
		IXMLDOMTextPtr newLineText = NULL;
		static CComBSTR newLine("\n");
		hr = m_xmlDocument->createTextNode(newLine, &newLineText);
		if (FAILED(hr))
			break;
		hr = node->appendChild(newLineText, &lastChild);
		if (FAILED(hr))
			break;
// 		COleVariant V = m_szFileName;
// 		VARIANT var = V;
// 		hr = m_xmlDocument->save(var);
		node.Release();
		return true;

	} while (0);
	node.Release();
	return false;
}
bool CLoadXML::save()
{
    COleVariant V = m_szFileName;
    VARIANT var = V;
    HRESULT hr = m_xmlDocument->save(var);
    if (FAILED(hr))
        return false;
    return true;
}
bool CLoadXML::save_as(const CString& path)
{
    COleVariant V = m_szFileName;
    VARIANT var = V;
    HRESULT hr = m_xmlDocument->save(var);
    if (FAILED(hr))
        return false;
    return true;
}