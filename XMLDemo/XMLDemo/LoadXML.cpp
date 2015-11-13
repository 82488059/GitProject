#include "stdafx.h"
#include "LoadXML.h"

#include<msxml2.h>
#include<comutil.h>

#pragma   comment(lib, "comsupp.lib ")
#pragma comment(lib,"msxml2.lib")

CLoadXML::CLoadXML(const CString& szFileName)
:m_szFileName(szFileName)
{
}


CLoadXML::~CLoadXML()
{
    release();
}
bool CLoadXML::init()
{
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
    if (NULL != m_xmlDocument.p)
        m_xmlDocument.Release();
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
        // old
//         if (FAILED(hr))
//         {
//             attrs.Release();
//             break;
//         }
//         if (NULL == attr.p)
//             break;
        
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
        return false;

    CComPtr<IXMLDOMNode> node;
    do 
    {
        HRESULT hr;

        hr = m_xmlDocument->selectSingleNode(nodeName, &node);
        if (FAILED(hr) || NULL == node.p)
            break;
        
        hr = node->get_text(&outValue);

        if (FAILED(hr))
            break;

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
        COleVariant V = m_szFileName;
        VARIANT var = V;
        m_xmlDocument->save(var);
        node.Release();
        return true;

    } while (0);

    node.Release();
    return false;
}

