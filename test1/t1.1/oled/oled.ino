#include <U8glib.h>

//注意：使用前要添加上U8glib.h这个库
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//库的使用
void draw(void) {
  // 设置字体  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);//与上面一行的字体不同
  
  u8g.drawStr( 0, 10, "Lijing");
  
  u8g.drawStr( 0, 60, "2020070718");
}

void setup(void) {
}

void loop(void) {
  // 调用此过程标志着图像循环的开始
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );//标志着结束
  
  delay(500);
}
