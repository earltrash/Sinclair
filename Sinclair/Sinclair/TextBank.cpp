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
