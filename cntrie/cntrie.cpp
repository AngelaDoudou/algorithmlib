#include "cntrie.h"
#include <algorithm>//sort
#include <string.h>//strlen
#include <iostream>//cout endl
using std::cout;
using std::endl;

long cntrie::insert(const string& str, const string& name){
    int cn = 0;
    if(m_name != name){
        cout << "destory!! m_name =" << m_name << " name=" << name << endl;
        _destory();
        m_name = name;
    }
    vector<string> vkeys;
    int size = _utf8split(vkeys, str.c_str());

    //debug
    //cout << "split:" << size;
    //for(auto &it :vkeys){
    //    cout << it << " ";
    //}
    //cout << endl;
    //end debug

    if(size > 0){
        map<string, cntrie_node_t*> *t = &m_trie;
        for(int i = 0; i < size; ++i){
            auto it = t->find(vkeys[i]);//it point  to the vkeys[i]'s info
            if( it != t->end()){
                //cout << "find:" << vkeys[i] << endl;
                if(i == size - 1){
                    it->second->isword = true;
                    it->second->count++;
                }
                //cout << "node:" << it->second->count << ":" << it->second->isword << ":" << it->second->trie.size() << endl;
                t = &(it->second->trie);
                cn = it->second->count;
            }else{
                //cout << "not find:" << vkeys[i] << endl;
                cntrie_node_t *pnode = NULL;
                if(i == size - 1){
                    pnode = new cntrie_node_t(true, 1);
                }else{
                    pnode = new cntrie_node_t(false, 0);
                }
                //cout << "node:" << pnode->count << ":" << pnode->isword << ":" << pnode->trie.size() << endl;
                t->insert(pair<string, cntrie_node_t*>(vkeys[i], pnode));
                //t[vkeys[i]] = pnode;
                t = &(pnode->trie);
                cn = 1;
            }
        }
    }
    return cn;
}
long cntrie::search(const string& str, const string& name){
    int cn = 0;
    if(m_name != name) return cn;
    vector<string> vkeys;
    int size = _utf8split(vkeys, str.c_str());

    if(size > 0){
        map<string, cntrie_node_t*> &t = m_trie;
        for(int i = 0; i < size; ++i){
            auto it = t.find(vkeys[i]);//it point  to the vkeys[i]'s info
            if( it != t.end()){
                t = it->second->trie;
                cn = it->second->count;
            }else{
                break;
            }
        }
    }
    return cn;
}
bool cntrie::_CmpStrCnPair(pair<string, long> p1, pair<string, long> p2){
    return (p1.second > p2.second) ? true : false;
}
long cntrie::walk(vector<pair<string, long> >& vec, const string& name){
    int cn = 0;
    if(m_name != name) return cn;
    string str = "";
    cn = _inwalk(vec, str, m_trie);
    if(cn > 0){
        sort(vec.begin(), vec.end(), _CmpStrCnPair);
    }
    return cn;
}
void cntrie::print(){
    int size = m_trie.size();
    cout << "m_trie size:" << size << endl;
    for(auto &it :m_trie){
        cout << it.first << ":";
        _print(it.second);
    }
}
void cntrie::_print(cntrie_node_t *pn){
    if(!pn) return;
    cout << pn->isword << ":" << pn->count << endl;
    for(auto & it : pn->trie){
        cout << "\t" << it.first << endl;
        _print(it.second);
    }
}
long cntrie::_inwalk(vector<pair<string, long> >& vec, string &str, map<string, cntrie_node_t *>& t){
    int size = t.size();
    if(size <= 0){
        return vec.size();
    }
    //size > 0
    for(auto &it : t){
        string tmpstr = str + it.first;
        if(it.second->isword){
            vec.push_back(pair<string, long>(tmpstr, it.second->count));
        }
        _inwalk(vec, tmpstr, it.second->trie);
    }
    return vec.size();
}
void cntrie::_destory(){
    _inwalkDestory(m_trie);
    m_name = "default";
}
void cntrie::_inwalkDestory(map<string, cntrie_node_t*>& t){
    int size = t.size();
    if(size <= 0){
        return;
    }
    //size > 0
    for(auto &it : t){
        _inwalkDestory(it.second->trie);
        delete it.second;
        it.second = NULL;
    }
    t.clear();
}
//左边第一个字节1的个数表示这个字符编码的字节数
////1个字节: 0xxxxxxx
////2个字节: 110xxxxx 10xxxxxx
////3个字节: 1110xxxx 10xxxxxx 10xxxxxx
////4个字节: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
////5,6个字节略
long cntrie::_utf8split(vector<string>& vec, const char* pszSentence){
    //const char* pszSentence, vector<string>& vec
    int iLen = strlen( pszSentence );
    const char* p = pszSentence;
    unsigned char * q;
    char *szCharacter = new char[iLen + 1];//[101];
    int iChar;
    int iNumChars = 0;
    vec.clear();
    string  strCharacter;
    while ( p != NULL && strlen( p ) > 0 )
    {
        q = ( unsigned char * ) p;
        if ( q[0] < 0x80 )
        {
            //p[ 0 ] must be an ASCII character
            iChar = 0;
            szCharacter[iChar++] = p[0];
            p++;
            q = ( unsigned char * ) p;
            while ( p != NULL && q[0] < 0x80 )
            {
                szCharacter[iChar++] = p[0];
                p++;
                q = ( unsigned char * ) p;
            }
            szCharacter[iChar] = '\0';
            vec.push_back( string( szCharacter ) );
            iNumChars++;
        }
        else if ( q[0] < 0xC0 )
        {
            //invalid char between 0x80 and 0xC0
            p++;
        }
        else if ( q[0] < 0xE0 )
        {
            //two chars
            szCharacter[0] = p[0];
            szCharacter[1] = p[1];
            szCharacter[2] = '\0';
            p = p + 2;

            strCharacter = string( szCharacter );
            vec.push_back( strCharacter );

            iNumChars++;
        }
        else if ( q[0] < 0xF0 )
        {
            //three chars
            szCharacter[0] = p[0];
            szCharacter[1] = p[1];
            szCharacter[2] = p[2];
            szCharacter[3] = '\0';
            p = p + 3;

            strCharacter = string( szCharacter );
            vec.push_back( strCharacter );

            //printf( "%s ", strCharacter.c_str( ) );

            iNumChars++;
        }
        else if ( q[0] < 0xF8 )
        {
            //four chars
            p += 4;
        }
        else if ( q[0] < 0xFC )
        {
            //five chars
            p += 5;
        }
        else if ( q[0] < 0xFE )
        {
            //6 chars
            p += 5;
        }
        else
        {
            //>=0xFE
            p++;
        }
    }
    delete[] szCharacter;
    return vec.size();
}
