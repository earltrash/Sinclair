#include "TextBank.h"


// �Լ� �ñ״�ó ����: ���͸� ���۷����� �޾Ƽ� ���� �����ϰų�, ���͸� ��ȯ
// "n"�� ���� ���� ���ڷ� ��ü�ϴ� �Լ�
string TextBank::replaceGeneration(const string& text, int generation)
{
    string result = text;
    string target = " n��";
    string replacement = " " + to_string(generation) + "��";

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
        // �糡 ū����ǥ ����
        if (!s.empty() && s.front() == '"' && s.back() == '"' && s.size() >= 2) {
            s = s.substr(1, s.size() - 2);
        }
        };

    auto parsePositive = [&](const string& raw) -> pair<Status_Total, Status_Total> {
        string s = raw;
        string a, b;
        // ���� Ʈ����
        string t = s;
        trim(t);
        size_t plus = t.find('+');
        if (plus == string::npos) {
            // ���� ����
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
        cout << "������ �� �� �����ϴ�: " << filename << endl;
        return;
    }

    string line;
    // ��� ��ŵ
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

        // 2) positive (A �Ǵ� A+B)
        if (getline(ss, field, '\t')) {
            e.positive = parsePositive(field);
        }

        // 3) negative ("?" ����)
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

        // 6) endingText (����ǥ ���� ����)
        if (getline(ss, field, '\t')) {
            trim(field);
            e.endingText = field;
        }
        else {
            // Ȥ�� �� ���� �� �������� �ؽ�Ʈ�� ���̽� ������
            // ������ �̹� '\t' ������ �а� �־ ���� �� ��.
        }

        // �� �ݵ�� push_back
        EndingVector.push_back(std::move(e));
    }

    file.close();
}