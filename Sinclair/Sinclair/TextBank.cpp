#include "TextBank.h"


// 함수 시그니처 수정: 벡터를 레퍼런스로 받아서 직접 수정하거나, 벡터를 반환
// "n"을 실제 세대 숫자로 교체하는 함수
string TextBank::replaceGeneration(const string& text, int generation)
{
    string result = text;
    string target = " x세";
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
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cout << "파일을 열 수 없습니다: " << filename << endl;
        return;
    }

    // 헤더 스킵
    getline(file, line);

    while (getline(file, line)) {
        istringstream ss(line);
        string field;
        Ending ending_data;

        // ID - 안전한 변환
        if (getline(ss, field, '\t')) {
            try {
                // 공백 제거
                field.erase(0, field.find_first_not_of(" \t\r\n"));
                field.erase(field.find_last_not_of(" \t\r\n") + 1);

                if (!field.empty()) {
                    ending_data.ID = stoi(field);
                }
                else {
                    ending_data.ID = 0; // 기본값
                }
            }
            catch (const exception& e) {
                cout << "ID 변환 오류: '" << field << "' - " << e.what() << endl;
                ending_data.ID = 0;
            }
        }

        // positive
        getline(ss, ending_data.positive, '\t');

        // negative  
        getline(ss, ending_data.negative, '\t');

        // 명성치 - 안전한 변환
        if (getline(ss, field, '\t')) {
            try {
                // 공백 제거
                field.erase(0, field.find_first_not_of(" \t\r\n"));
                field.erase(field.find_last_not_of(" \t\r\n") + 1);

                if (!field.empty()) {
                    ending_data.명성치 = stoi(field);
                }
                else {
                    ending_data.명성치 = 0; // 기본값
                }
            }
            catch (const exception& e) {
                cout << "명성치 변환 오류: '" << field << "' - " << e.what() << endl;
                ending_data.명성치 = 0;
            }
        }

        // 직업명
        getline(ss, ending_data.직업명, '\t');

        // 엔딩 텍스트
        getline(ss, ending_data.엔딩텍스트, '\t');

        //cout << "==============" << endl;
        //cout << "ID" << ending_data.ID << endl;
        //cout << "positive" << ending_data.positive << endl;
        //cout << "negative" << ending_data.negative << endl;
        //cout << "명성치" << ending_data.명성치 << endl;
        //cout << "직업명" << ending_data.직업명 << endl;
        //cout << "엔딩텍스트" << ending_data.엔딩텍스트 << endl;

        EndingVector.push_back(ending_data);
    }

    file.close();
    cout << "엔딩 텍스트" << endl;
}


