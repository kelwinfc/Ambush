#ifndef __AMBUSH_PATH_KNOWLEDGE
#define __AMBUSH_PATH_KNOWLEDGE

#include <map>
#include <vector>

using namespace std;

class path_knowledge {
    private:
        map<int, pair<float, vector<int> > > knowledge;
    
    public:
        path_knowledge();
        
        bool known(int i);
        vector<int>* get_path(int i);
        void merge(path_knowledge& a);
        void forget(int i);
        void learn(int i, float time, vector<int>& path);
    
    private:
        void merge_back(path_knowledge& a, bool back);
};

#endif