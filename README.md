I/ Cài đặt ứng dụng (tệp .xml sẽ chứa giao diện thiết kế và tệp .java chứa giải thuật cho chương trình)
1. Khởi động phần mềm Android Studio
2. Sao chép đường dẫn đến thư mục ứng dung (thư mục ứng dụng sẽ chứa các folder như .gradle, .idea, app, gradle và các file cài đặt khác) 
3. Vào Open Project để dán đường dẫn tới ứng dụng và chọn OK
4. Ứng dụng sẽ tự động Import project và cài đặt các thư viện liên quan
5. Vào mục Settings, ở tab Languages & Frameworks chọn Android SDK để cài đặt Android 14.0 ("UpsideDownCake") với API Level 34.
6. Vào Device Manager, tại dấu cộng Add a new device ta chọn Create Virtual Device để thêm máy ảo khởi chạy ứng dụng vào Android Studio.
7. Chọn máy ảo muốn khởi chạy tại mục Phone (khuyến nghị lựa chọn dòng máy Pixel 4).
8. Tại system image hệ thống sẽ tự động đưa ra đề xuất cài đặt phù hợp (khuyến nghị tải SV2 với API 32 cho máy ảo).
9. Khởi chạy ứng dụng sau khi đã cài đặt máy ảo bằng cách nhấn vào biểu tượng nút Play ở phía trên màn hình hoặc gõ Shift + F10. (Nếu muốn khởi chạy ứng dụng trên điện thoại hệ điều hành Android thay cho máy ảo thì tham khảo từ bước 10)
10. Để ứng dụng chạy trên điện thoại (hệ điều hành Android), ta vào cài đặt, mở thông tin phần mềm và nhấn 5 lần vào Số bản dựng trên điện thoại để mở tuỳ chọn cho nhà phát triển.
11. Tại mục tuỳ chọn cho nhà phát triển, ta lướt tới tab Gỡ lỗi và bật Gỡ lỗi qua kết nối không dây.
12. Chọn ghép nối thiết bị bằng mã QR (phải kết nối thiết bị và Android Studio trên cùng mạng Wi-Fi).
13. Tại mục Device Manager trên Android Studio ta chọn Pair Devices Using Wi-Fi để hiển thị mã QR ghép nối với điện thoại.
14. Nếu ghép nối thành công sẽ xuất hiện tên thiết bị đã ghép nối trên Android Studio
15. Để khởi chạy ứng dụng với thiết bị đã ghép nối, ta tiến hành ấn vào biểu tượng nút Play ở phía trên màn hình hoặc gõ Shift + F10.
16. Ứng dụng sẽ tự động được cài đặt vào điện thoại và khởi chạy.

II/ Khởi chạy phần cứng (tệp .ino sẽ được nạp vào ESP32 thông qua phần mềm Arduino IDE để khởi chạy phần cứng)
1. Thực hiện phát Wi-Fi băng tần 2.4GHz trên điện thoại với SSID: PaDoiThong và Password: 667788200801 (hoặc có thể thay đổi tên và mật khẩu trong tệp .ino để phù hợp) cho phần cứng có thể kết nối để gửi và nhận dữ liệu qua ứng dụng qua Firebase
2. Cấp nguồn cho phần cứng để phần cứng khởi chạy.
