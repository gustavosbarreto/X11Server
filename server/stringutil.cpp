#include <string>

namespace XUtility
{
    using namespace std;

    string trim_left(const string & str)
    {
        string sret;
        size_t i = str.find_first_not_of(" \t");
        if(i == string::npos) return sret;
        return str.substr(i, string::npos);
    }

    string trim_right(const string & str)
    {
        string sret;
        size_t i = str.find_last_not_of(" \t");
        if(i == string::npos) return sret;
        sret = str.substr(0, i+1);
        return sret;
    }

    string trim(const string & str)
    {
        return trim_left(trim_right(str));
    }
}
