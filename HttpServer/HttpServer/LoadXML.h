#pragma once
class CLoadXML
{
public:
    CLoadXML(const CString& szFileName);
    ~CLoadXML();

    bool init();

    void release();

	bool get_node(const CComBSTR& nodeName, IXMLDOMNodePtr& outNode);
	bool add_node(const CComBSTR& parentNodeName, IXMLDOMNodePtr& inNode);

    bool get_attr_string(const CComBSTR& nodeName, const CComBSTR& attrName, CComBSTR& outValue);
    bool get_node_text(const CComBSTR& nodeName, CComBSTR& outValue);
    bool set_node_text(const CComBSTR& nodeName, const CComBSTR& inValue);

	bool add_node_text(const CComBSTR& parentNodeName, const CComBSTR& nodeName, const CComBSTR& text);
    CString GetXMLString(const CString& path);
private:
    CString m_szFileName;
    CComPtr<IXMLDOMDocument> m_xmlDocument;
};

