#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#include <unordered_map>
#include <queue>
#include <array>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <string>
#include <random>
#include <time.h>

//VERSION1.0:����ʵ��alphabeta��֦Ҷ�㷨��������ǰ��ҵʹ�ù���unordered_set���Ż��������̣�ԭ���ǰ������ù�ϣ����ʽ�洢�����͸��Ӷȣ�ͬʱ��������������ʣ��Ѽ�����Χ�۽�������λ�õ�
//����4�񣨱����볢��2�񣬵�Ч���ܲ��ʵ��4���Ѿ��ӽ���ȫ�����ˡ��������ù�ϣ�洢�ܻ��㡣�����Ż�����1.�Ż��洢�ķ�����Ӧ�ô��ڱ����з������õı���������2.�Ż����ֱ�׼��ע�⵽
//������ʱ���ͷ��������ս����Ӱ��ܴ󣬳����뷨����Բ�ͬ������Բ�ͬ���ͽ��ж�̬�ĸ��֣��������ú������ȹ������д������Ż���3.������̱�Ե���㷨�д��Ż�����ǰ�ų�����ȫ�����ܵ�ֵ
//����ӱ���Ĩȥ��
//
//VERSION1.1��1.�۲���ľ����̣������Լ����㷨����Եط�����ʱ��ѡ����ض���׷������������ӷ���ϵ��2���ڵз���3ʱ����3���ı�����2.ͬʱ���������x_xx������ʱ���ᱻ��Ϊ��2���󸳷֡�
// 
//VERSION1.2��1.�Ż���ǰ�����߼�����������Ϊ12*12����board�в��ö�ά����LineInfo���洢12�У�12�У�16�����Խ��ߣ�16�����Խ����Ϻڰ��ӵĸ�ʽ������ʱֻ������ǰ�����������2����/��/�Խ��ߡ�
//2.�۲쵽�����̱�Եʱ����ȥ���Կ�����Ե��λ�ã���ʹ��λ���Ѿ������γ�5����Ŀǰ��������ʱ��Ա�Ե��������Ż������翿����Եʱ���м��֣������죩
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//VERSION 2.0:1.�ڽ���1.2�汾�Ż�ʱ���ڵ�ǰ�Ʒֺ������ڷ�����������������������֡�ͬʱ����ǰ�Ĵ�������ͷ����жϹ��ڼ򵥣������ڱ���������Ϊ�������������ڴ���2�����������ͷḻ����1�����λ
//2�����3�����9�������̱�Ե(�ո���0�Ǳ���ǰ��0Ӱ�������жϣ�����������unordered_map�ķ�ʽ�Ͷ�Ӧ�ķ�����ǰ�洢��������ʱֱ�������ͱ���ж��������֣��ڶ�������ʱ��Ϊ�˱��������������µ�
// ��ʤ�������ǰ�������ͽ��и��֣���ɡ�ɱ�塱���ԡ�
//���£������Ǵ�����ֱ��Ӵunorderedmap���洢�����ܶ�����Ͳ����и��ֵģ����Ƿ�������̶����͵ĳ��ȣ����ڱ���ʱ����ֺܶ���©����������޷����֣�ai˵���Կ���ʹ��Ĭ�ϵ����������������ֻص���ԭ
// ���İ汾�ˣ�Ŀǰ�ҵ��뷨�ǣ�����������Ȼ����ԭ�ȵİ�������ͨ������ֵ�ͷ��������и��֣���ԭ������洢���͵Ĺ�ϣ�������洢��в�������������Ϊ�յĻ����ȵȡ��ڽ��ܵ�place��������������в
// �жϣ����������в�����������alphabeta��֦�㷨��ֱ�ӽ���ɱ�壬������ֶԷ���ʤ�ľ��档
//���£�������ʵֻ��Ҫ�ж��������ϵ��������
// ���Ч�������ˡ�������
// 
// ����Ϊ���ͱ���Ż��㷨�����Ǹ��ĳ����ַ�����������˾����Ե����أ���Ȼ��ʱ����³��������ϵĺ��嵫����Ч���
//һЩ�뷨���Ƿ���Զ��������̵�λ�ý������ֶ����Ƕ��������̵����ͽ������֣��ܷ�����������̵Ĳ�ͬλ�ã����������λ�õĽ������������ط�����Ȼ��ÿ�������Ժ�̬�����Ǹ����Ӹ����ķ������ɡ�
//�������������2ά����Ļ����ϣ���ĳ����λ�������־��Ƕ����������к������Խ��߽�������Ҳ�����������ͱ�ÿ�����ӵĸ��¸�λ�ú���Χ�˸�λ�õķ������ɡ�Ȼ��ͨ�������̵�λ�ý��м�֦��
//
//VERSION 2.1:˼�����������������һ����Ҫ���ǰ˸�������ÿ�����Ӻ�Ҫ���ǵ����ݵȵ����أ��������ð������ֵķ���������ȫ�ع��������жϣ��ù�ϣͼ���յķ�ʽ��ʱ�������룬�ȵ��¸��汾����ƥ����в���ͣ�
// ������в����ֱ���ȴ������������йش��룩
//VERSION 2.2:�������в�жϲ��ִ��룬ÿ��һ�������ҷ���ʤ����ɱ�壬��ɱ��ʱ������ȼ������Ե�����Ŀǰ������������ͬ���ȼ��𣬵��Է����ֱ�ʤ��ʱҲ���е���������ֱ�ӷ�±�����ֱ�ʤ���͡�
//VERSION 2.3:Ϊ��ͬ����вλ�������Ȩ��ռ�ȣ��ڷ���ʱ���ȷ���Ȩ�ظߣ�Σ�գ���λ�á�
// �����Ż����1.�Ż�ab��֦�߼���2.����λ�ô洢���Ż���Ŀǰ��֦�в����¿���λ�õ������õı�������ǰ���¹����ᵯ������������Ǳ����Ը������ݽṹ�洢����λ�ã�ͬʱ�Ҿ���ֻ������Χ2Ȧ��һ������ķ�����
// ���ԸĻ�����3.���ֵ��Ż���������ֱ�Ӵ洢��ÿ������ʱ��ʵֻ��Ҫ�������ĸ����ķ������ɡ�4.��вʶ���㷨���Ż�������˫����в��ʶ��ͬʱ�����п��ÿ�λ���洢��ĳ�����У�������Щ�����вָ������ʱ�ڿ�����ô�㣩
// ѡ����õĵ㡣
// VERSION 2.4:���������ݹ��л���ֵ��������󣬸����˴������λ�õ����ݽṹ���Ż���alphabeta��֦�߼���ɾ��һ����������롣
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// VERSION 3.0
// ��д��alphabeta��֦�㷨��������ʽ���������ȱ������ֽϸߵ�λ�ã���ȫ�ع������ַ���������ϸ�������ƺ����ǲ��þ�ȷ���ֵõ���׼ȷ�ʸ��ߡ�ÿ���������ܷ�������ˢ��4������ķ����������ܷ��У��ܷ���Ϊ����ֱ�Ӵ洢��
// ʡȥ��ÿ�α����Ĺ��̣��Ż������ܣ�alphabeta������Ŀǰ�ݹ��ܴﵽ7�㣬����Zorbist��ֵ���Ż��洢������в���ӻ����ˣ����ʻ�����Ϊ��ǰ�����㷨�����ơ�������ʽ���ּ����˷������֣�Ч�������ǿ���ȳ���һ��7����Ȼ᲻�ᳬʱ��
//

// board information
#define BOARD_SIZE 12
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define INF 2147483647
#define _INF -2147483647
#define MAX_SCORE 10000000
#define MIN_SCORE -10000000

// command information
#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"

const int KILL_POS = 1000;//ɱ��λ��
const int PRIORITY_THREAT = 100; // �����в����
const int HIGH_THREAT = 10; // ����в����
const int NOMAL_THREAT = 1; //һ����в


const int SCORE_FIVE = 500000;
const int SCORE_FOUR = 40000;//��ͷ�޶µ�4��
const int SCORE_BLOCKED_FOUR = 1000;//һ���޶µ�4��

const int SCORE_THREE = 1000;//��ͷ�޶µ�3��
const int SCORE_BLOCKED_THREE = 1000;//����һ�ߵ�3��
const int SCORE_TWO = 100;//��ͷ�޶µ�2��
const int SCORE_ONE = 10;

const double TIME_LIMIT = 1.5;
const int MAX_DEPTH = 9;

const int HASH_EXACT = 0;
const int HASH_ALPHA = 1;
const int HASH_BETA = 2;
const int UNKNOWN = -999999;

int myFlag;
int enemyFlag;

const int BOARD_MIDDLE_1 = 5;
const int BOARD_MIDDLE_2 = 6;

struct HashEntry {
    int score;        // ���̵�����
    int depth;        // �������
    int flag;         // ��ϣ���ͱ�־ (���� EXACT��ALPHA��BETA)
};

std::vector<std::vector<std::vector<uint64_t>>> zobristTable(BOARD_SIZE, std::vector<std::vector<uint64_t>>(BOARD_SIZE, std::vector<uint64_t>(3, 0)));
std::unordered_map<uint64_t, HashEntry> transpositionTable;

uint64_t currentHash = 0;

const std::unordered_map<int, int> ThreatMap = {
    //��ʤ���
    {22222, PRIORITY_THREAT},// XXXXX
    {12222,PRIORITY_THREAT},// _XXXX
    {22221, PRIORITY_THREAT},// XXXX_
    {22212, PRIORITY_THREAT},// XXX_X
    {21222,PRIORITY_THREAT},//  X_XXX
    {22122, PRIORITY_THREAT},// XX_XX

    // ����в��
    {12221, HIGH_THREAT},
    {12122, HIGH_THREAT},//_X_XX
    {12212, HIGH_THREAT},
    {21221,HIGH_THREAT},

    {33331,KILL_POS},//�ҷ�ɱ��
    {13333,KILL_POS},
    {33313,KILL_POS},
    {31333,KILL_POS},
    {33133,KILL_POS},

    {13331,20},

};

const std::unordered_map<int, int> ScoreMap = {
    // 16������
    {22222, SCORE_FIVE},// XXXXX
    {122221,SCORE_FOUR},// _XXXX_
    {122211, SCORE_THREE},// _XXX__
    {112221, SCORE_THREE},// __XXX_
    {122121,SCORE_THREE},//  _XX_X_
    {121221, SCORE_THREE},// _X_XX_
    {22221, SCORE_THREE},//XXXX_
    {12222,SCORE_THREE},//_XXXX
    {22122,SCORE_THREE},//XX_XX
    {21222,SCORE_THREE},//X_XXX
    {22212,SCORE_THREE},//XXX_X
    {112211,SCORE_TWO},//__XX__
    {112121,SCORE_TWO},//__X_X_
    {121211,SCORE_TWO},//_X_X__
    {111211,SCORE_ONE},//___X__
    {112111,SCORE_ONE},//__X___
};

// ��ʼ�� Zobrist ��ϣ��
void initializeZobristTable() {
    std::mt19937_64 rng(std::random_device{}());
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            zobristTable[row][col][EMPTY] = rng();
            zobristTable[row][col][BLACK] = rng();
            zobristTable[row][col][WHITE] = rng();
        }
    }
}

void updateHash(int row, int col, int piece) {
    currentHash ^= zobristTable[row][col][piece];
}

void storeHashEntry(uint64_t hashValue, int score, int flag) {
    transpositionTable[hashValue] = { score,flag };
}

int retrieveHashEntry(uint64_t hashValue, int alpha, int beta) {
    if (transpositionTable.find(hashValue) != transpositionTable.end()) {
        HashEntry entry = transpositionTable[hashValue];
        if (entry.flag == HASH_EXACT) {
            return entry.score;
        }
        else if (entry.flag == HASH_ALPHA && entry.score <= alpha) {
            return entry.score;
        }
        else if (entry.flag == HASH_BETA && entry.score >= beta) {
            return entry.score;
        }
    }
    return UNKNOWN;
}

struct Coordinate {
    int x;
    int y;
    int score;
    //Ĭ�Ϲ��캯��
    Coordinate() :x(-1), y(-1) {}
    // ���ι��캯��
    Coordinate(int row, int col) :x(row), y(col) {}
    Coordinate(int row, int col, int _score) :x(row), y(col), score(_score) {}
    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    }
    bool operator<(const Coordinate& other) const {
        if (score != other.score) {
            return score > other.score; // ��������������
        }
        else if (x != other.x) {
            return x < other.x;  // ��������ͬ���� x ������������
        }
        else {
            return y < other.y;  // �������� x ���궼��ͬ���� y ������������
        }
    }
};
struct ThreatInfo {
    Coordinate position; // ��вλ��
    int weight;          // Ȩ��
};

// �Զ����ϣ��������
struct CoordinateHash {
    std::size_t operator()(const Coordinate& coord) const {
        // ʹ����Ϲ�ϣ����
        std::size_t hx = std::hash<int>()(coord.x);
        std::size_t hy = std::hash<int>()(coord.y);
        return hx ^ (hy << 1); // λ���Ա����ϣ��ͻ
    }
};

class Board {
public:
    int board[BOARD_SIZE][BOARD_SIZE] = { {0} };//���̲���
    //int line_info[2][54] = { {0} };//��¼���Ӱ��������жԽ��ߵĸ������Խ���ֻ���ǳ��ȴ��ڵ���5��
    bool _checkpos[BOARD_SIZE][BOARD_SIZE] = { {0} };
    int currentBoardScore[2] = { 0 };//��¼��ǰ���̶�˫���ķ�������ʼ��Ϊ120����2������

    int rowIndex(int row) { return row; }
    int colIndex(int col) { return 12 + col; }
    int diagIndex(int row, int col) { return 31 - (row - col); }
    int antiDiagIndex(int row, int col) { return 39 + (row + col - 4); }
    //void updateInitialLineInfo(int row, int col, int player);
    //void undoUpdate(int row, int col, int player);
    void UpdateCheckPos(int x, int y);
    void UndoUpdateCheckPos(int x, int y);
    void UpdateBoardScore(int x, int y);//ÿ������ʱ�������̷���
    void undoUpdateScore(int x, int y);
    Board() {
        initializeZobristTable();
        MakeMove(BOARD_MIDDLE_1, BOARD_MIDDLE_2, BLACK);
        MakeMove(BOARD_MIDDLE_2, BOARD_MIDDLE_1, BLACK);
        MakeMove(BOARD_MIDDLE_1, BOARD_MIDDLE_1, WHITE);
        MakeMove(BOARD_MIDDLE_2, BOARD_MIDDLE_2, WHITE);

    }
    void MakeMove(int x, int y, int player)
    {
        board[x][y] = player;
        UpdateBoardScore(x, y);
        //updateInitialLineInfo(x, y, player);
        UpdateCheckPos(x, y);
        updateHash(x, y, player);
    }
    void UndoMove(int x, int y) {
        //undoUpdate(x, y, board[x][y]);
        undoUpdateScore(x, y);
        UndoUpdateCheckPos(x, y);
        updateHash(x, y, EMPTY);
    }
};
int evaluateMove(Board& A, int x, int y, int currentPlayer);
int CalculateLineScore(Board& A, int player, int index);

void Board::UpdateCheckPos(int x, int y) {
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int newX = x + i;
            int newY = y + j;
            if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE && board[newX][newY] == EMPTY) {
                _checkpos[newX][newY] = true;
            }
        }
    }
    _checkpos[x][y] = false;
}

void Board::UndoUpdateCheckPos(int x, int y) {
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int newX = x + i;
            int newY = y + j;
            if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE && _checkpos[newX][newY]) {
                bool hasAdjacentPiece = false;
                for (int m = -1; m < 2; m++) {
                    for (int n = -1; n < 2; n++) {
                        int adjX = newX + m;
                        int adjY = newY + n;
                        if (adjX >= 0 && adjX < BOARD_SIZE && adjY >= 0 && adjY < BOARD_SIZE && board[adjX][adjY] != EMPTY) {
                            hasAdjacentPiece = true;
                            break;
                        }
                    }
                    if (hasAdjacentPiece) {
                        break;
                    }
                }
                if (!hasAdjacentPiece) {
                    _checkpos[newX][newY] = false;
                }
            }
        }
    }
}

void Board::UpdateBoardScore(int x, int y) {
    int player = board[x][y];
    board[x][y] = EMPTY;
    currentBoardScore[0] -= evaluateMove(*this, x, y, BLACK);
    currentBoardScore[1] -= evaluateMove(*this, x, y, WHITE);//���ܷ������ȼ�ȥ��λ��ԭ�����еķ���
    board[x][y] = player;
    currentBoardScore[0] += evaluateMove(*this, x, y, BLACK);
    currentBoardScore[1] += evaluateMove(*this, x, y, WHITE);//���ܷ��ϸ����ĸ����ķ���
}
void Board::undoUpdateScore(int x, int y) {
    currentBoardScore[0] -= evaluateMove(*this, x, y, BLACK);
    currentBoardScore[1] -= evaluateMove(*this, x, y, WHITE);
    board[x][y] = EMPTY;
    currentBoardScore[0] += evaluateMove(*this, x, y, BLACK);
    currentBoardScore[1] += evaluateMove(*this, x, y, WHITE);//���ܷ��ϸ����ĸ����ķ���

}
//���ڸ�ĳ��λ�õġ�|\/�������֣����������ܷ֣�ÿ������ʱ�������̷���
int evaluateMove(Board& A, int x, int y, int currentPlayer) {
    int score = 0;

    score += CalculateLineScore(A, currentPlayer, A.rowIndex(x));
    score += CalculateLineScore(A, currentPlayer, A.colIndex(y));
    score += CalculateLineScore(A, currentPlayer, A.diagIndex(x, y));
    score += CalculateLineScore(A, currentPlayer, A.antiDiagIndex(x, y));
    return score;
}
static bool judgeInRange(Coordinate temp) { //�ж�λ���Ƿ񳬳�����
    if (temp.x < 0)return false;
    if (temp.y < 0)return false;
    if (temp.x > BOARD_SIZE)return false;
    if (temp.y > BOARD_SIZE)return false;
    return true;
}

void GetDir(Coordinate& temp, int& index, int& dRow, int& dCol) {
    if (index < 12) { // ��
        temp.x = index;
        temp.y = 0;
        dRow = 0;
        dCol = 1;
    }
    else if (index < 24) { // ��
        temp.x = 0;
        temp.y = index - 12;
        dRow = 1;
        dCol = 0;
    }
    else if (index < 39) { // ���Խ���
        int offset = index - 24 - 7;
        temp.x = std::max(0, -offset);
        temp.y = std::max(0, offset);
        dRow = 1;
        dCol = 1;
    }
    else { // ���Խ���
        int offset = index - 39 + 4;
        if (offset < BOARD_SIZE) {
            temp.x = std::max(0, -offset);
            temp.y = std::max(0, offset);
        }
        else {
            offset -= 11;
            temp.x = std::max(0, offset);
            temp.y = std::max(11, -offset);
        }
        dRow = 1;
        dCol = -1;
    }
}
int CalculateLineScore(Board& A, int player, int index) {
    int lineScore = 0;
    int opponent = (player == BLACK) ? WHITE : BLACK;

    // ���� index �ķ�Χȷ����������
    Coordinate temp;
    int dRow, dCol;
    GetDir(temp, index, dRow, dCol);
    std::vector<int> window5(5, 1);  // ����Ϊ5�Ĵ���
    std::vector<int> window6(6, 1);  // ����Ϊ6�Ĵ���

    window5[0] = 9; // ��Ե
    window6[0] = 9;

    // ��ʼ��ǰ5��6��λ�õĴ���
    for (int i = 1; i < 5 && judgeInRange(temp); ++i) {
        window5[i] = (A.board[temp.x][temp.y] == EMPTY) ? 1 :
            (A.board[temp.x][temp.y] == player) ? 2 : 3;
        window6[i] = window5[i];
        temp.x += dRow;
        temp.y += dCol;
    }

    if (judgeInRange(temp)) {
        window6[5] = (A.board[temp.x][temp.y] == EMPTY) ? 1 :
            (A.board[temp.x][temp.y] == player) ? 2 : 3;
        temp.x += dRow;
        temp.y += dCol;
    }
    Coordinate temp5(temp.x - 1, temp.y - 1);//
    // �������ڣ���һ�ƶ�λ�ã�ֱ�� window6[5] == 9
    while (window6[5] != 9 && window5[4] != 9) {
        int pattern5 = 0, pattern6 = 0;
        bool matched5 = false, matched6 = false;

        // ����������ת��Ϊ��������
        for (int i = 0; i < 5; ++i) {
            pattern5 = pattern5 * 10 + window5[i];
        }
        for (int i = 0; i < 6; ++i) {
            pattern6 = pattern6 * 10 + window6[i];
        }

        // ��鵱ǰ5��6�Ĵ����Ƿ������ScoreMap��
        if (ScoreMap.find(pattern5) != ScoreMap.end()) {
            lineScore += ScoreMap.at(pattern5);
            matched5 = true;
        }
        if (ScoreMap.find(pattern6) != ScoreMap.end()) {
            lineScore += ScoreMap.at(pattern6);
            matched6 = true;
        }

        // ���ƥ��ɹ����ֱ�������Ӧ�Ĵ��ڳ��ȼ������
        if (matched5) {
            for (int i = 0; i < 4; ++i) {
                if (i == 0 && window5[4] == 1) {
                    window5[0] = window5[4];
                    continue;
                }
                window5[i] = (A.board[temp5.x][temp5.y] == EMPTY) ? 1 :
                    (A.board[temp5.x][temp5.y] == player) ? 2 : 3;
                temp5.x += dRow;
                temp5.y += dCol;
            }
        }
        if (matched6) {
            for (int i = 0; i < 5; ++i) {
                if (i == 0 && window6[5] == 1) {
                    window6[0] = window6[5];
                    continue;
                }
                window6[i] = (A.board[temp.x][temp.y] == EMPTY) ? 1 :
                    (A.board[temp.x][temp.y] == player) ? 2 : 3;
                temp.x += dRow;
                temp.y += dCol;
            }
        }
        if (matched5 || matched6) {
            continue;
        }

        // �ƶ����ڣ��Ƴ��ɵģ�����µ�
        for (int i = 0; i < 4; ++i) {
            window5[i] = window5[i + 1];
            window6[i] = window6[i + 1];
        }
        window6[4] = window6[5];
        window5[4] = window6[5];

        if (judgeInRange(temp)) {
            window6[5] = (A.board[temp.x][temp.y] == EMPTY) ? 1 :
                (A.board[temp.x][temp.y] == player) ? 2 : 3;
        }
        else {
            window6[5] = 9; // ������
        }

        // �ƶ�����һ��λ��
        temp.x += dRow;
        temp.y += dCol;
    }

    return lineScore;
}
int evaluateBoard(Board& A, int player) {
    if (player == BLACK) {
        return A.currentBoardScore[0];
    }
    else
    {
        return A.currentBoardScore[1];
    }
}

std::vector<Coordinate> generateAndSortMoves(Board& A, int player) {
    int opponent = (player == BLACK) ? WHITE : BLACK;
    // ʹ����С��������ǰ10����߷ֵ�����
    auto cmp = [](const Coordinate& a, const Coordinate& b) {
        return a.score > b.score;
        };
    std::priority_queue<Coordinate, std::vector<Coordinate>, decltype(cmp)> topMoves(cmp);

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (A._checkpos[row][col]) {
                int moveScore = evaluateMove(A, row, col, player);
                moveScore += evaluateMove(A, row, col, opponent);//��λ�÷�������
                Coordinate move(row, col, moveScore);

                if (topMoves.size() < 10) {
                    topMoves.push(move);
                }
                else if (move.score > topMoves.top().score) {
                    topMoves.pop();
                    topMoves.push(move);
                }
            }
        }
    }

    // �����е�Ԫ��ת��Ϊ����������������
    std::vector<Coordinate> moves;
    while (!topMoves.empty()) {
        moves.push_back(topMoves.top());
        topMoves.pop();
    }
    std::sort(moves.begin(), moves.end(), [](const Coordinate& a, const Coordinate& b) {
        return a.score > b.score;
        });

    return moves;
}

//�ж�currentmove�Ƿ��������в��λ
bool detectThreats(int player, Board& A, Coordinate& CurrentMove, std::vector<Coordinate>& threatPositions) {
    threatPositions.clear();
    // ���� index �ķ�Χȷ����������
    Coordinate temp;
    std::unordered_map<Coordinate, int, CoordinateHash> threatMap;  // �洢��в��Ϣ��Ȩ��
    int index[4];
    index[0] = A.rowIndex(CurrentMove.x);
    index[1] = A.colIndex(CurrentMove.y);
    index[2] = A.diagIndex(CurrentMove.x, CurrentMove.y);
    index[3] = A.antiDiagIndex(CurrentMove.x, CurrentMove.y);
    int dRow, dCol;
    for (int i = 0; i < 4; i++) {
        GetDir(temp, index[i], dRow, dCol);
        int binaryPattern = 0;
        int threatLevel = 0;

        std::vector<int> window(5, 0); // ��ʼ������

        // ��ʼ�����ڵ�ǰ5��λ��
        for (int k = 0; k < 5 && judgeInRange(temp); ++k) {
            window[k] = (A.board[temp.x][temp.y] == EMPTY) ? 1 : (A.board[temp.x][temp.y] == player) ? 2 : 3;
            temp.x += dRow;
            temp.y += dCol;
        }

        // �������ڱ���������/��/�Խ���
        while (judgeInRange(temp)) {
            // ����ǰ����ת��Ϊ10�������ͱ���
            int pattern = 0;
            for (int k = 0; k < 5; ++k) {
                pattern = pattern * 10 + window[k];
            }

            // ���������Ƿ��������в����
            if (ThreatMap.find(pattern) != ThreatMap.end()) {
                int threatValue = ThreatMap.at(pattern);

                Coordinate threatPosition1, threatPosition2;
                if (pattern == 13331 || pattern == 12221 || pattern == 21221 || pattern == 31331 || pattern == 12122 || pattern == 13133 || pattern == 12212 || pattern == 13313) { // ���� `21221` or `_XXX_`
                    if (pattern == 13331 || pattern == 12221) {
                        threatPosition1 = temp;   // ��һ����λ
                        threatPosition1.x -= 5 * dRow;
                        threatPosition1.y -= 5 * dCol;

                        threatPosition2 = temp;   // �ڶ�����λ
                        threatPosition2.x -= dRow;
                        threatPosition2.y -= dCol;
                    }
                    else if (pattern == 21221 || pattern == 31331) {
                        threatPosition1 = temp;   // ��һ����λ
                        threatPosition1.x -= 4 * dRow;
                        threatPosition1.y -= 4 * dCol;

                        threatPosition2 = temp;   // �ڶ�����λ
                        threatPosition2.x -= dRow;
                        threatPosition2.y -= dCol;
                    }
                    else if (pattern == 12122 || pattern == 13133) {
                        threatPosition1 = temp;
                        threatPosition1.x -= 3 * dRow;
                        threatPosition1.y -= 3 * dCol;

                        threatPosition2 = temp;   // �ڶ�����λ
                        threatPosition2.x -= 5 * dRow;
                        threatPosition2.y -= 5 * dCol;
                    }
                    else {// pattern==12212||pattern==13313
                        threatPosition1 = temp;   // ��һ����λ
                        threatPosition1.x -= 5 * dRow;
                        threatPosition1.y -= 5 * dCol;

                        threatPosition2 = temp;   // �ڶ�����λ
                        threatPosition2.x -= 2 * dRow;
                        threatPosition2.y -= 2 * dCol;
                    }
                    if (evaluateMove(A, threatPosition1.x, threatPosition1.y, myFlag) > evaluateMove(A, threatPosition2.x, threatPosition2.y, myFlag))
                    {
                        // �ۼ���вֵ
                        if (threatMap.find(threatPosition1) != threatMap.end()) {
                            threatMap[threatPosition1] += threatValue;
                        }
                        else {
                            threatMap[threatPosition1] = threatValue;
                        }
                    }
                    else
                    {
                        if (threatMap.find(threatPosition2) != threatMap.end()) {
                            threatMap[threatPosition2] += threatValue;
                        }
                        else {
                            threatMap[threatPosition2] = threatValue;
                        }
                    }
                }
                // ����������λ����вλ��

                else {
                    Coordinate threatPosition = temp;
                    if (pattern == 12222 || pattern == 13333) {
                        threatPosition.x -= 5 * dRow;
                        threatPosition.y -= 5 * dCol;
                    }
                    else if (pattern == 22221 || pattern == 33331) {
                        threatPosition.x -= dRow;
                        threatPosition.y -= dCol;
                    }
                    else if (pattern == 22212 || pattern == 33313) {
                        threatPosition.x -= 2 * dRow;
                        threatPosition.y -= 2 * dCol;
                    }
                    else if (pattern == 22122 || pattern == 33133) {
                        threatPosition.x -= 3 * dRow;
                        threatPosition.y -= 3 * dCol;
                    }
                    else if (pattern == 21222 || pattern == 31333) {
                        threatPosition.x -= 4 * dRow;
                        threatPosition.y -= 4 * dCol;
                    }

                    // �ۼ���вֵ
                    if (threatMap.find(threatPosition) != threatMap.end()) {
                        threatMap[threatPosition] += threatValue;
                    }
                    else {
                        threatMap[threatPosition] = threatValue;
                    }
                }
            }


            // �������ڣ��Ƴ��ɵģ�����µ�
            for (int k = 0; k < 4; ++k) {
                window[k] = window[k + 1];
            }
            window[4] = (A.board[temp.x][temp.y] == EMPTY) ? 1 : (A.board[temp.x][temp.y] == player) ? 2 : 3;
            temp.x += dRow;
            temp.y += dCol;
        }

    }

    // ����вλ�ð�Ȩ������
    std::vector<std::pair<Coordinate, int>> sortedThreats(threatMap.begin(), threatMap.end());
    std::sort(sortedThreats.begin(), sortedThreats.end(), [](const std::pair<Coordinate, int>& a, const std::pair<Coordinate, int>& b) {
        return a.second > b.second;  // ������вȨ�ؽ�������
        });

    // �������в������λ�÷���
    for (const auto& threat : sortedThreats) {
        if (judgeInRange(threat.first) && A.board[threat.first.x][threat.first.y] == EMPTY) {
            threatPositions.push_back(threat.first);
        }
    }

    return !threatPositions.empty();  // û����вʱ���� false
}

int AlphaBeta(Board& A, int depth, int alpha, int beta, int player, Coordinate& bestMove) {
    if (depth == 5);
    uint64_t hashValue = currentHash; // ���� currentHash �ǵ�ǰ����״̬�Ĺ�ϣֵ
    std::vector<Coordinate> moveline;
    if (depth != MAX_DEPTH) {
        int cachedScore = retrieveHashEntry(hashValue, alpha, beta);
        if (cachedScore != UNKNOWN) {
            return cachedScore;
        }
    }
    int score1 = evaluateBoard(A, myFlag);
    int score2 = evaluateBoard(A, enemyFlag);

    if (score1 >= SCORE_FIVE) {
        return MAX_SCORE - 1;
    }
    if (score2 >= SCORE_FIVE) {
        return MIN_SCORE + 1;
    }
    //����һ��ȡʤֱ�ӷ��أ��൱�ڵ���Ҷ�ڵ㣩
    if (depth == 0) {
        int finalScore = score1 - score2;
        storeHashEntry(hashValue, finalScore, HASH_EXACT);
        return finalScore;  // ����ݹ����ʱ��������ֵ
    }
    int count = 0;
    int flag = UNKNOWN;
    std::vector<Coordinate> possiblePositions = generateAndSortMoves(A, player);
    if (player == myFlag) {  // Maximizing player (AI)
        int value = _INF;
        int count = 0;
        for (const Coordinate& pos : possiblePositions) {

            A.MakeMove(pos.x, pos.y, player);

            value = std::max(value, AlphaBeta(A, depth - 1, alpha, beta, enemyFlag, bestMove));
            if (value > alpha) {
                alpha = value;
                flag = HASH_EXACT;
                if (depth == MAX_DEPTH)
                {
                    bestMove = pos;
                }
            }
            /*if (depth == MAX_DEPTH) {
                std::cout << "(" << pos.x << "," << pos.y << ")" << "score:" << value << std::endl;
            }*/
            A.UndoMove(pos.x, pos.y);
            count++;
            if (count >= 5) {
                break;
            }
            if (value >= beta) {  // Beta ��֦
                flag = HASH_BETA;
                break;
            }
        }
        storeHashEntry(hashValue, value, flag);
        return value;
    }
    else {  // Minimizing player (Opponent)
        int value = INF;
        int count = 0;
        for (const Coordinate& pos : possiblePositions) {
            A.MakeMove(pos.x, pos.y, player);

            value = std::min(value, AlphaBeta(A, depth - 1, alpha, beta, myFlag, bestMove));

            if (value < beta) {
                flag = HASH_EXACT;
                beta = value;
            }

            A.UndoMove(pos.x, pos.y);
            count++;
            if (count >= 5) {
                break;
            }
            if (value <= alpha) {  // Alpha ��֦
                flag = HASH_ALPHA;
                break;
            }
        }
        storeHashEntry(hashValue, value, flag);
        return value;
    }
}

void loop()
{
    Board A;
    char tag[10] = { 0 };
    int status;
    Coordinate command;
    Coordinate BestMove;
    std::vector<Coordinate> threatPositions;
    while (1) {
        memset(tag, 0, sizeof(tag));
        scanf("%s", tag);
        if (strcmp(tag, START) == 0) {
            scanf("%d", &myFlag);
            enemyFlag = 3 - myFlag;
            printf("OK\n");
            fflush(stdout);
        }
        else if (strcmp(tag, PLACE) == 0) {
            scanf("%d %d", &command.x, &command.y);
            A.MakeMove(command.x, command.y, enemyFlag);
        }
        else if (strcmp(tag, TURN) == 0) {
            if (judgeInRange(command) && detectThreats(enemyFlag, A, command, threatPositions)) {//�ж϶Է������Ƿ���γ���в���
                BestMove = threatPositions.front();
            }
            else {
                AlphaBeta(A, MAX_DEPTH, _INF, INF, myFlag, BestMove);
            }
            
            if (!judgeInRange(BestMove)) {
                for (int i = 0; i < BOARD_SIZE; i++) {
                    for (int j = 0; j < BOARD_SIZE; j++) {
                        if (A._checkpos[i][j]) {
                            BestMove = Coordinate(i, j);
                        }
                    }
                }
            }
            A.MakeMove(BestMove.x, BestMove.y, myFlag);
            printf("%d %d\n", BestMove.x, BestMove.y);
            fflush(stdout);
            std::cout << "black:" << A.currentBoardScore[0] << "white:" << A.currentBoardScore[1] << std::endl;
        }

        else if (strcmp(tag, END) == 0) {
            scanf("%d", &status);
            exit(0);
        }

    }
}
int main()
{
    loop();
    return 0;
}