#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>

namespace output {
const std::string Red = "\033[0;31m";
const std::string Green = "\033[0;32m";
const std::string Blue = "\033[0;34m";
const std::string Yellow = "\033[0;33m";
const std::string Purple = "\033[0;35m";
const std::string Cyan = "\033[0;36m";
const std::string Default = "\033[0;39m";

inline void console(const std::vector<std::pair<const std::string, const std::string>> &outputVector) {
    size_t index(0);
    for (auto &[header, text] : outputVector) {
        std::cout << (index == 0 ? (header == "EXCEPTION" || header == "STOPPED" || header == "DISCONNECTED" || header == "ERROR"
                                        ? output::Red
                                        : (header == "MESSAGE" || header == "QUERY"
                                               ? output::Purple
                                               : (header == "WARNING" ? output::Cyan : output::Green)))
                                 : output::Yellow)
                  << "[" << header << "]: " << output::Default << text << (index++ + 1 == outputVector.size() ? "" : "\n\t");
    }
    std::cout << std::endl;
}
}  // namespace output

#define EXCEPTION(message)                                                                            \
    {                                                                                                 \
        output::console({{"EXCEPTION", ""}, {"FILE", __FILE__}, {"LINE", std::to_string(__LINE__)}}); \
        throw std::runtime_error(message);                                                            \
    }

#endif  // UTILITIES_HPP