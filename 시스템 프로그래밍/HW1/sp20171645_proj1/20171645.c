#define _CRT_SECURE_NO_WARNINGS
#include "20171645.h"

//------------------------------------------------------ History 관련 함수들 ------------------------------------------

/*
	History 데이터를 저장하는 링크드 리스트 형태의 구조체를 동적할당해 반환한다.
	동적할당한 후 내용값들을 초기값 (0, NULL)로 초기화한다.
	Input : X
	Output : History_List* history
*/

History_List* construct_History_List()
{
	History_List* history = (History_List*)malloc(sizeof(History_List));
	history->size = 0;
	history->head = NULL;
	history->tail = NULL;
	return history;
}

/*
	History 를 저장하는 링크드 리스트에 새로운 데이터를 추가로 입력한다.
	입력되는 데이터 ( data ) 는 이미 정리가 완료된 문자열이다.
	History List의 마지막 노드 다음에 새로운 데이터를 추가하고, History List의 size를 1 증가시킨다.
	Input : History_List* history, char* data
	Output : History_List* history
*/

History_List* History_push(History_List* history, char* data)
{
	History_Node* input = (History_Node*)malloc(sizeof(History_Node));
	strncpy(input->data, data, 500);
	input->next = NULL;
	if (history->head == NULL && history->tail == NULL)
	{
		history->head = input;
		history->tail = input;
	}
	else
	{
		history->tail->next = input;
		history->tail = input;
	}
	history->size++;
	return history;
}

/*
	History List의 노드 하나의 메모리를 해제한다.
	Input : History_Node* p
	Output : X
*/

void History_free(History_Node* p)
{
	free(p);
}

/*
	History List에서 마지막 데이터를 삭제한다.
	Test 상의 이유로 구현
	사용하지 않음

History_List* History_pop(History_List* history)
{
	if (history->size == 0)
		return history;
	else if (history->size == 1)
	{
		//History_free(history->head);
		history->head = history->tail = NULL;
		return history;
	}

	History_Node* out = history->head;
	while (out->next->next != NULL) 
		out = out->next;
	//History_free(out->next);
	out->next = NULL;
	history->tail = out;
	history->size--;
	return history;
}

*/

/*
	그동안의 History 기록을 저장한 History List를 형식에 맞게 출력한다.
	가장 처음 입력한 명령어 부터 가장 마지막으로 입력한 명령어까지 순서대로 출력한다.
	이 함수가 실행되면 가장 마지막 명령어는 반드시 hi(story) 이다.
	Input : History_List* history
	Output : History_List* history
*/

History_List* History_print(History_List* history)
{
	int count = 1;
	History_Node* p = history->head;
	printf("\n");
	while (p != NULL)
	{
		printf("%d\t%s\n", count,p->data);
		p = p->next;
		count++;
	}
	printf("\n");
	return history;
}

//------------------------------------------------------ memory 관련 함수들 ------------------------------------------------

/*
	1Mb 크기의 메모리를 저장할 Memory_Array를 동적 할당한다.
	Input : X
	Output : Memory_Array* v_mem
*/

Memory_Array* construct_Memory_Array()
{
	Memory_Array* v_mem = (Memory_Array*)malloc(sizeof(*v_mem));

	return v_mem;
}

/*
	Memory_Array의 메모리를 해제한다.
	Input : Memory_Array* memory_state
	Output : X
*/

void destory_Memory_Array(Memory_Array* memory_state)
{
	free(memory_state);
}

/*
	저장된 메모리 공간에서 A부터 B까지의 메모리 주소와 메모리 내용, 메모리 내용이 대응하는 ASCII code값을 형식에 맞게 그려준다.
	주소값은 최대 5자, 메모리 내용은 최대 2자, ASCII 값은 1자로 출력한다.
	Input : Memory_Array* memory_state, int a, int b
	Output : X
*/

void print_memory(Memory_Array* memory_state, int a, int b)
{
	int start = (a / 16) * 16;
	int end = (b / 16) * 16;							//Input을 바탕으로 출력하는 시작, 끝점 설정. (ASCII는 a,b가 아니라 해당 줄 전체를 출력해야하므로)
	for (int i = start; i <= end; i += 16)
	{
		printf("%05X ", i);								//주소값 출력
		for (int j = 0; j < 16; j++)
		{
			int k = i + j;
			if (k < a || k > b)
				printf("   ");							//출력대상이 아니므로 공백처리
			else
				printf("%02X ", memory_state->data[k].value);	//메모리값 출력
		}
		printf("; ");
		for (int j = 0; j < 16; j++)
		{
			int k = i + j;
			if (k < a || k > b)
				printf(".");							//출력 대상이 아니므로 . 으로 출력
			else
			{
				if (memory_state->data[k].value <= 0x7E && memory_state->data[k].value >= 0x20)	// ASCII 출력 범위 내라면
					printf("%c", memory_state->data[k].value);	//ASCII 값 출력
				else printf(".");
			}
		}
		printf("\n");
	}
}

/*
	메모리의 주소와 변경할 값을 입력받으면, 해당 주소의 메모리값을 주어진 값으로 변경한다.
	주어진 값이 0~FF 사이가 아니라면, 변경하지않는다.
	Input : Memory_Array* memory_state, int adress, short value
	Output : X
*/

void modify_memory(Memory_Array* memory_state, int address, short value)
{
	if (value > 0xFF)
		return;
	else if (value < 0)
		return;
	else
		memory_state->data[address].value = value;
}

/*
	1Mb 크기의 모든 메모리를 0으로 초기화한다.
	Input : Memory_Array* memory_state
	Output : X
*/

void reset_memory(Memory_Array* memory_state)
{
	for (int i = 0; i < 16*65536; i++)
		modify_memory(memory_state, i, 0);
}

//------------------------------------------------------------- OPcode 관련 함수 ---------------------------------------------

/*
	Opcode 구조체를 동적할당해 반환한다.
	Input : X
	Output : Opcode* opcode
*/

Opcode* construct_Opcode() 
{
	Opcode* opcode = (Opcode*)malloc(sizeof(Opcode));
	opcode->code = -1;
	return opcode;
}

/*
	Opcode 를 저장하는 링크드리스트의 배열, Opcode_Table을 동적 할당한다.
	링크드리스트의 주소값을 저장하는 배열인 Opcode_Table을 동적 할당한 후, 총 20개의 배열 각각에 대해
	Opcode를 갖는 링크드 리스트를 동적할당해준다.
	동적할당한 링크드리스트의 값을 초기화해준다.
	Input : X
	Output : Opcode_Table* table 
*/

Opcode_Table* construct_Opcode_Table()
{
	Opcode_Table* table = (Opcode_Table*)malloc(sizeof(Opcode_Table));
	table->list = (Opcode_List**)malloc(sizeof(Opcode_List*) * 20);
	for (int i = 0; i < 20; i++)
	{
		table->list[i] = (Opcode_List*)malloc(sizeof(Opcode_List));
		table->list[i]->head = NULL;
		table->list[i]->tail = NULL;
		table->list[i]->size = 0;
	}
	return table;
}

/*
	Opcode를 Opcode_Table에 삽입한다. 이때, 입력받는 hash_idx를 통해 어느 링크드리스트에 들어갈지 결정한다.
	해당 링크드리스트의 마지막으로 입력받은 opcode를 삽입한다.
	Input : Opcode_Table* table, Opcode* opcode, int hash_idx
	Output : X
*/

void insert_Opcode(Opcode_Table* table, Opcode* opcode, int hash_idx)
{
	Opcode_Node* opcode_node = (Opcode_Node*)malloc(sizeof(Opcode_Node));
	opcode_node->data = opcode;
	opcode_node->next = NULL;
	if (table->list[hash_idx]->head == NULL && table->list[hash_idx]->tail == NULL)
	{
		table->list[hash_idx]->head = opcode_node;
		table->list[hash_idx]->tail = opcode_node;
	}
	else
	{
		table->list[hash_idx]->tail->next = opcode_node;
		table->list[hash_idx]->tail = opcode_node;
	}
	table->list[hash_idx]->size++;
}

/*
	Opcode_Table에 찾고있는 Opcode가 있는지 확인하는 함수
	입력받은 문자열을 해시 한 뒤, 해당 해시 index에서 일치하는 opcode가 있는지 탐색한다.
	있다면, 해당 Opcode의 Code값을 반환하고, 없다면 -1을 반환한다.
	Input : Opcode_Table* table, char* command
	Output : int code / -1
*/

int find_Opcode_table(Opcode_Table* table, char* command)
{
	int hash_idx = command[0] % 20;
	Opcode_Node* p = table->list[hash_idx]->head;
	while (p != NULL)
	{
		if (strncmp(p->data->command, command, 50) == 0)
			return p->data->code;

		p = p->next;
	}
	return -1;
}

/*
	Opcode_Table의 모든 Opcode를 형식에 맞춰 출력한다.
	해시된 Opcode Table을 출력하기 때문에, 해시된 기준에 따라 순서대로 출력한다.
	Input : Opcode_Table* table
	Output : X
*/

void print_Opcode_table(Opcode_Table* table)
{
	for (int i = 0; i < 20; i++)
	{
		Opcode_Node* p = table->list[i]->head;
		printf("\n%d : ",i);
		while (p != NULL)
		{
			printf("[%s,%X]", p->data->command, p->data->code);
			p = p->next;
			if (p == NULL) break;
			printf(" -> ");
		}
	}
	printf("\n");
}

/*
	Opcode.txt 파일을 읽어들여서, 해당 파일의 모든 Opcode를 해시 후 Opcode_Table에 적절히 넣어
	Opcode_Table을 완성한다.
	입력받은 Opcode의 code값이 FF를 넘을경우 예외처리한다.
	Input : Opcode_Table* table
	Output : X
*/

void read_Opcode_Table(Opcode_Table* table)
{
	FILE* fp = fopen("opcode.txt", "rt");
	if (!fp)
	{
		printf("\nCannot open opcode file\n");
        return;
    }

	int code;
	char command[50];
	char format[50];

	while (fscanf(fp, "%X %6s %5s", &code, command, format) != EOF)
	{
		Opcode* opcode = construct_Opcode();
		strncpy(opcode->command, command, 50);
		strncpy(opcode->format, format, 50);
		opcode->code = code;
		if (opcode->code > 0xFF)
		{
			printf("\nInvalid input from Opcode_Table\n");
			continue;
		}

		//Hash
		insert_Opcode(table, opcode, opcode->command[0] % 20);
	}
}


//--------------------------------------------------------------------- state 관련 함수 ---------------------------------------

/*
	state 구조체 안의 history_state, memory_state, opcode_state를 각각 동적할당하고, 초기화한다.
	Input : X
	Output : State* state
*/

State* construct_state()
{
	State* state = (State*)malloc(sizeof(*state));

	state->history_state = construct_History_List();

	state->memory_state = construct_Memory_Array();
	reset_memory(state->memory_state);

	state->opcode_state = construct_Opcode_Table();
	read_Opcode_Table(state->opcode_state);

	return state;
}

//---------------------------------------------------------------------------------- dir

/*
	파일의 현재 directory를 출력한다.
	실행할 수 있는 파일이면 "~*"로 출력한다.
	디렉토리이면 "~/" 로 출력한다.
	그 이외에는 "~ " 로 출력한다.
	한줄에 최대 4개까지 출력한다.
	Input : X
	Output : X
*/


void print_dir() {
	int i=0;
	char path[1024];

	DIR* dir = opendir(".");
	struct dirent* ent;
	struct stat stat;

	char* name;
	char* format;

	if (!dir) 
		fprintf(stderr, "\n Directory Open Error\n");

	ent = readdir(dir);

	while (ent) 
	{
		name = ent->d_name;
		lstat(name, &stat);

		if (S_ISDIR(stat.st_mode)) 
			format = "%s/";
		else if (S_IXUSR & stat.st_mode) 
			format = "%s*";
		else 
			format = "%s ";

		sprintf(path, format, ent->d_name);
		printf("%-20s", path);

		if (++i % 4 == 0)
            printf("\n");
		ent = readdir(dir);
	}

	if (i % 4 != 0)
		printf("\n");

	closedir(dir);
}



//--------------------------------------------------------------------- util

/*
	해당하는 문자열이 16진수인지 확인한다.
	16진수라면 true, 아니라면 false를 반환한다.
	Input : char* string
	Output : true / false
*/

bool check_xdigit(char* string)
{
	int ch;
    int f = strlen(string);
	for (int i = 0; i < f; i++)
	{
		ch = string[i];
		if (isxdigit(ch) == 0)
			return false;
	}
	return true;
}

//---------------------------------------------------------- shell ----------------------------

/*
	SIC 에서 수행할수 있는 기능들을 표시한다.
*/

void function_help()
{
	printf("\nh[elp]\n");
	printf("d[ir]\n");
	printf("q[uit]\n");
	printf("hi[story]\n");
	printf("du[mp] [start, end]\n");
	printf("e[dit] address, value\n");
	printf("f[ill] start, end, value\n");
	printf("reset\n");
	printf("opcode mnemonic\n");
	printf("opcodelist\n\n");
}

/*
1. 입력을 토큰화함. ex) >[du    5  ,   3]    -> [du 5 3]
명령어는 token[0], 그외 최대 인자 3개까지 token[1], token[2], token[3]에 저장한다.
명령어가 올바른 형식 ex) dump AA , AB 인지 확인한다
2. 토큰[0] 을 기준으로, 문자열을 비교하며 어떤 명령인지 확인
3. 해당하는 명령에 맞는 기능을 수행한다.
4. 만약 기능중 q[uit] 기능을 수행한다면, true를 반환하고 아니면 false를 반환한다.
5. 잘못된 입력에 대한 예외처리를 수행한다.
*/

bool tokenize(State* state, char* command, int* dump_idx)
{
	int token_num = 0;
	char token[5][500] = { "", };
	char input[500];
	char tokenized[500] = "";
	strncpy(input, command, 500);
	int count = 0;
    int t=0;
	bool flag_1 = false;				//인자가 2개 이상일때 사이에 ,가 존재하는지 확인
	bool flag_2 = false;				//인자가 3개 이상일때 2,3번째 인자 사이에 ,가 존재하는지 확인
	bool flag_3 = false;				//마지막 인자 뒤에 ,가 없는지 확인

	char* ptr = strtok(input, " ,\t\n");//',' ' ' '\t' '\n'을 기준으로 tokenize한다.
	while (ptr != NULL)
	{
		if (token_num >= 5) break;
		strcpy(token[token_num++], ptr);
		ptr = strtok(NULL, " ,\t\n");
	}
	
	if (token_num == 4)					//fill A, B, C 인경우
	{
		ptr = strstr(command, token[1]);
        t = strlen(ptr);
		for (int i = 0; i < t; i++)
			if (ptr[i] == ',')
				count++;
		if (count == 2)
			flag_1 = true;                  //A "," B
		count = 0;

		ptr = strstr(command, token[2]);
        t = strlen(ptr);
		for (int i = 0; i < t; i++)
			if (ptr[i] == ',')
				count++;
		if (count == 1)
			flag_2 = true;                  //B "," C
		count = 0;

		ptr = strstr(command, token[3]);
        t = strlen(ptr);
		for (int i = 0; i < t; i++)
			if (ptr[i] == ',')
				count++;
		if (count == 0)
			flag_3 = true;                  //C !","

		if (!flag_1 || !flag_2 || !flag_3)      // !(fill A , B , C)
		{
			printf("Invalid Input Format\n");
			return false;
		}
	}
	
	else if (token_num == 3)			//dump AA, AB 인경우
	{
		ptr = strstr(command, token[1]);
        t = strlen(ptr);
		for (int i = 0; i < t; i++)
			if (ptr[i] == ',')
				count++;
		if (count == 1)
			flag_1 = true;
		count = 0;

		ptr = strstr(command, token[2]);
        t = strlen(ptr);
		for (int i = 0; i < t; i++)
			if (ptr[i] == ',')
				count++;
		if (count == 0)
			flag_2 = true;

		if (!flag_1 || !flag_2)
		{
			printf("Invalid Input Format\n");
			return false;
		}
	}

	if (strncmp(token[0], "h", 500) == 0 || strncmp(token[0], "help", 500) == 0)	// 첫번째 token이 h[elp] 라면
	{
		if (token_num == 1) 
		{
			function_help();
			History_push(state->history_state, token[0]);							//명령어 목록 출력, history에 명령어 저장
		}
		else
			printf("Wrong Input From h[elp]\n");
		return false;
	}

	
	else if (strncmp(token[0], "d", 500) == 0 || strncmp(token[0], "dir", 500) == 0)	//첫번째 token이 d[ir] 라면
	{
		if (token_num == 1) 
		{
		    print_dir();											
			History_push(state->history_state, token[0]);							//현재 디렉토리 출력, history에 명령어 저장
		}
		else
			printf("Wrong Input\n");
		return false;
	}
	

	else if (strncmp(token[0], "q", 500) == 0 || strncmp(token[0], "quit", 500) == 0)	//첫번째 token이 q[uit] 라면
	{
		if (token_num == 1)
			return true;
		else
			printf("Wrong Input From q[uit]\n");
		return false;
	}

	else if (strncmp(token[0], "hi", 500) == 0 || strncmp(token[0], "history", 500) == 0)	//첫번째 token이 hi[story] 라면
	{
		if (token_num == 1) 
		{
			History_push(state->history_state, token[0]);		//history에 명령어 입력 먼저!
			History_print(state->history_state);				//history 출력
		}
		else
			printf("Wrong Input From h[istory]\n");
		return false;
	}

	else if (strncmp(token[0], "du", 500) == 0 || strncmp(token[0], "dump", 500) == 0)		//첫번째 token이 du[mp] 라면
	{
		if (token_num == 1) // "dump" 하나만
		{
			print_memory(state->memory_state, *dump_idx, *dump_idx + 159);
			*dump_idx += 160;
			History_push(state->history_state, token[0]);
			return false;
		}

		if (check_xdigit(token[1]) == false) // 첫번째 인자 확인 (token[1])
		{
			printf("Wrong Input From du[mp] : first argument is invalid\n");
			return false;
		}

		//첫번째 인자를 16진수로 적절히 변환 및 예외처리
		int start;
		if (strncmp(token[1], "0", 500) == 0) start = 0;
		else if (strtol(token[1], NULL, 16) != 0)
			start = strtol(token[1], NULL, 16);

		if (token_num == 2 && start >= 0 && start < 16 * 65536) // "dump start" 라면
		{
			if (start > 16 * 65536 - 160)						//메모리 범위를 벗어나는지 확인
				print_memory(state->memory_state, start, 16 * 65536 - 1);
			else
				print_memory(state->memory_state, start, start + 159);

			strncat(tokenized, token[0], 500);
			strncat(tokenized, " ", 500);
			strncat(tokenized, token[1], 500);
			History_push(state->history_state, tokenized);		//token화 된 문자들을 합쳐 형식적인 명령어로 변환 후 history에 저장

			return false;
		}

		if (check_xdigit(token[2]) == false) // 두번째 인자 확인
		{
			printf("Wrong Input From du[mp] : second argument is invalid\n");
			return false;
		}

		//두번째 인자를 16진수로 적절히 변환 및 예외처리
		int end;
		if (strncmp(token[2], "0", 500) == 0) end = 0;
		else if (strtol(token[2], NULL, 16) != 0)
			end = strtol(token[2], NULL, 16);

		if (token_num == 3 && start >= 0 && end > 0 && end >= start && start < 16 * 65536 && end < 16 * 65536) // "dump start end" 라면
		{
			print_memory(state->memory_state, start, end);

			strncat(tokenized, token[0], 500);
			strncat(tokenized, " ", 500);
			strncat(tokenized, token[1], 500);
			strncat(tokenized, ", ", 500);
			strncat(tokenized, token[2], 500);
			History_push(state->history_state, tokenized);	//token화 된 문자들을 합쳐 형식적인 명령어로 변환 후 history에 저장

			return false;
		}

		printf("Wrong Input From du[mp] : arguments invalid\n");
		return false;
	}
	else if (strncmp(token[0], "e", 500) == 0 || strncmp(token[0], "edit", 500) == 0)	//첫번째 token이 e[dit] 라면
	{
		if (token_num == 3 && check_xdigit(token[1]) == true && check_xdigit(token[2]) == true)		//"edit address value"인지 확인
		{
			//16진수로 변환 및 예외처리
			int start = -1;
			int end = -1;
			if (strncmp(token[1], "0", 500) == 0) start = 0;
			else if (strtol(token[1], NULL, 16) != 0)
				start = strtol(token[1], NULL, 16);
			if (strncmp(token[2], "0", 500) == 0) end = 0;
			else if (strtol(token[2], NULL, 16) != 0)
				end = strtol(token[2], NULL, 16);
			if (start == -1 || end == -1) {
				printf("Wrong Input From e[dit]\n");
				return false;
			}

			modify_memory(state->memory_state, start, end);	// 해당 메모리값 변경

			strncat(tokenized, token[0], 500);
			strncat(tokenized, " ", 500);
			strncat(tokenized, token[1], 500);
			strncat(tokenized, ", ", 500);
			strncat(tokenized, token[2], 500);
			History_push(state->history_state, tokenized);	//token화 된 문자들을 합쳐 형식적인 명령어로 변환 후 history에 저장
		}
		else
			printf("Wrong Input From e[dit]\n");
        
        return false;
	}

	else if (strncmp(token[0], "f", 500) == 0 || strncmp(token[0], "fill", 500) == 0)	//첫번째 token이 f[ill] 라면
	{
		if (token_num == 4 && check_xdigit(token[1]) == true && check_xdigit(token[2]) == true && check_xdigit(token[3]) == true)	//"fill start end value"인지 확인, 전부 16진수인지 확인
		{
			//16진수로 변환 및 예외처리
			int start=-1, end=-1, value=-1;
			if (strncmp(token[1], "0", 500) == 0) start = 0;
			else if (strtol(token[1], NULL, 16) != 0)
				start = strtol(token[1], NULL, 16);
			if (strncmp(token[2], "0", 500) == 0) end = 0;
			else if (strtol(token[2], NULL, 16) != 0)
				end = strtol(token[2], NULL, 16);
			if (strncmp(token[3], "0", 500) == 0) value = 0;
			else if (strtol(token[3], NULL, 16) != 0)
				value = strtol(token[3], NULL, 16);
			
			for (int i = start; i <= end; i++) {
				modify_memory(state->memory_state, i, value);	//start 부터 end까지 메모리 값 변경
			}

			strncat(tokenized, token[0], 500);
			strncat(tokenized, " ", 500);
			strncat(tokenized, token[1], 500);
			strncat(tokenized, ", ", 500);
			strncat(tokenized, token[2], 500);
			strncat(tokenized, ", ", 500);
			strncat(tokenized, token[3], 500);
			History_push(state->history_state, tokenized);	//token화 된 문자들을 합쳐 형식적인 명령어로 변환 후 history에 저장
		}
        else
			printf("Wrong Input From f[ill]\n");
        
        return false;
	}

	else if (strncmp(token[0], "reset", 500) == 0)	//첫번째 token이 reset 라면
	{
		reset_memory(state->memory_state);
		strncat(tokenized, token[0], 500);
		History_push(state->history_state, tokenized);
        return false;
	}

	else if (strncmp(token[0], "opcode", 500) == 0)	//첫번째 token이 opcode 라면
	{
		if (token_num == 2)	//"opcode command" 인지 확인
		{
			int t = find_Opcode_table(state->opcode_state, token[1]);	//입력받은 command가 Opcode table에 있는지 확인
			if (t == -1)
			{
				printf("Cannot find searching OPCODE\n");
			}
			else
			{
				printf("opcode is %X\n", t);
				strncat(tokenized, token[0], 500);
				strncat(tokenized, " ", 500);
				strncat(tokenized, token[1], 500);
				History_push(state->history_state, tokenized);	//token화 된 문자들을 합쳐 형식적인 명령어로 변환 후 history에 저장
			}
            return false;
		}
        
        printf("Wrong Input foramt from Opcode\n");
        return false;
	}
			
	else if (strncmp(token[0], "opcodelist", 500) == 0) //첫번째 token이 opcodelist 라면
	{
		print_Opcode_table(state->opcode_state);
		printf("\n");
		strncat(tokenized, token[0], 500);
		History_push(state->history_state, tokenized);
		return false;
	}

	printf("Invalid Input\n");
	return false;
}

/*
	프로그램을 종료하기 전 State 변수 내부의 모든 동적할당된 메모리를 해제한다.
	Input : State* state
	Output : X
*/

void dealloc_memory(State* state)
{
    History_Node* ptr = state->history_state->head;
    History_Node* bptr;
    while (ptr != NULL)
    {
        bptr = ptr;
        ptr = ptr->next;
        free(bptr);
    }
    free(state->history_state);
    free(state->memory_state);
    
    Opcode_Node* nptr;
    Opcode_Node* bnptr;
    for(int i=0; i<20; i++)
    {
        nptr = state->opcode_state->list[i]->head;
        while(nptr != NULL)
        {
            bnptr = nptr;
            nptr = nptr->next;
            free(bnptr);
        }
        free(state->opcode_state->list[i]);
    }
    free(state->opcode_state);
    free(state);
    printf("Deallocate Success\n");
}

/*
	sicsim> 출력후 사용자의 입력에 따라 적절한 처리를한다.
	만약 q[uit]라면, 종료한다.
	Input : State* state
	Output : X
*/

void shell_main(State* state)
{
	char input[500];
	int dump_idx = 0;
	bool flag = false;
	while (1)
	{
		printf("sicsim> ");
		fgets(input, 501, stdin);
		flag = tokenize(state, input, &dump_idx);
		if (flag)
			return;
	}
}

int main()
{
	// construct data structure
	State* state = construct_state();
	
	// execute command shell
	shell_main(state);
    
    // dealloc memory
    dealloc_memory(state);

	return 0;
}

