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
