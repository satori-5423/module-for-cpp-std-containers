module;
// Personal formatting module for C++ standard library containers
// Note: Minimum C++ standard level to use this library: 23.
// Note: This library does not currently provide specialized templates for `::std::mdspan` and `::std::{ stack, queue, priority_queue }`.

/**
* Warning: To enable out-of-the-box use, this module proactively specializes standard library containers within the std namespace.
* Therefore, this module is not safe and should not be deployed in critical projects.
* If you must use it, consider appropriately modifying this module to add an additional layer of encapsulation around the standard library containers!
*/

export module zm.fmt;

/*************************
*  Version: 1.2.0-beta   *
*  Author:  Zhao Mengfu  *
*  Update:  2024.06.11   *
**************************/

#pragma warning(push)
#if defined(_MSC_VER)
#pragma warning(disable: 4819 5244) // Suppress specific warnings, may not be useful.
#endif

/**
* By default, it is assumed that your compiler has enabled the C++23 standard library modules.
* If not, please manually replace the `import std;` statement with the necessary standard library container header files.
*/
import std;

namespace zm::fmt {
    // Format function
    export [[nodiscard]] ::std::string format(const auto, auto&&...);

    // Print functions
    export [[noreturn]] void print(const ::std::string_view, auto&&...);

    export [[noreturn]] void println(const ::std::string_view, auto&&...);

    namespace priv {
        namespace concepts {
            template <typename Seque>
            // Concept for `::std::{ vector, deque, forward_list, list }`.
            concept seque = ::std::same_as<Seque, ::std::vector<typename Seque::value_type, typename Seque::allocator_type>>
                || ::std::same_as<Seque, ::std::deque<typename Seque::value_type, typename Seque::allocator_type>>
                || ::std::same_as<Seque, ::std::forward_list<typename Seque::value_type, typename Seque::allocator_type>>
                || ::std::same_as<Seque, ::std::list<typename Seque::value_type, typename Seque::allocator_type>>;

            template <typename Assoc>
            // Concept for `::std::{ set, multiset, unordered_set, unordered_multiset }`.
            concept sets = requires (Assoc cont) {
                requires requires {
                    { cont.key_comp() } -> ::std::same_as<typename Assoc::key_compare>;
                    { cont.value_comp() } -> ::std::same_as<typename Assoc::value_compare>;
                } || requires {
                    { cont.key_eq() } -> ::std::same_as<typename Assoc::key_equal>;
                    { cont.hash_function() } -> ::std::same_as<typename Assoc::hasher>;
                };
            };

            template <typename Assoc>
            // Concept for `::std::{ map, multimap, unordered_map, unordered_multimap }`.
            concept maps = requires {
                typename Assoc::mapped_type;
            } && sets<Assoc>;
        } // namespace zm::fmt::priv::concepts

        namespace chars {
            const std::unordered_map<char, char> chars_map = {
                { '(', ')' }, { '[', ']' }, { '{', '}' }, { '<', '>' },
                { ')', '(' }, { ']', '[' }, { '}', '{' }, { '>', '<' }
            };

            const char escape{ '#' };
            /**
            * For characters in the parsed string that might cause ambiguity, such as "}", you can escape them by adding a "#" before the character. For example, "<}" can be written as "<#}". For the "#" character itself, also use "#" to escape it.
            * 
            * Example:
            * 
            * std::array arr{ 1, 2, 3 };
            * 
            * // auto result = std::vformat("{:{<}}}", std::make_format_args(arr));
            * // Error: "The format string must start with '{'."
            * 
            * auto result = std::vformat("{:{<#}}}", std::make_format_args(arr));
            * // Ok, the value of result is "<}1, 2, 3{>".
            *
            */

            const char literal{ '$' };
            /**
            * 
            * For characters in the parsed string that you do not want to be mapped by `chars_map`, you can skip the escape operation by adding a "$" after the character. For the "$" character itself, you can use "#$" (escape) or "$$" (skip) to represent it.
            * 
            * Example:
            * 
            * std::array arr{ 1, 2, 3 };
            * 
            * auto result0 = std::vformat("{:{<<}}", std::make_format_args(arr));
            * // The value of result0 is "<<1, 2, 3>>".
            * 
            * auto result1 = std::vformat("{:{<$<$}}", std::make_format_args(arr));
            * // The value of result1 is "<<1, 2, 3<<".
            *
            */

            constexpr auto start = [](char flag) -> bool { return flag == 'A' || flag == 'a'; };
            /**
            * Use a pair of `A` or `a` to wrap a string, making the wrapped string appear only on the left side of the formatted string. Since there is no corresponding right-side string in this case, the special meaning of "$" becomes invalid.
            * 
            * Example:
            * 
            * std::map map{ std::pair{ 1, "abc" }, { 2, "def" }, { 3, "ghi" } };
            * 
            * auto result = std::vformat("{:{{(Akey: A}{), (key: }{, val: }}", std::make_format_args(map));
            * 
            * // The value of result is "{(key: 1, val: abc), (key: 2, val: def), (key: 3, val: ghi)}".
            * 
            */

            constexpr auto reverse = [](char flag) -> bool { return flag == 'R' || flag == 'r'; };
            /**
            * 
            * By default, the strings on both sides after formatting will be symmetrically matched. If you need them not to be symmetrically matched, you can declare this by adding `R` or `r` at the end of the format string.
            * 
            * Example:
            * 
            * std::array arr{ 1, 2, 3 };
            * 
            * auto result0 = std::vformat("{:{{[}}", std::make_format_args(arr));
            * // The value of result0 is "{[1, 2, 3]}".
            * 
            * auto result1 = std::vformat("{:{{[r}}", std::make_format_args(arr));
            * // The value of result1 is "{[1, 2, 3}]".
            * 
            */

            constexpr auto end = [](char flag) -> bool { return flag == 'Z' || flag == 'z'; };
            /**
            * 
            * Use a pair of `Z` or `z` to wrap a string, making the wrapped string appear only on the right side of the formatted string. Since there is no corresponding left-side string in this case, the special meaning of "$" becomes invalid.
            * Note: Due to the characteristic of left-right matching, if `r` is declared at the end of the format string, the relative left-right position of the strings wrapped by `Z` or `z` will also change accordingly!!!
            * 
            * Example:
            * 
            * std::map map{ std::pair{ 1, "abc" }, { 2, "def" }, { 3, "ghi" } };
            * 
            * auto result = std::vformat("{:{{z, ... za a(Akey: A}{), (key: }{, val: }}", std::make_format_args(map));
            * 
            * // The value of result is "{ (key: 1, val: abc), (key: 2, val: def), (key: 3, val: ghi), ... }".
            * 
            */

            static ::std::string mapping;
        } // namespace zm::fmt::priv::chars

        namespace format {
            [[nodiscard]] inline static constexpr auto containers(const auto& begin, const auto& end, const auto& space, const auto& mapping) noexcept {
                return ::std::accumulate(begin, end, ::std::string{},
                    [space, mapping](::std::string result, const auto& item) constexpr {
                        return ::std::move(result.append(result.empty() ? "" : space)) + ::std::format("{}", zm::fmt::format(mapping, item));
                    });
            }

            [[nodiscard]] inline static constexpr auto maps(const auto& begin, const auto& end, const auto& space, const auto& mapping) noexcept {
                return ::std::accumulate(begin, end, ::std::string{},
                    [space, mapping](::std::string result, const auto& pair) constexpr {
                        return ::std::move(result.append(result.empty() ? "" : space)) + ::std::format("{}{}{}", pair.first, mapping, pair.second);
                    });
            }

            template <size_t... args>
            [[nodiscard]] inline static constexpr auto tuple(const auto& tuple, const ::std::index_sequence<args...>&, const auto& space) noexcept {
                return [tuple, space](auto&& result) constexpr {
                        ((result << (args ? space : "") << ::std::format("{}", ::std::get<args>(tuple))), ...);
                        return result.str();
                    }(::std::move(::std::stringstream{}));
            }
        } // namespace zm::fmt::priv::format

        [[noreturn]] inline static void missing_error(const auto& iter, const auto& context) {
            if (iter == context.end()) { throw ::std::format_error("Missing '}' in format string."); }
        }

        [[noreturn]] inline static void start_error(const auto& iter, const auto& context) {
            if (iter != context.end()) { throw ::std::format_error("The format string must start with '{'."); }
        }

        [[noreturn]] inline static void literal(auto& iter, auto& mapped_spec, const auto& context) {
            if (*iter == chars::literal) {
                ++iter;
                missing_error(iter, context);
            } else if (chars::chars_map.contains(mapped_spec.back())) {
                mapped_spec.back() = chars::chars_map.at(mapped_spec.back());
            }
        }

        [[nodiscard]] inline static constexpr auto push_chars(auto& iter, auto&& self, const auto& context) {
            while (*iter != '}') {
                self.push_back(*iter++);
                missing_error(iter, context);
                if (self.back() == chars::escape) {
                    self.back() = *iter++;
                    missing_error(iter, context);
                }
            }
            return self;
        }

        [[noreturn]] inline static void take_yes(auto& temp_format_spec, auto& temp_mapped_spec, auto& end_strs, auto& mapped_spec) {
            temp_format_spec.pop_back();
            temp_mapped_spec.pop_back();
            for (auto&& strs : end_strs) { mapped_spec.append(strs); }
            mapped_spec.append(temp_mapped_spec);
        }

        [[noreturn]] inline static void take_no(auto& temp_mapped_spec, auto& end_strs, auto& mapped_spec) {
            end_strs.push_back(temp_mapped_spec);
            size_t location{}, size{ end_strs.size() / 2 * 2 };
            for (location = 0; location < end_strs.size(); location += 2) {
                ::std::reverse(end_strs[location].begin(), end_strs[location].end());
            }
            for (location = 0; location < size / 2; location += 2) {
                end_strs[location].swap(end_strs[size - location]);
            }
            for (auto&& strs : end_strs) { mapped_spec.append(strs); }
        }

        [[nodiscard]] inline static auto take_reverse(auto& temp_format_spec, auto& temp_mapped_spec, auto& end_strs, auto& mapped_spec) {
            if (chars::reverse(temp_format_spec.back())) {
                take_yes(temp_format_spec, temp_mapped_spec, end_strs, mapped_spec);
                return temp_format_spec;
            }
            take_no(temp_mapped_spec, end_strs, mapped_spec);
            return temp_format_spec;
        }

        [[noreturn]] inline static void start_or_end(auto& iter, auto& format_start, auto& format_end, auto& end_strs, auto& mapped_spec, const auto& context) {
            if (chars::start(*iter)) {
                ++iter;
                format_end = !format_end;
            } else if (chars::end(*iter)) {
                ++iter;
                format_start = !format_start;
                end_strs.push_back(mapped_spec);
                mapped_spec.clear();
            }
            missing_error(iter, context);
        }

        [[noreturn]] inline static void fmt_start_and_end(auto& format_spec, auto& mapped_spec, auto& iter, const auto& context) {
            format_spec.push_back(*iter);
            mapped_spec.push_back(*iter++);
            missing_error(iter, context);
            if (format_spec.back() == chars::escape) {
                format_spec.back() = *iter;
                mapped_spec.back() = *iter++;
                missing_error(iter, context);
            }
            literal(iter, mapped_spec, context);
        }

        [[noreturn]] inline static void fmt_start_and_not_end(auto& format_spec, auto& iter, const auto& context) {
            // start
            format_spec.push_back(*iter++);
            missing_error(iter, context);
            if (format_spec.back() == chars::escape) {
                format_spec.back() = *iter++;
                missing_error(iter, context);
            }
        }

        [[noreturn]] inline static void fmt_not_start_and_end(auto& mapped_spec, auto& iter, const auto& context) {
            // end
            mapped_spec.push_back(*iter++);
            missing_error(iter, context);
            if (mapped_spec.back() == chars::escape) {
                mapped_spec.back() = *iter++;
                missing_error(iter, context);
            }
        }

        [[noreturn]] inline static void parse_fmt_spec(auto& iter, auto& start, auto& end, auto& end_strs, auto& format_spec, auto& mapped_spec, const auto& context) {
            while (*iter != '}') {
                start_or_end(iter, start, end, end_strs, mapped_spec, context);
                if (start && end) {
                    fmt_start_and_end(format_spec, mapped_spec, iter, context);
                } else if (start && !end) {
                    fmt_start_and_not_end(format_spec, iter, context);
                } else if (!start && end) {
                    fmt_not_start_and_end(mapped_spec, iter, context);
                } else {
                    throw ::std::format_error("`A` and `a` cannot be nested with `Z` and `z`!");
                }
                start_or_end(iter, start, end, end_strs, mapped_spec, context);
            }
        }

        namespace parse {
            /**
            * 
            * Parsing custom formatted strings:
            * A custom formatted string will be divided into three parts, each enclosed in a pair of curly braces `{}`.
            * The first part is used to set the "brackets" around the formatted result.
            * The second part is used to set the string that separates elements within the container.
            * The third part has different roles depending on the type of container being formatted:
            *  - Sequence container: this part of the string will be applied as a formatting rule to each element within the container;
            *  - Mapping container: this part of the string will serve as the separator string for each mapping element;
            *  - `std::pair` or `std::tuple`: this part will have no effect.
            * 
            */
            [[nodiscard]] inline static constexpr auto parse(const auto& context, auto& format_spec, auto& mapped_spec, auto& space, auto& mapping) {
                auto iter{ context.begin() };
                if (iter != context.end() && *iter != '}') {
                    if (*iter++ != '{') {
                        start_error(iter, context);
                        return iter;
                    }

                    // Parse the strings that wrap around the formatted string on both sides. ^^^
                    ::std::string temp_format_spec, temp_mapped_spec;
                    ::std::vector<::std::string> end_strs;
                    bool format_start{ true }, format_end{ true };
                    parse_fmt_spec(iter, format_start, format_end, end_strs, temp_format_spec, temp_mapped_spec, context);
                    mapped_spec.clear();
                    format_spec = take_reverse(temp_format_spec, temp_mapped_spec, end_strs, mapped_spec);
                    // ---------------------------------------------------------------------- vvv

                    auto iter_test{ ++iter };
                    if (++iter_test == context.end()) { return iter; }
                    if (*iter++ != '{') {
                        start_error(iter, context);
                        return iter;
                    }

                    // Parse the spacing strings between formatted elements. ^^^
                    space = push_chars(iter, ::std::string{}, context);
                    // ----------------------------------------------------- vvv

                    iter_test = ++iter;
                    if (++iter_test == context.end()) { return iter; }
                    if (*iter++ != '{') {
                        start_error(iter, context);
                        return iter;
                    }

                    // Parse the spacing strings between mapping pairs. ^^^
                    mapping = push_chars(iter, ::std::string{}, context);
                    // ------------------------------------------------ vvv

                    if (++iter != context.end() && *iter != '}') {
                        // Parse end of format string.
                        throw ::std::format_error("Invalid format string.");
                    }
                }
                return iter;
            }
        } // namespace zm::fmt::priv::parse::parse
        
        template <typename... Types>
        [[nodiscard]] inline static constexpr auto make_fmt_args(Types&&... args) {
            return ::std::make_format_args(::std::forward<Types>(args)...);
        }

        [[nodiscard]] inline static constexpr auto vformat(const auto fmt, auto&&... args) {
            return ::std::vformat(fmt, make_fmt_args(args...));
        }
    } // namespace zm::fmt::priv

    // Format function
    export [[nodiscard]] ::std::string format(const auto fmt, auto&&... args) {
        return priv::vformat(fmt, args...);
    }

    // Print functions
    export [[noreturn]] void print(const ::std::string_view fmt, auto&&... args) {
        ::std::print("{}", format(fmt, args...));
    }

    export [[noreturn]] void println(const ::std::string_view fmt, auto&&... args) {
        ::std::println("{}", format(fmt, args...));
    }
}// namespace zm::fmt

namespace std {
    // Specialization for `::std::array`.
    export template <typename Ty, size_t size>
    struct formatter<array<Ty, size>> : formatter<string_view> {
    public:
        template <typename Pc = basic_format_parse_context<char>>
        constexpr Pc::iterator parse(type_identity_t<Pc&> context) {
            return zm::fmt::priv::parse::parse(context, format_spec, mapped_spec, space, mapping);
        }

        template <typename FormatContext>
        FormatContext::iterator format(const array<Ty, size>& value, FormatContext& context) const {
            return formatter<string_view>::format(::std::format("{}{}{}", format_spec, zm::fmt::priv::format::containers(value.begin(), value.end(), space, mapping), mapped_spec), context);
        }
    private:
        string format_spec{ "[" };
        string mapped_spec{ "]" };
        string space{ ", " };
        string mapping{ "{}" };
    };

    /**
    * 
    * Example usage:
    * 
    * std::array arr{ 1, 2, 3 };
    * 
    * auto result = std::format("{}", arr);
    * 
    * // The value of result is "[1, 2, 3]".
    * 
    **/

    // Specialization for `::std::span`.
    export template <typename Ty, size_t extent>
    struct formatter<span<Ty, extent>> : formatter<string_view> {
    public:
        template <typename Pc = basic_format_parse_context<char>>
        constexpr Pc::iterator parse(type_identity_t<Pc&> context) {
            return zm::fmt::priv::parse::parse(context, format_spec, mapped_spec, space, mapping);
        }

        template <typename FormatContext>
        FormatContext::iterator format(const span<Ty, extent>& value, FormatContext& context) const {
            return formatter<string_view>::format(::std::format("{}{}{}", format_spec, zm::fmt::priv::format::containers(value.begin(), value.end(), space, mapping), mapped_spec), context);
        }
    private:
        string format_spec{ "[" };
        string mapped_spec{ "]" };
        string space{ ", " };
        string mapping{ "{}" };
    };

    /**
    * 
    * Example usage:
    * 
    * std::array arr{ 1, 2, 3, 4, 5 };
    * std::span span(arr.begin() + 1, arr.end() - 1);
    * 
    * auto result = std::format("{}", span);
    * 
    * // The value of result is "[2, 3, 4]".
    * 
    **/

    // Specialization for `::std::{ vector, deque, forward_list, list }`.
    export template <zm::fmt::priv::concepts::seque Seque>
    struct formatter<Seque> : formatter<string_view> {
    public:
        template <typename Pc = basic_format_parse_context<char>>
        constexpr Pc::iterator parse(type_identity_t<Pc&> context) {
            return zm::fmt::priv::parse::parse(context, format_spec, mapped_spec, space, mapping);
        }

        template <typename FormatContext>
        FormatContext::iterator format(const Seque& value, FormatContext& context) const {
            return formatter<string_view>::format(::std::format("{}{}{}", format_spec, zm::fmt::priv::format::containers(value.begin(), value.end(), space, mapping), mapped_spec), context);
        }
    private:
        string format_spec{ "[" };
        string mapped_spec{ "]" };
        string space{ ", " };
        string mapping{ "{}" };
    };

    /**
    * 
    * Example usage:
    * 
    * std::vector seque0{ 'a', 'b', 'c'};
    * std::deque seque1{ 'd', 'e', 'f'};
    * std::forward_list seque2{ 'g', 'h', 'i'};
    * std::list seque3{ 'j', 'k', 'l'};
    * 
    * auto result = std::format("{}", std::tuple{ seque0, seque1, seque2, seque3 });
    * 
    * // The value of result is "([a, b, c], [d, e, f], [g, h, i], [j, k, l])".
    * 
    **/

    // Specialization for `::std::{ set, multiset, unordered_set, unordered_multiset }`.
    export template <zm::fmt::priv::concepts::sets Assoc>
    struct formatter<Assoc> : formatter<string_view> {
    public:
        template <typename Pc = basic_format_parse_context<char>>
        constexpr Pc::iterator parse(type_identity_t<Pc&> context) {
            return zm::fmt::priv::parse::parse(context, format_spec, mapped_spec, space, mapping);
        }

        template <typename FormatContext>
        FormatContext::iterator format(const Assoc& value, FormatContext& context) const {
            return formatter<string_view>::format(::std::format("{}{}{}", format_spec, zm::fmt::priv::format::containers(value.begin(), value.end(), space, mapping), mapped_spec), context);
        }
    private:
        string format_spec{ "{" };
        string mapped_spec{ "}" };
        string space{ ", " };
        string mapping{ "{}" };
    };

    /**
    * 
    * Example usage:
    * 
    * std::set set0{ 'a', 'b', 'c'};
    * std::multiset set1{ 'd', 'e', 'f'};
    * std::unordered_set set2{ 'g', 'h', 'i'};
    * std::unordered_multiset set3{ 'j', 'k', 'l'};
    * 
    * auto result0 = std::format("{}", std::pair{ set0, set1 });
    * auto result1 = std::format("{}", std::pair{ set2, set3 });
    * 
    * // The value of result0 is "({a, b, c}, {d, e, f})".
    * // One possible value of result1 is "({g, h, i}, {j, k, l})".
    * 
    **/

    // Specialization for `::std::{ map, multimap, unordered_map, unordered_multimap }`.
    export template <zm::fmt::priv::concepts::maps Assoc>
    struct formatter<Assoc> : formatter<string_view> {
    public:
        template <typename Pc = basic_format_parse_context<char>>
        constexpr Pc::iterator parse(type_identity_t<Pc&> context) {
            return zm::fmt::priv::parse::parse(context, format_spec, mapped_spec, space, mapping);
        }

        template <typename FormatContext>
        FormatContext::iterator format(const Assoc& value, FormatContext& context) const {
            return formatter<string_view>::format(::std::format("{}{}{}", format_spec, zm::fmt::priv::format::maps(value.begin(), value.end(), space, mapping), mapped_spec), context);
        }
    private:
        string format_spec{ "{(" };
        string mapped_spec{ ")}" };
        string space{ "), (" };
        string mapping{ " -> " };
    };

    /**
    * 
    * Example usage:
    * 
    * std::map map0{ std::pair{ 'a', 1 }, { 'b', 2 }, { 'c', 3 } };
    * std::multimap map1{ std::pair{ 'd', 1 }, { 'e', 2 }, { 'f', 3 } };
    * std::unordered_map map2{ std::pair{ 'g', 1 }, { 'h', 2 }, { 'i', 3 } };
    * std::unordered_multimap map3{ std::pair{ 'j', 1 }, { 'k', 2 }, { 'l', 3 } };
    * 
    * auto result0 = std::format("{}", std::pair{ map0, map1 });
    * auto result1 = std::format("{}", std::pair{ map2, map3 });
    * 
    * // The value of result0 is "({(a -> 1), (b -> 2), (c -> 3)}, {(d -> 1), (e -> 2), (f -> 3)})".
    * // One possible value of result1 is "({(g -> 1), (h -> 2), (i -> 3)}, {(j -> 1), (k -> 2), (l -> 3)})".
    * 
    **/

    // Specialization for `::std::pair`.
    export template <typename Ty1, typename Ty2>
    struct formatter<pair<Ty1, Ty2>> : formatter<string_view> {
    public:
        template <typename Pc = basic_format_parse_context<char>>
        constexpr Pc::iterator parse(type_identity_t<Pc&> context) {
            return zm::fmt::priv::parse::parse(context, format_spec, mapped_spec, space, zm::fmt::priv::chars::mapping);
        }

        template <typename FormatContext>
        FormatContext::iterator format(const pair<Ty1, Ty2>& value, FormatContext& context) const {
            return formatter<string_view>::format(::std::format("{}{}{}{}{}", format_spec, value.first, space, value.second, mapped_spec), context);
        }
    private:
        string format_spec{ "(" };
        string mapped_spec{ ")" };
        string space{ ", " };
    };

    /**
    * 
    * Example usage:
    * 
    * std::pair pair{ 1, 2 };
    * 
    * auto result = std::format("{}", pair);
    * 
    * // The value of result is "(1, 2)".
    * 
    **/

    // Specialization for `::std::tuple`.
    export template <typename... Rest>
    struct formatter<tuple<Rest...>> : formatter<string_view> {
    public:
        template <typename Pc = basic_format_parse_context<char>>
        constexpr Pc::iterator parse(type_identity_t<Pc&> context) {
            return zm::fmt::priv::parse::parse(context, format_spec, mapped_spec, space, zm::fmt::priv::chars::mapping);
        }

        template <typename FormatContext>
        FormatContext::iterator format(const tuple<Rest...>& value, FormatContext& context) const {
            return formatter<string_view>::format(::std::format("{}{}{}", format_spec, zm::fmt::priv::format::tuple(value, make_index_sequence<tuple_size_v<tuple<Rest...>>>(), space), mapped_spec), context);
        }
    private:
        string format_spec{ "(" };
        string mapped_spec{ ")" };
        string space{ ", " };
    };

    /**
    * 
    * Example usage:
    * 
    * std::tuple tuple{ 1, 'A', "Hello, world!", 3.14 };
    * 
    * auto result = std::format("{}", tuple);
    * 
    * // The value of result is "(1, A, Hello, world!, 3.14)".
    * 
    **/
} // namespace std

#pragma warning(pop)
