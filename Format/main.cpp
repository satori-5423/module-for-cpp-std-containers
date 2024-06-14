
import std;
import zm.fmt;

using namespace std;
using namespace zm;

int main() {
    fmt::println(
        "{:"
            "{Aforw#ard_list: A{\n\t\b\b\b\b}"                 // forward_list 左右括号
            "{,\n\t\b\b\b\b}"                                  // forward_list 元素间隔
            "{{:"
                "{Avector: A[Z\t\b\b\b\bZ\n\t| #}"                   // vector 左右括号
                "{ |\n\t| #}"                                        // vector 元素间隔
                "{{:{aNumber: a###}{\t\b\b=>  English: ###}###}#}"   // pair 自定义格式
            "#}}"
        "}",
        forward_list<vector<pair<int, string_view>>>{
            { { 1, "one   " }, {  2, "two   " }, {  3, "three " }, {  4, "four  " } },
            { { 5, "five  " }, {  6, "six   " }, {  7, "seven " }, {  8, "eight " } },
            { { 9, "nine  " }, { 10, "ten   " }, { 11, "eleven" }, { 12, "twelve" } }
        }
    );
    return 0x0;
}
