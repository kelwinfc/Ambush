#include "args_manager.hpp"

args_manager::args_manager()
{
    
}

args_manager::args_manager(const rapidjson::Value& description)
{
    if ( description.IsArray() ){
        for (rapidjson::SizeType j = 0; j < description.Size(); j++){
            const rapidjson::Value& next_arg = description[j];
            const rapidjson::Value& name = next_arg["arg"];
            const rapidjson::Value& value = next_arg["value"];
            
            if ( name.IsString() ){
                if ( value.IsNumber() ){
                    this->set_float_arg(name.GetString(), value.GetDouble());
                } else if ( value.IsBool() ){
                    this->set_bool_arg(name.GetString(), value.GetBool());
                } else if ( value.IsString() ){
                    this->set_string_arg(name.GetString(), value.GetString());
                }
            }
        }
    }
}

void args_manager::clear_arg(string name)
{
    if ( this->has_arg(name ) ){
        
        args_type type = this->arg_type(name);
        switch ( type ) {
            case ARG_INT:
                this->int_args.erase(name);
                break;
            case ARG_FLOAT:
                this->float_args.erase(name);
                break;
            case ARG_BOOL:
                this->bool_args.erase(name);
                break;
            case ARG_STRING:
                this->string_args.erase(name);
                break;
        }
        this->type_args.erase(name);
    }
}

bool args_manager::has_arg(string name)
{
    return this->type_args.find(name) != this->type_args.end();
}

bool args_manager::has_arg(string name, args_manager::args_type type)
{
    switch ( type ) {
        case ARG_INT:
            return this->int_args.find(name) != this->int_args.end();
        case ARG_FLOAT:
            return this->float_args.find(name) != this->float_args.end();
        case ARG_BOOL:
            return this->bool_args.find(name) != this->bool_args.end();
        case ARG_STRING:
            return this->string_args.find(name) != this->string_args.end();
    }
    
    return false;
}

args_manager::args_type args_manager::arg_type(string name)
{
    assert( this->has_arg(name) );
    return this->type_args[name];
}

void args_manager::set_int_arg(string name, int value)
{
    this->clear_arg(name);
    this->type_args[name] = ARG_INT;
    this->int_args[name] = value;
}

void args_manager::set_float_arg(string name, float value)
{
    this->clear_arg(name);
    this->type_args[name] = ARG_FLOAT;
    this->float_args[name] = value;
}

void args_manager::set_bool_arg(string name, bool value)
{
    this->clear_arg(name);
    this->type_args[name] = ARG_BOOL;
    this->bool_args[name] = value;
}

void args_manager::set_string_arg(string name, string value)
{
    this->clear_arg(name);
    this->type_args[name] = ARG_STRING;
    this->string_args[name] = value;
}

int args_manager::get_int_arg(string name)
{
    assert( this->arg_type(name) == ARG_INT );
    return this->int_args[name];
}

float args_manager::get_float_arg(string name)
{
    assert( this->arg_type(name) == ARG_FLOAT || 
            this->arg_type(name) == ARG_INT);
    return this->float_args[name];
}

bool args_manager::get_bool_arg(string name)
{
    assert( this->arg_type(name) == ARG_BOOL );
    return this->bool_args[name];
}

string args_manager::get_string_arg(string name)
{
    assert( this->arg_type(name) == ARG_STRING );
    return this->string_args[name];
}
