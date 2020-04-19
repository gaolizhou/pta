//
// Created by lizgao on 4/19/20.
//

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

namespace {
    int is_possiable(vector<int> &input) {
        sort(input.begin(), input.end());

        while (*input.begin() != input.back()) {
            int v = input.back() - *input.begin();
            *input.begin() = v;
            input.back() = v;
            sort(input.begin(), input.end());
        }
        return *input.begin();
    }


}


int a_easy_task_main() {
    vector<vector<int>> data;
    int case_cnt = 0;
    int data_cnt = 0;
    cin >> case_cnt;
    if (case_cnt <=0) {
        return 0;
    }
    for(int i=0;i<case_cnt;i++) {
        cin >> data_cnt;
        vector<int> x;
        for(int j=0;j<data_cnt;j++) {
            int t;
            cin >> t;
            x.push_back(t);
        }
        data.push_back(x);
    }
    for (auto &v : data) {
        cout << is_possiable(v) << endl;
    }
    return 0;
}