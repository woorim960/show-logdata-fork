// 헤더 선언
#include "../headers/init.h"
#include "../headers/structures.h"

// set_logfile.c의 메인 함수
void set_logfile(char log_file[]) {
  // 터미널을 깨끗히 비운다.
  system("clear");

  // setup.txt의 레코드를 원활하게 사용하도록 도와줄 레코드 선언
  ENV_PARAMETER env;

  // 변수 선언
  int env_fd;
  int read_len = 1; // read 데이터의 길이를 저장할 변수
  char env_file[20]; // 환경 변수가 기록되어 있는 파일의 이름이 저장될 변수

  // 환경 변수 셋업 파일 입력
  puts("세팅에 필요한 파일을 입력해주세요.");
  printf(">> ");
  fgets(env_file, 20, stdin);
  env_file[strlen(env_file) -1] = '\0'; // fgets는 문자열의 맨 끝에 개행 문자를 삽입하게 되므로 개행문자를 지워줘야한다(파일 open할 때 발생하는 에러를 방지함). 여기서는 문자열의 맨 끝임을 알리는 '\0'으로 대신해주었다.
  
  // 환경 변수 셋업 파일 오픈
  if ((env_fd = open(env_file, O_RDONLY)) == -1) {
    puts("파일이 존재하지 않거나 잘못된 파일명입니다. (에러코드 : 1)");
    exit(1);
  }

  // 환경 변수의 key 값이 "file"인 것을 찾기 위한 반복문 => 해당 key 값에 따른 value 값(writelog.txt)의 데이터를 읽기 위한 초기 과정
  int i = 0;
  while(read_len) {
    // 모든 레코드를 읽어 한 줄씩 비교하기 위한 커서(lseek) 이동
    lseek(env_fd, i * sizeof(env), SEEK_SET);
    read_len = read(env_fd, &env, sizeof(env)); // 읽은 데이터가 0이면(없으면) 반복문 종료

    // 환경 변수의 key 값이 "file"이면 해당 value를 log 변수에 저장
    if (!strncmp(env.key, "file", 4)) {
      // "file" key 값의 value를 log_file에 복사
      strcpy(log_file, env.value);
      break; // 해당 값을 찾았으면 더이상 순회할 필요가 없으니 반복문 종료
    }
    i++;
  }

  // 환경 변수 파일은 더이상 쓸 일이 없으므로 파일 디스크립터 반환(종료)
  close(env_fd);
}