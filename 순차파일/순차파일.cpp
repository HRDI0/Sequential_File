#include "순차파일_f.h"

int main() {
	fopen_s(&transactionFile, "transactionFile.txt", "w");		//트랜잭션파일 입력
	Record input_t;
	char stop = 'n';
	while(1){													//명령어 입력
		printf("명령을 입력해 주십시오. ( 입력 : I / 수정 : C / 삭제 : D )\n");
		scanf_s(" %c", &input_t.code, 1);
		switch (input_t.code)
		{
		case 'I':												//I 명령인 경우 레코드 입력
			printf("※주의 \n 학번순으로 입력해 주십시오. \n 각 명령어는 공백으로 구분지어 주십시오. \n(예시 : 1 201621763 Jhone CE 2 100) \n");
			scanf_s("%d %s %s %s %d %d",&input_t.key, &input_t.std_num, sizeof(input_t.std_num), &input_t.name, sizeof(input_t.name), &input_t.major, sizeof(input_t.major), &input_t.grade, &input_t.score);
			if (strcmp(input_t.major, "CE") != 0 && strcmp(input_t.major, "IE") != 0 && strcmp(input_t.major, "ME") != 0	//학과 오류 확인
				&& strcmp(input_t.major, "EE") != 0 && strcmp(input_t.major, "AR") != 0) {
				printf("잘못된 학과입니다.error code %s\n", input_t.major);
				break;
			}
			else if (input_t.grade != 1 && input_t.grade != 2 && input_t.grade != 3 && input_t.grade != 4) {		//학년 오류 확인
				printf("잘못된 학년입니다.error code %d\n", input_t.grade);
				break;
			}
			else if (input_t.score < 0 && input_t.score > 100) {													//점수 오류 확인
				printf("잘못된 점수입니다.error code %d\n", input_t.score);
				break;
			}																										//모든 오류를 통과했을시 트랜잭션 파일에 입력
			fprintf_s(transactionFile, "%c %d %s %s %s %d %d\n", input_t.code, input_t.key, input_t.std_num, input_t.name, input_t.major, input_t.grade, input_t.score);
			break;
		case 'C':																								//수정 명령어 입력시 레코드 입력
			printf("※주의 \n 학번순으로 입력해 주십시오. \n 각 명령어는 공백으로 구분지어 주십시오. \n(예시 : 1 201712345 Reon EE 1 20) \n");
			scanf_s("%d %s %s %s %d %d", &input_t.key, &input_t.std_num, sizeof(input_t.std_num), &input_t.name, sizeof(input_t.name), &input_t.major, sizeof(input_t.major), &input_t.grade, &input_t.score);
			if (strcmp(input_t.major, "CE") != 0 && strcmp(input_t.major, "IE") != 0 && strcmp(input_t.major, "ME") != 0		//오류 확인 이하동문
				&& strcmp(input_t.major, "EE") != 0 && strcmp(input_t.major, "AR") != 0) {
				printf("잘못된 학과입니다.error code %s\n", input_t.major);
				break;
			}
			else if (input_t.grade != 1 && input_t.grade != 2 && input_t.grade != 3 && input_t.grade != 4) {
				printf("잘못된 학년입니다.error code %d\n", input_t.grade);
				break;
			}
			else if (input_t.score < 0 && input_t.score > 100) {
				printf("잘못된 점수입니다.error code %d\n", input_t.score);
				break;
			}
			fprintf_s(transactionFile, "%c %d %s %s %s %d %d\n", input_t.code, input_t.key, input_t.std_num, input_t.name, input_t.major, input_t.grade, input_t.score);
			break;
		case 'D':																			//삭제 명령어 입력시
			printf("※주의 \n(예시 : 2 ) \n");												//키값만 입력
			scanf_s("%d", &input_t.key);
			fprintf_s(transactionFile, "%c %d\n", input_t.code, input_t.key);
			break;
		default:
			fprintf(stderr, "잘못된 코드입니다.error code : %c \n", input_t.code);			//명령어 오류 확인
			break;
		}
		
		printf("명령어 입력을 마치셨습니까? y , n \n");										//트랜잭션 파일 작성 완료
		scanf_s(" %c", &stop,1);
		if (stop == 'y') {
			fclose(transactionFile);
			break;
		}
	}
	Update();																				//갱신,수정,삭제 실행
	rename("OldmasterFile.txt", "Oldest_masterFile.txt");									//기존의 Old masterFile을 Oldest masterFile로 바꾸고
	if (rename("masterFile.txt", "OldmasterFile.txt") == 0) {								//기존의 masterFile을 Old masterFile 로 바꾼다.
		rename("new_masterFile.txt", "masterFile.txt");										//마지막으로 기존의 new_masterFile을 masterFile로 바꾼다.
	}
	return 0;
}