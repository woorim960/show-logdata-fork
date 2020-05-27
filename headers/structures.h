// headers/structures.h

// 구조체 사이즈 설정
#define DATE_SIZE 20
#define MASSAGE_SIZE 60
#define ENV_KEY_SIZE 10
#define ENV_VALUE_SIZE 30

// writelog.txt의 데이터를 읽어오기 위해 참조될 구조체
typedef struct _receive_record {
  char datetime[DATE_SIZE];
  char message[MASSAGE_SIZE];
} RECEIVE_RECORD;

// setup.txt의 데이터를 읽어오기 위해 참조될 구조체
typedef struct _environment_parameter {
  int seq;
  char key[ENV_KEY_SIZE];
  char value[ENV_VALUE_SIZE];
} ENV_PARAMETER;