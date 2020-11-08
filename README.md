# fork_pthread_Learning


2次fork 之後，就會產生四個程式行程
https://blog.gtwang.org/programming/c-fork-tutorial-multi-process-programming/


什麼是程序 (process)
在開始談 fork 之前, 必須要了解什麼是程序 (process):
程式碼 (program) ： 假設你今天寫了是一支程式叫 example.c , 而且你尚未執行它, 則此時這支程式就叫 program
程序 (process) ： 倘若你把 example.c 編譯並執行, 程式被載入記憶體, 而且進到作業系統排程執行時, 便稱為程序, 而且每個程序都會有自己專屬的編號, 叫做 ProcessID


fork 就是把當前的 process (父程序) 又分支出另一個 process (子程序) , 而且父子程序長的一模一樣
https://wenyuangg.github.io/posts/linux/fork-use.html

重點1:
fork
多行程程式的變數與資料
不同行程之間的變數與資料都是互相獨立的，所以在其中一個行程中更改變數中的資料，並不會影響另外一個行程：
  pid_t pid;

  // 建立一個變數
  int x = 1;
  pid = fork();
  if (pid == 0) {
    // 子行程的變數
    printf("Child has x = %d\n", ++x);   x=2
  } else if (pid > 0) {
    // 父行程的變數
    printf("Parent has x = %d\n", --x);   x=0
  } else {
    printf("Error!n");
  }


自fork呼叫回傳之後，整個程式的執行流程就會一分為二，
父程序得到的回傳值（型別為pid_t）是子程序的程序ID
fork() 可能會有以下三種回傳值：
-1 ： 發生錯誤
0 ： 代表為子程序
大於 0 ： 代表為父程序, 其回傳值為子程序的 ProcessID

pid_t pid = fork();

if(pid == 0) {
	/*child process*/
} else {
	/*parent process*/
}

重點1:
何謂僵屍程序 (Zombie Process)：
僵屍程序是指一支存活在系統中, 但是他卻沒有做任何事, 只是佔著並耗用系統資源的程序
當使用 fork() 來建立子程序多工運行時, 如果子程序還沒運行結束就將父程序關閉的話, 就會有僵屍程序產生

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    // 從呼叫 fork 開始, 會分成兩支程序多工進行
    pid_t PID = fork();

    switch(PID){
        // PID == -1 代表 fork 出錯
        case -1:
            perror("fork()");
            exit(-1);
        
        // PID == 0 代表是子程序
        case 0:
            printf("I'm Child process\n");
            printf("Child's PID is %d\n", getpid());
            sleep(3);
            break;
        
        // PID > 0 代表是父程序
        default:
            printf("I'm Parent process\n");
            printf("Parent's PID is %d\n", getpid());
    }

    return 0;
}



https://blog.gtwang.org/programming/pthread-multithreading-programming-in-c-tutorial/
***pthread

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

編譯要加上:gcc hello.c -lpthread -o hello

pthread_daat資料傳遞

而在計算完之後再將結果傳回來，而子執行緒在傳回資料時通常都會以 malloc 配置記憶體空間來存放傳回的資料，以下是一個典型的範例：

加入一個互斥鎖（mutex），將那些不可以被多個執行緒同時執行的程式碼片段，用互斥鎖包起來，
當一個執行緒執行到該處時，就會先上鎖，避免其他的執行緒進入，
若其他的執行緒同時也要執行該處的程式碼時，就必須等待先前的執行緒執行完之後，才能接著進入（也就是排隊輪流使用的概念），
這樣就可以避免多個執行緒混雜執行，讓結果出錯的問題。



//

旗標（Semaphore）
如果我們現在有兩個執行緒，分別負責一份工作的前半段與後半段，也就是說第一個執行緒會把它處理好的資料，發包給第二個執行緒繼續處理，
而兩個執行緒的處理速度有可能不同，這種狀況我們就可以使用旗標（Semaphore）的方式來串接。

旗標本身就是一個計數器，也就是紀錄目前尚未處理的工作數量，
我們可以使用 sem_wait 來判斷是否有尚未處理的工作，當工作數量大於 0 時，sem_wait 就會讓執行緒進入處理，
並且把工作數量遞減 1，而如果工作數量為 0 的時候，則會讓執行緒等待，直到有新的工作來臨時，才讓執行緒進入。

另外在產生工作的執行緒中，可以使用 sem_post 放入新的工作（也就讓將計數器遞增 1），
這樣就可以將多個執行緒串接起來處理大型的工作流程。