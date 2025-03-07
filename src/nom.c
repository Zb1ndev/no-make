#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nom.h"

// #define DEBUG

char* GetBuildTemplate(char* _tag, char* _workingPath) {

    char* _fileDir = malloc(strlen(NOM_getcwd()) + 9);
    sprintf(_fileDir, "%s%s", NOM_getcwd(), "/nom");
    
    NOM_lines _lines;
    NOM_fgetl(&_lines, _fileDir);

    size_t _templateIndex = 0;
    NOM_strkvp* _templates = malloc(sizeof(NOM_strkvp) * _lines.maxIndex);

    for (size_t l = 0; l < _lines.maxIndex; l++) {
        if (strchr(_lines.content[l].content, '#') == NULL && strchr(_lines.content[l].content, ':') != NULL) {
            char* _buffer = strchr(_lines.content[l].content,':');
            _templates[_templateIndex].value = malloc(strlen(_buffer));
            memcpy(_templates[_templateIndex].value, _buffer+1, strlen(_buffer));
            size_t _keyIndex = (strlen(_lines.content[l].content) - strlen(_templates[_templateIndex].value));
            _templates[_templateIndex].key = malloc(_keyIndex + 1); 
            memcpy(_templates[_templateIndex].key, _lines.content[l].content, _keyIndex-1); 
            _templates[_templateIndex++].key[_keyIndex-1] = '\0';
        }
    }
    for (size_t t = 0; t < _templateIndex; t++) {
        if (strcmp(_tag, NOM_remchr(_templates[t].key, ' ')) == 0) { 
            return _templates[t].value; 
        }
    }

    NOM_assert(1,"[ERROR] No Command with the provided tag was found.");

}
NOM_macros GetMacros(char* _workingPath) {

    char* _fileDir = malloc(strlen(NOM_getcwd()) + 9);
    sprintf(_fileDir, "%s%s", NOM_getcwd(), "/nom");
    
    NOM_lines _lines;
    NOM_fgetl(&_lines, _fileDir);

    NOM_macros _macros;
    size_t _macroIndex = 0;
    _macros.content = malloc(_lines.maxIndex * sizeof(NOM_strkvp));

    for (size_t l = 0; l < _lines.maxIndex; l++) {
        if (strchr(_lines.content[l].content, '#') != NULL) {
            char* _macro = strchr(_lines.content[l].content, '#');
            char* _buffer = strchr(_macro,':');
            _macros.content[_macroIndex].value = malloc(strlen(_macro));
            memcpy(_macros.content[_macroIndex].value,_buffer+1, strlen(_buffer));
            size_t _keyIndex = (strlen(_macro) - strlen(_buffer));
            _macros.content[_macroIndex].key = malloc(_keyIndex + 1);
            memcpy(_macros.content[_macroIndex].key, _macro+1, _keyIndex-1);
            _macros.content[_macroIndex++].key[_keyIndex-1] = '\0';
        }
    }

    _macros.maxIndex = _macroIndex;

    return _macros;

}
char* GenerateCommand(char* _template, NOM_macros _macros) {


    size_t _size = strlen(_template) + 1;
    for (size_t i = 0; i < _macros.maxIndex; i++)
        _size += strlen(_macros.content[i].value);
    
    char* _command = malloc(_size); 
    memset(_command, 0, _size);

    size_t _skip = 0;
    char* _buffer = strtok(_template, " ");
    while (_buffer != NULL) {
        
        _buffer = NOM_remchr(_buffer, '\n');

        for (size_t i = 0; i < _macros.maxIndex; i++) {
            if (strcmp(_buffer, NOM_remchr(_macros.content[i].key, ' ')) == 0) {
                sprintf(_command, "%s %s", _command, NOM_remchr(_macros.content[i].value, '\n'));
                _skip++;
                break;
            }
        }

        if (_skip == 0)
            sprintf(_command, "%s %s", _command, _buffer);

        _buffer = strtok(NULL, " ");
        _skip = 0;
    }

    return _command;

}
int main(int argc, char** argv) {

    #ifdef DEBUG

        char* _template = GetBuildTemplate("b", NOM_getcwd());
        char* _command = GenerateCommand(_template, GetMacros(NOM_getcwd()));
        printf("%s\n", _command);

    #else
    
        NOM_assert((argv[1] == NULL),"[ERROR] Please Enter and Argument. \n");
        char* _template = GetBuildTemplate(argv[1], NOM_getcwd());
        char* _command = GenerateCommand(_template, GetMacros(NOM_getcwd()));
        NOM_assert((_command == NULL),"[ERROR] Generated Command was Invalid. \n");
        system(_command);

    #endif

    return 0;

}
