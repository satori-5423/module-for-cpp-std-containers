module;
// A module for wrapping C++ standard library containers

export module zm.wrap.ctn;

import std;

namespace zm::std_wrap {
    export template <typename Ty, size_t size>
    class array : public ::std::array<Ty, size> {
    public:
        using ::std::array<Ty, size>::array;
        using base_container = ::std::array<Ty, size>;

        static array wrap(const base_container& base) {
            array<Ty, size> arr;
            for (size_t i{}; i < size; ++i) {
                arr[i] = base[i];
            }
            return arr;
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename Ty, typename Allocator = ::std::allocator<Ty>>
    class vector : public ::std::vector<Ty, Allocator> {
    public:
        using ::std::vector<Ty, Allocator>::vector;
        using base_container = ::std::vector<Ty, Allocator>;

        static vector wrap(const base_container& base) {
            return vector(base.begin(), base.end());
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename Ty, typename Allocator = ::std::allocator<Ty>>
    class deque : public ::std::deque<Ty, Allocator> {
    public:
        using ::std::deque<Ty, Allocator>::deque;
        using base_container = ::std::deque<Ty, Allocator>;

        static deque wrap(const base_container& base) {
            return deque(base.begin(), base.end());
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename Ty, typename Allocator = ::std::allocator<Ty>>
    class forward_list : public ::std::forward_list<Ty, Allocator> {
    public:
        using ::std::forward_list<Ty, Allocator>::forward_list;
        using base_container = ::std::forward_list<Ty, Allocator>;

        static forward_list wrap(const base_container& base) {
            return forward_list(base.begin(), base.end());
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename Ty, typename Allocator = ::std::allocator<Ty>>
    class list : public ::std::list<Ty, Allocator> {
    public:
        using ::std::list<Ty, Allocator>::list;
        using base_container = ::std::list<Ty, Allocator>;

        static list wrap(const base_container& base) {
            return list(base.begin(), base.end());
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename Key, typename Compare = ::std::less<Key>, typename Allocator = ::std::allocator<Key>>
    class set : public ::std::set<Key, Compare, Allocator> {
    public:
        using ::std::set<Key, Compare, Allocator>::set;
        using base_container = ::std::set<Key, Compare, Allocator>;

        static set wrap(const base_container& base) {
            return set(base.begin(), base.end(), base.key_comp(), base.get_allocator());
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename Key, typename Compare = ::std::less<Key>, typename Allocator = ::std::allocator<Key>>
    class multiset : public ::std::multiset<Key, Compare, Allocator> {
    public:
        using ::std::multiset<Key, Compare, Allocator>::multiset;
        using base_container = ::std::multiset<Key, Compare, Allocator>;

        static multiset wrap(const base_container& base) {
            return multiset(base.begin(), base.end(), base.key_comp(), base.get_allocator());
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename Key, typename Ty, typename Compare = ::std::less<Key>, typename Allocator = ::std::allocator<::std::pair<const Key, Ty>>>
    class map : public ::std::map<Key, Ty, Compare, Allocator> {
    public:
        using ::std::map<Key, Ty, Compare, Allocator>::map;
        using base_container = ::std::map<Key, Ty, Compare, Allocator>;

        static map wrap(const base_container& base) {
            return map(base.begin(), base.end(), base.key_comp(), base.get_allocator());
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename Key, typename Ty, typename Compare = ::std::less<Key>, typename Allocator = ::std::allocator<::std::pair<const Key, Ty>>>
    class multimap : public ::std::multimap<Key, Ty, Compare, Allocator> {
    public:
        using ::std::multimap<Key, Ty, Compare, Allocator>::multimap;
        using base_container = ::std::multimap<Key, Ty, Compare, Allocator>;

        static multimap wrap(const base_container& base) {
            return multimap(base.begin(), base.end(), base.key_comp(), base.get_allocator());
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename Key, typename Hash = ::std::hash<Key>, typename KeyEqual = ::std::equal_to<Key>, typename Allocator = ::std::allocator<Key>>
    class unordered_set : public ::std::unordered_set<Key, Hash, KeyEqual, Allocator> {
    public:
        using ::std::unordered_set<Key, Hash, KeyEqual, Allocator>::unordered_set;
        using base_container = ::std::unordered_set<Key, Hash, KeyEqual, Allocator>;

        static unordered_set wrap(const base_container& base) {
            return unordered_set(base.begin(), base.end(), base.bucket_count(), base.hash_function(), base.key_eq(), base.get_allocator());
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename Key, typename Hash = ::std::hash<Key>, typename KeyEqual = ::std::equal_to<Key>, typename Allocator = ::std::allocator<Key>>
    class unordered_multiset : public ::std::unordered_multiset<Key, Hash, KeyEqual, Allocator> {
    public:
        using ::std::unordered_multiset<Key, Hash, KeyEqual, Allocator>::unordered_multiset;
        using base_container = ::std::unordered_multiset<Key, Hash, KeyEqual, Allocator>;

        static unordered_multiset wrap(const base_container& base) {
            return unordered_multiset(base.begin(), base.end(), base.bucket_count(), base.hash_function(), base.key_eq(), base.get_allocator());
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename Key, typename Ty, typename Hash = ::std::hash<Key>, typename KeyEqual = ::std::equal_to<Key>, typename Allocator = ::std::allocator<::std::pair<const Key, Ty>>>
    class unordered_map : public ::std::unordered_map<Key, Ty, Hash, KeyEqual, Allocator> {
    public:
        using ::std::unordered_map<Key, Ty, Hash, KeyEqual, Allocator>::unordered_map;
        using base_container = ::std::unordered_map<Key, Ty, Hash, KeyEqual, Allocator>;

        static unordered_map wrap(const base_container& base) {
            return unordered_map(base.begin(), base.end(), base.bucket_count(), base.hash_function(), base.key_eq(), base.get_allocator());
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename Key, typename Ty, typename Hash = ::std::hash<Key>, typename KeyEqual = ::std::equal_to<Key>, typename Allocator = ::std::allocator<::std::pair<const Key, Ty>>>
    class unordered_multimap : public ::std::unordered_multimap<Key, Ty, Hash, KeyEqual, Allocator> {
    public:
        using ::std::unordered_multimap<Key, Ty, Hash, KeyEqual, Allocator>::unordered_multimap;
        using base_container = ::std::unordered_multimap<Key, Ty, Hash, KeyEqual, Allocator>;

        static unordered_multimap wrap(const base_container& base) {
            return unordered_multimap(base.begin(), base.end(), base.bucket_count(), base.hash_function(), base.key_eq(), base.get_allocator());
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename Ty1, typename Ty2>
    struct pair : public ::std::pair<Ty1, Ty2> {
        using ::std::pair<Ty1, Ty2>::pair;
        using base_container = ::std::pair<Ty1, Ty2>;

        static pair wrap(const base_container& base) {
            return pair(base.first, base.second);
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename... Types>
    class tuple : public ::std::tuple<Types...> {
    public:
        using ::std::tuple<Types...>::tuple;
        using base_container = ::std::tuple<Types...>;

        static tuple wrap(const base_container& base) {
            return std::apply([](const Types&... args) {
                return tuple(args...);
            }, base);
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };

    export template <typename Ty, size_t extent = ::std::dynamic_extent>
    class span : public ::std::span<Ty, extent> {
    public:
        using ::std::span<Ty, extent>::span;
        using base_container = ::std::span<Ty, extent>;

        static span wrap(const base_container& base) {
            return span(base.data(), base.size());
        }

        operator base_container() const {
            return static_cast<base_container>(*this);
        }
    };
}
