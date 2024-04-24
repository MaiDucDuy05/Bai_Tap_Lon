# **Đối Kháng**
*Dự án cho bài thi giữa kì tại UET*
Xin chào mọi người, tên mình là Mai Đức Duy . Đây là dự án đầu tiên của mình. Trong dự án này, mình sẽ làm về tựa game Đối Kháng, Nhập vai với một chút sáng tạo riêng, sử dụng ngôn ngữ [C++](https://en.wikipedia.org/wiki/C++) và thư viện [SDL 2.0](https://www.libsdl.org/download-2.0.php).


Nếu bạn muốn học về SDL2.0, hãy vào [website](https://lazyfoo.net/tutorials/SDL/index.php) này.

Lần đầu viết một project nên code có hơi khó đọc.

## Mục lục
1. [Cách tải và cài đặt game](#1-cách-tải-và-cài-đặt-game)
2. [Chi tiết về game và cách chơi](#2-chi-tiết-về-game-và-cách-chơi)
3. [Thuật toán của game](#3-thuật-toán-của-game) 
4. [Nguồn ảnh và âm thanh](#nguồn-ảnh-và-âm-thanh)

   ### Nội dung
   ### 1. Cách tải và cài đặt game
   Đầu tiên bạn sẽ cần cài đặt Visual Studio 2022 để chạy C++, làm theo các hướng dẫn để cài đặt:  
        - [SDL 2.0](https://www.libsdl.org/download-2.0.php)  
        - [SDL_image](https://www.libsdl.org/projects/SDL_image/)  
        - [SDL_mixer](https://www.libsdl.org/projects/SDL_mixer/)  
        - [SDL_ttf](https://www.libsdl.org/projects/SDL_ttf/)
    Sau khi xong, tải file zip game của mình trên github về và giải nén chúng.
    Sau đó Mở file Game_SDL2.0.sln để chạy chương trình.
    ### 2. Chi tiết về game và cách chơi
   Dự án game này là một trò chơi đối kháng dựa trên ý tưởng Game Naruto vs Bleach  là một trải nghiệm chiến đấu hành động độc đáo, nơi người chơi sẽ được đưa vào cuộc phiêu lưu đầy kịch tính giữa thế giới của các siêu anh hùng.
   người chơi sẽ được tham gia vào các trận đấu đỉnh cao, sử dụng các kỹ năng và chiêu thức đặc biệt để chiến thắng. 
   #### Cách chơi:
   Sử dụng linh hoạt các phím để có thể giành chiến thắng

   Để di chuyển Người chơi thứ nhất ![logo](https://i.imgur.com/MIjV6pI.png)
   
      Sử dụng Phím `A` Để di chuyển sang trái
   
      Sử dụng phím `D` Để di chuyển sang phải
   
      Sử dụng phím `W` Để nhảy lên trên
   
      Sử dụng phím `S` Để phòng thủ
   
      Sử Dùng các phím `J``U` `I` Để sử dụng các chiêu đánh của nhân vật
   


   Để di chuyển Người chơi thứ hai ![logo](https://i.imgur.com/S48p4xx.png)

     Sử dụng Phím `⇦` Để di chuyển sang trái
   
      Sử dụng phím `⇨` Để di chuyển sang phải
   
      Sử dụng phím `⇧` Để nhảy lên trên
   
      Sử dụng phím `⇩` Để phòng thủ
   
      Sử Dùng các phím `1` `4` `6` Để sử dụng các chiêu đánh của nhân vật

   #### Chi tiết 
   Ở màn hình Menu chính bạn có thể xem hướng dẫn chi tiết bằng cách nhấn vào `HELP`. Ở đây có chế độ chơi chơi theo hướng dẫn bạn có thể nhấn vào để hiểu rõ luật chơi

   Ở phần `Play Game` Bạn có 3 chế độ chơi
   ##### 1. `VS Human`:

   Đây là chế độ chơi 2 người sử dùng các bộ bàn phím như trên. Bạn phải sử dụng các thao tác một cách linh hoạt để có thể giành chiến thắng

   ##### 2. `VS Moster`

   Đây là chế độ săn Rồng gồm các chế độ 1 người chơi và 2 người chơi . Trong Vòng 200s bạn phải săn Rồng một cách nhanh chóng và nhiều nhất để có thể giành chiến thắng.

   ##### 3. `VS Com`
    Đây là chế độ một người chơi. Ở đây bạn sẽ sử dụng nhân vật ![logo](https://i.imgur.com/MIjV6pI.png) để đánh với nhân vật  ![logo](https://i.imgur.com/S48p4xx.png)do máy chơi
    Bạn phải sử dụng các thao tác một cách linh hoạt để có thể giành chiến thắng

  #### PREVIEW
  ![logo](https://i.imgur.com/T3GuIon.png)
   ### 3. Thuật toán của game 
   
   

