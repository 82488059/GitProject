#include <xstring>
#include <tchar.h>
namespace ms{

#ifdef _UNICODE
typedef std::wstring string;
#else
typedef std::string string;
#endif
}
