Mã hóa AES-128 chế độ EBC - Padding: None

file chính: 
aes.h
aes.c
aes.cpp 

Sử dụng file .cpp cho lập trình ứng dụng bằng Arduino IDE

Folder:
Gateway : code cổng IoT bằng Arduino IDE
Node : code thiết bị IoT bằng Arduino IDE

Chạy chương trình trên máy tính:
Bash:
> gcc -o main main.c aes.c
>./main

Thêm mã hóa AES-128 vào ứng dụng:
Thêm 2 file aes.h và aes.c (hoặc aes.cpp) vào project

Khai báo thư viện:
 
> #include "aes.h"

Sử dụng hàm:

Hàm mã hóa:

> void handleDataEncrypt(const char *input_str, char *data_sent, uint8_t *key);

input_str : chuỗi ký tự muốn mã hóa
data_sent: chuỗi trống để ghi kết quả mã hóa
key: khóa bí mật

Hàm giải mã:

> void handleDataDecrypt(const char* data_encrypt, char* data_decrypt, uint8_t* key);

input_str : chuỗi ký tự đã mã hóa
data_sent: chuỗi trống để ghi kết quả giải mã
key: khóa bí mật