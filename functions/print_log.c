// 헤더 선언
#include "../headers/init.h"
#include "../headers/structures.h"
#include <signal.h>

//  exit_main 함수에 매개변수를 전달할 방법이 없어(전달하기 위해서는 signal 구조체 사용) 전역 변수로 선언
int log_fd;

// 함수 원형 선언
void __exit__(int sig);

// print_log.c의 메인 함수
void print_log(char log_file[]) {
  // 터미널을 깨끗히 비운다.
  system("clear");

  // writelog.txt의 레코드를 원활하게 사용하도록 도와줄 레코드 선언
  RECEIVE_RECORD log_data;

  // 변수 선언
  int pid;
  char exit_msg[10];

  // SIGALRM 신호 발생시 
  signal(SIGALRM, __exit__);

  // set_env에서 설정된 로그 파일을 open
  if ((log_fd = open(log_file, O_RDONLY)) == -1) {
    printf("%s : 해당 경로 혹은 파일이 존재하지 않습니다. (에러코드 : 1)\n", log_file);
    exit(1);
  }

  // 자식 프로세스 생성
  pid = fork();

  // exit_pid 프로세스가 실행할 구문
  if (pid == 0) {
    while (1) {
      scanf("%s", exit_msg); // 종료 메시지 입력받기

      // 입력 메세지가 q 이거나 e 이면 종료
      if (!strncmp(exit_msg, "q", 4) || !strncmp(exit_msg, "e", 1)) {
        puts("프로그램을 종료합니다.");
        kill(getppid(), SIGALRM); // 부모 프로세스에게 SIGALRM 신호 전달
        exit(0); // 자식 프로세스 종료
        break;
      }
    }
  } else if (pid == -1) {
    // 자식 프로세스 생성 오류시 실행
    puts("\n자식 프로세스(exit_pid) 생성 에러 (에러코드 : 2");
    exit(2);
  } 

  // 부모 프로세스가 실행할 구문
  while (1) {
    // UI 출력
    printf("\n\nSystem Log Status    File Name : %s", log_file);
    puts("\n\nDate/Time            Log Details");
    puts("===================  ======================================");

    // 시작 시퀀스 값부터 10개까지 출력한다.
    for (int i = 1; i < 11; i++) {
      // SEEK_END로 맨 뒤부터 데이터를 10개 읽게 된다.
      lseek(log_fd, -i * sizeof(log_data), SEEK_END);
      read(log_fd, &log_data, sizeof(log_data));
      // 데이터 출력
      printf("%s  %s\n", log_data.datetime, log_data.message);
    }

    puts("\n종료 : 'q' or 'e'");

    // 2초 중지
    sleep(2);

    // 터미널을 깨끗히 비운다.
    system("clear");
  }

  // 파일 디스크립터 반환(종료)
  close(log_fd);
}

// 자식 프로세스가 종료되고 SIGALRM 신호가 부모에게 전달되면 실행되는 함수
void __exit__(int sig) {
  
  // 전달된 신호가 SIGALRM이면 실행
  if (sig == SIGALRM) {
    // 파일 디스크립터 반환(종료)
    close(log_fd);
    exit(0);
  }
}