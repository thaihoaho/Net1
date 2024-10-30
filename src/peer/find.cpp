#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

int main() {
    std::string path = "./files"; // Replace with your folder path
    std::string filename = "video.mp4";
    bool x=1;
    try {
        for (const auto &entry : fs::directory_iterator(path)) {
            if (x == 1 && entry.path().filename().string() == filename){
                    std::cout << (long)(fs::file_size(entry));
                }
            if (x == 0)
                std::cout << "File: " << entry.path().filename() << " - Size: "
                          << fs::file_size(entry) << " bytes" << std::endl;
        }
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }

    return 0;
}
