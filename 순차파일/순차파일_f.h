#ifndef ��������_f			//�ߺ� ȣ�� ����
#define ��������_f

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Record {        //���ڵ� ����ü
	char code;
	int key;
	char std_num[10];
	char name[11];
	char major[3];
	int grade;
	int score;
} Record;

FILE* masterFile;			//������ ����
FILE* transactionFile;		//Ʈ����� ����
FILE* new_masterFile;		//�� ������ ����

Record master;				//������ ���� ���ڵ�
Record trans;				//Ʈ��������� ���ڵ�

char buffer[50];			//�ӽù���

void Update();				//����,����,����			
void getNextTrans();		//���� Ʈ����� ���ڵ� ȣ��
void getNextMaster();		//���� ������ ���ڵ� ȣ��
void endFile();				//�� ������ �ϳ��� ������ ��

void Update()
{
	fopen_s(&masterFile, "masterFile.txt", "r");				//�ؽ�Ʈ���� ����
	fopen_s(&transactionFile, "transactionFile.txt", "r");
	fopen_s(&new_masterFile, "new_masterFile.txt", "w");

	if (masterFile == NULL || transactionFile == NULL) {	//���� ���翩�� Ȯ��
		fprintf(stderr, "������ �������� �ʽ��ϴ�.");
		exit(1);
	}

	getNextTrans();
	getNextMaster();

	while (master.key != EOF && trans.key != EOF) {			//�� ������ ���������� �ݺ�
		if (master.key < trans.key) {						//������ ���� �Է�
			fprintf_s(new_masterFile, "%d %s %s %s %d %d\n", master.key, master.std_num, master.name, master.major, master.grade, master.score);
			getNextMaster();
		}
		else {												// ������ ���ڵ尡 �ִ°�
			if (master.key == trans.key) {
				switch (trans.code)
				{
				case 'I':									//����ó��
					fprintf(stderr, "������ �Ұ��� �մϴ�.error code : %c %d \n",trans.code, trans.key);
					getNextTrans();
					break;
				case 'C':									//���ڵ� ����
					fprintf_s(new_masterFile, "%d %s %s %s %d %d \n", trans.key, trans.std_num, trans.name, trans.major, trans.grade, trans.score);
					getNextTrans();
					getNextMaster();
					break;
				case 'D':									//���ڵ� ����
					getNextMaster();
					getNextTrans();
					break;
				default:									//���� ó��
					printf("�߸��� ������Ʈ �ڵ��Դϴ�.error code : %c \n", trans.code);
					getNextTrans();
					break;
				}
			}
			else {											//master.key > trans.key �� ���
				endFile();
			}
		}
		if (master.key == EOF) {							//������ ������ ���� �������
			while (trans.key != EOF)
				endFile();
		}
		if (trans.key == EOF) {								//Ʈ����� ������ ���� ���� ���
			while (master.key != EOF) {
				fprintf_s(new_masterFile, "%d %s %s %s %d %d \n", master.key, master.std_num, master.name, master.major, master.grade, master.score);
				getNextMaster();
			}
		}
	}
	fclose(masterFile);										//���� �ݱ�
	fclose(transactionFile);
	fclose(new_masterFile);
}

void getNextTrans()											//���� Ʈ����� ���ڵ�
{
	if (feof(transactionFile)) {
		trans.key = EOF;
	}
	else {
		fgets(buffer, sizeof(buffer), transactionFile);	//���� ������ ���ۿ� �ִ´�.

		if (buffer[0] == 'D') sscanf_s(buffer, " %c %d", &trans.code, sizeof(trans.code), &trans.key);		//�����ϰ�� �ڵ�� Ű���� �Է¹޴´�.
		else sscanf_s(buffer, " %c %d %s %s %s %d %d", &trans.code, sizeof(trans.code), &trans.key, &trans.std_num,		//�Է��̳� �����ϰ�� ���� �Է¹޴´�.
			sizeof(trans.std_num), &trans.name, sizeof(trans.name), &trans.major, sizeof(trans.major), &trans.grade, &trans.score);
	}
	if (feof(transactionFile)) {							//������ ���ΰ�� Ű���� EOF�� �ٲ۴�
		trans.key = EOF;
	}
}

void getNextMaster()									//���� ������ ���ڵ�
{
	if (feof(masterFile)) {
		master.key = EOF;
	}
	else {												//������ ������ ���� ������ ���˿� ���� �Է¹޴´�.
		fscanf_s(masterFile, "%d %s %s %s %d %d", &master.key, &master.std_num, sizeof(master.std_num), 
			&master.name, sizeof(master.name), &master.major, sizeof(master.major), &master.grade, &master.score);
	}
	if (feof(masterFile)) {								//������ ���ΰ�� Ű���� EOF�� �ٲ۴�.
		master.key = EOF;
	}


}

void endFile()											//master.key > trans.key �� ��� �Ǵ� ������ ������ ���� ���� ���
{
	switch (trans.code)
	{
	case 'I':											//���� Ʈ����� ���ڵ带 �� ������ ���Ͽ� �Է�
		fprintf_s(new_masterFile, "%d %s %s %s %d %d \n", trans.key, trans.std_num, trans.name, trans.major, trans.grade, trans.score);
		getNextTrans();
		break;
	case 'C':											//����ó��
		fprintf(stderr, "������ ���Ͽ� ��ġ�ϴ� ������ ���ڵ尡 �����ϴ�. error code : %c %d \n",trans.code,trans.key);
		getNextTrans();
		break;
	case 'D':											//���� ó��
		fprintf(stderr, "������ ���Ͽ� ��ġ�ϴ� ������ ���ڵ尡 �����ϴ�.error code : %c %d \n", trans.code, trans.key);
		getNextTrans();
		break;
	default:											//����ó��
		fprintf(stderr, "�߸��� ������Ʈ �ڵ��Դϴ�.error code : %c \n", trans.code);
		getNextTrans();
		break;
	}
}

#endif