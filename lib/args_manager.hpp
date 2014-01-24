#ifndef __AMBUSH_ARGS_MANAGER
#define __AMBUSH_ARGS_MANAGER

#include <map>
#include <cstring>
#include <iostream>
#include <cassert>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"
#include "rapidjson/stringbuffer.h"

using namespace std;

class args_manager {
    
    private:
        enum args_type { ARG_INT, ARG_FLOAT, ARG_STRING, ARG_BOOL };
        map<string, args_type>    type_args;
        
        map<string, int>    int_args;
        map<string, float>  float_args;
        map<string, bool>   bool_args;
        map<string, string> string_args;
    
    public:
        args_manager();
        args_manager(const rapidjson::Value& description);
        
        void clear_arg(string name);
        bool has_arg(string name);
        args_type arg_type(string name);
        
        void set_int_arg(string name, int value);
        void set_float_arg(string name, float value);
        void set_bool_arg(string name, bool value);
        void set_string_arg(string name, string value);
        
        int    get_int_arg(string name);
        float  get_float_arg(string name);
        bool   get_bool_arg(string name);
        string get_string_arg(string name);
    
    private:
        bool has_arg(string name, args_type type);
};

#endif