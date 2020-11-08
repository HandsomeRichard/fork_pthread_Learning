#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


//主執行緒管理記憶體

typedef struct mydata{
	
	int a;
	int b;
	int result;
	
}mydata;


// 子執行緒函數
void *child(void *arg) {
   //第一步先轉型
   mydata *data = (mydata *)arg;
   
   int a  = data->a;
   int b =  data->b;
   int result = a+b;
    
	data->result = result;
	pthread_exit(NULL);
	
}


//pthread_create (threadid,attr,start_routine,arg)
// thread：返回一個不透明的，唯一的新執行緒識別符號。
//attr：不透明的執行緒屬性物件。可以指定一個執行緒屬性物件，或者NULL為預設值。
//start_routine：執行緒將會執行一次的C函式。
//arg: 傳遞給start_routine單個引數，傳遞時必須轉換成指向void的指標型別。沒有引數傳遞時，可設定為NULL。

// 主程式
int main() {
   pthread_t t;
   
   mydata data;
   
   data.a = 1;
   data.b = 2;
   
   //建立子執行緒，傳入data進行運算
   pthread_create(&t,NULL,child,(void*)&data);
   
   pthread_join(t,NULL);
   
      // 從 data.result 取回計算結果
   printf("%d + %d = %d\n", data.a, data.b, data.result);

   return 0;
}