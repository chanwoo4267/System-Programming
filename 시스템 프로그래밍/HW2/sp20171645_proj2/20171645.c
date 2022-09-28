#define _CRT_SECURE_NO_WARNINGS
#include "20171645.h"

//제출전 "수정" 확인, "test" 삭제 , 보충 추가작성
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
    strncpy(input->data, data, MAX_LEN);
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
        printf("%d\t%s\n", count, p->data);
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
    int end = (b / 16) * 16;                            //Input을 바탕으로 출력하는 시작, 끝점 설정. (ASCII는 a,b가 아니라 해당 줄 전체를 출력해야하므로)
    for (int i = start; i <= end; i += 16)
    {
        printf("%05X ", i);                                //주소값 출력
        for (int j = 0; j < 16; j++)
        {
            int k = i + j;
            if (k < a || k > b)
                printf("   ");                            //출력대상이 아니므로 공백처리
            else
                printf("%02X ", memory_state->data[k].value);    //메모리값 출력
        }
        printf("; ");
        for (int j = 0; j < 16; j++)
        {
            int k = i + j;
            if (k < a || k > b)
                printf(".");                            //출력 대상이 아니므로 . 으로 출력
            else
            {
                if (memory_state->data[k].value <= 0x7E && memory_state->data[k].value >= 0x20)    // ASCII 출력 범위 내라면
                    printf("%c", memory_state->data[k].value);    //ASCII 값 출력
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
    for (int i = 0; i < 16 * 65536; i++)
        modify_memory(memory_state, i, 0);
}

//------------------------------------------------------------- Symbol Table, Assemble Table 관련 함수 ---------------------------------------------

/*
    Assemble 데이터를 저장하는 링크드 리스트 형태의 구조체를 동적할당해 반환한다.
    동적할당한 후 내용값들을 초기값 (0, NULL, NULL)로 초기화한다.
    Input : X
    Output : Assmeble_Table* asstab
*/

Assemble_Table* construct_Assemble_Table()
{
    Assemble_Table* asstab = (Assemble_Table*)malloc(sizeof(Assemble_Table));
    asstab->len = 0;
    asstab->head = NULL;
    asstab->tail = NULL;
    return asstab;
}

/*
    Assemble Table에 새로운 Assemble Node를 추가한다.
    Input : Assemble_Table* asstab, bool is_comment, int line_num, int loc, char* label, char* instruction, char* operand, char* object_code, int format
    Output : X
*/

void push_Assemble_Table(Assemble_Table* asstab, bool is_comment, int line_num, int loc, char* label, char* instruction, char* operand, char* object_code, int format)
{
    Assemble_Node* in = (Assemble_Node*)malloc(sizeof(Assemble_Node));

    if (is_comment == true)
        in->is_comment = true;
    else
        in->is_comment = false;
    in->line_num = line_num;
    strncpy(in->instruction, instruction, MAX_LEN);
    strncpy(in->label, label, MAX_LEN);
    strncpy(in->operand, operand, MAX_LEN);
    in->loc = loc;
    in->format = format;
    strncpy(in->objectcode, object_code, 9);
    in->next = NULL;

    if (asstab->head == NULL && asstab->tail == NULL)
    {
        asstab->head = in;
        asstab->tail = in;
    }
    else
    {
        asstab->tail->next = in;
        asstab->tail = in;
    }
}

/*
    Symbol 데이터를 저장하는 링크드 리스트 형태의 구조체를 동적할당해 반환한다.
    동적할당한 후 내용값들을 초기값 (0, NULL, NULL)로 초기화한다.
    Input : X
    Output : Symbol_Table* symtab
*/

Symbol_Table* construct_Symbol_Table()
{
    Symbol_Table* symtab = (Symbol_Table*)malloc(sizeof(Symbol_Table));
    symtab->size = 0;
    symtab->head = NULL;
    symtab->tail = NULL;
    return symtab;
}

/*
    Symbol Table에 새로운 symbol node 데이터를 추가한다.
    Input : Symbol_Table* symtab, char* symbol, int loc
    Output : X
*/

void push_Symbol_Table(Symbol_Table* symtab, char* symbol, int loc)
{
    Symbol_Node* in = (Symbol_Node*)malloc(sizeof(Symbol_Node));
    strncpy(in->symbol, symbol, MAX_LEN);
    in->next = NULL;
    in->loc = loc;
    Symbol_Node* fp = symtab->head;
    // NULL이면
    if (symtab->head == NULL && symtab->tail == NULL)
    {
        symtab->head = in;
        symtab->tail = in;
    }
    else if (strncmp(in->symbol, fp->symbol, MAX_LEN) < 0)
    {
        in->next = fp;
        symtab->head = in;
    }
    else
    {
        while (1)
        {
            if (fp->next == NULL)
            {
                fp->next = in;
                symtab->tail = in;
                break;
            }
            if (strncmp(in->symbol, fp->symbol, MAX_LEN) > 0 && strncmp(in->symbol, fp->next->symbol, MAX_LEN) <= 0)
            {
                in->next = fp->next;
                fp->next = in;
                break;
            }
            fp = fp->next;
        }
    }
    symtab->size++;
}

/*
    Symbol Table에서 입력받은 Symbol이 있는지 탐색한다.
    찾았다면 해당 Symbol의 loc 값을 return하고, 없다면 -1을 return한다.
    Input : char* symbol
    Output : loc / -1
*/

int search_Symbol_Table(Symbol_Table* symtab, char* symbol)
{
    Symbol_Node* fp = symtab->head;
    while (fp != NULL)
    {
        if (strncmp(fp->symbol, symbol, MAX_LEN) == 0)
            return fp->loc;

        fp = fp->next;
    }
    return -1;
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
        if (strncmp(p->data->command, command, MAX_LEN) == 0)
            return p->data->code;
        p = p->next;
    }
    return -1;
}

/*
    Opcode_Table에 찾고있는 Opcode가 있는지 확인하는 함수
    입력받은 문자열을 해시 한 뒤, 해당 해시 index에서 일치하는 opcode가 있는지 탐색한다.
    있다면, 해당 Opcode의 FORMAT값을 반환하고, 없다면 "EOF"을 반환한다.
    Input : Opcode_Table* table, char* command
    Output : char* format / "EOF"
*/

char* return_Opcode_format(Opcode_Table* table, char* command)
{
    int hash_idx = command[0] % 20;
    Opcode_Node* p = table->list[hash_idx]->head;
    while (p != NULL)
    {
        if (strncmp(p->data->command, command, MAX_LEN) == 0)
            return p->data->format;
        p = p->next;
    }
    return "EOF";
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
        printf("\n%d : ", i);
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
        printf("Cannot open opcode file\n");
        exit(0);
    }

    int code;
    char command[MAX_LEN];
    char format[MAX_LEN];

    while (fscanf(fp, "%X %6s %5s", &code, command, format) != EOF)
    {
        Opcode* opcode = construct_Opcode();
        strncpy(opcode->command, command, MAX_LEN);
        strncpy(opcode->format, format, MAX_LEN);
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

    state->symtab_state = construct_Symbol_Table();

    state->assemble_state = construct_Assemble_Table();

    state->final_symbol_table = NULL;
    state->final_assemble_table = NULL;

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
    int i = 0;
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
    printf("opcodelist\n");
    printf("assemble filename\n");
    printf("type filename\n");
    printf("symbol\n\n");
}

/*
    file의 내용을 형식에 맞게 출력한다.
    Input : char* file
    Output : true, false
*/

bool type_file(char* file)
{
    FILE* fp = fopen(file, "rt");
    char buffer[1024];

    if (!fp)
    {
        printf("Cannot Open File : Invalid File Name\n");
        return false;
    }

    while (fgets(buffer, 1024, fp)) //sizeof
        fputs(buffer, stdout);

    fclose(fp);
    return true;
}

/*
    찾는 Opcode의 foramt이 1, 2, 3/4인지 구분하고 이를 return한다.
    Input : State* state, char* opcode
    Output : 1,2,3
*/

int check_format(State* state, char* opcode)
{
    char* p = return_Opcode_format(state->opcode_state, opcode);
    if (strncmp(p, "1", MAX_LEN) == 0) //Instruction size 1
        return 1;
    else if (strncmp(p, "2", MAX_LEN) == 0) // 2
        return 2;
    else if (strncmp(p, "3/4", MAX_LEN) == 0) // 3/4
        return 3;
    else
        return 0;
}

//------------------------------------------------------------- Assemble 관련 함수 ---------------------------------------------

/*
    .asm 확장자를 갖는 파일을 입력받아, PASS2 과정을 통해 해당 내용을 해석해 .lst 파일과 .obj 파일로 변환한 후 저장한다.
    올바른 파일이 아닐경우 예외처리하고, 처리중 오류가 생길경우 오류가 생긴 줄을 출력하고 예외처리한다.
    제대로 변환이 끝난 경우에만 .lst 파일과 .obj 파일을 저장한다.
    Input : State* state, char* opcode
    Output : 1,2,3
*/


bool assemble(State* state, char* filename)
{
    //file type error
    if (strlen(filename) < 5 || strncmp(&filename[strlen(filename) - 3], "asm", 3) != 0)
    {
        printf("File Format Error, input .asm\n");
        return false;
    }

    FILE* fp = fopen(filename, "r");

    if (!fp)
    {
        printf("Cannot open assemble file\n");
        return false;
    }

    bool end_flag = false;  //END 명령을 지났는지 확인
    char* templine;         //tokenize 과정에 필요
    char temp[MAX_LEN];         //tokenize 과정에 필요
    char comment[MAX_LEN];      //comment line일경우 tokenize하지않은 원본을 저장
    char token[4][MAX_LEN];     //해당 줄을 token화함
    int token_num = 0;          //tokenize 후 갯수 저장
    int locadd = 0;             //해당 줄의 format
    int line_num = 5;           //line num을 저장
    int locctr = 0;             //local counter을 저장

    while (!feof(fp) && !end_flag)
    {
        // initialize
        for (int i = 0; i < 4; i++)
            strncpy(token[i], "", MAX_LEN);
        token_num = 0;
        strncpy(temp, "", MAX_LEN);
        locadd = 0;

        // 파일로부터 한줄을 입력받아 토큰화한다.
        templine = fgets(temp, MAX_LEN, fp);
        strncpy(comment, temp, MAX_LEN);

        char* ptr = strtok(templine, " \t\n");
        if (ptr[0] != '.')
        {
            while (ptr != NULL)
            {
                if (token_num >= 5) break;
                strncpy(token[token_num++], ptr, 500);
                ptr = strtok(NULL, " \t\n");
            }
        }
        else //comment일경우
        {
            ptr = strtok(comment, "\t\n");
            push_Assemble_Table(state->assemble_state, true, line_num, -1, "NULL", comment, "NULL", "NULL", 0);
            line_num += 5;
            continue;
        }

        //잘린 토큰 다시 operand에 붙여주기 (operand에 두가지 입력이 오는경우 ex) LDCH BUFFER, X )
        if (token[2][strlen(token[2]) - 1] == ',' && token[3][0] != '\0') {
            strncat(token[2], token[3], strlen(token[3]));
            token_num--;
        }
        else if (token[1][strlen(token[1]) - 1] == ',' && token[2][0] != '\0') {
            strncat(token[1], token[2], strlen(token[2]));
            token_num--;
        }

        // START일경우
        if (strncmp(token[1], "START", MAX_LEN) == 0) // START
        {
            locctr = atoi(token[2]);
            push_Assemble_Table(state->assemble_state, false, line_num, locctr, token[0], token[1], token[2], "NULL", 0);
            line_num += 5;
            continue;
        }
        //END일경우 Line_Num만 붙여서 저장
        else if (strncmp(token[0], "END", MAX_LEN) == 0)
        {
            push_Assemble_Table(state->assemble_state, false, line_num, -1, "NULL", token[0], token[1], "NULL", 0);
            state->assemble_state->len = locctr;
            line_num += 5;
            end_flag = true;
            continue;
        }
        //BASE 일경우
        else if (strncmp(token[0], "BASE", MAX_LEN) == 0)
        {
            push_Assemble_Table(state->assemble_state, false, line_num, -1, "NULL", token[0], token[1], "NULL", 0);
            line_num += 5;
            end_flag = false;
            continue;
        }
        else
        {
            //첫번째 인자가 opcode라면
            //extended 여부 체크
            if (token[0][0] == '+') {
                locadd = 4;
                strncpy(temp, token[0] + 1, MAX_LEN);
            }
            else 
            {
                strncpy(temp, token[0], MAX_LEN);
                locadd = check_format(state, token[0]);
            }
            if (find_Opcode_table(state->opcode_state, temp) != -1 && locadd != 0)
            {
                push_Assemble_Table(state->assemble_state, false, line_num, locctr, "NULL", token[0], token[1], "NULL", locadd);
                locctr += locadd;
                line_num += 5;
                continue;
            } //opcode 끝

            // 첫번째 인자가 Symbol 이라면
            if (search_Symbol_Table(state->symtab_state, token[0]) == -1) // Symbol table에 없는 symbol이라면
            {
                // WORD, BYTE, RESW, RESB 일 경우
                if (strncmp(token[1], "WORD", MAX_LEN) == 0)
                    locadd = 3;
                else if (strncmp(token[1], "BYTE", MAX_LEN) == 0)
                {
                    if (token[2][0] == 'C')
                        locadd = strlen(token[2]) - 3;
                    else if (token[2][0] == 'X')
                        locadd = (strlen(token[2]) - 3) / 2;
                    else {
                        printf("BYTE type ERROR : Line %d\n", line_num);
                        return false;
                    }
                }
                else if (strncmp(token[1], "RESW", MAX_LEN) == 0)
                    locadd = atoi(token[2]) * 3;
                else if (strncmp(token[1], "RESB", MAX_LEN) == 0)
                    locadd = atoi(token[2]);

                //extended 일경우
                else if (token[1][0] == '+') {
                    locadd = 4;
                    strncpy(temp, token[1] + 1, MAX_LEN);
                }
                else
                {
                    strncpy(temp, token[1], MAX_LEN);
                    locadd = check_format(state, token[1]);
                }
                //적절한 opcode라면
                if (find_Opcode_table(state->opcode_state, temp) != -1 || strncmp(token[1], "RESB", MAX_LEN) == 0 || strncmp(token[1], "RESW", MAX_LEN) == 0 || strncmp(token[1], "BYTE", MAX_LEN) == 0 || strncmp(token[1], "WORD", MAX_LEN) == 0)
                {
                    push_Symbol_Table(state->symtab_state, token[0], locctr);
                    push_Assemble_Table(state->assemble_state, false, line_num, locctr, token[0], token[1], token[2], "NULL", locadd);

                    locctr += locadd;
                    line_num += 5;
                    continue;
                }
                else
                {
                    printf("Opcode Error : Line %d\n", line_num);
                    return false;
                }
            } //symbol 끝

            else    //  Instruction ERROR
            {       // 1. RESB, RESW, WORD, BYTE 앞에 Label이 존재하지 않음. 2. 중복된 Lable 값 3. Invalid Instruction Input
                printf("Instruction ERROR : Line %d\n", line_num);
                return false;
            }
        }

    } //end of file reading
    //end of PASS1

    // start PASS2
    // list 파일 제작, Object Code Construct
    Assemble_Node* an = state->assemble_state->head;
    int base = 0;               //base값을 저장

    //object code 생성 및 저장
    while (an != NULL)
    {
        strncpy(temp, "", MAX_LEN);
        char temp_in[MAX_LEN];              
        char temp_op[MAX_LEN] = "";
        int opc;            
        char hex[10];
        int symloc;
        int xbpe = 0;
        //해당 줄이 START, END, RESW, RESB, 혹은 comment라면 다음줄로 넘어감
        if (strncmp(an->instruction, "START", MAX_LEN) == 0 || strncmp(an->instruction, "END", MAX_LEN) == 0 || strncmp(an->instruction, "RESW", MAX_LEN) == 0 || strncmp(an->instruction, "RESB", MAX_LEN) == 0 || an->is_comment == true)
        {
            strncpy(an->objectcode, "NULL", 9);
            an = an->next;
            continue;
        }
        //해당 줄이 BYTE라면
        if (strncmp(an->instruction, "BYTE", MAX_LEN) == 0)
        {
            if (an->operand[0] == 'C') {
                int len = strlen(an->operand) - 3;
                for (int i = 0; i < len; i++)
                {
                    sprintf(hex, "%X", (int)an->operand[i + 2]);
                    strncat(temp, hex, 2);
                }
            }
            else {
                int len = (strlen(an->operand) - 3);
                for (int i = 0; i < len; i++)
                    temp[i] = an->operand[i + 2];
            }
            strncpy(an->objectcode, "", 9);
            strncpy(an->objectcode, temp, strlen(temp));
            an = an->next;
            continue;
        }
        //해당 줄이 WORD라면
        if (strncmp(an->instruction, "WORD", MAX_LEN) == 0)
        {
            sprintf(hex, "%X", atoi(an->operand));
            strncpy(an->objectcode, "", 9);
            strncpy(an->objectcode, hex, 9);
            an = an->next;
            continue;
        }
        //해당 줄이 BASE라면
        if (strncmp(an->instruction, "BASE", MAX_LEN) == 0)
        {
            base = search_Symbol_Table(state->symtab_state, an->operand);
            an = an->next;
            continue;
        }
        //해당 줄의 Opcode의 format이 1이라면, project2에서는 고려안함
        if (an->format == 1) 
        {
            an = an->next;
            continue;
        }
        //해당 줄의 Opcode의 format이 2이라면,
        if (an->format == 2)
        {
            opc = find_Opcode_table(state->opcode_state, an->instruction);
            int f, s = 0;
            sprintf(hex, "%X", opc);
            strncat(temp, hex, MAX_LEN);
            if (an->operand[0] == 'A')
                f = 0;
            else if (an->operand[0] == 'X')
                f = 1;
            else if (an->operand[0] == 'L')
                f = 2;
            else if (an->operand[0] == 'P' && an->operand[1] == 'C')
                f = 8;
            else if (an->operand[0] == 'S' && an->operand[1] == 'W')
                f = 9;
            else if (an->operand[0] == 'B')
                f = 3;
            else if (an->operand[0] == 'S')
                f = 4;
            else if (an->operand[0] == 'T')
                f = 5;
            else if (an->operand[0] == 'F')
                f = 6;

            if (strlen(an->operand) > 1)
            {
                if (an->operand[strlen(an->operand) - 1] == 'A')
                    s = 0;
                else if (an->operand[strlen(an->operand) - 1] == 'X')
                    s = 1;
                else if (an->operand[strlen(an->operand) - 1] == 'L')
                    s = 2;
                else if (an->operand[strlen(an->operand) - 1] == 'C' && an->operand[strlen(an->operand) - 2] == 'P')
                    s = 8;
                else if (an->operand[strlen(an->operand) - 1] == 'W' && an->operand[strlen(an->operand) - 2] == 'S')
                    s = 9;
                else if (an->operand[strlen(an->operand) - 1] == 'B')
                    s = 3;
                else if (an->operand[strlen(an->operand) - 1] == 'S')
                    s = 4;
                else if (an->operand[strlen(an->operand) - 1] == 'T')
                    s = 5;
                else if (an->operand[strlen(an->operand) - 1] == 'F')
                    s = 6;
            }
            sprintf(hex, "%X", f);
            strncat(temp, hex, MAX_LEN);
            sprintf(hex, "%X", s);
            strncat(temp, hex, MAX_LEN);
            strncpy(an->objectcode, "", 9);
            strncpy(an->objectcode, temp, strlen(temp));
            an = an->next;
            continue;
        }

        //extended일경우, +를 빼주는 작업 수행
        if (an->instruction[0] == '+')
            strncpy(temp_in, an->instruction + 1, MAX_LEN);
        else strncpy(temp_in, an->instruction, MAX_LEN);
        opc = find_Opcode_table(state->opcode_state, temp_in);

        if (opc != -1)                      //opcode가 존재하는 instruction이면
        {
            if (an->operand[0] == '#')      // imediate address, ni = 01
                sprintf(hex, "%X", opc + 1);  // ni = 01
            else if (an->operand[0] == '@') //indirect address, ni = 10
                sprintf(hex, "%X", opc + 2);  // ni = 10
            else                            //simple address
                sprintf(hex, "%X", opc + 3);  // ni = 11

            strncat(temp, hex, MAX_LEN);      //insert opcode + ni

            // #이나 @가 붙어있다면, 이를 제거
            if (an->operand[0] == '#' || an->operand[0] == '@')
                strncpy(temp_op, an->operand + 1, MAX_LEN);
            else strncpy(temp_op, an->operand, MAX_LEN);
            if (temp_op[strlen(temp_op) - 1] == 'X')
            {
                xbpe += 8;
                char t[MAX_LEN] = "";
                strncpy(t, temp_op, MAX_LEN);
                strncpy(temp_op, t, strlen(t) - 3);
                temp_op[strlen(t) - 2] = '\0';
            }

            symloc = search_Symbol_Table(state->symtab_state, temp_op);
            if (symloc == -1) // operand가 #상수 혹은 없을경우
            {
                if (check_xdigit(temp_op) == true) // #상수
                {
                    if (an->instruction[0] == '+') {    //extended라면
                        sprintf(hex, "%X", 1);
                        strncat(temp, hex, MAX_LEN);
                        sprintf(hex, "%05X", atoi(temp_op));
                        strncat(temp, hex, MAX_LEN);
                    }
                    else {
                        sprintf(hex, "%X", 0);
                        strncat(temp, hex, MAX_LEN);
                        sprintf(hex, "%03X", atoi(temp_op));
                        strncat(temp, hex, MAX_LEN);
                    }
                }
                else if (strncmp(temp_op, "", MAX_LEN) == 0)
                {
                    strncat(temp, "0000", MAX_LEN);
                }
                else
                {
                    printf("ERROR Line %d\n", an->line_num);
                    return false;
                }
                strncpy(an->objectcode, "", 9);
                strncpy(an->objectcode, temp, strlen(temp));
                an = an->next;
                continue;
            }
            //해당 줄의 Opcode이 extended 라면
            if (an->instruction[0] == '+') 
            {
                sprintf(hex, "%X", 1);
                strncat(temp, hex, MAX_LEN);
                sprintf(hex, "%05X", symloc);
                strncat(temp, hex, MAX_LEN);
                strncpy(an->objectcode, "", 9);
                strncpy(an->objectcode, temp, strlen(temp));
                an = an->next;
                continue;
            }
            //symbol table에 없다면
            if (symloc == -1) {
                printf("Symbol Table search ERROR Line %d\n", an->line_num);
                return false;
            }
            //pc relative range 안에 들어간다면
            if (abs(symloc - (an->loc + an->format)) < 2048 || symloc - an->loc - an->format == -2048) {     
                sprintf(hex, "%X", xbpe + 2);
                strncat(temp, hex, MAX_LEN);
                sprintf(hex, "%03X", symloc - (an->loc + an->format));
                strncat(temp, hex + (strlen(hex) - 3), MAX_LEN);
            }
            //base relative range안에 들어간다면
            else if (symloc - base < 4096 && symloc - base > -1)
            {
                sprintf(hex, "%X", xbpe + 4);
                strncat(temp, hex, MAX_LEN);
                sprintf(hex, "%03X", symloc - base);
                strncat(temp, hex, MAX_LEN);
            }

            strncpy(an->objectcode, "", 9);
            strncpy(an->objectcode, temp, 9);
            an = an->next;
            continue;
        }
        //예외처리
        printf("ERROR on line %d\n", an->line_num);
        return false;
    }
    //end of construct listing file

    //save .lst File
    char filenamet[MAX_LEN] = "";
    char filenames[MAX_LEN] = "";
    strncpy(filenamet, filename, strlen(filename) - 3);
    filenamet[strlen(filenamet) - 1] = '\0';
    strncpy(filenames, filenamet, MAX_LEN);
    strncat(filenamet, ".lst", 4);              //filename : filename.lst
    strncat(filenames, ".obj", 4);              //filename : filename.obj
    FILE* sp = fopen(filenamet, "w");
    Assemble_Node* fpp = state->assemble_state->head;
    while (fpp != NULL)
    {
        if (fpp->is_comment)            //comment일경우
        {
            fprintf(sp, "%d\t    \t%s\n", fpp->line_num, fpp->instruction);
        }
        else        // Linenum, Loc, Label, Instruction, Operand, Objectcode 순으로 출력
        {
            fprintf(sp, "%d\t", fpp->line_num);

            if (fpp->loc != -1)
                fprintf(sp, "%04X\t", fpp->loc);
            else
                fprintf(sp, "    \t");

            if (strncmp(fpp->label, "NULL", MAX_LEN) != 0)
                fprintf(sp, "%s\t", fpp->label);
            else
                fprintf(sp, "\t");

            fprintf(sp, "%s\t%-15s\t", fpp->instruction, fpp->operand);

            if (strncmp(fpp->objectcode, "NULL", 9) != 0)
                fprintf(sp, "\t%s", fpp->objectcode);

            fprintf(sp, "\n");
        }
        fpp = fpp->next;
    }
    //end of creating lst file
    //start creating obj file

    strncpy(temp, "", MAX_LEN);
    fpp = state->assemble_state->head;
    FILE* op = fopen(filenames, "w");

    //H 출력

    fprintf(op, "H%-6s", fpp->label);
    for (int i = 0; i < 6 - strlen(fpp->operand); i++)
        fprintf(op, "0");
    fprintf(op, "%s%06X\n", fpp->operand, state->assemble_state->len);

    fpp = fpp->next;

    bool is_first = true;               //T 출력 여부
    bool is_print = false;              //출력 시작 확인 flag
    int line_len = 0;                   //한줄의 길이,길이가 1E를 넘는지 확인
    int count = 0;                      //M 출력 위치 확인
    int counter[1000] = { 0, };         //M 출력 위치 저장
    int idx = 0;                        //counter 의 index
    strncpy(temp, "", MAX_LEN);

    while (fpp != NULL && strncmp(fpp->instruction, "END", MAX_LEN) != 0)       //문자열 temp에 범위를 벗어나지 않을때까지 objectcode를 계속 덧붙이다, RESW, RESB가 나오거나 범위를 벗어나면 한번에 출력
    {
        if (fpp->is_comment)        //comment라면 무시
        {
            fpp = fpp->next;
            continue;
        }
        if (is_first) {             //줄바꿈 직후라면
            fprintf(op, "T%06X", fpp->loc);         //T와 현재줄의 loc 출력

            is_first = false;
        }
        if (is_print)               //형식에 맞춰 출력
        {
            fprintf(op, "%02X%s\n", line_len,temp);
            line_len = 0;
            strncpy(temp, "", MAX_LEN);
            is_first = true;
            is_print = false;
            continue;
        }

        if (strncmp(fpp->instruction, "RESB", MAX_LEN) == 0 || strncmp(fpp->instruction, "RESB", MAX_LEN) == 0)     //RESB,l RESW면 출력
            is_print = true;
        if (strncmp(fpp->objectcode, "NULL", MAX_LEN) == 0) //Objectcode가 없다면, 무시
        {
            fpp = fpp->next;
            continue;
        }
        
        for (int i = 0; i < fpp->format * 2 - strlen(fpp->objectcode); i++)
            strncat(temp, "0", MAX_LEN);
        strncat(temp, fpp->objectcode, MAX_LEN);
        line_len += fpp->format;
        count += fpp->format;
        if (fpp->format == 4)       //format4라면
        {
            if (fpp->operand[0] == '#' || fpp->operand[0] == '@')   
            {
                if (!check_xdigit(fpp->operand + 1))    //operand가 상수가 아니라면, Modification
                {
                    counter[idx] = count;
                    idx++;
                }
            }
            else
                if (!check_xdigit(fpp->operand))        //operand가 상수가 아니라면, Modification
                {
                    counter[idx] = count;
                    idx++;
                }
        }
        fpp = fpp->next;
        if (line_len < 31 && line_len + fpp->format >= 31)      //길이가 1E가 넘으면
            is_print = true;
    }

    fprintf(op, "%02X%s\n", line_len, temp);    //그동안 덧붙인 objectcode들을 출력

    //M 출력
    for (int i = 0; i < 1000; i++)
        if (counter[i] != 0)
            fprintf(op, "M%06X05\n", counter[i] - 3);
    
    //E 출력
    fprintf(op, "E%06X\n", state->assemble_state->head->loc);

    fclose(fp);
    fclose(sp);
    fclose(op);
    return true;
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
    char token[5][MAX_LEN] = { "", };
    char input[MAX_LEN];
    char tokenized[MAX_LEN] = "";
    strncpy(input, command, MAX_LEN);
    int count = 0;
    int t = 0;
    bool flag_1 = false;                //인자가 2개 이상일때 사이에 ,가 존재하는지 확인
    bool flag_2 = false;                //인자가 3개 이상일때 2,3번째 인자 사이에 ,가 존재하는지 확인
    bool flag_3 = false;                //마지막 인자 뒤에 ,가 없는지 확인

    char* ptr = strtok(input, " ,\t\n");//',' ' ' '\t' '\n'을 기준으로 tokenize한다.
    while (ptr != NULL)
    {
        if (token_num >= 5) break;
        strcpy(token[token_num++], ptr);
        ptr = strtok(NULL, " ,\t\n");
    }

    if (token_num == 4)                    //fill A, B, C 인경우
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

    else if (token_num == 3)            //dump AA, AB 인경우
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

    if (strncmp(token[0], "h", MAX_LEN) == 0 || strncmp(token[0], "help", MAX_LEN) == 0)    // 첫번째 token이 h[elp] 라면
    {
        if (token_num == 1)
        {
            function_help();
            History_push(state->history_state, token[0]);                            //명령어 목록 출력, history에 명령어 저장
        }
        else
            printf("Wrong Input From h[elp]\n");
        return false;
    }


    else if (strncmp(token[0], "d", MAX_LEN) == 0 || strncmp(token[0], "dir", MAX_LEN) == 0)    //첫번째 token이 d[ir] 라면
    {
        if (token_num == 1)
        {
            print_dir();
            History_push(state->history_state, token[0]);                            //현재 디렉토리 출력, history에 명령어 저장
        }
        else
            printf("Wrong Input\n");
        return false;
    }


    else if (strncmp(token[0], "q", MAX_LEN) == 0 || strncmp(token[0], "quit", MAX_LEN) == 0)    //첫번째 token이 q[uit] 라면
    {
        if (token_num == 1)
            return true;
        else
            printf("Wrong Input From q[uit]\n");
        return false;
    }

    else if (strncmp(token[0], "hi", MAX_LEN) == 0 || strncmp(token[0], "history", MAX_LEN) == 0)    //첫번째 token이 hi[story] 라면
    {
        if (token_num == 1)
        {
            History_push(state->history_state, token[0]);        //history에 명령어 입력 먼저!
            History_print(state->history_state);                //history 출력
        }
        else
            printf("Wrong Input From h[istory]\n");
        return false;
    }

    else if (strncmp(token[0], "du", MAX_LEN) == 0 || strncmp(token[0], "dump", MAX_LEN) == 0)        //첫번째 token이 du[mp] 라면
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
        if (strncmp(token[1], "0", MAX_LEN) == 0) start = 0;
        else if (strtol(token[1], NULL, 16) != 0)
            start = strtol(token[1], NULL, 16);

        if (token_num == 2 && start >= 0 && start < 16 * 65536) // "dump start" 라면
        {
            if (start > 16 * 65536 - 160)                        //메모리 범위를 벗어나는지 확인
                print_memory(state->memory_state, start, 16 * 65536 - 1);
            else
                print_memory(state->memory_state, start, start + 159);

            strncat(tokenized, token[0], MAX_LEN);
            strncat(tokenized, " ", MAX_LEN);
            strncat(tokenized, token[1], MAX_LEN);
            History_push(state->history_state, tokenized);        //token화 된 문자들을 합쳐 형식적인 명령어로 변환 후 history에 저장

            return false;
        }

        if (check_xdigit(token[2]) == false) // 두번째 인자 확인
        {
            printf("Wrong Input From du[mp] : second argument is invalid\n");
            return false;
        }

        //두번째 인자를 16진수로 적절히 변환 및 예외처리
        int end;
        if (strncmp(token[2], "0", MAX_LEN) == 0) end = 0;
        else if (strtol(token[2], NULL, 16) != 0)
            end = strtol(token[2], NULL, 16);

        if (token_num == 3 && start >= 0 && end > 0 && end >= start && start < 16 * 65536 && end < 16 * 65536) // "dump start end" 라면
        {
            print_memory(state->memory_state, start, end);

            strncat(tokenized, token[0], MAX_LEN);
            strncat(tokenized, " ", MAX_LEN);
            strncat(tokenized, token[1], MAX_LEN);
            strncat(tokenized, ", ", MAX_LEN);
            strncat(tokenized, token[2], MAX_LEN);
            History_push(state->history_state, tokenized);    //token화 된 문자들을 합쳐 형식적인 명령어로 변환 후 history에 저장

            return false;
        }

        printf("Wrong Input From du[mp] : arguments invalid\n");
        return false;
    }
    else if (strncmp(token[0], "e", MAX_LEN) == 0 || strncmp(token[0], "edit", MAX_LEN) == 0)    //첫번째 token이 e[dit] 라면
    {
        if (token_num == 3 && check_xdigit(token[1]) == true && check_xdigit(token[2]) == true)        //"edit address value"인지 확인
        {
            //16진수로 변환 및 예외처리
            int start = -1;
            int end = -1;
            if (strncmp(token[1], "0", MAX_LEN) == 0) start = 0;
            else if (strtol(token[1], NULL, 16) != 0)
                start = strtol(token[1], NULL, 16);
            if (strncmp(token[2], "0", MAX_LEN) == 0) end = 0;
            else if (strtol(token[2], NULL, 16) != 0)
                end = strtol(token[2], NULL, 16);
            if (start == -1 || end == -1) {
                printf("Wrong Input From e[dit]\n");
                return false;
            }

            modify_memory(state->memory_state, start, end);    // 해당 메모리값 변경

            strncat(tokenized, token[0], MAX_LEN);
            strncat(tokenized, " ", MAX_LEN);
            strncat(tokenized, token[1], MAX_LEN);
            strncat(tokenized, ", ", MAX_LEN);
            strncat(tokenized, token[2], MAX_LEN);
            History_push(state->history_state, tokenized);    //token화 된 문자들을 합쳐 형식적인 명령어로 변환 후 history에 저장
        }
        else
            printf("Wrong Input From e[dit]\n");

        return false;
    }

    else if (strncmp(token[0], "f", MAX_LEN) == 0 || strncmp(token[0], "fill", MAX_LEN) == 0)    //첫번째 token이 f[ill] 라면
    {
        if (token_num == 4 && check_xdigit(token[1]) == true && check_xdigit(token[2]) == true && check_xdigit(token[3]) == true)    //"fill start end value"인지 확인, 전부 16진수인지 확인
        {
            //16진수로 변환 및 예외처리
            int start = -1, end = -1, value = -1;
            if (strncmp(token[1], "0", MAX_LEN) == 0) start = 0;
            else if (strtol(token[1], NULL, 16) != 0)
                start = strtol(token[1], NULL, 16);
            if (strncmp(token[2], "0", MAX_LEN) == 0) end = 0;
            else if (strtol(token[2], NULL, 16) != 0)
                end = strtol(token[2], NULL, 16);
            if (strncmp(token[3], "0", MAX_LEN) == 0) value = 0;
            else if (strtol(token[3], NULL, 16) != 0)
                value = strtol(token[3], NULL, 16);

            for (int i = start; i <= end; i++) {
                modify_memory(state->memory_state, i, value);    //start 부터 end까지 메모리 값 변경
            }

            strncat(tokenized, token[0], MAX_LEN);
            strncat(tokenized, " ", MAX_LEN);
            strncat(tokenized, token[1], MAX_LEN);
            strncat(tokenized, ", ", MAX_LEN);
            strncat(tokenized, token[2], MAX_LEN);
            strncat(tokenized, ", ", MAX_LEN);
            strncat(tokenized, token[3], MAX_LEN);
            History_push(state->history_state, tokenized);    //token화 된 문자들을 합쳐 형식적인 명령어로 변환 후 history에 저장
        }
        else
            printf("Wrong Input From f[ill]\n");

        return false;
    }

    else if (strncmp(token[0], "reset", MAX_LEN) == 0)    //첫번째 token이 reset 라면
    {
        reset_memory(state->memory_state);
        strncat(tokenized, token[0], MAX_LEN);
        History_push(state->history_state, tokenized);
        return false;
    }

    else if (strncmp(token[0], "opcode", MAX_LEN) == 0)    //첫번째 token이 opcode 라면
    {
        if (token_num == 2)    //"opcode command" 인지 확인
        {
            int t = find_Opcode_table(state->opcode_state, token[1]);    //입력받은 command가 Opcode table에 있는지 확인
            if (t == -1)
            {
                printf("Cannot find searching OPCODE\n");
            }
            else
            {
                printf("opcode is %X\n", t);
                strncat(tokenized, token[0], MAX_LEN);
                strncat(tokenized, " ", MAX_LEN);
                strncat(tokenized, token[1], MAX_LEN);
                History_push(state->history_state, tokenized);    //token화 된 문자들을 합쳐 형식적인 명령어로 변환 후 history에 저장
            }
            return false;
        }

        printf("Wrong Input foramt from Opcode\n");
        return false;
    }

    else if (strncmp(token[0], "opcodelist", MAX_LEN) == 0) //첫번째 token이 opcodelist 라면
    {
        print_Opcode_table(state->opcode_state);
        printf("\n");
        strncat(tokenized, token[0], MAX_LEN);
        History_push(state->history_state, tokenized);
        return false;
    }

    else if (strncmp(token[0], "assemble", MAX_LEN) == 0) //첫번째 token이 assemble 라면
    {
        bool flag = false;

        if (token_num == 2) {
            // initialize symtab and assemble_table
            state->assemble_state = construct_Assemble_Table();
            state->symtab_state = construct_Symbol_Table();
            flag = assemble(state, token[1]);
        }

        if (flag)       //flag 여부로, assemble이 성공했을때 symbol 및 assemble table저장
        {
            //if success, save SYMBOL Table and ASSEMBLE Table
            state->final_symbol_table = state->symtab_state;
            state->final_assemble_table = state->assemble_state;
            //history push
            strncat(tokenized, token[0], MAX_LEN);
            strncat(tokenized, " ", MAX_LEN);
            strncat(tokenized, token[1], MAX_LEN);
            History_push(state->history_state, tokenized);
            return false;
        }
        else
        {
            printf("Assemble Command Error\n");
            return false;
        }
    }

    else if (strncmp(token[0], "type", MAX_LEN) == 0) //첫번째 token이 type 라면
    {
        bool flag = false;
        if (token_num == 2)
            flag = type_file(token[1]);
        if (flag)
        {
            strncat(tokenized, token[0], MAX_LEN);
            strncat(tokenized, " ", MAX_LEN);
            strncat(tokenized, token[1], MAX_LEN);
            History_push(state->history_state, tokenized);
            return false;
        }
        else
        {
            printf("Type Command Error\n");
            return false;
        }
    }

    else if (strncmp(token[0], "symbol", MAX_LEN) == 0) //첫번째 token이 symbol 라면
    {
        if (state->final_symbol_table == NULL)  //symbol table 생성 전이라면
        {
            printf("No Symbol Table Yet\n");
            return false;
        }
        Symbol_Node* fppp = state->final_symbol_table->head;
        if (fppp == NULL)   //symbol table이 비어있다면
        {
            printf("No Symbol Table\n");
            return false;
        }
        while (fppp != NULL)
        {
            printf("\t%s\t%04X\n", fppp->symbol, fppp->loc);
            fppp = fppp->next;
        }
        History_push(state->history_state, token[0]);
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
    for (int i = 0; i < 20; i++)
    {
        nptr = state->opcode_state->list[i]->head;
        while (nptr != NULL)
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
    char input[MAX_LEN];
    int dump_idx = 0;
    bool flag = false;
    while (1)
    {
        printf("sicsim> ");
        fgets(input, MAX_LEN, stdin);
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

