//
// Created by lizgao on 4/18/20.
//

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <unordered_map>
#include <tuple>
#include <stdlib.h>
#include <stack>
#include <sys/resource.h>

using namespace std;


namespace {

    void set_stack() {
        const rlim_t kStackSize = 64 * 1024 * 1024;   // min stack size = 16 MB
        struct rlimit rl;
        int result;

        result = getrlimit(RLIMIT_STACK, &rl);
        if (result == 0)
        {
            if (rl.rlim_cur < kStackSize)
            {
                rl.rlim_cur = kStackSize;
                result = setrlimit(RLIMIT_STACK, &rl);
                if (result != 0)
                {
                    fprintf(stderr, "setrlimit returned result = %d\n", result);
                }
            }
        }
    }

    class Solution {
        struct Stats {
            int offset;
            bool push_a;
            bool push_b;
            int width;
            int vector_idx;
        };

    public:
        Solution(const string &c, int n, int m) : c_(c), n_(n), m_(m) {
            build_table();
        }
        bool build_table() {
            for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < 10; ++j) {
                    if(i != j) {
                        table_[i*j].push_back({i,j});
                    } else {
                        bool found = false;
                        for (const auto &t : table_[i*j]) {
                            if (t.first == i) {
                                found = true;
                            }
                        }
                        if(!found) {
                            table_[i*j].push_back({i,j});
                        }
                    }
                }
            }
        }
        bool Solve(string &a, string &b) {
            stack<Stats> stack_status;
            int offset = 0;
            int level = 0;
            int k = 1;
            int array_idx = 0;

            while (offset <c_.length() && level < m_*n_) {
                int x = level / m_;
                int y = level % m_;
                int v = 0;

                if (k == 1) {
                    v = (c_[offset] - '0');
                } else {
                    v = (c_[offset] - '0') * 10 + (c_[offset + 1] - '0');
                    if(v==0) {
                        //restore
                        if (stack_status.empty()) {
                            return false;
                        }
                        offset = stack_status.top().offset;
                        level -= 1;
                        if(stack_status.top().push_a) {
                            a.pop_back();
                        }
                        if(stack_status.top().push_b) {
                            b.pop_back();
                        }
                        k=stack_status.top().width;
                        array_idx = stack_status.top().vector_idx;
                        stack_status.pop();

                        continue;
                    }
                }
                auto it = table_.find(v);
                if (it == table_.end()) {
                    //restore
                    if (stack_status.empty()) {
                        return false;
                    }
                    offset = stack_status.top().offset;
                    level -= 1;
                    if(stack_status.top().push_a) {
                        a.pop_back();
                    }
                    if(stack_status.top().push_b) {
                        b.pop_back();
                    }
                    k=stack_status.top().width;
                    array_idx = stack_status.top().vector_idx;
                    stack_status.pop();

                    continue;
                }
                bool need_break = false;
                for (int i=array_idx; i < it->second.size(); i++) {
                    auto &p = it->second[i];
                    cout << "v=" << v <<",p=" << p.first << "/" << p.second << endl;
                    bool push_a = (a.length() < x + 1);
                    bool push_b = (b.length() < y + 1);
                    if (push_a && push_b) {
                        a.push_back(p.first + '0');
                        b.push_back(p.second + '0');
                    } else if (!push_a && push_b) {
                        if (a[x] != p.first + '0') {
                            continue;
                        }
                        b.push_back(p.second + '0');
                    } else if (push_a && !push_b) {
                        if (b[y] != p.second + '0') {
                            continue;
                        }
                        a.push_back(p.first + '0');
                    } else {
                        //last line
                        //cout << "x=" << x<<",y="<<y<< ",a=" << a << ",b="<<b << ",p="<<p.first << "/" << p.second << ",v="<<v<< endl;
                        if (a[x] != p.first + '0' || b[y] != p.second + '0') {
                            continue;
                        }
                        if (x==n_-1 && y==m_-1 && offset + k == c_.length() && level+1 == (n_*m_)) {
                            return true;
                        }
                    }
                    stack_status.push({offset,  push_a, push_b, (k==1)?2:1, i+1});
                    offset = offset + k;
                    level += 1;
                    k = (k==1)?2:1;
                    array_idx = i+1;
                    need_break = true;
                    break;
                }
                if(!need_break) {
                    if (stack_status.empty()) {
                        return false;
                    }
                    offset = stack_status.top().offset;
                    level -= 1;
                    if(stack_status.top().push_a) {
                        a.pop_back();
                    }
                    if(stack_status.top().push_b) {
                        b.pop_back();
                    }
                    k=stack_status.top().width;
                    array_idx = stack_status.top().vector_idx;
                    stack_status.pop();
                }
            }
            if (offset == c_.length() && level == m_*n_) {
                return true;
            }
            return false;
        }


        bool Solve(int offset, string &a, string &b, int level) const{
            //cout << "level=" << level << '\n';
            //cout << "offset=" << offset << ",a=" << a << ",b=" << b << ",level=" << level << endl;
            if (offset == c_.length() && level == m_*n_) {
                return true;
            }
            int bytes_left = c_.length() - offset;
            int level_left = m_*n_ - level;
            if(bytes_left < level_left && bytes_left > 2 * level_left) {
                return false;
            }
            bool only_k_1 = false;
            bool only_k_2 = false;
            if(bytes_left == level_left) {
                only_k_1 = true;
            } else if (bytes_left == 2 * level_left) {
                only_k_2 = true;
            }

            if(level == m_*n_) {
                return false;
            }
            if(offset == c_.length()) {
                return false;
            }
            int x = level / m_;
            int y = level % m_;
            int v = 0;
            for (int k = 1; k <= 2; ++k) {
                if (only_k_1 && k==2) {
                    continue;
                }
                if (only_k_2 && k==1) {
                    continue;
                }
                if (k == 1) {
                    v = (c_[offset] - '0');
                } else {
                    v = (c_[offset] - '0') * 10 + (c_[offset + 1] - '0');
                    if(v<10 || v > 81) {
                        continue;
                    }
                }
                auto it = table_.find(v);
                if (it == table_.end()) {
                    continue;
                }
                for (const auto &p : it->second) {
                    //cout << "v=" << v <<",p=" << p.first << "/" << p.second << endl;
                    bool push_a = (a.length() < x + 1);
                    bool push_b = (b.length() < y + 1);
                    if (push_a && push_b) {
                        a.push_back(p.first + '0');
                        b.push_back(p.second + '0');
                    } else if (!push_a && push_b) {
                        if (a[x] != p.first + '0') {
                            continue;
                        }
                        b.push_back(p.second + '0');
                    } else if (push_a && !push_b) {
                        if (b[y] != p.second + '0') {
                            continue;
                        }
                        a.push_back(p.first + '0');
                    } else {
                        //last line
                        //cout << "x=" << x<<",y="<<y<< ",a=" << a << ",b="<<b << ",p="<<p.first << "/" << p.second << ",v="<<v<< endl;
                        if (a[x] != p.first + '0' || b[y] != p.second + '0') {
                            continue;
                        }
                        if (x==n_-1 && y==m_-1 && offset + k == c_.length() && level+1 == (n_*m_)) {
                        //if (x==n_-1 && y==m_-1) {
                            //cout << "offset=" << offset << ",k=" << k <<", len" <<c_.length() <<endl;
                            return true;
                        }
                    }
                    if (Solve(offset + k, a, b, level + 1)) {
                        return true;
                    }
                    if (push_a) {
                        a.pop_back();
                    }
                    if(push_b) {
                        b.pop_back();
                    }
                }
            }
            return false;
        }

    private:
        unordered_map<int, vector<pair<int, int>>> table_;
        const string &c_;
        int n_;
        int m_;
    };

    string build_c(const string &a, const string &b) {
        string c;
        c.reserve(200000);
        for (auto x : a) {
            for (auto y : b) {
                int v = (x-'0') * (y - '0');
                if(v/10 != 0) {
                    c.push_back(v/10 + '0');
                }
                c.push_back(v%10 + '0');
            }
        }
        return c;
    }
    string build_case(const string &a, const string &b) {
        string c;
        c += std::to_string(a.length());
        c.push_back(' ');
        c += std::to_string(b.length());
        c.push_back('\n');
        return c + build_c(a, b);
    }


    int my_power(int k) {
        int p=1;
        for (int i = 0; i < k; ++i) {
            p *=10;
        }
        return p;
    }
    void unit_test() {
        srand (time(NULL));
        for (int i = 0; i < 10000; ++i) {

            int int_a = rand()%100+1;
            int int_b = rand()%100+1;
            string a = to_string(int_a);
            string b = to_string(int_b);
            string c = build_c(a, b);
            string result_a;
            string result_b;
            auto result = Solution(c, a.length(), b.length()).Solve(0, result_a, result_b, 0);
            if(!result || a != result_a || b!=result_b) {
                cout << "a=" << a << ", b=" << b << ",c=" << c << ",result="<<result_a << "," << result_b <<",result="<<result<< endl;
            } else {
                //cout << "Success!" << endl;
                //cout << "a=" << a << ", b=" << b << ",c=" << c << ",result="<<result_a << "," << result_b << endl;
            }
        }


    }
}


void magic_multip() {
    set_stack();
    unit_test();
    //cout << build_case("197", "122") << endl;
    //cout << build_case("9", "197") << endl;
    //cout << build_case("34", "10") << endl;
    //cout << build_case("68", "50") << endl;

    string aa(100, '3');
    string bb(1000, '4');
    string cc = build_c(aa, bb);
    cout << "len(cc)=" << cc.length() << '\n';
    string a,b;
    a.reserve(100000);
    b.reserve(100000);
    auto result = Solution(cc, aa.length(), bb.length()).Solve(0, a, b, 0);
    if(result) {
        cout << "a=" << a << ", b=" << b << '\n';
    } else {
        cout << "Impossible" << endl;
    }
    a.clear();
    b.clear();
#if 0
    result = Solution("80164248120012024636121800801642481200120246361218008016424812000000000000000000000080164248120012024636121800", 9, 10).Solve(0, a, b, 0);
    if(result) {
        cout << "a=" << a << ", b=" << b << endl;
    } else {
        cout << "Impossible" << endl;
    }
    a.clear();
    b.clear();

    cout << build_case("6466518", "2") << endl;
    string a,b;
    auto result = Solution("128121210216", 7, 1).Solve(0, a, b, 0);
    if(result) {
        cout << "a=" << a << ", b=" << b << endl;
    } else {
        cout << "Impossible" << endl;
    }
    a.clear();
    b.clear();
    result = Solution("0", 1, 4).Solve(0, a, b, 0);
    if(result) {
        cout << "a=" << a << ", b=" << b << endl;
    } else {
        cout << "Impossible" << endl;
    }
    a.clear();
    b.clear();
    result = Solution("100000001000", 3, 4).Solve(0, a, b, 0);
    if(result) {
        cout << "a=" << a << ", b=" << b << endl;
    } else {
        cout << "Impossible" << endl;
    }
    a.clear();
    b.clear();
    result = Solution("8101215", 2, 2).Solve(0, a, b, 0);
    if(result) {
        cout << "a=" << a << ", b=" << b << endl;
    } else {
        cout << "Impossible" << endl;
    }
    a.clear();
    b.clear();
    result = Solution("80101215", 2, 2).Solve(0, a, b, 0);
    if(result) {
        cout << "a=" << a << ", b=" << b << endl;
    } else {
        cout << "Impossible" << endl;
    }
#endif
#if 0
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int case_cnt = 0;
    int n = 0;
    int m = 0;
    cin >> case_cnt;
    string c;
    string a,b;
    for (int i=0;i<case_cnt;i++) {
        a.clear();
        b.clear();
        c.clear();
        cin >> n;
        cin >> m;
        cin >> c;
        auto result = Solution(c, n, m).Solve(0, a, b, 0);
        if(result) {
            cout << a << " " << b << endl;
        } else {
            cout << "Impossible" << endl;
        }
    }
#endif
}