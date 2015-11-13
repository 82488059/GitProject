#pragma once
class CLoadXML
{
public:
    CLoadXML(const CString& szFileName);
    ~CLoadXML();

    bool init();

    void release();

    bool get_attr_string(const CComBSTR& nodeName, const CComBSTR& attrName, CComBSTR& outValue);

    CString GetXMLString(const CString& path);
private:
    CString m_szFileName;
    CComPtr<IXMLDOMDocument> m_xmlDocument;
};

