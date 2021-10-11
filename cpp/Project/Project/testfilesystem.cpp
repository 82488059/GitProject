//#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <iostream>
#include <filesystem>


// vs下选择c++语言标准选择 ISO C++17 标准 (/std:c++17)
// 使用此库可能要求额外的编译器/链接器选项。 
// 9.1 前的 GNU 实现要求用 -lstdc++fs 链接，而 LLVM 9.0 前的 LLVM 实现要求用 -lc++fs 链接。


int testfilesystem()
{
    namespace fs = std::filesystem;


    auto testdir = fs::path("f:/testdir");
   
    if (!fs::exists(testdir))
    {
        std::cout << "file or directory is not exists!" << std::endl;
    }

    // none	（默认）跳过符号链接，权限拒绝是错误。
    // follow_directory_symlink	跟随而非跳过符号链接。
    // skip_permission_denied	跳过若不跳过就会产生权限拒绝错误的目录。
    fs::directory_options opt(fs::directory_options::none);

    fs::directory_entry dir(testdir);
    // 遍历当前目录
    std::cout << "show:\t" << dir.path().filename() << std::endl;
    for (fs::directory_entry const& entry : fs::directory_iterator(testdir, opt)) 
    {
        entry.status();
        if (entry.is_regular_file()) 
        {
            std::cout << entry.path().filename()
                << "\t size: " <<  entry.file_size()  << std::endl;
        }
        else if (entry.is_directory())
        {
            std::cout << entry.path().filename()
                << "\t dir" << std::endl;
        }
    }
    std::cout << std::endl;
    std::cout << std::endl;

    // 递归遍历所有的文件
    std::cout << "show all:\t" << dir.path().filename() << std::endl;
    for (fs::directory_entry const& entry : fs::recursive_directory_iterator(testdir, opt))
    {
        if (entry.is_regular_file())
        {
            
            std::cout << entry.path().filename()
                << "\t size: " << entry.file_size() << "\t parent: " << entry.path().parent_path() << std::endl;
        }
        else if (entry.is_directory())
        {
            std::cout << entry.path().filename()
                << "\t dir" << std::endl;
        }
    }
    return 0;
}