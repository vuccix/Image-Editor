#include <View/Utils.h>

namespace Utils {

void openURL(const std::string& url) {
#ifdef _WIN32
    const std::string command = "start \"\" \"" + url + "\"";
#elif __APPLE__
    const std::string command = "open \"" + url + "\"";
#else
    const std::string command = "xdg-open \"" + url + "\"";
#endif

    std::system(command.c_str());
}

}
