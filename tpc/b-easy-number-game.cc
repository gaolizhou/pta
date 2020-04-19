//
// Created by lizgao on 4/19/20.
//

#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;
namespace {
    uint64_t min_prooduct(vector<int> &input, int cnt) {
        partial_sort(input.begin(), input.begin() + 2*cnt, input.end());
        uint64_t sum = 0;
        for (int i=0;i<cnt;i++) {
            sum += input[i]*input[2*cnt-1-i];
        }
        return sum;
    }

}
int b_easy_number_game_main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    vector<vector<int>> data;
    vector<int> m_list;
    int case_cnt = 0;
    int data_cnt = 0;
    int m = 0;
    cin >> case_cnt;
    if (case_cnt <=0) {
        return 0;
    }
    for(int i=0;i<case_cnt;i++) {
        cin >> data_cnt;
        cin >> m;
        m_list.push_back(m);
        vector<int> x;
        for(int j=0;j<data_cnt;j++) {
            int t;
            cin >> t;
            x.push_back(t);
        }
        data.push_back(std::move(x));
    }
    for (int i=0;i<case_cnt;i++) {
        cout << min_prooduct(data[i], m_list[i]) << endl;
    }
}
