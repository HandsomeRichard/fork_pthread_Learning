#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// 子執行緒函數
void* child(void* data) {
  char *str = (char*) data; // 取得輸入資料
  for(int i = 0;i < 3;++i) {
    printf("%s\n", str); // 每秒輸出文字
    sleep(1);
  }
  pthread_exit(NULL); // 離開子執行緒
}

//pthread_create (threadid,attr,start_routine,arg)
// thread：返回一個不透明的，唯一的新執行緒識別符號。
//attr：不透明的執行緒屬性物件。可以指定一個執行緒屬性物件，或者NULL為預設值。
//start_routine：執行緒將會執行一次的C函式。
//arg: 傳遞給start_routine單個引數，傳遞時必須轉換成指向void的指標型別。沒有引數傳遞時，可設定為NULL。

// 主程式
int main() {
  pthread_t t; // 宣告 pthread 變數
  pthread_create(&t, NULL, child, "Child"); // 建立子執行緒

  // 主執行緒工作
  for(int i = 0;i < 3;++i) {
    printf("Master\n"); // 每秒輸出文字
    sleep(1);
  }

  pthread_join(t, NULL); // 等待子執行緒執行完成
  return 0;
}