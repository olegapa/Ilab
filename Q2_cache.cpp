#include <stdio.h>
#include <iostream>
#include <list>
#include <unordered_map>
#include <iterator>
#include <cassert>
#include <assert.h>

enum{
    HOTCACHE = 0,
    OUTCACHE,
    INCACHE

};

template <typename T, typename KeyT = int>
struct cache_t {
    size_t in_sz, out_sz, hot_sz;
    std::list<T> in, out, hot;

    typedef std::list<int>::iterator ListIt;
    std::unordered_map<KeyT, ListIt> hash_in, hash_out, hash_hot;

    bool full(const int is_cache) const
    {
        switch(is_cache){
            case HOTCACHE: return (hot_sz == hot.size());
            case OUTCACHE: return (out_sz == out.size());
            case INCACHE: return (in_sz == in.size());
            default: printf("somethink is wrong");
                return false;
        }
    }
    bool LookUp(KeyT key)
    {
        auto hit = hash_hot.find(key);

        if(hit != hash_hot.end())
        {



            return true;
        }

        hit = hash_out.find(key);

        if(hit != hash_out.end())
        {
            if(full(HOTCACHE))
            {
                hash_hot.erase(hot.back());
                hot.pop_back();
            }

            hot.push_front(key);
            hash_hot[key] = hot.begin();

            return true;
        }

        hit = hash_in.find(key);

        if(full(INCACHE))
        {
            if(full(OUTCACHE))
            {
                hash_out.erase(out.back());
                out.pop_back();
            }

            out.push_front(in.back());
            hash_out[in.back()] = out.begin();

            hash_in.erase(in.back());
            in.pop_back();

            in.push_front(key);
            hash_in[key] = in.begin();
        }

        else
        {
            in.push_front(key);
            hash_in[key] = in.begin();
        }
        if(hit == hash_in.end())
            return false;

        return true;
    }

    cache_t(size_t insz, size_t outsz,size_t hotsz) : in_sz(insz), out_sz(outsz), hot_sz(hotsz) {}


};

int main()
{
    int hits = 0, n;
    size_t m1, m2, m3;

    std::cin >> m1 >> m2 >> m3 >> n;
    assert(std::cin.good());

    cache_t<int> c{m1, m2, m3};

    for(int i = 0; i<n; i++)
    {
        int q;

        std::cin >> q;
        assert(std::cin.good());

        if(c.LookUp(q))
            hits++;

    }

    std::cout << hits << "\n";

    return 0;
}


