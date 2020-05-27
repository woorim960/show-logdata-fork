// 헤더 선언
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

// 구조체 사이즈 설정
#define DATE_SIZE 20
#define MESSAGE_SIZE 60

// writelog.txt의 데이터를 읽어오기 위해 참조될 구조체
typedef struct _receive_record {
  char datetime[DATE_SIZE];
  char message[MESSAGE_SIZE];
} RECEIVE_RECORD;

int main() {

  // 구조체 선언
  RECEIVE_RECORD env;

  // 변수 선언
  int fd;

  // 파일 쓰기전용으로 오픈  
  if ((fd = open("writelog.txt", O_WRONLY | O_APPEND)) == -1) {
    puts("'writelog.txt' : 해당 파일이 존재하지 않습니다. (에러코드 : 1)");
    exit(1);
  }

  while (1) {
    // 날짜 데이터를 다루기 위한 변수 선언
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // 현재 날짜와 시간을 env.datetime에 저장(날짜와 시간이 정수형이기 때문에 문자열로 변환한 뒤 저장한다).
    sprintf(
      env.datetime, 
      "%04d/%02d/%02d %02d:%02d:%02d", 
      tm.tm_year+1900, 
      tm.tm_mon+1, 
      tm.tm_mday, 
      tm.tm_hour, 
      tm.tm_min, 
      tm.tm_sec
    );

    printf("테스트를 위한 로그 데이터를 입력해주세요(종료 : 'exit').\n>> ");
    fgets(env.message, MESSAGE_SIZE, stdin); // 로그 데이터 입력 받기
    env.message[strlen(env.message) -1] = '\0'; // fgets는 문자열의 맨 끝에 개행 문자를 삽입하게 되므로 개행문자를 지워줘야한다. 여기서는 문자열의 맨 끝임을 알리는 '\0'으로 대신해주었다.

    // 입력한 메세지가 exit이면 종료
    if (!strncmp(env.message, "exit", 4)) {
      puts("프로그램을 종료합니다.");
      close(fd);
      exit(0);
    }

    // 입력한 데이터 파일에 쓰기
    write(fd, &env, sizeof(env));
  }

  close(fd);
  exit(0);
}