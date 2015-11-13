#pragma once
class CLoadXML
{
public:
    CLoadXML(const CString& szFileName);
    ~CLoadXML();

    bool init();

    void release();

    bool get_attr_string(const CComBSTR& nodeName, const CComBSTR& attrName, CComBSTR& outValue);
    bool get_node_text(const CComBSTR& nodeName, CComBSTR& outValue);
    bool set_node_text(const CComBSTR& nodeName, const CComBSTR& inValue);

    CString GetXMLString(const CString& path);
private:
    CString m_szFileName;
    CComPtr<IXMLDOMDocument> m_xmlDocument;
};

