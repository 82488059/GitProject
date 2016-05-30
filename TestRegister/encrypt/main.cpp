#include "sha/sha1.h"
#include "md5/md5.h"
#include "iostream"


int main()
{
    using namespace std;

    string  str;
    cin >> str;
    MD5 md5(str);

    cout << "md5:" << md5.toString() << endl;
    char buf[41];
    SHA1 sha1;
    sha1.SHA_GO(str.c_str (), buf);
    cout << "sha1:" << buf << endl;



    return 0;
}