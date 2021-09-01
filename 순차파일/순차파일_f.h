#ifndef 순차파일_f			//중복 호출 방지
#define 순차파일_f

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Record {        //레코드 구조체
	char code;
	int key;
	char std_num[10];
	char name[11];
	char major[3];
	int grade;
	int score;
} Record;

FILE* masterFile;			//마스터 파일
FILE* transactionFile;		//트랜잭션 파일
FILE* new_masterFile;		//신 마스터 파일

Record master;				//마스터 파일 레코드
Record trans;				//트랜잭션파일 레코드

char buffer[50];			//임시버퍼

void Update();				//갱신,수정,삭제			
void getNextTrans();		//다음 트랜잭션 레코드 호출
void getNextMaster();		//다음 마스터 레코드 호출
void endFile();				//두 파일중 하나가 끝났을 때

void Update()
{
	fopen_s(&masterFile, "masterFile.txt", "r");				//텍스트파일 열기
	fopen_s(&transactionFile, "transactionFile.txt", "r");
	fopen_s(&new_masterFile, "new_masterFile.txt", "w");

	if (masterFile == NULL || transactionFile == NULL) {	//파일 존재여부 확인
		fprintf(stderr, "파일이 존재하지 않습니다.");
		exit(1);
	}

	getNextTrans();
	getNextMaster();

	while (master.key != EOF && trans.key != EOF) {			//두 파일이 끝날때까지 반복
		if (master.key < trans.key) {						//마스터 파일 입력
			fprintf_s(new_masterFile, "%d %s %s %s %d %d\n", master.key, master.std_num, master.name, master.major, master.grade, master.score);
			getNextMaster();
		}
		else {												// 수정할 레코드가 있는가
			if (master.key == trans.key) {
				switch (trans.code)
				{
				case 'I':									//오류처리
					fprintf(stderr, "덮어쓰기는 불가능 합니다.error code : %c %d \n",trans.code, trans.key);
					getNextTrans();
					break;
				case 'C':									//레코드 수정
					fprintf_s(new_masterFile, "%d %s %s %s %d %d \n", trans.key, trans.std_num, trans.name, trans.major, trans.grade, trans.score);
					getNextTrans();
					getNextMaster();
					break;
				case 'D':									//레코드 삭제
					getNextMaster();
					getNextTrans();
					break;
				default:									//오류 처리
					printf("잘못된 업데이트 코드입니다.error code : %c \n", trans.code);
					getNextTrans();
					break;
				}
			}
			else {											//master.key > trans.key 인 경우
				endFile();
			}
		}
		if (master.key == EOF) {							//마스터 파일이 먼저 끝난경우
			while (trans.key != EOF)
				endFile();
		}
		if (trans.key == EOF) {								//트랜잭션 파일이 먼저 끝난 경우
			while (master.key != EOF) {
				fprintf_s(new_masterFile, "%d %s %s %s %d %d \n", master.key, master.std_num, master.name, master.major, master.grade, master.score);
				getNextMaster();
			}
		}
	}
	fclose(masterFile);										//파일 닫기
	fclose(transactionFile);
	fclose(new_masterFile);
}

void getNextTrans()											//다음 트랙잭션 레코드
{
	if (feof(transactionFile)) {
		trans.key = EOF;
	}
	else {
		fgets(buffer, sizeof(buffer), transactionFile);	//다음 한줄을 버퍼에 넣는다.

		if (buffer[0] == 'D') sscanf_s(buffer, " %c %d", &trans.code, sizeof(trans.code), &trans.key);		//삭제일경우 코드와 키값만 입력받는다.
		else sscanf_s(buffer, " %c %d %s %s %s %d %d", &trans.code, sizeof(trans.code), &trans.key, &trans.std_num,		//입력이나 수정일경우 전부 입력받는다.
			sizeof(trans.std_num), &trans.name, sizeof(trans.name), &trans.major, sizeof(trans.major), &trans.grade, &trans.score);
	}
	if (feof(transactionFile)) {							//파일의 끝인경우 키값을 EOF로 바꾼다
		trans.key = EOF;
	}
}

void getNextMaster()									//다음 마스터 레코드
{
	if (feof(masterFile)) {
		master.key = EOF;
	}
	else {												//마스터 파일의 다음 한줄을 포맷에 맞춰 입력받는다.
		fscanf_s(masterFile, "%d %s %s %s %d %d", &master.key, &master.std_num, sizeof(master.std_num), 
			&master.name, sizeof(master.name), &master.major, sizeof(master.major), &master.grade, &master.score);
	}
	if (feof(masterFile)) {								//파일의 끝인경우 키값을 EOF로 바꾼다.
		master.key = EOF;
	}


}

void endFile()											//master.key > trans.key 인 경우 또는 마스터 파일이 먼저 끝난 경우
{
	switch (trans.code)
	{
	case 'I':											//남은 트랜잭션 레코드를 신 마스터 파일에 입력
		fprintf_s(new_masterFile, "%d %s %s %s %d %d \n", trans.key, trans.std_num, trans.name, trans.major, trans.grade, trans.score);
		getNextTrans();
		break;
	case 'C':											//오류처리
		fprintf(stderr, "마스터 파일에 일치하는 수정할 레코드가 없습니다. error code : %c %d \n",trans.code,trans.key);
		getNextTrans();
		break;
	case 'D':											//오류 처리
		fprintf(stderr, "마스터 파일에 일치하는 삭제할 레코드가 없습니다.error code : %c %d \n", trans.code, trans.key);
		getNextTrans();
		break;
	default:											//오류처리
		fprintf(stderr, "잘못된 업데이트 코드입니다.error code : %c \n", trans.code);
		getNextTrans();
		break;
	}
}

#endif