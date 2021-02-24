#pragma once
#import "msxml3.dll"

class CLoadXML
{
public:
    CLoadXML(const CString& szFileName);
    CLoadXML();
    ~CLoadXML();

    bool init();
    bool save();
    bool save_as(const CString& path);
    void release();

    bool get_node(const CString& nodeName, IXMLDOMNodePtr& outNode){
        return get_node(CComBSTR(nodeName), outNode);
    }
	bool add_node(const CString& parentNodeName, IXMLDOMNodePtr& inNode){
		return add_node(CComBSTR(parentNodeName), inNode);
	}
	bool get_node_text(const CString& nodeName, CString& outValue){
        CComBSTR value;
        bool r = get_node_text(CComBSTR(nodeName), value);
        outValue = value;
        return r;
	}
	bool set_node_text(const CString& nodeName, const CString& inValue){
		return set_node_text(CComBSTR(nodeName), CComBSTR(inValue));
	}
public:
	bool get_node(const CComBSTR& nodeName, IXMLDOMNodePtr& outNode);
	bool add_node(const CComBSTR& parentNodeName, IXMLDOMNodePtr& inNode);

    bool get_attr_string(const CComBSTR& nodeName, const CComBSTR& attrName, CComBSTR& outValue);
    bool get_node_text(const CComBSTR& nodeName, CComBSTR& outValue);
    bool set_node_text(const CComBSTR& nodeName, const CComBSTR& inValue);

	bool add_node_text(const CComBSTR& parentNodeName, const CComBSTR& nodeName, const CComBSTR& text);
    
    void SetPath(const CString& path){ m_szFileName = path; }
    CString GetXMLString(const CString& path);

private:
    CString m_szFileName;
    CComPtr<IXMLDOMDocument> m_xmlDocument;
};

