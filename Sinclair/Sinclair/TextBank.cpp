#include "TextBank.h"


// 함수 시그니처 수정: 벡터를 레퍼런스로 받아서 직접 수정하거나, 벡터를 반환
// "n"을 실제 세대 숫자로 교체하는 함수
string TextBank::replaceGeneration(const string& text, int generation)
{
    string result = text;
    string target = " n세";
    string replacement = " " + to_string(generation) + "세";

    size_t pos = 0;
    while ((pos = result.find(target, pos)) != string::npos) {
        result.replace(pos, target.length(), replacement);
        pos += replacement.length();
    }

    return result;
}


void TextBank::parseTSV_Ending(const string& filename)
{
    auto trim = [](string& s) {
        if (s.empty()) return;
        size_t b = s.find_first_not_of(" \t\r\n");
        size_t e = s.find_last_not_of(" \t\r\n");
        if (b == string::npos) { s.clear(); return; }
        s = s.substr(b, e - b + 1);
        // 양끝 큰따옴표 제거
        if (!s.empty() && s.front() == '"' && s.back() == '"' && s.size() >= 2) {
            s = s.substr(1, s.size() - 2);
        }
        };

    auto parsePositive = [&](const string& raw) -> pair<Status_Total, Status_Total> {
        string s = raw;
        string a, b;
        // 먼저 트리밍
        string t = s;
        trim(t);
        size_t plus = t.find('+');
        if (plus == string::npos) {
            // 단일 스탯
            return { stringToStatType(t), Status_Total::Null };
        }
        else {
            a = t.substr(0, plus);
            b = t.substr(plus + 1);
            trim(a); trim(b);
            return { stringToStatType(a), stringToStatType(b) };
        }
        };

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "파일을 열 수 없습니다: " << filename << endl;
        return;
    }

    string line;
    // 헤더 스킵
    if (!getline(file, line)) { return; }

    while (getline(file, line)) {
        if (line.empty()) continue;

        istringstream ss(line);
        string field;
        Ending e{};
        e.ID = 0; e.fame = 0;
        e.positive = { Status_Total::Null, Status_Total::Null };
        e.negative = Status_Total::Null;
        e.job.clear();
        e.endingText.clear();

        // 1) ID
        if (getline(ss, field, '\t')) {
            trim(field);
            try { if (!field.empty()) e.ID = stoi(field); }
            catch (...) { e.ID = 0; }
        }

        // 2) positive (A 또는 A+B)
        if (getline(ss, field, '\t')) {
            e.positive = parsePositive(field);
        }

        // 3) negative ("?" 가능)
        if (getline(ss, field, '\t')) {
            trim(field);
            e.negative = stringToStatType(field);
        }

        // 4) fame
        if (getline(ss, field, '\t')) {
            trim(field);
            try { if (!field.empty()) e.fame = stoi(field); }
            catch (...) { e.fame = 0; }
        }

        // 5) job
        if (getline(ss, field, '\t')) {
            trim(field);
            e.job = field;
        }

        // 6) endingText (따옴표 포함 가능)
        if (getline(ss, field, '\t')) {
            trim(field);
            e.endingText = field;
        }
        else {
            // 혹시 탭 없이 줄 끝까지가 텍스트인 케이스 방지용
            // 위에서 이미 '\t' 단위로 읽고 있어서 보통 안 옴.
        }

        // ★ 반드시 push_back
        EndingVector.push_back(std::move(e));
    }

    file.close();
}