#include "path_knowledge.hpp"

path_knowledge::path_knowledge()
{
    
}
        
bool path_knowledge::known(int i)
{
    return this->knowledge.find(i) != this->knowledge.end();
}

void path_knowledge::merge(path_knowledge& a)
{
    this->merge_back(a, true);
}

void path_knowledge::merge_back(path_knowledge& a, bool back)
{
    map<int, pair<float, vector<int> > >::iterator it = this->knowledge.begin(),
                                                   end = this->knowledge.end();
    
    for ( ; it != end; ++it ){
        this->learn(it->first, it->second.first, it->second.second);
    }
    
    if ( back ){
        a.merge_back(*this, false);
    }
}

void path_knowledge::forget(int i)
{
    this->knowledge.erase(i);  
}

void path_knowledge::learn(int i, float time, vector<int>& path)
{
    if ( this->knowledge.find(i) == this->knowledge.end() || 
         this->knowledge[i].first > time )
    {
        this->knowledge[i] = make_pair(time, path);
    }
}
