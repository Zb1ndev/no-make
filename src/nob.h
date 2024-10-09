#ifndef NOB_H
#define NOB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct NOB_strkvp {
    char* key;
    char* value; 
} NOB_strkvp;
typedef struct NOB_macros {
    NOB_strkvp* content;
    size_t maxIndex; 
} NOB_macros;
typedef struct NOB_line {
    size_t index;
    char* content;
} NOB_line;
typedef struct NOB_lines {
    size_t maxIndex;
    NOB_line* content;
} NOB_lines;

#pragma region NOB_getcwd
    #ifdef WINDOWS
        #include <direct.h>
        /* Returns the full path of your working directory. */
        char* NOB_getcwd() {
            char* _buf;
            _buf = malloc(_buf, sizeof(char) * 256);
            if (_getcwd(_buf, sizeof(_buf)) != NULL)
                return _buf;
            return NULL;
        }
    #else
        #include <unistd.h>
        /* Returns the full path of your working directory. */
        char* NOB_getcwd() { 
            char* _currentDir = getcwd(NULL, 0);
            return _currentDir;
        }
    #endif
#pragma endregion
#define NOB_assert(expression, message) if(expression) { printf(message); exit(1); }
#define NOB_strmov(movstrdest, movstrsrc) \
    movstrdest = malloc(strlen(movstrsrc)+1); \
    memcpy(movstrdest, movstrsrc, strlen(movstrsrc)+1);
#define NOB_strclr(strclrsrc) memset(strclrsrc, 0, strlen(strclrsrc));

size_t NOB_carrsize(char** _arr) {
    size_t _length = 0;
    while (_arr[_length++] != NULL);
    return _length-1;
}
size_t NOB_fsizeof(char* _fileDir) {

    FILE* _file = fopen(_fileDir, "r");
    NOB_assert((_file == NULL),"[ERROR] File Not Found. \n");
  
    fseek(_file, 0L, SEEK_END); 
    size_t _length = ftell(_file); 

    fclose(_file);
    return _length;
    
}
char* NOB_remchr(char* _src, char _delimiter) {

    char* _ret; 
    NOB_strmov(_ret, _src);
    
    char* _endbuffer = malloc(strlen(_src) + 1);
    char* _startbuffer = malloc(strlen(_src) + 1);
    size_t _stringLength = 0;

    for (size_t i = 0; i < (strlen(_src) + 1); i++){
        if (strchr(_ret, _delimiter) != NULL) {
            
            NOB_strclr(_endbuffer);
            NOB_strclr(_startbuffer);

            size_t _index = -(strlen(strchr(_ret, _delimiter)) - strlen(_ret));
            memcpy(_endbuffer, _ret+_index+1, strlen(_ret) + 1);
            memcpy(_startbuffer, _ret, _index);
            sprintf(_ret, "%s%s", _startbuffer, _endbuffer);
            _stringLength++;

        }
    }

    _ret[(strlen(_src)-1) - (_stringLength-1)] = '\0';
    return _ret;

}
void NOB_fgetl(NOB_lines* _dest, char* _fileDir) {

    FILE* _file = fopen(_fileDir, "r");
    NOB_assert((_file == NULL),"[ERROR] File Not Found. \n");

    size_t _index = 0;
    size_t _size = NOB_fsizeof(_fileDir) + 1;
    char* _buffer = malloc(_size);
    _dest->content = malloc(_size * sizeof(NOB_line));

    NOB_assert((_size <= 1), "[ERROR] File is Empty.");

    while (fgets(_buffer, _size, _file) != NULL) {
        NOB_strmov(_dest->content[_index].content, NOB_remchr(_buffer, '\n'));
        _dest->content->index = _index++;
    }
    _dest->maxIndex = _index;
        
    free(_buffer);
    fclose(_file);

}

#endif