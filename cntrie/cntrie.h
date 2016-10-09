#ifndef __CHINESE_TRIE_H__
#define __CHINESE_TRIE_H__
#include <vector>
#include <map>
#include <string>

using std::map;
using std::vector;
using std::string;
using std::pair;

class cntrie{
        class cntrie_node_t{
            public:
                cntrie_node_t(bool _isword, long _count):isword(_isword),count(_count){
                    trie.clear();
                }
                bool isword;
                long count;
                map<string, cntrie_node_t *> trie;
        };
        map<string, cntrie_node_t*> m_trie;
        string m_name;
    public:
        cntrie(string name = "default"):m_name(name){}
        ~cntrie(){
            _destory();
        }
        long insert(const string& str, const string& name);
        long search(const string& str, const string& name);
        long walk(vector<pair<string, long> >& vec, const string& name);
        void print();

    private:
        void _print(cntrie_node_t *pn);
        static bool _CmpStrCnPair(pair<string, long> p1, pair<string, long> p2);
        long _inwalk(vector<pair<string, long> >& vec, string &str, map<string, cntrie_node_t *>& t);
        void _destory();
        void _inwalkDestory(map<string, cntrie_node_t *>& t);
        //long _utf8split(vector<string>& vec, const char* pszSentence);
        long _utf8split(vector<string>& vec, const char* pszSentence);

};
#endif//__CHINESE_TRIE_H__
