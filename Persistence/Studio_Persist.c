/*
* Copyright(C), 2007-2008, XUPT Univ.
* ������ţ�TTMS_UC_01		 
* File name: Studio_Persist.c			  
* Description : �ݳ��������־û���	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��	
*/

#include "Studio_Persist.h"
#include "../Service/Studio.h" 
#include "EntityKey_Persist.h"	 
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>


static const char STUDIO_DATA_FILE[] = "Studio.dat"; //�ݳ����ļ������� 
static const char STUDIO_DATA_TEMP_FILE[] = "StudioTmp.dat"; //�ݳ�����ʱ�ļ������� 
static const char STUDIO_KEY_NAME[] = "Studio"; //�ݳ��������� 

/*
��ʶ����TTMS_SCU_Studio_Perst_Insert
�������ܣ��������ļ�������һ�����ݳ������ݡ�
����˵����dataΪstudio_t����ָ�룬����Ҫ���ӵ��ݳ������ݽ�㡣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ��������ݳ����ı�־��
*/ 
int Studio_Perst_Insert(studio_t *data) {	 
	assert(NULL!=data);

	////����������Ʒ����������ӵĴ���
	////����������Ʒ����������ӵĴ���
	long key = EntKey_Perst_GetNewKeys(STUDIO_KEY_NAME, 1); //Ϊ���ݳ��������ȡ
	if(key<=0)			//��������ʧ�ܣ�ֱ�ӷ���
		return 0;
	data->id = key;		//�����¶�����ص�UI��
	////����������Ʒ����������ӵĴ���
	////����������Ʒ����������ӵĴ���



	FILE *fp = fopen(STUDIO_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", STUDIO_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(studio_t), 1, fp);

	fclose(fp);
	return rtn;
}

/*
��ʶ����TTMS_SCU_Studio_Perst_Update 
�������ܣ��������ļ��и���һ���ݳ������ݡ�
����˵����dataΪstudio_t����ָ�룬����Ҫ���µ��ݳ������ݽ�㡣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ��������ݳ����ı�־��
*/
int Studio_Perst_Update(const studio_t * data) {
	assert(NULL!=data);

	FILE *fp = fopen(STUDIO_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", STUDIO_DATA_FILE);
		return 0;
	}

	studio_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(studio_t), 1, fp)) {
			if (buf.id == data->id) {
				fseek(fp, -((int)sizeof(studio_t)), SEEK_CUR);
				fwrite(data, sizeof(studio_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;
}

/*
��ʶ����TTMS_SCU_Studio_Perst_DeleteByID
�������ܣ����ڴ��ļ���ɾ��һ���ݳ��������ݡ�
����˵������һ������IDΪ���ͣ���ʾ��Ҫ�������ݵ��ݳ���ID���ڶ�������bufΪstudio_tָ�룬ָ�������ݳ������ݵ�ָ�롣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ�ɾ�����ݳ����ı�־��
*/
int Studio_Perst_DeleteByID(int ID) {

	//��ԭʼ�ļ���������Ȼ���ȡ��������д�뵽�����ļ��У�����Ҫɾ����ʵ����˵���

	//��ԭʼ�����ļ�������
	if(rename(STUDIO_DATA_FILE, STUDIO_DATA_TEMP_FILE)<0){
		printf("Cannot open file %s!\n", STUDIO_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(STUDIO_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("Cannot open file %s!\n", STUDIO_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(STUDIO_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("Cannot open file %s!\n", STUDIO_DATA_TEMP_FILE);
		return 0;
	}


	studio_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(studio_t), 1, fpSour)) {
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(studio_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//ɾ����ʱ�ļ�
	remove(STUDIO_DATA_TEMP_FILE);
	return found;
}

/*
��ʶ����TTMS_SCU_Studio_Perst_SelectByID 
�������ܣ����ڴ��ļ�������һ���ݳ��������ݡ�
����˵������һ������IDΪ���ͣ���ʾ��Ҫ�������ݵ��ݳ���ID���ڶ�������bufΪstudio_tָ�룬ָ�������ݳ������ݵ�ָ�롣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ��������ݳ����ı�־��
*/
int Studio_Perst_SelectByID(int ID, studio_t *buf) {
	assert(NULL!=buf);

	FILE *fp = fopen(STUDIO_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	studio_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(studio_t), 1, fp)) {
			if (ID == data.id) {
				*buf = data;
				found = 1;
				break;
			};

		}
	}
	fclose(fp);

	return found;
}

/*
��ʶ����TTMS_SCU_Studio_Perst_SelAll 
�������ܣ����ڴ��ļ������������ݳ������ݡ�
����˵����list��studio_list_t����ָ�룬ָ���ݳ���������ͷָ�롣
�� �� ֵ�����ͣ���ʾ�ɹ��������ݳ����ĸ�����
*/
int Studio_Perst_SelectAll(studio_list_t list) {
	studio_node_t *newNode;
	studio_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, studio_node_t);

	FILE *fp = fopen(STUDIO_DATA_FILE, "rb");
	if (NULL == fp) { //�ļ�������
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(studio_t), 1, fp)) {
			newNode = (studio_node_t*) malloc(sizeof(studio_node_t));
			if (!newNode) {
				printf(
						"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}
