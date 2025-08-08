#include "TextBank.h"


// �Լ� �ñ״�ó ����: ���͸� ���۷����� �޾Ƽ� ���� �����ϰų�, ���͸� ��ȯ
// "n"�� ���� ���� ���ڷ� ��ü�ϴ� �Լ�
string TextBank::replaceGeneration(const string& text, int generation)
{
    string result = text;
    string target = " x��";
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
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cout << "������ �� �� �����ϴ�: " << filename << endl;
        return;
    }

    // ��� ��ŵ
    getline(file, line);

    while (getline(file, line)) {
        istringstream ss(line);
        string field;
        Ending ending_data;

        // ID - ������ ��ȯ
        if (getline(ss, field, '\t')) {
            try {
                // ���� ����
                field.erase(0, field.find_first_not_of(" \t\r\n"));
                field.erase(field.find_last_not_of(" \t\r\n") + 1);

                if (!field.empty()) {
                    ending_data.ID = stoi(field);
                }
                else {
                    ending_data.ID = 0; // �⺻��
                }
            }
            catch (const exception& e) {
                cout << "ID ��ȯ ����: '" << field << "' - " << e.what() << endl;
                ending_data.ID = 0;
            }
        }

        // positive
        getline(ss, ending_data.positive, '\t');

        // negative  
        getline(ss, ending_data.negative, '\t');

        // ��ġ - ������ ��ȯ
        if (getline(ss, field, '\t')) {
            try {
                // ���� ����
                field.erase(0, field.find_first_not_of(" \t\r\n"));
                field.erase(field.find_last_not_of(" \t\r\n") + 1);

                if (!field.empty()) {
                    ending_data.��ġ = stoi(field);
                }
                else {
                    ending_data.��ġ = 0; // �⺻��
                }
            }
            catch (const exception& e) {
                cout << "��ġ ��ȯ ����: '" << field << "' - " << e.what() << endl;
                ending_data.��ġ = 0;
            }
        }

        // ������
        getline(ss, ending_data.������, '\t');

        // ���� �ؽ�Ʈ
        getline(ss, ending_data.�����ؽ�Ʈ, '\t');

        //cout << "==============" << endl;
        //cout << "ID" << ending_data.ID << endl;
        //cout << "positive" << ending_data.positive << endl;
        //cout << "negative" << ending_data.negative << endl;
        //cout << "��ġ" << ending_data.��ġ << endl;
        //cout << "������" << ending_data.������ << endl;
        //cout << "�����ؽ�Ʈ" << ending_data.�����ؽ�Ʈ << endl;

        EndingVector.push_back(ending_data);
    }

    file.close();
    cout << "���� �ؽ�Ʈ" << endl;
}


