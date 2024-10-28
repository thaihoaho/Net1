#include <stdio.h>
#include <string.h>

int main() {
    // Dữ liệu đầu vào
    char *buffer = "11111111110000000000a.txt 200 12 23";

    // Thông tin HASHINFO
    char hashinfo[11]; // 10 ký tự + 1 cho null terminator
    strncpy(hashinfo, buffer + 10, 10);
    hashinfo[10] = '\0'; // Đảm bảo chuỗi kết thúc đúng cách

    printf("Receive publish request with hashinfo: %s\n", hashinfo);

    // Các biến để lưu thông tin còn lại
    char name[100];       // Dự trữ cho tên file
    int filesize, piececount, piecesize;

    // Sử dụng sscanf để trích xuất thông tin
    if (sscanf(buffer + 20, "%99s %d %d %d", name, &filesize, &piececount, &piecesize) == 4) {
        // In ra thông tin đã lấy
        printf("Name: %s\n", name);
        printf("Filesize: %d\n", filesize);
        printf("Piececount: %d\n", piececount);
        printf("Piecesize: %d\n", piecesize);
    } else {
        printf("Failed to parse remaining information.\n");
    }

    return 0;
}