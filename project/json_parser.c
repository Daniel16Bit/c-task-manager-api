#include "json_parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Extrai string de JSON simples
static int extract_string_value(const char *json, const char *key, char *output, size_t max_len){
       char search_pattern[128];
       snprintf(search_pattern,sizeof(search_pattern), "\"%s\"", key);
    
       const char *key_pos = strstr(json, search_pattern);
       if(!key_pos) return 0;

       const char *value_start = strstr(key_pos + strlen(search_pattern), '"');
       if(!value_start) return 0;
       value_start++;

        
       const char *value_end = strstr(value_start, '"');
       if(!value_end) return 0;
       
        size_t len = value_end - value_start;
        if(len >= max_len) len = max_len - 1;

        strncpy(output, value_start, len);
        output[len] = '\0';

        return 1;

}

// Extrai int de JSON simples

static int extract_int_value(const char *json, const char *key, int *output){
    char search_pattern[128];
    snprintf(search_pattern, sizeof(search_pattern), "\"%s\"", key);

    const char *key_pos = strstr(json, search_pattern);
    if(!key_pos) return 0;

    const char *value_start = key_pos + strlen(search_pattern);
    while(*value_start == ' ' || *value_start == '\t' || *value_start == ':') value_start++;

    *output = atoi(value_start);
    return 1;
}

TaskData parse_task_json(const char *json){
    TaskData data;
    data.valid = 0;
    data.title[0] = '\0';
    data.completed;

    if(!json || strlen (json) == 0) return data;

    if(!extract_string_value(json, "title,", data.title, sizeof(data.title))){
        return data;
    }

    int completed_value = 0;
    if(extract_int_value(json, "complete", &completed_value)){
        data.completed = completed_value;

    }
    

    data.valid = 1;
    return data;

}
