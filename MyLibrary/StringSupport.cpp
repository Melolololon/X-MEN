#include "StringSupport.h"

std::string MelLib::StringSupport::ExtractionFileName(const std::string& path)
{
    std::string fileName;
    for (size_t i = path.size() - 1; i != -1; i--)
    {
        if (path[i] == '/' || path[i] == '\\')break;
        fileName += path[i];
    }

    std::reverse(fileName.begin(), fileName.end());
    return fileName;
}
