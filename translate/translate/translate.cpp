#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    std::ifstream ifs;
    string topic[6] = {"sampel01.txt","sampel02.txt" ,"sampel03.txt" ,"sampel04.txt" ,"sampel05.txt" ,"sampel06.txt"};
    int wq;
    cout << "請選擇第幾題" << endl;
    while (cin >> wq )
    {
        int n = 0;
        if (wq > 6) {
            return 1;
        }
        string input = "..\\topic\\";
        int address[256] = { 0 };
        address[0] = 0;
        char read[256] = { 0 };
        string Introduction[256];
        string object[256];
        input += topic[wq-1];
        ifs.open(input);
        if (ifs.fail()) {
            cout << "Failed to open file.\n";
            return 1;
        }

        while (!ifs.eof()) {                                        //一行一行抓檔案內容
            ifs.getline(read, sizeof(read));
            Introduction[n] = read;
            n++;
        }
        ifs.close();
        for (int i = 0; i < n-1; i++) {                             //設定位址
            size_t found1 = Introduction[i].find("WORD");
            size_t found2 = Introduction[i].find("BYTE");
            size_t found3 = Introduction[i].find("RESW");
            size_t found4 = Introduction[i].find("RESB");
            if (found1 != string::npos) {                           //遇到WORD
                size_t comma = Introduction[i].find(",");
                if (comma != string::npos) {
                    int time=1;
                    string row;
                    for (int j = 0; j <= Introduction[i].length(); j++) {
                        row = row.assign(Introduction[i], j, 1);
                        if (row == ",") {
                            time++;
                        }
                    }
                    address[i + 1] = address[i] + time * 4;
                }
                else {
                    address[i + 1] = address[i] + 4;
                }

            }else if (found2 != string::npos) {                        //遇到BYTE
                size_t comma = Introduction[i].find(",");
                if (comma != string::npos) {
                    int time = 1;
                    string row;
                    for (int j = 0; j <= Introduction[i].length(); j++) {
                        row = row.assign(Introduction[i], j, 1);
                        if (row == ",") {
                            time++;
                        }
                    }
                    address[i + 1] = address[i] + time * 1;
                }
                else {
                    address[i + 1] = address[i] + 1;
                }

            }else if (found3 != string::npos) {                        //遇到RESW
                string svalue;
                svalue = svalue.assign(Introduction[i], 8, Introduction[i].length() - 8);
                int ivalue = stoi(svalue);
                address[i + 1] = address[i] + ivalue*4;

            }else if (found4 != string::npos) {                         //遇到RESB
                string svalue;
                svalue = svalue.assign(Introduction[i], 8,Introduction[i].length()-8);
                int ivalue = stoi(svalue);
                address[i + 1] = address[i] + ivalue;

            }else {                                                     //正常情況
                address[i+1] = address[i] + 4;

            }
        }
        char address16[256][4];                                     //位址轉16進位
        for (int i = 0; i <= 20; i++) {
            for (int j = 0; j <= 3; j++) {
                address16[i][j] = '0';
            }
        }
        int time = 0;
        for (int i = 1; i <= 256; i++) {
            if (address[i] == 0) {
                break;
            }
            int ten = 48;
            while (address[i] >= 16)
            {
                time++;
                ten ++;
                address[i] -= 16;
                address16[i][2] = ten;
            }
            if (address[i] == 10)
            {
                address16[i][3] = 'A';
            }
            else if (address[i] == 11)
            {
                address16[i][3] = 'B';
            }
            else if (address[i] == 12)
            {
                address16[i][3] = 'C';
            }
            else if (address[i] == 13)
            {
                address16[i][3] = 'D';
            }
            else if (address[i] == 14)
            {
                address16[i][3] = 'E';
            }
            else if (address[i] == 15)
            {
                address16[i][3] = 'F';
            }
            else {
                address16[i][3] = address[i]+48;
            }
            address[i] += time * 16;
            time = 0;
        }

        for (int i = 0; i <= 20; i++) {
            if (address16[i][2] == '0' && i > 4) {
                break;
            }
            size_t found1 = Introduction[i].find("LD");
            size_t found2 = Introduction[i].find("ST");
            size_t found3 = Introduction[i].find("RET");
            size_t found4 = Introduction[i].find("LDI");
            size_t found5 = Introduction[i].find("ADD");
            size_t found6 = Introduction[i].find("SUB");
            size_t found7 = Introduction[i].find("CMP");
            size_t found8 = Introduction[i].find("JNE");
            size_t found9 = Introduction[i].find("JLT");
            size_t found10= Introduction[i].find("JMP");
            size_t found11= Introduction[i].find("MUL");
            size_t found12= Introduction[i].find("LDB");
            size_t found13= Introduction[i].find("JEQ");
            size_t found14= Introduction[i].find("WORD");
            size_t found15= Introduction[i].find("BYTE");
            size_t found16= Introduction[i].find("RESW");
            size_t found17= Introduction[i].find("RESB");
            if (found12!= string::npos) {                               //目的碼(LDB)
                object[i] = "02";
                if (Introduction[i].find("FOR:",0) != 0) {
                    object[i].append(Introduction[i].substr(Introduction[i].find("R", 1) + 1, 1));
                }
                else {
                    object[i].append(Introduction[i].substr(Introduction[i].find("R", 0) + 1, 1));
                }
                object[i].append(Introduction[i].substr(Introduction[i].find("R", 0) + 1, 1));
                for (int j = 0; j <= 20; j++) {
                    if (address[j] == 0) {
                        continue;
                    }
                    if (Introduction[j].find(Introduction[i].substr(Introduction[i].find(",") + 2)) == 0) {
                        int f = (address[j] - address[i + 1]);
                        object[i].append("f");
                        object[i].append("0");
                        object[i].append("0");
                        int time = 0;
                        while (f >= 16) {
                            f -= 16;
                            time++;
                        }
                        char ten = time + '0';
                        object[i].append(1, ten);
                        if (f > 9) {
                            char Digits = f + '0' + 7;
                            object[i].append(1, Digits);
                        }
                        else {
                            char Digits = f + '0';
                            object[i].append(1, Digits);
                        }
                        break;
                    }
                }

                if (object[i].length() < 8) {
                    object[i].append(Introduction[i].substr(Introduction[i].find("[R", 0) + 2, 1));
                    object[i].append("0000");
                }
            }else if (found2 != string::npos) {                         //目的碼(ST)
                object[i] = "01";
                object[i].append(Introduction[i].substr(Introduction[i].find("R", 0)+1,1));
                for (int j = 0; j <= 20; j++) {
                    if (address[j] == 0) {
                        continue;
                    }
                    if (Introduction[j].find(Introduction[i].substr(Introduction[i].find(",") + 2)) == 0) {
                        int f = (address[j] - address[i + 1]);
                        //cout << j << " " << address[j] << " " << i+1 << " " << address[i + 1] << endl;
                        object[i].append("f");
                        object[i].append("0");
                        object[i].append("0");
                        int time = 0;
                        while (f >= 16) {
                            f -= 16;
                            time++;
                        }
                        char ten = time + '0';
                        object[i].append(1, ten);
                        if (f > 9) {
                            char Digits = f + '0' + 7;
                            object[i].append(1, Digits);
                        }
                        else {
                            char Digits = f + '0';
                            object[i].append(1, Digits);
                        }
                        break;
                    }
                }
                if (object[i].length() < 8) {
                    object[i].append(Introduction[i].substr(Introduction[i].find("[R", 0) + 2, 1));
                    object[i].append("0000");
                }
            }else if (found3 != string::npos) {                         //目的碼(RET)
                object[i] = "2C";
                object[i].append("000000");
            }else if (found4 != string::npos) {                         //目的碼(LDI)
                object[i] = "08";
                object[i].append(Introduction[i].substr(Introduction[i].find("R", 0)+1,1));
                object[i].append("0000");
                object[i].append(Introduction[i].substr(Introduction[i].find(",", 0)+2));
            }else if (found5 != string::npos) {                         //目的碼(ADD)
                object[i] = "13";
                object[i].append(Introduction[i].substr(Introduction[i].find("R", 0) + 1, 1));
                object[i].append(Introduction[i].substr(Introduction[i].find("R", 1) + 1, 1));
                object[i].append(Introduction[i].substr(Introduction[i].find("R", 2) + 1, 1));
                object[i].append("000");
            }else if (found6 != string::npos) {                         //目的碼(SUB)
                object[i] = "14";
                object[i].append(Introduction[i].substr(Introduction[i].find("R", 0) + 1, 1));
                object[i].append(Introduction[i].substr(Introduction[i].find("R", 1) + 1, 1));
                object[i].append(Introduction[i].substr(Introduction[i].find("R", 2) + 1, 1));
                object[i].append("000");
            }else if (found7 != string::npos) {                         //目的碼(CMP)
                object[i] = "10";
                if (Introduction[i].find("FOR:", 0) != 0) {
                    object[i].append(Introduction[i].substr(Introduction[i].find("R", 1) + 1, 1));
                }
                else {
                    object[i].append(Introduction[i].substr(Introduction[i].find("R", 0) + 1, 1));
                }
                object[i].append(Introduction[i].substr(Introduction[i].find("R", 0) + 1, 1));
                for (int j = 0; j <= 20; j++) {
                    if (address[j] == 0) {
                        continue;
                    }
                    if (Introduction[j].find(Introduction[i].substr(Introduction[i].find(",") + 2)) == 0) {
                        int f = (address[j] - address[i + 1]);
                        object[i].append("f");
                        object[i].append("0");
                        object[i].append("0");
                        int time = 0;
                        while (f >= 16) {
                            f -= 16;
                            time++;
                        }
                        char ten = time + '0';
                        object[i].append(1, ten);
                        if (f > 9) {
                            char Digits = f + '0' + 7;
                            object[i].append(1, Digits);
                        }
                        else {
                            char Digits = f + '0';
                            object[i].append(1, Digits);
                        }
                        break;
                    }
                }

                if (object[i].length() < 8) {
                    object[i].append(Introduction[i].substr(Introduction[i].find("[R", 0) + 2, 1));
                    object[i].append("0000");
                }
            }else if (found8 != string::npos) {                         //目的碼(JNE)
                object[i] = "21";
                for (int j = 0; j <= 20; j++) {
                    if (address[j] == 0 && j != 0) {
                        continue;
                    }
                    if (Introduction[j].find(Introduction[i].substr(Introduction[i].find("E") + 2)) == 0) {
                        if (address[j] > address[i + 1]) {
                            int f = (address[j] - address[i + 1]);
                            //cout << j << " " << address[j] << " " << i << " " << address[i + 1] << " " << f << endl;
                            object[i].append("f");
                            object[i].append("0");
                            object[i].append("0");
                            object[i].append("0");
                            int time = 0;
                            while (f >= 16) {
                                f -= 16;
                                time++;
                            }
                            char ten = time + '0';
                            object[i].append(1, ten);
                            if (f > 9) {
                                char Digits = f + '0' + 7;
                                object[i].append(1, Digits);
                            }
                            else {
                                char Digits = f + '0';
                                object[i].append(1, Digits);
                            }
                            break;
                        }
                        else {
                            object[i].append("f");
                            object[i].append("f");
                            object[i].append("f");
                            object[i].append("f");
                            int f = (address[i + 1] - address[j]);
                            f = 256 - f;
                            int time = 0;
                            while (f >= 16) {
                                f -= 16;
                                time++;
                            }
                            cout << time << endl;
                            char ten = time + '0' + 7;
                            object[i].append(1, ten);
                            if (f > 9) {
                                char Digits = f + '0' + 7;
                                object[i].append(1, Digits);
                            }
                            else {
                                char Digits = f + '0';
                                object[i].append(1, Digits);
                            }
                            break;
                        }
                    }
                }
            }else if (found9 != string::npos) {                         //目的碼(JLT)
                object[i] = "22";
                for (int j = 0; j <= 20; j++) {
                    if (address[j] == 0 && j != 0) {
                        continue;
                    }
                    if (Introduction[j].find(Introduction[i].substr(Introduction[i].find("T") + 2)) == 0) {
                        if (address[j] > address[i + 1]) {
                            int f = (address[j] - address[i + 1]);
                            //cout << j << " " << address[j] << " " << i << " " << address[i + 1] << " " << f << endl;
                            object[i].append("f");
                            object[i].append("0");
                            object[i].append("0");
                            object[i].append("0");
                            int time = 0;
                            while (f >= 16) {
                                f -= 16;
                                time++;
                            }
                            char ten = time + '0';
                            object[i].append(1, ten);
                            if (f > 9) {
                                char Digits = f + '0' + 7;
                                object[i].append(1, Digits);
                            }
                            else {
                                char Digits = f + '0';
                                object[i].append(1, Digits);
                            }
                            break;
                        }
                        else {
                            object[i].append("f");
                            object[i].append("f");
                            object[i].append("f");
                            object[i].append("f");
                            int f = (address[i + 1] - address[j]);
                            f = 256 - f;
                            int time = 0;
                            while (f >= 16) {
                                f -= 16;
                                time++;
                            }
                            cout << time << endl;
                            char ten = time + '0' + 7;
                            object[i].append(1, ten);
                            if (f > 9) {
                                char Digits = f + '0' + 7;
                                object[i].append(1, Digits);
                            }
                            else {
                                char Digits = f + '0';
                                object[i].append(1, Digits);
                            }
                            break;
                        }
                    }
                }
            }else if (found10 != string::npos) {                        //目的碼(JMP)
                object[i] = "26";
                for (int j = 0; j <= 20; j++) {
                    if (address[j] == 0 && j !=0 ) {
                        continue;
                    }
                    if (Introduction[j].find(Introduction[i].substr(Introduction[i].find("P") + 2)) == 0) {
                        if (address[j] > address[i + 1]) {
                            int f = (address[j] - address[i + 1]);
                            //cout << j << " " << address[j] << " " << i << " " << address[i + 1] << " " << f << endl;
                            object[i].append("f");
                            object[i].append("0");
                            object[i].append("0");
                            object[i].append("0");
                            int time = 0;
                            while (f >= 16) {
                                f -= 16;
                                time++;
                            }
                            char ten = time + '0';
                            object[i].append(1, ten);
                            if (f > 9) {
                                char Digits = f + '0' + 7;
                                object[i].append(1, Digits);
                            }
                            else {
                                char Digits = f + '0';
                                object[i].append(1, Digits);
                            }
                            break;
                        }
                        else {
                            object[i].append("f");
                            object[i].append("f");
                            object[i].append("f");
                            object[i].append("f");
                            int f = (address[i + 1] - address[j]);
                            f = 256 - f;
                            int time = 0;
                            while (f >= 16) {
                                f -= 16;
                                time++;
                            }
                            cout << time << endl;
                            char ten = time + '0'+7;
                            object[i].append(1, ten);
                            if (f > 9) {
                                char Digits = f + '0' + 7;
                                object[i].append(1, Digits);
                            }
                            else {
                                char Digits = f + '0';
                                object[i].append(1, Digits);
                            }
                            break;
                        }
                    }
                }
            }else if (found11!= string::npos) {                         //目的碼(MUL)
                object[i] = "15";
                object[i].append(Introduction[i].substr(Introduction[i].find("R", 0) + 1, 1));
                object[i].append(Introduction[i].substr(Introduction[i].find("R", 1) + 1, 1));
                object[i].append(Introduction[i].substr(Introduction[i].find("R", 2) + 1, 1));
                object[i].append("000");
            }else if (found1!= string::npos) {                          //目的碼(LD)
                int x = 9;
                object[i] = "00";
                size_t d = Introduction[i].find("FOR");
                if (d != string::npos) {
                    object[i].append(Introduction[i],x, 1);
                }else{
                    object[i].append(Introduction[i],x-4 ,1);
                }
                x += 5;
                for (int j = 0; j <= 20; j++) {
                    if (address[j] == 0 && j>0) {
                        continue;
                    }
                    if (Introduction[j].find(Introduction[i].substr(Introduction[i].find(",") + 2)) == 0) {
                        int f = (address[j] - address[i + 1]);
                        object[i].append("f");
                        object[i].append("0");
                        object[i].append("0");
                        int time = 0;
                        while (f >= 16) {
                            f -= 16;
                            time++;
                        }
                        char ten = time + '0';
                        object[i].append(1, ten);
                        if (f > 9) {
                            char Digits = f + '0' + 7;
                            object[i].append(1, Digits);
                        }else{
                            char Digits = f + '0';
                            object[i].append(1, Digits);
                        }
                        break;
                    }
                }
                if (object[i].length() < 8) {
                    size_t d = Introduction[i].find("FOR");
                    if (d != string::npos) {
                        object[i].append(Introduction[i],x, 1);
                    }
                    else {
                        object[i].append(Introduction[i], x-4,1);
                    }
                    object[i].append("0000");
                }
            }else if (found13!= string::npos) {                         //目的碼(JEQ)
                object[i] = "20";
                for (int j = 0; j <= 20; j++) {
                    if (address[j] == 0 && j != 0) {
                        continue;
                    }
                    if (Introduction[j].find(Introduction[i].substr(Introduction[i].find("Q") + 2)) == 0) {
                        if (address[j] > address[i + 1]) {
                            int f = (address[j] - address[i + 1]);
                            //cout << j << " " << address[j] << " " << i << " " << address[i + 1] << " " << f << endl;
                            object[i].append("f");
                            object[i].append("0");
                            object[i].append("0");
                            object[i].append("0");
                            int time = 0;
                            while (f >= 16) {
                                f -= 16;
                                time++;
                            }
                            char ten = time + '0';
                            object[i].append(1, ten);
                            if (f > 9) {
                                char Digits = f + '0' + 7;
                                object[i].append(1, Digits);
                            }
                            else {
                                char Digits = f + '0';
                                object[i].append(1, Digits);
                            }
                            break;
                        }else{
                            object[i].append("f");
                            object[i].append("f");
                            object[i].append("f");
                            object[i].append("f");
                            int f = (address[i + 1] - address[j]);
                            f = 256 - f;
                            int time = 0;
                            while (f >= 16) {
                                f -= 16;
                                time++;
                            }
                            char ten = time + '0'+7;
                            object[i].append(1, ten);
                            if (f > 9) {
                                char Digits = f + '0' + 7;
                                object[i].append(1, Digits);
                            }
                            else {
                                char Digits = f + '0';
                                object[i].append(1, Digits);
                            }
                            break;
                        }
                    }
                }
            }else if (found14!= string::npos) {                         //目的碼(WORD)
                size_t e = Introduction[i].find("a");
                if (e != string::npos) {
                    object[i].append("0000");
                    for (int j = 0; j <= 20; j++) {
                        if (Introduction[j].find("a") == 0) {
                            object[i].append(1,address16[j][0]);
                            object[i].append(1, address16[j][1]);
                            object[i].append(1, address16[j][2]);
                            object[i].append(1, address16[j][3]);
                            break;
                        }
                    }
                    continue;
                }
                size_t d = Introduction[i].find("b");
                if (d != string::npos) {
                    object[i].append("0000");
                    for (int j = 0; j <= 20; j++) {
                        if (Introduction[j].find("b") == 0) {
                            object[i].append(1, address16[j][0]);
                            object[i].append(1, address16[j][1]);
                            object[i].append(1, address16[j][2]);
                            object[i].append(1, address16[j][3]);
                            break;
                        }
                    }
                    continue;
                }
                int time = 1;
                string row;
                for (int j = 0; j <= Introduction[i].length(); j++) {
                    row = row.assign(Introduction[i], j, 1);
                    if (row == ",") {
                        time++;
                    }
                }
                int x = 8;
                for (int j = 1; j <= time; j++) {
                    string svalue , carry;
                    int y;
                    svalue = svalue.assign(Introduction[i], x, 1);
                    carry = carry.assign(Introduction[i], x+1, 1);
                    if (carry=="9") {
                        object[i].append("0000001D");
                        continue;
                    }
                    if (svalue == "D") {
                        svalue = svalue.assign(Introduction[i], x+2, 1);
                        object[i].append("0000000" + svalue );
                        break;
                    }
                    object[i].append("0000000" + svalue);
                    x += 3;
                }
            }else if (found15!= string::npos) {                         //目的碼(BYTE)
                int time = 1;
                string row;
                for (int j = 0; j <= Introduction[i].length(); j++) {
                    row = row.assign(Introduction[i], j, 1);
                    if (row == ",") {
                        time++;
                    }
                }
                int x = 8;
                for (int j = 1; j <= time; j++) {
                    string svalue; 
                    svalue = svalue.assign(Introduction[i], x, 1);
                    object[i].append("0"+svalue);
                    x+=3;
                }
            }else if (found16!= string::npos) {                         //目的碼(RESW)
                string svalue;
                svalue = svalue.assign(Introduction[i], 8, Introduction[i].length() - 8);
                int ivalue = stoi(svalue);
                for (int j = 0; j < ivalue; j++) {
                    object[i].append("00000000");
                }
            }else if (found17!= string::npos) {                         //目的碼(RESB)
                string svalue;
                svalue = svalue.assign(Introduction[i], 8, Introduction[i].length() - 8);
                int ivalue = stoi(svalue);
                for (int j = 0; j < ivalue; j++) {
                    object[i].append("00");
                }
            }

        }
        for (int i = 0; i <= 20; i++) {
            if (address16[i][2] == '0' && i > 4) {
                break;
            }
            for (int j = 0; j <= 3; j++) {
                cout << address16[i][j];
            }
            cout << " " << Introduction[i] << " " << object[i] << endl;
        }
        ofstream ofs;
        string filename = "..\\answer\\";
        char m = wq + '0';
        filename += m;
        filename += ".txt";
        ofs.open(filename);
        for (int j = 0; j <= 20; j++) {
            if (address[j] == 0 && j > 0) {
                continue;
            }
            ofs << object[j] << " ";
        }
        ofs.close();
        cout << "請選擇第幾題" << endl;
    }
    return 0;
}