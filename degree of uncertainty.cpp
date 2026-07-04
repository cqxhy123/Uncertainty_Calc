// 计算不确定度（类型A、类型B），按要求：
// 最终总不确定度除以其对应物理量 = sqrt( sum( (u_i / x_i)^2 ) )
#include <bits/stdc++.h>
using namespace std;

// 读取并解析为 long double，支持科学计数法（1e-3 等）
long double read_ld(const string &prompt){
    string s;
    cout<<prompt;
    while(true){
        if(!(cin >> s)){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"输入无效，请重新输入：";
            continue;
        }
        try{
            size_t idx = 0;
            long double v = stold(s, &idx);
            if(idx != s.size()) throw invalid_argument("extra chars");
            return v;
        } catch(...) {
            cout<<"输入无效，请使用数字或科学计数法（例如 1e-3），重新输入：";
        }
    }
}

// 科学计数法格式化，12 位小数（即较高精度）
string fmt_sci(long double v){
    ostringstream oss;
    oss<<scientific<<setprecision(12)<<v;
    return oss.str();
}

int main(){
    cout<<"作者：川崎小黄鸭\n说明：本程序用于计算合成不确定度，需手动输入变量名称、对应a、b类不确定度和乘的系数（比如2）\n简化不确定度计算（合成规则： uc/f = sqrt( sum( (u_i / x_i)^2 ) ) ）\n";
    int n = (int)read_ld("请输入分物理量个数 n = ");
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 吸收换行，保证后续 getline 可用

    vector<string> name(n);
    vector<long double> xi(n), uA(n), uB(n), ux(n), urel(n), coeff(n);

    for(int i=0;i<n;i++){
        cout<<"\n--- 第 "<<(i+1)<<" 个分量 ---\n";
        // 要求名称必须输入且不能全为空白
        string tmp;
        auto is_blank = [](const string &s)->bool {
            for(char c: s) if(!isspace((unsigned char)c)) return false;
            return true;
        };
        while (true) {
            cout<<"名称（必须输入，不可留空）：";
            getline(cin, tmp);
            if (tmp.empty() || is_blank(tmp)) {
                cout<<"名称不能为空或全为空格，请重新输入。\n";
                continue;
            }
            break;
        }
        name[i] = tmp;

        xi[i] = read_ld("测量值 x = ");
        uA[i] = read_ld("类型A不确定度 uA = ");
        uB[i] = read_ld("类型B不确定度 uB = ");
        cout<<"该分量合成不确定度需乘的系数（默认输入1 表示不乘） coeff = ";
        coeff[i] = read_ld("");

        // 合成该变量的不确定度（A与B合成后乘系数）
        ux[i] = sqrt((long double)(uA[i]*uA[i] + uB[i]*uB[i]));
        ux[i] *= coeff[i];

        if (fabsl(xi[i]) > 0.0L) {
            urel[i] = ux[i] / fabsl(xi[i]);
        } else {
            cout<<"注意：x = 0，无法计算相对不确定度。请输入该分量的相对不确定度 u_i/x_i 手动值：";
            urel[i] = read_ld("");
        }
    }

    long double mainVal = read_ld("\n请输入主物理量的测量值（f） = ");

    // 高精度合成（long double）
    long double rel2 = 0.0L;
    for(int i=0;i<n;i++) rel2 += urel[i]*urel[i];
    long double rel_total = sqrt((long double)rel2);
    long double uc = fabsl(mainVal) * rel_total;

    // 输出结果（均用科学计数法，高精度显示）
    cout<<"\n--- 计算结果 ---\n";
    cout<<left<<setw(12)<<"名称"
        <<setw(26)<<"x (sci)"
        <<setw(24)<<"uA (sci)"
        <<setw(24)<<"uB (sci)"
        <<setw(26)<<"u_x (sci)"
        <<setw(26)<<"u_i/x_i (sci)"<<'\n';

    for(int i=0;i<n;i++){
        cout<<left<<setw(12)<<name[i]
            <<setw(26)<<fmt_sci(xi[i])
            <<setw(24)<<fmt_sci(uA[i])
            <<setw(24)<<fmt_sci(uB[i])
            <<setw(26)<<fmt_sci(ux[i])
            <<setw(26)<<fmt_sci(urel[i])<<'\n';
    }

    cout<<"\n主物理量 f = "<<fmt_sci(mainVal)<<"\n";
    cout<<"合成标准不确定度 uc = "<<fmt_sci(uc)<<"   (科学计数法显示)\n";
    if (fabsl(mainVal) > 0.0L) {
        cout<<"相对不确定度 uc/f = "<<fmt_sci(uc/fabsl(mainVal))<<"\n";
    } else {
        cout<<"相对不确定度 uc/f = N/A (f = 0)\n";
    }

    return 0;
}