#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// 子執行緒函數
void *child(void *arg) {
   int *input = (int *) arg; // 取得資料
   int *result = malloc(sizeof(int) * 1); // 配置記憶體
   result[0] = input[0] + input[1]; // 進行計算
   pthread_exit((void *) result); // 傳回結果
}


//pthread_create (threadid,attr,start_routine,arg)
// thread：返回一個不透明的，唯一的新執行緒識別符號。
//attr：不透明的執行緒屬性物件。可以指定一個執行緒屬性物件，或者NULL為預設值。
//start_routine：執行緒將會執行一次的C函式。
//arg: 傳遞給start_routine單個引數，傳遞時必須轉換成指向void的指標型別。沒有引數傳遞時，可設定為NULL。

// 主程式
int main() {
   pthread_t t;
   void *ret; // 子執行緒傳回值
   int input[2] = {1, 2}; // 輸入的資料

   // 建立子執行緒，傳入 input 進行計算
   pthread_create(&t, NULL, child, (void*) input);

   // 等待子執行緒計算完畢
   pthread_join(t, &ret);

   // 取得計算結果
   int *result = (int *) ret;

   // 輸出計算結果
   printf("%d + %d = %d\n", input[0], input[1], result[0]);

   // 釋放記憶體
   free(result);

   return 0;
}