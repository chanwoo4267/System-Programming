#ifndef __20171645_H__
#define __20171645_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

//------------------------------------------------------ History 관련 -------------------------------------------------------

/*
 링크드리스트인 History_List 의 노드 역활을 하는 구조체
*/
typedef struct history_node 
{
	char data[501];
	struct history_node* next;
} History_Node;
/*
 링크드 리스트 형태로 명령어 기록을 저장하는 구조체
*/
typedef struct history_list 
{
	int size;
	struct history_node* head;
	struct history_node* tail;
} History_List;

//------------------------------------------------------ Memory 관련 --------------------------------------------------------
/*
Memory Value를 저장하는 구조체
*/
typedef struct memory
{
	short value;
} Memory;
/*
 전체 메모리 Array를 저장하는 구조체
*/
typedef struct memory_array
{
	Memory data[16*65536];
} Memory_Array;

//----------------------------------------------------- OPCode 관련 -----------------------------------------------------------

/*
Opcode의 데이터를 저장하는 구조체
*/

typedef struct opcode 
{
	int code;
	char command[50];
	char format[50];
} Opcode;

/*
Opcode_Table의 노드 역활을 하는 구조체
*/

typedef struct opcode_node
{
	Opcode* data;
	struct opcode_node* next;
} Opcode_Node;

/*
Opcode 를 링크드 리스트 형태로 저장하는 구조체
즉, 해시한 값이 같은 Opcode 들을 링크드 리스트 형태로 저장한다
*/

typedef struct opcode_list
{
	struct opcode_node* head;
	struct opcode_node* tail;
	int size;
} Opcode_List;

/*
Opcode를 저장하는 링크드리스트 들을 저장하는 구조체
즉, 해시되어 분류된 Opcode를 갖는 링크드 리스트를 배열 형태로 저장
*/

typedef struct opcode_table
{
	Opcode_List** list;
} Opcode_Table;

//--------------------------------------------------- state 관련 ---------------------------------------------------

/*
프로그램 동작에 필요한 구조체 3가지
History, Memory, Opcode를 묶는 구조체
*/

typedef struct state {
	// command history
	History_List* history_state;
	// memory
	Memory_Array* memory_state;
	// opcode
	Opcode_Table* opcode_state;
} State;

#endif